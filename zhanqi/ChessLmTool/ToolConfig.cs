using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace ChessLmTool
{
    public class ToolConfig
    {
        private static ToolConfig _instance = new ToolConfig();

        private ToolConfig()
        {
            GameList = new List<GameItem>();
        }

        public static ToolConfig Instance
        {
            get
            {
                return _instance;
            }
        }

        public void loadConfig()
        {
            XmlDocument xd = new XmlDocument();
            xd.Load(Path.Combine(Application.StartupPath, "config.xml"));

            DefaultItem = GameItem.parseXmlNode(xd.SelectSingleNode("config/gamelist/defaultitem"), null);

            GameList.Clear();
            XmlNodeList xnList = xd.SelectNodes("config/gamelist/gameitem");
            foreach (XmlNode xn in xnList)
            {
                GameList.Add(GameItem.parseXmlNode(xn, DefaultItem));
            }

            Current = GameList[0];
        }

        public List<GameItem> GameList
        {
            get;
            private set;
        }

        public GameItem Current
        {
            set;
            get;
        }

        public GameItem DefaultItem
        {
            get;
            private set;
        }
    }

    public class GameItem
    {
        public string game_name = "";
        public string[] job_name_array = { };
        public int job_items_count = 0;
        public int gk_good_id_judge = 0;
        public int gk_good_id_add = 0;
        public int e5_hero_face_id_len = 1;
        public int e5_hero_rimg_id_len = 1;
        public int e5_job_num = 0;
        public int e5_job_attr_size = 0;
        public int e5_job_attr_offset = 0;
        public int e5_jobLandLen = 0;
        public int e5_job_land_info_offset = 0;
        public int e5_skill_num = 0;
        public int e5_skill_attr_offset = 0;
        public int e5_skill_attr_size = 0;
        public int e5_gk_attr_offset = 0;
        public int e5_gk_num = 0;
        public int e5_item_attr_offset = 0;
        public int e5_item_num = 0;
        public int imsg_skill_info_offset = 0;
        public int imsg_hero_info_offset = 0;
        public int imsg_hero_info_num = 0;
        public int star_e5_item_num = 0;

        public void copy(GameItem item)
        {
            this.game_name = item.game_name;
            this.job_name_array = item.job_name_array;
            this.job_items_count = item.job_items_count;
            this.gk_good_id_judge = item.gk_good_id_judge;
            this.gk_good_id_add = item.gk_good_id_add;
            this.e5_hero_face_id_len = item.e5_hero_face_id_len;
            this.e5_hero_rimg_id_len = item.e5_hero_rimg_id_len;
            this.e5_job_num = item.e5_job_num;
            this.e5_job_attr_size = item.e5_job_attr_size;
            this.e5_jobLandLen = item.e5_jobLandLen;
            this.e5_job_land_info_offset = item.e5_job_land_info_offset;
            this.e5_job_attr_offset = item.e5_job_attr_offset;
            this.e5_skill_num = item.e5_skill_num;
            this.e5_skill_attr_offset = item.e5_skill_attr_offset;
            this.e5_skill_attr_size = item.e5_skill_attr_size;
            this.e5_gk_attr_offset = item.e5_gk_attr_offset;
            this.e5_gk_num = item.e5_gk_num;
            this.e5_item_attr_offset = item.e5_item_attr_offset;
            this.e5_item_num = item.e5_item_num;
            this.imsg_skill_info_offset = item.imsg_skill_info_offset;
            this.imsg_hero_info_offset = item.imsg_hero_info_offset;
            this.imsg_hero_info_num = item.imsg_hero_info_num;
            this.star_e5_item_num = item.star_e5_item_num;
        }

        public static GameItem parseXmlNode(XmlNode xn, GameItem defaultItem)
        {
            GameItem result = new GameItem();
            if (defaultItem != null)
            {
                result.copy(defaultItem);
            }

            result.game_name = xn.SelectSingleNode("game_name").InnerText;
            result.job_name_array = xn.SelectSingleNode("job_name_array").InnerText.Split(',');
            for (int i = 0; i < result.job_name_array.Length; i++ )
            {
                result.job_name_array[i] = result.job_name_array[i].Trim();
            }
            parseIntValue(ref result.job_items_count, xn, "job_items_count");
            parseIntValue(ref result.gk_good_id_judge, xn, "gk_good_id_judge");
            parseIntValue(ref result.gk_good_id_add, xn, "gk_good_id_add");
            parseIntValue(ref result.e5_hero_face_id_len, xn, "e5_hero_face_id_len");
            parseIntValue(ref result.e5_hero_rimg_id_len, xn, "e5_actor_image_id_len");
            parseIntValue(ref result.e5_job_num, xn, "e5_job_num");
            parseIntValue(ref result.e5_job_attr_size, xn, "e5_job_attr_size");
            parseIntValue(ref result.e5_job_attr_offset, xn, "e5_job_attr_offset");
            parseIntValue(ref result.e5_jobLandLen, xn, "e5_jobLandLen");
            parseIntValue(ref result.e5_job_land_info_offset, xn, "e5_job_land_info_offset");
            parseIntValue(ref result.e5_skill_num, xn, "e5_skill_num");
            parseIntValue(ref result.e5_skill_attr_offset, xn, "e5_skill_attr_offset");
            parseIntValue(ref result.e5_skill_attr_size, xn, "e5_skill_attr_size");
            parseIntValue(ref result.e5_gk_attr_offset, xn, "e5_gk_attr_offset");
            parseIntValue(ref result.e5_gk_num, xn, "e5_gk_num");
            parseIntValue(ref result.e5_item_attr_offset, xn, "e5_item_attr_offset");
            parseIntValue(ref result.e5_item_num, xn, "e5_item_num");
            parseIntValue(ref result.imsg_skill_info_offset, xn, "imsg_skill_info_offset");
            parseIntValue(ref result.imsg_hero_info_offset, xn, "imsg_hero_info_offset");
            parseIntValue(ref result.imsg_hero_info_num, xn, "imsg_hero_info_num");
            parseIntValue(ref result.star_e5_item_num, xn, "star_e5_item_num");

            return result;
        }

        private static bool parseIntValue(ref int intValue, XmlNode parent, string path)
        {
            XmlNode xn = parent.SelectSingleNode(path);
            if (xn == null)
            {
                return false;
            }
            string text = xn.InnerText;
            if (text.ToLower().StartsWith("0x"))
            {
                intValue = Convert.ToInt32(text, 16);
                return true;
            }
            intValue = Convert.ToInt32(text);
            return true;
        }
    }
}
