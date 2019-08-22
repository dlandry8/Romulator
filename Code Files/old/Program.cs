/*
 * Learning how to use SerialPort - Hari Wiguna, September 2010
 *                             and German Kutsy, May       2013
 * This works with Tera Term VT on COM4 for both IN and OUT
 * The FTDI serial converter is wired to Netduino like this:
 * Netduino     FTDI
 * GND          GND
 * -            CTS
 * 3.3V         3.3V
 * D0 (rx)      TXO (computer tx)
 * D1 (tx)      RXI (computer rx)
 * -            DTR
 */

using System;
using Microsoft.SPOT.Hardware;
using SecretLabs.NETMF.Hardware.Netduino;
using System.Threading;

namespace SerialPortLearn
{
    public class Program
    {
        static OutputPort led = new OutputPort(Pins.ONBOARD_LED, false);
        static InterruptPort interruptPort;
        static int buttonPressCount = 0;

        //Defaults to: SerialPorts.COM1 (uses D0 and D1), 9600, Parity.None, 8, StopBits.One
        static SerialPortHelper serialPortHelper = new SerialPortHelper();

        public static void Main()
        {
            SetupButton(); // sets up serial i/o

            while (true)
            {
                // normally can't read in more than one char at a time:
                // Thus, serialPortHelper class created to read in one line at a time
                string line = serialPortHelper.ReadLine();
                if (line.Length > 0)
                {
                    led.Write(line.IndexOf("ON") == 0);
                }
            }
        }

        private static void SetupButton()
        {
            // serial i/o is setup here
            interruptPort = new InterruptPort(Pins.ONBOARD_SW1, false, Port.ResistorMode.PullUp, Port.InterruptMode.InterruptEdgeLow);
            interruptPort.OnInterrupt += new NativeEventHandler(interruptPort_OnInterrupt);
        }

        static void interruptPort_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            buttonPressCount++;
            //serialPortHelper.PrintClear();
            serialPortHelper.PrintLine("Button pressed " + buttonPressCount.ToString() + " times");
        }
    }
}
