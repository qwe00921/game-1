using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckUnique : CmdBase
    {
        private string[] Columns
        {
            get;
            set;
        }
        private string columnSrc = "";

        public CmdCheckUnique()
        {
            Columns = new string[] { };
        }

        public override bool loadConfig(XmlNode node)
        {
            columnSrc = node.Attributes["columns"].Value;
            Columns = columnSrc.Split(',');
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

            if (DataType == ColumnType.TypeInteger)
            {
                foreach (String col in Columns)
                {
                    Dictionary<int, object> colContainer = new Dictionary<int, object>();

                    for (int i = 0; i < jarr.Count; i++)
                    {
                        JObject obj = (JObject)jarr[i];

                        if (obj[col] == null)
                        {
                            logText("错误:" + Name + "," + col + " 不存在");
                            return false;
                        }

                        int v = obj[col].ToObject<int>();
                        if (colContainer.ContainsKey(v))
                        {
                            logText("错误:" + Name + "," + col + " 值重复:" + obj.ToString());
                            return false;
                        }
                        colContainer.Add(v, obj);                        
                    }
                }
            }
            else if (DataType == ColumnType.TypeString)
            {
                foreach (String col in Columns)
                {
                    Dictionary<string, object> colContainer = new Dictionary<string, object>();

                    for (int i = 0; i < jarr.Count; i++)
                    {
                        JObject obj = (JObject)jarr[i];

                        if (obj[col] == null)
                        {
                            logText("错误:" + Name + "," + col + " 不存在");
                            return false;
                        }

                        string v = obj[col].ToObject<string>();
                        if (colContainer.ContainsKey(v))
                        {
                            logText("错误:" + Name + "," + col + " 值重复:" + obj.ToString());
                            return false;
                        }
                        colContainer.Add(v, obj);
                    }
                }
            }
            else if (DataType == ColumnType.TypeMix)
            {
                Dictionary<string, object> colContainer = new Dictionary<string, object>();

                for (int i = 0; i < jarr.Count; i++)
                {
                    JObject obj = (JObject)jarr[i];
                    StringBuilder sb = new StringBuilder();

                    foreach (String col in Columns)
                    {
                        if (obj[col] == null)
                        {
                            logText("错误:" + Name + "," + col + " 不存在");
                            return false;
                        }

                        if (obj[col].Type == JTokenType.Integer)
                        {
                            sb.Append(obj[col].ToObject<int>());
                        }
                        else
                        {
                            sb.Append(obj[col].ToObject<string>());
                        }
                        sb.Append(',');                        
                    }

                    string v = sb.ToString();
                    if (colContainer.ContainsKey(v))
                    {
                        logText("错误:" + Name + ":" + columnSrc + " 值重复:" + obj.ToString());
                        return false;
                    }
                    colContainer.Add(v, obj);
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
            return String.Format("{0},{1}.{2}", "CmdCheckUnique", Name, columnSrc);
        }
    }
}
