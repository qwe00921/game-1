using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameCheck.Command.Scene
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

        public String getNodeText()
        {
            return "Scene";
        }
    }

}
