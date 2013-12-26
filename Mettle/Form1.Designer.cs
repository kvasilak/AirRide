namespace Mettle
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.tabMain = new System.Windows.Forms.TabControl();
            this.tabChart = new System.Windows.Forms.TabPage();
            this.tagText8 = new MettleLib.TagText();
            this.tagText2 = new MettleLib.TagText();
            this.tagIO8 = new MettleLib.TagIO();
            this.tagIO7 = new MettleLib.TagIO();
            this.tagIO6 = new MettleLib.TagIO();
            this.tagIO5 = new MettleLib.TagIO();
            this.tagIO4 = new MettleLib.TagIO();
            this.tagText7 = new MettleLib.TagText();
            this.tagText6 = new MettleLib.TagText();
            this.tagText5 = new MettleLib.TagText();
            this.tagChart1 = new MettleLib.TagChart();
            this.tagGauge2 = new MettleLib.TagGauge();
            this.tagText4 = new MettleLib.TagText();
            this.tagText3 = new MettleLib.TagText();
            this.tagIO3 = new MettleLib.TagIO();
            this.tagIO2 = new MettleLib.TagIO();
            this.tagIO1 = new MettleLib.TagIO();
            this.tagState1 = new MettleLib.TagState();
            this.tagText1 = new MettleLib.TagText();
            this.tagGauge1 = new MettleLib.TagGauge();
            this.tabText = new System.Windows.Forms.TabPage();
            this.txtAllText = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtModules = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtTagData = new System.Windows.Forms.TextBox();
            this.txtUniques = new System.Windows.Forms.TextBox();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.btnPlay = new System.Windows.Forms.ToolStripButton();
            this.btnStop = new System.Windows.Forms.ToolStripButton();
            this.btnTools = new System.Windows.Forms.ToolStripButton();
            this.btnAbout = new System.Windows.Forms.ToolStripButton();
            this.BtnReset = new System.Windows.Forms.ToolStripButton();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.stripStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.stripError = new System.Windows.Forms.ToolStripStatusLabel();
            this.tagState2 = new MettleLib.TagState();
            this.tagState3 = new MettleLib.TagState();
            this.tabMain.SuspendLayout();
            this.tabChart.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tagChart1)).BeginInit();
            this.tabText.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart2)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabMain
            // 
            this.tabMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabMain.Controls.Add(this.tabChart);
            this.tabMain.Controls.Add(this.tabText);
            this.tabMain.Location = new System.Drawing.Point(0, 28);
            this.tabMain.Name = "tabMain";
            this.tabMain.SelectedIndex = 0;
            this.tabMain.Size = new System.Drawing.Size(1039, 542);
            this.tabMain.TabIndex = 0;
            // 
            // tabChart
            // 
            this.tabChart.BackColor = System.Drawing.SystemColors.Control;
            this.tabChart.Controls.Add(this.tagState3);
            this.tabChart.Controls.Add(this.tagState2);
            this.tabChart.Controls.Add(this.tagText8);
            this.tabChart.Controls.Add(this.tagText2);
            this.tabChart.Controls.Add(this.tagIO8);
            this.tabChart.Controls.Add(this.tagIO7);
            this.tabChart.Controls.Add(this.tagIO6);
            this.tabChart.Controls.Add(this.tagIO5);
            this.tabChart.Controls.Add(this.tagIO4);
            this.tabChart.Controls.Add(this.tagText7);
            this.tabChart.Controls.Add(this.tagText6);
            this.tabChart.Controls.Add(this.tagText5);
            this.tabChart.Controls.Add(this.tagChart1);
            this.tabChart.Controls.Add(this.tagGauge2);
            this.tabChart.Controls.Add(this.tagText4);
            this.tabChart.Controls.Add(this.tagText3);
            this.tabChart.Controls.Add(this.tagIO3);
            this.tabChart.Controls.Add(this.tagIO2);
            this.tabChart.Controls.Add(this.tagIO1);
            this.tabChart.Controls.Add(this.tagState1);
            this.tabChart.Controls.Add(this.tagText1);
            this.tabChart.Controls.Add(this.tagGauge1);
            this.tabChart.Location = new System.Drawing.Point(4, 22);
            this.tabChart.Name = "tabChart";
            this.tabChart.Padding = new System.Windows.Forms.Padding(3);
            this.tabChart.Size = new System.Drawing.Size(1031, 516);
            this.tabChart.TabIndex = 1;
            this.tabChart.Tag = "Right";
            this.tabChart.Text = "Chart";
            // 
            // tagText8
            // 
            this.tagText8.AcceptsReturn = true;
            this.tagText8.AcceptsTab = true;
            this.tagText8.Location = new System.Drawing.Point(336, 21);
            this.tagText8.Module = "Main";
            this.tagText8.Multiline = true;
            this.tagText8.Name = "tagText8";
            this.tagText8.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tagText8.Size = new System.Drawing.Size(100, 197);
            this.tagText8.Sort = "LRHeightFilt";
            this.tagText8.TabIndex = 26;
            this.tagText8.Tag = "LRHeightFilt";
            // 
            // tagText2
            // 
            this.tagText2.AcceptsReturn = true;
            this.tagText2.AcceptsTab = true;
            this.tagText2.Location = new System.Drawing.Point(230, 21);
            this.tagText2.Module = "Main";
            this.tagText2.Multiline = true;
            this.tagText2.Name = "tagText2";
            this.tagText2.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tagText2.Size = new System.Drawing.Size(100, 197);
            this.tagText2.Sort = "RRHeight";
            this.tagText2.TabIndex = 25;
            this.tagText2.Tag = "RRHeight";
            // 
            // tagIO8
            // 
            this.tagIO8.Checked = false;
            this.tagIO8.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.tagIO8.Location = new System.Drawing.Point(577, 122);
            this.tagIO8.Module = "Module2";
            this.tagIO8.Name = "tagIO8";
            this.tagIO8.Size = new System.Drawing.Size(23, 23);
            this.tagIO8.Sort = "Solenoid";
            this.tagIO8.TabIndex = 24;
            this.tagIO8.Tag = "Solenoid";
            this.tagIO8.U16_Mask = ((ushort)(16));
            // 
            // tagIO7
            // 
            this.tagIO7.Checked = false;
            this.tagIO7.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.tagIO7.Location = new System.Drawing.Point(577, 93);
            this.tagIO7.Module = "Module2";
            this.tagIO7.Name = "tagIO7";
            this.tagIO7.Size = new System.Drawing.Size(23, 23);
            this.tagIO7.Sort = "Solenoid";
            this.tagIO7.TabIndex = 23;
            this.tagIO7.Tag = "Solenoid";
            this.tagIO7.U16_Mask = ((ushort)(8));
            // 
            // tagIO6
            // 
            this.tagIO6.Checked = false;
            this.tagIO6.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.tagIO6.Location = new System.Drawing.Point(577, 65);
            this.tagIO6.Module = "Module2";
            this.tagIO6.Name = "tagIO6";
            this.tagIO6.Size = new System.Drawing.Size(23, 23);
            this.tagIO6.Sort = "Solenoid";
            this.tagIO6.TabIndex = 22;
            this.tagIO6.Tag = "Solenoid";
            this.tagIO6.U16_Mask = ((ushort)(4));
            // 
            // tagIO5
            // 
            this.tagIO5.Checked = false;
            this.tagIO5.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.tagIO5.Location = new System.Drawing.Point(577, 37);
            this.tagIO5.Module = "Module2";
            this.tagIO5.Name = "tagIO5";
            this.tagIO5.Size = new System.Drawing.Size(23, 23);
            this.tagIO5.Sort = "Solenoid";
            this.tagIO5.TabIndex = 21;
            this.tagIO5.Tag = "Solenoid";
            this.tagIO5.U16_Mask = ((ushort)(2));
            // 
            // tagIO4
            // 
            this.tagIO4.Checked = false;
            this.tagIO4.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.tagIO4.Location = new System.Drawing.Point(577, 8);
            this.tagIO4.Module = "Module2";
            this.tagIO4.Name = "tagIO4";
            this.tagIO4.Size = new System.Drawing.Size(23, 23);
            this.tagIO4.Sort = "Solenoid";
            this.tagIO4.TabIndex = 20;
            this.tagIO4.Tag = "Solenoid";
            this.tagIO4.U16_Mask = ((ushort)(1));
            // 
            // tagText7
            // 
            this.tagText7.AcceptsReturn = true;
            this.tagText7.AcceptsTab = true;
            this.tagText7.Location = new System.Drawing.Point(703, 79);
            this.tagText7.Module = "Module2";
            this.tagText7.Name = "tagText7";
            this.tagText7.Size = new System.Drawing.Size(80, 20);
            this.tagText7.Sort = "LED3";
            this.tagText7.TabIndex = 19;
            // 
            // tagText6
            // 
            this.tagText6.AcceptsReturn = true;
            this.tagText6.AcceptsTab = true;
            this.tagText6.Location = new System.Drawing.Point(703, 50);
            this.tagText6.Module = "Module2";
            this.tagText6.Name = "tagText6";
            this.tagText6.Size = new System.Drawing.Size(80, 20);
            this.tagText6.Sort = "LED2";
            this.tagText6.TabIndex = 18;
            // 
            // tagText5
            // 
            this.tagText5.AcceptsReturn = true;
            this.tagText5.AcceptsTab = true;
            this.tagText5.Location = new System.Drawing.Point(563, 151);
            this.tagText5.Module = "Module2";
            this.tagText5.Name = "tagText5";
            this.tagText5.Size = new System.Drawing.Size(45, 20);
            this.tagText5.Sort = "Solenoid";
            this.tagText5.TabIndex = 17;
            this.tagText5.Text = " ";
            // 
            // tagChart1
            // 
            this.tagChart1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.AxisX.Maximum = 100D;
            chartArea1.AxisX.Minimum = 0D;
            chartArea1.AxisY.Maximum = 768D;
            chartArea1.AxisY.Minimum = 0D;
            chartArea1.Name = "ChartArea1";
            this.tagChart1.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.tagChart1.Legends.Add(legend1);
            this.tagChart1.Location = new System.Drawing.Point(3, 259);
            this.tagChart1.Module = "Module2";
            this.tagChart1.Name = "tagChart1";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series1.Legend = "Legend1";
            series1.Name = "XAxis";
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series2.Legend = "Legend1";
            series2.Name = "YAxis";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series3.Legend = "Legend1";
            series3.Name = "ZAxis";
            this.tagChart1.Series.Add(series1);
            this.tagChart1.Series.Add(series2);
            this.tagChart1.Series.Add(series3);
            this.tagChart1.Size = new System.Drawing.Size(1028, 235);
            this.tagChart1.TabIndex = 16;
            this.tagChart1.Text = "tagChart1";
            // 
            // tagGauge2
            // 
            this.tagGauge2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.tagGauge2.BaseArcColor = System.Drawing.Color.Gray;
            this.tagGauge2.BaseArcRadius = 150;
            this.tagGauge2.BaseArcStart = 35;
            this.tagGauge2.BaseArcSweep = -70;
            this.tagGauge2.BaseArcWidth = 2;
            this.tagGauge2.Cap_Idx = ((byte)(1));
            this.tagGauge2.CapColors = new System.Drawing.Color[] {
        System.Drawing.Color.Black,
        System.Drawing.Color.Black,
        System.Drawing.Color.Black,
        System.Drawing.Color.Black,
        System.Drawing.Color.Black};
            this.tagGauge2.CapPosition = new System.Drawing.Point(10, 10);
            this.tagGauge2.CapsPosition = new System.Drawing.Point[] {
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10)};
            this.tagGauge2.CapsText = new string[] {
        "",
        "",
        "",
        "",
        ""};
            this.tagGauge2.CapText = "";
            this.tagGauge2.Center = new System.Drawing.Point(-80, 110);
            this.tagGauge2.Location = new System.Drawing.Point(904, 6);
            this.tagGauge2.MaxValue = 1024F;
            this.tagGauge2.MinValue = 0F;
            this.tagGauge2.Module = "Module2";
            this.tagGauge2.Name = "tagGauge2";
            this.tagGauge2.NeedleColor1 = MettleLib.TagGauge.NeedleColorEnum.Red;
            this.tagGauge2.NeedleColor2 = System.Drawing.Color.DimGray;
            this.tagGauge2.NeedleRadius = 160;
            this.tagGauge2.NeedleType = 0;
            this.tagGauge2.NeedleWidth = 2;
            this.tagGauge2.Range_Idx = ((byte)(1));
            this.tagGauge2.RangeColor = System.Drawing.Color.Red;
            this.tagGauge2.RangeEnabled = false;
            this.tagGauge2.RangeEndValue = 400F;
            this.tagGauge2.RangeInnerRadius = 70;
            this.tagGauge2.RangeOuterRadius = 80;
            this.tagGauge2.RangesColor = new System.Drawing.Color[] {
        System.Drawing.Color.LightGreen,
        System.Drawing.Color.Red,
        System.Drawing.SystemColors.Control,
        System.Drawing.SystemColors.Control,
        System.Drawing.SystemColors.Control};
            this.tagGauge2.RangesEnabled = new bool[] {
        false,
        false,
        false,
        false,
        false};
            this.tagGauge2.RangesEndValue = new float[] {
        300F,
        400F,
        0F,
        0F,
        0F};
            this.tagGauge2.RangesInnerRadius = new int[] {
        70,
        70,
        70,
        70,
        70};
            this.tagGauge2.RangesOuterRadius = new int[] {
        80,
        80,
        80,
        80,
        80};
            this.tagGauge2.RangesStartValue = new float[] {
        -100F,
        300F,
        0F,
        0F,
        0F};
            this.tagGauge2.RangeStartValue = 300F;
            this.tagGauge2.ScaleLinesInterColor = System.Drawing.Color.Red;
            this.tagGauge2.ScaleLinesInterInnerRadius = 145;
            this.tagGauge2.ScaleLinesInterOuterRadius = 150;
            this.tagGauge2.ScaleLinesInterWidth = 2;
            this.tagGauge2.ScaleLinesMajorColor = System.Drawing.Color.Black;
            this.tagGauge2.ScaleLinesMajorInnerRadius = 140;
            this.tagGauge2.ScaleLinesMajorOuterRadius = 150;
            this.tagGauge2.ScaleLinesMajorStepValue = 128F;
            this.tagGauge2.ScaleLinesMajorWidth = 2;
            this.tagGauge2.ScaleLinesMinorColor = System.Drawing.Color.Gray;
            this.tagGauge2.ScaleLinesMinorInnerRadius = 145;
            this.tagGauge2.ScaleLinesMinorNumOf = 9;
            this.tagGauge2.ScaleLinesMinorOuterRadius = 150;
            this.tagGauge2.ScaleLinesMinorWidth = 1;
            this.tagGauge2.ScaleNumbersColor = System.Drawing.Color.Black;
            this.tagGauge2.ScaleNumbersFormat = null;
            this.tagGauge2.ScaleNumbersRadius = 162;
            this.tagGauge2.ScaleNumbersRotation = 90;
            this.tagGauge2.ScaleNumbersStartScaleLine = 1;
            this.tagGauge2.ScaleNumbersStepScaleLines = 2;
            this.tagGauge2.Size = new System.Drawing.Size(114, 222);
            this.tagGauge2.Sort = "YAxis";
            this.tagGauge2.TabIndex = 15;
            this.tagGauge2.Tag = "YAxis";
            this.tagGauge2.Text = "tagGauge2";
            this.tagGauge2.Value = 0F;
            // 
            // tagText4
            // 
            this.tagText4.AcceptsReturn = true;
            this.tagText4.AcceptsTab = true;
            this.tagText4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tagText4.Location = new System.Drawing.Point(789, 24);
            this.tagText4.Module = "Module2";
            this.tagText4.Multiline = true;
            this.tagText4.Name = "tagText4";
            this.tagText4.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tagText4.Size = new System.Drawing.Size(100, 194);
            this.tagText4.Sort = "Right";
            this.tagText4.TabIndex = 14;
            this.tagText4.Tag = "Right";
            // 
            // tagText3
            // 
            this.tagText3.AcceptsReturn = true;
            this.tagText3.AcceptsTab = true;
            this.tagText3.Location = new System.Drawing.Point(622, 123);
            this.tagText3.Module = "Module2";
            this.tagText3.Multiline = true;
            this.tagText3.Name = "tagText3";
            this.tagText3.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tagText3.Size = new System.Drawing.Size(161, 95);
            this.tagText3.Sort = "*";
            this.tagText3.TabIndex = 13;
            this.tagText3.Tag = "*";
            // 
            // tagIO3
            // 
            this.tagIO3.Checked = false;
            this.tagIO3.ForeColor = System.Drawing.Color.Yellow;
            this.tagIO3.Location = new System.Drawing.Point(622, 79);
            this.tagIO3.Module = "Module2";
            this.tagIO3.Name = "tagIO3";
            this.tagIO3.Size = new System.Drawing.Size(75, 23);
            this.tagIO3.Sort = "LED3";
            this.tagIO3.TabIndex = 11;
            this.tagIO3.Tag = "LED3";
            this.tagIO3.U16_Mask = ((ushort)(1));
            // 
            // tagIO2
            // 
            this.tagIO2.Checked = false;
            this.tagIO2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.tagIO2.Location = new System.Drawing.Point(622, 50);
            this.tagIO2.Module = "Module2";
            this.tagIO2.Name = "tagIO2";
            this.tagIO2.Size = new System.Drawing.Size(75, 23);
            this.tagIO2.Sort = "LED2";
            this.tagIO2.TabIndex = 10;
            this.tagIO2.Tag = "LED2";
            this.tagIO2.U16_Mask = ((ushort)(1));
            // 
            // tagIO1
            // 
            this.tagIO1.Checked = false;
            this.tagIO1.ForeColor = System.Drawing.Color.Red;
            this.tagIO1.Location = new System.Drawing.Point(622, 21);
            this.tagIO1.Module = "Module2";
            this.tagIO1.Name = "tagIO1";
            this.tagIO1.Size = new System.Drawing.Size(75, 23);
            this.tagIO1.Sort = "PushButton";
            this.tagIO1.TabIndex = 9;
            this.tagIO1.Tag = "PushButton";
            this.tagIO1.U16_Mask = ((ushort)(1));
            // 
            // tagState1
            // 
            this.tagState1.Checked = false;
            this.tagState1.Location = new System.Drawing.Point(447, 6);
            this.tagState1.Module = "Main";
            this.tagState1.Name = "tagState1";
            this.tagState1.Size = new System.Drawing.Size(103, 23);
            this.tagState1.Sort = "MainState";
            this.tagState1.State = "RUNHEIGHT";
            this.tagState1.TabIndex = 3;
            this.tagState1.Tag = "";
            this.tagState1.Text = "RUNHEIGHT";
            // 
            // tagText1
            // 
            this.tagText1.AcceptsReturn = true;
            this.tagText1.AcceptsTab = true;
            this.tagText1.Location = new System.Drawing.Point(124, 21);
            this.tagText1.Module = "Main";
            this.tagText1.Multiline = true;
            this.tagText1.Name = "tagText1";
            this.tagText1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tagText1.Size = new System.Drawing.Size(100, 197);
            this.tagText1.Sort = "LRHeight";
            this.tagText1.TabIndex = 2;
            this.tagText1.Tag = "LRHeight";
            // 
            // tagGauge1
            // 
            this.tagGauge1.BaseArcColor = System.Drawing.Color.Gray;
            this.tagGauge1.BaseArcRadius = 150;
            this.tagGauge1.BaseArcStart = 145;
            this.tagGauge1.BaseArcSweep = 70;
            this.tagGauge1.BaseArcWidth = 2;
            this.tagGauge1.Cap_Idx = ((byte)(1));
            this.tagGauge1.CapColors = new System.Drawing.Color[] {
        System.Drawing.Color.Black,
        System.Drawing.Color.Black,
        System.Drawing.Color.Black,
        System.Drawing.Color.Black,
        System.Drawing.Color.Black};
            this.tagGauge1.CapPosition = new System.Drawing.Point(10, 10);
            this.tagGauge1.CapsPosition = new System.Drawing.Point[] {
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10),
        new System.Drawing.Point(10, 10)};
            this.tagGauge1.CapsText = new string[] {
        "",
        "",
        "",
        "",
        ""};
            this.tagGauge1.CapText = "";
            this.tagGauge1.Center = new System.Drawing.Point(175, 110);
            this.tagGauge1.Location = new System.Drawing.Point(8, 6);
            this.tagGauge1.MaxValue = 1024F;
            this.tagGauge1.MinValue = 0F;
            this.tagGauge1.Module = "Main";
            this.tagGauge1.Name = "tagGauge1";
            this.tagGauge1.NeedleColor1 = MettleLib.TagGauge.NeedleColorEnum.Red;
            this.tagGauge1.NeedleColor2 = System.Drawing.Color.DimGray;
            this.tagGauge1.NeedleRadius = 160;
            this.tagGauge1.NeedleType = 0;
            this.tagGauge1.NeedleWidth = 2;
            this.tagGauge1.Range_Idx = ((byte)(1));
            this.tagGauge1.RangeColor = System.Drawing.Color.Red;
            this.tagGauge1.RangeEnabled = false;
            this.tagGauge1.RangeEndValue = 400F;
            this.tagGauge1.RangeInnerRadius = 70;
            this.tagGauge1.RangeOuterRadius = 80;
            this.tagGauge1.RangesColor = new System.Drawing.Color[] {
        System.Drawing.Color.LightGreen,
        System.Drawing.Color.Red,
        System.Drawing.SystemColors.Control,
        System.Drawing.SystemColors.Control,
        System.Drawing.SystemColors.Control};
            this.tagGauge1.RangesEnabled = new bool[] {
        false,
        false,
        false,
        false,
        false};
            this.tagGauge1.RangesEndValue = new float[] {
        300F,
        400F,
        0F,
        0F,
        0F};
            this.tagGauge1.RangesInnerRadius = new int[] {
        70,
        70,
        70,
        70,
        70};
            this.tagGauge1.RangesOuterRadius = new int[] {
        80,
        80,
        80,
        80,
        80};
            this.tagGauge1.RangesStartValue = new float[] {
        -100F,
        300F,
        0F,
        0F,
        0F};
            this.tagGauge1.RangeStartValue = 300F;
            this.tagGauge1.ScaleLinesInterColor = System.Drawing.Color.Red;
            this.tagGauge1.ScaleLinesInterInnerRadius = 145;
            this.tagGauge1.ScaleLinesInterOuterRadius = 150;
            this.tagGauge1.ScaleLinesInterWidth = 2;
            this.tagGauge1.ScaleLinesMajorColor = System.Drawing.Color.Black;
            this.tagGauge1.ScaleLinesMajorInnerRadius = 140;
            this.tagGauge1.ScaleLinesMajorOuterRadius = 150;
            this.tagGauge1.ScaleLinesMajorStepValue = 128F;
            this.tagGauge1.ScaleLinesMajorWidth = 2;
            this.tagGauge1.ScaleLinesMinorColor = System.Drawing.Color.Gray;
            this.tagGauge1.ScaleLinesMinorInnerRadius = 145;
            this.tagGauge1.ScaleLinesMinorNumOf = 9;
            this.tagGauge1.ScaleLinesMinorOuterRadius = 150;
            this.tagGauge1.ScaleLinesMinorWidth = 1;
            this.tagGauge1.ScaleNumbersColor = System.Drawing.Color.Black;
            this.tagGauge1.ScaleNumbersFormat = null;
            this.tagGauge1.ScaleNumbersRadius = 162;
            this.tagGauge1.ScaleNumbersRotation = 90;
            this.tagGauge1.ScaleNumbersStartScaleLine = 1;
            this.tagGauge1.ScaleNumbersStepScaleLines = 2;
            this.tagGauge1.Size = new System.Drawing.Size(110, 222);
            this.tagGauge1.Sort = "LRHeight";
            this.tagGauge1.TabIndex = 1;
            this.tagGauge1.Tag = "LRHeight";
            this.tagGauge1.Text = "tagGauge1";
            this.tagGauge1.Value = 0F;
            // 
            // tabText
            // 
            this.tabText.Controls.Add(this.txtAllText);
            this.tabText.Controls.Add(this.label5);
            this.tabText.Controls.Add(this.txtModules);
            this.tabText.Controls.Add(this.label3);
            this.tabText.Controls.Add(this.label2);
            this.tabText.Controls.Add(this.label1);
            this.tabText.Controls.Add(this.txtTagData);
            this.tabText.Controls.Add(this.txtUniques);
            this.tabText.Location = new System.Drawing.Point(4, 22);
            this.tabText.Name = "tabText";
            this.tabText.Padding = new System.Windows.Forms.Padding(3);
            this.tabText.Size = new System.Drawing.Size(1031, 516);
            this.tabText.TabIndex = 0;
            this.tabText.Text = "Text";
            this.tabText.UseVisualStyleBackColor = true;
            // 
            // txtAllText
            // 
            this.txtAllText.AcceptsReturn = true;
            this.txtAllText.AcceptsTab = true;
            this.txtAllText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtAllText.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.txtAllText.HideSelection = false;
            this.txtAllText.Location = new System.Drawing.Point(34, 68);
            this.txtAllText.Multiline = true;
            this.txtAllText.Name = "txtAllText";
            this.txtAllText.ReadOnly = true;
            this.txtAllText.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtAllText.Size = new System.Drawing.Size(277, 325);
            this.txtAllText.TabIndex = 86;
            this.txtAllText.WordWrap = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(391, 25);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(138, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Modules; Click to see  Sorts";
            // 
            // txtModules
            // 
            this.txtModules.AcceptsReturn = true;
            this.txtModules.AcceptsTab = true;
            this.txtModules.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.txtModules.HideSelection = false;
            this.txtModules.Location = new System.Drawing.Point(394, 41);
            this.txtModules.Multiline = true;
            this.txtModules.Name = "txtModules";
            this.txtModules.ReadOnly = true;
            this.txtModules.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtModules.Size = new System.Drawing.Size(172, 388);
            this.txtModules.TabIndex = 6;
            this.txtModules.WordWrap = false;
            this.txtModules.MouseClick += new System.Windows.Forms.MouseEventHandler(this.txtModules_MouseClick);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(780, 25);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(52, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Tag Data";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(587, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(118, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Sorts; Click to see Data";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(31, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "All serial data";
            // 
            // txtTagData
            // 
            this.txtTagData.AcceptsReturn = true;
            this.txtTagData.AcceptsTab = true;
            this.txtTagData.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.txtTagData.HideSelection = false;
            this.txtTagData.Location = new System.Drawing.Point(783, 41);
            this.txtTagData.Multiline = true;
            this.txtTagData.Name = "txtTagData";
            this.txtTagData.ReadOnly = true;
            this.txtTagData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtTagData.Size = new System.Drawing.Size(172, 388);
            this.txtTagData.TabIndex = 2;
            this.txtTagData.WordWrap = false;
            // 
            // txtUniques
            // 
            this.txtUniques.AcceptsReturn = true;
            this.txtUniques.AcceptsTab = true;
            this.txtUniques.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.txtUniques.HideSelection = false;
            this.txtUniques.Location = new System.Drawing.Point(590, 41);
            this.txtUniques.Multiline = true;
            this.txtUniques.Name = "txtUniques";
            this.txtUniques.ReadOnly = true;
            this.txtUniques.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtUniques.Size = new System.Drawing.Size(172, 388);
            this.txtUniques.TabIndex = 1;
            this.txtUniques.WordWrap = false;
            this.txtUniques.MouseClick += new System.Windows.Forms.MouseEventHandler(this.textUniques_MouseClick);
            // 
            // chart1
            // 
            this.chart1.Location = new System.Drawing.Point(0, 0);
            this.chart1.Name = "chart1";
            this.chart1.Size = new System.Drawing.Size(300, 300);
            this.chart1.TabIndex = 0;
            // 
            // chart2
            // 
            this.chart2.Location = new System.Drawing.Point(0, 0);
            this.chart2.Name = "chart2";
            this.chart2.Size = new System.Drawing.Size(300, 300);
            this.chart2.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnPlay,
            this.btnStop,
            this.btnTools,
            this.btnAbout,
            this.BtnReset});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1039, 25);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // btnPlay
            // 
            this.btnPlay.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnPlay.Image = ((System.Drawing.Image)(resources.GetObject("btnPlay.Image")));
            this.btnPlay.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnPlay.Name = "btnPlay";
            this.btnPlay.Size = new System.Drawing.Size(23, 22);
            this.btnPlay.Text = "toolStripButton1";
            this.btnPlay.ToolTipText = "Start !";
            this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // btnStop
            // 
            this.btnStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnStop.Image = ((System.Drawing.Image)(resources.GetObject("btnStop.Image")));
            this.btnStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(23, 22);
            this.btnStop.Text = "toolStripButton2";
            this.btnStop.ToolTipText = "Stop!";
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnTools
            // 
            this.btnTools.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnTools.Image = ((System.Drawing.Image)(resources.GetObject("btnTools.Image")));
            this.btnTools.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnTools.Name = "btnTools";
            this.btnTools.Size = new System.Drawing.Size(23, 22);
            this.btnTools.ToolTipText = "Setup";
            this.btnTools.Click += new System.EventHandler(this.btnTools_Click);
            // 
            // btnAbout
            // 
            this.btnAbout.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnAbout.Image = ((System.Drawing.Image)(resources.GetObject("btnAbout.Image")));
            this.btnAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnAbout.Name = "btnAbout";
            this.btnAbout.Size = new System.Drawing.Size(44, 22);
            this.btnAbout.Text = "About";
            this.btnAbout.Click += new System.EventHandler(this.btnAbout_Click);
            // 
            // BtnReset
            // 
            this.BtnReset.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.BtnReset.Image = ((System.Drawing.Image)(resources.GetObject("BtnReset.Image")));
            this.BtnReset.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.BtnReset.Name = "BtnReset";
            this.BtnReset.Size = new System.Drawing.Size(39, 22);
            this.BtnReset.Text = "Reset";
            this.BtnReset.ToolTipText = "Reset and clear UI";
            this.BtnReset.Click += new System.EventHandler(this.BtnReset_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.stripStatus,
            this.stripError});
            this.statusStrip1.Location = new System.Drawing.Point(0, 547);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1039, 22);
            this.statusStrip1.TabIndex = 2;
            this.statusStrip1.Text = "Status";
            // 
            // stripStatus
            // 
            this.stripStatus.AutoSize = false;
            this.stripStatus.Name = "stripStatus";
            this.stripStatus.Size = new System.Drawing.Size(100, 17);
            this.stripStatus.Text = "Status";
            this.stripStatus.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.stripStatus.ToolTipText = "Application status";
            // 
            // stripError
            // 
            this.stripError.AutoSize = false;
            this.stripError.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.stripError.Name = "stripError";
            this.stripError.Size = new System.Drawing.Size(200, 17);
            this.stripError.Text = "No Error";
            this.stripError.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.stripError.ToolTipText = "Application errors";
            // 
            // tagState2
            // 
            this.tagState2.Checked = false;
            this.tagState2.Location = new System.Drawing.Point(447, 35);
            this.tagState2.Module = "Main";
            this.tagState2.Name = "tagState2";
            this.tagState2.Size = new System.Drawing.Size(103, 23);
            this.tagState2.Sort = "MainState";
            this.tagState2.State = "RUNHEIGHT";
            this.tagState2.TabIndex = 27;
            this.tagState2.Tag = "";
            this.tagState2.Text = "DUMPTANK";
            // 
            // tagState3
            // 
            this.tagState3.Checked = false;
            this.tagState3.Location = new System.Drawing.Point(447, 64);
            this.tagState3.Module = "Main";
            this.tagState3.Name = "tagState3";
            this.tagState3.Size = new System.Drawing.Size(103, 23);
            this.tagState3.Sort = "MainState";
            this.tagState3.State = "RUNHEIGHT";
            this.tagState3.TabIndex = 28;
            this.tagState3.Tag = "";
            this.tagState3.Text = "DUMPINGTANK";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1039, 569);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.tabMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(908, 455);
            this.Name = "FormMain";
            this.Text = "Mettle; Firmware inspector";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabMain.ResumeLayout(false);
            this.tabChart.ResumeLayout(false);
            this.tabChart.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tagChart1)).EndInit();
            this.tabText.ResumeLayout(false);
            this.tabText.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart2)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabMain;
        private System.Windows.Forms.TabPage tabText;
        private System.Windows.Forms.TabPage tabChart;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart2;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton btnPlay;
        private System.Windows.Forms.ToolStripButton btnStop;
        private System.Windows.Forms.ToolStripButton btnTools;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtTagData;
        private System.Windows.Forms.TextBox txtUniques;
        private System.Windows.Forms.ToolStripButton btnAbout;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtModules;
        private System.Windows.Forms.ToolStripButton BtnReset;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel stripStatus;
        private System.Windows.Forms.ToolStripStatusLabel stripError;
        private System.Windows.Forms.TextBox txtAllText;
        private MettleLib.TagGauge tagGauge1;
        private MettleLib.TagState tagState1;
        private MettleLib.TagText tagText1;
        private MettleLib.TagIO tagIO3;
        private MettleLib.TagIO tagIO2;
        private MettleLib.TagIO tagIO1;
        private MettleLib.TagGauge tagGauge2;
        private MettleLib.TagText tagText4;
        private MettleLib.TagText tagText3;
        private MettleLib.TagChart tagChart1;
        private MettleLib.TagText tagText7;
        private MettleLib.TagText tagText6;
        private MettleLib.TagText tagText5;
        private MettleLib.TagIO tagIO8;
        private MettleLib.TagIO tagIO7;
        private MettleLib.TagIO tagIO6;
        private MettleLib.TagIO tagIO5;
        private MettleLib.TagIO tagIO4;
        private MettleLib.TagText tagText8;
        private MettleLib.TagText tagText2;
        private MettleLib.TagState tagState3;
        private MettleLib.TagState tagState2;
    }
}

