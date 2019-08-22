#include <windows.h>
#include <stdio.h>

int main() {
	//-------------------------
	// Variable declarations:
	//-------------------------
	// Port variables
	char comPortName[] = "\\\\.\\COM5";					// 	Port name.
	HANDLE hComm;										// 	Windows handle. Handles handle file and port communications.
	BOOL Status;										// 	Records status during port operations. Aborts whenever it is set to false.
	DCB dcbSerialParams = { 0 };						//	Struct containing all the properties for serial communications.
	COMMTIMEOUTS timeouts = { 0 };						// 	Struct containing all the timeout properties.
	// Transmit Variables
	char sendBuffer[] = "Hello world";					// 	Message to send to the Arduino.
	DWORD dNoOfBytesToWrite = 0;						//	Number of bytes to write.
	DWORD dNoOfBytesWritten = 0;						//	Number of bytes written.
	// Receive Variables
	char receiveBuffer[256];							// 	Message to receive from the Arduino.
	char tempChar;										//	An incoming character from the Arduino.
	DWORD NoBytesRead;									//	Number of bytes read thus far.
	int stringIterator = 0;								//	String iterator used to build the receiveBuffer from incoming bytes.
	DWORD dwEventMask;									//	Used to establish a serial event for receiving data.
	int loopCounter = 0;
	
	//-------------------------------------------------------
	// Establish serial communications at our desired port.
	//-------------------------------------------------------
	hComm = CreateFile(	comPortName,					// Name of the serial port to open.
						GENERIC_READ | GENERIC_WRITE, 	// Access mode.
						0, 								// Sharing options (can't be shared = 0 for serial).
						NULL, 							// File operations. NULL for serial.
						OPEN_EXISTING, 					// Open existing port.
						0, 								// 0 = non-overlapped IO. Use overlapping IO for multithread programming.
						NULL);							// NULL for serial.
	printf("hComm = %s. Address = %X.\n", comPortName, hComm);
	if (hComm == INVALID_HANDLE_VALUE) {					
		printf("Error in opening serial port.\n");
		CloseHandle(hComm);
		return 0;
	}
	else
		printf("Opening serial port successful.\n");
	// Initialize our serial parameters struct:
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	// Get the serial status and test for validity:
	Status = GetCommState(hComm, &dcbSerialParams);
	if (Status == FALSE) {
		printf("Could not get port status.\n");
		CloseHandle(hComm);
		return 0;
	}
	else
		printf("Successfully acquired port status.\n");
	printf("DCB address: %X\n", &dcbSerialParams);
	// Set serial properties
	dcbSerialParams.BaudRate	= CBR_9600;
	dcbSerialParams.ByteSize	= 8;
	dcbSerialParams.StopBits	= ONESTOPBIT;
	dcbSerialParams.Parity		= NOPARITY;
	SetCommState(hComm, &dcbSerialParams);
	// Set com timeouts
	timeouts.ReadIntervalTimeout			= 50;		// Max time between arrival of two bytes.
	timeouts.ReadTotalTimeoutConstant		= 50;		// Added to product of multiplier & # of bytes.
	timeouts.ReadTotalTimeoutMultiplier		= 10;		// Total timeout period for read operations. This * # bytes.
	timeouts.WriteTotalTimeoutConstant		= 50;		// Added to product of multiplier & # of bytes.
	timeouts.WriteTotalTimeoutMultiplier	= 10;		// Total timeout period for write operations. This * # bytes.
	SetCommTimeouts(hComm, &timeouts);
	
	
	//----------------------------------
	//	Serial transmit
	//----------------------------------
	printf("Attempting to send the string \"%s\":\n", sendBuffer);
	dNoOfBytesToWrite = sizeof(sendBuffer);
	Status = WriteFile(hComm, sendBuffer, dNoOfBytesToWrite, &dNoOfBytesWritten, NULL);
	if (Status == TRUE)
		printf("Transmission was successful!\n");
	else {
		printf("There was a problem sending over the string. Mission aborted.\n");
		CloseHandle(hComm);
		return 0;
	}
	

	
	//----------------------------------
	//	Serial receive
	//----------------------------------
	
	Status = SetCommMask(hComm, EV_RXCHAR);
	if (Status == FALSE) {
		puts("Failed setting the comm mask.");
		CloseHandle(hComm);
		return 0;
	}
	puts("Waiting for serial event. Please stand by.");
	Status = WaitCommEvent(hComm, &dwEventMask, NULL);
	if (Status == FALSE) {
		puts("Failed establishing comm event.");
		CloseHandle(hComm);
		return 0;
	}
	// Get our message!
	puts("Reading data in the comm buffer:");
	do {
		Status = ReadFile(hComm, &tempChar, sizeof(tempChar), &NoBytesRead, NULL);
		if (Status == FALSE) {
			puts("Invalid data.");
			CloseHandle(hComm);
			return 0;
		}

		receiveBuffer[stringIterator++] = tempChar;
	} //while (NoBytesRead > 0);
	while (tempChar != '\0');
	
	
	puts("The Arduino says:");
	puts(receiveBuffer);

	// We have what we need, so close the port.
	CloseHandle(hComm);
	
	
	//-----------------------------------
	//	The rest of the stuff to do.
	//-----------------------------------
	if (strcmp(receiveBuffer, sendBuffer) != 0)
		puts("Received message does not match sent message. Bummer!");
	else
		puts("Message received matches message sent. Mission successful!");

	
	return 0;
	
	
	
}
