using Newtonsoft.Json.Linq;
using TRGameEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TRGameEditor.Scenes
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
            set
            {
                commandList = value;
            }
        }

        public bool loadReader(EexBinaryReader reader)
        {
            commandList.Clear();

            int sectionLen = reader.readUShort();
            int endIndex = reader.getIndex() + sectionLen;

            int buildTreeCmd = -1;

            while (reader.getIndex() < endIndex)
            {
                int cmdId = reader.readUShort();
                ScriptCommand cmd = ScriptCommand.createCommand(cmdId);
                try
                {
                    cmd.loadReader(reader);
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

        public void computeExValue(EexBinaryWriter writer)
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
                    cmd.saveWriter(writer);
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
                    cmd.loadReader(reader);
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

        public void saveWriter(EexBinaryWriter writer)
        {
            computeExValue(writer);

            int sectionStart = writer.getIndex();
            writer.writeUShort(0);

            for(int i = 0; i < commandList.Count; i++)
            {
                ScriptCommand cmd = commandList[i];
                writer.writeUShort((ushort)cmd.Id);
                cmd.saveWriter(writer);

                if (cmd.isExtValueAvailable())
                {
                    writer.writeUShort((ushort)cmd.getExtValue());
                }
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
                cmd.saveWriter(writer);

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

        public void addCmd(int index, ScriptCommand cmd)
        {
            commandList.Insert(index, cmd);
        }

        public int addCode(int id,int index,bool ext = false)
        {
            ScriptCommand cmd = ScriptCommand.createCommand(id);
            if(ext)
            {
                cmd.setExtValue(2);
            }
            commandList.Insert(index, cmd);
            return index;
        }
        public int addCode(int id, ScriptCommand cmd,bool ext = false)
        {
            return addCode(id, FindCommand(cmd) + 1, ext);
        }

        public int FindCommand(ScriptCommand cmd)
        {
            return commandList.IndexOf(cmd);
        }

        public void CobyCode(int id, ScriptCommand cmd)
        {
            commandList.Insert(commandList.IndexOf(cmd) + 1, cmd);
        }

        public void removeCode(ScriptCommand cmd)
        {
            removeCode(commandList.IndexOf(cmd));
        }

        public void removeCode(int index)
        {
            commandList.RemoveAt(index);
        }
    }
}
