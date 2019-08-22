#include "emulator.h"
//-----------------------------------------------------------------------------
//	Method:			getPortNumber
//	Description:	Finds the port com number that the Romulator (specifically,
//						the Arduino controlling the Romulator) is attached
//						to.
//	Parameter:		inputString is the friendly name to attempt to match.
//	Return:			A status: not found or found.
//	Side Effect:	Modifies the inputString.
//-----------------------------------------------------------------------------
int getPortNumber(char* inputString) {
	int foundIt = -1; 	
	SP_DEVINFO_DATA devInfoData = {};
	devInfoData.cbSize = sizeof(devInfoData);
	
	// Get port info tree:
	HDEVINFO hDeviceInfo = SetupDiGetClassDevs(	NULL,
												NULL,
												NULL,
												DIGCF_ALLCLASSES | DIGCF_PRESENT
												);
	if (hDeviceInfo == INVALID_HANDLE_VALUE)
		return DEVICE_NOT_FOUND;
	
	// Iterate over devices
	int nDevice = 0;
	while(SetupDiEnumDeviceInfo(	hDeviceInfo,
									nDevice++,
									&devInfoData)
			&& foundIt < 0) {
		DWORD regDataType;
		DWORD reqSize = 0;
		
		// Find size required to hold device info.
		SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
											&devInfoData,
											SPDRP_HARDWAREID,
											NULL,
											NULL,
											0,
											&reqSize);
		BYTE* hardwareID = (BYTE*)malloc((reqSize > 1) ? sizeof(BYTE) * reqSize : 1);
		// Store in buffer
		if (SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
												&devInfoData,
												SPDRP_HARDWAREID,
												&regDataType,
												hardwareID,
												sizeof(hardwareID) * reqSize,
												NULL)) {
			// Find size needed to hold friendly name.
			reqSize = 0;
			SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
												&devInfoData,
												SPDRP_FRIENDLYNAME,
												NULL,
												NULL,
												0,
												&reqSize);
			BYTE* friendlyName = (BYTE*)malloc((reqSize > 1) ? sizeof(BYTE) * reqSize : 1);
			// Store in buffer:
			if (!SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
													&devInfoData,
													SPDRP_FRIENDLYNAME,
													NULL,
													friendlyName,
													sizeof(friendlyName) * reqSize,
													NULL)) {
				// Device does not have this property set:
				memset(friendlyName, 0, (reqSize > 1) ? reqSize : 1);
			}
			//-----------------------------------------------------------------------
			//	Use Friendly Name
			//-----------------------------------------------------------------------
			if (strstr(friendlyName, DEVICE_NAME) != NULL) {
				inputString = strncpy(inputString, friendlyName + NAME_LENGTH, END_OF_NAME);
				inputString[END_OF_NAME] = '\0';
				foundIt = 1;
			}
			//-----------------------------------------------------------------------
			//	End use friendly name
			//-----------------------------------------------------------------------
			free(friendlyName);
		}
		free(hardwareID);
	}
	return foundIt;
}

//-----------------------------------------------------------------------------
//	Method:			startSerial
//	Description:	Opens up the serial port connected to the romulator's
//						serial input.
//	Parameters:		HANDLE* hComm:			Pointer to the file handle for 
//												serial communications.
//					char* portName:			The string literal for the serial
//												device's port name 
//												(e.g., "COM4").
//					DCB* dcbSerialParams:	Pointer to the struct containing
//												serial properties, such as
//												baud rate and byte size.
//					COMMTIMEOUTS* timeouts:	Pointer to the struct containing
//												read and write timeout lengths
//												for the com port.
//	Returns:		Status code stating whether or not the port was
//												successfully opened.
//-----------------------------------------------------------------------------
int startSerial(SERIAL_PARAMETERS) {
	BOOL Status;
	printf("%s\n", portName);
	*hComm = CreateFile(	portName,						// Name of the serial port to open.
							GENERIC_READ | GENERIC_WRITE, 	// Access mode.
							0, 								// Sharing options (can't be shared = 0 for serial).
							NULL, 							// File operations. NULL for serial.
							OPEN_EXISTING, 					// Open existing port.
							0,								// 0 = non-overlapped IO. Use overlapping IO for multithread programming.
							NULL);							// NULL for serial.
	if (*hComm == INVALID_HANDLE_VALUE) {					
		return SERIAL_ERROR;
	}
	// Initialize our serial parameters struct:
	dcbSerialParams->DCBlength = sizeof(dcbSerialParams);
	// Get the serial status and test for validity:
	Status = GetCommState(*hComm, dcbSerialParams);
	if (Status == FALSE) {
		return SERIAL_ERROR;
	}
	// Set serial properties
	dcbSerialParams->BaudRate	= 9600;
	dcbSerialParams->ByteSize	= 8;
	dcbSerialParams->StopBits	= ONESTOPBIT;
	dcbSerialParams->Parity		= NOPARITY;
	SetCommState(hComm, dcbSerialParams);
	// Set com timeouts
	timeouts->ReadIntervalTimeout			= 50;		// Max time between arrival of two bytes.
	timeouts->ReadTotalTimeoutConstant		= 50;		// Added to product of multiplier & # of bytes.
	timeouts->ReadTotalTimeoutMultiplier	= 10;		// Total timeout period for read operations. This * # bytes.
	timeouts->WriteTotalTimeoutConstant		= 50;		// Added to product of multiplier & # of bytes.
	timeouts->WriteTotalTimeoutMultiplier	= 10;		// Total timeout period for write operations. This * # bytes.
	SetCommTimeouts(hComm, timeouts);
	return SERIAL_ESTABLISHED;
}

//-----------------------------------------------------------------------------
//	Method:			serialTransmit
//	Description:	Transmits data serially to our Romulator
//	Parameters:		char* sendBuffer:		The string to send over.
//					HANDLE* hComm:			Pointer to the file handle for 
//												serial communications.
//					char* portName:			The string literal for the serial
//												device's port name 
//												(e.g., "COM4").
//					DCB* dcbSerialParams:	Pointer to the struct containing
//												serial properties, such as
//												baud rate and byte size.
//					COMMTIMEOUTS* timeouts:	Pointer to the struct containing
//												read and write timeout lengths
//												for the com port.
//	Returns:		Status code stating whether or not the port was
//												successfully opened.
//-----------------------------------------------------------------------------
int serialTransmit(char* sendBuffer, SERIAL_PARAMETERS) {
	BOOL Status;
	DWORD dNoOfBytesToWrite = 0;		//	Number of bytes to write.
	DWORD dNoOfBytesWritten = 0;		//	Number of bytes already written.
	dNoOfBytesToWrite = sizeof(sendBuffer);
	Status = WriteFile(*hComm, sendBuffer, dNoOfBytesToWrite, &dNoOfBytesWritten, NULL);
	if (Status == TRUE)
		puts("Transmission was successful!");
	else {	
		puts("There was a problem sending over the string. Transmission aborted.");
		CloseHandle(*hComm);
		return TRANSMIT_ERROR;
	}
	//CloseHandle(*hComm);
	return TRANSMIT_SUCCESSFUL;
}

//-----------------------------------------------------------------------------
//	Method:			serialReceive
//	Description:	Transmits data serially to our Romulator
//	Parameters:		char* receiveBuffer:	The string to receive to.
//					HANDLE* hComm:			Pointer to the file handle for 
//												serial communications.
//					char* portName:			The string literal for the serial
//												device's port name 
//												(e.g., "COM4").
//					DCB* dcbSerialParams:	Pointer to the struct containing
//												serial properties, such as
//												baud rate and byte size.
//					COMMTIMEOUTS* timeouts:	Pointer to the struct containing
//												read and write timeout lengths
//												for the com port.
//	Returns:		Status code stating whether or not the port was
//												successfully opened.
//-----------------------------------------------------------------------------
int serialReceive(char* receiveBuffer, SERIAL_PARAMETERS) {
	BOOL Status;
	char tempChar;										//	An incoming character from the Arduino.
	DWORD NoBytesRead;									//	Number of bytes read thus far.
	int stringIterator = 0;								//	String iterator used to build the receiveBuffer from incoming bytes.
	DWORD dwEventMask;									//	Used to establish a serial event for receiving data.
	int loopCounter = 0;
	puts("Waiting for serial event. Please stand by.");
	Status = SetCommMask(*hComm, EV_RXCHAR);
	if (Status == FALSE) {
		puts("Failed setting the comm mask.");
		//CloseHandle(hComm);
		return RECEIVE_ERROR;
	}
	Status = WaitCommEvent(*hComm, &dwEventMask, NULL);
	if (Status == FALSE) {
		puts("Failed establishing comm event.");
		//CloseHandle(hComm);
		return RECEIVE_ERROR;
	}
	// Get our message!
	puts("Reading data in the comm buffer:");
	do {
		Status = ReadFile(*hComm, &tempChar, sizeof(tempChar), &NoBytesRead, NULL);
		if (Status == FALSE) {
			puts("Invalid data.");
			//CloseHandle(hComm);
			return RECEIVE_ERROR;
		}
		receiveBuffer[stringIterator++] = tempChar;
		//Sleep(10);
	} //while (NoBytesRead > 0);
	while (tempChar != '\0');
	

	return RECEIVE_SUCCESSFUL;
}
