using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace gogoboardcs
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM3";
            serialPort1.Open();
            byte[] byte_array = new byte[1];
            byte_array[0] = 84;
            serialPort1.Write(byte_array, 0, 1);
            byte_array[0] = 254;
            serialPort1.Write(byte_array, 0, 1);
            byte_array[0] = 196;
            serialPort1.Write(byte_array, 0, 1);
            byte_array[0] = 0;
            serialPort1.Write(byte_array, 0, 1);
            serialPort1.Close();
        }
    }
}
