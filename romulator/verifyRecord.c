#include "emulator.h"
//-----------------------------------------------------------------------------
//	verifyRecord.c
//	This subprogram is a component of the Romulator software.
//	It verifies that the given file consists of valid SRecords or Intel Hexes.
//	It performs 4 tests:
// 		Test 1: Does the file exist?
// 		Test 2: Does each record start with the correct symbol?
// 		Test 3: Is each record the correct length?
// 		Test 4: Is the checksum correct?
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Method:			verifyRecord
//	Description:	The main verification method.
//	Parameters:		A string representing a file name.
//					A pointer to a 3-element integer array containing the
//						status of the record.
//-----------------------------------------------------------------------------
void verifyRecord(char filename[], int* returnStatus) {
	FILE* record = fopen(filename, "r");
	int charCount = 0;
	int recordCount = 0;
	int recordType = NOT_YET_DEFINED;
	int status = VALID_RECORD;
	char testchar, temp;
	//-----------------------------
	// Test 1: Does the file exist?
	//-----------------------------
	if (record == NULL) {
		//	If the file stream points to NULL, then the file does not exist,
		//		so it fails the first test.
		fclose(record);
		returnStatus[0] = 0;
		returnStatus[1] = 0;
		returnStatus[2] = INVALID_FILENAME;
		return;
	}
	//	The file passed the existence test, so move on to test the records
	//		themselves.
	while ((testchar = getc(record)) != EOF) {
		charCount++;
		//--------------------------------------------------------
		// Test 2: Does each record start with the correct symbol?
		//--------------------------------------------------------
		// S for SRecord:
		if (testchar == 's' || testchar == 'S') {
			if (recordType == NOT_YET_DEFINED)
				recordType = S_RECORD;		// Establish the file as SRecords.
			if (recordType == S_RECORD) {
				//	The present record passed the record test type. So, call
				//		verifySRecord to see if it passes the remaining 2
				//		tests.
				status = verifySRecord(record);
			}
			else {
				//	In this case, the file has been established as an SRecord,
				//		but a record starts with a character other than 'S', so
				//		it has failed the test.
				fclose(record);
				returnStatus[0] = 0;
				returnStatus[1] = 0;
				returnStatus[2] = INVALID_FORMAT;
				return;
			}
		}
		
		// : for Intel Hex:
		else if (testchar == ':') {
			if (recordType == NOT_YET_DEFINED)
				recordType = INTEL_HEX;		// Establish file as Intel Hex.
			if (recordType == INTEL_HEX) {
				//	The present record passed the record test type. So, call
				//		verifyIntelHex to see if it passes the remaining 2
				//		tests.
				status = verifyIntelHex(record);
			}
			else {
				//	In this case, the file has been established as an SRecord,
				//		but a record starts with a character other than 'S', so
				//		it has failed the test.
				fclose(record);
				returnStatus[0] = 0;
				returnStatus[1] = 1;
				returnStatus[2] = INVALID_FORMAT;
				return;
			}
		}
		
		//	If a record starts with any other symbol, it's invalid, so return
		//		the error:
		else {
			fclose(record);
			returnStatus[0] = 0;
			returnStatus[1] = 1;
			returnStatus[2] = INVALID_FORMAT;
			return;
		}
		
		//	Analyze the status returned by the current record being scanned. If
		//		the record follows a correct format (for length and checksum),
		//		status will equal the character count that was analyzed for
		//		correctness. Otherwise, an error status will be returned.
		//	This needs to be altered for use with the new return type.
		if (status < 0) {
			fclose(record);
			returnStatus[0] = 0;
			returnStatus[1] = 1;
			returnStatus[2] = status;
			return;
		}
		// Get the total cumulative character count.
		charCount += status;
		recordCount++;
	}
	returnStatus[0] = charCount;
	returnStatus[1] = recordCount;
	returnStatus[2] = recordType;
	fclose(record);
	return;
}



//-----------------------------------------------------------------------------
//	Method:			verifySRecord
//	Description:	This is the method that validates each SRecord.
//	Parameter:		Pointer to the file stream used in verifyRecord.
//	Return:			An integer representing either an error status number or
//						the count of characters analyzed.
//-----------------------------------------------------------------------------
int verifySRecord(FILE* record) {
	// Setup.
	// Valid SRecord: Stnnaaaadddd...ddcc\n
	//		S:	Establishes record as an SRecord.
	//		t:	Record type. 0-9.
	//		n:	Number of bytes that follow (2 hex digits per byte) (count).
	//		a:	Starting address for the record (address)
	//		d:	The data (data).
	//		c:	Checksum (1s complement of the sum of bytes of address, count,
	//				and data.	
	int counter = 0;
	// Get the SRecord type.
	char testRecordType = getc(record);
	counter++;
	// Get the count byte.
	char count[3];			// The count hex digits as a string.
	int countNum = 0;		// The count hex digits.
	int recordSize = 0;		// Number of characters that follow.
	count[0] = getc(record);
	count[1] = getc(record);
	count[2] = '\0';
	counter += 2;
	countNum = (int)strtol(count, (char**)NULL, 16);
	recordSize = countNum * 0x2;
	
	//-------------------------------------------
	// Test 3: Is each record the correct length?
	//-------------------------------------------
	//	contents is a string consisting of all the rest of the hex digits.
	//		It's kind of like establishing a string stream.
	//	recordSize + 1 is the number of remaining characters plus '\0'.
	char contents[recordSize + 1];
	for (int i = 0; i < recordSize; i++) {
		//	Get a character from the file stream. If it's '\n' then the record
		//		is shorter than the indicated count and is thus invalid.
		if ((contents[i] = getc(record)) == '\n')
			return INVALID_LENGTH;
		counter++;
	}
	//	Concatenate the '\0' at the end of the contents array, making it a C-
	//		style string.
	contents[recordSize] = '\0';
	//	After getting the count size of characters from the record, the next
	//		character should be '\n'. If this is not the case, then the record
	//		is too long.
	if (getc(record) != '\n')
		return INVALID_LENGTH;
	counter++;

	//---------------------------------
	// Test 4: Is the checksum correct?
	//---------------------------------
	int sum = 0x0;
	//	Take the sum of each byte. Each byte consists of two hex digits.
	for (int i = 0; i < recordSize-2; i += 2) {
		char numbers[3];
		numbers[0] = contents[i];
		numbers[1] = contents[i+1];
		numbers[2] = '\0';
		sum += (int)strtol(numbers, (char**)NULL, 16);
	}
	// Add countNum to sum (the hex value of count, already extracted).
	sum += countNum;
	// Take the 1's complement of the sum:
	sum = ~sum;
	//	Crafty trick to get a number equal to sum, minus the LSB. The LSB will
	//		equal 0.
	int shifted = sum >> 8;
	shifted <<= 8;
	//	check is equal to sum - shifted: the isolated LSB of sum.
	int check = sum - shifted;
	//	Extract the checksum from the contents string.
	char cksmstring[3];
	cksmstring[0] = contents[recordSize - 2];
	cksmstring[1] = contents[recordSize - 1];
	cksmstring[2] = '\0';
	int checksum = (int)strtol(cksmstring, (char**)NULL, 16);
	//	Compare check with checksum. If they're not equal, then the record is
	//		invalid.
	if (check != checksum)
		return INVALID_VALUES;
	//	If the record is valid, then return the value of the running counter.
	return counter;
}



//-----------------------------------------------------------------------------
//	Method:			verifyIntelHex
//	Description:	This is the method that validates each hex record.
//	Parameter:		Pointer to the file stream used in verifyRecord.
//	Return:			An integer representing either an error status number or
//						the count of characters analyzed.
//-----------------------------------------------------------------------------
int verifyIntelHex(FILE* record) {
	// Setup
	// Valid Intel Hex: :nnaaaattdd...ddcc\n
	//		n:	Number of bytes that follow (2 hex digits per byte) (count).
	//		a:	Starting address for the record (address)
	//		t:	The hex record type. (type)
	//		d:	The data (data).
	//		c:	Checksum (2s complement of the sum of bytes of non-checksum
	//				values).
	int counter = 0;
	int totalCharacters = 0;
	char count[3];			// The count hex digits as a string.
	int countNum = 0;		// The count hex digits.
	int recordSize = 0;		// Number of characters that follow.
	count[0] = getc(record);
	count[1] = getc(record);
	count[2] = '\0';
	counter += 2;
	countNum = (int)strtol(count, (char**)NULL, 16);
	recordSize = countNum * 0x2;
	totalCharacters = recordSize/*data*/ + 4/*address*/ + 2/*type*/ + 2/*checksum*/;
	
	//-------------------------------------------
	// Test 3: Is each record the correct length?
	//-------------------------------------------
	char contents[totalCharacters + 1]; // number of remaining characters plus \0.
	for (int i = 0; i < totalCharacters; i++) {
		if ((contents[i] = getc(record)) == '\n')
			return INVALID_LENGTH;
		counter++;
	}
	contents[totalCharacters] = '\0';
	if (getc(record) != '\n')
		return INVALID_LENGTH;
	counter++;

	//---------------------------------
	// Test 4: Is the checksum correct?
	//---------------------------------
	int sum = 0x0;
	for (int i = 0; i < totalCharacters-2; i += 2) {
		char numbers[3];
		numbers[0] = contents[i];
		numbers[1] = contents[i+1];
		numbers[2] = '\0';
		sum += (int)strtol(numbers, (char**)NULL, 16);
	}
	sum += countNum;
	sum = ~sum + 1;
	int shifted = sum >> 8;
	shifted <<= 8;
	// shifted should equal xx...x00.
	int check = sum - shifted;
	char cksmstring[3];
	cksmstring[0] = contents[totalCharacters - 2];
	cksmstring[1] = contents[totalCharacters - 1];
	cksmstring[2] = '\0';
	int checksum = (int)strtol(cksmstring, (char**)NULL, 16);
	if (check != checksum)
		return INVALID_VALUES;
	return counter;
}
