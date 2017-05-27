namespace TRGameRepack
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label4;
            System.Windows.Forms.Label label5;
            System.Windows.Forms.Label label9;
            System.Windows.Forms.ToolTip ttHelp;
            System.Windows.Forms.Label label6;
            System.Windows.Forms.Label label7;
            this.tbApkPath = new System.Windows.Forms.TextBox();
            this.tbLog = new System.Windows.Forms.TextBox();
            this.tbResourceDelete = new System.Windows.Forms.TextBox();
            this.tbNormalOverwrite = new System.Windows.Forms.TextBox();
            this.tbPermissionDelete = new System.Windows.Forms.TextBox();
            this.tbEncryptOverwrite = new System.Windows.Forms.TextBox();
            this.tbResourceApk = new System.Windows.Forms.TextBox();
            this.tbOtherJsonFile = new System.Windows.Forms.TextBox();
            this.tbGloOverwrite = new System.Windows.Forms.TextBox();
            this.tbVersionName = new System.Windows.Forms.TextBox();
            this.clbChannel = new System.Windows.Forms.CheckedListBox();
            this.btnSelectFile = new System.Windows.Forms.Button();
            this.cbPackageName = new System.Windows.Forms.ComboBox();
            this.btnRun = new System.Windows.Forms.Button();
            this.btnDecompile = new System.Windows.Forms.Button();
            this.btnUpdateHistory = new System.Windows.Forms.Button();
            this.btnPermissionHelp = new System.Windows.Forms.Button();
            this.cbEnableVersionName = new System.Windows.Forms.CheckBox();
            this.cbDeleteGameResDir = new System.Windows.Forms.CheckBox();
            this.btnResourceApk = new System.Windows.Forms.Button();
            this.cbResourceApk = new System.Windows.Forms.CheckBox();
            this.btnOtherJson = new System.Windows.Forms.Button();
            this.cbEnableVersionCode = new System.Windows.Forms.CheckBox();
            this.tbVersionCode = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.rbPlatformWinPhone = new System.Windows.Forms.RadioButton();
            this.rbPlatformAndroid = new System.Windows.Forms.RadioButton();
            this.label8 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            label4 = new System.Windows.Forms.Label();
            label5 = new System.Windows.Forms.Label();
            label9 = new System.Windows.Forms.Label();
            ttHelp = new System.Windows.Forms.ToolTip(this.components);
            label6 = new System.Windows.Forms.Label();
            label7 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(248, 45);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(71, 12);
            label1.TabIndex = 1;
            label1.Text = "apk文件路径";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(290, 439);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(29, 12);
            label2.TabIndex = 4;
            label2.Text = "日志";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(290, 15);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(29, 12);
            label3.TabIndex = 5;
            label3.Text = "包名";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new System.Drawing.Point(244, 103);
            label4.Name = "label4";
            label4.Size = new System.Drawing.Size(77, 12);
            label4.TabIndex = 9;
            label4.Text = "删除资源路径";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new System.Drawing.Point(246, 244);
            label5.Name = "label5";
            label5.Size = new System.Drawing.Size(77, 12);
            label5.TabIndex = 13;
            label5.Text = "普通覆盖目录";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new System.Drawing.Point(265, 383);
            label9.Name = "label9";
            label9.Size = new System.Drawing.Size(53, 12);
            label9.TabIndex = 21;
            label9.Text = "关闭权限";
            // 
            // ttHelp
            // 
            ttHelp.IsBalloon = true;
            // 
            // tbApkPath
            // 
            this.tbApkPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbApkPath.Location = new System.Drawing.Point(326, 42);
            this.tbApkPath.Name = "tbApkPath";
            this.tbApkPath.Size = new System.Drawing.Size(418, 21);
            this.tbApkPath.TabIndex = 2;
            ttHelp.SetToolTip(this.tbApkPath, "用来设定原始apk的路径，可直接编辑");
            // 
            // tbLog
            // 
            this.tbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLog.Location = new System.Drawing.Point(326, 436);
            this.tbLog.Multiline = true;
            this.tbLog.Name = "tbLog";
            this.tbLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbLog.Size = new System.Drawing.Size(520, 102);
            this.tbLog.TabIndex = 7;
            ttHelp.SetToolTip(this.tbLog, "显示日志");
            // 
            // tbResourceDelete
            // 
            this.tbResourceDelete.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbResourceDelete.Enabled = false;
            this.tbResourceDelete.Location = new System.Drawing.Point(357, 100);
            this.tbResourceDelete.Multiline = true;
            this.tbResourceDelete.Name = "tbResourceDelete";
            this.tbResourceDelete.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbResourceDelete.Size = new System.Drawing.Size(488, 50);
            this.tbResourceDelete.TabIndex = 10;
            ttHelp.SetToolTip(this.tbResourceDelete, "删除指定的目录或文件");
            // 
            // tbNormalOverwrite
            // 
            this.tbNormalOverwrite.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbNormalOverwrite.Location = new System.Drawing.Point(326, 241);
            this.tbNormalOverwrite.Multiline = true;
            this.tbNormalOverwrite.Name = "tbNormalOverwrite";
            this.tbNormalOverwrite.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbNormalOverwrite.Size = new System.Drawing.Size(519, 51);
            this.tbNormalOverwrite.TabIndex = 14;
            ttHelp.SetToolTip(this.tbNormalOverwrite, "普通覆盖目录是直接覆盖对应目录，支持多个目录，使用英文分号;分隔路径");
            this.tbNormalOverwrite.TextChanged += new System.EventHandler(this.tbNormalOverwrite_TextChanged);
            // 
            // tbPermissionDelete
            // 
            this.tbPermissionDelete.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbPermissionDelete.Location = new System.Drawing.Point(325, 380);
            this.tbPermissionDelete.Multiline = true;
            this.tbPermissionDelete.Name = "tbPermissionDelete";
            this.tbPermissionDelete.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbPermissionDelete.Size = new System.Drawing.Size(520, 50);
            this.tbPermissionDelete.TabIndex = 22;
            ttHelp.SetToolTip(this.tbPermissionDelete, "使用英文分号;分隔需要去除的权限");
            // 
            // tbEncryptOverwrite
            // 
            this.tbEncryptOverwrite.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbEncryptOverwrite.Location = new System.Drawing.Point(326, 181);
            this.tbEncryptOverwrite.Multiline = true;
            this.tbEncryptOverwrite.Name = "tbEncryptOverwrite";
            this.tbEncryptOverwrite.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbEncryptOverwrite.Size = new System.Drawing.Size(519, 54);
            this.tbEncryptOverwrite.TabIndex = 25;
            ttHelp.SetToolTip(this.tbEncryptOverwrite, "加密覆盖目录是加密资源后覆盖对应目录，支持多个目录，使用英文分号;分隔路径");
            this.tbEncryptOverwrite.TextChanged += new System.EventHandler(this.tbEncryptOverwrite_TextChanged);
            // 
            // tbResourceApk
            // 
            this.tbResourceApk.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbResourceApk.Enabled = false;
            this.tbResourceApk.Location = new System.Drawing.Point(357, 154);
            this.tbResourceApk.Name = "tbResourceApk";
            this.tbResourceApk.Size = new System.Drawing.Size(387, 21);
            this.tbResourceApk.TabIndex = 30;
            ttHelp.SetToolTip(this.tbResourceApk, "用来设定原始apk的路径，可直接编辑");
            // 
            // tbOtherJsonFile
            // 
            this.tbOtherJsonFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbOtherJsonFile.Location = new System.Drawing.Point(326, 298);
            this.tbOtherJsonFile.Multiline = true;
            this.tbOtherJsonFile.Name = "tbOtherJsonFile";
            this.tbOtherJsonFile.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbOtherJsonFile.Size = new System.Drawing.Size(419, 40);
            this.tbOtherJsonFile.TabIndex = 34;
            ttHelp.SetToolTip(this.tbOtherJsonFile, "JSON");
            // 
            // tbGloOverwrite
            // 
            this.tbGloOverwrite.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbGloOverwrite.Location = new System.Drawing.Point(355, 345);
            this.tbGloOverwrite.Name = "tbGloOverwrite";
            this.tbGloOverwrite.Size = new System.Drawing.Size(368, 21);
            this.tbGloOverwrite.TabIndex = 41;
            ttHelp.SetToolTip(this.tbGloOverwrite, "globalres覆盖目录是直接覆盖对应目录，支持多个目录，使用英文分号;分隔路径");
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new System.Drawing.Point(246, 184);
            label6.Name = "label6";
            label6.Size = new System.Drawing.Size(77, 12);
            label6.TabIndex = 24;
            label6.Text = "加密覆盖目录";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new System.Drawing.Point(242, 301);
            label7.Name = "label7";
            label7.Size = new System.Drawing.Size(77, 12);
            label7.TabIndex = 33;
            label7.Text = "附加JSON文件";
            // 
            // tbVersionName
            // 
            this.tbVersionName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbVersionName.Enabled = false;
            this.tbVersionName.Location = new System.Drawing.Point(326, 72);
            this.tbVersionName.Name = "tbVersionName";
            this.tbVersionName.Size = new System.Drawing.Size(121, 21);
            this.tbVersionName.TabIndex = 27;
            // 
            // clbChannel
            // 
            this.clbChannel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.clbChannel.CheckOnClick = true;
            this.clbChannel.FormattingEnabled = true;
            this.clbChannel.HorizontalScrollbar = true;
            this.clbChannel.Location = new System.Drawing.Point(1, 82);
            this.clbChannel.Name = "clbChannel";
            this.clbChannel.Size = new System.Drawing.Size(233, 500);
            this.clbChannel.TabIndex = 0;
            // 
            // btnSelectFile
            // 
            this.btnSelectFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSelectFile.Location = new System.Drawing.Point(771, 40);
            this.btnSelectFile.Name = "btnSelectFile";
            this.btnSelectFile.Size = new System.Drawing.Size(75, 23);
            this.btnSelectFile.TabIndex = 3;
            this.btnSelectFile.Text = "选择文件";
            this.btnSelectFile.UseVisualStyleBackColor = true;
            this.btnSelectFile.Click += new System.EventHandler(this.btnSelectFile_Click);
            // 
            // cbPackageName
            // 
            this.cbPackageName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cbPackageName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbPackageName.FormattingEnabled = true;
            this.cbPackageName.Location = new System.Drawing.Point(327, 12);
            this.cbPackageName.Name = "cbPackageName";
            this.cbPackageName.Size = new System.Drawing.Size(418, 20);
            this.cbPackageName.TabIndex = 6;
            this.cbPackageName.SelectedIndexChanged += new System.EventHandler(this.cbPackageName_SelectedIndexChanged);
            // 
            // btnRun
            // 
            this.btnRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnRun.Location = new System.Drawing.Point(536, 544);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(67, 32);
            this.btnRun.TabIndex = 8;
            this.btnRun.Text = "执行打包";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // btnDecompile
            // 
            this.btnDecompile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnDecompile.Location = new System.Drawing.Point(657, 544);
            this.btnDecompile.Name = "btnDecompile";
            this.btnDecompile.Size = new System.Drawing.Size(88, 32);
            this.btnDecompile.TabIndex = 11;
            this.btnDecompile.Text = "执行反编译";
            this.btnDecompile.UseVisualStyleBackColor = true;
            this.btnDecompile.Click += new System.EventHandler(this.btnDecompile_Click);
            // 
            // btnUpdateHistory
            // 
            this.btnUpdateHistory.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnUpdateHistory.Location = new System.Drawing.Point(379, 544);
            this.btnUpdateHistory.Name = "btnUpdateHistory";
            this.btnUpdateHistory.Size = new System.Drawing.Size(103, 32);
            this.btnUpdateHistory.TabIndex = 16;
            this.btnUpdateHistory.Text = "更新历史记录";
            this.btnUpdateHistory.UseVisualStyleBackColor = true;
            this.btnUpdateHistory.Click += new System.EventHandler(this.btnUpdateHistory_Click);
            // 
            // btnPermissionHelp
            // 
            this.btnPermissionHelp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPermissionHelp.Location = new System.Drawing.Point(257, 407);
            this.btnPermissionHelp.Name = "btnPermissionHelp";
            this.btnPermissionHelp.Size = new System.Drawing.Size(63, 23);
            this.btnPermissionHelp.TabIndex = 23;
            this.btnPermissionHelp.Text = "权限说明";
            this.btnPermissionHelp.UseVisualStyleBackColor = true;
            this.btnPermissionHelp.Click += new System.EventHandler(this.btnPermissionHelp_Click);
            // 
            // cbEnableVersionName
            // 
            this.cbEnableVersionName.AutoSize = true;
            this.cbEnableVersionName.Location = new System.Drawing.Point(258, 74);
            this.cbEnableVersionName.Name = "cbEnableVersionName";
            this.cbEnableVersionName.Size = new System.Drawing.Size(60, 16);
            this.cbEnableVersionName.TabIndex = 28;
            this.cbEnableVersionName.Text = "版本名";
            this.cbEnableVersionName.UseVisualStyleBackColor = true;
            this.cbEnableVersionName.CheckedChanged += new System.EventHandler(this.cbEnableVersionName_CheckedChanged);
            // 
            // cbDeleteGameResDir
            // 
            this.cbDeleteGameResDir.AutoSize = true;
            this.cbDeleteGameResDir.Checked = true;
            this.cbDeleteGameResDir.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbDeleteGameResDir.Location = new System.Drawing.Point(685, 74);
            this.cbDeleteGameResDir.Name = "cbDeleteGameResDir";
            this.cbDeleteGameResDir.Size = new System.Drawing.Size(144, 16);
            this.cbDeleteGameResDir.TabIndex = 29;
            this.cbDeleteGameResDir.Text = "删除原有游戏资源目录";
            this.cbDeleteGameResDir.UseVisualStyleBackColor = true;
            this.cbDeleteGameResDir.CheckedChanged += new System.EventHandler(this.cbDeleteGameResDir_CheckedChanged);
            // 
            // btnResourceApk
            // 
            this.btnResourceApk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnResourceApk.Location = new System.Drawing.Point(750, 152);
            this.btnResourceApk.Name = "btnResourceApk";
            this.btnResourceApk.Size = new System.Drawing.Size(109, 23);
            this.btnResourceApk.TabIndex = 31;
            this.btnResourceApk.Text = "选择资源APK文件";
            this.btnResourceApk.UseVisualStyleBackColor = true;
            this.btnResourceApk.Click += new System.EventHandler(this.btnResourceApk_Click);
            // 
            // cbResourceApk
            // 
            this.cbResourceApk.AutoSize = true;
            this.cbResourceApk.Location = new System.Drawing.Point(246, 156);
            this.cbResourceApk.Name = "cbResourceApk";
            this.cbResourceApk.Size = new System.Drawing.Size(90, 16);
            this.cbResourceApk.TabIndex = 32;
            this.cbResourceApk.Text = "指定资源apk";
            this.cbResourceApk.UseVisualStyleBackColor = true;
            this.cbResourceApk.CheckedChanged += new System.EventHandler(this.cbResourceApk_CheckedChanged);
            // 
            // btnOtherJson
            // 
            this.btnOtherJson.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOtherJson.Location = new System.Drawing.Point(759, 301);
            this.btnOtherJson.Name = "btnOtherJson";
            this.btnOtherJson.Size = new System.Drawing.Size(100, 23);
            this.btnOtherJson.TabIndex = 35;
            this.btnOtherJson.Text = "选择JSON文件";
            this.btnOtherJson.UseVisualStyleBackColor = true;
            this.btnOtherJson.Click += new System.EventHandler(this.btnOtherJson_Click);
            // 
            // cbEnableVersionCode
            // 
            this.cbEnableVersionCode.AutoSize = true;
            this.cbEnableVersionCode.Location = new System.Drawing.Point(468, 74);
            this.cbEnableVersionCode.Name = "cbEnableVersionCode";
            this.cbEnableVersionCode.Size = new System.Drawing.Size(60, 16);
            this.cbEnableVersionCode.TabIndex = 37;
            this.cbEnableVersionCode.Text = "版本号";
            this.cbEnableVersionCode.UseVisualStyleBackColor = true;
            this.cbEnableVersionCode.CheckedChanged += new System.EventHandler(this.cbEnableVersionCode_CheckedChanged);
            // 
            // tbVersionCode
            // 
            this.tbVersionCode.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbVersionCode.Enabled = false;
            this.tbVersionCode.Location = new System.Drawing.Point(536, 72);
            this.tbVersionCode.Name = "tbVersionCode";
            this.tbVersionCode.Size = new System.Drawing.Size(121, 21);
            this.tbVersionCode.TabIndex = 36;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rbPlatformWinPhone);
            this.groupBox1.Controls.Add(this.rbPlatformAndroid);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(230, 71);
            this.groupBox1.TabIndex = 39;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "选择平台";
            // 
            // rbPlatformWinPhone
            // 
            this.rbPlatformWinPhone.AutoSize = true;
            this.rbPlatformWinPhone.Location = new System.Drawing.Point(18, 42);
            this.rbPlatformWinPhone.Name = "rbPlatformWinPhone";
            this.rbPlatformWinPhone.Size = new System.Drawing.Size(125, 16);
            this.rbPlatformWinPhone.TabIndex = 1;
            this.rbPlatformWinPhone.Text = "Windows Phone 8.0";
            this.rbPlatformWinPhone.UseVisualStyleBackColor = true;
            // 
            // rbPlatformAndroid
            // 
            this.rbPlatformAndroid.AutoSize = true;
            this.rbPlatformAndroid.Checked = true;
            this.rbPlatformAndroid.Location = new System.Drawing.Point(18, 19);
            this.rbPlatformAndroid.Name = "rbPlatformAndroid";
            this.rbPlatformAndroid.Size = new System.Drawing.Size(65, 16);
            this.rbPlatformAndroid.TabIndex = 0;
            this.rbPlatformAndroid.TabStop = true;
            this.rbPlatformAndroid.Text = "Android";
            this.rbPlatformAndroid.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(242, 348);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(107, 12);
            this.label8.TabIndex = 40;
            this.label8.Text = "globalres覆盖目录";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(759, 342);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(87, 23);
            this.button1.TabIndex = 42;
            this.button1.Text = "选择glob目录";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(871, 588);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.tbGloOverwrite);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cbEnableVersionCode);
            this.Controls.Add(this.tbVersionCode);
            this.Controls.Add(this.btnOtherJson);
            this.Controls.Add(this.tbOtherJsonFile);
            this.Controls.Add(label7);
            this.Controls.Add(this.cbResourceApk);
            this.Controls.Add(this.btnResourceApk);
            this.Controls.Add(this.tbResourceApk);
            this.Controls.Add(this.cbDeleteGameResDir);
            this.Controls.Add(this.cbEnableVersionName);
            this.Controls.Add(this.tbVersionName);
            this.Controls.Add(this.tbEncryptOverwrite);
            this.Controls.Add(label6);
            this.Controls.Add(this.btnPermissionHelp);
            this.Controls.Add(this.tbPermissionDelete);
            this.Controls.Add(label9);
            this.Controls.Add(this.btnUpdateHistory);
            this.Controls.Add(this.tbNormalOverwrite);
            this.Controls.Add(label5);
            this.Controls.Add(this.btnDecompile);
            this.Controls.Add(this.tbResourceDelete);
            this.Controls.Add(label4);
            this.Controls.Add(this.btnRun);
            this.Controls.Add(this.tbLog);
            this.Controls.Add(this.cbPackageName);
            this.Controls.Add(label3);
            this.Controls.Add(label2);
            this.Controls.Add(this.btnSelectFile);
            this.Controls.Add(this.tbApkPath);
            this.Controls.Add(label1);
            this.Controls.Add(this.clbChannel);
            this.Name = "FormMain";
            this.Text = "TRGAME渠道打包程序";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckedListBox clbChannel;
        private System.Windows.Forms.TextBox tbApkPath;
        private System.Windows.Forms.Button btnSelectFile;
        private System.Windows.Forms.ComboBox cbPackageName;
        private System.Windows.Forms.TextBox tbLog;
        private System.Windows.Forms.Button btnRun;
        private System.Windows.Forms.TextBox tbResourceDelete;
        private System.Windows.Forms.Button btnDecompile;
        private System.Windows.Forms.TextBox tbNormalOverwrite;
        private System.Windows.Forms.Button btnUpdateHistory;
        private System.Windows.Forms.TextBox tbPermissionDelete;
        private System.Windows.Forms.Button btnPermissionHelp;
        private System.Windows.Forms.TextBox tbEncryptOverwrite;
        private System.Windows.Forms.TextBox tbVersionName;
        private System.Windows.Forms.CheckBox cbEnableVersionName;
        private System.Windows.Forms.CheckBox cbDeleteGameResDir;
        private System.Windows.Forms.TextBox tbResourceApk;
        private System.Windows.Forms.Button btnResourceApk;
        private System.Windows.Forms.CheckBox cbResourceApk;
        private System.Windows.Forms.TextBox tbOtherJsonFile;
        private System.Windows.Forms.Button btnOtherJson;
        private System.Windows.Forms.CheckBox cbEnableVersionCode;
        private System.Windows.Forms.TextBox tbVersionCode;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rbPlatformWinPhone;
        private System.Windows.Forms.RadioButton rbPlatformAndroid;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox tbGloOverwrite;
        private System.Windows.Forms.Button button1;
    }
}

