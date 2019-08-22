using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Velleman;
using System.IO.Ports;
using System.Threading;

namespace RemoteLabSystem
{
    
    public partial class RLS_Main : Form
    {
        // define globals:
        
        //static SerialPort serial;

        public RLS_Main()
        {
            InitializeComponent();
        }

        private void RLS_Main_Load(object sender, EventArgs e)
        {
            //serial = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
            //serial.Open();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //if ()
            //{

            //}
            var dbu = new DBU001();
            dbu.Show();
            //encapsulateSerial();
        }

        //static void encapsulateSerial()
        //{
        //    // Usage Info for user
        //    System.Console.WriteLine("Enter some text and hit ENTER:");
        //    System.Console.WriteLine("Type 'x' and hit ENTER to exit!");
        //    System.Console.WriteLine();

        //    // initialize serial to be COM5
        //    serial = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
        //    // open port to send-receive data
        //    serial.Open();
        //    // add event-handler for handling incoming data
        //    serial.DataReceived += new SerialDataReceivedEventHandler(serialDataReceived);

        //}
        //static void serialDataReceived(object sender, SerialDataReceivedEventArgs e)
        //{
        //    // wait 100 ms for buffer to fill
        //    System.Threading.Thread.Sleep(100);

        //    // create an array for incoming bytes
        //    byte[] bytes = new byte[serial.BytesToRead];
        //    // read the bytes
        //    serial.Read(bytes, 0, bytes.Length);
        //    // convert bytes into a string
        //    string str = System.Text.Encoding.UTF8.GetString(bytes);

        //    // write rcvd bytes, as a string, to console
        //    System.Console.WriteLine("echo: " + str);
        //    System.Console.WriteLine();
        //}

        private void button_serialTest_Click(object sender, EventArgs e)
        {


            var port = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
            port.Open();
            port.Write("get_board_id\r");
            port.Close();
            //serial = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
            //serial.Open();
            //serial.Write("get_board_id");
            //serial.Close();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
