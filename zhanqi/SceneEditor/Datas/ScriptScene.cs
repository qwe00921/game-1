using Newtonsoft.Json.Linq;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor.Datas
{
    public class ScriptScene
    {
        private List<ScriptSection> sectionList = new List<ScriptSection>();
        public List<int> cmdIndexList = new List<int>();

        public List<ScriptSection> SectionList
        {
            get
            {
                return sectionList;
            }
        }

        public bool loadReader(EexBinaryReader reader, int group)
        {
            sectionList.Clear();
            cmdIndexList.Clear();
            int sectionNum = reader.readUShort();
            for (int j = 0; j < sectionNum; j++)
            {
                ScriptSection section = new ScriptSection();
                cmdIndexList.Add(reader.getIndex());
                section.loadReader(reader, group);
                sectionList.Add(section);
                foreach (int index in section.cmdIndexList)
                {
                    cmdIndexList.Add(index);
                }
            }

            return true;
        }

        public bool loadNewReader(EexBinaryReader reader)
        {
            sectionList.Clear();
            int sectionNum = reader.readUShort();
            for (int j = 0; j < sectionNum; j++)
            {
                ScriptSection section = new ScriptSection();
                section.loadNewReader(reader);
                sectionList.Add(section);
            }

            return true;
        }

        public void saveWriter(EexBinaryWriter writer, int group)
        {
            cmdIndexList.Clear();
            writer.writeUShort((ushort)sectionList.Count);
            foreach (ScriptSection item in sectionList)
            {
                cmdIndexList.Add(writer.getIndex());
                item.saveWriter(writer, group);
                foreach (int index in item.cmdIndexList)
                {
                    cmdIndexList.Add(index);
                }
            }
        }

        public void saveNewWriter(EexBinaryWriter writer)
        {
            writer.writeUShort((ushort)sectionList.Count);
            foreach (ScriptSection item in sectionList)
            {
                item.saveNewWriter(writer);
            }
        }

        public JObject toJsonObject()
        {
            JObject ret = new JObject();

            JArray jarray = new JArray();
            ret.Add("sections", jarray);
            foreach (ScriptSection section in sectionList)
            {
                jarray.Add(section.toJsonObject());
            }

            return ret;
        }

        public bool loadJson(JObject jobj)
        {
            sectionList.Clear();

            JArray jar = (JArray)jobj["sections"];
            foreach (JObject item in jar)
            {
                ScriptSection section = new ScriptSection();
                section.loadJson(item);
                sectionList.Add(section);
            }

            return true;
        }

        public String getNodeText()
        {
            return "Scene";
        }
    }

}
