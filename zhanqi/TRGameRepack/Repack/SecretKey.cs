using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameRepack.Repack
{
    class SecretKey
    {
        private byte[] keyArray;
        private byte[] keySource;
        private int keyIndex;
        private int keyTimes;

        public SecretKey() 
        {
            keyArray = new byte[0];

            keySource = (byte[])keyArray.Clone();
            keyIndex = 0;
            keyTimes = 0;
        }

        public int getKeyLength()
        {
            return keySource.Length;
        }

        public byte[] getKeyValue()
        {
            return keyArray;
        }

        public byte xorByte(byte byteValue)
        {
            int ret = byteValue;

            foreach (byte bV in keyArray)
            {
                ret = ret ^ bV;
            }

            return (byte)ret;
        }

        public void setDefault()
        {
            Array.Copy(keySource, keyArray, keySource.Length);
            keyIndex = 0;
            keyTimes = 0;
        }

        public void setKey(String keyValue)
        {
            keyArray = Encoding.UTF8.GetBytes(keyValue);

            keySource = (byte[])keyArray.Clone();
            keyIndex = 0;
            keyTimes = 0;
        }

        public void setKey(byte[] keyValue, int startPos, int length)
        {
            keyArray = new byte[length];
            Array.Copy(keyValue, startPos, keyArray, 0, length);

            keySource = (byte[])keyArray.Clone();
            keyIndex = 0;
            keyTimes = 0;
        }

        public void computeKey(int minLength, int maxLength, byte[] fileContent)
        {
            int length = minLength + fileContent.Length % (maxLength - minLength + 1);
            keyArray = new byte[length];
            Array.Clear(keyArray, 0, keyArray.Length);

            for (int i=0; i<fileContent.Length; i++)
            {
                int index = i % keyArray.Length;
                keyArray[index] = (byte)(keyArray[index] ^ fileContent[i]);
            }

            keySource = (byte[])keyArray.Clone();
            keyIndex = 0;
            keyTimes = 0;
        }

        public void encryptAndNextChange(ref byte byteValue)
        {
            byteValue = (byte)(byteValue ^ keyArray[keyIndex]);

            keyIndex++;
            if (keyIndex >= keyArray.Length)
            {
                keyChange();
            }
        }

        public void encryptAndNextIndex(ref byte byteValue)
        {
            byteValue = (byte)(byteValue ^ keyArray[keyIndex]);

            keyIndex++;
            if (keyIndex >= keyArray.Length)
            {
                keyIndex = 0;
            }
        }

        private void keyChange()
        {
            if (keyIndex < keyArray.Length)
            {
                return;
            }

            keyIndex = 0;
            keyTimes++;

            int changeIndex = keyTimes % keyArray.Length;
            int changeValue = (keyTimes * 7) & 0xff;
            keyArray[changeIndex] = (byte)((keyArray[changeIndex] + changeValue) & 0xff);
        }
    }
}
