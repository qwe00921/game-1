namespace ChessLmTool
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
            this.btnConvert = new System.Windows.Forms.Button();
            this.cbGame = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnExcelJson = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnConvert
            // 
            this.btnConvert.Location = new System.Drawing.Point(88, 133);
            this.btnConvert.Name = "btnConvert";
            this.btnConvert.Size = new System.Drawing.Size(79, 33);
            this.btnConvert.TabIndex = 0;
            this.btnConvert.Text = "数据解析";
            this.btnConvert.UseVisualStyleBackColor = true;
            this.btnConvert.Click += new System.EventHandler(this.btnConvert_Click);
            // 
            // cbGame
            // 
            this.cbGame.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbGame.FormattingEnabled = true;
            this.cbGame.Location = new System.Drawing.Point(88, 28);
            this.cbGame.Name = "cbGame";
            this.cbGame.Size = new System.Drawing.Size(268, 20);
            this.cbGame.TabIndex = 1;
            this.cbGame.SelectedIndexChanged += new System.EventHandler(this.cbGame_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "游戏名";
            // 
            // btnExcelJson
            // 
            this.btnExcelJson.Location = new System.Drawing.Point(277, 133);
            this.btnExcelJson.Name = "btnExcelJson";
            this.btnExcelJson.Size = new System.Drawing.Size(108, 33);
            this.btnExcelJson.TabIndex = 3;
            this.btnExcelJson.Text = "Excel Json转换";
            this.btnExcelJson.UseVisualStyleBackColor = true;
            this.btnExcelJson.Click += new System.EventHandler(this.btnExcelJson_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(494, 269);
            this.Controls.Add(this.btnExcelJson);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cbGame);
            this.Controls.Add(this.btnConvert);
            this.Name = "FormMain";
            this.Text = "ChessLmTool";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConvert;
        private System.Windows.Forms.ComboBox cbGame;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnExcelJson;
    }
}

