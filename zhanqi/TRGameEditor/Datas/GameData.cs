using Newtonsoft.Json.Linq;
using NPOI.SS.UserModel;
using NPOI.XSSF.UserModel;
using System;
using System.Data;
using System.IO;

using System.Collections.Generic;
using System.Text;

namespace TRGameEditor.Datas
{
    class GameData
    {
        private static GameData instance = new GameData();

        public static GameData Instance
        {
            get
            {
                return instance;
            }
        }

        private GameData()
        {
        }

        public DataTable data_hero;
        public DataTable data_item;
        public string game_path;
        public string game_dir;

        public void loadData(string path)
        {
            game_path = path;
            string temp = Path.Combine(path, "function.txt");
            using (StreamReader sr = new StreamReader(temp, Encoding.UTF8))
            {
                String text = sr.ReadToEnd();
                JObject obj = JObject.Parse(text);
                game_dir = obj["GamePath"].ToObject<string>();
            }
            data_hero = ConvertJson2DataTable(game_path + "\\" + game_dir + "\\json\\dic_hero.json");
            data_item = ConvertJson2DataTable(game_path + "\\" + game_dir + "\\json\\dic_item.json");
        }

        public static DataTable ConvertJson2DataTable(string jsonPath)
        {
            String text = "";
            using (StreamReader sr = new StreamReader(jsonPath, Encoding.UTF8))
            {
                text = sr.ReadToEnd();
            }
            JArray arr = JArray.Parse(text);
            DataTable dt = Json2DataTable(arr);
            return dt;
        }
        public static DataTable Json2DataTable(JArray arr)
        {
            DataTable ret = new DataTable();

            ret.BeginLoadData();
            for (int i = 0; i < arr.Count; i++)
            {
                JObject item = (JObject)arr[i];
                if (i == 0)
                {
                    foreach (KeyValuePair<string, JToken> kv in item)
                    {
                        if (kv.Value.Type == JTokenType.Integer)
                        {
                            ret.Columns.Add(kv.Key, typeof(int));
                        }
                        else
                        {
                            ret.Columns.Add(kv.Key);
                        }
                    }
                }
                DataRow dr = ret.NewRow();
                for (int j = 0; j < ret.Columns.Count; j++)
                {
                    if (ret.Columns[j].DataType == typeof(int))
                    {
                        dr[j] = item[ret.Columns[j].ColumnName].ToObject<int>();
                    }
                    else
                    {
                        dr[j] = item[ret.Columns[j].ColumnName].ToString();
                    }
                }
                ret.Rows.Add(dr);
            }
            ret.EndLoadData();

            return ret;
        }
    }
}
