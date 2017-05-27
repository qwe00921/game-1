using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class DicHeroAttr : BaseParse
    {
        //dic_hero.json

        private const int BODY_LENGTH = 1252;
        private const int MAX_NAME_LEN = 20;
        private const int MAX_INFO_LEN = 400;

        public ushort hid;
        public ushort face_id;					//对话时头像
        public ushort rimg_id;					//形象
        public ushort image_fight_id;
        public ushort jobid;					//职业
        public ushort atk;						//武力
        public ushort def;					//防御力
        public ushort ints;				//智力
        public ushort burst;//speed			//敏捷（命中率、格档率、连击率）
        public ushort fortune;//energy			//士气（爆发率）
        public ushort level;
        public ushort weapon_id;
        public ushort coat_id;
        public ushort assist_id;				//辅助装备
        public ushort hp;
        public ushort mp;
        public string name = "";	//姓名
        public string info = "";
        public string kill = "";				//奋力一击台词
        public string retreat = "";				//撤退台词
        public int seid = 0;
        public int tujian_id = 0;
        public int level_max = 99;
        public int up_ex = 100;
        public int skill = 0;

        public override bool parseLM(MemoryReader mr)
        {
            hid = mr.readUShort();
            face_id = mr.readUShort();					//对话时头像
            rimg_id = mr.readUShort();					//形象
            image_fight_id = mr.readUShort();
            jobid = mr.readUShort();					//职业
            atk = mr.readUShort();						//武力
            def = mr.readUShort();					//防御力
            ints = mr.readUShort();				//智力
            burst = mr.readUShort();						//敏捷（命中率、格档率、连击率）
            fortune = mr.readUShort();						//士气（爆发率）
            level = mr.readUShort();
            weapon_id = mr.readUShort();
            coat_id = mr.readUShort();
            assist_id = mr.readUShort();				//辅助装备
            hp = mr.readUShort();
            mp = mr.readUShort();
            name = mr.readString(MAX_NAME_LEN);
            info = mr.readString(MAX_INFO_LEN);
            kill = mr.readString(MAX_INFO_LEN);				//奋力一击台词
            retreat = mr.readString(MAX_INFO_LEN);				//撤退台词
            return true;
        }

        public override bool parseE5(MemoryReader mr)
        {
            GameItem item = ToolConfig.Instance.Current;
            int startIndex = mr.getIndex();            

            name = mr.readString(0x0d);
            if (item.e5_hero_face_id_len == 2)
            {
                face_id = mr.setIndex(startIndex + 0x0d).readUShort();
            }
            else
            {
                face_id = mr.setIndex(startIndex + 0x0d).readByte();
            }
            if (item.e5_hero_rimg_id_len == 2)
            {
                rimg_id = mr.setIndex(startIndex + 0x0f).readUShort();
            }
            else
            {
                rimg_id = mr.setIndex(startIndex + 0x0f).readByte();
            }
            image_fight_id = 0;
            atk = (ushort)(mr.setIndex(startIndex + 0x12).readByte() * 2);
            def = (ushort)(mr.readByte() * 2);
            ints = (ushort)(mr.readByte() * 2);
            burst = (ushort)(mr.readByte() * 2);
            fortune = (ushort)(mr.readByte() * 2);
            hp = mr.readByte();
            mp = mr.setIndex(startIndex + 0x19).readByte();
            jobid = mr.readByte();
            level = mr.readByte();
            weapon_id = mr.setIndex(startIndex + 0x1d).readByte();
            coat_id = mr.readByte();
            assist_id = mr.readByte();

            return true;
        }

        public override JObject toJsonObject()
        {
            JObject line = new JObject();

            DicHeroAttr hero = this;

            line["hid"] = hero.hid + 1;
            line["name"] = hero.name;

            line["face_id"] = hero.face_id + 1;
            line["rimg_id"] = hero.rimg_id + 1;
            jObjAdd1(line, "jobid", hero.jobid);
            line["atk"] = hero.atk;
            line["def"] = hero.def;
            line["ints"] = hero.ints;
            line["burst"] = hero.burst;
            line["fortune"] = hero.fortune;
            line["level"] = hero.level;
            jObjAdd1(line, "weapon_id", hero.weapon_id);
            jObjAdd1(line, "coat_id", hero.coat_id);
            jObjAdd1(line, "assist_id", hero.assist_id);
            line["hp"] = hero.hp;
            line["mp"] = hero.mp;
            line["killid"] = 0;            
            
            line["kill"] = hero.kill;
            line["retreat"] = hero.retreat;

            line["seid"] = hero.seid.ToString() + "&";
            line["tujian_id"] = hero.tujian_id;
            line["level_max"] = hero.level_max;
            line["up_ex"] = hero.up_ex;
            line["skill"] = hero.skill;

            line["info"] = hero.info;

            return line;
        }
    }
}
