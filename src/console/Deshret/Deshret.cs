using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Deshret
{
    public partial class Deshret : Form
    {
        public Deshret()
        {
            InitializeComponent();
            this.ActiveControl = textBox1;

            listView1.View = View.Details;
            listView1.AllowColumnReorder = true;
            listView1.FullRowSelect = true;
            listView1.GridLines = true;

            listView1.Columns.Add("Variable", -2, HorizontalAlignment.Left);
            listView1.Columns.Add("Value", -2, HorizontalAlignment.Right);

            listView2.View = View.Details;
            listView2.AllowColumnReorder = true;
            listView2.FullRowSelect = true;
            listView2.GridLines = true;

            listView2.Columns.Add("Timestamp", -2, HorizontalAlignment.Left);
            listView2.Columns.Add("Event", -2, HorizontalAlignment.Right);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.WriteLine(textBox1.Text);
                textBox1.Text = "";
            }
            catch
            {
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Error";
                }));
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                if (comboBox1.SelectedIndex == 0)
                {
                    serialPort1.Close();
                    button2.Invoke(new MethodInvoker(delegate
                    {
                        button2.Text = "Offline";
                    }));
                }
                else
                {
                    serialPort1.Close();
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    button2.Invoke(new MethodInvoker(delegate
                    {
                        button2.Text = "Online";
                    }));
                }
            }
            catch
            {
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Error";
                }));
            }
        }

        private void comboBox1_DropDown(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            comboBox1.Items.Add("Off");
            foreach (string s in SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(s);
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            try
            {
                string line = serialPort1.ReadLine();
                this.BeginInvoke(new LineReceivedEvent(LineReceived), line);
            }
            catch
            {
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Error";
                }));
            }
        }

        private delegate void LineReceivedEvent(string raw);
        private void LineReceived(string raw)
        {
            richTextBox1.Text += raw + "\n";
            string[] lines = raw.Split('#');
            foreach (string line in lines)
            {
                if (line[0] == '$')
                {
                    if (line[1] == 'E')
                    {
                        string eventText = "";
                        string timeStamp = DateTime.Now.ToString();
                        for (int i = 3; i < line.Length; i++)
                            eventText += line[i];
                        ListViewItem item = new ListViewItem(timeStamp, 0);
                        item.SubItems.Add(eventText);
                        listView2.Items.Add(item);
                    }
                    else
                    {
                        string variable = "";
                        string value = "";
                        int i;
                        for (i = 1; line[i] != '$' && i < line.Length; i++)
                            variable += line[i];
                        for (i = i + 1; i < line.Length; i++)
                            value += line[i];

                        for (i = 0; i < listView1.Items.Count; i++)
                        {
                            if (listView1.Items[i].Text == variable)
                            {
                                listView1.Items[i].SubItems[1].Text = value;
                                break;
                            }
                        }
                        if (i == listView1.Items.Count)
                        {
                            ListViewItem item = new ListViewItem(variable, 0);
                            item.SubItems.Add(value);
                            listView1.Items.Add(item);
                        }
                    }
                }
            }
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                if(dataGridView1.Rows[e.RowIndex].Cells[0].Value != null && dataGridView1.Rows[e.RowIndex].Cells[1].Value != null)
                    serialPort1.WriteLine("$" + dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString() + "$" + dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString());
            }
            catch
            {
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Error";
                }));
            }
        }

        private void serialPort1_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            button2.Invoke(new MethodInvoker(delegate
            {
                button2.Text = "Error";
            }));
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (comboBox1.SelectedIndex == 0)
                {
                    serialPort1.Close();
                    button2.Invoke(new MethodInvoker(delegate
                    {
                        button2.Text = "Offline";
                    }));
                }
                else
                {
                    serialPort1.Close();
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    button2.Invoke(new MethodInvoker(delegate
                    {
                        button2.Text = "Online";
                    }));
                }
            }
            catch
            {
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Error";
                }));
            }
        }

        private void Deshret_Load(object sender, EventArgs e)
        {
            try
            {
                serialPort1.BaudRate = 9600;
                serialPort1.PortName = comboBox1.Text;
                serialPort1.Open();
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Online";
                }));
            }
            catch
            {
                button2.Invoke(new MethodInvoker(delegate
                {
                    button2.Text = "Error";
                }));
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            listView1.Items.Clear();
            listView2.Items.Clear();
        }
    }
}
