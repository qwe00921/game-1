namespace ResourceEncrypt
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
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label4;
            this.btnEncrypt = new System.Windows.Forms.Button();
            this.btnDecrypt = new System.Windows.Forms.Button();
            this.tbDirSource = new System.Windows.Forms.TextBox();
            this.tbDirDest = new System.Windows.Forms.TextBox();
            this.btnDirSource = new System.Windows.Forms.Button();
            this.btnDirDest = new System.Windows.Forms.Button();
            this.cbWriteSource = new System.Windows.Forms.CheckBox();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.tbVersion = new System.Windows.Forms.TextBox();
            this.tbCodeKey = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.tbAllFileExt = new System.Windows.Forms.TextBox();
            this.tbPartFileExt = new System.Windows.Forms.TextBox();
            this.tbPartLen = new System.Windows.Forms.TextBox();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(40, 68);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(41, 12);
            label1.TabIndex = 2;
            label1.Text = "源目录";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(28, 109);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(53, 12);
            label2.TabIndex = 3;
            label2.Text = "目标目录";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(12, 27);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(41, 12);
            label3.TabIndex = 10;
            label3.Text = "版本号";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(121, 27);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(53, 12);
            label4.TabIndex = 12;
            label4.Text = "代码密钥";
            // 
            // btnEncrypt
            // 
            this.btnEncrypt.Location = new System.Drawing.Point(552, 183);
            this.btnEncrypt.Name = "btnEncrypt";
            this.btnEncrypt.Size = new System.Drawing.Size(75, 29);
            this.btnEncrypt.TabIndex = 0;
            this.btnEncrypt.Text = "加密";
            this.btnEncrypt.UseVisualStyleBackColor = true;
            this.btnEncrypt.Click += new System.EventHandler(this.btnEncrypt_Click);
            // 
            // btnDecrypt
            // 
            this.btnDecrypt.Location = new System.Drawing.Point(552, 229);
            this.btnDecrypt.Name = "btnDecrypt";
            this.btnDecrypt.Size = new System.Drawing.Size(75, 29);
            this.btnDecrypt.TabIndex = 1;
            this.btnDecrypt.Text = "解密";
            this.btnDecrypt.UseVisualStyleBackColor = true;
            this.btnDecrypt.Click += new System.EventHandler(this.btnDecrypt_Click);
            // 
            // tbDirSource
            // 
            this.tbDirSource.Location = new System.Drawing.Point(113, 65);
            this.tbDirSource.Name = "tbDirSource";
            this.tbDirSource.Size = new System.Drawing.Size(420, 21);
            this.tbDirSource.TabIndex = 4;
            this.tbDirSource.Text = "D:\\temp\\test\\a";
            // 
            // tbDirDest
            // 
            this.tbDirDest.Location = new System.Drawing.Point(113, 103);
            this.tbDirDest.Name = "tbDirDest";
            this.tbDirDest.Size = new System.Drawing.Size(420, 21);
            this.tbDirDest.TabIndex = 5;
            this.tbDirDest.Text = "D:\\temp\\test\\b";
            // 
            // btnDirSource
            // 
            this.btnDirSource.Location = new System.Drawing.Point(551, 60);
            this.btnDirSource.Name = "btnDirSource";
            this.btnDirSource.Size = new System.Drawing.Size(75, 29);
            this.btnDirSource.TabIndex = 6;
            this.btnDirSource.Text = "选择";
            this.btnDirSource.UseVisualStyleBackColor = true;
            this.btnDirSource.Click += new System.EventHandler(this.btnDirSource_Click);
            // 
            // btnDirDest
            // 
            this.btnDirDest.Location = new System.Drawing.Point(551, 98);
            this.btnDirDest.Name = "btnDirDest";
            this.btnDirDest.Size = new System.Drawing.Size(75, 29);
            this.btnDirDest.TabIndex = 7;
            this.btnDirDest.Text = "选择";
            this.btnDirDest.UseVisualStyleBackColor = true;
            this.btnDirDest.Click += new System.EventHandler(this.btnDirDest_Click);
            // 
            // cbWriteSource
            // 
            this.cbWriteSource.AutoSize = true;
            this.cbWriteSource.Location = new System.Drawing.Point(530, 23);
            this.cbWriteSource.Name = "cbWriteSource";
            this.cbWriteSource.Size = new System.Drawing.Size(96, 16);
            this.cbWriteSource.TabIndex = 8;
            this.cbWriteSource.Text = "覆盖原始文件";
            this.cbWriteSource.UseVisualStyleBackColor = true;
            this.cbWriteSource.CheckedChanged += new System.EventHandler(this.cbWriteSource_CheckedChanged);
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Location = new System.Drawing.Point(1, 284);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbLog.Size = new System.Drawing.Size(710, 304);
            this.tbLog.TabIndex = 9;
            // 
            // tbVersion
            // 
            this.tbVersion.Location = new System.Drawing.Point(59, 24);
            this.tbVersion.Name = "tbVersion";
            this.tbVersion.Size = new System.Drawing.Size(49, 21);
            this.tbVersion.TabIndex = 11;
            this.tbVersion.Text = "1";
            // 
            // tbCodeKey
            // 
            this.tbCodeKey.Location = new System.Drawing.Point(180, 24);
            this.tbCodeKey.Name = "tbCodeKey";
            this.tbCodeKey.Size = new System.Drawing.Size(268, 21);
            this.tbCodeKey.TabIndex = 13;
            this.tbCodeKey.Text = "ABCDEFGH1234";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(29, 152);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(113, 12);
            this.label5.TabIndex = 14;
            this.label5.Text = "全部加密文件扩展名";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(29, 198);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(113, 12);
            this.label6.TabIndex = 15;
            this.label6.Text = "部分加密文件扩展名";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(29, 237);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(77, 12);
            this.label7.TabIndex = 16;
            this.label7.Text = "部分加密长度";
            // 
            // tbAllFileExt
            // 
            this.tbAllFileExt.Location = new System.Drawing.Point(157, 149);
            this.tbAllFileExt.Name = "tbAllFileExt";
            this.tbAllFileExt.Size = new System.Drawing.Size(337, 21);
            this.tbAllFileExt.TabIndex = 17;
            this.tbAllFileExt.Text = "eex_new;json;LM;dat;id";
            // 
            // tbPartFileExt
            // 
            this.tbPartFileExt.Location = new System.Drawing.Point(157, 195);
            this.tbPartFileExt.Name = "tbPartFileExt";
            this.tbPartFileExt.Size = new System.Drawing.Size(337, 21);
            this.tbPartFileExt.TabIndex = 18;
            this.tbPartFileExt.Text = "bmp;e5;png";
            // 
            // tbPartLen
            // 
            this.tbPartLen.Location = new System.Drawing.Point(157, 234);
            this.tbPartLen.Name = "tbPartLen";
            this.tbPartLen.Size = new System.Drawing.Size(337, 21);
            this.tbPartLen.TabIndex = 19;
            this.tbPartLen.Text = "2048";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(714, 591);
            this.Controls.Add(this.tbPartLen);
            this.Controls.Add(this.tbPartFileExt);
            this.Controls.Add(this.tbAllFileExt);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tbCodeKey);
            this.Controls.Add(label4);
            this.Controls.Add(this.tbVersion);
            this.Controls.Add(label3);
            this.Controls.Add(this.tbLog);
            this.Controls.Add(this.cbWriteSource);
            this.Controls.Add(this.btnDirDest);
            this.Controls.Add(this.btnDirSource);
            this.Controls.Add(this.tbDirDest);
            this.Controls.Add(this.tbDirSource);
            this.Controls.Add(label2);
            this.Controls.Add(label1);
            this.Controls.Add(this.btnDecrypt);
            this.Controls.Add(this.btnEncrypt);
            this.Name = "FormMain";
            this.Text = "资源加密和解密工具";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnEncrypt;
        private System.Windows.Forms.Button btnDecrypt;
        private System.Windows.Forms.TextBox tbDirSource;
        private System.Windows.Forms.TextBox tbDirDest;
        private System.Windows.Forms.Button btnDirSource;
        private System.Windows.Forms.Button btnDirDest;
        private System.Windows.Forms.CheckBox cbWriteSource;
        private System.Windows.Forms.TextBox tbLog;
        private System.Windows.Forms.TextBox tbVersion;
        private System.Windows.Forms.TextBox tbCodeKey;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbAllFileExt;
        private System.Windows.Forms.TextBox tbPartFileExt;
        private System.Windows.Forms.TextBox tbPartLen;
    }
}

