using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckForeignKey : CmdBase
    {
        private string column = "";        
        private TableColumn foreign = new TableColumn();
        private int valueJudge = 0;
        private int valueAdd = 0;

        public CmdCheckForeignKey()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            column = node.Attributes["column"].Value;
            foreign.loadText(node.Attributes["foreign"].Value);
            if (node.Attributes["valueJudge"] != null)
            {
                valueJudge = int.Parse(node.Attributes["valueJudge"].Value);
            }
            if (node.Attributes["valueAdd"] != null)
            {
                valueAdd = int.Parse(node.Attributes["valueAdd"].Value);
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

            if (!cfg.JsonFiles.ContainsKey(foreign.Table))
            {
                logText("错误:" + Name + "," + foreign.Table + " 不存在");
                return false;
            }
            JArray foreignarr = cfg.JsonFiles[foreign.Table] as JArray;
            if (jarr == null)
            {
                logText("错误:" + Name + "," + foreign.Table + " 类型不对");
                return false;
            }

            if (dataType == ColumnType.TypeInteger)
            {
                Dictionary<int, object> foreignValue = new Dictionary<int, object>();
                for(int i=0; i<foreignarr.Count; i++)
                {
                    JObject item = (JObject)foreignarr[i];

                    if (item[foreign.Column] == null)
                    {
                        logText("错误:" + Name + "," + foreign.ToString() + " 不存在");
                        return false;
                    }

                    int v = item[foreign.Column].ToObject<int>();
                    foreignValue[v] = item;
                }

                for (int i = 0; i < jarr.Count; i++)
                {
                    JObject item = (JObject)jarr[i];

                    if (item[column] == null)
                    {
                        if (optionalFile)
                        {
                            continue;
                        }
                        logText("错误:" + Name + "," + column + " 不存在");
                        return false;
                    }

                    int v = item[column].ToObject<int>();
                    if (v == 0 && ignoreZero)
                    {
                        continue;
                    }
                    if (ignoreValue.ContainsKey(v))
                    {
                        continue;
                    }
                    if (!foreignValue.ContainsKey(v))
                    {
                        logText("错误:" + Name + "," + column + "=" + v + ",不存在:" + item.ToString());
                        return false;
                    }
                }
            }
            else if (dataType == ColumnType.TypeIntegerAnd)
            {
                Dictionary<int, object> foreignValue = new Dictionary<int, object>();
                for (int i = 0; i < foreignarr.Count; i++)
                {
                    JObject item = (JObject)foreignarr[i];

                    if (item[foreign.Column] == null)
                    {
                        logText("错误:" + Name + "," + foreign.ToString() + " 不存在");
                        return false;
                    }

                    int v = item[foreign.Column].ToObject<int>();
                    foreignValue[v] = item;
                }

                for (int i = 0; i < jarr.Count; i++)
                {
                    JObject item = (JObject)jarr[i];

                    if (item[column] == null)
                    {
                        logText("错误:" + Name + "," + column + " 不存在");
                        return false;
                    }

                    string colValue = item[column].ToObject<string>();
                    if (colValue.EndsWith("&"))
                    {
                        colValue = colValue.Substring(0, colValue.Length - 1);
                    }
                    if (String.IsNullOrEmpty(colValue))
                    {
                        if (ignoreEmpty)
                        {
                            continue;
                        }
                        logText("错误:" + Name + "," + column + " 为空");
                        return false;
                    }
                    else
                    {
                        string[] strArr = colValue.Split('&');
                        foreach (string strItem in strArr)
                        {
                            int iItem = int.Parse(strItem);
                            if (iItem == 0 && ignoreZero)
                            {
                                continue;
                            }
                            if (iItem >= valueJudge)
                            {
                                iItem = iItem + valueAdd;
                            }
                            if (!foreignValue.ContainsKey(iItem))
                            {
                                logText("错误:" + Name + "," + column + "=" + iItem + ",不存在:" + item.ToString());
                                return false;
                            }
                        }
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
            return String.Format("{0},{1}.{2}", "CmdCheckForeignKey", Name, column);
        }
    }
}
