using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SaveEditor
{
    public partial class FormMain : Form
    {
        SaveBuild saveBuild;
        public FormMain()
        {
            InitializeComponent();

            saveBuild = new SaveBuild();
        }

        private void saveFile_Click(object sender, EventArgs e)
        {
            if(! checkInput())
            {
                return;
            }
        }

        private void searchFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "请选择输入文件路径";
            ofd.Filter = "All files (*.*)|*.*";
            if (ofd.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            using (FileStream fs = new FileStream(ofd.FileName, FileMode.Open))
            {
                byte[] content = new byte[fs.Length];
                int readIndex = 0;
                int readLen = fs.Read(content, 0, content.Length - readIndex);
                while (readLen > 0 && readIndex + readLen < content.Length)
                {
                    readIndex += readLen;
                    readLen = fs.Read(content, 0, content.Length - readIndex);
                }

                MemoryReader mr = new MemoryReader(content);
                SaveData sd = new SaveData();
                sd.parse(mr);
            }

            MessageBox.Show(this, "读取完毕");
        }

        private bool checkInput()
        {
            int versionCode = 0;
            if (!Int32.TryParse(tbVersion.Text, out versionCode))
            {
                MessageBox.Show(this, "版本号必须为整数！");
                tbVersion.Focus();
                return false;
            }
            saveBuild.setVersionCode(versionCode);

            tbEventName.Text = tbEventName.Text.Trim();
            if (String.IsNullOrEmpty(tbEventName.Text))
            {
                MessageBox.Show(this, "事件名称不能为空！");
                tbEventName.Focus();
                return false;
            }
            saveBuild.setEventName(tbEventName.Text);

            tbSceneName.Text = tbSceneName.Text.Trim();
            if (String.IsNullOrEmpty(tbSceneName.Text))
            {
                MessageBox.Show(this, "场景名称不能为空！");
                tbSceneName.Focus();
                return false;
            }
            saveBuild.setSceneName(tbSceneName.Text);

            tbBattleName.Text = tbBattleName.Text.Trim();
            if (String.IsNullOrEmpty(tbBattleName.Text))
            {
                MessageBox.Show(this, "战斗名称不能为空！");
                tbBattleName.Focus();
                return false;
            }
            saveBuild.setBattleName(tbBattleName.Text);

            int mainLevelNum = 0;
            if (!Int32.TryParse(tbMainLevel.Text, out mainLevelNum))
            {
                MessageBox.Show(this, "级别必须为整数！");
                tbMainLevel.Focus();
                return false;
            }
            saveBuild.setMainLevel(mainLevelNum);

            int curRoundNum = 0;
            if (!Int32.TryParse(tbCurRound.Text, out curRoundNum))
            {
                MessageBox.Show(this, "战斗回合数必须为整数！");
                tbCurRound.Focus();
                return false;
            }
            saveBuild.setCurRound(curRoundNum);

            saveBuild.setStoreTime(tbStoreTime.Text);
            return true;
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            tbStoreTime.Text = DateTime.Now.ToString();
        }

      

      
    }
}
