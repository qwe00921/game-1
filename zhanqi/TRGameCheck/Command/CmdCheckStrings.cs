using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace TRGameCheck.Command
{
    public class CmdCheckStrings : CmdBase
    {
        private string sourceFile = "";

        public CmdCheckStrings()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            sourceFile = node.Attributes["src"].Value;
            return base.loadConfig(node);
        }

        public override bool runCmd(CommandConfig cfg)
        {
            string destFile = Path.Combine(cfg.CheckPath, sourceFile.Replace("%gamedir%", cfg.GameDir));
            if (!File.Exists(destFile))
            {
                if (optionalFile)
                {
                    return true;
                }
                logText("错误:" + Name + "," + destFile + " 文件不存在");
                return false;
            }

            XmlDocument xd = new XmlDocument();
            xd.Load(destFile);

            XmlNode dictNode = xd.SelectSingleNode("plist/dict");
            bool readKey = true;
            for (int i = 0; i < dictNode.ChildNodes.Count; i++)
            {
                XmlNode childNode = dictNode.ChildNodes[i];
                if (childNode.NodeType != XmlNodeType.Element)
                {
                    continue;
                }

                if (readKey)
                {
                    if (!childNode.Name.Equals("key"))
                    {
                        logText("错误:" + Name + "," + sourceFile + "," + childNode.OuterXml);
                        return false;
                    }
                    readKey = false;
                    continue;
                }
                else
                {
                    if (!childNode.Name.Equals("string"))
                    {
                        logText("错误:" + Name + "," + sourceFile + "," + childNode.OuterXml);
                        return false;
                    }
                    readKey = true;
                    continue;
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
            return String.Format("{0},{1},{2}", "CmdCheckStrings", Name, sourceFile);
        }
    }
}
