using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace TRGameEditor.Tools
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


        public int FieldRepeat
        {
            get;
            private set;
        }

        public int FieldCount
        {
            get
            {
                return FieldIds.Length;
            }
        }

        public CommandConfig()
        {
            Id = 0;
            FieldIds = new int[0];
            Name = "";
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

            XmlNode xnRepeat = xn.Attributes["FieldRepeat"];
            if (xnRepeat != null)
            {
                FieldRepeat = Int32.Parse(xnRepeat.Value);
            }
        }
    }
}
