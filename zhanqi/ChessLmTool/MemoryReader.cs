using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool
{
    public class MemoryReader
    {
        private byte[] content;
        private int index;
        private Encoding mEncoding;

        public MemoryReader(byte[] source) : this(source, "UTF-8")
        {
            
        }
        public MemoryReader(byte[] source, String encoding)
        {
            content = source;
            index = 0;
            mEncoding = Encoding.GetEncoding(encoding);
        }

        public MemoryReader setIndex(int i)
        {
            index = i;
            return this;
        }

        public int getIndex()
        {
            return index;
        }

        public int readInt32()
        {
            if (index + 4 > content.Length)
            {
                throw new IndexOutOfRangeException("readInt32,index=" + index + ",content=" + content.Length);
            }
            int ret = content[index] | (content[index + 1] << 8) | (content[index + 2] << 16) | (content[index + 3] << 24);
            index += 4;
            return ret;
        }

        public ushort readUShort()
        {
            if (index + 2 > content.Length)
            {
                throw new IndexOutOfRangeException("readUShort,index=" + index + ",content=" + content.Length);
            }
            ushort ret = (ushort)(content[index] | (content[index + 1] << 8));
            index += 2;
            return ret;
        }

        public byte readByte()
        {
            if (index + 1 > content.Length)
            {
                throw new IndexOutOfRangeException("readByte,index=" + index + ",content=" + content.Length);
            }
            byte ret = content[index];
            index += 1;
            return ret;
        }

        public string readString(int length)
        {
            if (length < 0)
            {
                throw new ArgumentOutOfRangeException("readString,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new IndexOutOfRangeException("readUShort,index=" + index + ",length="+length+",content=" + content.Length);
            }
            int realLength = length;
            for (int i=0; i<=length; i++)
            {
                if (content[index + i] == 0)
                {
                    realLength = i;
                    break;
                }
            }

            string ret = mEncoding.GetString(content, index, realLength);
            index += length;
            ret = ret.Replace("\0", "");
            return ret;
        }

        public void skip(int length)
        {
            if (length < 0)
            {
                throw new ArgumentOutOfRangeException("skip,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new IndexOutOfRangeException("skip,index=" + index + ",length=" + length + ",content=" + content.Length);
            }
            index += length;
        }

        public void readBytes(byte[] result)
        {
            readBytes(result, 0, result.Length);
        }

        public void readBytes(byte[] result, int startPos, int length)
        {
            Array.Copy(content, index, result, 0, length);
            index += length;
        }
    }
}
