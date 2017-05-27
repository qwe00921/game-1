using SceneEditor.Datas;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace SceneEditor.Forms
{
    public partial class FormBatchConvert : Form
    {
        public FormBatchConvert()
        {
            InitializeComponent();
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            int commandId = -1;
            if (tbCommand.Enabled)
            {
                try
                {
                    commandId = Convert.ToInt32(tbCommand.Text, 16);
                    if (commandId < 0)
                    {
                        MessageBox.Show(this, "指令格式必须为16进制的数字");
                        tbCommand.Focus();
                        return;
                    }
                }
                catch (Exception)                
                {
                    MessageBox.Show(this, "指令格式必须为16进制的数字");
                    tbCommand.Focus();
                    return;
                }
            }

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "打开剧本文件";
            ofd.Multiselect = true;
            ofd.Filter = "旧版本eex剧本文件(*.eex)|*.eex|新版本eex剧本文件(*.eex)|*.eex|eex new剧本文件(*.eex_new)|*.eex_new|eex json剧本文件(*.eexjs)|*.eexjs";
            ofd.Multiselect = true;
            if (ofd.ShowDialog() != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }

            string loadExcelPath = "";
            if (cbLoadExcel.Checked)
            {
                FolderBrowserDialog fbdLoad = new FolderBrowserDialog();
                fbdLoad.Description = "选择Excel翻译文件目录";
                if (fbdLoad.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                {
                    return;
                }
                loadExcelPath = fbdLoad.SelectedPath;
            }

            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "选择输出文件目录";
            if (fbd.ShowDialog() != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }

            int inGroup = 0;
            if (ofd.FilterIndex == 2)
            {
                inGroup = 1;
            }
            int outGroup = 0;
            if (rbNewEex.Checked)
            {
                outGroup = 1;
            }
            foreach (string inFilePath in ofd.FileNames)
            {
                try
                {
                    ScriptRoot sr = EexFile.loadFile(inFilePath, inGroup);

                    if(commandId>=0 && !sr.containCommand(commandId))
                    {
                        continue;
                    }

                    if (!String.IsNullOrEmpty(loadExcelPath))
                    {
                        string excelPath = Path.Combine(loadExcelPath, Path.GetFileNameWithoutExtension(inFilePath) + ".xlsx");
                        if (File.Exists(excelPath))
                        {
                            bool result = EexFile.loadStringFromExcel(sr, excelPath);
                            if (!result)
                            {
                                MessageBox.Show(this, String.Format("加载文件{0}出错，行数太少", excelPath));
                                return;
                            }
                        }
                    }

                    if (rbXlsxText.Checked)
                    {
                        EexFile.saveStringToExcel(sr, Path.Combine(fbd.SelectedPath, getOutFileName(inFilePath)));
                    }
                    else
                    {
                        EexFile.saveFile(sr, Path.Combine(fbd.SelectedPath, getOutFileName(inFilePath)), outGroup);
                    }
                }
                catch (EexReaderException exception)
                {
                    MessageBox.Show(this, String.Format("文件{0}，解析指令0x{1:x}出错，内部信息为:{2}", inFilePath, exception.CommandId, exception.Message));
                    return;
                }
            }
            MessageBox.Show(this, "转换完成");
        }

        private String getOutFileName(String inFileName)
        {
            String fileNameNoExt = Path.GetFileNameWithoutExtension(inFileName);
            if (rbOldEex.Checked || rbNewEex.Checked)
            {
                return fileNameNoExt + EexFile.getFileExtension(EexFile.EexFileType.TYPE_EEX);
            }
            if (rbEexNew.Checked)
            {
                return fileNameNoExt + EexFile.getFileExtension(EexFile.EexFileType.TYPE_EEX_NEW);
            }
            if (rbEexJson.Checked)
            {
                return fileNameNoExt + EexFile.getFileExtension(EexFile.EexFileType.TYPE_EEXJS);
            }
            if (rbXlsxText.Checked)
            {
                return fileNameNoExt + ".xlsx";
            }
            return fileNameNoExt + EexFile.getFileExtension(EexFile.EexFileType.TYPE_UNKNOW);
        }

        private void cbCommand_CheckedChanged(object sender, EventArgs e)
        {
            tbCommand.Enabled = cbCommand.Checked;
        }
    }
}
