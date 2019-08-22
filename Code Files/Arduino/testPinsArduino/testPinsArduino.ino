#include <ctype.h>
//------------------------------------------------------------------------
//  File:         testPinsArduino
//  By:           Team Romulator:
//                David Landry, chief software engineer
//                Biniyam Yemane-Berhane, Nema Karimi, Thien Nguyen
//  For:          CE / BEE Capstone; University of Washington, Bothell
//                Developed for the School of STEM
//  Advisor:      Dr. Arnold S. Berger
//  Date:         August 9, 2019
//  Description:  This program allows a tester to toggle pins on the
//                    Arduino via serial monitor. The tester may then
//                    probe the connections on the other side of the
//                    Arduino ports.
//------------------------------------------------------------------------
volatile int pinNumber;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    for (int i = 2; i < 14; i++)
        pinMode(i, OUTPUT);
    for (int i = 22; i < 39; i++)
        pinMode(i, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    if (Serial.available() == 0)
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);
        Serial.println("Select a pin to test:");
        Serial.println("2 -> Buffer u2, pin 15, RAM, pin 5");
        Serial.println("3 -> Buffer u2, pin 13, RAM pin 6");
        Serial.println("4 -> Buffer u2, pin 11, RAM pin 7");
        Serial.println("5 -> Buffer U2, pin 8, RAM pin 8");
        Serial.println("6 -> Buffer U2, pin 6, RAM pin 9");
        Serial.println("7 -> Buffer U2, pin 4, RAM pin 10");
        Serial.println("8 -> Buffer U2, pin 2, RAM pin 11");
        Serial.println("9 -> Buffer U3, pin 17 RAM pin 12");
        /*
        Serial.println("10 -> Mux IC2, pin 5 (~CS)");
        Serial.println("11 -> RAM IC1, pin 29 (~WE)");
        Serial.println("12 -> Mux IC2, pin 2 (~RD)");
        Serial.println("13 -> Mux IC2, pin 1 (~PROG)");
        */
        Serial.println("22 -> Buffer U2, pin 17");
        Serial.println("23 -> Buffer U1, pin 2");
        Serial.println("24 -> Buffer U1, pin 4");
        Serial.println("25 -> Buffer U1, pin 6");
        Serial.println("26 -> Buffer U1, pin 8");
        Serial.println("27 -> Buffer U1, pin 11");
        Serial.println("28 -> Buffer U1, pin 13");
        Serial.println("29 -> Buffer U1, pin 15");
        Serial.println("30 -> Buffer U1, pin 17");
        Serial.println("31 -> Buffer U8, pin 2");
        Serial.println("32 -> Buffer U8, pin 4");
        Serial.println("33 -> Buffer U8, pin 6");
        Serial.println("34 -> Buffer U8, pin 8");
        Serial.println("35 -> Buffer U8, pin 11");
        Serial.println("36 -> Buffer U8, pin 13");
        Serial.println("37 -> Buffer U8, pin 15");
        Serial.println("38 -> Buffer U8, pin 17");
    
    while (Serial.available() == 0) ;
    if (Serial.available() > 0) {
        bool validInput = true;
        String input = Serial.readString();
        pinNumber = input.toInt();
        if ( (pinNumber >= 2 && pinNumber <= 13) || (pinNumber >= 22 && pinNumber <= 38) ) {
            Serial.print("Testing pin ");
            Serial.print(pinNumber);
            Serial.println(".");
            for (int i = 0; i < 5; i++) {
                Serial.print("Test ");
                Serial.print(i+1);
                Serial.println(" of 5.");
                digitalWrite(pinNumber, HIGH);
                delay(1500);
                digitalWrite(pinNumber, LOW);
                delay(1500);
            }
        }
    }
}
