using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace ResourceEncrypt
{
    public partial class FormMain : Form
    {
        SecretFile secretFile;
        private Hashtable mPartEncExt = new Hashtable();
        private Hashtable mAllEncExt = new Hashtable();
        private int mPartEncLength = 0;

        public FormMain()
        {
            InitializeComponent();

            secretFile = new SecretFile();
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
            secretFile.setVersionCode(versionCode);

            String codeKeyText = tbCodeKey.Text.Trim();
            if (codeKeyText.Length < 4 || codeKeyText.Length > 16)
            {
                MessageBox.Show(this, "代码密钥必须在4-16位之间！");
                tbCodeKey.Focus();
                return false;
            }
            secretFile.setCodeKey(codeKeyText);

            tbDirSource.Text = tbDirSource.Text.Trim();
            if (!Directory.Exists(tbDirSource.Text))
            {
                MessageBox.Show(this, "源目录不存在！");
                tbDirSource.Focus();
                return false;
            }

            if (!cbWriteSource.Checked)
            {
                tbDirDest.Text = tbDirDest.Text.Trim();
                if (!Directory.Exists(tbDirDest.Text))
                {
                    MessageBox.Show(this, "目标目录不存在！");
                    tbDirDest.Focus();
                    return false;
                }
            }

            mAllEncExt.Clear();
            {
                String[] allEncArray = tbAllFileExt.Text.Trim().Split(';');
                foreach (String item in allEncArray)
                {
                    if (!String.IsNullOrEmpty(item))
                    {
                        mAllEncExt.Add("." + item, item);
                    }
                }
            }
            mPartEncExt.Clear();
            {
                String[] partEncArray = tbPartFileExt.Text.Trim().Split(';');
                foreach (String item in partEncArray)
                {
                    if (!String.IsNullOrEmpty(item))
                    {
                        mPartEncExt.Add("." + item, item);
                    }
                }
            }
            int partLen = 0;
            if (!Int32.TryParse(tbPartLen.Text, out partLen))
            {
                MessageBox.Show(this, "部分加密长度需要为整数！");
                tbPartLen.Focus();
                return false;
            }
            mPartEncLength = partLen;

            return true;
        }

        private void btnEncrypt_Click(object sender, EventArgs e)
        {
            if (!checkInput())
            {
                return;
            }

            tbLog.Text = String.Empty;

            Thread thread = new Thread(encryptAll);
            thread.IsBackground = true;
            thread.Start();
        }

        private void encryptAll()
        {
            String sourceDir = tbDirSource.Text;
            String destDir = tbDirDest.Text;
            String[] allFiles = Directory.GetFiles(tbDirSource.Text, "*.*", SearchOption.AllDirectories);

            if (cbWriteSource.Checked)
            {
                foreach (String filePath in allFiles)
                {
                    encryptFile(filePath, filePath);
                    int encLen = getFileEncLength(filePath);
                    addLog("加密" + encLen + "," + filePath + "完成！");
                }
            }
            else
            {
                foreach (String filePath in allFiles)
                {
                    String relativePath = getRelativePath(filePath, sourceDir);
                    if (String.IsNullOrEmpty(relativePath))
                    {
                        addLog("失败" + filePath + "！");
                    }
                    else
                    {
                        encryptFile(filePath, Path.Combine(destDir, relativePath));
                        int encLen = getFileEncLength(filePath);
                        addLog("加密" + encLen + "," + filePath + "完成！");
                    }
                }
            }
            addLog("加密文件处理完成！");
        }

        private void decryptAll()
        {
            String sourceDir = tbDirSource.Text;
            String destDir = tbDirDest.Text;
            String[] allFiles = Directory.GetFiles(tbDirSource.Text, "*.*", SearchOption.AllDirectories);
                        
            if (cbWriteSource.Checked)
            {
                foreach (String filePath in allFiles)
                {
                    decryptFile(filePath, filePath);
                    addLog("解密" + filePath + "完成！");
                }
            }
            else
            {
                foreach (String filePath in allFiles)
                {
                    String relativePath = getRelativePath(filePath, sourceDir);
                    if (String.IsNullOrEmpty(relativePath))
                    {
                        addLog("失败" + filePath + "！");
                    }
                    else
                    {
                        decryptFile(filePath, Path.Combine(destDir, relativePath));
                        addLog("解密" + relativePath + "完成！");
                    }
                }
            }
            addLog("解密文件处理完成！");
        }

        private void btnDecrypt_Click(object sender, EventArgs e)
        {
            if (!checkInput())
            {
                return;
            }

            tbLog.Text = String.Empty;

            Thread thread = new Thread(decryptAll);
            thread.IsBackground = true;
            thread.Start();
        }

        private String getRelativePath(String filePath, String dirPath)
        {
            filePath = Path.Combine(Path.GetDirectoryName(filePath), Path.GetFileName(filePath));
            dirPath = Path.Combine(Path.GetDirectoryName(dirPath), Path.GetFileName(dirPath));

            if (filePath.StartsWith(dirPath))
            {
                return filePath.Substring(dirPath.Length + 1);
            }
            return "";
        }

        private int getFileEncLength(String sourcePath)
        {
            String extName = Path.GetExtension(sourcePath);
            if (mPartEncExt.ContainsKey(extName))
            {
                return mPartEncLength;
            }

            if (mAllEncExt.ContainsKey(extName))
            {
                FileInfo fi = new FileInfo(sourcePath);
                return (int)fi.Length;
            }

            return -1;
        }

        private void encryptFile(String sourcePath, String destPath)
        {
            if (!Directory.Exists(Path.GetDirectoryName(destPath)))
            {
                Directory.CreateDirectory(Path.GetDirectoryName(destPath));
            }

            int encLength = getFileEncLength(sourcePath);
            if (encLength == -1)
            {
                if (!sourcePath.Equals(destPath))
                {
                    File.Delete(destPath);
                    File.Copy(sourcePath, destPath);
                }
                return;
            }

            byte[] fileBuffer = null;
            //读取文件
            using (FileStream fs = new FileStream(sourcePath, FileMode.Open))
            {
                int readIndex = 0;
                fileBuffer = new byte[fs.Length];
                int readLen = fs.Read(fileBuffer, readIndex, fileBuffer.Length - readIndex);
                while (readLen > 0 && readLen + readIndex < fileBuffer.Length)
                {
                    readIndex += readLen;
                    readLen = fs.Read(fileBuffer, readIndex, fileBuffer.Length - readIndex);
                }
            }

            secretFile.saveFile(fileBuffer, destPath, encLength);       
        }

        private bool decryptFile(String sourcePath, String destPath)
        {
            String destDir = Path.GetDirectoryName(destPath);
            if (!Directory.Exists(destDir))
            {
                Directory.CreateDirectory(destDir);
            }

            int encLength = getFileEncLength(sourcePath);
            if (encLength == -1)
            {
                if (!sourcePath.Equals(destPath))
                {
                    File.Delete(destPath);
                    File.Copy(sourcePath, destPath);
                }
                return true;
            }

            //解密文件
            byte[] fileBuffer = null;
            SecretFile.ReadFileResult result = secretFile.loadFile(ref fileBuffer, sourcePath);
            if (result != SecretFile.ReadFileResult.Succeed)
            {
                addLog(sourcePath + "失败=" + result.ToString());
                return false;
            }

            //写入文件
            using (FileStream fs = new FileStream(destPath, FileMode.Create))
            {
                fs.Write(fileBuffer, 0, fileBuffer.Length);
            }
            return true;
        }

        private delegate void LogAction();
        private void addLog(String logText)
        {
            LogAction logMethod = new LogAction(delegate() 
                { 
                    tbLog.AppendText(logText + "\r\n"); 
                });

            tbLog.Invoke(logMethod);            
        }

        private void cbWriteSource_CheckedChanged(object sender, EventArgs e)
        {
            if (cbWriteSource.Checked)
            {
                tbDirDest.Text = String.Empty;
                tbDirDest.Enabled = false;
            }
            else
            {
                tbDirDest.Enabled = true;
            }
        }

        private void btnDirSource_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderDlg = new FolderBrowserDialog();
            if (folderDlg.ShowDialog() == DialogResult.OK)
            {
                tbDirSource.Text = folderDlg.SelectedPath;
            }
        }

        private void btnDirDest_Click(object sender, EventArgs e)
        {
            if (!tbDirDest.Enabled)
            {
                return;
            }

            FolderBrowserDialog folderDlg = new FolderBrowserDialog();
            if (folderDlg.ShowDialog() == DialogResult.OK)
            {
                tbDirDest.Text = folderDlg.SelectedPath;
            }
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = "资源加密和解密工具：" + version;
        }
    }
}
