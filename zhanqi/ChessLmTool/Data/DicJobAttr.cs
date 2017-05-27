using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class DicJobAttr : BaseParse
    {
        //dic_job.json
        public const int TYPE_JOB_ITEM = 1;

        private const int BODY_LENGTH = 446;
        private const int MAX_NAME_LEN = 20;
        private const int MAX_INFO_LEN = 400;

        public ushort jobid;
        public ushort job_level;
        public ushort atkid;
        public ushort seid;
        public ushort image_fight_id;
        public ushort spe;//base_move
        public ushort atk;
        public ushort def;
        public ushort ints;
        public ushort burst;//base_speed
        public ushort fortune;//base_energy
        public ushort hp_up;
        public ushort mp_up;
        public string name = "";
        public string info = "";
        public byte[] job_items = { };
        public int imgid = 0;
        public int move = 2;
        public int music = 0;

        public override bool parseLM(MemoryReader mr)
        {
            jobid = mr.readUShort();
            job_level = mr.readUShort();
            atkid = mr.readUShort();
            seid = mr.readUShort();
            image_fight_id = mr.readUShort();
            spe = mr.readUShort();
            atk = mr.readUShort();
            def = mr.readUShort();
            ints = mr.readUShort();
            burst = mr.readUShort();
            fortune = mr.readUShort();
            hp_up = mr.readUShort();
            mp_up = mr.readUShort();
            name = mr.readString(MAX_NAME_LEN);
            info = mr.readString(MAX_INFO_LEN);
            return true;
        }

        public override bool parseE5(MemoryReader mr)
        {
            GameItem item = ToolConfig.Instance.Current;

            name = item.job_name_array[jobid];
            spe = mr.readByte();
            atkid = mr.readByte();
            atk = mr.readByte();
            def = mr.readByte();
            ints = mr.readByte();
            burst = mr.readByte();
            fortune = mr.readByte();
            hp_up = mr.readByte();
            mp_up = mr.readByte();

            job_items = new byte[item.job_items_count];
            mr.readBytes(job_items);

            return true;
        }

        public override JObject toJsonObject()
        {
            JObject line = new JObject();
            DicJobAttr job = this;

            jObjAdd1(line, "jobid", job.jobid);
            line["name"] = job.name;

            line["spe"] = job.spe;
            line["atk"] = job.atk;
            line["def"] = job.def;
            line["ints"] = job.ints;
            line["burst"] = job.burst;
            line["fortune"] = job.fortune;
            line["hp_up"] = job.hp_up;
            line["mp_up"] = job.mp_up;
            jObjAdd1(line, "atkid", job.atkid);
            line["imgid"] = job.imgid;
            line["move"] = job.move;
            line["music"] = job.music;
            line["seid"] = job.seid.ToString() + "&";
            
            line["info"] = job.info;
            

            return line;
        }

        public override JObject toTypeJsonObject(int type)
        {
            JObject line = new JObject();
            
            switch (type)
            {
                case TYPE_JOB_ITEM:
                    {
                        line.Add("id", jobid + 1);
                        for (int i = 0; i < job_items.Length; i++ )
                        {
                            line.Add((i + 1).ToString(), job_items[i]);
                        }
                    }
                    break;
                default:
                    break;
            }

            return line;
        }
    }
}
