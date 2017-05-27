using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckType : CmdBase
    {
        private string[] Columns = new string[]{};
        private int? maxValue = null;
        private Dictionary<int, int> valueContain = new Dictionary<int, int>();

        public CmdCheckType()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            Columns = node.Attributes["columns"].Value.Split(',');
            if (node.Attributes["maxValue"] != null)
            {
                maxValue = int.Parse(node.Attributes["maxValue"].Value);
            }
            if (node.Attributes["valueContain"] != null)
            {
                string[] strArray = node.Attributes["valueContain"].Value.Split(',');
                foreach (string strItem in strArray)
                {
                    int intV = Convert.ToInt32(strItem);
                    valueContain[intV] = intV;
                }
            }
            return base.loadConfig(node);
        }

        public override bool runCmd(CommandConfig cfg)
        {
            if (!cfg.JsonFiles.ContainsKey(Name))
            {
                if (optionalFile)
                {
                    return true;
                }
                logText("错误:" + Name + " 不存在");
                return false;
            }

            JArray jarr = cfg.JsonFiles[Name] as JArray;
            if (jarr == null)
            {
                logText("错误:" + Name + " 类型不对");
                return false;
            }

            foreach (String col in Columns)
            {
                for(int i=0; i<jarr.Count; i++)
                {
                    JObject obj = (JObject)jarr[i];
                    if (obj[col] == null)
                    {
                        if (optionalFile)
                        {
                            continue;
                        }
                        logText("错误:" + Name + "," + col + " 不存在");
                        return false;
                    }
                    switch (DataType)
                    {
                        case ColumnType.TypeInteger:
                            {
                                if (obj[col].Type != JTokenType.Integer)
                                {
                                    logText("错误:" + Name + "," + col + " 类型不对:" + obj.ToString());
                                    return false;
                                }
                                int intValue = obj[col].ToObject<int>();
                                if (maxValue != null)
                                {
                                    if (intValue > maxValue)
                                    {
                                        logText("错误:" + Name + "," + col + " 值太大:" + obj.ToString());
                                        return false;
                                    }
                                }
                                if (valueContain.Count > 0)
                                {
                                    if (!valueContain.ContainsKey(intValue))
                                    {
                                        logText("错误:" + Name + "," + col + " 超出指定范围:" + obj.ToString());
                                        return false;
                                    }
                                }
                            }
                            break;
                        case ColumnType.TypeString:
                            if (obj[col].Type != JTokenType.String)
                            {
                                logText("错误:" + Name + "," + col + " 类型不对:" + obj.ToString());
                                return false;
                            }
                            break;
                        case ColumnType.TypeIntegerAnd:
                            {
                                if (obj[col].Type != JTokenType.String)
                                {
                                    logText("错误:" + Name + "," + col + " 类型不对:" + obj.ToString());
                                    return false;
                                }
                                string colV = obj[col].ToObject<string>();
                                if (colV.EndsWith("&"))
                                {
                                    colV = colV.Substring(0, colV.Length - 1);
                                }
                                if (ignoreEmpty && String.IsNullOrEmpty(colV))
                                {

                                }
                                else
                                {
                                    string[] strArr = colV.Split('&');
                                    foreach (string strItem in strArr)
                                    {
                                        int temp = 0;
                                        if (!int.TryParse(strItem, out temp))
                                        {
                                            logText("错误:" + Name + "," + col + " 类型不对:" + obj.ToString());
                                            return false;
                                        }
                                        if (maxValue != null)
                                        {
                                            if (temp > maxValue)
                                            {
                                                logText("错误:" + Name + "," + col + " 值太大:" + obj.ToString());
                                                return false;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        default:
                            logText("错误:" + Name + "," + col + " 类型不对:" + obj.ToString());
                            return false;
                    }
                }
            }

            if (cfg.ShowAllLog)
            {
                logText(ToString() + " 完毕");
            }
            return true;
        }

        public override string ToString()
        {
            return String.Format("{0},{1},{2}", "CmdCheckType", Name, DataType);
        }
    }
}
