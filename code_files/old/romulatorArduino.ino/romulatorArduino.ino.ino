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


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
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

void loop() {
    String incoming2 = "";
    // Wait for incoming data.

    // Once data is received, serialEvent is triggered.

    // Write Mode:
    // The Arduino received an S Record or Intel Hex.
    if (romulatorMode == WRITE_MODE) {
        // Echo back the record for verification.
        //digitalWrite(LED_BUILTIN, HIGH);
        Serial.println(incomingMessage);
        
        // Wait for a response.
        while (Serial.available() == 0) ;
        
        delay(10);
        //digitalWrite(LED_BUILTIN, LOW);
        // Check the acknowledgement.
        while (Serial.available() > 0) {
            incoming2 += inByte;
            delay(10);
        }
        if (incomingMessage.equals("yes")) {
            // Forward to the writeMode method, where the record will be
            //    parsed and sent to memory.
            writeMode(incomingMessage);
        }
        digitalWrite(LED_BUILTIN, HIGH);
        incomingMessage = "";
        romulatorMode = DEFAULT_MODE;
    }
}

void serialEvent() {
    digitalWrite(LED_BUILTIN, HIGH);
    
    delay(10);
    while (Serial.available() > 0) {
        inByte = (char)Serial.read();
        incomingMessage += inByte;
        delay(10);
    }
    
    if (incomingMessage[0] == 'S' || incomingMessage[0] == ':')
        romulatorMode = WRITE_MODE;
    digitalWrite(LED_BUILTIN, LOW);
}


void writeMode(String record) {
    int address = 0;
    int data = 0;
    parseRecord(record, address, data);
    writeBits(address, data);
    // woo hoo
}

void parseRecord(String record, int& address, int& data) {
    int recordIterator = 0;
    int recordType = 0;
    String currentSequence = "";
    char testChar = record[recordIterator++];
    if (testChar == 'S') {
        // It's an S Record
        // Get the record type.
        currentSequence = "" + record[recordIterator++];
        char csbs[currentSequence.length()];
        currentSequence.toCharArray(csbs, currentSequence.length());
        recordType = strtol(csbs, NULL, 16);
        Serial.println(recordType);
//        recordType = strtol(currentSequence.toCharArray(csbs, currentSequence.length()), NULL, 16);
    }
    else {
        // It's a Hex Record
    }
    for (int i = 0; i < record.length(); i++) {
        testChar = record[i];
    }
    // woo hoo
}

void writeBits(int address, int data) {
    ;
}
