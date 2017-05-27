using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using TRGameCheck.Command;

namespace TRGameCheck
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void btnRun_Click(object sender, EventArgs e)
        {
            tbLog.AppendText(DateTime.Now.ToString() + " 开始检查\r\n");
            try
            {
                CommandConfig ccfg = new CommandConfig();
                ccfg.init(tbCheckPath.Text);
                ccfg.ShowAllLog = cbAllLog.Checked;

                CommandList cl = new CommandList();
                cl.CheckFileExist = cbFileExist.Checked;
                cl.CheckFileName = cbFileName.Checked;
                cl.CheckJson = cbJson.Checked;
                cl.CheckScene = cbScene.Checked;
                cl.CheckStrings = cbStrings.Checked;
                cl.CheckGlobalRes = cbGlobalRes.Checked;
                cl.OutputDataReceived += DataReceived;
                if (cl.loadXml("commandlist.xml"))
                {
                    cl.runCmd(ccfg);
                }                
            }
            catch (Exception ex)
            {
                logText(ex.ToString());
            }
        }

        private void logText(string text)
        {
            if (text != null)
            {
                tbLog.AppendText(text + "\r\n");
            }
        }

        private void DataReceived(String text)
        {
            tbLog.Invoke((ThreadStart)delegate()
            {
                logText(text);
            });
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = "TRGameCheck数据检查程序：" + version;
        }

        private void btnClean_Click(object sender, EventArgs e)
        {
            tbLog.Clear();
        }
    }
}
