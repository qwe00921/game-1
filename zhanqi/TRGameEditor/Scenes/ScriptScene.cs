using Newtonsoft.Json.Linq;
using TRGameEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameEditor.Scenes
{
    public class ScriptScene
    {
        private List<ScriptSection> sectionList = new List<ScriptSection>();

        public List<ScriptSection> SectionList
        {
            get
            {
                return sectionList;
            }
            set
            {
                sectionList = value;
            }
        }

        public bool loadReader(EexBinaryReader reader)
        {
            sectionList.Clear();
            int sectionNum = reader.readUShort();
            for (int j = 0; j < sectionNum; j++)
            {
                ScriptSection section = new ScriptSection();
                section.loadReader(reader);
                sectionList.Add(section);
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

        public void saveWriter(EexBinaryWriter writer)
        {
            writer.writeUShort((ushort)sectionList.Count);
            foreach (ScriptSection item in sectionList)
            {
                item.saveWriter(writer);
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
        public ScriptSection addSection(ScriptSection sec, bool addDefault = true)
        {
            return addSection(sectionList.IndexOf(sec) + 1,addDefault);
        }
        public ScriptSection addSection(int index, bool addDefault = true)
        {
            ScriptSection section = new ScriptSection();
            sectionList.Insert(index, section);
            if (addDefault)
            {
                section.addCode(2, 0);
                section.addCode(0, 1, true);
                section.addCode(0, 2);
            }
            return sectionList[index];
        }

        public void removeSection(ScriptSection sec)
        {
            removeSection(sectionList.IndexOf(sec));
        }
        public void removeSection(int index)
        {
            ScriptSection section = sectionList[index];            
            sectionList.RemoveAt(index);
        }
    }

}
