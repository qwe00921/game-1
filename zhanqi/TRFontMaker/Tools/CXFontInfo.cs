using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace TRFontMaker.Tools
{
    public class CXFontInfo
    {
        public class CXGlyphInfo
        {
            public char code = '\0';
            public ushort width = 0;
            public int endIndex = 0;
            public byte[] array = { };
        };

        public ushort height = 0;
        public ushort size = 0;
        public CXGlyphInfo[] array = { };

        public CXFontInfo(ushort h)
        {
            height = h;
        }

        public CXGlyphInfo findChar(char ch)
        {
            for (int i = 0; i < array.Length; i++)
            {
                if (array[i].code == ch)
                {
                    return array[i];
                }
            }

            return null;
        }

        public void loadFile(string path)
        {
            byte[] content = readFileContent(path);
            MemoryReader mr = new MemoryReader(content);
            size = mr.readUShort();
            array = new CXGlyphInfo[size];
            for (int i = 0; i < size; i++)
            {
                CXGlyphInfo info = new CXGlyphInfo();
                info.code = (char)mr.readUShort();
                info.width = mr.readUShort();
                info.endIndex = mr.readInt32();
                array[i] = info;
            }
            for (int i = 0; i < size; i++)
            {
                int bufLen = (array[i].width + 7) / 8 * height;
                Debug.Assert(bufLen == array[i].endIndex - mr.getIndex());
                array[i].array = new byte[bufLen];
                mr.readBytes(array[i].array);
            }
        }

        public void saveFile(string path)
        {
            array[0].endIndex = 2 + size * 8 + array[0].array.Length;
            for (int i = 1; i < size; i++)
            {
                array[i].endIndex = array[i - 1].endIndex + array[i].array.Length;
            }

            MemoryWriter mw = new MemoryWriter();
            mw.writeUShort(size);
            for (int i = 0; i < size; i++)
            {
                CXGlyphInfo info = array[i];
                mw.writeUShort((ushort)info.code);
                mw.writeUShort(info.width);
                mw.writeInt32(info.endIndex);
            }
            for (int i = 0; i < size; i++)
            {
                mw.writeBytes(array[i].array);
            }

            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                fs.Write(mw.getContent(), 0, mw.getIndex());
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

        public DPixelFont ToPixelFont()
        {
            DPixelFont pf = new DPixelFont();

            pf.FontName = "CXFontInfo";
            pf.MapHeight = this.height;
            pf.FontHeight = this.height;
            pf.MapWidth = this.height;
            pf.CharArray = new DPixelFont.CharItem[this.size];
            for (int i = 0; i < pf.CharArray.Length; i++)
            {
                DPixelFont.CharItem item = new DPixelFont.CharItem();
                item.Code = array[i].code;
                item.Data = array[i].array;
                item.CharWidth = array[i].width;
                pf.CharArray[i] = item;
            }

            return pf;
        }
    };
}
