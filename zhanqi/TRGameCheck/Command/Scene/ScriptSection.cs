using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameCheck.Command.Scene
{
    public class ScriptSection
    {
        private List<ScriptCommand> commandList = new List<ScriptCommand>();

        public List<ScriptCommand> CommandList
        {
            get
            {
                return commandList;
            }
        }

        public bool loadReader(EexBinaryReader reader)
        {
            commandList.Clear();

            int sectionCount = reader.readUShort();

            int buildTreeCmd = -1;

            for (int i = 0; i < sectionCount; i++ )
            {
                int cmdId = reader.readUShort();
                ScriptCommand cmd = ScriptCommand.createCommand(cmdId);

                cmd.loadReader(reader);
                commandList.Add(cmd);

                if (buildTreeCmd != -1)
                {
                    if ((buildTreeCmd == (int)CommandId.CMD_CHILD_INFO_ACTION
                          && cmdId == (int)CommandId.CMD_ACTION_END)
                        || (buildTreeCmd == (int)CommandId.CMD_CHILD_ACTION))
                    {
                        buildTreeCmd = -1;
                        cmd.setExtValue(reader.readUShort());
                        continue;
                    }
                }

                switch (cmdId)
                {
                    case (int)CommandId.CMD_CHILD_ACTION:// = 0x1;//子事件设定
                    case (int)CommandId.CMD_CHILD_INFO_ACTION:// = 0x2;//子事件设定带信息
                        buildTreeCmd = cmdId;
                        break;
                    default:
                        break;
                }
            }

            return true;
        } 

        public String getNodeText()
        {
            return "Section";
        }
    }
}
