using System;
using System.Collections.Generic;
using System.Text;

namespace TRFontMaker.Tools
{
    public class MemoryReader
    {
        public const String ENCODING_GBK = "GBK";
        public const String ENCODING_UTF8 = "UTF-8";

        private byte[] content;
        private int index;
        private Encoding defaultEncoding;

        public MemoryReader(byte[] source)
            : this(source, ENCODING_GBK)
        {
        }

        public MemoryReader(byte[] source, String encodingName)
        {
            content = source;
            index = 0;
            defaultEncoding = Encoding.GetEncoding(encodingName);
        }

        public byte readByte()
        {
            if (index + 1 > content.Length)
            {
                throw new TRFontException("readByte,index=" + index + ",content=" + content.Length);
            }
            byte ret = content[index];
            index += 1;
            return ret;
        }

        public int readInt32()
        {
            if (index + 4 > content.Length)
            {
                throw new TRFontException("readInt32,index=" + index + ",content=" + content.Length);
            }
            int ret = content[index] | (content[index + 1] << 8) | (content[index + 2] << 16) | (content[index + 3] << 24);
            index += 4;
            return ret;
        }

        public ushort readUShort()
        {
            if (index + 2 > content.Length)
            {
                throw new TRFontException("readUShort,index=" + index + ",content=" + content.Length);
            }
            ushort ret = (ushort)(content[index] | (content[index + 1] << 8));
            index += 2;
            return ret;
        }

        public string readFixString(int length)
        {
            if (length < 0)
            {
                throw new TRFontException("readString,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new TRFontException(String.Format("readUShort,index={0},length={1},content={2}", index, length, content.Length));
            }
            string ret = defaultEncoding.GetString(content, index, length);
            index += length;
            ret = ret.Replace("\0", "");
            return ret;
        }

        private string readCString()
        {
            int endIndex = content.Length;
            for (int i = index; i < content.Length; i++)
            {
                if (content[i] == 0)
                {
                    endIndex = i + 1;
                    break;
                }
            }
            return readFixString(endIndex - index);
        }        

        private MemoryReader skip(int length)
        {
            if (length < 0)
            {
                throw new TRFontException("skip,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new TRFontException("skip,index=" + index + ",length=" + length + ",content=" + content.Length);
            }
            index += length;
            return this;
        }

        public MemoryReader setIndex(int index)
        {
            this.index = index;
            return this;
        }

        public int getIndex()
        {
            return index;
        }

        public void readBytes(byte[] result)
        {
            readBytes(result, 0, result.Length);
        }

        private void readBytes(byte[] result, int startPos, int length)
        {
            Array.Copy(content, index, result, startPos, length);
            index += length;
        }
    }

    public class TRFontException : ApplicationException
    {
        public int CommandId = 0;

        public TRFontException() : base()
        {
        }
        public TRFontException(string message) : base(message)
        {
        }
        public TRFontException(string message, Exception innerException)
            : base(message, innerException)
        {
        }
    }
}
