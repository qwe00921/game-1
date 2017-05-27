using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace TRGameCheck.Command.Scene
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

        public void loadConfig(XmlNode node)
        {
            fieldTypes.Clear();
            addFieldMap(node, "type/UShort", FTUShort, "FTUShort");
            addFieldMap(node, "type/Integer", FTInteger, "FTInteger");
            addFieldMap(node, "type/Boolean", FTBool, "FTBool");
            addFieldMap(node, "type/String", FTString, "FTString");
            addFieldMap(node, "type/UShortArray", FTUShortArray, "FTUShortArray");

            commandInfos.Clear();
            XmlNodeList xnList = node.SelectNodes("commandList/command");
            foreach (XmlNode xn in xnList)
            {
                CommandConfig ci = new CommandConfig();
                ci.loadXmlNode(xn);
                if (commandInfos.ContainsKey(ci.Id))
                {
                    throw new ApplicationException(String.Format("CommandId:0x{0:x2} 重复", ci.Id));
                }
                commandInfos.Add(ci.Id, ci);
            }
        }

        private void addFieldMap(XmlNode node, String path, int val, String name)
        {
            XmlNodeList xnList = node.SelectNodes(path);
            foreach (XmlNode xn in xnList)
            {
                String content = xn.Attributes["FieldIds"].Value;
                int[] ids = Util.getIntArray(content);
                foreach (int fieldId in ids)
                {
                    if (fieldTypes.ContainsKey(fieldId))
                    {
                        throw new ApplicationException(String.Format("{0} fieldid:0x{1:x0} 重复", name, fieldId));
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
        private Dictionary<int, CommandConfig> commandInfos = new Dictionary<int, CommandConfig>();

        public EexType getEexType(int fieldId)
        {
            return (EexType)fieldTypes[fieldId];
        }


        public CommandConfig getCommandConfig(int cmdId)
        {
            return commandInfos[cmdId];
        }

        public bool containsCommand(int cmdId)
        {
            return commandInfos.ContainsKey(cmdId);
        }
    }
}
