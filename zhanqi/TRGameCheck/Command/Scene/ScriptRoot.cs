using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameCheck.Command.Scene
{
    public class ScriptRoot
    {
        private ScriptHeader header = new ScriptHeader();
        private List<ScriptScene> sceneList = new List<ScriptScene>();
        
        public List<ScriptScene> SceneList
        {
            get
            {
                return sceneList;
            }
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

        public String getNodeText()
        {
            return "Root";
        }
    }
}
