using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;

namespace TRFontMaker.Tools
{
    public class HanzFontMaker
    {
        public class HanzFontItem : IComparable<HanzFontItem>
        {
            public char code;
            public byte[] array = { };

            public int CompareTo(HanzFontItem other)
            {
                return code.CompareTo(other.code);
            }
        }

        public ushort HdrSize = 0;
        public string DescText = "";
        public ushort CharSize = 0;
        public byte CharStride = 0;
        public byte CharLines = 0;
        public byte MapWidth = 0;
        public byte MapHeight = 0;
        public byte AscWidth = 0;
        public byte AscHeight = 0;
        public byte GbkWidth = 0;
        public byte GbkHeight = 0;
        public byte[] Reserved = new byte[20];

        public Size[] AscFontSize = new Size[96];

        public HanzFontItem[] FontArray = new HanzFontItem[96 + 8178];

        public void loadFile(string path)
        {
            byte[] content = readFileContent(path);

            MemoryReader mr = new MemoryReader(content, MemoryReader.ENCODING_GBK);
            HdrSize = mr.readUShort();
            DescText = mr.readFixString(32);
            CharSize = mr.readUShort();
            CharStride = mr.readByte();
            CharLines = mr.readByte();
            MapWidth = mr.readByte();
            MapHeight = mr.readByte();
            Debug.Assert(MapHeight == CharLines);
            AscWidth = mr.readByte();
            AscHeight = mr.readByte();
            GbkWidth = mr.readByte();
            GbkHeight = mr.readByte();
            mr.readBytes(Reserved);

            for (int i = 0; i < AscFontSize.Length; i++)
            {
                Size sz = new Size();
                sz.Width = mr.readByte();
                sz.Height = mr.readByte();
                AscFontSize[i] = sz;
            }

            for (int i = 0; i < 96; i++)
            {
                FontArray[i] = loadAscFont(mr, CharSize, 0x20 + i);
            }

            for (int j = 0; j < 87; j++)
            {
                for (int h = 0; h < 94; h++)
                {
                    int gbkCode = 0xa1a1 + 0x100 * j + h;
                    int i = 96 + h + 94 * j;
                    FontArray[i] = loadGbkFont(mr, CharSize, gbkCode);
                }
            }

            Array.Sort<HanzFontItem>(FontArray);
        }

        public HanzFontItem findChar(char ch)
        {
            for (int i = 0; i < FontArray.Length; i++)
            {
                if (FontArray[i].code == ch)
                {
                    return FontArray[i];
                }
            }

            return null;
        }

        private byte[] gbkBuffer = new byte[4];
        private Encoding gbkEncoding = Encoding.GetEncoding("GBK");
        private HanzFontItem loadGbkFont(MemoryReader mr, int charSize, int code)
        {
            HanzFontItem ret = new HanzFontItem();
            gbkBuffer[1] = (byte)(code & 0xff);
            gbkBuffer[0] = (byte)((code >> 8) & 0xff);
            gbkBuffer[2] = 0;

            ret.code = gbkEncoding.GetChars(gbkBuffer)[0];
            ret.array = new byte[charSize];
            mr.readBytes(ret.array);

            return ret;
        }

        private HanzFontItem loadAscFont(MemoryReader mr, int charSize, int code)
        {
            HanzFontItem ret = new HanzFontItem();
            gbkBuffer[0] = (byte)(code & 0xff);
            gbkBuffer[1] = 0;

            ret.code = gbkEncoding.GetChars(gbkBuffer)[0];
            ret.array = new byte[charSize];
            mr.readBytes(ret.array);

            return ret;
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

        public DPixelFont ToPixelFont()
        {
            DPixelFont pf = new DPixelFont();

            pf.FontName = "HanzFont";
            pf.FontHeight = this.CharLines;
            pf.MapHeight = this.CharLines;
            pf.MapWidth = this.MapWidth;
            pf.CharArray = new DPixelFont.CharItem[this.FontArray.Length];
            for (int i = 0; i < pf.CharArray.Length; i++)
            {
                DPixelFont.CharItem item = HanzToPixel(FontArray[i]);
                pf.CharArray[i] = item;
            }

            return pf;
        }

        private byte copyLeftBit(byte v, int bitNum)
        {
            if (bitNum <= 0)
            {
                return 0;
            }
            else if (bitNum <= 8)
            {
                byte mask = (byte)(~(0xff >> bitNum));
                byte result = (byte)(v & mask);
                return result;
            }

            return v;
        }
        private DPixelFont.CharItem HanzToPixel(HanzFontItem hanzItem)
        {
            DPixelFont.CharItem ret = new DPixelFont.CharItem();
            ret.Code = hanzItem.code;
            if (hanzItem.code < 128)
            {
                if (hanzItem.code - 0x20 <= AscFontSize.Length)
                {
                    ret.CharWidth = (ushort)AscFontSize[hanzItem.code - 0x20].Width;
                }
                else
                {
                    ret.CharWidth = AscWidth;
                }
                ret.Data = new byte[(ret.CharWidth + 7) / 8 * CharLines];
                for (int i = 0; i < CharLines; i++)
                {
                    int pixelStart = (ret.CharWidth + 7) / 8 * i;
                    int hanzStart = (MapWidth + 7) / 8 * i;

                    for (int j = 0; j < ret.CharWidth; j = j + 8)
                    {
                        ret.Data[pixelStart + j / 8] = copyLeftBit(hanzItem.array[hanzStart + j / 8], ret.CharWidth - j);
                    }
                }
            }
            else
            {
                ret.CharWidth = this.MapWidth;
                ret.Data = hanzItem.array;
            }

            return ret;
        }
    }
}
