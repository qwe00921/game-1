using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdLoadJArray : CmdBase
    {
        public string Src
        {
            get;
            set;
        }

        public CmdLoadJArray()
        {
            Src = "";
        }

        public override bool loadConfig(XmlNode node)
        {
            Src = node.Attributes["src"].Value;
            return base.loadConfig(node);
        }

        public override bool runCmd(CommandConfig cfg)
        {
            if (cfg.JsonFiles.ContainsKey(Name))
            {
                logText("错误:" + Name + " 重复加载");
                return false;
            }

            string destFile = Path.Combine(cfg.CheckPath, Src.Replace("%gamedir%", cfg.GameDir));
            if (!File.Exists(destFile))
            {
                if (optionalFile)
                {
                    return true;
                }
                logText("错误:" + Name + "," + destFile + " 文件不存在");
                return false;
            }

            String text = "";
            using (StreamReader sr = new StreamReader(destFile, Encoding.UTF8))
            {
                text = sr.ReadToEnd();
            }

            try
            {
                JArray obj = JArray.Parse(text);
                cfg.JsonFiles[Name] = obj;               
            }
            catch (Exception)
            {
                logText("错误:" + Name + "," + destFile + " 文件解析 json 失败");
                return false;
            }

            if (cfg.ShowAllLog)
            {
                logText(ToString() + " 完毕");
            }
            return true;
        }

        public override string ToString()
        {
            return String.Format("{0},{1}", "CmdLoadJArray", Name);
        }
    }
}
