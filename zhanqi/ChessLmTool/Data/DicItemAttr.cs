using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class DicItemAttr : BaseParse
    {
        //dic_item.json

        private const int BODY_LENGTH = 442;
        private const int MAX_NAME_LEN = 24;
        private const int MAX_INFO_LEN = 400;

        public ushort good_id;
        public ushort itid;
        public ushort power;
        public ushort seid;
        public ushort special_power;
        public ushort image_id;
        public ushort price;//*100
        public ushort up_val;
        public ushort is_consume;
        public string name = "";
        public string info = "";

        public int skill_id = 0;
        public int level_max = 5;
        public int up_ex = 100;
        public int tujian_id = 0;
        public string mid_seid = "0&";
        public string super_seid = "0&";

        public override bool parseLM(MemoryReader mr)
        {
            good_id = mr.readUShort();
            itid = mr.readUShort();
            power = mr.readUShort();
            seid = mr.readUShort();
            special_power = mr.readUShort();
            image_id = mr.readUShort();
            price = mr.readUShort();
            up_val = mr.readUShort();
            is_consume = mr.readUShort();
            name = mr.readString(MAX_NAME_LEN);
            info = mr.readString(MAX_INFO_LEN);

            return true;
        }

        public override bool parseE5(MemoryReader mr)
        {
            name = mr.readString(0x11);
            itid = mr.readByte();
            seid = mr.readByte();
            price = mr.readByte();
            image_id = mr.readByte();
            power = mr.readByte();
            special_power = mr.readByte();
            up_val = mr.readByte();
            is_consume = mr.readByte();

            return true;
        }

        public override JObject toJsonObject()
        {
            JObject line = new JObject();
            DicItemAttr item = this;

            jObjAdd1(line, "good_id", item.good_id);
            line["name"] = item.name;

            jObjAdd1(line, "image_id", item.image_id);
            jObjAdd1(line, "itid", item.itid);

            line["seid"] = (item.seid - 17).ToString() + "&";

            line["ueid"] = 0;
            line["cid"] = "0&";
            line["power"] = item.power;
            line["special_power"] = item.special_power;
            line["price"] = item.price;
            line["up_val"] = item.up_val;

            line["skill_id"] = item.skill_id;
            line["level_max"] = item.level_max;
            line["up_ex"] = item.up_ex;
            line["tujian_id"] = item.tujian_id;
            line["mid_seid"] = item.mid_seid;
            line["super_seid"] = item.super_seid;

            line["info"] = item.info;

            return line;
        }
    }
}
