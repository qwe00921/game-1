using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace TRGameRepack
{
    public partial class UpdateHistory : Form
    {
        public UpdateHistory(string title, string fileName)
        {
            InitializeComponent();

            this.Text = title;

            String filePath = Path.Combine(Application.StartupPath, fileName);
            using (StreamReader tr = new StreamReader(filePath, Encoding.UTF8))
            {
                string content = tr.ReadToEnd();
                tbHistory.AppendText(content);
            }
        }

        private void UpdateHistory_Load(object sender, EventArgs e)
        {
        }
    }
}
