using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckFile : CmdBase
    {
        private string column = "";
        private string dir = "";
        private string file = "";
        private Expression expression;

        public CmdCheckFile()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            if (!base.loadConfig(node))
            {
                return false;
            }
            column = node.Attributes["column"].Value;
            dir = node.Attributes["dir"].Value;
            file = node.Attributes["file"].Value;
            if (dataType == ColumnType.TypeInteger)
            {
                expression = new Expression(file);
            }
            return true;
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

            if (dataType == ColumnType.TypeNone)
            {
                string destPath = Path.Combine(cfg.CheckPath, dir.Replace("%gamedir%", cfg.GameDir));

                string[] files = file.Split(';');
                foreach(string item in files)
                {
                    string destFile = Path.Combine(destPath, item);
                    if (!File.Exists(destFile))
                    {
                        logText("错误:" + Name + "," + destFile + " 不存在！:");
                        return false;
                    }
                }
            }
            else if (dataType == ColumnType.TypeInteger)
            {
                string col = column;

                string destPath = Path.Combine(cfg.CheckPath, dir.Replace("%gamedir%", cfg.GameDir));

                for(int i=0; i<jarr.Count; i++)
                {
                    JObject obj = (JObject)jarr[i];
                    int v = obj[col].ToObject<int>();
                    if (ignoreValue.ContainsKey(v))
                    {
                        continue;
                    }
                    string destFile = Path.Combine(destPath, expression.ComputeInt(v));
                    if (!File.Exists(destFile))
                    {
                        logText("错误:" + Name + "," + destFile + " 不存在！:" + obj.ToString());
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
            return String.Format("{0},{1}.{2},{3}", "CmdCheckFile", Name, column, file);
        }
    }
}
