namespace TRFontMaker
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
            this.btnTest = new System.Windows.Forms.Button();
            this.pbChar = new System.Windows.Forms.PictureBox();
            this.tbChar = new System.Windows.Forms.TextBox();
            this.btnLoadTRFont = new System.Windows.Forms.Button();
            this.btnShow = new System.Windows.Forms.Button();
            this.btnLoadFontMaker = new System.Windows.Forms.Button();
            this.btnLoadPixelFont = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pbChar)).BeginInit();
            this.SuspendLayout();
            // 
            // btnTest
            // 
            this.btnTest.Location = new System.Drawing.Point(504, 245);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(71, 41);
            this.btnTest.TabIndex = 0;
            this.btnTest.Text = "Test";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // pbChar
            // 
            this.pbChar.Location = new System.Drawing.Point(27, 30);
            this.pbChar.Name = "pbChar";
            this.pbChar.Size = new System.Drawing.Size(256, 256);
            this.pbChar.TabIndex = 1;
            this.pbChar.TabStop = false;
            // 
            // tbChar
            // 
            this.tbChar.Font = new System.Drawing.Font("SimSun", 16F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tbChar.Location = new System.Drawing.Point(339, 40);
            this.tbChar.MaxLength = 1;
            this.tbChar.Name = "tbChar";
            this.tbChar.Size = new System.Drawing.Size(100, 32);
            this.tbChar.TabIndex = 2;
            // 
            // btnLoadTRFont
            // 
            this.btnLoadTRFont.Location = new System.Drawing.Point(41, 340);
            this.btnLoadTRFont.Name = "btnLoadTRFont";
            this.btnLoadTRFont.Size = new System.Drawing.Size(125, 41);
            this.btnLoadTRFont.TabIndex = 3;
            this.btnLoadTRFont.Text = "加载TR原有字库";
            this.btnLoadTRFont.UseVisualStyleBackColor = true;
            this.btnLoadTRFont.Click += new System.EventHandler(this.btnLoadTRFont_Click);
            // 
            // btnShow
            // 
            this.btnShow.Location = new System.Drawing.Point(504, 37);
            this.btnShow.Name = "btnShow";
            this.btnShow.Size = new System.Drawing.Size(71, 41);
            this.btnShow.TabIndex = 4;
            this.btnShow.Text = "显示";
            this.btnShow.UseVisualStyleBackColor = true;
            this.btnShow.Click += new System.EventHandler(this.btnShow_Click);
            // 
            // btnLoadFontMaker
            // 
            this.btnLoadFontMaker.Location = new System.Drawing.Point(201, 340);
            this.btnLoadFontMaker.Name = "btnLoadFontMaker";
            this.btnLoadFontMaker.Size = new System.Drawing.Size(182, 41);
            this.btnLoadFontMaker.TabIndex = 5;
            this.btnLoadFontMaker.Text = "加载HanzFontMaker字库";
            this.btnLoadFontMaker.UseVisualStyleBackColor = true;
            this.btnLoadFontMaker.Click += new System.EventHandler(this.btnLoadFontMaker_Click);
            // 
            // btnLoadPixelFont
            // 
            this.btnLoadPixelFont.Location = new System.Drawing.Point(410, 340);
            this.btnLoadPixelFont.Name = "btnLoadPixelFont";
            this.btnLoadPixelFont.Size = new System.Drawing.Size(131, 41);
            this.btnLoadPixelFont.TabIndex = 6;
            this.btnLoadPixelFont.Text = "加载PixelFont字库";
            this.btnLoadPixelFont.UseVisualStyleBackColor = true;
            this.btnLoadPixelFont.Click += new System.EventHandler(this.btnLoadPixelFont_Click);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(618, 443);
            this.Controls.Add(this.btnLoadPixelFont);
            this.Controls.Add(this.btnLoadFontMaker);
            this.Controls.Add(this.btnShow);
            this.Controls.Add(this.btnLoadTRFont);
            this.Controls.Add(this.tbChar);
            this.Controls.Add(this.pbChar);
            this.Controls.Add(this.btnTest);
            this.Name = "FormMain";
            this.Text = "TRFontMaker";
            this.Load += new System.EventHandler(this.FormMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pbChar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.PictureBox pbChar;
        private System.Windows.Forms.TextBox tbChar;
        private System.Windows.Forms.Button btnLoadTRFont;
        private System.Windows.Forms.Button btnShow;
        private System.Windows.Forms.Button btnLoadFontMaker;
        private System.Windows.Forms.Button btnLoadPixelFont;
    }
}

