testAddress0AAA.ino is a test sketch for the Arduino Mega. It attempts to write 0x55 to the RAM's address 0x0AAAA.
This test verifies whether we can successfully write data to the RAM.
Successful test:
  * The upper row of address and data buffers should be on.
      * Their chip select should be low.
      * There should be virtually no resistance between each input and output line.
  * The lower row of address and data buffers should be off.
      * Their chip select should be high.
      * Resistance between input and output should read open/OL.
  * RAM's Output Enable should be high. Write Enable and Chip Select should be low.
  * The RAM's address lines should match 0x0AAAA.
  * The RAM's data lines should match 0x55.
External tools:
  * Ohmmeter.
  * Logic probe.
