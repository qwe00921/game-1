using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SceneEditor.Tools
{
    public class CommandConfig
    {
        public int Id
        {
            get;
            private set;
        }

        public int[] FieldIds
        {
            get;
            private set;
        }

        public String Name
        {
            get;
            private set;
        }

        public EexType[] FieldTypes
        {
            get;
            private set;
        }

        public int FieldRepeat
        {
            get;
            private set;
        }

        public bool useFieldIds()
        {
            return FieldIds.Length >= FieldTypes.Length;
        }

        public int FieldCount
        {
            get
            {
                return Math.Max(FieldIds.Length, FieldTypes.Length);
            }
        }

        public CommandConfig()
        {
            Id = 0;
            FieldIds = new int[0];
            Name = "";
            FieldTypes = new EexType[0];
            FieldRepeat = 1;
        }

        public void loadXmlNode(XmlNode xn)
        {
            String cmdId = xn.Attributes["CommandId"].Value;
            if (!cmdId.StartsWith("0x"))
            {
                throw new EexReaderException(String.Format("CommandId:{0} must start with 0x", cmdId));
            }
            Id = Convert.ToInt32(cmdId, 16);
            FieldIds = Util.getIntArray(xn.Attributes["FieldIds"].Value);
            if (xn.Attributes["Name"] != null)
            {
                Name = xn.Attributes["Name"].Value;
            }

            XmlNode xnTypes = xn.Attributes["FieldTypes"];
            if (xnTypes != null && !String.IsNullOrEmpty(xnTypes.Value))
            {
                String[] strArr = xnTypes.Value.Split(',');
                FieldTypes = new EexType[strArr.Length];
                for (int i = 0; i<strArr.Length; i++)
                {
                    String item = strArr[i];
                    switch (item)
                    {
                        case "UShort":
                            FieldTypes[i] = EexType.EexUShort;
                            break;
                        case "Integer":
                            FieldTypes[i] = EexType.EexInteger;
                            break;
                        case "String":
                            FieldTypes[i] = EexType.EexString;
                            break;
                        default:
                            throw new EexReaderException(String.Format("EexType:{0} not find!", item));
                    }
                }
            }

            XmlNode xnRepeat = xn.Attributes["FieldRepeat"];
            if (xnRepeat != null)
            {
                FieldRepeat = Int32.Parse(xnRepeat.Value);
            }
        }
    }
}
