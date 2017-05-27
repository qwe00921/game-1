using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace TRFontMaker.Tools
{
    public class DPixelFont
    {
        public class CharItem
        {
            public char Code = '\0';
            public ushort CharWidth = 0;
            public byte[] Data = { };
        }

        //public string FileHead = "";
        //public int FileVersion = 0;
        private const string FILE_HEAD = "TRPF";
        private const int FILE_VERSION = 1;
        public string FontName = "";
        public int FontHeight = 0;
        public int MapWidth = 0;
        public int MapHeight = 0;
        public CharItem[] CharArray = { };

        public CharItem findChar(char ch)
        {
            for (int i = 0; i < CharArray.Length; i++)
            {
                if (CharArray[i].Code == ch)
                {
                    return CharArray[i];
                }
            }

            return null;
        }

        public void loadFile(string path)
        {
            byte[] content = readFileContent(path);
            MemoryReader mr = new MemoryReader(content, MemoryReader.ENCODING_UTF8);

            string FileHead = mr.readFixString(4);
            int FileVersion = mr.readInt32();
            FontName = mr.readFixString(32);
            FontHeight = mr.readInt32();
            MapWidth = mr.readInt32();
            MapHeight = mr.readInt32();

            int arrLen = mr.readInt32();
            CharArray = new CharItem[arrLen];
            for (int i = 0; i < arrLen; i++)
            {
                CharItem item = new CharItem();
                item.Code = (char)mr.readUShort();
                item.CharWidth = mr.readUShort();
                CharArray[i] = item;
            }
            for (int i = 0; i < arrLen; i++)
            {
                int bufLen = (CharArray[i].CharWidth + 7) / 8 * FontHeight;
                CharArray[i].Data = new byte[bufLen];
                mr.readBytes(CharArray[i].Data);
            }
        }

        public void saveFile(string path)
        {
            MemoryWriter mw = new MemoryWriter(MemoryWriter.ENCODING_UTF8);
            mw.writeFixString(FILE_HEAD, 4);
            mw.writeInt32(FILE_VERSION);
            mw.writeFixString(FontName, 32);
            mw.writeInt32(FontHeight);
            mw.writeInt32(MapWidth);
            mw.writeInt32(MapHeight);

            mw.writeInt32(CharArray.Length);
            for (int i = 0; i < CharArray.Length; i++)
            {
                CharItem item = CharArray[i];
                mw.writeUShort((ushort)item.Code);
                mw.writeUShort(item.CharWidth);
            }
            for (int i = 0; i < CharArray.Length; i++)
            {
                mw.writeBytes(CharArray[i].Data);
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
    }
}
