using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public interface IParse
    {
        bool parseLM(MemoryReader mr);
        bool parseE5(MemoryReader mr);
    }
}
