using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using TRGameRepack.Repack;

namespace TRGameRepack
{
    public partial class FormMain : Form
    {
        private RepackConfig config = new RepackConfig();

        public FormMain()
        {
            InitializeComponent();            

            config.loadConfig();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            for (int i = 0; i < config.ChannelList.Count; i++)
            {
                clbChannel.Items.Add(config.ChannelList[i].ChannelName);
            }

            for (int i=0; i<config.PackageList.Count; i++)
            {
                cbPackageName.Items.Add(config.PackageList[i].Title);
            }
            cbPackageName.SelectedIndex = 0;

            string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = "TRGAME渠道打包程序" + version;
        }

        private void btnSelectFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "选择 apk or xap 文件";
            ofd.Filter = "应用文件 (*.apk;*.xap)|*.apk;*.xap";
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbApkPath.Text = ofd.FileName;
            }
        }

        private CommandProcess cmd;
        private Thread thread;

        private void btnRun_Click(object sender, EventArgs e)
        {
            if (!checkInput())
            {
                return;
            }

            cmd = new CommandProcess(config);
            cmd.OutputDataReceived += DataReceived;
            cmd.SourceFile = tbApkPath.Text;
            cmd.Package = config.PackageList[cbPackageName.SelectedIndex];
            cmd.DeleteGameResDir = cbDeleteGameResDir.Checked;
            if (tbResourceApk.Enabled)
            {
                cmd.ResourceApkPath = tbResourceApk.Text;
            }
            if (tbVersionName.Enabled)
            {
                cmd.VersionName = tbVersionName.Text;
            }
            if (tbVersionCode.Enabled)
            {
                cmd.VersionCode = int.Parse(tbVersionCode.Text);
            }
            {
                cmd.DeletePath.Clear();
                string[] strArr = tbResourceDelete.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    string item = strItem.Trim();
                    if (!String.IsNullOrEmpty(item))
                    {
                        cmd.DeletePath.Add(item);
                    }
                }
            }
            {
                cmd.EncryptOverwrite.Clear();
                string[] strArr = tbEncryptOverwrite.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    string item = strItem.Trim();
                    if (!String.IsNullOrEmpty(item))
                    {
                        cmd.EncryptOverwrite.Add(item);
                    }
                }
            }
            {
                cmd.NormalOverwrite.Clear();
                string[] strArr = tbNormalOverwrite.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    string item = strItem.Trim();
                    if (!String.IsNullOrEmpty(item))
                    {
                        cmd.NormalOverwrite.Add(item);
                    }
                }
            }
            {
                cmd.OtherJsonFile.Clear();
                string[] strArr = tbOtherJsonFile.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    string item = strItem.Trim();
                    if (!String.IsNullOrEmpty(item))
                    {
                        cmd.OtherJsonFile.Add(item);
                    }
                }
            }
            {
                cmd.GlobalresOverwrite.Clear();
                string[] strArr = tbGloOverwrite.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    string item = strItem.Trim();
                    if (!String.IsNullOrEmpty(item))
                    {
                        cmd.GlobalresOverwrite.Add(item);
                    }
                }
            }
            {
                cmd.PermissionDelete.Clear();
                string[] strArr = tbPermissionDelete.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    string trimItem = strItem.Trim();
                    if (!String.IsNullOrEmpty(trimItem))
                    {
                        cmd.PermissionDelete.Add(trimItem);
                    }
                }
            }
            foreach (int index in clbChannel.CheckedIndices)
            {
                cmd.ChannelList.Add(config.ChannelList[index]);
            }

            thread = new Thread(new ThreadStart(delegate() {
                if (rbPlatformAndroid.Checked)
                {
                    cmd.runAndroidCommand();
                }
                else if (rbPlatformWinPhone.Checked)
                {
                    cmd.runWinPhoneCommand();
                }                
                this.Invoke((ThreadStart)delegate()
                    {
                        MessageBox.Show(this, "打包完成");
                    });
            }));
            thread.IsBackground = true;
            thread.Start();
        }

        private void DataReceived(String text)
        {
            tbLog.Invoke((ThreadStart)delegate()
            {
                if (text != null)
                {
                    tbLog.AppendText(text + "\r\n");
                }
            });
        }

        private bool checkInput()
        {
            if (!File.Exists(tbApkPath.Text))
            {
                MessageBox.Show("源文件不存在");
                tbApkPath.Focus();
                return false;
            }

            if (rbPlatformAndroid.Checked)
            {
                if (!tbApkPath.Text.EndsWith(".apk"))
                {
                    MessageBox.Show("源文件必须为apk文件");
                    tbApkPath.Focus();
                    return false;
                }
            }
            else if (rbPlatformWinPhone.Checked)
            {
                if (!tbApkPath.Text.EndsWith(".xap"))
                {
                    MessageBox.Show("源文件必须为xap文件");
                    tbApkPath.Focus();
                    return false;
                }
            }

            if (tbResourceApk.Enabled)
            {
                if (!File.Exists(tbResourceApk.Text))
                {
                    MessageBox.Show("指定资源文件不存在");
                    tbResourceApk.Focus();
                    return false;
                }

                if (rbPlatformAndroid.Checked)
                {
                    if (!tbResourceApk.Text.EndsWith(".apk"))
                    {
                        MessageBox.Show("资源文件必须为apk文件");
                        tbResourceApk.Focus();
                        return false;
                    }
                }
                else if (rbPlatformWinPhone.Checked)
                {
                    if (!tbResourceApk.Text.EndsWith(".xap"))
                    {
                        MessageBox.Show("资源文件必须为xap文件");
                        tbResourceApk.Focus();
                        return false;
                    }
                }
            }            

            if (clbChannel.CheckedIndices.Count == 0)
            {
                MessageBox.Show("请选择渠道");
                clbChannel.Focus();
                return false;
            }

            if (tbVersionName.Enabled)
            {
                tbVersionName.Text = tbVersionName.Text.Trim();
                if (String.IsNullOrEmpty(tbVersionName.Text))
                {
                    MessageBox.Show("版本名不能为空");
                    tbVersionName.Focus();
                    return false;
                }
            }
            if (tbVersionCode.Enabled)
            {
                int verCode = 0;
                if (int.TryParse(tbVersionCode.Text, out verCode) && verCode > 0)
                {

                }
                else
                {
                    MessageBox.Show("版本号必须为大于0的整数");
                    tbVersionCode.Focus();
                    return false;
                }
            }

            if (!String.IsNullOrEmpty(tbEncryptOverwrite.Text))
            {
                string[] strArr = tbEncryptOverwrite.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    if (!String.IsNullOrEmpty(strItem) && !Directory.Exists(strItem))
                    {
                        MessageBox.Show(String.Format("加密覆盖目录 '{0}' 不存在", strItem));
                        tbNormalOverwrite.Focus();
                        return false;
                    }
                }
            }

            if (!String.IsNullOrEmpty(tbNormalOverwrite.Text))
            {
                string[] strArr = tbNormalOverwrite.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    if (!String.IsNullOrEmpty(strItem) && !Directory.Exists(strItem))
                    {
                        MessageBox.Show(String.Format("普通覆盖目录 '{0}' 不存在", strItem));
                        tbNormalOverwrite.Focus();
                        return false;
                    }
                }
            }

            if (!String.IsNullOrEmpty(tbOtherJsonFile.Text))
            {
                string[] strArr = tbOtherJsonFile.Text.Split(';');
                foreach (string strItem in strArr)
                {
                    if (!String.IsNullOrEmpty(strItem) && !File.Exists(strItem))
                    {
                        MessageBox.Show(String.Format("附加JSON文件 '{0}' 不存在", strItem));
                        tbOtherJsonFile.Focus();
                        return false;
                    }
                }
            }

            return true;
        }

        private void btnDecompile_Click(object sender, EventArgs e)
        {
            if (!File.Exists(tbApkPath.Text))
            {
                MessageBox.Show("源文件不存在");
                tbApkPath.Focus();
                return;
            }

            if (rbPlatformAndroid.Checked)
            {
                if (!tbApkPath.Text.EndsWith(".apk"))
                {
                    MessageBox.Show("源文件必须为apk文件");
                    tbApkPath.Focus();
                    return;
                }
            }
            else if (rbPlatformWinPhone.Checked)
            {
                if (!tbApkPath.Text.EndsWith(".xap"))
                {
                    MessageBox.Show("源文件必须为xap文件");
                    tbApkPath.Focus();
                    return;
                }
            }

            cmd = new CommandProcess(config);
            cmd.OutputDataReceived += DataReceived;
            cmd.SourceFile = tbApkPath.Text;
            cmd.Package = config.PackageList[cbPackageName.SelectedIndex];
            List<String> channelIds = new List<string>();

            thread = new Thread(new ThreadStart(delegate()
            {
                if (rbPlatformAndroid.Checked)
                {
                    cmd.runAndroidDecompile();
                }
                else if (rbPlatformWinPhone.Checked)
                {
                    cmd.runWinPhoneDecompile();
                }
                this.Invoke((ThreadStart)delegate()
                {
                    cbEnableVersionName.Checked = false;
                    tbVersionName.Text = cmd.VersionName;
                    tbVersionCode.Text = cmd.VersionCode.ToString();
                    MessageBox.Show(this, "反编译完成");
                });
            }));
            thread.IsBackground = true;
            thread.Start();
        }

        private void cbPackageName_SelectedIndexChanged(object sender, EventArgs e)
        {
            tbResourceDelete.Text = config.PackageList[cbPackageName.SelectedIndex].ResourceDelete;
            tbEncryptOverwrite.Text = config.PackageList[cbPackageName.SelectedIndex].ResourceEncrypt;
            tbNormalOverwrite.Text = config.PackageList[cbPackageName.SelectedIndex].ResourceNormal;
        }

        private void btnUpdateHistory_Click(object sender, EventArgs e)
        {
            UpdateHistory uh = new UpdateHistory("更新历史记录", "readme.txt");
            uh.ShowDialog(this);
        }

        private void btnPermissionHelp_Click(object sender, EventArgs e)
        {
            UpdateHistory uh = new UpdateHistory("权限说明", "permission.txt");
            uh.ShowDialog(this);
        }

        private void cbEnableVersionName_CheckedChanged(object sender, EventArgs e)
        {
            tbVersionName.Enabled = cbEnableVersionName.Checked;
        }

        private void btnResourceApk_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "选择资源 apk or xap 文件";
            ofd.Filter = "应用文件 (*.apk;*.xap)|*.apk;*.xap";
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbResourceApk.Text = ofd.FileName;
            }
        }

        private void cbResourceApk_CheckedChanged(object sender, EventArgs e)
        {
            tbResourceApk.Enabled = cbResourceApk.Checked;
            if (cbResourceApk.Checked)
            {
                tbEncryptOverwrite.Text = "";
            }            
        }

        private void btnOtherJson_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "选择json文件";
            ofd.Filter = "json文件 (*.*)|*.*";
            ofd.Multiselect = true;
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string allJson = "";
                foreach (string fileName in ofd.FileNames)
                {
                    if (String.IsNullOrEmpty(allJson))
                    {
                        allJson = fileName;
                    }
                    else
                    {
                        allJson = allJson + ";" + fileName;
                    }
                }
                tbOtherJsonFile.Text = allJson;
            }
        }

        private void cbDeleteGameResDir_CheckedChanged(object sender, EventArgs e)
        {
            tbResourceDelete.Enabled = !cbDeleteGameResDir.Checked;
            if (cbDeleteGameResDir.Checked)
            {
                tbResourceDelete.Text = "";
            }       
        }

        private void cbEnableVersionCode_CheckedChanged(object sender, EventArgs e)
        {
            tbVersionCode.Enabled = cbEnableVersionCode.Checked;
        }

        private void tbNormalOverwrite_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog ofd = new FolderBrowserDialog();
            ofd.Description = "选择资源文件夹";
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbGloOverwrite.Text = ofd.SelectedPath;
            }
        }

        private void tbEncryptOverwrite_TextChanged(object sender, EventArgs e)
        {

        }

    }
}
