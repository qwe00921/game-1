using ChessLmTool.Data;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows.Forms;

namespace ChessLmTool
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void btnConvert_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "请选择输入文件路径";
            ofd.Filter = "LM files (*.LM)|*.LM|E5 files (*.E5)|*.E5|Exe files (*.EXE)|*.EXE";
            if (ofd.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            ToolConfig.Instance.Current = ToolConfig.Instance.GameList[cbGame.SelectedIndex];
            processFile(ofd.FileName);

            MessageBox.Show(this, "转换完毕，请查看 json_save 目录");
        }

        private void processFile(String filePath)
        {
            string fname = filePath;
            byte[] content = readFileContent(fname);
            GameData gd = new GameData();

            switch (Path.GetExtension(fname).ToLower())
            {
                case ".lm":
                    if (gd.parseLM(new MemoryReader(content)))
                    {
                        gd.saveJson();
                    }
                    break;
                case ".e5":
                    if (gd.parseE5(new MemoryReader(content, "GBK")))
                    {
                        {
                            string starFileName = Path.Combine(Path.GetDirectoryName(fname), "Star.e5");
                            byte[] starE5 = readFileContent(starFileName);
                            if (starE5 != null)
                            {
                                gd.parseStarE5(new MemoryReader(starE5, "GBK"));
                            }
                        }

                        {
                            string imsgFileName = Path.Combine(Path.GetDirectoryName(fname), "Imsg.e5");
                            byte[] imsg = readFileContent(imsgFileName);
                            if (imsg != null)
                            {
                                gd.parseImsg(new MemoryReader(imsg, "GBK"));
                            }
                        }                        
                        gd.saveJson();
                    }
                    break;
                case ".exe":
                    if (gd.parseExe(new MemoryReader(content)))
                    {
                        gd.saveJson();
                    }
                    break;
                default:
                    break;
            }
        }

        private static byte[] readFileContent(String filePath)
        {
            if (!File.Exists(filePath))
            {
                return null;
            }

            using (FileStream fs = new FileStream(filePath, FileMode.Open))
            {
                byte[] content = new byte[fs.Length];
                int readIndex = 0;
                int readLen = fs.Read(content, 0, content.Length - readIndex);
                while (readLen > 0 && readIndex + readLen < content.Length)
                {
                    readIndex += readLen;
                    readLen = fs.Read(content, 0, content.Length - readIndex);
                }

                return content;
            }
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            ToolConfig.Instance.loadConfig();
            foreach (GameItem item in ToolConfig.Instance.GameList)
            {
                cbGame.Items.Add(item.game_name);
            }
            cbGame.SelectedIndex = 0;
            ToolConfig.Instance.Current = ToolConfig.Instance.GameList[0];

            string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = "ChessLmTool解析程序：" + version;
            //processFile(@"C:\Users\xujinmao\Desktop\FSB2.1\EKD5.exe");

            //bool result = ExcelJson.ConvertExcel2Json(@"D:\projects\trgame\曹操传\doc\4兵种&技能.xlsx");
            //bool result = ExcelJson.ConvertJson2Excel(@"D:\projects\gametools\ChessLmTool\bin\Debug\excel_save\dic_jobSkill.json");
        }

        private void cbGame_SelectedIndexChanged(object sender, EventArgs e)
        {
            ToolConfig.Instance.Current = ToolConfig.Instance.GameList[cbGame.SelectedIndex];
        }

        private void btnExcelJson_Click(object sender, EventArgs e)
        {
            //ExcelJson.ConvertExcel2Json(@"D:\projects\trgame\曹操传\doc\4兵种&技能.xlsx");
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "请选择输入文件路径";
            ofd.Filter = "Excel files (*.xlsx)|*.xlsx|JSON files (*.json)|*.json";
            ofd.Multiselect = true;
            if (ofd.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            if (ofd.FilterIndex == 1)
            {
                foreach (string fileName in ofd.FileNames)
                {
                    string jsonPath;
                    ExcelJson.ConvertExcel2Json(fileName, out jsonPath);
                    ExcelJson.ConvertJson2Excel(jsonPath);
                }
            }
            else
            {
                foreach (string fileName in ofd.FileNames)
                {
                    string excelPath;
                    ExcelJson.ConvertJson2Excel(fileName, out excelPath);
                    ExcelJson.ConvertExcel2Json(excelPath);
                }
            }

            MessageBox.Show(this, "转换完毕，请查看 excel_save, json_save 目录");
        }        
    }
}
