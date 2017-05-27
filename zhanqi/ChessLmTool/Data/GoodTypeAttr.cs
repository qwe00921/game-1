using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public class GoodTypeAttr : BaseParse
    {
        public override bool parseLM(MemoryReader mr)
        {
            mr.skip(26);
            return true;
        }
    }
}
