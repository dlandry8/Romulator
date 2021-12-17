/******************************************************************************
C# Experimental Serial Program

This program attempts to do some reading and writing with the serial port
using C# instead of C++. Code adapted from Microsoft's .NET site, specifically
the page on the SerialPort Class found at 
https://docs.microsoft.com/en-us/dotnet/api/system.io.ports.serialport?view=netframework-4.8
******************************************************************************/
using System;
using System.IO.Ports;
using System.Threading;
using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.Win32;

namespace Romulator {
	
	class Program {
		
		static SerialPort romulatorPort;
		
		public static void Main(string[] args) {
			// Establish our serial port.
			romulatorPort = new SerialCommunications();
		}
		
		
		
	}
	
	
	public class SerialCommunications
    {
		
		static SerialPort serialPort;
		
		public SerialCommunications(string portName = SerialPorts.COM5, int baudRate = 9600, Parity parity = Parity.None, int dataBits = 8, StopBits stopBits = StopBits.One)
        {
            serialPort = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
			string romCom = findPortName();
			serialPort.PortName = romCom;
            serialPort.ReadTimeout = 10; // Set to 10ms. Default is -1?!
            serialPort.DataReceived += new SerialDataReceivedEventHandler(serialPort_DataReceived);
            serialPort.Open();
        }
		
		
		
		
		
		public static string findPortName()
        {
            // Get a list of serial port names.
            string[] ports = SerialPort.GetPortNames();

            // Display each port name to the console.
			int portCounter = 0;
			string portName = "";
            foreach(string port in ports)
            {
				portCounter++;
				if (portCounter == 1)
					portName = String.Copy(port);
				else
					Console.WriteLine("There are more than one attached ports.");
				portCounter++;
            }
			return portName;
        }
		
		public static int setBaudRate(int default) {
			
		}
		
		
		
    }
	/*
	
	public class PortTest {
		static SerialPort mySerialPort;
		
		public static void Main() {
			mySerialPort = new SerialPort();
			
			// Set the serial port's properties
			mySerialPort.PortName = SetPortName();
			
		}
		
		string SetPortName() {
			string[] ports = SerialPort.GetPortNames();
			
		}
	}
	*/
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


