using System.IO.Ports;
using System.Threading;
using Microsoft.SPOT;
using SecretLabs.NETMF.Hardware.Netduino;
using SerialInterface;
using System.Collections;

namespace NetduinoSerialTest
{
    public class Program
    {
        static SerialPort serial_port;
        

        public static void Main()
        {
            CommPort port = new CommPort(SerialPorts.COM1, BaudRate.Baudrate9600, Parity.None, 8, StopBits.One);
            while (true)
            {
                string text = port.ReadLine();
                if (text.Length != 0)
                {
                    // print the command sent to the NETduino
                    Debug.Print("Command: " + text);
                }
                Thread.Sleep(1000);
            }
        }
    }
}
