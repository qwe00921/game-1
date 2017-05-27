using Newtonsoft.Json.Linq;
using TRGameEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameEditor.Scenes
{
    public class ScriptRoot
    {
        public string m_path = null;
        public bool changed = false;
        private ScriptHeader header = new ScriptHeader();
        private List<ScriptScene> sceneList = new List<ScriptScene>();
        public ScriptRoot()
        {

        }
        public ScriptRoot(string path)
        {
            m_path = path;
        }
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

        public bool loadReader(EexBinaryReader reader)
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
                scene.loadReader(reader);
                sceneList.Add(scene);
            }

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

        public void saveWriter(EexBinaryWriter writer)
        {
            header.saveWriter(writer);

            writer.setIndex(10 + sceneList.Count * 4);

            for (int i=0; i<sceneList.Count;i++ )
            {
                int wIndex = writer.getIndex();
                writer.setIndex(10 + i * 4);
                writer.writeInt32(wIndex);
                writer.setIndex(wIndex);

                ScriptScene item = sceneList[i];
                item.saveWriter(writer);                
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
            if (m_path != null)
                return m_path;
            return "Root";
        }

        public ScriptScene addScene(int index)
        {
            ScriptScene sce = new ScriptScene();
            sce.addSection(0);
            sceneList.Insert(index,sce);
            return sceneList[index];
        }
        public ScriptScene addScene(ScriptScene sce)
        {
            return addScene(sceneList.IndexOf(sce)+1);
        }


        public void removeScence(int index) {
            sceneList.RemoveAt(index);
        }

        public void removeScene(ScriptScene sce) 
        {
            removeScence(sceneList.IndexOf(sce));
        }

    }
}
