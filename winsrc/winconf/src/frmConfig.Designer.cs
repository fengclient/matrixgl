namespace MatrixGLConfig
{
    partial class frmConfig
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
            this.boxStyle = new System.Windows.Forms.GroupBox();
            this.btnStyleStatic = new System.Windows.Forms.RadioButton();
            this.btnStyle3D = new System.Windows.Forms.RadioButton();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.boxColor = new System.Windows.Forms.GroupBox();
            this.btnColorBlue = new System.Windows.Forms.RadioButton();
            this.btnColorRed = new System.Windows.Forms.RadioButton();
            this.btnColorGreen = new System.Windows.Forms.RadioButton();
            this.boxCredits = new System.Windows.Forms.GroupBox();
            this.btnCreditsYes = new System.Windows.Forms.RadioButton();
            this.btnCreditsNo = new System.Windows.Forms.RadioButton();
            this.boxStyle.SuspendLayout();
            this.boxColor.SuspendLayout();
            this.boxCredits.SuspendLayout();
            this.SuspendLayout();
            // 
            // boxStyle
            // 
            this.boxStyle.Controls.Add(this.btnStyleStatic);
            this.boxStyle.Controls.Add(this.btnStyle3D);
            this.boxStyle.Location = new System.Drawing.Point(12, 12);
            this.boxStyle.Name = "boxStyle";
            this.boxStyle.Size = new System.Drawing.Size(188, 71);
            this.boxStyle.TabIndex = 0;
            this.boxStyle.TabStop = false;
            this.boxStyle.Text = "Style";
            // 
            // btnStyleStatic
            // 
            this.btnStyleStatic.AutoSize = true;
            this.btnStyleStatic.Location = new System.Drawing.Point(16, 43);
            this.btnStyleStatic.Name = "btnStyleStatic";
            this.btnStyleStatic.Size = new System.Drawing.Size(52, 17);
            this.btnStyleStatic.TabIndex = 1;
            this.btnStyleStatic.TabStop = true;
            this.btnStyleStatic.Text = "Static";
            this.btnStyleStatic.UseVisualStyleBackColor = true;
            this.btnStyleStatic.CheckedChanged += new System.EventHandler(this.btnStyleStatic_CheckedChanged);
            // 
            // btnStyle3D
            // 
            this.btnStyle3D.AutoSize = true;
            this.btnStyle3D.Location = new System.Drawing.Point(16, 20);
            this.btnStyle3D.Name = "btnStyle3D";
            this.btnStyle3D.Size = new System.Drawing.Size(39, 17);
            this.btnStyle3D.TabIndex = 0;
            this.btnStyle3D.TabStop = true;
            this.btnStyle3D.Text = "3D";
            this.btnStyle3D.UseVisualStyleBackColor = true;
            this.btnStyle3D.CheckedChanged += new System.EventHandler(this.btnStyle3D_CheckedChanged);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(12, 268);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(61, 23);
            this.btnSave.TabIndex = 0;
            this.btnSave.Text = "&Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(140, 268);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(58, 23);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // boxColor
            // 
            this.boxColor.Controls.Add(this.btnColorBlue);
            this.boxColor.Controls.Add(this.btnColorRed);
            this.boxColor.Controls.Add(this.btnColorGreen);
            this.boxColor.Location = new System.Drawing.Point(12, 166);
            this.boxColor.Name = "boxColor";
            this.boxColor.Size = new System.Drawing.Size(188, 90);
            this.boxColor.TabIndex = 3;
            this.boxColor.TabStop = false;
            this.boxColor.Text = "Color";
            // 
            // btnColorBlue
            // 
            this.btnColorBlue.AutoSize = true;
            this.btnColorBlue.Location = new System.Drawing.Point(16, 65);
            this.btnColorBlue.Name = "btnColorBlue";
            this.btnColorBlue.Size = new System.Drawing.Size(46, 17);
            this.btnColorBlue.TabIndex = 2;
            this.btnColorBlue.TabStop = true;
            this.btnColorBlue.Text = "Blue";
            this.btnColorBlue.UseVisualStyleBackColor = true;
            // 
            // btnColorRed
            // 
            this.btnColorRed.AutoSize = true;
            this.btnColorRed.Location = new System.Drawing.Point(16, 42);
            this.btnColorRed.Name = "btnColorRed";
            this.btnColorRed.Size = new System.Drawing.Size(45, 17);
            this.btnColorRed.TabIndex = 1;
            this.btnColorRed.TabStop = true;
            this.btnColorRed.Text = "Red";
            this.btnColorRed.UseVisualStyleBackColor = true;
            // 
            // btnColorGreen
            // 
            this.btnColorGreen.AutoSize = true;
            this.btnColorGreen.Location = new System.Drawing.Point(16, 19);
            this.btnColorGreen.Name = "btnColorGreen";
            this.btnColorGreen.Size = new System.Drawing.Size(54, 17);
            this.btnColorGreen.TabIndex = 0;
            this.btnColorGreen.TabStop = true;
            this.btnColorGreen.Text = "Green";
            this.btnColorGreen.UseVisualStyleBackColor = true;
            // 
            // boxCredits
            // 
            this.boxCredits.Controls.Add(this.btnCreditsYes);
            this.boxCredits.Controls.Add(this.btnCreditsNo);
            this.boxCredits.Location = new System.Drawing.Point(12, 89);
            this.boxCredits.Name = "boxCredits";
            this.boxCredits.Size = new System.Drawing.Size(188, 71);
            this.boxCredits.TabIndex = 2;
            this.boxCredits.TabStop = false;
            this.boxCredits.Text = "Credits";
            // 
            // btnCreditsYes
            // 
            this.btnCreditsYes.AutoSize = true;
            this.btnCreditsYes.Location = new System.Drawing.Point(16, 43);
            this.btnCreditsYes.Name = "btnCreditsYes";
            this.btnCreditsYes.Size = new System.Drawing.Size(107, 17);
            this.btnCreditsYes.TabIndex = 1;
            this.btnCreditsYes.TabStop = true;
            this.btnCreditsYes.Text = "Credits on startup";
            this.btnCreditsYes.UseVisualStyleBackColor = true;
            // 
            // btnCreditsNo
            // 
            this.btnCreditsNo.AutoSize = true;
            this.btnCreditsNo.Location = new System.Drawing.Point(16, 20);
            this.btnCreditsNo.Name = "btnCreditsNo";
            this.btnCreditsNo.Size = new System.Drawing.Size(123, 17);
            this.btnCreditsNo.TabIndex = 0;
            this.btnCreditsNo.TabStop = true;
            this.btnCreditsNo.Text = "No credits on startup";
            this.btnCreditsNo.UseVisualStyleBackColor = true;
            // 
            // frmConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(214, 302);
            this.Controls.Add(this.boxCredits);
            this.Controls.Add(this.boxColor);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.boxStyle);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "frmConfig";
            this.Text = "MatrixGL Configuration";
            this.boxStyle.ResumeLayout(false);
            this.boxStyle.PerformLayout();
            this.boxColor.ResumeLayout(false);
            this.boxColor.PerformLayout();
            this.boxCredits.ResumeLayout(false);
            this.boxCredits.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox boxStyle;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.RadioButton btnStyleStatic;
        private System.Windows.Forms.RadioButton btnStyle3D;
        private System.Windows.Forms.GroupBox boxColor;
        private System.Windows.Forms.RadioButton btnColorBlue;
        private System.Windows.Forms.RadioButton btnColorRed;
        private System.Windows.Forms.RadioButton btnColorGreen;
        private System.Windows.Forms.GroupBox boxCredits;
        private System.Windows.Forms.RadioButton btnCreditsYes;
        private System.Windows.Forms.RadioButton btnCreditsNo;
    }
}

