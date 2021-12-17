This folder contains all of the programs to upload to the Arduino on the emulator board.

  * testPinsArduino is used for the first test of the board to ensure proper connection between the Arduino ports and the
      destination pins on the connected components.
  * testRAMArduino is an extension of testPinsArduino, used to test whether address and data lines in the RAM can be properly
      toggled. This tests whether the Arduino can successfully toggle the control lines on the RAM (~OE, ~WE, ~CS) and chip
      select on the upper row of address and data buffers. It also tests the connection between the upper row of buffers and
      the address & data lines of the RAM.
  * testAddress0AAAA is a test to determine whether the RAM can be correctly written to. It writes 0x55 (0s and 1s alternating)
      to address 0x0AAAA (also 0s and 1s alternating), which can then be probed by a logic probe.
  * testReadAddr0 is a test to determine whether the RAM can be correctly read from a test system. For this test, the value
      0x55 is written to the ROM Emulator's RAM's address 0x0. A tester sets up a breadboard with a DIP plug connected to the
      ROM Emulator's interface socket, and jumps the address lines for an appropriate-sized ROM chip low as well as ~OE and
      ~CS. ~WE/VCC is jumped high. The breadboard rows corresponding to the data lines may then be probed to check for
      appropriate high & low bits.
  * romArd contains the final program to be loaded to the Arduino.
