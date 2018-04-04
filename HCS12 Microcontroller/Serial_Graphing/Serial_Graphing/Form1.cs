using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO.Ports;
using System.Threading;


//TRY THE STRING BUILDER!!!


namespace Serial_Graphing
{


    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        SerialPort myPort;

        int xCurr=0;
        char[] rawCharInput = new char[12];
        int[] rawInput = new int[5000];

        //timer variables..
        int total;
        int totalBeats;
        int sumofInput;
        //random stupid stuff;
        int byteLength;

        StringBuilder sb = new System.Text.StringBuilder();

        string rawString = "";

        Queue<double> myList = new Queue<double>();

        private void button1_Click(object sender, EventArgs e)
        {
            myPort = new SerialPort("COM4", 38400, Parity.None, 8);
            myPort.Encoding = System.Text.Encoding.ASCII;

            myPort.Open();    

            var thread1 = new Thread(commThread); 
            thread1.Start();

            button1.Enabled = false;
        }

        void commThread()
        {
            Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisY.Minimum = 1.58));
            Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisY.Maximum = 1.73));
            Invoke(new Action(() => label1.Text = "START"));

            while (true)
            {
                while (myPort.BytesToRead >= 1)
                {
                    Invoke(new Action(() => label1.Text = myPort.BytesToRead.ToString()));

                    sb.Clear();
                    byteLength = myPort.BytesToRead;
                    char[] rawChar = new char[byteLength];
                    myPort.Read(rawChar, 0, byteLength);

                    sb.Append(rawChar);
                    rawString = rawString + sb.ToString();


                    while (rawString.IndexOf(',') >= 0)
                    {
                        int divisorIndex = rawString.IndexOf(',');
                        string val = rawString.Substring(0, divisorIndex);
                        if (val.Length > 0)
                        {
                            if (Int32.Parse(val) < 200)
                            {
                                totalBeats = Int32.Parse(val);
                            }
                            else
                            {
                                if (checkBox1.Checked == true)
                                {
                                    myList.Enqueue((Double.Parse(val) / 4096) * 5);
                                }
                                else
                                {
                                    myList.Enqueue(Double.Parse(val));
                                }                             
                              }
                        }
                        rawString = rawString.Substring(divisorIndex + 1);
                    }

                    xCurr++;
               
                    if (xCurr > 5)
                    {
                        Invoke(new Action(() => MC_Analog.Series["Pulse"].Points.SuspendUpdates()));

                        for (int i = 0; i< myList.Count; i++)
                        {
                            Invoke(new Action(() => MC_Analog.Series["Pulse"].Points.AddY(myList.Dequeue())));
                            total++;
                        }
                        myList.Clear();

                        if (total > 500)
                        {
                            Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisX.Minimum = total-500));
                            Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisX.Maximum= total));
                        }                        
                        Invoke(new Action(() => MC_Analog.Series["Pulse"].Points.ResumeUpdates()));
                        Invoke(new Action(() => label4.Text = "BPM: " + totalBeats.ToString()));
                        xCurr = 0;
                    }                   
                }
            }
        }

        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisY.Minimum = 1.58));
                Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisY.Maximum = 1.73));
                Invoke(new Action(() => label7.Text = "Analog Input (Voltage)"));
            }
            else
            {
                Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisY.Minimum = 1300));
                Invoke(new Action(() => MC_Analog.ChartAreas[0].AxisY.Maximum = 1400));
                Invoke(new Action(() => label7.Text = "Analog Input (Bits)"));
            }
        }

    }
}
