using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameEditor.Tools
{
    public class EexBinaryReader
    {
        public const String ENCODING_GBK = "GBK";
        public const String ENCODING_UTF8 = "UTF-8";

        private byte[] content;
        private int index;
        private Encoding defaultEncoding;

        public EexBinaryReader(byte[] source)
            : this(source, ENCODING_GBK)
        {
        }

        public EexBinaryReader(byte[] source, String encodingName)
        {
            content = source;
            index = 0;
            defaultEncoding = Encoding.GetEncoding(encodingName);
        }

        public int readInt32()
        {
            if (index + 4 > content.Length)
            {
                throw new EexReaderException("readInt32,index=" + index + ",content=" + content.Length);
            }
            int ret = content[index] | (content[index + 1] << 8) | (content[index + 2] << 16) | (content[index + 3] << 24);
            index += 4;
            return ret;
        }

        public ushort readUShort()
        {
            if (index + 2 > content.Length)
            {
                throw new EexReaderException("readUShort,index=" + index + ",content=" + content.Length);
            }
            ushort ret = (ushort)(content[index] | (content[index + 1] << 8));
            index += 2;
            return ret;
        }

        public string readFixString(int length)
        {
            if (length < 0)
            {
                throw new EexReaderException("readString,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new EexReaderException(String.Format("readUShort,index={0},length={1},content={2}", index, length, content.Length));
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
        
        public Object readFieldValue(EexType etype)
        {            
            switch (etype)
            {
                case EexType.EexString:
                    return readCString();
                case EexType.EexBoolean:
                    {
                        int readInt = readUShort();
                        if (readInt == 1)
                        {
                            return true;
                        }
                        if (readInt == 0)
                        {
                            return false;
                        }
                        throw new EexReaderException("readFieldBoolean,boolValue=" + readInt.ToString());
                    }
                case EexType.EexInteger:
                    return readInt32();
                case EexType.EexUShort:
                    return readUShort();
                case EexType.EexUShortArray:
                    {
                        int readInt = readUShort();
                        ushort[] result = new ushort[readInt];
                        for(int i= 0; i<readInt;i++)
                        {
                            result[i] = readUShort();
                        }
                        return result;
                    }
                default:
                    throw new EexReaderException(String.Format("unknow type={0}", etype.ToString()));
            }
        }

        public Object readFieldObject(int fieldId)
        {
            int readId = readUShort();
            if (readId != fieldId)
            {
                throw new EexReaderException(String.Format("readFieldObject,fieldId=0x{0:x2},readId=0x{1:x2}", fieldId, readId));
            }
            EexType etype = ScriptConfig.Instance.getEexType(fieldId);
            return readFieldValue(etype);
        }

        private EexBinaryReader skip(int length)
        {
            if (length < 0)
            {
                throw new EexReaderException("skip,length=" + length);
            }
            if (index + length > content.Length)
            {
                throw new EexReaderException("skip,index=" + index + ",length=" + length + ",content=" + content.Length);
            }
            index += length;
            return this;
        }

        public EexBinaryReader setIndex(int index)
        {
            this.index = index;
            return this;
        }

        public int getIndex()
        {
            return index;
        }

        private void readBytes(byte[] result)
        {
            readBytes(result, 0, result.Length);
        }

        private void readBytes(byte[] result, int startPos, int length)
        {
            Array.Copy(content, index, result, startPos, length);
            index += length;
        }
    }

    public class EexReaderException : ApplicationException
    {
        public int CommandId = 0;

        public EexReaderException() : base()
        {
        }
        public EexReaderException(string message) : base(message)
        {
        }
        public EexReaderException(string message, Exception innerException)
            : base(message, innerException)
        {
        }
    }
}
