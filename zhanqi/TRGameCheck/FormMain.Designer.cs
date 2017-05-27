namespace TRGameCheck
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
            this.label1 = new System.Windows.Forms.Label();
            this.tbCheckPath = new System.Windows.Forms.TextBox();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnRun = new System.Windows.Forms.Button();
            this.cbAllLog = new System.Windows.Forms.CheckBox();
            this.cbJson = new System.Windows.Forms.CheckBox();
            this.cbFileExist = new System.Windows.Forms.CheckBox();
            this.cbScene = new System.Windows.Forms.CheckBox();
            this.cbStrings = new System.Windows.Forms.CheckBox();
            this.btnClean = new System.Windows.Forms.Button();
            this.cbFileName = new System.Windows.Forms.CheckBox();
            this.cbGlobalRes = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(25, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "游戏资源目录";
            // 
            // tbCheckPath
            // 
            this.tbCheckPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbCheckPath.Location = new System.Drawing.Point(109, 21);
            this.tbCheckPath.Name = "tbCheckPath";
            this.tbCheckPath.Size = new System.Drawing.Size(606, 21);
            this.tbCheckPath.TabIndex = 1;
            this.tbCheckPath.Text = "D:\\projects\\trgame\\曹魏传\\正式发布资源\\assets";
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Location = new System.Drawing.Point(150, 62);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbLog.Size = new System.Drawing.Size(565, 453);
            this.tbLog.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(61, 65);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "日志";
            // 
            // btnRun
            // 
            this.btnRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRun.Location = new System.Drawing.Point(491, 535);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(80, 28);
            this.btnRun.TabIndex = 4;
            this.btnRun.Text = "执行";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // cbAllLog
            // 
            this.cbAllLog.AutoSize = true;
            this.cbAllLog.Location = new System.Drawing.Point(27, 95);
            this.cbAllLog.Name = "cbAllLog";
            this.cbAllLog.Size = new System.Drawing.Size(72, 16);
            this.cbAllLog.TabIndex = 5;
            this.cbAllLog.Text = "完整日志";
            this.cbAllLog.UseVisualStyleBackColor = true;
            // 
            // cbJson
            // 
            this.cbJson.AutoSize = true;
            this.cbJson.Checked = true;
            this.cbJson.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbJson.Location = new System.Drawing.Point(6, 148);
            this.cbJson.Name = "cbJson";
            this.cbJson.Size = new System.Drawing.Size(72, 16);
            this.cbJson.TabIndex = 6;
            this.cbJson.Text = "检查json";
            this.cbJson.UseVisualStyleBackColor = true;
            // 
            // cbFileExist
            // 
            this.cbFileExist.AutoSize = true;
            this.cbFileExist.Checked = true;
            this.cbFileExist.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbFileExist.Location = new System.Drawing.Point(6, 192);
            this.cbFileExist.Name = "cbFileExist";
            this.cbFileExist.Size = new System.Drawing.Size(96, 16);
            this.cbFileExist.TabIndex = 7;
            this.cbFileExist.Text = "检查文件存在";
            this.cbFileExist.UseVisualStyleBackColor = true;
            // 
            // cbScene
            // 
            this.cbScene.AutoSize = true;
            this.cbScene.Checked = true;
            this.cbScene.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbScene.Location = new System.Drawing.Point(6, 214);
            this.cbScene.Name = "cbScene";
            this.cbScene.Size = new System.Drawing.Size(72, 16);
            this.cbScene.TabIndex = 8;
            this.cbScene.Text = "检查剧本";
            this.cbScene.UseVisualStyleBackColor = true;
            // 
            // cbStrings
            // 
            this.cbStrings.AutoSize = true;
            this.cbStrings.Checked = true;
            this.cbStrings.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbStrings.Location = new System.Drawing.Point(6, 236);
            this.cbStrings.Name = "cbStrings";
            this.cbStrings.Size = new System.Drawing.Size(138, 16);
            this.cbStrings.TabIndex = 9;
            this.cbStrings.Text = "检查strings.xml格式";
            this.cbStrings.UseVisualStyleBackColor = true;
            // 
            // btnClean
            // 
            this.btnClean.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClean.Location = new System.Drawing.Point(361, 535);
            this.btnClean.Name = "btnClean";
            this.btnClean.Size = new System.Drawing.Size(80, 28);
            this.btnClean.TabIndex = 10;
            this.btnClean.Text = "清空";
            this.btnClean.UseVisualStyleBackColor = true;
            this.btnClean.Click += new System.EventHandler(this.btnClean_Click);
            // 
            // cbFileName
            // 
            this.cbFileName.AutoSize = true;
            this.cbFileName.Checked = true;
            this.cbFileName.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbFileName.Location = new System.Drawing.Point(6, 170);
            this.cbFileName.Name = "cbFileName";
            this.cbFileName.Size = new System.Drawing.Size(84, 16);
            this.cbFileName.TabIndex = 11;
            this.cbFileName.Text = "检查文件名";
            this.cbFileName.UseVisualStyleBackColor = true;
            // 
            // cbGlobalRes
            // 
            this.cbGlobalRes.AutoSize = true;
            this.cbGlobalRes.Checked = true;
            this.cbGlobalRes.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbGlobalRes.Location = new System.Drawing.Point(6, 258);
            this.cbGlobalRes.Name = "cbGlobalRes";
            this.cbGlobalRes.Size = new System.Drawing.Size(126, 16);
            this.cbGlobalRes.TabIndex = 12;
            this.cbGlobalRes.Text = "检查globalres目录";
            this.cbGlobalRes.UseVisualStyleBackColor = true;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(735, 589);
            this.Controls.Add(this.cbGlobalRes);
            this.Controls.Add(this.cbFileName);
            this.Controls.Add(this.btnClean);
            this.Controls.Add(this.cbStrings);
            this.Controls.Add(this.cbScene);
            this.Controls.Add(this.cbFileExist);
            this.Controls.Add(this.cbJson);
            this.Controls.Add(this.cbAllLog);
            this.Controls.Add(this.btnRun);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbLog);
            this.Controls.Add(this.tbCheckPath);
            this.Controls.Add(this.label1);
            this.Name = "FormMain";
            this.Text = "数据检查程序";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbCheckPath;
        private System.Windows.Forms.TextBox tbLog;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnRun;
        private System.Windows.Forms.CheckBox cbAllLog;
        private System.Windows.Forms.CheckBox cbJson;
        private System.Windows.Forms.CheckBox cbFileExist;
        private System.Windows.Forms.CheckBox cbScene;
        private System.Windows.Forms.CheckBox cbStrings;
        private System.Windows.Forms.Button btnClean;
        private System.Windows.Forms.CheckBox cbFileName;
        private System.Windows.Forms.CheckBox cbGlobalRes;
    }
}

