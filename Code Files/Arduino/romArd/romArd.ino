#include "romArd.h"

//------------------------------------------------------------------------------
//  Method:       setup
//  By:           Team Romulator:
//                David Landry, chief software engineer
//                Biniyam Yemane-Berhane, Nema Karimi, Thien Nguyen
//  For:          CE / BEE Capstone; University of Washington, Bothell
//                Developed for the School of STEM
//  Advisor:      Dr. Arnold S. Berger
//  Date:         August 9, 2019
//  Description:  Starting setup method for the Arduino.
//------------------------------------------------------------------------------
void setup() 
{
    // Set up default pin modes. Everything is set high by default.
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    for (int i = 22; i <= 38; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    
    // Initialize variables.
    incomingMessage = "";
    
    // Open serial on the Arduino side.
    Serial.begin(9600, SERIAL_8N1);
    // Flush the serial buffer
    while (Serial.available() > 0) {
        Serial.read();
    }
}



//------------------------------------------------------------------------------
//  Method:       loop
//  Description:  Main loop of the Arduino program.
//  Calls:        waitForMessage to get the emulator mode from the PC.
//                writeMode to handle the emulator's write mode.
//                emulateMode to handle the emulator's emulate mode.
//                readMode to handle the emulator's read (dump) mode.
//------------------------------------------------------------------------------
void loop() 
{
    // Wait for incoming data.
    incomingMessage = waitForMessage();

    // Analyze incoming data to determine which mode to switch to.
    // Set to Write mode if the incoming message starts with an 'S' or ':'.
    if (  incomingMessage.charAt(0) == 'S' || 
          incomingMessage.charAt(0) == ':') {
        // First half of the record.
        Serial.println("Got it! Thanks!");
        // Wait for second half of the record.
        incomingMessage.concat(waitForMessage());
        writeMode();
    }
    // Set to Emulate mode if the emulator receives any form of "emulate".
    else if (verifyMessage(incomingMessage, "emulat"))
        emulateMode();

    // Set to Read mode (RAM dump) if the emulator receives "read".
    else if (verifyMessage(incomingMessage, "read"))
        readMode();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Method:         writeMode
//  Description:    Write mode for the Arduino. Parse an incoming
//                      record and write it to RAM.
//  Helper Methods: parseSRecord, parseHexRecord to parse the record into
//                      address and data fields.
//                  writeBits, verifyWrite to write the bits into memory and
//                      verify that the data was successfully written in.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void writeMode() 
{
    bool isDataRecord = true;
    String incoming2 = "";      // Acknowledgement from the PC.
    int addressBytes = 0;
    unsigned long address = 0;
    int dataBytes = 0;
    byte data[MAX_DATA_BUFFER] = {0};
    bool verified = false;

    // Set up the address and data pin modes.
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    for (int i = 22; i <= 38; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    // Echo back the record for verification.
    Serial.println(incomingMessage);
    // Wait for a response.
    incoming2 = waitForMessage();
    // Check the acknowledgement.
    if (verifyMessage(incoming2, "yes")) {
        // Determine which kind of record it is.
        if (incomingMessage.charAt(0) == 'S')
            isDataRecord = parseSRecord(  address, 
                                        addressBytes, 
                                        data, 
                                        dataBytes);
        else if (incomingMessage.charAt(0) == ':')
            isDataRecord = parseHexRecord(  address, 
                                          addressBytes, 
                                          data, 
                                          dataBytes);
        if (isDataRecord)
            verified = writeBits(address, data, dataBytes);
        else
            verified = true;
    }
    setup();
    
    if (verified)
        Serial.println("ready");
    else
        Serial.println("error");
        
}


//------------------------------------------------------------------------------
//  Method:       parseSRecord
//  Description:  Assign the address and a sequence of data values from the
//                  S Record
//------------------------------------------------------------------------------
bool parseSRecord(  unsigned long& address, 
                    int& addressBytes, 
                    byte data[], 
                    int& dataBytes) 
{
    int recordIterator = 1;
    int recordType = 0;
    int recordBytes = 0;
    int recordChars = 0;
    int addressChars = 8;
    int dataChars = 0;
    int checksumChars = 2;
    String currentSequence = "";
        
    // Get the record type.
    currentSequence = incomingMessage.charAt(recordIterator++);
    char *type = currentSequence.c_str();
    recordType = strtol(type, NULL, 16);
    switch (recordType) {
        case 0:
        case 9:
        case 8:
        case 7:
            return false;
        case 1:
            addressChars = 4;
            break;
        case 2:
            addressChars = 6;
            break;
        case 3:
            addressChars = 8;
            break;
        default:
            return false;
    }
    addressBytes = addressChars / 2;

    // Get the record length.
    currentSequence = incomingMessage.charAt(recordIterator++);
    currentSequence = currentSequence + 
                      incomingMessage.charAt(recordIterator++);
    char* lengthBytes = currentSequence.c_str();
    recordBytes = strtol(lengthBytes, NULL, 16);
    recordChars = recordBytes * 2;
    dataChars = recordChars - addressChars - checksumChars;
    dataBytes = dataChars / 2;

    // Get the address.
    currentSequence = "";
    for (int i = 0; i < addressChars; i++) {
        currentSequence = currentSequence + 
                          incomingMessage.charAt(recordIterator++);
    }
    char* addressBytesChar = currentSequence.c_str();
    address = strtol(addressBytesChar, NULL, 16);

    // Get the data.
    currentSequence = "";
    for (int i = 0; i < dataChars; i++) {
        currentSequence = currentSequence + 
                          incomingMessage.charAt(recordIterator++);
    }
    for (int i = 0; i < dataChars; i += 2) {
        String dataString = "";
        char character1 = currentSequence.charAt(i);
        char character2 = currentSequence.charAt(i+1);
        dataString.concat(character1);
        dataString.concat(character2);
        char* dataByteChars = dataString.c_str();
        data[i/2] = strtol(dataByteChars, NULL, 16);
    }

    return true;
}


//------------------------------------------------------------------------------
//  Method:       parseHexRecord
//  Description:  Assign the address and a sequence of data values from the
//                  Intel Hex Record
//------------------------------------------------------------------------------
bool parseHexRecord(  unsigned long& address, 
                      int& addressBytes, 
                      byte data[], 
                      int& dataBytes) 
{
    int recordIterator = 1;
    int recordType = 0;
    int recordBytes = 0;
    int recordChars = 0;
    int addressChars = 4;
    int dataChars = 0;
    int checksumChars = 2;
    String currentSequence = "";

    // Get the number of data characters
    currentSequence = incomingMessage.charAt(recordIterator++);
    currentSequence = currentSequence + 
                      incomingMessage.charAt(recordIterator++);
    char* dataLength = currentSequence.c_str();
    dataChars = strtol(dataLength, NULL, 16) * 2;

    // Get the address.
    currentSequence = "";
    for (int i = 0; i < addressChars; i++) {
        currentSequence = currentSequence + 
                          incomingMessage.charAt(recordIterator++);
    }
    char* addressBytesChar = currentSequence.c_str();
    address = strtol(addressBytesChar, NULL, 16);
    
    // Get the record type.
    currentSequence = incomingMessage.charAt(recordIterator++);
    currentSequence = currentSequence + 
                      incomingMessage.charAt(recordIterator++);
    char* type = currentSequence.c_str();
    recordType = strtol(type, NULL, 16);
    if (recordType != 0) {
        return false;
    }

    // Get the data.
    currentSequence = "";
    for (int i = 0; i < dataChars; i++) {
        currentSequence = currentSequence + 
                          incomingMessage.charAt(recordIterator++);
    }
    for (int i = 0; i < dataChars; i += 2) {
        String dataString = "";
        char character1 = currentSequence.charAt(i);
        char character2 = currentSequence.charAt(i+1);
        dataString.concat(character1);
        dataString.concat(character2);
        char* dataByteChars = dataString.c_str();
        data[i/2] = strtol(dataByteChars, NULL, 16);
    }
    
    return true;
}


//------------------------------------------------------------------------------
//  Method:       writeBits
//  Description:  Manipulate the bits to write memory to the RAM.
//------------------------------------------------------------------------------
bool writeBits( unsigned long& address, byte data[], int& dataBytes )
{
    digitalWrite(PROG, LOW);
    digitalWrite(RD, LOW);
 //   digitalWrite(SEL, LOW);
    for (int i = 0; i < dataBytes; i++) {
        // Set the address bits for the current address.
        for (int a = 0; a < ADDRESS_BITS; a++) {
            digitalWrite(((a <= 7) ? (9-a) : (22+a-8)) ,
                          (address & (1<<a)) ? HIGH : LOW);
        }
        // Set the data bits for data at the current address.
        for (int d = 0; d < DATA_BITS; d++) {
            digitalWrite((31+d) , ((data[i]&(1<<d)) ? HIGH : LOW));
        }
        address++;
        // Assert the write bit.
        digitalWrite(SEL, LOW);
        digitalWrite(WRITE, LOW);
        //delay(100);
        digitalWrite(SEL, HIGH);
        digitalWrite(WRITE, HIGH);
    }
    return true;
    //return verifyWrite(address, data, dataBytes);
}

//------------------------------------------------------------------------------
//  Method:       verifyWrite
//  Description:  Read the contents of the RAM to verify it was correctly
//                    written to.
//------------------------------------------------------------------------------
bool verifyWrite( unsigned long address, byte data[], int dataBytes )
{
    byte tempData2 = 0;
    digitalWrite(RD, HIGH);
    digitalWrite(PROG, LOW);
    digitalWrite(SEL, LOW);
    digitalWrite(WRITE, HIGH);
    // Set address bit modes
    for (int i = 9; i >= 2; i--)
        pinMode(i, OUTPUT);
    // Set more address bit modes
    for (int i = 22; i <= 30; i++)
        pinMode(i, OUTPUT);
    // Set data bit modes.
    for (int i = 31; i <= 38; i++)
        pinMode(i, INPUT);
    // Go through each byte address to read & verify.
    for (int i = 0; i < dataBytes; i++) {
        // Set the address bits for the current address.
        for (int a = 0; a < ADDRESS_BITS; a++) {
            digitalWrite(((a <= 7) ? (9-a) : (22+a-8)) ,
                          (address & (1<<a)) ? HIGH : LOW);
        }
        delay(1);
        // Get the data bits for that address.
        byte tempData = 0;
        for (int d = 0; d < DATA_BITS; d++) {
            tempData += (digitalRead(d+31) == HIGH) ? (1 << d) : 0;
        }
        tempData2 = tempData;
      
        if (tempData != data[i])
            return false;
      
    }

    return true;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Method:       emulateMode
//  Description:  This is what the ROM emulator is all about! It sets up the
//                    circuitry to be able to emulate ROM on a target system.
//  Precondition: This method assumes that data has already beenn programmed in
//                    to the RAM of the emulator. If it hasn't, results can
//                    be unpredictable and could potentially damage the target
//                    system.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void emulateMode()
{
    String quitCommand = "";
    digitalWrite(RD, HIGH);
    digitalWrite(PROG, HIGH);
    digitalWrite(SEL, HIGH);
    digitalWrite(WRITE, HIGH);
    do {
        quitCommand = waitForMessage();
    } while (!verifyMessage(quitCommand, "quit"));
    
    setup();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Method:       readMode
//  Description:  This mode produces a RAM dump to read the contents of the
//                    RAM on the emulator.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void readMode()
{
    digitalWrite(RD, HIGH);
    digitalWrite(PROG, LOW);
    digitalWrite(SEL, LOW);
    digitalWrite(WRITE, HIGH);
}


//------------------------------------------------------------------------------
//  Method:       waitForMessage
//  Description:  Makes the Arduino wait for an incoming message from the
//                    PC.
//  Returns:      The message.
//------------------------------------------------------------------------------
String waitForMessage() {
    String incoming2 = "";
    while (Serial.available() == 0) ;
    delay(10);
    // Read the acknowledgement from the PC.
    while (Serial.available() > 0) {
        char inByte = (char)Serial.read();
        incoming2 += inByte;
        delay(10);
    }
    return incoming2;
}

//------------------------------------------------------------------------------
//  Method:       verifyMessage
//  Description:  Compares a message that came in from the PC with a message
//                  that was expected whilst flushing the output buffer.
//------------------------------------------------------------------------------
bool verifyMessage(String test, String want) {
    Serial.flush();
    while(Serial.available())
        Serial.read();
    if (test.startsWith(want))
        return true;
    else
        return false;
}
