//Mettle, an embedded software analysis tool
//Copyright (C) 2013  Keith Vasilakes
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.IO.Ports;
//using System.Collections;
using System.Diagnostics;
//using System.Threading;
using MettleLib;

//
// This program is designed to recieve tagged data from an embedded device over a serial connection. RS232 or USB
// a tag consists of a name and data. When a tag is recieved, it is ent to all the custom controls on all the tabs
// Each controll then looks at the tag name to decide if they are interested in it. If they are they can get the data and display it
// If a tag contains a valid integer, the integer is available as well as the string representation.
//A tag can use the name and the data or just the name or anything it wants to do with the data when it recieves the event.
//
//To use this program, just drag one of the custom controls onto a tab, set the tag property to what you want it to respond to and 
//set the name for controls like tagstate. All other properties are available for use as a normal program.
//Then run the program and watch the magic!
//
namespace Mettle
{
    public partial class FormMain : Form
    {
        string RxString = string.Empty;

        MettleHead myMettle = new MettleHead();

        //keep track of the unique tags
        private Module SelectedModule = null;
        private List<Module> ModuleList = new List<Module>();

        public FormMain()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            myMettle.FindControlls(this);

            myMettle.TagLine += new MettleHead.TagLIneHandeler(NewLIneRecieved);
            myMettle.TagEvents += new MettleHead.TagHandeler(MyUpdate);
            myMettle.TagErrorEvent +=new MettleHead.ErrorHandeler(myErrorHandeler);
        }

        private void NewLIneRecieved(string s)
        {
            AppendLineTextBox(s + "\n");
        }

        public void AppendLineTextBox(string value)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AppendLineTextBox), new object[] { value });
                return;
            }
            txtAllText.AppendText(value);
            txtAllText.ScrollToCaret();
        }

        //update the list of unique tags
        private void MyUpdate(TagEvent e)
        {
            bool ModuleNameFound = false;

            //Search to see if tag exists
            foreach (Module m in ModuleList)
            {
                if (m.ModuleName == e.ModuleName)
                {
                    ModuleNameFound = true;

                    //module found, add tag or data if unique
                    m.Uniques(e);
                }
            }

            if (false == ModuleNameFound)
            {
                ModuleList.Add(new Module(e));

                ModuleList.Sort();

                //we have a new tag, redisplay them all
                ClearModuleTextBox('d');

                foreach (Module m in ModuleList)
                {
                    Trace.WriteLine(m.ModuleName + "\n");
                    AppendModuleTextBox(m.ModuleName + "\r\n");
                }
            }
        }

        public void ClearModuleTextBox(char c)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<char>(ClearModuleTextBox), new object[] { c });
                return;
            }
            txtModules.Clear();
        }

        public void AppendModuleTextBox(string value)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AppendModuleTextBox), new object[] { value });
                return;
            }
            txtModules.AppendText(value);
            txtModules.ScrollToCaret();
        }

        //Display any errors
        private void myErrorHandeler(string s)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(myErrorHandeler), new object[] { s });
                return;
            }
            stripError.Text = "Error! " + s;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
           if( myMettle.Open(Properties.Settings.Default.COMport.ToString(), Properties.Settings.Default.BaudRate))
                stripStatus.Text = "Running";
        }

        private void btnPlay_Click(object sender, EventArgs e)
        {
            if (myMettle.Open(Properties.Settings.Default.COMport.ToString(), Properties.Settings.Default.BaudRate))
                stripStatus.Text = "Running";
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            myMettle.Close();

            stripStatus.Text = "Stopped";
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            myMettle.Close();
            stripStatus.Text = "Stopped";
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            myMettle.Close();
            stripStatus.Text = "Stopped";
        }

        private void btnTools_Click(object sender, EventArgs e)
        {
            dlgSerial SetupDialog = new dlgSerial();

            SetupDialog.StartPosition = FormStartPosition.CenterParent;

            SetupDialog.ShowDialog(this);

        }

        private void textUniques_MouseClick(object sender, MouseEventArgs e)
        {
            txtTagData.Clear();

            if (txtUniques.Text.Length > 0)
            {
                int charindex = txtUniques.GetCharIndexFromPosition(e.Location);
                int line = txtUniques.GetLineFromCharIndex(charindex);

                string sx = txtUniques.Lines[line];

                foreach (Tags t in SelectedModule.TagList)
                {
                    if (t.Name == sx)
                    {
                        if (t.ValueValid)
                        {
                            txtTagData.AppendText("Max; " + t.max + "\n");
                            txtTagData.AppendText("Min; " + t.min + "\n");
                        }
                        else
                        {
                            //Display all data from tag
                            foreach (string s in t.Data)
                            {
                                txtTagData.AppendText(s + "\n");
                            }
                        }
                    }
                }
            }
        }

        private void txtModules_MouseClick(object sender, MouseEventArgs e)
        {
            txtUniques.Clear();
            txtTagData.Clear();


            if (txtModules.Text.Length > 0)
            {
                int charindex = txtModules.GetCharIndexFromPosition(e.Location);
                int line = txtModules.GetLineFromCharIndex(charindex);

                if (line >= 0)
                {
                    string sx = txtModules.Lines[line];

                    foreach (Module m in ModuleList)
                    {
                        if (m.ModuleName == sx)
                        {
                            SelectedModule = m;

                            foreach (Tags t in m.TagList)
                            {
                                txtUniques.AppendText(t.Name + "\n");
                            }
                        }
                    }
                }
            }
        }

        private void btnAbout_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Embedded Monitoring Tool; V1.0\nCopyright 2013 Keith Vasilakes\n\nLicensed under GPL\nhttp://www.gnu.org/licenses", "Embedded Monitor");
        }

        private void BtnReset_Click(object sender, EventArgs e)
        {
            myMettle.Reset(this);

            ModuleList.Clear();

            txtAllText.Clear();
            txtModules.Clear();
            txtUniques.Clear();
            txtTagData.Clear();
        }
    }
}
