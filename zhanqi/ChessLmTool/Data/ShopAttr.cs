using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class ShopAttr : BaseParse
    {
        private const int MAX_SHOP_GOOD_NUM = 16;
        public ushort actor_id_store;
	    public ushort actor_id_shop;
	    public byte[] buy = new byte[MAX_SHOP_GOOD_NUM];
	    public byte[] sell = new byte[MAX_SHOP_GOOD_NUM];

        public override bool parseLM(MemoryReader mr)
        {
            mr.skip(MAX_SHOP_GOOD_NUM*2+4);
            return true;
        }

        public override bool parseE5(MemoryReader mr)
        {
            actor_id_store = mr.readUShort();
            actor_id_shop = mr.readUShort();
            mr.readBytes(buy);
            mr.readBytes(sell);

            return true;
        }
    }
}
