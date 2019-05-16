#include "emulator.h"

//-----------------------------------------------------------------------------
//  Method:         main
//  Description:    The main method. Establishes serial communication with the
//						connected Romulator and directs traffic.
//	Parameters:		0, 1, or 2 additional command line arguments. Additional
//						command line (CL) arguments are:
//						argv[1]:	command (read, write, emulate, or help).
//						argv[2]:	file name to send from or receive to.
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	// Some variable declarations:
    char filename[BUFFER_SIZE] = "";			// The filename of the record.
	char command[BUFFER_SIZE] = "";				// Command to romulator.
	HANDLE* hComm;								// Serial file handle.
	DCB dcbSerialParamInit = {0};				// Serial Parameters
	DCB* dcbSerialParams = &dcbSerialParamInit;	// Serial Parameters
	COMMTIMEOUTS timeoutInit = {0};				// Serial Timeouts
	COMMTIMEOUTS* timeouts = &timeoutInit;		// Serial Timeouts
	char portName[7];							// Serial com port number
	int startStatus = 0;
	int canStartSerial = 0;	
	// Don't start serial communications if all the user wants to do is load
	//		the help screen.
	if (argc == 2) {
		if (	!strcmp(argv[1], "help") ||
				!strcmp(argv[1], "-h") ||
				!strcmp(argv[1], "--help") ) 
			canStartSerial = 0;
		else
			canStartSerial = 1;
	}
	else
		canStartSerial = 1;
	
	// Open up a line of communication with the Romulator.
	if (canStartSerial == 1) {
		puts("=======================================");
		puts("Romulator");
		puts("Copyright 2019 by Team Romulator");
		puts("Department of Science and Engineering");
		puts("University of Washington, Bothell");
		puts("=======================================");
		// Scan com ports for the Arduino.
		// getPortNumber defined in serialCommunications.c.
		if (getPortNumber(portName) < 0) {
			puts("The Romulator is not attached to the PC.");
			return 0;
		}
		else
			printf("Romulator attached to %s.\n", portName);
		// Establish serial communication with the Arduino.
		// startSerial defined in serialCommunications.c.
		if (startSerial(SERIAL_PARAMS) == SERIAL_ERROR) {
			puts("Failed to establish a stable serial connection.");
			CloseHandle(hComm);
			return 0;
		}
	}

	// Check for command line arguments. They follow the format presented in
	//		this table:
	//	argc	argv
	//--------------------------------------------------
	//	1		[0]romulator
	//	2		[0]romulator [1]<command>
	//	3		[0]romulator [1]<command> [2]<filename>
	if (argc >= 2) {		//	argc >= 2 means there are arguments passed in
							//		from the command line.
		if (argc > 3) {		//	There can't be more than 3 command line
							//		arguments
			puts ("Too many arguments. Type \"romulator help\" or \"romulator"
				" -h\" for help.");
			startStatus = INVALID_ARGUMENTS;
		}
		else {				//	There are 2 or 3 arguments.
			for (int i = 0; i < strlen(argv[1]); i++)
				argv[1][i] = tolower(argv[1][i]);
			// 	Copy the second argument to the string command, which is easier
			//		to work with than argv[1].
			strcpy(command, argv[1]);
			// 	Direct to help, regardless of whether there are 2 or 3
			//		arguments.
			if (	!strcmp(command, "help") ||
					!strcmp(command, "-h") ||
					!strcmp(command, "--help") ) {
				romulatorHelp();
			}
			// Direct to emulate, but only if there are 2 CL arguments.
			else if (	!strcmp(command, "-e") || 
						!strcmp(command, "emulate") || 
						!strcmp(command, "--emulate") ) {
				if (argc == 2)
					emulate();
				else {
					puts("Too many arguments. Format is romulator -e.");
					startStatus = INVALID_ARGUMENTS;
				}
			}
			// Direct to read, but only if there are 3 CL arguments.
			else if ( 	!strcmp(command, "-r") || 
						!strcmp(command, "read") ||
						!strcmp(command, "readdata") || 
						!strcmp(command, "--read") ||
						!strcmp(command, "--readdata") ) {
				if (argc == 3) {
					strcpy(filename, argv[2]);
					readData(filename);
				}
				else {
					puts("Too few arguments. The correct format is romulator"
						" -r <filename>.");
					startStatus = INVALID_ARGUMENTS;
				}
			}
			// Direct to write, but only if there are 3 CL arguments.
			else if (	!strcmp(command, "-w") || 
						!strcmp(command, "write") ||
						!strcmp(command, "writedata") ||
						!strcmp(command, "--write") ||
						!strcmp(command, "--writedata") ) {
				if (argc == 3) {
					strcpy(filename, argv[2]);
					writeData(filename, SERIAL_PARAMS);
				}
				else {
					puts("Too few arguments. The correct format is romulator"
						" -w <filename>.");
					startStatus = INVALID_ARGUMENTS;
				}
			}
			// If any other command is issued, it's invalid.
			else {
				puts("Invalid command. Type \"romulator help\" or \"romulator"
					" -h\" for help.");
				startStatus = INVALID_ARGUMENTS;
			}
		}
	}// End if there are 2 or more CL arguments.
	//	If no CL arguments are entered, then direct the user to the main menu.
	else {
		mainMenu(SERIAL_PARAMS);
	}
	// Close the port and end the program.
	puts("End of program. Closing serial connection.");
	CloseHandle(hComm);
	return startStatus;
}


//-----------------------------------------------------------------------------
//  Method:         mainMenu
//  Description:    The main menu used when there are no command line
//                      arguments or if arguments are erroneous.
//	Parameters:		The common serial parameters, listed in emulator.h:
//-----------------------------------------------------------------------------
void mainMenu(SERIAL_PARAMETERS) {
	puts("\nMain menu:");
	puts("----------");
    int complete = EXIT_FAILURE;
    char filename[BUFFER_SIZE] = "";
    unsigned int mode = 0;
    do {
        int sel = 0;
        printf("Select a mode:\n"
            "\t1.\tRead data from emulator's RAM, saving it to the PC.\n"
            "\t2.\tWrite program on the PC to the emulator's RAM.\n"
            "\t3.\tEmulate ROM on target system.\n"
            "\t0.\tExit.\n\n");
		putchar('>');
        scanf("%d", &sel);
        while ((getchar()) != '\n');
        switch (sel) {
        case 1:
            printf("Please give the file you want to save to a name. ");
            gets(filename);
            complete = readData(filename);
            break;
        case 2:
            printf("Type a file name to load. ");
            gets(filename);
            complete = writeData(filename, SERIAL_PARAMS);
            break;
        case 3:
            complete = emulate();
            break;        case 0:
            return;
        default:
            printf("Invalid selection. Please try again. ");
        }
    } while (complete == EXIT_FAILURE);
}

//-------------------------------------------------------------------
//  Method:         readData
//  Description:    Directs to program flow for reading data from
//                      the ROM emulator's RAM.
//-------------------------------------------------------------------
int readData(char filename[]) {
    printf("This method will read the contents of the RAM and save the data to the file\n"
        "named %s.\n", filename);
    return EXIT_SUCCESS;
}

//-------------------------------------------------------------------
//  Method:         writeData
//  Description:    Directs to program flow for writing data to the
//                      ROM emulator's RAM.
//-------------------------------------------------------------------
int writeData(char filename[], SERIAL_PARAMETERS) {
	//	status is a 3-digit array consisting of:
	//		status[0] is the character count of the record file.
	//		status[1] is the record count of the record file.
	//		status[2] is the type of the record file.
	int* status = (int*)malloc(sizeof(int) * 3);
	FILE* fileStream;
	char currentRecord[MAX_RECORD_SIZE];
	int charCount = 0;
	char tempCharacter = ' ';
	int isValidFile = 0;
	//	verifyRecord sets status properties while determing if it's
	//		a valid record. It will also automatically distinguish
	//		between an SRecord and an Intel Hex. It is located in
	//		verifyRecord.c.
	verifyRecord(filename, status);
	switch (*(status + 2)) {
		case INVALID_FILENAME:
			puts("No file with that name exists. Try again.");
			isValidFile = INVALID_FILENAME;
			break;
		case INVALID_FORMAT:
			puts("The file is an invalid format. Try again.");
			isValidFile = INVALID_FORMAT;
			break;
		case INVALID_LENGTH:
			puts("A record in the file has an incorrect length. Try again.");
			isValidFile = INVALID_LENGTH;
			break;
		case INVALID_VALUES:
			puts("A record in the file failed the checksum test. Try again.");
			isValidFile = INVALID_VALUES;
			break;
		default:
			puts("Valid record. Preparing to send.");
			fileStream = fopen(filename, "r");
			for (int i = 0; i < *(status + 1); i++) {
				int j = 0;
				while((tempCharacter = getc(fileStream)) != '\n') {
					currentRecord[j++] = tempCharacter;
				}
				// Append the null character to the end of the string.
				currentRecord[j] = '\0';
				// Create a truncated string to send over as little data as possible.
				char* currentRecordTruncated = (char*)malloc(sizeof(char) * (strlen(currentRecord) + 1));
				currentRecordTruncated = strcpy(currentRecordTruncated, currentRecord);
				printf("Sending record %d...\n", i+1);
				printf("%s\n",currentRecordTruncated);
				serialTransmit(currentRecordTruncated, SERIAL_PARAMS);
				//char acknowledgement[strlen(currentRecord) + 1];
				//serialReceive(acknowledgement, SERIAL_PARAMS);
				free(currentRecordTruncated);
				Sleep(10);
				// Echo back to see if it received it.
				char* receiveBuffer = (char*)malloc(sizeof(char) * (strlen(currentRecord) + 1));
				printf("Test: receiveBuffer has %d slots allocated.\n", strlen(currentRecord) + 1);
				serialReceive(receiveBuffer, SERIAL_PARAMS);
				puts("The Arduino says:");
				puts(receiveBuffer);
				free(receiveBuffer);
				// End echo back

				
				Sleep(3000);
			}
			puts("Done.");
			fclose(fileStream);
			return charCount;
	}
	//free(currentRecordTruncated);
	return isValidFile;
}

//-------------------------------------------------------------------
//  Method:         emulate
//  Description:    Directs to program flow for emulating ROM.
//-------------------------------------------------------------------
int emulate() {
    printf("This method will allow access to the RAM by the system under test. It will\n"
        "cause our RAM to behave just like ROM to the target system.\n");
    return EXIT_SUCCESS;
}

void romulatorHelp() {
	FILE* helpFile = fopen("help.txt", "r");
	int character;
	while((character = getc(helpFile)) != EOF)
		putchar(character);
}