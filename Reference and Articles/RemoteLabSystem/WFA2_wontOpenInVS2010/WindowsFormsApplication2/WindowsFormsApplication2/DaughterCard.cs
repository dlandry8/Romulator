using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    public partial class DaughterCard : Form
    {
        public DaughterCard()
        {
            InitializeComponent();
        }

        private void DaughterCard_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox_o1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox_o1.Text.Equals("Test Point 1"))
                TP1.BackColor = Color.Red;
            if (comboBox_o1.Text.Equals("Test Point 2"))
                TP2.BackColor = Color.Red;
            if (comboBox_o1.Text.Equals("Test Point 3"))
                TP3.BackColor = Color.Red;
            if (comboBox_o1.Text.Equals("Test Point 4"))
                TP4.BackColor = Color.Red;
            if (comboBox_o1.Text.Equals("Test Point 5"))
                TP5.BackColor = Color.Red;
            if (comboBox_o1.Text.Equals("Test Point 6"))
                TP6.BackColor = Color.Red;
        }

        private void comboBox_fg_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox_fg.Text.Equals("FG Point 1"))
                FG1.BackColor = Color.DarkRed;
            if (comboBox_fg.Text.Equals("FG Point 2"))
                FG2.BackColor = Color.DarkRed;
        }

        private void comboBox_o2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox_o2.Text.Equals("Test Point 1"))
                TP1.BackColor = Color.DarkOrange;
            if (comboBox_o2.Text.Equals("Test Point 2"))
                TP2.BackColor = Color.DarkOrange;
            if (comboBox_o2.Text.Equals("Test Point 3"))
                TP3.BackColor = Color.DarkOrange;
            if (comboBox_o2.Text.Equals("Test Point 4"))
                TP4.BackColor = Color.DarkOrange;
            if (comboBox_o2.Text.Equals("Test Point 5"))
                TP5.BackColor = Color.DarkOrange;
            if (comboBox_o2.Text.Equals("Test Point 6"))
                TP6.BackColor = Color.DarkOrange;
        }

    }
}
