using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameEditor.Tools
{
    public static class Util
    {
        public static int[] getIntArray(String content)
        {
            if (String.IsNullOrEmpty(content))
            {
                return new int[0];
            }
            String[] strArray = content.Split(',');
            int[] result = new int[strArray.Length];
            for (int i = 0; i < strArray.Length; i++)
            {
                String item = strArray[i];
                if (!item.StartsWith("0x"))
                {
                    throw new EexReaderException(String.Format("{0} in {1} must start with 0x", item, content));
                }
                result[i] = Convert.ToInt32(item, 16);
            }
            return result;
        }

        public static String int2HexString(int v)
        {
            return "0x" + v.ToString("x2");
        }

        public static int hexString2Int(String text)
        {
            return Convert.ToInt32(text, 16);
        }
    }
}
