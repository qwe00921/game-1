namespace SceneEditor.Forms
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
            System.Windows.Forms.Label label2;
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnBatch = new System.Windows.Forms.Button();
            this.tbContent = new System.Windows.Forms.TextBox();
            this.treeViewRoot = new System.Windows.Forms.TreeView();
            label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label2
            // 
            label2.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            label2.Location = new System.Drawing.Point(735, 31);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(173, 102);
            label2.TabIndex = 8;
            label2.Text = "旧指令集包括曹操传，杨家将；\r\n新指令集包括东吴传，封神榜；";
            // 
            // btnLoad
            // 
            this.btnLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnLoad.Location = new System.Drawing.Point(787, 342);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(69, 29);
            this.btnLoad.TabIndex = 1;
            this.btnLoad.Text = "加载";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.Location = new System.Drawing.Point(787, 394);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(69, 29);
            this.btnSave.TabIndex = 2;
            this.btnSave.Text = "保存";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnBatch
            // 
            this.btnBatch.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBatch.Location = new System.Drawing.Point(787, 245);
            this.btnBatch.Name = "btnBatch";
            this.btnBatch.Size = new System.Drawing.Size(69, 29);
            this.btnBatch.TabIndex = 5;
            this.btnBatch.Text = "批量转换";
            this.btnBatch.UseVisualStyleBackColor = true;
            this.btnBatch.Click += new System.EventHandler(this.btnBatch_Click);
            // 
            // tbContent
            // 
            this.tbContent.Location = new System.Drawing.Point(245, 1);
            this.tbContent.MaxLength = 0;
            this.tbContent.Multiline = true;
            this.tbContent.Name = "tbContent";
            this.tbContent.ReadOnly = true;
            this.tbContent.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbContent.Size = new System.Drawing.Size(474, 458);
            this.tbContent.TabIndex = 0;
            // 
            // treeViewRoot
            // 
            this.treeViewRoot.Location = new System.Drawing.Point(2, 1);
            this.treeViewRoot.Name = "treeViewRoot";
            this.treeViewRoot.Size = new System.Drawing.Size(237, 458);
            this.treeViewRoot.TabIndex = 0;
            this.treeViewRoot.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewRoot_AfterSelect);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(929, 461);
            this.Controls.Add(this.treeViewRoot);
            this.Controls.Add(this.tbContent);
            this.Controls.Add(label2);
            this.Controls.Add(this.btnBatch);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Name = "FormMain";
            this.Text = "剧本编辑器";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnBatch;
        private System.Windows.Forms.TextBox tbContent;
        private System.Windows.Forms.TreeView treeViewRoot;
    }
}

