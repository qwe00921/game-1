namespace SceneEditor.Forms
{
    partial class FormBatchConvert
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.rbXlsxText = new System.Windows.Forms.RadioButton();
            this.rbEexJson = new System.Windows.Forms.RadioButton();
            this.rbEexNew = new System.Windows.Forms.RadioButton();
            this.rbOldEex = new System.Windows.Forms.RadioButton();
            this.btnOpen = new System.Windows.Forms.Button();
            this.cbLoadExcel = new System.Windows.Forms.CheckBox();
            this.cbCommand = new System.Windows.Forms.CheckBox();
            this.tbCommand = new System.Windows.Forms.TextBox();
            this.rbNewEex = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rbNewEex);
            this.groupBox1.Controls.Add(this.rbXlsxText);
            this.groupBox1.Controls.Add(this.rbEexJson);
            this.groupBox1.Controls.Add(this.rbEexNew);
            this.groupBox1.Controls.Add(this.rbOldEex);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(206, 153);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "输出文件格式";
            // 
            // rbXlsxText
            // 
            this.rbXlsxText.AutoSize = true;
            this.rbXlsxText.Location = new System.Drawing.Point(15, 119);
            this.rbXlsxText.Name = "rbXlsxText";
            this.rbXlsxText.Size = new System.Drawing.Size(173, 16);
            this.rbXlsxText.TabIndex = 3;
            this.rbXlsxText.TabStop = true;
            this.rbXlsxText.Text = "输出EXCEL文本文件(*.xlsx)";
            this.rbXlsxText.UseVisualStyleBackColor = true;
            // 
            // rbEexJson
            // 
            this.rbEexJson.AutoSize = true;
            this.rbEexJson.Location = new System.Drawing.Point(15, 97);
            this.rbEexJson.Name = "rbEexJson";
            this.rbEexJson.Size = new System.Drawing.Size(173, 16);
            this.rbEexJson.TabIndex = 2;
            this.rbEexJson.TabStop = true;
            this.rbEexJson.Text = "eex json剧本文件(*.eexjs)";
            this.rbEexJson.UseVisualStyleBackColor = true;
            // 
            // rbEexNew
            // 
            this.rbEexNew.AutoSize = true;
            this.rbEexNew.Checked = true;
            this.rbEexNew.Location = new System.Drawing.Point(15, 75);
            this.rbEexNew.Name = "rbEexNew";
            this.rbEexNew.Size = new System.Drawing.Size(179, 16);
            this.rbEexNew.TabIndex = 1;
            this.rbEexNew.TabStop = true;
            this.rbEexNew.Text = "eex new剧本文件(*.eex_new)";
            this.rbEexNew.UseVisualStyleBackColor = true;
            // 
            // rbOldEex
            // 
            this.rbOldEex.AutoSize = true;
            this.rbOldEex.Location = new System.Drawing.Point(15, 29);
            this.rbOldEex.Name = "rbOldEex";
            this.rbOldEex.Size = new System.Drawing.Size(167, 16);
            this.rbOldEex.TabIndex = 0;
            this.rbOldEex.TabStop = true;
            this.rbOldEex.Text = "旧版本eex剧本文件(*.eex)";
            this.rbOldEex.UseVisualStyleBackColor = true;
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(80, 247);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(78, 32);
            this.btnOpen.TabIndex = 1;
            this.btnOpen.Text = "打开文件";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // cbLoadExcel
            // 
            this.cbLoadExcel.AutoSize = true;
            this.cbLoadExcel.Location = new System.Drawing.Point(27, 181);
            this.cbLoadExcel.Name = "cbLoadExcel";
            this.cbLoadExcel.Size = new System.Drawing.Size(162, 16);
            this.cbLoadExcel.TabIndex = 2;
            this.cbLoadExcel.Text = "加载对应的Excel翻译文件";
            this.cbLoadExcel.UseVisualStyleBackColor = true;
            // 
            // cbCommand
            // 
            this.cbCommand.AutoSize = true;
            this.cbCommand.Location = new System.Drawing.Point(27, 212);
            this.cbCommand.Name = "cbCommand";
            this.cbCommand.Size = new System.Drawing.Size(84, 16);
            this.cbCommand.TabIndex = 3;
            this.cbCommand.Text = "限制指令0x";
            this.cbCommand.UseVisualStyleBackColor = true;
            this.cbCommand.CheckedChanged += new System.EventHandler(this.cbCommand_CheckedChanged);
            // 
            // tbCommand
            // 
            this.tbCommand.Enabled = false;
            this.tbCommand.Location = new System.Drawing.Point(114, 210);
            this.tbCommand.Name = "tbCommand";
            this.tbCommand.Size = new System.Drawing.Size(44, 21);
            this.tbCommand.TabIndex = 4;
            // 
            // rbNewEex
            // 
            this.rbNewEex.AutoSize = true;
            this.rbNewEex.Location = new System.Drawing.Point(15, 53);
            this.rbNewEex.Name = "rbNewEex";
            this.rbNewEex.Size = new System.Drawing.Size(167, 16);
            this.rbNewEex.TabIndex = 4;
            this.rbNewEex.TabStop = true;
            this.rbNewEex.Text = "新版本eex剧本文件(*.eex)";
            this.rbNewEex.UseVisualStyleBackColor = true;
            // 
            // FormBatchConvert
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(236, 304);
            this.Controls.Add(this.tbCommand);
            this.Controls.Add(this.cbCommand);
            this.Controls.Add(this.cbLoadExcel);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.groupBox1);
            this.Name = "FormBatchConvert";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "批量转换";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rbOldEex;
        private System.Windows.Forms.RadioButton rbEexNew;
        private System.Windows.Forms.RadioButton rbEexJson;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.RadioButton rbXlsxText;
        private System.Windows.Forms.CheckBox cbLoadExcel;
        private System.Windows.Forms.CheckBox cbCommand;
        private System.Windows.Forms.TextBox tbCommand;
        private System.Windows.Forms.RadioButton rbNewEex;
    }
}