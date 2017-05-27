using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace ChessLmTool.Data
{
    public abstract class BaseParse : IParse
    {
        public virtual bool parseLM(MemoryReader mr)
        {
            return false;
        }

        public virtual bool parseE5(MemoryReader mr)
        {
            return false;
        }

        public virtual JObject toJsonObject()
        {
            return new JObject();
        }

        public virtual JObject toTypeJsonObject(int type)
        {
            return new JObject();
        }

        protected static void jObjAdd1(JObject jo, string name, ushort val)
        {
            if (val == 255)
            {
                jo[name] = 0;
                //jo[name] = val + 1;
            }
            else
            {
                jo[name] = val + 1;
            }
        }
    }
}
