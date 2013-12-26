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
using System.Collections;

namespace Mettle
{
    public partial class dlgSerial : Form
    {
        public dlgSerial()
        {
            InitializeComponent();

            cboBaud.SelectedIndex = 1;
        }

        private void cboPort_DropDown(object sender, EventArgs e)
        {
            cboPort.Items.Clear();

            ArrayList items = new ArrayList();
            items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
            items.Sort();
            cboPort.Items.AddRange(items.ToArray());
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.COMport = cboPort.Text;
            Properties.Settings.Default.BaudRate = int.Parse(cboBaud.Text);
            Properties.Settings.Default.Save();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void dlgSerial_Load(object sender, EventArgs e)
        {
            cboPort.Items.Clear();

            ArrayList items = new ArrayList();
            items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
            items.Sort();
            cboPort.Items.AddRange(items.ToArray());

            cboPort.SelectedText = Properties.Settings.Default.COMport;
        }
    }
}
