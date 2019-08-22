/******************************************************************************
File:			emulator.h
Description:	This is the main header file for romulator. This contains all
					constants and function prototypes used throughout the code.

Files that include this header file
-----------------------------------
This header file serves as the main hub for all source code affiliated with
Romulator. As such, every .c file should #include "emulator.h". String
literals and "magic numbers" should be changed here as opposed to in the
.c files. This will allow for maximum consistency across all files in the
case of changed components or ports.
					
Common serial parameters
------------------------
Throughout the source code, the common serial parameters will be referenced
via macro constants SERIAL_PARAMETERS and SERIAL_PARAMS. They are as follows:
	HANDLE* hComm:			points to the Windows file handle that "handles" 
								the flow of information through the com port.
	char* portName:			the string name of the com port used by Romulator.
	DCB* dcbSerialParams:	points to the struct containing communications
								properties including baud rate, byte size,
								etc.
	COMMTIMEOUTS* timeouts:	points to the struct containing timeout info.
******************************************************************************/
#ifndef ROMULATOR_H
#define ROMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <initguid.h>
#include <devguid.h>
#include <setupapi.h>
#include <stdio.h>
#include <string.h>

//-----------------------------------------------------------------------------
//	Constants
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 255
#define MAX_RECORD_SIZE 512

// Serial codes:
#define DEVICE_NOT_FOUND -1
#define DEVICE_FOUND 0

#define SERIAL_ERROR -1
#define SERIAL_ESTABLISHED 0

#define TRANSMIT_ERROR -1
#define TRANSMIT_SUCCESSFUL 0

#define RECEIVE_ERROR -1
#define RECEIVE_SUCCESSFUL 0

#define DEVICE_NAME "Arduino Mega 2560"
#define NAME_LENGTH strlen(DEVICE_NAME) + 2
#define END_OF_NAME strlen(friendlyName) - strlen( "Arduino Mega 2560 (" ) - 1	
// These next 2 are super important!
#define SERIAL_PARAMETERS HANDLE* hComm, char* portName, DCB* dcbSerialParams, COMMTIMEOUTS* timeouts
#define SERIAL_PARAMS hComm, portName, dcbSerialParams, timeouts

// File types to write to the emulator
#define NOT_YET_DEFINED 0
#define S_RECORD 1
#define INTEL_HEX 2

// Starting status codes:
#define INVALID_ARGUMENTS -1

// Status codes:
#define VALID_RECORD 0
#define INVALID_FILENAME -1
#define INVALID_FORMAT -2
#define INVALID_LENGTH -3
#define INVALID_VALUES -4

//-----------------------------------------------------------------------------
//	Method Prototypes
//-----------------------------------------------------------------------------
// Serial method prototypes
int getPortNumber(char*);
int startSerial(SERIAL_PARAMETERS);
int serialTransmit(char* sendBuffer, SERIAL_PARAMETERS);
int serialReceive(char* receiveBuffer, SERIAL_PARAMETERS);

// Interface method prototypes
void mainMenu(SERIAL_PARAMETERS);
int readData(char filename[]);
int writeData(char filename[], SERIAL_PARAMETERS);
int emulate();
void romulatorHelp();

// Record verification
void verifyRecord(char filename[], int* returnStatus);
int verifySRecord(FILE* record);
int verifyIntelHex(FILE* record);

#endif