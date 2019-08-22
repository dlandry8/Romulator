using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Velleman;
//using NetDuinoComm;
using System.IO.Ports;
using System.Threading;

namespace RemoteLabSystem
{
    public partial class DBU001 : Form
    {
        RLS_Main main_form;
        Thread serial_thread;
        Boolean serial_active = true;
        Queue<String> queue;

        public DBU001()
        {
            InitializeComponent();
            serial_thread = new Thread(readWriteToSerialPort);
            serial_thread.Start(); // parallel computing at this point
            queue = new Queue<String>();
        }

        public DBU001(RLS_Main mf)
        {
            InitializeComponent();
            main_form = mf; // now able to access methods in the RLS
        }

        private void readWriteToSerialPort()
        {
            while (serial_active)
            {
                Thread.Sleep(200);
                if (queue.Count != 0)
                {
                    // put reading and writing here
                    var port = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);
                    port.Open();
                    port.Write(queue.Dequeue());
                    port.Close();

                    serial_active = false;
                }
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            var v = new VellemanForm();
            v.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox_o1_SelectedIndexChanged(object sender, EventArgs e)
        {
            // assume "select_oscope1" asserted at this point
            queue.Enqueue("select_oscope1");
            
            string tp1_select = "";
            //var port = new SerialPort("COM5", 9600, Parity.None, 8, StopBits.One);

            if (comboBox_o1.Text.Equals("Test Point 1"))
                TP1.BackColor = Color.Green; tp1_select = "test_point1";
            if (comboBox_o1.Text.Equals("Test Point 2"))
                TP2.BackColor = Color.Green; tp1_select = "test_point2";
            if (comboBox_o1.Text.Equals("Test Point 3"))
                TP3.BackColor = Color.Green; tp1_select = "test_point3";
            if (comboBox_o1.Text.Equals("Test Point 4"))
                TP4.BackColor = Color.Green; tp1_select = "test_point4";
            if (comboBox_o1.Text.Equals("Test Point 5"))
                TP5.BackColor = Color.Green; tp1_select = "test_point5";
            if (comboBox_o1.Text.Equals("Test Point 6"))
                TP6.BackColor = Color.Green; tp1_select = "test_point6";
            //if (comboBox_o1.Text.Equals("Test Point 7"))
            //    TP7.BackColor = Color.CadetBlue; tp1_select = "test_point7";
            //if (comboBox_o1.Text.Equals("Test Point 8"))
            //    TP8.BackColor = Color.CadetBlue; tp1_select = "test_point8";

            if (tp1_select != "")
            {
                queue.Enqueue(tp1_select);
            }
            readWriteToSerialPort();
        }

        private void comboBox_o2_SelectedIndexChanged(object sender, EventArgs e)
        {
            queue.Enqueue("select_oscope1");
            string tp2_select = "";

            if (comboBox_o2.Text.Equals("Test Point 1"))
                TP1.BackColor = Color.Green;
            if (comboBox_o2.Text.Equals("Test Point 2"))
                TP2.BackColor = Color.Green;
            if (comboBox_o2.Text.Equals("Test Point 3"))
                TP3.BackColor = Color.Green;
            if (comboBox_o2.Text.Equals("Test Point 4"))
                TP4.BackColor = Color.Green;
            if (comboBox_o2.Text.Equals("Test Point 5"))
                TP5.BackColor = Color.Green;
            if (comboBox_o2.Text.Equals("Test Point 6"))
                TP6.BackColor = Color.Green;

            readWriteToSerialPort();
        }

        private void comboBox_fg_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox_fg.Text.Equals("Test Point 1"))
                TP1.BackColor = Color.Blue;
            if (comboBox_fg.Text.Equals("Test Point 2"))
                TP2.BackColor = Color.Blue;
            if (comboBox_fg.Text.Equals("Test Point 4"))
                TP4.BackColor = Color.Blue;
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void DBU001_Load(object sender, EventArgs e)
        {

        }

        private void textbox_send_TextChanged(object sender, EventArgs e)
        {

        }

        private void DBU001_FormClosed(object sender, FormClosedEventArgs e)
        {
            serial_active = false;
        }
    }
}

//namespace NetDuinoComm
//{
//    class NetDuinoComm
//    {
        
//        //netduinocomm.sendcommand("POT0=" + netvalue);
//        //changepot(potnumber, newvalue);
//    }
//}
