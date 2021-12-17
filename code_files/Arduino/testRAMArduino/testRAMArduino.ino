#include <ctype.h>
#include "romArd.h"
//------------------------------------------------------------------------
//  File:         testRAMArduino
//  By:           Team Romulator:
//                David Landry, chief software engineer
//                Biniyam Yemane-Berhane, Nema Karimi, Thien Nguyen
//  For:          CE / BEE Capstone; University of Washington, Bothell
//                Developed for the School of STEM
//  Advisor:      Dr. Arnold S. Berger
//  Date:         August 9, 2019
//  Description:  This program tests whether the Arduino can toggle
//                    pins on the RAM high & low via serial monitor.
//------------------------------------------------------------------------
//volatile int pinNumber;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    for (int i = 2; i < 14; i++)
        pinMode(i, OUTPUT);
    for (int i = 22; i < 39; i++)
        pinMode(i, OUTPUT);
    digitalWrite(PROG, LOW);
    digitalWrite(RD, LOW);
    digitalWrite(SEL, LOW);
    digitalWrite(WRITE, LOW);
}

void loop() {
    // put your main code here, to run repeatedly:
    if (Serial.available() == 0)
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
        Serial.println("Select a pin to test:");
        Serial.println("9  -> A0  (pin 12");
        Serial.println("8  -> A1  (pin 11");
        Serial.println("7  -> A2  (pin 10");
        Serial.println("6  -> A3  (pin 9");
        Serial.println("5  -> A4  (pin 8");
        Serial.println("4  -> A5  (pin 7");
        Serial.println("3  -> A6  (pin 6");
        Serial.println("2  -> A7  (pin 5");
        Serial.println("22 -> A8  (pin 27");
        Serial.println("23 -> A9  (pin 26");
        Serial.println("24 -> A10 (pin 23");
        Serial.println("25 -> A11 (pin 25");
        Serial.println("26 -> A12 (pin 4");
        Serial.println("27 -> A13 (pin 28");
        Serial.println("28 -> A14 (pin 3");
        Serial.println("29 -> A15 (pin 31");
        Serial.println("30 -> A16 (pin 2");
        Serial.print("\n");
        Serial.println("31 -> D0 (pin 13");
        Serial.println("32 -> D1 (pin 14");
        Serial.println("33 -> D2 (pin 15");
        Serial.println("34 -> D3 (pin 17");
        Serial.println("35 -> D4 (pin 18");
        Serial.println("36 -> D5 (pin 19");
        Serial.println("37 -> D6 (pin 20");
        Serial.println("38 -> D7 (pin 21");
    
    while (Serial.available() == 0) ;
    
    if (Serial.available() > 0) {
        bool validInput = true;
        String input = Serial.readString();
        int pinNumber = input.toInt();
        Serial.println(pinNumber);
                    if (pinNumber < 31)
                Serial.print("Testing A");
            else
                Serial.print("Testing D");
            Serial.print(input);
            Serial.print(" in: ");
            for (int i = 7; i > 0; i--) {
                Serial.print(i);
                Serial.print("...");
                delay(1000);
            }
            for (int i = 0; i < 3; i++) {
                Serial.print("Test ");
                Serial.print(i);
                Serial.println(" of 3:");
                Serial.println("\tHigh for 5 seconds.");
                digitalWrite(pinNumber, HIGH);
                delay(5000);
                Serial.println("\tLow for 5 seconds.");
                digitalWrite(pinNumber, LOW);
                delay(5000);
            }

        if ( (pinNumber >= 9 && pinNumber <= 2) || (pinNumber >= 22 && pinNumber <= 30) ||  // Address lines
             (pinNumber >= 31 && pinNumber <= 38) ) {                                       // Data lines
        }
    }
}
