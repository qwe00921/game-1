using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace SaveEditor
{
    class SaveData
    {
        private int version = 0;
        private string eventName = "";
        private string sceneName = "";
        private string battleName = "";
        private string shortTime = "";
        private int mainLevel = 0;
        private int culRound = 0;
        public bool parse(MemoryReader mr)
        {
            version = mr.readInt32();
            eventName = mr.readString(30);
            sceneName = mr.readString(30);
            battleName = mr.readString(30);
            shortTime = mr.readString(20);
            mainLevel = mr.readInt32();
            culRound = mr.readInt32();
            
            JObject line = new JObject();
            line["版本号"] = version;
            line["事件名称"] = eventName;
            line["场景名称"] = sceneName;
            line["战斗名称"] = battleName;
            line["存档时间"] = shortTime;
            line["级别"] = mainLevel;
            line["回合数"] = culRound;

            JArray content = new JArray();
            content.Add(line);

            string fileContent = content.ToString();

            saveSave(fileContent);
            return true;
        }
        private void saveSave(string fileContent)
        {
            string saveFile = "D:\\1.txt";
            //String saveFile = Path.Combine(Application.StartupPath, "SaveTest/test.txt");
            String saveDir = Path.GetDirectoryName("D:\\1.txt");
            if (!Directory.Exists(saveDir))
            {
                Directory.CreateDirectory(saveDir);
            }


            using (StreamWriter tw = new StreamWriter(saveFile, false, Encoding.UTF8))
            {
                tw.Write(fileContent);
            }
        }
    }
}
