// Buffer
const int MAX_DATA_BUFFER = 100;
volatile int timesReceived = 0;

// Romulator Mode
const char READ_MODE = 1;
const char WRITE_MODE = 2;
const char EMULATE_MODE = 3;
const char DEFAULT_MODE = -1;

// Record type
const char S_RECORD = 1;
const char HEX_RECORD = 2;

volatile int romulatorMode = DEFAULT_MODE;
String incomingMessage = "";
volatile char inByte;



//---------------------------------------------------------------------
//  Method:       setup
//  Description:  Starting setup method for the Arduino.
//---------------------------------------------------------------------
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    timesReceived = 0;
    romulatorMode = DEFAULT_MODE;
    incomingMessage = "";
    // Open serial on the Arduino side.
    Serial.begin(9600, SERIAL_8N1);
    // Blink after serial has been established.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    // Flush the serial buffer
    while (Serial.available() > 0) {
        Serial.read();
    }
}



//---------------------------------------------------------------------
//  Method:       loop
//  Description:  Main loop of the Arduino program.
//---------------------------------------------------------------------
void loop() {
    // Wait for incoming data.
    // Once data is received, serialEvent is triggered.
    switch(romulatorMode) {
        case WRITE_MODE:
            // Write Mode:
            // The Arduino received an S Record or Intel Hex.
            writeMode();
            break;
        case READ_MODE:
            // Read Mode:
            // The Arduino received an instruction to prepare a
            //    RAM dump to send to the PC.
            break;
        case EMULATE_MODE:
            // Emulate Mode:
            // The Arduino received an instruction to put the
            //    emulator into emulation mode.
            break;
    }
}



//---------------------------------------------------------------------
//  Method:       serialEvent
//  Description:  Event handler for incoming data to the Arduino.
//                  Equivalent to if(available()){...}.
//---------------------------------------------------------------------
void serialEvent() {
    //delay(10);
    // Read in characters to the incomingMessage string 1 byte at a
    //    time. Since Serial is asynchronous, wait to process each
    //    character.
    while (Serial.available() > 0) {
        inByte = (char)Serial.read();
        incomingMessage += inByte;
        delay(10);
    }
    // The first character will determine which mode the Arduino
    //    should put the Romulator into.
    if (incomingMessage[0] == 'S' || incomingMessage[0] == ':' || timesReceived > 0) {
        romulatorMode = WRITE_MODE;
        timesReceived++;
        Serial.print("Times received: ");
        Serial.println(timesReceived);
    }
}



//---------------------------------------------------------------------
//  Method:       writeMode
//  Description:  Write mode for the Arduino. Parse an incoming
//                    record and write it to RAM.
//---------------------------------------------------------------------
void writeMode() {
    String incoming2 = "";      // Acknowledgement from the PC.
    int addressBytes = 0;
    int address = 0;
    int dataBytes = 0;
    byte data[MAX_DATA_BUFFER] = {0};

    // Echo back the record for verification.
    Serial.println(incomingMessage);
    // Wait for a response.
        //----------------------------------------------------------
        // Debug
        if (timesReceived == 2)
            Serial.println("Is this what you sent?");
        // End Debug
        //----------------------------------------------------------
    while (Serial.available() == 0) ;
    delay(10);
    // Wait for acknowledgement.
    while (Serial.available() > 0) {
        inByte = (char)Serial.read();
        incoming2 += inByte;
        delay(10);
    }

    // Check the acknowledgement.
    if (incoming2.startsWith("yes")) {
        timesReceived = 0;
        parseRecord(incomingMessage, address, addressBytes, data, dataBytes);
        writeBits(address, addressBytes, data, dataBytes);
    }
    else if (timesReceived == 2) {
        incomingMessage = incomingMessage + incoming2;
    }
    
    incomingMessage = "";
    romulatorMode = DEFAULT_MODE;
    
}



bool parseRecord(String record, int& address, int& addressBytes, byte data[], int& dataBytes) {
    int recordIterator = 0;
    int recordType = 0;
    int recordBytes = 0;
    int recordChars = 0;
    int addressChars = 8;
    int dataChars = 0;
    int checksumChars = 2;
    String currentSequence = "";
    char testChar = record[recordIterator++];
    if (testChar == 'S') {
        // It's an S Record
        
        // Get the record type.
        currentSequence = record.charAt(recordIterator++);
        char *type = currentSequence.c_str();
        recordType = strtol(type, NULL, 16);
        if (recordType == 0) {
                //----------------------------------------------------------
                // Debug
                Serial.println("Type 0 is a header record. Can be discarded.");
                // End debug
                //----------------------------------------------------------
            return false;
        }
        switch (recordType) {
            case 1:
            case 9:
                addressChars = 4;
                break;
            case 2:
            case 8:
                addressChars = 6;
                break;
            case 3:
            case 7:
                addressChars = 8;
                break;
            default:
                addressChars = 0;
        }
        addressBytes = addressChars / 2;
            //----------------------------------------------------------
            // Debug
            Serial.println("Processing record type.");
            Serial.print("Current sequence: ");
            Serial.println(currentSequence);
            Serial.print("type (string): ");
            Serial.println(type);
            Serial.print("Record type (int): ");
            Serial.println(recordType);
            Serial.print("Address characters: ");
            Serial.println(addressChars);
            Serial.print("\n");
            // End debug
            //----------------------------------------------------------
        
        // Get the record length.
        currentSequence = record.charAt(recordIterator++);
        currentSequence = currentSequence + record.charAt(recordIterator++);
        char* lengthBytes = currentSequence.c_str();
        recordBytes = strtol(lengthBytes, NULL, 16);
        recordChars = recordBytes * 2;
        dataChars = recordChars - addressChars - checksumChars;
        dataBytes = dataChars / 2;
            //----------------------------------------------------------
            // Debug
            Serial.println("Processing record length.");
            Serial.print("Current sequence: ");
            Serial.println(currentSequence);
            Serial.print("length (string): ");
            Serial.println(lengthBytes);
            Serial.print("length (int): ");
            Serial.println(recordBytes);
            Serial.print("Characters remaining in the record: ");
            Serial.println(recordChars);
            Serial.print("Address characters: ");
            Serial.println(addressChars);
            Serial.print("Data characters: ");
            Serial.println(dataChars);
            Serial.print("Checksum: ");
            Serial.println(checksumChars);
            //----------------------------------------------------------

        // Get the address.
        currentSequence = "";
        for (int i = 0; i < addressChars; i++) {
            currentSequence = currentSequence + record.charAt(recordIterator++);
        }
        char* addressBytes = currentSequence.c_str();
        address = strtol(addressBytes, NULL, 16);
            //----------------------------------------------------------
            // Debug
            Serial.print("Address (hex): ");
            Serial.println(addressBytes);
            Serial.print("Address (value): ");
            Serial.println(address, HEX);
            //----------------------------------------------------------

        // Get the data.
        currentSequence = "";
        for (int i = 0; i < dataChars; i++) {
            currentSequence = currentSequence + record.charAt(recordIterator++);
        }
        //char* dataBytes = currentSequence.c_str();
        Serial.println('\n');
        for (int i = 0; i < dataChars; i += 2) {
            String dataString = "";
            char character1 = currentSequence.charAt(i);
            char character2 = currentSequence.charAt(i+1);
            dataString.concat(character1);
            dataString.concat(character2);
            char* dataByteChars = dataString.c_str();
            data[i/2] = strtol(dataByteChars, NULL, 16);
        }
        Serial.println('\n');
        //data = strtol(dataBytes, NULL, 16);
            //----------------------------------------------------------
            // Debug
            Serial.print("Data (hex): ");
            Serial.println(dataBytes);
            Serial.print("Data (values): ");
            for (int i = 0; i < dataBytes; i++)
                Serial.println(data[i], HEX);
            //----------------------------------------------------------
    }
    else {
        // It's a Hex Record
    }
    return true;
}

void writeBits(int address, int addressBytes, byte data[], int dataBytes) {
    const int bytes = dataBytes;
    int addressBits = 17;
    int dataBits = 8;
    byte addressBit[bytes][17] = {0};
    byte dataBit[bytes][8] = {0};
    for (int i = 0; i < dataBytes; i++) {
        int currentAddress = address + i;
        Serial.print("Current address: ");
        Serial.println(currentAddress, HEX);
        Serial.print("Address: ");
        for (int j = 0; j < addressBits; j++) {
            addressBit[i][j] = currentAddress << j;
            Serial.print(addressBit[i][j]);
            addressBit[i][j] >>= 16;
            Serial.println(addressBit[i][j]);
        }
        Serial.print("\nData: ");
        for (int k = 0; k < dataBits; k++) {
            dataBit[i][k] = data[i] << k;
            dataBit[i][k] >>= 7;
            Serial.print(dataBit[i][k]);
        }
        Serial.print("\n");
    }
    //int addressBits = addressBytes * 8;
}
