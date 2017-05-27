using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace TRGameCheck.Command
{
    class CommandList
    {
        private List<CmdBase> cmdList = new List<CmdBase>();

        public bool loadXml(string xmlPath)
        {
            XmlDocument xd = new XmlDocument();
            xd.Load(Path.Combine(Application.StartupPath, "commandlist.xml"));
            XmlNodeList nodeList = xd.SelectNodes("root/CommandList");
            foreach (XmlNode node in nodeList)
            {
                for (int i = 0; i < node.ChildNodes.Count; i++)
                {
                    XmlNode childNode = node.ChildNodes[i];
                    if (childNode.NodeType != XmlNodeType.Element)
                    {
                        continue;
                    }
                    CmdBase cb = CreateCmd(childNode.Name);
                    if (cb == null)
                    {
                        return false;
                    }
                    cb.loadConfig(childNode);
                    cb.OutputDataReceived += logText;
                    cmdList.Add(cb);
                }
            }

            return true;
        }

        public bool CheckJson = true;
        public bool CheckFileExist = true;
        public bool CheckFileName = true;
        public bool CheckScene = true;
        public bool CheckStrings = true;
        public bool CheckGlobalRes = true;

        public event Action<String> OutputDataReceived;
        private void logText(String text)
        {
            if (OutputDataReceived != null)
            {
                OutputDataReceived(text);
            }
        }

        private CmdBase CreateCmd(string nodeName)
        {
            CmdBase result = null;

            switch (nodeName)
            {
                case "CheckFileName":
                    result = new CmdCheckFileName();
                    break;
                case "CheckFileExist":
                    result = new CmdCheckFileExist();
                    break;
                case "CheckScene":
                    result = new CmdCheckScene();
                    break;
                case "LoadJArray":
                    result = new CmdLoadJArray();
                    break;
                case "CheckType":
                    result = new CmdCheckType();
                    break;
                case "CheckUnique":
                    result = new CmdCheckUnique();
                    break;
                case "CheckFile":
                    result = new CmdCheckFile();
                    break;
                case "CheckForeignKey":
                    result = new CmdCheckForeignKey();
                    break;
                case "CheckStrings":
                    result = new CmdCheckStrings();
                    break;
                default:
                    logText("cmd:" + nodeName + " not found!");
                    return null;
            }

            return result;
        }

        public bool runCmd(CommandConfig cfg)
        {
            bool result = true;
            for (int i=0; i<cmdList.Count; i++)
            {
                CmdBase cb = cmdList[i];
                if (needRun(cb))
                {
                    if (!cb.runCmd(cfg))
                    {
                        result = false;
                    }
                }
            }

            logText(DateTime.Now.ToString() + " 检查完毕");

            return result;
        }
        
        private bool needRun(CmdBase cmd)
        {
            if (CheckGlobalRes)
            {
                if (cmd.Group == 1)
                {
                    return true;
                }
            }

            if (CheckJson)
            {
                if (cmd is CmdCheckForeignKey
                    || cmd is CmdCheckType
                    || cmd is CmdCheckUnique
                    || cmd is CmdLoadJArray)
                {
                    return true;
                }
            }
            
            if (CheckFileExist)
            {
                if (cmd is CmdCheckFile
                    || cmd is CmdCheckFileExist)
                {
                    return true;
                }
            }

            if (CheckFileName)
            {
                if (cmd is CmdCheckFileName)
                {
                    return true;
                }
            }
            
            if (CheckScene)
            {
                if (cmd is CmdCheckScene)
                {
                    return true;
                }
            }
            
            if (CheckStrings)
            {
                if (cmd is CmdCheckStrings)
                {
                    return true;
                }
            }            

            return false;
        }
    }
}
