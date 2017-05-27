namespace SaveEditor
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
            this.searchFile = new System.Windows.Forms.Button();
            this.saveFile = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbVersion = new System.Windows.Forms.TextBox();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tbEventName = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tbSceneName = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.tbBattleName = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.tbStoreTime = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.tbMainLevel = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.tbCurRound = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // searchFile
            // 
            this.searchFile.Location = new System.Drawing.Point(216, 8);
            this.searchFile.Name = "searchFile";
            this.searchFile.Size = new System.Drawing.Size(75, 23);
            this.searchFile.TabIndex = 1;
            this.searchFile.Text = "浏览";
            this.searchFile.UseVisualStyleBackColor = true;
            this.searchFile.Click += new System.EventHandler(this.searchFile_Click);
            // 
            // saveFile
            // 
            this.saveFile.Location = new System.Drawing.Point(321, 10);
            this.saveFile.Name = "saveFile";
            this.saveFile.Size = new System.Drawing.Size(75, 23);
            this.saveFile.TabIndex = 2;
            this.saveFile.Text = "保存\r\n";
            this.saveFile.UseVisualStyleBackColor = true;
            this.saveFile.Click += new System.EventHandler(this.saveFile_Click);
            // 
            // label1
            // 
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Location = new System.Drawing.Point(0, 46);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(651, 2);
            this.label1.TabIndex = 3;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(59, 69);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "版本";
            // 
            // tbVersion
            // 
            this.tbVersion.Location = new System.Drawing.Point(104, 66);
            this.tbVersion.Name = "tbVersion";
            this.tbVersion.Size = new System.Drawing.Size(100, 21);
            this.tbVersion.TabIndex = 5;
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(0, 0);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(3, 434);
            this.splitter1.TabIndex = 6;
            this.splitter1.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(331, 74);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "label3";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(35, 111);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "事件名称";
            // 
            // tbEventName
            // 
            this.tbEventName.Location = new System.Drawing.Point(104, 108);
            this.tbEventName.Name = "tbEventName";
            this.tbEventName.Size = new System.Drawing.Size(100, 21);
            this.tbEventName.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(319, 111);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 10;
            this.label5.Text = "场景名称";
            // 
            // tbSceneName
            // 
            this.tbSceneName.Location = new System.Drawing.Point(389, 108);
            this.tbSceneName.Name = "tbSceneName";
            this.tbSceneName.Size = new System.Drawing.Size(100, 21);
            this.tbSceneName.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(35, 152);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 12);
            this.label6.TabIndex = 12;
            this.label6.Text = "战斗名称";
            // 
            // tbBattleName
            // 
            this.tbBattleName.Location = new System.Drawing.Point(104, 149);
            this.tbBattleName.Name = "tbBattleName";
            this.tbBattleName.Size = new System.Drawing.Size(100, 21);
            this.tbBattleName.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(319, 152);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 14;
            this.label7.Text = "存档时间";
            // 
            // tbStoreTime
            // 
            this.tbStoreTime.Location = new System.Drawing.Point(389, 148);
            this.tbStoreTime.Name = "tbStoreTime";
            this.tbStoreTime.ReadOnly = true;
            this.tbStoreTime.Size = new System.Drawing.Size(128, 21);
            this.tbStoreTime.TabIndex = 15;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(59, 195);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(29, 12);
            this.label8.TabIndex = 16;
            this.label8.Text = "级别";
            // 
            // tbMainLevel
            // 
            this.tbMainLevel.Location = new System.Drawing.Point(104, 192);
            this.tbMainLevel.Name = "tbMainLevel";
            this.tbMainLevel.Size = new System.Drawing.Size(100, 21);
            this.tbMainLevel.TabIndex = 17;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(319, 195);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(53, 12);
            this.label9.TabIndex = 18;
            this.label9.Text = "当前回合";
            // 
            // tbCurRound
            // 
            this.tbCurRound.Location = new System.Drawing.Point(389, 192);
            this.tbCurRound.Name = "tbCurRound";
            this.tbCurRound.Size = new System.Drawing.Size(100, 21);
            this.tbCurRound.TabIndex = 19;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoValidate = System.Windows.Forms.AutoValidate.Disable;
            this.ClientSize = new System.Drawing.Size(650, 434);
            this.Controls.Add(this.tbCurRound);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.tbMainLevel);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.tbStoreTime);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.tbBattleName);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tbSceneName);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tbEventName);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this.tbVersion);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.saveFile);
            this.Controls.Add(this.searchFile);
            this.Name = "FormMain";
            this.Text = "存档编辑器";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button searchFile;
        private System.Windows.Forms.Button saveFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.TextBox tbVersion;
        private System.Windows.Forms.TextBox tbEventName;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbSceneName;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbBattleName;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbStoreTime;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox tbMainLevel;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox tbCurRound;
    }
}

