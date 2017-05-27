using Newtonsoft.Json.Linq;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor.Datas
{
    public class ScriptRoot
    {
        private ScriptHeader header = new ScriptHeader();
        private List<ScriptScene> sceneList = new List<ScriptScene>();
        public List<int> cmdIndexList = new List<int>();
        
        public List<ScriptScene> SceneList
        {
            get
            {
                return sceneList;
            }
        }

        public bool containCommand(int cmdId)
        {
            foreach (ScriptScene scene in sceneList)
            {
                foreach (ScriptSection section in scene.SectionList)
                {
                    foreach (ScriptCommand cmd in section.CommandList)
                    {
                        if (cmd.Id == cmdId)
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        private bool CorrectCmdData(bool isEEX) //修正76指令参数错误的问题；返回值bCorrect标记是否做过修正
        {
            bool bCorrect = false;
            int cmdIndex = 0;
            for (int i = 0; i < sceneList.Count; i++)
            {
                cmdIndex++;
                for (int j = 0; j < sceneList[i].SectionList.Count; j++)
                {
                    cmdIndex++;
                    for (int k = 0; k < sceneList[i].SectionList[j].CommandList.Count; k++)
                    {
                        ScriptCommand curCmd = sceneList[i].SectionList[j].CommandList[k];
                        cmdIndex++;
                        if (curCmd.Id == 0x76)
                        {
                            if (isEEX)
                            {
                                int relaIndex = (int)curCmd.Values[0] + cmdIndexList[cmdIndex];
                                for (int l = 0; l < cmdIndexList.Count; l++)
                                {
                                    if (cmdIndexList[l] == relaIndex)
                                    {
                                        sceneList[i].SectionList[j].CommandList[k].Values[0] = l;
                                        bCorrect = true;
                                    }
                                }
                            }
                            else
                            {
                                int index = (int)sceneList[i].SectionList[j].CommandList[k].Values[0];
                                sceneList[i].SectionList[j].CommandList[k].Values[0] = cmdIndexList[index] - cmdIndexList[cmdIndex];
                                bCorrect = true;
                            }
                        }
                    }
                }
            }

            return bCorrect;
        }

        public bool loadReader(EexBinaryReader reader, int group)
        {
            header.loadReader(reader);

            sceneList.Clear();
            cmdIndexList.Clear();
            int sceneNum = (reader.readInt32() - 10) / 4;
            for (int i = 0; i < sceneNum; i++)
            {
                reader.setIndex(10 + i * 4);
                int pos = reader.readInt32();
                reader.setIndex(pos);
                cmdIndexList.Add(pos);
                ScriptScene scene = new ScriptScene();
                scene.loadReader(reader, group);
                sceneList.Add(scene);
                foreach (int index in scene.cmdIndexList)
                {
                    cmdIndexList.Add(index);
                }
            }
            CorrectCmdData(true);

            return true;
        }

        public bool loadNewReader(EexBinaryReader reader)
        {
            header.loadReader(reader);

            sceneList.Clear();
            int sceneNum = (reader.readInt32() - 10) / 4;
            for (int i = 0; i < sceneNum; i++)
            {
                reader.setIndex(10 + i * 4);
                int pos = reader.readInt32();
                reader.setIndex(pos);
                ScriptScene scene = new ScriptScene();
                scene.loadNewReader(reader);
                sceneList.Add(scene);
            }

            return true;
        }

        public void saveWriter(EexBinaryWriter writer, int group)
        {
            cmdIndexList.Clear();
            header.saveWriter(writer);

            writer.setIndex(10 + sceneList.Count * 4);

            for (int i=0; i<sceneList.Count;i++ )
            {
                int wIndex = writer.getIndex();
                writer.setIndex(10 + i * 4);
                writer.writeInt32(wIndex);
                writer.setIndex(wIndex);
                cmdIndexList.Add(wIndex);

                ScriptScene item = sceneList[i];
                item.saveWriter(writer, group);                
                foreach (int index in item.cmdIndexList)
                {
                    cmdIndexList.Add(index);
                }
            }

            if (CorrectCmdData(false))
            {
                writer.setIndex(10 + sceneList.Count * 4);
                for (int i = 0; i < sceneList.Count; i++)
                {
                    int wIndex = writer.getIndex();
                    writer.setIndex(10 + i * 4);
                    writer.writeInt32(wIndex);
                    writer.setIndex(wIndex);
                    ScriptScene item = sceneList[i];
                    item.saveWriter(writer, group);
                }
            }
        }

        public void saveNewWriter(EexBinaryWriter writer)
        {
            header.saveWriter(writer);

            writer.setIndex(10 + sceneList.Count * 4);

            for (int i = 0; i < sceneList.Count; i++)
            {
                int wIndex = writer.getIndex();
                writer.setIndex(10 + i * 4);
                writer.writeInt32(wIndex);
                writer.setIndex(wIndex);

                ScriptScene item = sceneList[i];
                item.saveNewWriter(writer);
            }
        }

        public bool loadJson(JObject jobj)
        {
            header.loadJson((JObject)jobj["header"]);

            sceneList.Clear();

            JArray jar = (JArray)jobj["scenes"];
            foreach(JObject item in jar)
            {
                ScriptScene sce = new ScriptScene();
                sce.loadJson(item);
                sceneList.Add(sce);
            }

            return true;
        }

        public JObject toJsonObject()
        {
            JObject ret = new JObject();
            ret.Add("header", header.toJsonObject());

            JArray jarray = new JArray();
            ret.Add("scenes", jarray);
            foreach (ScriptScene scene in sceneList)
            {
                jarray.Add(scene.toJsonObject());
            }

            return ret;
        }

        public String getNodeText()
        {
            return "Root";
        }
    }
}
