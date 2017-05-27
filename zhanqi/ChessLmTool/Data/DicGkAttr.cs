using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class DicGkAttr : BaseParse
    {
        //dic_gk.json

        public ushort gkid;
        public string gkname = "";
        public int[] buyid = { };
        public ushort openhid;
        public ushort buyhid;

        public override bool parseE5(MemoryReader mr)
        {
            openhid = mr.readUShort();
            buyhid = mr.readUShort();

            GameItem gameItem = ToolConfig.Instance.Current;

            List<int> goodList = new List<int>();
            goodList.Clear();
            for (int i = 0; i < 0x20; i++ )
            {
                int readB = mr.readByte();
                if (readB != 0xff)
                {
                    if (readB >= gameItem.gk_good_id_judge)
                    {
                        //特殊处理
                        readB = readB + gameItem.gk_good_id_add;
                    }
                    goodList.Add(readB);
                }
            }
            buyid = goodList.ToArray();

            return true;
        }

        public override JObject toJsonObject()
        {
            JObject line = new JObject();

            jObjAdd1(line, "gkid", gkid);
            line["gkname"] = gkname;

            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < buyid.Length; i++ )
            {
                if (i !=0 )
                {
                    sb.Append('&');
                }
                sb.Append(buyid[i].ToString());
            }

            line["buyid"] = sb.ToString();
            line["openhid"] = openhid;
            line["buyhid"] = buyhid;

            return line;
        }
    }
}
