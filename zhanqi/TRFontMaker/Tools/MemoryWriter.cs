using System;
using System.Collections.Generic;
using System.Text;

namespace TRFontMaker.Tools
{
    public class MemoryWriter
    {
        public const String ENCODING_GBK = "GBK";
        public const String ENCODING_UTF8 = "UTF-8";

        private byte[] content;
        private int index;
        private Encoding defaultEncoding;

        public MemoryWriter() : this(ENCODING_GBK)
        {
        }

        public MemoryWriter(String encodingName)
        {
            content = new byte[1024];
            index = 0;
            defaultEncoding = Encoding.GetEncoding(encodingName);
        }

        public MemoryWriter setIndex(int i)
        {
            reAlloc(i + 1);
            index = i;
            return this;
        }

        public int getIndex()
        {
            return index;
        }

        public byte[] getContent()
        {
            return content;
        }

        public void skip(int len)
        {
            setIndex(index + len);
        }

        public void writeBytes(byte[] result)
        {
            writeBytes(result, 0, result.Length);
        }

        private void writeBytes(byte[] result, int startPos, int len)
        {
            if (index + len >= content.Length)
            {
                int newCapacity1 = index + len + 1;
                int newCapacity2 = content.Length * 2 + 1;
                reAlloc(Math.Max(newCapacity1, newCapacity2));
            }

            Array.Copy(result, startPos, content, index, len);
            index += len;
        }

        private void reAlloc(int newLen)
        {
            if (newLen <= content.Length)
            {
                return;
            }

            byte[] newContent = new byte[newLen];
            Array.Copy(content, newContent, index);
            content = newContent;
        }

        public void writeInt32(int v)
        {
            byte[] buf = new byte[4];
            buf[0] = (byte)(v & 0xff);
            buf[1] = (byte)((v >> 8) & 0xff);
            buf[2] = (byte)((v >> 16) & 0xff);
            buf[3] = (byte)((v >> 24) & 0xff);

            writeBytes(buf);
        }

        public void writeUShort(ushort v)
        {
            byte[] buf = new byte[2];
            buf[0] = (byte)(v & 0xff);
            buf[1] = (byte)((v >> 8) & 0xff);

            writeBytes(buf);
        }

        private void writeByte(byte v)
        {
            byte[] buf = new byte[1];
            buf[0] = v;

            writeBytes(buf);
        }

        public void writeFixString(string v, int len)
        {
            byte[] strArr = defaultEncoding.GetBytes(v);
            byte[] buf = new byte[len];
            Array.Clear(buf, 0, buf.Length);
            Array.Copy(strArr, buf, Math.Min(strArr.Length, len - 1));

            writeBytes(buf);
        }

        private void writeCString(string v)
        {
            byte[] strArr = defaultEncoding.GetBytes(v);

            writeBytes(strArr);
            writeByte(0);
        }        
    }
}
