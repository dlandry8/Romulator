#ifndef ROMULATOR_ARDUINO
#define ROMULATOR_ARDUINO
//------------------------------------------------------------------------------
//	File:			romArd.h
//	By:				Team Romulator:
//					David Landry, chief software engineer
//					Biniyam Yemane-Berhane, Nema Karimi, Thien Nguyen
//	For:			CE / BEE Capstone; University of Washington, Bothell
//					Developed for the School of STEM
//	Advisor:		Dr. Arnold S. Berger
//	Date:			August 9, 2019


//------------------------------------------------------------------------------
// Control Pins
//------------------------------------------------------------------------------

// PROG controls the selection pin on the multiplexers, selecting the input:
//    Low: Romulator
//    High: Target system.
//    Write mode:   Set low.
//    Emulate mode: Set high.
const char PROG = 13;

// RD selects which buffer route.
//    Low:  OE high, LBUF high, UBUF low. Upper buffers. Used to write to RAM.
//    High: OE low, LBUF low, UBUF high. Lower buffers. Used to read from RAM
//        by the target system.
//    Write mode:   Set low.
//    Emulate mode: Set high.
const char RD = 12;

// WRITE toggles the RAM's WE pin.
//    Low:  Set the RAM's WE pin low, enabling writing to the RAM.
//    High: Set its WE pin high, disabling writing.
//    Write mode:   Set low.
//    Emulate mode: Set high.
const char WRITE = 11;

// SEL toggles chip eneble.
//    Low: CE1 low. Enabled.
//    High: CE1 high. Disabled.
//    Emulate mode: Doesn't matter.
//    Should always be low.
const char SEL = 10;

// Address bits
const char Addr[17] = {9, 8, 7, 6, 5, 4, 3, 2, 22, 23, 24, 25, 26, 27, 28, 29, 30};
// Data bits
const char Data[8] = {31, 32, 33, 34, 35, 36, 37, 38};

//------------------------------------------------------------------------------
//  More constants
//------------------------------------------------------------------------------

// Buffer
const int MAX_DATA_BUFFER = 100;

// bit count
const char ADDRESS_BITS = 17;
const char DATA_BITS = 8;

// Record type
const char S_RECORD = 1;
const char HEX_RECORD = 2;

// Global Variables
String incomingMessage = "";


//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

// Write mode:
void writeMode();
bool parseSRecord(	unsigned long& address, 
                    int& addressBytes, 
                    byte data[], 
                    int& dataBytes);
bool parseHexRecord(unsigned long& address, 
                    int& addressBytes, 
                    byte data[], 
                    int& dataBytes);
bool writeBits( unsigned long& address, byte data[], int& dataBytes );
bool verifyWrite( unsigned long address, byte data[], int dataBytes );

// Read mode:
void readMode();

// Emulate mode:
void emulateMode();

// Helper methods:
String waitForMessage();
bool verifyMessage(String test, String want);

#endif
