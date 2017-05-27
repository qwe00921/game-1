using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using TRFontMaker.Tools;

namespace TRFontMaker
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        DPixelFont pxFont = null;
        Bitmap fontImage = null;

        private void btnTest_Click(object sender, EventArgs e)
        {
            CXFontInfo fi = new CXFontInfo(12);
            fi.loadFile(@"D:\cocos2d-x-2.2.6\projects\trgame\globalres\fonts\font_song_12.bin");
            fi.saveFile(@"D:\cocos2d-x-2.2.6\projects\trgame\globalres\fonts\font_kai_12_bak.bin");
        }

        private void btnLoadTRFont_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "请选择输入TRFont点阵字库路径";
            ofd.Filter = "bin files (*.bin)|*.bin";
            if (ofd.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            try
            {
                ushort fontHeight = getFontHeight(ofd.FileName);
                CXFontInfo info = new CXFontInfo(fontHeight);
                info.loadFile(ofd.FileName);
                pxFont = info.ToPixelFont();
                pxFont.saveFile(Path.Combine(Application.StartupPath, "pxfont.bin"));
            }
            catch (TRFontException)
            {
                MessageBox.Show(this, "加载字体文件失败");
                return;
            }
        }

        private void btnLoadFontMaker_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "请选择输入HanzFontMaker点阵字库路径";
            ofd.Filter = "bin files (*.bin)|*.bin";
            if (ofd.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            try
            {
                HanzFontMaker info = new HanzFontMaker();
                info.loadFile(ofd.FileName);
                pxFont = info.ToPixelFont();
                pxFont.saveFile(Path.Combine(Application.StartupPath, "pxfont.bin"));
            }
            catch (TRFontException)
            {
                MessageBox.Show(this, "加载字体文件失败");
                return;
            }
        }

        private void btnLoadPixelFont_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "请选择输入PixelFont点阵字库路径";
            ofd.Filter = "bin files (*.bin)|*.bin";
            if (ofd.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            try
            {
                DPixelFont info = new DPixelFont();
                info.loadFile(ofd.FileName);
                pxFont = info;
            }
            catch (TRFontException)
            {
                MessageBox.Show(this, "加载字体文件失败");
                return;
            }
        }

        private ushort getFontHeight(string filePath)
        {
            ushort result = 0;
            string fileName = Path.GetFileName(filePath);
            switch (fileName)
            {
                default:
                    result = 0;
                    throw new TRFontException("字体文件名不对");
                case "font_kai_12.bin":
                    result = 12;
                    break;
                case "font_kai_14.bin":
                    result = 14;
                    break;
                case "font_kai_16.bin":
                    result = 16;
                    break;
                case "font_kai_20.bin":
                    result = 20;
                    break;
                case "font_song_12.bin":
                    result = 12;
                    break;
                case "font_song_13.bin":
                    result = 13;
                    break;
                case "font_song_14.bin":
                    result = 14;
                    break;
                case "font_song_15.bin":
                    result = 15;
                    break;
                case "font_song_16.bin":
                    result = 16;
                    break;
                case "font_song_20.bin":
                    result = 20;
                    break;
                case "font_song_border_14.bin":
                    result = 14;
                    break;
                case "font_song_border_16.bin":
                    result = 16;
                    break;
                case "font_song_border_72.bin":
                    result = 71;
                    break;
            }
            return result;
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            fontImage = new Bitmap(256, 256);
            using (Graphics g = Graphics.FromImage(fontImage))
            {
                g.Clear(Color.White);
            }
            pbChar.Image = fontImage;
        }

        private void btnShow_Click(object sender, EventArgs e)
        {
            if (pxFont == null)
            {
                MessageBox.Show(this, "请先加载字体文件");
                return;
            }

            if (tbChar.TextLength != 1)
            {
                return;
            }

            if (!drawChar(pxFont, tbChar.Text[0], fontImage))
            {
                MessageBox.Show(this, "fontInfo找不到'" + tbChar.Text[0].ToString() + "'");
                return;
            }
            pbChar.Invalidate();
        }

        private bool drawChar(DPixelFont fontInfo, char ch, Bitmap bmp)
        {
            DPixelFont.CharItem info = fontInfo.findChar(ch);
            if (info == null)
            {
                return false;
            }

            int gridH = bmp.Height / fontInfo.MapHeight;
            int gridW = gridH;

            using (Graphics g = Graphics.FromImage(bmp))
            {
                g.Clear(Color.White);

                for (int i = 0; i < fontInfo.FontHeight; i++)
                {
                    int startIndex = (info.CharWidth + 7) / 8 * i;
                    for (int j = 0; j < info.CharWidth; j++)
                    {
                        int byteIndex = j / 8 + startIndex;
                        int bitIndex = j % 8;
                        int needDraw = (info.Data[byteIndex] << bitIndex) & 0x80;
                        if (needDraw != 0)
                        {
                            g.FillRectangle(Brushes.Black, j * gridW, i * gridH, gridW, gridH);
                        }
                        else
                        {
                            g.FillRectangle(Brushes.Yellow, j * gridW, i * gridH, gridW, gridH);
                        }
                    }
                }
            }

            return true;
        }
    }
}
