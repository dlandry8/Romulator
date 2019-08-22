#include "romArd.h"
//------------------------------------------------------------------------
//  File:         testReadAddr0
//  By:           Team Romulator:
//                David Landry, chief software engineer
//                Biniyam Yemane-Berhane, Nema Karimi, Thien Nguyen
//  For:          CE / BEE Capstone; University of Washington, Bothell
//                Developed for the School of STEM
//  Advisor:      Dr. Arnold S. Berger
//  Date:         August 9, 2019
//  Description:  This program works in two parts. It first writes the
//                    data $55 to address 0 on the RAM. It then puts
//                    the emulator into read mode and tests if a simulated
//                    test board (i.e., plug attached to a breadboard
//                    with address lines grounded, OE & CS grounded, and
//                    WE high) will set the nodes connected to the data
//                    pins high or low.
//------------------------------------------------------------------------
void setup() {
    //------------------------------------------------------------
    //  Part 1:   Set up defaults
    //------------------------------------------------------------
    // Set everything high by default.
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    for (int i = 22; i <= 38; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }

    //------------------------------------------------------------
    //  Part 2:   Write $55 to address $0
    //------------------------------------------------------------
    // Toggle pins for write mode.
    digitalWrite(PROG, LOW);
    digitalWrite(WRITE, LOW);
    digitalWrite(RD, LOW);
    digitalWrite(SEL, LOW);
    // Toggle address pins for $0.
    for (int i = 0; i < 17; i++) {
        digitalWrite(Addr[i], LOW);
    }
    // Write address $55 to the address
    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0)
            digitalWrite(Data[i], HIGH);
        else 
            digitalWrite(Data[i], LOW);
    }
    delayMicroseconds(1);

    //------------------------------------------------------------
    //  Part 3: Read the data in the address via the breadboard
    //------------------------------------------------------------
    // Give control of memory to the interface:
    digitalWrite(PROG, HIGH);
    digitalWrite(WRITE, HIGH);
    digitalWrite(RD, HIGH);
    // The other end should be attached to a breadboard with all
    //    address lines going to ground. Jump WRITE high,
    //    OE and CS to ground on the breadboard.

}

void loop() {
  // put your main code here, to run repeatedly:

}
