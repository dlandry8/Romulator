#include "romArd.h"
//------------------------------------------------------------------------
//  File:         testAddress0AAA
//  By:           Team Romulator:
//                David Landry, chief software engineer
//                Biniyam Yemane-Berhane, Nema Karimi, Thien Nguyen
//  For:          CE / BEE Capstone; University of Washington, Bothell
//                Developed for the School of STEM
//  Advisor:      Dr. Arnold S. Berger
//  Date:         August 9, 2019
//  Description:  This program writes $55 to address $0AAAA. This is used
//                    for a DC test to verify that we can write data to
//                    the RAM on the emulator board.
//------------------------------------------------------------------------
void setup() {
    // Set everything high by default.
    for (int i = 2; i <= 13; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    for (int i = 22; i <= 38; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
    // Toggle pins for write mode.
    digitalWrite(PROG, LOW);
    digitalWrite(WRITE, LOW);
    digitalWrite(RD, LOW);
    digitalWrite(SEL, LOW);
    // Toggle address pins for 0AAAA.
    for (int i = 0; i < 17; i++) {
        if ((i % 2) == 0)
            digitalWrite(Addr[i], LOW);
    }
    // Toggle data pins for 55.
    for (int i = 0; i < 8; i++) {
        if ((i % 2) != 0)
            digitalWrite(Data[i], LOW);
    }
}

void loop() {
}
