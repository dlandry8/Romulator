using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Globalization;


namespace PCSGU250DLL_Demo
{
    public partial class Form1 : Form
    {
        int[] DataBuffer1 = new int[5000];
        int[] DataBuffer2 = new int[5000];
        NumberFormatInfo provider = new NumberFormatInfo();

        public Form1()
        {
            InitializeComponent();
        }

        [DllImport("PCSGU250.dll")]
        public static extern void ReadCh1(int[] Buffer);

        [DllImport("PCSGU250.dll")]
        public static extern void ReadCh2(int[] Buffer);

        [DllImport("PCSGU250.dll")]
        public static extern bool DataReady();

        [DllImport("PCSGU250.dll")]
        public static extern void Voltage1(int Volts);

        [DllImport("PCSGU250.dll")]
        public static extern void Voltage2(int Volts);

        [DllImport("PCSGU250.dll")]
        public static extern void Time(int Rate);

        [DllImport("PCSGU250.dll")]
        public static extern void RunOn(int Run_on);

        [DllImport("PCSGU250.dll")]
        public static extern void SingleOn(int Single_on);

        [DllImport("PCSGU250.dll")]
        public static extern void YPosition1(int y_pos);

        [DllImport("PCSGU250.dll")]
        public static extern void YPosition2(int y_pos);

        [DllImport("PCSGU250.dll")]
        public static extern void TrgOn(int trg_on);

        [DllImport("PCSGU250.dll")]
        public static extern void TrgLevel(int Trg_Level);   
     
        [DllImport("PCSGU250.dll")]
        public static extern void TrgSource(int Trg_Source);   

        [DllImport("PCSGU250.dll")]
        public static extern void TrgEdge(int Positive_Negative);   

        [DllImport("PCSGU250.dll")]
        public static extern void Coupling1(int AC_DC_GND);   

        [DllImport("PCSGU250.dll")]
        public static extern void Coupling2(int AC_DC_GND);  

        [DllImport("PCSGU250.dll")]
        public static extern void Show_PCSGU250(int Visible);  

        [DllImport("PCSGU250.dll")]
        public static extern bool Start_PCSGU250();  

        [DllImport("PCSGU250.dll")]
        public static extern void Stop_PCSGU250();  

        [DllImport("PCSGU250.dll")]
        public static extern bool GetSettings(int[] SettingsArray);  

        [DllImport("PCSGU250.dll")]
        public static extern void StartGen();  

        [DllImport("PCSGU250.dll")]
        public static extern void StopGen();  

        [DllImport("PCSGU250.dll")]
        public static extern void SetGen(int func, Single freq, Single ampl, Single offset);  

        [DllImport("PCSGU250.dll")]
        public static extern void SetSweep(Single freq1, Single freq2, Single  ampl,Single  offset, Single  time);  

        [DllImport("PCSGU250.dll")]
        public static extern void SetLibWave(Single freq, Single ampl, Single offset, string filename);

        private void Form1_Load(object sender, EventArgs e)
        {
            provider.NumberDecimalSeparator = ".";
            provider.NumberGroupSeparator = ",";
            provider.NumberGroupSizes = new int[] { 3 };
        }

        private void Button1_Click(object sender, EventArgs e)
        {     
            if (Start_PCSGU250())
            {
                CheckBox1.Enabled = true;
                Button5.Enabled = true;
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Stop_PCSGU250();
        }

        private void CheckBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (CheckBox1.Checked)
                RunOn(1);
            else
                RunOn(0);
            Button5.Enabled = !(CheckBox1.Checked);
        }

        private void Button5_Click(object sender, EventArgs e)
        {
            SingleOn(1);
        }

        private void Button8_Click(object sender, EventArgs e)
        {
            int i;
            ReadCh1(DataBuffer1);
            ReadCh2(DataBuffer2);
            ListBox1.Items.Clear();
            ListBox1.Items.Add("Sample rate [Hz]" + (char)9 + DataBuffer1[0].ToString() + (char)9 + DataBuffer2[0].ToString());
            ListBox1.Items.Add("Full scale [mV]" + (char)9 + DataBuffer1[1].ToString() + (char)9 + DataBuffer2[1].ToString());
            ListBox1.Items.Add("GND level [counts]" + (char)9 + DataBuffer1[2].ToString() + (char)9 + DataBuffer2[2].ToString());
            for (i = 0; i < 11; i++)
                ListBox1.Items.Add("Data" + i.ToString()+(char)9 + DataBuffer1[i+3].ToString() + (char)9 + DataBuffer2[i+3].ToString());
        }

        private void Button3_Click(object sender, EventArgs e)
        {
            Show_PCSGU250(1);
        }

        private void Button4_Click(object sender, EventArgs e)
        {
            Show_PCSGU250(0);
        }

        private void Button2_Click(object sender, EventArgs e)
        {
            Stop_PCSGU250();
            CheckBox1.Enabled = false;
            Button5.Enabled = false;
        }

        private void Button7_Click(object sender, EventArgs e)
        {
            int[] SettingsArray = new int[11];
            if (GetSettings(SettingsArray))
            {
                switch (SettingsArray[0])
                {
                    case 0:
                        RadioButton1.Checked = true;
                        break;
                    case 1:
                        RadioButton2.Checked = true;
                        break;
                    case 2:
                        RadioButton3.Checked = true;
                        break;
                    case 3:
                        RadioButton4.Checked = true;
                        break;
                    case 4:
                        RadioButton5.Checked = true;
                        break;
                    case 5:
                        RadioButton6.Checked = true;
                        break;
                }

                switch (SettingsArray[1])
                {
                    case 0:
                        RadioButton12.Checked = true;
                        break;
                    case 1:
                        RadioButton11.Checked = true;
                        break;
                    case 2:
                        RadioButton10.Checked = true;
                        break;
                    case 3:
                        RadioButton9.Checked = true;
                        break;
                    case 4:
                        RadioButton8.Checked = true;
                        break;
                    case 5:
                        RadioButton7.Checked = true;
                        break;
                }
                switch (SettingsArray[2])
                {
                    case 0:
                        m500.Checked = true;
                        break;
                    case 1:
                        m200.Checked = true;
                        break;
                    case 2:
                        m100.Checked = true;
                        break;
                    case 3:
                        m50.Checked = true;
                        break;
                    case 4:
                        m20.Checked = true;
                        break;
                    case 5:
                        m10.Checked = true;
                        break;
                    case 6:
                        m5.Checked = true;
                        break;
                    case 7:
                        m2.Checked = true;
                        break;
                    case 8:
                        m1.Checked = true;
                        break;
                    case 9:
                        m05.Checked = true;
                        break;
                    case 10:
                        m02.Checked = true;
                        break;
                    case 11:
                        m01.Checked = true;
                        break;
                    case 12:
                        u50.Checked = true;
                        break;
                    case 13:
                        u20.Checked = true;
                        break;
                    case 14:
                        u10.Checked = true;
                        break;
                    case 15:
                        u5.Checked = true;
                        break;
                    case 16:
                        u2.Checked = true;
                        break;
                }
                switch (SettingsArray[5])
                {
                    case 0:
                        RadioButton18.Checked = true;
                        break;
                    case 1:
                        RadioButton17.Checked = true;
                        break;
                    case 2:
                        RadioButton16.Checked = true;
                        break;
                }
                switch (SettingsArray[6])
                {
                    case 0:
                        RadioButton15.Checked = true;
                        break;
                    case 1:
                        RadioButton14.Checked = true;
                        break;
                    case 2:
                        RadioButton13.Checked = true;
                        break;
                }
                switch (SettingsArray[7])
                {
                    case 0:
                        RadioButton20.Checked = true;
                        break;
                    case 1:
                        RadioButton21.Checked = true;
                        break;
                }
                switch (SettingsArray[8])
                {
                    case 0:
                        RadioButton22.Checked = true;
                        break;
                    case 1:
                        RadioButton19.Checked = true;
                        break;
                }
                switch (SettingsArray[9])
                {
                    case 0:
                        RadioButton23.Checked = true;
                        break;
                    case 1:
                        RadioButton24.Checked = true;
                        break;
                }
            TrackBar1.Value = -SettingsArray[3];
            TrackBar2.Value = -SettingsArray[4];
            TrackBar3.Value = 254 - SettingsArray[10];
            }
        }

        private void Button6_Click(object sender, EventArgs e)
        {
            if (DataReady())
                Label1.Text = "Yes";
            else
                Label1.Text = "No";
        }

        private void RadioButton1_CheckedChanged(object sender, EventArgs e)
        {
            Voltage1(0);
        }

        private void RadioButton2_CheckedChanged(object sender, EventArgs e)
        {
            Voltage1(1);
        }

        private void RadioButton3_CheckedChanged(object sender, EventArgs e)
        {
            Voltage1(2);
        }

        private void RadioButton4_CheckedChanged(object sender, EventArgs e)
        {
            Voltage1(3);
        }

        private void RadioButton5_CheckedChanged(object sender, EventArgs e)
        {
            Voltage1(4);
        }

        private void RadioButton6_CheckedChanged(object sender, EventArgs e)
        {
            Voltage1(5);
        }

        private void RadioButton12_CheckedChanged(object sender, EventArgs e)
        {
            Voltage2(0);
        }

        private void RadioButton11_CheckedChanged(object sender, EventArgs e)
        {
            Voltage2(1);
        }

        private void RadioButton10_CheckedChanged(object sender, EventArgs e)
        {
            Voltage2(2);
        }

        private void RadioButton9_CheckedChanged(object sender, EventArgs e)
        {
            Voltage2(3);
        }

        private void RadioButton8_CheckedChanged(object sender, EventArgs e)
        {
            Voltage2(4);
        }

        private void RadioButton7_CheckedChanged(object sender, EventArgs e)
        {
            Voltage2(5);
        }

        private void m500_CheckedChanged(object sender, EventArgs e)
        {
            Time(0);
        }

        private void m200_CheckedChanged(object sender, EventArgs e)
        {
            Time(1);
        }

        private void m100_CheckedChanged(object sender, EventArgs e)
        {
            Time(2);
        }

        private void m50_CheckedChanged(object sender, EventArgs e)
        {
            Time(3);
        }

        private void m20_CheckedChanged(object sender, EventArgs e)
        {
            Time(4);
        }

        private void m10_CheckedChanged(object sender, EventArgs e)
        {
            Time(5);
        }

        private void m5_CheckedChanged(object sender, EventArgs e)
        {
            Time(6);
        }

        private void m2_CheckedChanged(object sender, EventArgs e)
        {
            Time(7);
        }

        private void m1_CheckedChanged(object sender, EventArgs e)
        {
            Time(8);
        }

        private void m05_CheckedChanged(object sender, EventArgs e)
        {
            Time(9);
        }

        private void m02_CheckedChanged(object sender, EventArgs e)
        {
            Time(10);
        }

        private void m01_CheckedChanged(object sender, EventArgs e)
        {
            Time(11);
        }

        private void u50_CheckedChanged(object sender, EventArgs e)
        {
            Time(12);
        }

        private void u20_CheckedChanged(object sender, EventArgs e)
        {
            Time(13);
        }

        private void u10_CheckedChanged(object sender, EventArgs e)
        {
            Time(14);
        }

        private void u5_CheckedChanged(object sender, EventArgs e)
        {
            Time(15);
        }

        private void u2_CheckedChanged(object sender, EventArgs e)
        {
            Time(16);
        }

        private void RadioButton18_CheckedChanged(object sender, EventArgs e)
        {
            Coupling1(0);
        }

        private void RadioButton17_CheckedChanged(object sender, EventArgs e)
        {
            Coupling1(1);
        }

        private void RadioButton16_CheckedChanged(object sender, EventArgs e)
        {
            Coupling1(2);
        }

        private void RadioButton15_CheckedChanged(object sender, EventArgs e)
        {
            Coupling2(0);
        }

        private void RadioButton14_CheckedChanged(object sender, EventArgs e)
        {
           Coupling2(1);
        }

        private void RadioButton13_CheckedChanged(object sender, EventArgs e)
        {
           Coupling2(2);
        }

        private void RadioButton21_CheckedChanged(object sender, EventArgs e)
        {
            TrgOn(1);
        }

        private void RadioButton20_CheckedChanged(object sender, EventArgs e)
        {
            TrgOn(0);
        }

        private void RadioButton22_CheckedChanged(object sender, EventArgs e)
        {
            TrgSource(0);
        }

        private void RadioButton19_CheckedChanged(object sender, EventArgs e)
        {
            TrgSource(1);
        }

        private void RadioButton24_CheckedChanged(object sender, EventArgs e)
        {
            TrgEdge(1);
        }

        private void RadioButton23_CheckedChanged(object sender, EventArgs e)
        {
            TrgEdge(0);
        }

        private void TrackBar1_Scroll(object sender, EventArgs e)
        {
            YPosition1(-TrackBar1.Value);
        }

        private void TrackBar2_Scroll(object sender, EventArgs e)
        {
            YPosition2(-TrackBar2.Value);
        }

        private void TrackBar3_Scroll(object sender, EventArgs e)
        {
            TrgLevel(254 - TrackBar3.Value);
        }

        private void RadioButton42_CheckedChanged(object sender, EventArgs e)
        {
            Single freq = Convert.ToSingle(TextBox1.Text, provider);
            Single ampl = Convert.ToSingle(TextBox2.Text, provider);
            Single offset = Convert.ToSingle(TextBox3.Text, provider);
            SetGen(1, freq, ampl, offset);
            StartGen();
        }

        private void RadioButton43_CheckedChanged(object sender, EventArgs e)
        {
            Single freq = Convert.ToSingle(TextBox1.Text, provider);
            Single ampl = Convert.ToSingle(TextBox2.Text, provider);
            Single offset = Convert.ToSingle(TextBox3.Text, provider);
            SetGen(2, freq, ampl, offset);
            StartGen();
        }

        private void RadioButton44_CheckedChanged(object sender, EventArgs e)
        {
            Single freq = Convert.ToSingle(TextBox1.Text, provider);
            Single ampl = Convert.ToSingle(TextBox2.Text, provider);
            Single offset = Convert.ToSingle(TextBox3.Text, provider);
            SetGen(3, freq, ampl, offset);
            StartGen();
        }

        private void RadioButton46_CheckedChanged(object sender, EventArgs e)
        {
            Single freq1 = Convert.ToSingle(TextBox5.Text, provider);
            Single freq2 = Convert.ToSingle(TextBox6.Text, provider);
            Single ampl = Convert.ToSingle(TextBox2.Text, provider);
            Single offset = Convert.ToSingle(TextBox3.Text, provider);
            Single time = Convert.ToSingle(TextBox7.Text, provider);
            SetSweep(freq1, freq2, ampl, offset, time);
            StartGen();
        }

        private void RadioButton45_CheckedChanged(object sender, EventArgs e)
        {
            Single freq = Convert.ToSingle(TextBox1.Text, provider);
            Single ampl = Convert.ToSingle(TextBox2.Text, provider);
            Single offset = Convert.ToSingle(TextBox3.Text, provider);
            SetLibWave(freq, ampl, offset, TextBox4.Text);
            StartGen();
        }

        private void RadioButton47_CheckedChanged(object sender, EventArgs e)
        {
            StopGen();
        }


    }
}
