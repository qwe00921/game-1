using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class LandformAttr : BaseParse
    {
        public static readonly string[] all_landname = {
            "平原","草原","树林","荒地","山地","岩山","山崖","雪原","桥梁","浅滩",
            "沼泽","池塘","小河","大河","栅栏","城墙","城内","城门","城池","关隘",
            "鹿砦","村庄","兵营","民居","宝库","火池","火堆","船只","障碍","地下",
            };

        public ushort landform_id;
        public ushort can_move;
        public ushort skill_attr;
        public string landform_name = "";

        public override bool parseLM(MemoryReader mr)
        {
            mr.skip(18);
            return true;
        }

        public override bool parseE5(MemoryReader mr)
        {
            landform_name = all_landname[landform_id];
            can_move = 1;
            skill_attr = 0;

            return true;
        }
    }
}
