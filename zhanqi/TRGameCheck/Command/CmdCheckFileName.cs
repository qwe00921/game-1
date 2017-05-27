using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckFileName : CmdBase
    {
        private string dir = "";
        private string file = "";
        private bool onlyAsc = false;
        private bool onlyTop = false;
        private bool onlyExtension = false;
        private bool onlyLower = false;
        private bool onlyFileName = false;

        public CmdCheckFileName()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            dir = node.Attributes["dir"].Value;
            file = node.Attributes["file"].Value;
            onlyAsc = node.Attributes["onlyAsc"].Value.Equals("1");
            onlyTop = node.Attributes["onlyTop"].Value.Equals("1");
            onlyExtension = node.Attributes["onlyExtension"].Value.Equals("1");
            onlyLower = node.Attributes["onlyLower"].Value.Equals("1");
            onlyFileName = node.Attributes["onlyFilename"].Value.Equals("1");
            return base.loadConfig(node);
        }

        public override bool runCmd(CommandConfig cfg)
        {
            string destPath = Path.Combine(cfg.CheckPath, dir.Replace("%gamedir%", cfg.GameDir));
            if (!Directory.Exists(destPath))
            {
                if (optionalFile)
                {
                    return true;
                }
                logText("错误:" + destPath + " 不存在！");
                return false;
            }

            string[] files = Directory.GetFiles(destPath, file,
                onlyTop ? SearchOption.TopDirectoryOnly : SearchOption.AllDirectories);
            foreach (string filePath in files)
            {
                string subPath = filePath;
                if (subPath.StartsWith(destPath))
                {
                    subPath = subPath.Substring(destPath.Length);
                }
                if (onlyFileName)
                {
                    subPath = Path.GetFileName(subPath);
                }
                if (onlyExtension)
                {
                    subPath = Path.GetExtension(subPath);
                }
                if (onlyAsc)
                {
                    char[] nameArr = subPath.ToCharArray();
                    foreach (char ch in nameArr)
                    {

                        if (ch < 0 || ch > 127)
                        {
                            logText("错误:" + filePath + "," + subPath + " 包含中文字符！");
                            return false;
                        }
                    }
                }
                if (onlyLower)
                {
                    if (!subPath.Equals(subPath.ToLower()))
                    {
                        logText("错误:" + filePath + "," + subPath + " 包含大写！");
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
            return String.Format("{0},{1}/{2}", "CmdCheckFileName", dir, file);
        }
    }
}
