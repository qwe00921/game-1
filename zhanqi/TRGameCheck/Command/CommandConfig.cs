using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace TRGameCheck.Command
{
    public class CommandConfig
    {
        public CommandConfig()
        {
            JsonFiles = new Dictionary<string, JContainer>();
            CheckPath = "";
            GameDir = "";
            ShowAllLog = false;
        }

        public Dictionary<string, JContainer> JsonFiles
        {
            get;
            private set;
        }

        public string CheckPath
        {
            get;
            private set;
        }

        public string GameDir
        {
            get;
            set;
        }

        public bool ShowAllLog
        {
            get;
            set;
        }

        public string GamePath
        {
            get
            {
                return Path.Combine(CheckPath, GameDir);
            }
        }

        public void init(string path)
        {
            JsonFiles.Clear();
            CheckPath = path;

            using (StreamReader sr = new StreamReader(Path.Combine(path, "function.txt"), Encoding.UTF8))
            {
                String text = sr.ReadToEnd();
                JObject obj = JObject.Parse(text);
                GameDir = obj["GamePath"].ToObject<string>();
            }
        }
    }
}
