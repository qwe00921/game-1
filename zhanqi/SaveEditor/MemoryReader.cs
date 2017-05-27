using System;
using System.Collections.Generic;
using System.Text;

namespace SaveEditor
{
    class MemoryReader
    {
        private byte[] content;
        private int index;
        public MemoryReader(byte[] source)
        {
            content = source;
            index = 0;
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

        public string readString(int length)
        {
            if (length < 0)
            {
                throw new ArgumentOutOfRangeException("readString,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new IndexOutOfRangeException("readUShort,index=" + index + ",length=" + length + ",content=" + content.Length);
            }
            string ret = Encoding.UTF8.GetString(content, index, length);
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