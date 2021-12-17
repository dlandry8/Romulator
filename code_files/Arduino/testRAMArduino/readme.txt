This is an alternative second test for the ROM emulator board.
This test sets the RAM to write mode (CS low, WE low, OE high) and allows testers to attempt to toggle address and data lines on the RAM.
    * One tester toggles address and data lines on the Arduino.
    * Another tester checks connection with the RAM using a logic probe.
This test is considered obsolete, replaced with testWrite0AAAA. It was included here for posterity.
