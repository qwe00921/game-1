using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace ResourceEncrypt
{
    class SecretFile
    {
        private SecretKey fileKey;
        private SecretKey codeKey;
        private int versionCode;
        private readonly byte[] idCode;

        public SecretFile() 
        {
            fileKey = new SecretKey();
            codeKey = new SecretKey();
            versionCode = 0;
            idCode = Encoding.UTF8.GetBytes("BGT1");
        }

        public void setCodeKey(String codeText)
        {
            codeKey.setKey(codeText);
        }

        public void setVersionCode(int version)
        {
            versionCode = version;
        }

        public void saveFile(byte[] content, String path, int encLength)
        {
            codeKey.setDefault();
            //fileKey.randomKey(16, 32);
            fileKey.computeKey(16, 32, content);

            int fileLength = 22 + fileKey.getKeyLength() + content.Length;

            byte[] fileBuffer = new byte[fileLength];

            //识别码
            Array.Copy(idCode, fileBuffer, 4);

            //版本号
            Array.Copy(int2Array(versionCode), 0, fileBuffer, 4, 4);

            //文件长度
            Array.Copy(int2Array(fileLength), 0, fileBuffer, 8, 4);

            //文件密钥长度
            byte fileKeyLength = (byte)fileKey.getKeyLength();
            //文件密钥长度进行加密
            codeKey.setDefault();
            fileKeyLength = codeKey.xorByte(fileKeyLength);
            fileBuffer[13] = (byte)fileKeyLength;

            //文件密钥内容
            Array.Copy(fileKey.getKeyValue(), 0, fileBuffer, 14, fileKey.getKeyLength());
            //文件密钥内容进行加密
            for(int i=0;i<fileKey.getKeyLength();i++)
            {
                int index = 14 + i;
                fileBuffer[index] = (byte)(fileBuffer[index] ^ fileKey.getKeyLength());
                codeKey.encryptAndNextIndex(ref fileBuffer[index]);
            }

            //包体长度
            Array.Copy(int2Array(content.Length), 0, fileBuffer, 14 + fileKey.getKeyLength(), 4);
            //包体长度加密
            codeKey.setDefault();
            fileKey.setDefault();
            for (int i=0; i<4; i++)
            {
                codeKey.encryptAndNextIndex(ref fileBuffer[14 + fileKey.getKeyLength() + i]);
                fileKey.encryptAndNextIndex(ref fileBuffer[14 + fileKey.getKeyLength() + i]);
            }

            //加密长度
            Array.Copy(int2Array(encLength), 0, fileBuffer, 18 + fileKey.getKeyLength(), 4);
            //加密长度加密
            codeKey.setDefault();
            fileKey.setDefault();
            for (int i = 0; i < 4; i++)
            {
                codeKey.encryptAndNextIndex(ref fileBuffer[18 + fileKey.getKeyLength() + i]);
                fileKey.encryptAndNextIndex(ref fileBuffer[18 + fileKey.getKeyLength() + i]);
            }

            //复制内容
            Array.Copy(content, 0, fileBuffer, 22 + fileKey.getKeyLength(), content.Length);
            //内容加密
            codeKey.setDefault();
            fileKey.setDefault();
            int headLength = 22 + fileKey.getKeyLength();
            int encryptLength = Math.Min(encLength, content.Length);
            for (int i = 0; i < encryptLength; i++)
            {
                codeKey.encryptAndNextChange(ref fileBuffer[headLength + i]);
                fileKey.encryptAndNextChange(ref fileBuffer[headLength + i]);
            }

            //计算校验码
            int checkCode = 0;
            for (int i=0; i<12; i++)
            {
                checkCode = checkCode ^ fileBuffer[i];
            }
            for (int i=13; i<fileBuffer.Length; i++)
            {
                checkCode = checkCode ^ fileBuffer[i];
            }
            fileBuffer[12] = (byte)checkCode;

            //写入文件
            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                fs.Write(fileBuffer, 0, fileBuffer.Length);
            }
        }

        public enum ReadFileResult
        {
            Succeed,
            FileNotExist,
            IdCodeError,
            VersionError,
            FileLengthError,
            CheckCodeError,
            FileKeyLengthError,
            BodyLengthError
        };

        public ReadFileResult loadFile(ref byte[] content, String path)
        {
            codeKey.setDefault();

            if (!File.Exists(path))
            {
                return ReadFileResult.FileNotExist;
            }

            byte[] fileBuffer = null;
            //读取文件
            using (FileStream fs = new FileStream(path, FileMode.Open))
            {
                int readIndex = 0;
                fileBuffer = new byte[fs.Length];
                int readLen = fs.Read(fileBuffer, readIndex, fileBuffer.Length - readIndex);
                while (readLen > 0 && readLen + readIndex < fileBuffer.Length)
                {
                    readIndex += readLen;
                    readLen = fs.Read(fileBuffer, readIndex, fileBuffer.Length - readIndex);
                }
            }

            //识别码
            if (!byteArrayEquals(fileBuffer, 0, idCode, 0, idCode.Length))
            {
                return ReadFileResult.IdCodeError;
            }

            //版本号
            int readVersion = fileBuffer[4] | (fileBuffer[5] << 8) | (fileBuffer[6] << 16) | (fileBuffer[7] << 24);
            if (versionCode != readVersion)
            {
                return ReadFileResult.VersionError;
            }

            //文件长度
            int fileLength = fileBuffer[8] | (fileBuffer[9] << 8) | (fileBuffer[10] << 16) | (fileBuffer[11] << 24);
            if (fileLength != fileBuffer.Length)
            {
                return ReadFileResult.FileLengthError;
            }

            //校验码
            int checkCode = 0;
            for (int i = 0; i < 12; i++)
            {
                checkCode = checkCode ^ fileBuffer[i];
            }
            for (int i = 13; i < fileBuffer.Length; i++)
            {
                checkCode = checkCode ^ fileBuffer[i];
            }
            if (checkCode != fileBuffer[12])
            {
                return ReadFileResult.CheckCodeError;
            }

            //文件密钥长度
            int fileKeyLength = fileBuffer[13] & 0xff;
            //文件密钥长度进行解密
            fileKeyLength = codeKey.xorByte((byte)fileKeyLength);
            if (14 + fileKeyLength + 4 >= fileBuffer.Length)
            {
                return ReadFileResult.FileKeyLengthError;
            }

            //文件密钥
            byte[] fileKeyValue = new byte[fileKeyLength];
            Array.Copy(fileBuffer, 14, fileKeyValue, 0, fileKeyLength);
            //文件密钥解密
            codeKey.setDefault();
            for (int i = 0; i < fileKeyValue.Length; i++)
            {
                fileKeyValue[i] = (byte)(fileKeyValue[i] ^ fileKeyLength);
                codeKey.encryptAndNextIndex(ref fileKeyValue[i]);
            }
            fileKey.setKey(fileKeyValue, 0, fileKeyLength);

            //包体长度
            int bodyLen = 0;
            {
                byte[] bodyLenArray = new byte[4];
                Array.Copy(fileBuffer, 14 + fileKeyLength, bodyLenArray, 0, 4);
                //包体长度解密
                codeKey.setDefault();
                fileKey.setDefault();
                for (int i = 0; i < 4; i++)
                {
                    codeKey.encryptAndNextIndex(ref bodyLenArray[i]);
                    fileKey.encryptAndNextIndex(ref bodyLenArray[i]);
                }
                bodyLen = bodyLenArray[0] | (bodyLenArray[1] << 8) | (bodyLenArray[2] << 16) | (bodyLenArray[3] << 24);
                if (22 + fileKeyLength + bodyLen != fileBuffer.Length)
                {
                    return ReadFileResult.BodyLengthError;
                }
            }

            //加密长度
            int encLen = 0;
            {
                byte[] encLenArray = new byte[4];
                Array.Copy(fileBuffer, 18 + fileKeyLength, encLenArray, 0, 4);
                //加密长度解密
                codeKey.setDefault();
                fileKey.setDefault();
                for (int i = 0; i < 4; i++)
                {
                    codeKey.encryptAndNextIndex(ref encLenArray[i]);
                    fileKey.encryptAndNextIndex(ref encLenArray[i]);
                }
                encLen = encLenArray[0] | (encLenArray[1] << 8) | (encLenArray[2] << 16) | (encLenArray[3] << 24);
            }

            //包体内容
            byte[] bodyArray = new byte[bodyLen];
            Array.Copy(fileBuffer, 22 + fileKeyLength, bodyArray, 0, bodyLen);
            //包体内容解密
            codeKey.setDefault();
            fileKey.setDefault();
            int decLength = Math.Min(encLen, bodyLen);
            for (int i = 0; i < decLength; i++)
            {
                codeKey.encryptAndNextChange(ref bodyArray[i]);
                fileKey.encryptAndNextChange(ref bodyArray[i]);
            }

            content = bodyArray;

            return ReadFileResult.Succeed;
        }

        private bool byteArrayEquals(byte[] btArray1, int startPos1, byte[] btArray2, int startPos2, int length)
        {
            if (startPos1 + length > btArray1.Length
                || startPos2 + length > btArray2.Length)
            {
                return false;
            }

            for (int i=0; i<length; i++)
            {
                if (btArray1[startPos1+i] != btArray2[startPos2+i])
                {
                    return false;
                }
            }

            return true;
        }

        private byte[] int2Array(int intValue)
        {
            byte[] ret = new byte[4];
            ret[0] = (byte)(intValue & 0xff);
            ret[1] = (byte)((intValue >> 8) & 0xff);
            ret[2] = (byte)((intValue >> 16) & 0xff);
            ret[3] = (byte)((intValue >> 24) & 0xff);

            return ret;
        }
    }
}
