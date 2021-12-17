namespace WindowsFormsApplication2
{
    partial class RLS_Main
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RLS_Main));
            this.launch_oscope = new System.Windows.Forms.Button();
            this.launch_dla = new System.Windows.Forms.Button();
            this.comboBox1_dcs = new System.Windows.Forms.ComboBox();
            this.select_experiment = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // launch_oscope
            // 
            this.launch_oscope.Location = new System.Drawing.Point(94, 80);
            this.launch_oscope.Name = "launch_oscope";
            this.launch_oscope.Size = new System.Drawing.Size(118, 23);
            this.launch_oscope.TabIndex = 0;
            this.launch_oscope.Text = "Velleman";
            this.launch_oscope.UseVisualStyleBackColor = true;
            this.launch_oscope.Click += new System.EventHandler(this.launch_oscope_Click);
            // 
            // launch_dla
            // 
            this.launch_dla.Location = new System.Drawing.Point(94, 168);
            this.launch_dla.Name = "launch_dla";
            this.launch_dla.Size = new System.Drawing.Size(118, 23);
            this.launch_dla.TabIndex = 3;
            this.launch_dla.Text = "Logic Analyzer";
            this.launch_dla.UseVisualStyleBackColor = true;
            this.launch_dla.Click += new System.EventHandler(this.launch_dla_Click);
            // 
            // comboBox1_dcs
            // 
            this.comboBox1_dcs.FormattingEnabled = true;
            this.comboBox1_dcs.Items.AddRange(new object[] {
            "Daughterboard 1",
            "Daughterboard 2"});
            this.comboBox1_dcs.Location = new System.Drawing.Point(91, 292);
            this.comboBox1_dcs.Name = "comboBox1_dcs";
            this.comboBox1_dcs.Size = new System.Drawing.Size(121, 21);
            this.comboBox1_dcs.TabIndex = 6;
            this.comboBox1_dcs.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // select_experiment
            // 
            this.select_experiment.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.select_experiment.Location = new System.Drawing.Point(94, 355);
            this.select_experiment.Name = "select_experiment";
            this.select_experiment.Size = new System.Drawing.Size(118, 62);
            this.select_experiment.TabIndex = 7;
            this.select_experiment.Text = "GO!";
            this.select_experiment.UseVisualStyleBackColor = false;
            this.select_experiment.Click += new System.EventHandler(this.button1_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(91, 263);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(118, 23);
            this.button1.TabIndex = 8;
            this.button1.Text = "Daughterboard";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(64, 107);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(204, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "\"Velleman\" illuminates yellow when active";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(64, 194);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(188, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "\"Logic Analyzer\" turns yellow when on";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(29, 247);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(251, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "\"Daughterboard\" turns yellow when DCUs attached";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(386, 80);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(504, 337);
            this.pictureBox1.TabIndex = 12;
            this.pictureBox1.TabStop = false;
            // 
            // RLS_Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(918, 497);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.select_experiment);
            this.Controls.Add(this.comboBox1_dcs);
            this.Controls.Add(this.launch_dla);
            this.Controls.Add(this.launch_oscope);
            this.Name = "RLS_Main";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button launch_oscope;
        private System.Windows.Forms.Button launch_dla;
        private System.Windows.Forms.ComboBox comboBox1_dcs;
        private System.Windows.Forms.Button select_experiment;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}

