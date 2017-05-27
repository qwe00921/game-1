using Newtonsoft.Json.Linq;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace SceneEditor.Datas
{
    public class ScriptSection
    {
        private List<ScriptCommand> commandList = new List<ScriptCommand>();
        public List<int> cmdIndexList = new List<int>();

        public List<ScriptCommand> CommandList
        {
            get
            {
                return commandList;
            }
        }

        public bool loadReader(EexBinaryReader reader, int group)
        {
            commandList.Clear();
            cmdIndexList.Clear();

            int sectionLen = reader.readUShort();
            int endIndex = reader.getIndex() + sectionLen;

            int buildTreeCmd = -1;

            while (reader.getIndex() < endIndex)
            {
                int cmdIndex = reader.getIndex();
                int cmdId = reader.readUShort();
                ScriptCommand cmd = ScriptCommand.createCommand(cmdId);
                try
                {
                    cmd.loadReader(reader, group);
                }
                catch (EexReaderException exception)
                {
                    exception.CommandId = cmdId;
                    throw;
                }
                commandList.Add(cmd);
                cmdIndexList.Add(cmdIndex);

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

                switch ((CommandId)cmdId)
                {
                    case CommandId.CMD_CHILD_ACTION:// = 0x1;//子事件设定
                    case CommandId.CMD_CHILD_INFO_ACTION:// = 0x2;//子事件设定带信息
                        buildTreeCmd = cmdId;
                        break;
                }
            }

            return true;
        }

        public void computeExValue(EexBinaryWriter writer, int group)
        {
            int writerIndex = writer.getIndex();
            for (int i=0; i<commandList.Count; i++)
            {
                if (!commandList[i].isExtValueAvailable())
                {
                    continue;
                }

                int level = 1;
                writer.setIndex(writerIndex);
                for(int j = i + 1; j < commandList.Count; j++)
                {
                    ScriptCommand cmd = commandList[j];
                    if (cmd.isExtValueAvailable())
                    {
                        level++;
                    }
                    writer.writeUShort((ushort)cmd.Id);
                    cmd.saveWriter(writer, group);
                    if (cmd.isExtValueAvailable())
                    {
                        writer.writeUShort((ushort)cmd.getExtValue());
                    }

                    if (cmd.Id == (int)CommandId.CMD_ACTION_END)
                    {
                        level--;
                        if (level==0)
                        {
                            break;
                        }
                    }
                }
                commandList[i].setExtValue(writer.getIndex() - writerIndex);
            }
            writer.setIndex(writerIndex);
        }

        public bool loadNewReader(EexBinaryReader reader)
        {
            commandList.Clear();

            int sectionCount = reader.readUShort();

            int buildTreeCmd = -1;

            for (int i = 0; i < sectionCount; i++ )
            {
                int cmdId = reader.readUShort();
                ScriptCommand cmd = ScriptCommand.createCommand(cmdId);
                try
                {
                    cmd.loadReader(reader, 0);
                }
                catch (EexReaderException exception)
                {
                    exception.CommandId = cmdId;
                    throw;
                }
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

                switch ((CommandId)cmdId)
                {
                    case CommandId.CMD_CHILD_ACTION:// = 0x1;//子事件设定
                    case CommandId.CMD_CHILD_INFO_ACTION:// = 0x2;//子事件设定带信息
                        buildTreeCmd = cmdId;
                        break;
                }
            }

            return true;
        }

        public void saveWriter(EexBinaryWriter writer, int group)
        {
            cmdIndexList.Clear();
            computeExValue(writer, group);

            int sectionStart = writer.getIndex();
            writer.writeUShort(0);

            for(int i = 0; i < commandList.Count; i++)
            {
                ScriptCommand cmd = commandList[i];
                int cmdIndex = writer.getIndex();
                writer.writeUShort((ushort)cmd.Id);
                cmd.saveWriter(writer, group);

                if (cmd.isExtValueAvailable())
                {
                    writer.writeUShort((ushort)cmd.getExtValue());
                }
                cmdIndexList.Add(cmdIndex);
            }

            int sectionEnd = writer.getIndex();
            writer.setIndex(sectionStart).writeUShort((ushort)(sectionEnd-sectionStart-2));
            writer.setIndex(sectionEnd);
        }

        public void saveNewWriter(EexBinaryWriter writer)
        {
            int sectionStart = writer.getIndex();
            writer.writeUShort(0);

            for (int i = 0; i < commandList.Count; i++)
            {
                ScriptCommand cmd = commandList[i];
                writer.writeUShort((ushort)cmd.Id);
                cmd.saveWriter(writer, 0);

                if (cmd.isExtValueAvailable())
                {
                    writer.writeUShort((ushort)cmd.getExtValue());
                }
            }

            int sectionEnd = writer.getIndex();
            writer.setIndex(sectionStart).writeUShort((ushort)commandList.Count);
            writer.setIndex(sectionEnd);
        }

        public JObject toJsonObject()
        {
            JObject ret = new JObject();

            JArray jarray = new JArray();
            ret.Add("commands", jarray);
            foreach (ScriptCommand command in commandList)
            {
                jarray.Add(command.toJsonObject());
            }

            return ret;
        }

        public bool loadJson(JObject jobj)
        {
            commandList.Clear();

            JArray jar = (JArray)jobj["commands"];

            int buildTreeCmd = -1;

            foreach (JObject item in jar)
            {
                int cmdId = ScriptCommand.parseCommandId(item);

                ScriptCommand cmd = ScriptCommand.createCommand(cmdId);
                try
                {
                    cmd.loadJson(item);
                }
                catch (EexReaderException exception)
                {
                    exception.CommandId = cmdId;
                    throw;
                }
                commandList.Add(cmd);

                if (buildTreeCmd != -1)
                {
                    if ((buildTreeCmd == (int)CommandId.CMD_CHILD_INFO_ACTION
                          && cmdId == (int)CommandId.CMD_ACTION_END)
                        || (buildTreeCmd == (int)CommandId.CMD_CHILD_ACTION))
                    {
                        buildTreeCmd = -1;
                        cmd.setExtValue(ScriptCommand.parseExtValue(item));
                        continue;
                    }
                }

                switch ((CommandId)cmdId)
                {
                    case CommandId.CMD_CHILD_ACTION:// = 0x1;//子事件设定
                    case CommandId.CMD_CHILD_INFO_ACTION:// = 0x2;//子事件设定带信息
                        buildTreeCmd = cmdId;
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
