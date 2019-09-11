This is the very first test to run on the emulator board.
The tester will toggle pins on the Arduino via serial monitor on the Arduino IDE.
A second tester ensures proper connection on the other side of the trace connected to that port of the Arduino.
    * A logic probe will register high or low on the other end as the Arduino's port is toggled high & low. (i.e., on both the
        Arduino's pinout and the pin where it is connected on the PCB).
    * An ohmmeter will ensure there is virtually no resistance between the Arduino's port and its destination on the board
        when the port is driven low.
