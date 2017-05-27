using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckFileExist : CmdBase
    {
        private string dir = "";
        private string file = "";
        private bool onlyTop = false;
        private bool notExist = false;

        public CmdCheckFileExist()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            dir = node.Attributes["dir"].Value;
            file = node.Attributes["file"].Value;
            onlyTop = node.Attributes["onlyTop"].Value.Equals("1");
            notExist = node.Attributes["notExist"].Value.Equals("1");
            return base.loadConfig(node);
        }

        public override bool runCmd(CommandConfig cfg)
        {
            string destPath = Path.Combine(cfg.CheckPath, dir.Replace("%gamedir%", cfg.GameDir));
            string[] files = Directory.GetFiles(destPath, file,
                onlyTop ? SearchOption.TopDirectoryOnly : SearchOption.AllDirectories);
            if (notExist)
            {
                if (files.Length > 0)
                {
                    logText("错误:" + files[0] + " 不应该存在！");
                }
            }
            else
            {
                if (files.Length == 0)
                {
                    logText("错误:" + dir + "/" + file + " 找不到！");
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
            return String.Format("{0},{1}/{2}", "CmdCheckFileExist", dir, file);
        }
    }
}
