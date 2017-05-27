using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameCheck.Command.Scene
{
    public class ScriptHeader
    {
        private String flag = "";
        private ushort short1 = 0;
        private ushort short2 = 0;
        private ushort short3 = 0;

        public bool loadReader(EexBinaryReader reader)
        {
            flag = reader.readFixString(4);
            short1 = reader.readUShort();
            short2 = reader.readUShort();
            short3 = reader.readUShort();

            return true;
        }
    }
}
