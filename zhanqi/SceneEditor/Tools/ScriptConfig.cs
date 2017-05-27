using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace SceneEditor.Tools
{
    public class ScriptConfig
    {
        private static ScriptConfig instance = new ScriptConfig();

        public static ScriptConfig Instance
        {
            get
            {
                return instance;
            }
        }

        private ScriptConfig()
        {
        }

        public void loadConfig()
        {
            XmlDocument xd = new XmlDocument();
            xd.Load(Path.Combine(Application.StartupPath, "Script.xml"));

            fieldTypes.Clear();
            addFieldMap(xd, "script/type/UShort", FTUShort, "FTUShort");
            addFieldMap(xd, "script/type/Integer", FTInteger, "FTInteger");
            addFieldMap(xd, "script/type/Boolean", FTBool, "FTBool");
            addFieldMap(xd, "script/type/String", FTString, "FTString");
            addFieldMap(xd, "script/type/UShortArray", FTUShortArray, "FTUShortArray");

            XmlNodeList nodeList = xd.SelectNodes("script/CommandList");
            foreach (XmlNode node in nodeList)
            {
                XmlNodeList xnList = node.SelectNodes("command");
                Dictionary<int, CommandConfig> cmdInfos = new Dictionary<int, CommandConfig>();
                foreach (XmlNode xn in xnList)
                {
                    CommandConfig ci = new CommandConfig();
                    ci.loadXmlNode(xn);
                    if (cmdInfos.ContainsKey(ci.Id))
                    {
                        throw new EexReaderException(String.Format("CommandId:0x{0:x2} 重复", ci.Id));
                    }
                    cmdInfos.Add(ci.Id, ci);
                }
                int groupId = Int32.Parse(node.Attributes["group"].Value);
                allCmdInfos[groupId] = cmdInfos;
            }
        }

        private void addFieldMap(XmlDocument xd, String path, int val, String name)
        {
            XmlNodeList xnList = xd.SelectNodes(path);
            foreach (XmlNode xn in xnList)
            {
                String content = xn.Attributes["FieldIds"].Value;
                int[] ids = Util.getIntArray(content);
                foreach (int fieldId in ids)
                {
                    if (fieldTypes.ContainsKey(fieldId))
                    {
                        throw new EexReaderException(String.Format("{0} fieldid:0x{1:x0} 重复", name, fieldId));
                    }
                    fieldTypes.Add(fieldId, val);
                }
            }
        }

        private const int FTBool = (int)EexType.EexBoolean;
        private const int FTUShort = (int)EexType.EexUShort;
        private const int FTInteger = (int)EexType.EexInteger;
        private const int FTString = (int)EexType.EexString;
        private const int FTUShortArray = (int)EexType.EexUShortArray;

        private Dictionary<int, int> fieldTypes = new Dictionary<int, int>();
        private Dictionary<int, Dictionary<int, CommandConfig>> allCmdInfos = new Dictionary<int, Dictionary<int, CommandConfig>>();

        public EexType getEexType(int fieldId)
        {
            return (EexType)fieldTypes[fieldId];
        }

        public object getEexDefaultValue(int fieldId)
        {
            switch (getEexType(fieldId))
            {
                case EexType.EexBoolean:
                    return false;
                default:
                case EexType.EexInteger:
                    return (int)0;
                case EexType.EexString:
                    return (string)"";
                case EexType.EexUShort:
                    return (ushort)0;
                case EexType.EexUShortArray:
                    return new ushort[0];
            }
        }

        public CommandConfig getCommandConfig(int cmdId, int group = 0)
        {
            if (allCmdInfos.ContainsKey(group))
            {
                if (allCmdInfos[group].ContainsKey(cmdId))
                {
                    return allCmdInfos[group][cmdId];
                }
            }
            if (allCmdInfos[0].ContainsKey(cmdId))
            {
                return allCmdInfos[0][cmdId];
            }
            return null;
        }

        public bool containsCommand(int cmdId, int group = 0)
        {
            if (allCmdInfos.ContainsKey(group))
            {
                if (allCmdInfos[group].ContainsKey(cmdId))
                {
                    return true;
                }
            }
            return allCmdInfos[0].ContainsKey(cmdId);
        }
    }
}
