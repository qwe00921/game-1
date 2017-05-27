using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;
using TRGameCheck.Command.Scene;

namespace TRGameCheck.Command
{
    public class CmdCheckScene : CmdBase
    {
        private string dir = "";
        private string file = "";

        public CmdCheckScene()
        {
        }

        public override bool loadConfig(XmlNode node)
        {
            dir = node.Attributes["dir"].Value;
            file = node.Attributes["file"].Value;

            ScriptConfig.Instance.loadConfig(node);
            return base.loadConfig(node);
        }

        public override bool runCmd(CommandConfig cfg)
        {
            string destPath = Path.Combine(cfg.CheckPath, dir.Replace("%gamedir%", cfg.GameDir));
            string[] files = Directory.GetFiles(destPath, file, SearchOption.AllDirectories);

            ScriptRoot root = new ScriptRoot();
            foreach(string filePath in files)
            {
                bool eexFileOk = true;
                try
                {
                    using (FileStream fs = new FileStream(filePath, FileMode.Open))
                    {
                        byte[] buffer = new byte[fs.Length];
                        int readLen = fs.Read(buffer, 0, buffer.Length);
                        fs.Close();
                        if (readLen == buffer.Length)
                        {
                            eexFileOk = root.loadReader(new EexBinaryReader(buffer, EexBinaryReader.ENCODING_UTF8));
                        }
                        else
                        {
                            eexFileOk = false;
                        }                        
                    }
                }
                catch(Exception)
                {
                    eexFileOk = false;
                }
                if (!eexFileOk)
                {
                    logText("错误:" + filePath + " 格式不对！");
                    return false;
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
            return String.Format("{0},{1}.{2}", "CmdCheckScene", dir, file);
        }
    }
}
