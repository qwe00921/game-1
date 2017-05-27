using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TRGameCheck.Command.Scene
{
    class CommandObjectArray : ScriptCommand
    {
        public CommandObjectArray()
        {
        }

        public override int Id
        {
            get
            {
                return base.Id;
            }
            set
            {
                base.Id = value;

                CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(value);

                fieldCount = cfg.FieldCount;
                keys = new int[fieldCount];
                if (cfg.useFieldIds())
                {
                    Array.Copy(cfg.FieldIds, keys, fieldCount);
                }
                values = new Object[fieldCount];

                if (cfg.FieldRepeat > 1)
                {
                    arrayValues = new Object[cfg.FieldRepeat][];
                }
            }
        }

        public override bool loadReader(EexBinaryReader reader)
        {
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(id);
            if (cfg.useFieldIds())
            {
                if (cfg.FieldRepeat <= 1)
                {
                    for (int i = 0; i < fieldCount; i++)
                    {
                        values[i] = reader.readFieldObject(keys[i]);
                    }
                }
                else
                {
                    for (int i = 0; i < cfg.FieldRepeat; i++)
                    {
                        Object[] objs = new Object[fieldCount];
                        for (int j = 0; j < fieldCount; j++)
                        {
                            objs[j] = reader.readFieldObject(keys[j]);
                        }
                        arrayValues[i] = objs;
                    }
                }
            }
            else
            {
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < fieldCount; i++)
                {
                    sb.Append("0x").Append(reader.readUShort().ToString("x2")).Append(",");
                    values[i] = reader.readFieldValue(cfg.FieldTypes[i]);
                }
                String sth = sb.ToString();
                Console.WriteLine(sth);
                MessageBox.Show(sth);
            }
            return true;
        }

        private JObject createKV(int k, Object v)
        {
            JObject ret = new JObject();
            ret.Add("k", Util.int2HexString(k));
            ret.Add("v", JToken.FromObject(v));
            return ret;
        }
    }
}
