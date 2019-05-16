volatile bool isLastByte = false;
String incomingMessage = "";
volatile char inByte;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    isLastByte = false;
    incomingMessage = "";
    // Open serial on the Arduino side.
    Serial.begin(9600, SERIAL_8N1);
    // Blink after serial has been established.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    // Flush the serial buffer
    while (Serial.available() > 0) {
        char junk = Serial.read();
    }
}

void loop() {
    // Wait for incoming data. Echo it back.
    while (Serial.available() > 0) {
        inByte = (char)Serial.read();
        incomingMessage += inByte;
        delay(10);
        isLastByte = true;
    }

    if (isLastByte) {
        char sendMessage[incomingMessage.length() + 1];
        incomingMessage.toCharArray(sendMessage,incomingMessage.length() + 1);
        for (int i = 0; i < incomingMessage.length() + 1; i++) {
            Serial.print(sendMessage[i]);
        }
//        Serial.print('\0');
        incomingMessage = "";
        while (Serial.available() > 0)
            Serial.read();
        isLastByte = false;
    }
}

void serialEvent() {
    while (Serial.available() > 0) {
        inByte = (char)Serial.read();
        incomingMessage += inByte;
        delay(10);
    }
    isLastByte = true;
}
