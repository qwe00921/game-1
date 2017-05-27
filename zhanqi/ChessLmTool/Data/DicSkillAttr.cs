using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class DicSkillAttr : BaseParse
    {
        //dic_skill.json

        private static readonly string[] all_skilleffect = {
            "攻击(火)","攻击(水)","攻击(风)","攻击(地)",
            "吸收HP","吸收HP","吸收MP","能力下降","防御下降",
            "敏捷下降","士气下降","能力上升","防御上升",
            "敏捷上升","士气上升","混乱","中毒","麻痹","禁咒",
            "恢复HP","恢复MP","恢复状态","再次行动",
            "气候变化","谜","四神","全部上升","移动上升"
            };

        private const int BODY_LENGTH = 474;
        private const int MAX_NAME_LEN = 20;
        private const int MAX_INFO_LEN = 400;
        private const int MAX_SKILL_EFFECT_LEN = 40;

        public ushort skid;
        public ushort type;
        public ushort skill_object;
        public ushort atkid;
        public ushort mpid;
        public ushort imgid;
        public ushort mp_consume;
        public string name = "";
        public string info = "";
        public string eff_txt = "";
        public int enemy_strict = 0;

        public int skill_type = 900;
        public int hurt_num = 50;
        public int mark_num = 100;
        public int skill_limit = 0;
        public int skill_act = 0;
        public int seid = 0;

        public override bool parseLM(MemoryReader mr)
        {
            skid = mr.readUShort();
            type = mr.readUShort();
            skill_object = mr.readUShort();
            atkid = mr.readUShort();
            mpid = mr.readUShort();
            imgid = mr.readUShort();
            mp_consume = mr.readUShort();

            name = mr.readString(MAX_NAME_LEN);
            info = mr.readString(MAX_INFO_LEN);
            eff_txt = mr.readString(MAX_SKILL_EFFECT_LEN);

            return true;
        }

        public override bool parseE5(MemoryReader mr)
        {
            int startIndex = mr.getIndex();
            name = mr.readString(0xb);
            type = mr.readByte();
            if (skill_type < all_skilleffect.Length)
            {
                eff_txt = all_skilleffect[skill_type];
            }
            else
            {
                eff_txt = "";
            }
            skill_object = mr.readByte();
            atkid = mr.readByte();
            mpid = mr.readByte();
            mp_consume = mr.readByte();
            imgid = mr.readByte();

            return true;
        }

        public override JObject toJsonObject()
        {
            JObject line = new JObject();
            DicSkillAttr skill = this;

            jObjAdd1(line, "skid", skill.skid);
            line["name"] = skill.name;

            line["type"] = skill.type;
            line["object"] = skill.skill_object;
            jObjAdd1(line, "atkid", skill.atkid);
            jObjAdd1(line, "mpid", skill.mpid);
            line["mp_consume"] = skill.mp_consume;
            jObjAdd1(line, "imgid", skill.imgid);
            
            
            line["eff_txt"] = skill.eff_txt;
            line["enemy_strict"] = skill.enemy_strict;

            line["skill_type"] = skill.skill_type;
            line["hurt_num"] = skill.hurt_num;
            line["mark_num"] = skill.mark_num;
            line["skill_limit"] = skill.skill_limit;
            line["skill_act"] = skill.skill_act;
            line["seid"] = skill.seid;

            line["info"] = skill.info;

            return line;
        }
    }
}
