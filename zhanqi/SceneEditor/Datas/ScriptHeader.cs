using Newtonsoft.Json.Linq;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor.Datas
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

        public void saveWriter(EexBinaryWriter writer)
        {
            writer.writeFixString(flag, 4);
            writer.writeUShort(short1);
            writer.writeUShort(short2);
            writer.writeUShort(short3);
        }

        public JObject toJsonObject()
        {
            JObject ret = new JObject();
            ret.Add("flag", flag);
            ret.Add("short1", short1);
            ret.Add("short2", short2);
            ret.Add("short3", short3);

            return ret;
        }

        public bool loadJson(JObject jobj)
        {
            flag = jobj["flag"].Value<string>();
            short1 = jobj["short1"].Value<ushort>();
            short2 = jobj["short2"].Value<ushort>();
            short3 = jobj["short3"].Value<ushort>();

            return true;
        }
    }
}
