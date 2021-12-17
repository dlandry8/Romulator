This is the third and final preliminary read/write test for the ROM emulator. This test incorporates one of the ribbon cables attached to
a proto board. To run this test:
  * Connect a 28-line cable from the emulator's interface socket to a proto board.
  * Reference the schematic of a typical 28-pin EPROM chip. Jump the corresponding address pins of the plug attached to the proto board
      to ground.
  * Jump the appropriate control lines high or low according to specifications:
      - OE should be low
      - CS should be low
  * Upload testReadAddr0.ino
  * The Arduino will begin by writing address 0x0 and data 0x55 to the RAM.
  * It will then set OE low and WE high. It will also toggle the buffers to turn the lower buffers on. This will grant the proto board
      read access to the RAM and cut off the Arduino's access to the RAM.
  * A second tester should now probe the corresponding data lines on the proto board. Are the correct data bits high or low?
  
Note: When we ran this test, we found that our lower address buffers were installed incorrectly. Please reference our schematic and corrected
  schematic as well as the pinout for the LS244 buffers for details.
