using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor.Tools
{
    public class EexFieldReader
    {
        private int index;
        private JArray fieldArray;

        public EexFieldReader(JArray fieldArr)
        {
            index = 0;
            fieldArray = fieldArr;
        }

        private Object readFieldValue(EexType etype, int i)
        {
            switch (etype)
            {
                case EexType.EexString:
                    return fieldArray[i]["v"].Value<string>();
                case EexType.EexBoolean:
                    return fieldArray[i]["v"].Value<bool>();
                case EexType.EexInteger:
                    return fieldArray[i]["v"].Value<int>();
                case EexType.EexUShort:
                    return fieldArray[i]["v"].Value<ushort>();
                case EexType.EexUShortArray:
                    {
                        JArray ja = (JArray)fieldArray[i]["v"];
                        ushort[] result = new ushort[ja.Count];
                        for (int j = 0; j < ja.Count; j++) 
                        {
                            result[j] = ja[j].Value<ushort>();
                        }
                        return result;
                    }
                default:
                    throw new EexReaderException(String.Format("unknow type={0}", etype.ToString()));
            }
        }

        public Object readFieldObject(int fieldId)
        {
            int k = Util.hexString2Int(fieldArray[index]["k"].Value<string>());
            if (k != fieldId)
            {
                throw new EexReaderException(String.Format("readFieldObject,fieldId={0},k={1}", fieldId, k));
            }
            EexType etype = ScriptConfig.Instance.getEexType(fieldId);
            Object ret = readFieldValue(etype, index);
            index++;
            return ret;
        }
    }
}
