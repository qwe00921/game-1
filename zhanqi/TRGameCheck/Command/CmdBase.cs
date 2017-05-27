using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class TableColumn
    {
        public string Table = "";
        public string Column = "";

        public bool loadText(string text)
        {
            string[] strArr = text.Split('.');
            if (strArr.Length != 2)
            {
                return false;
            }

            Table = strArr[0];
            Column = strArr[1];
            return true;
        }

        public override string ToString()
        {
            return Table + "." + Column;
        }
    }

    public abstract class CmdBase
    {
        #region 属性

        protected ColumnType dataType = ColumnType.TypeNone;
        protected bool optionalFile = false;
        protected bool ignoreZero = false;
        protected bool ignoreEmpty = false;
        protected Dictionary<int, int> ignoreValue = new Dictionary<int, int>();
        protected string name = "";
        protected int group = 0;

        public int Group
        {
            get
            {
                return group;
            }
        }

        public string Name
        {
            get
            {
                return name;
            }
        }

        public ColumnType DataType
        {
            get
            {
                return dataType;
            }
        }

        #endregion

        public virtual bool loadConfig(XmlNode node)
        {
            name = node.Attributes["name"].Value;
            if (node.Attributes["optionalFile"] != null)
            {
                optionalFile = node.Attributes["optionalFile"].Value.Equals("1");
            }
            if (node.Attributes["ignoreZero"] != null)
            {
                ignoreZero = node.Attributes["ignoreZero"].Value.Equals("1");
            }
            if (node.Attributes["ignoreEmpty"] != null)
            {
                ignoreEmpty = node.Attributes["ignoreEmpty"].Value.Equals("1");
            }
            if (node.Attributes["ignoreValue"] != null)
            {
                string[] strArray = node.Attributes["ignoreValue"].Value.Split(',');
                foreach (string strItem in strArray)
                {
                    int intValue = Convert.ToInt32(strItem);
                    ignoreValue[intValue] = intValue;
                }
            }
            if (node.Attributes["group"] != null)
            {
                group = Convert.ToInt32(node.Attributes["group"].Value);
            }
            if (node.Attributes["dataType"] != null)
            {
                try
                {
                    string strV = node.Attributes["dataType"].Value;
                    dataType = parseColumnType(strV);
                }
                catch (ApplicationException)
                {
                    return false;
                }
            }
            return true;
        }

        public abstract bool runCmd(CommandConfig cfg);

        public event Action<String> OutputDataReceived;
        public void logText(String text)
        {
            if (OutputDataReceived != null)
            {
                OutputDataReceived(text);
            }
        }

        private ColumnType parseColumnType(string text)
        {
            ColumnType result;

            switch (text)
            {
                case "none":
                    result = ColumnType.TypeNone;
                    break;
                case "integer":
                    result = ColumnType.TypeInteger;
                    break;
                case "string":
                    result = ColumnType.TypeString;
                    break;
                case "integerand":
                    result = ColumnType.TypeIntegerAnd;
                    break;
                case "mix":
                    result = ColumnType.TypeMix;
                    break;
                default:
                    logText(text + " unknow!");
                    throw new ApplicationException(text + " unknow!");
            }
            return result;
        }
    }

    public enum ColumnType
    {
        TypeNone,
        TypeInteger,
        TypeString,
        TypeIntegerAnd,
        TypeMix
    }
}
