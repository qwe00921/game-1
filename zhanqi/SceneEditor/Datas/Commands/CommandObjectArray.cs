using Newtonsoft.Json.Linq;
using SceneEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace SceneEditor.Datas.Commands
{
    class CommandObjectArray : ScriptCommand
    {
        public CommandObjectArray()
        {
        }

        private void initKVArray(int group)
        {
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(Id, group);

            keys = new int[cfg.FieldCount];
            if (cfg.useFieldIds())
            {
                Array.Copy(cfg.FieldIds, keys, FieldCount);
            }
            values = new Object[FieldCount];

            if (cfg.FieldRepeat > 1)
            {
                arrayValues = new Object[cfg.FieldRepeat][];
            }
        }

        public override bool loadReader(EexBinaryReader reader, int group)
        {
            initKVArray(group);
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(Id, group);

            if (cfg.useFieldIds())
            {
                if (cfg.FieldRepeat <= 1)
                {
                    for (int i = 0; i < FieldCount; i++)
                    {
                        values[i] = reader.readFieldObject(keys[i]);
                    }
                }
                else
                {
                    for (int i = 0; i < cfg.FieldRepeat; i++)
                    {
                        Object[] objs = new Object[FieldCount];
                        for (int j = 0; j < FieldCount; j++)
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
                for (int i = 0; i < FieldCount; i++)
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

        public override void saveWriter(EexBinaryWriter writer, int group)
        {
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(id, group);

            if (cfg.FieldRepeat <= 1)
            {
                for (int i = 0; i < cfg.FieldCount; i++)
                {
                    int fieldId = cfg.FieldIds[i];
                    int keyIndex = i;
                    if (keyIndex < values.Length)
                    {
                        writer.writeFieldObject(fieldId, values[keyIndex]);
                    }
                    else
                    {
                        writer.writeFieldObject(fieldId, ScriptConfig.Instance.getEexDefaultValue(fieldId));
                    }
                }
            }
            else
            {
                for (int i = 0; i < cfg.FieldRepeat; i++)
                {
                    for (int j = 0; j < cfg.FieldCount; j++)
                    {
                        int fieldId = cfg.FieldIds[j];
                        int keyIndex = j;
                        if (keyIndex < arrayValues[i].Length)
                        {
                            writer.writeFieldObject(fieldId, arrayValues[i][keyIndex]);
                        }
                        else
                        {
                            writer.writeFieldObject(fieldId, ScriptConfig.Instance.getEexDefaultValue(fieldId));
                        }
                    }
                }
            }
        }

        public override JObject toJsonObject()
        {
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(id);
            JObject ret = base.toJsonObject();

            if (arrayValues.GetLength(0) > 1)
            {
                JArray jar = new JArray();
                ret.Add("array", jar);

                for (int i = 0; i < arrayValues.GetLength(0); i++)
                {
                    JArray kvArr = new JArray();
                    jar.Add(kvArr);
                    for (int j = 0; j < FieldCount; j++)
                    {
                        kvArr.Add(createKV(keys[j], arrayValues[i][j]));
                    }
                }
            }
            else
            {
                JArray fields = new JArray();
                ret.Add("fields", fields);
                for (int j = 0; j < cfg.FieldIds.Length; j++)
                {
                    int fieldId = cfg.FieldIds[j];
                    int keyIndex = j;
                    fields.Add(createKV(fieldId, values[keyIndex]));
                }
            }

            return ret;
        }

        private JObject createKV(int k, Object v)
        {
            JObject ret = new JObject();
            ret.Add("k", Util.int2HexString(k));
            ret.Add("v", JToken.FromObject(v));
            return ret;
        }

        public override bool loadJson(JObject jobj)
        {
            base.loadJson(jobj);
            initKVArray(0);
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(id);

            if (cfg.FieldRepeat <= 1)
            {
                EexFieldReader reader = new EexFieldReader((JArray)jobj["fields"]);

                for (int i = 0; i < FieldCount; i++)
                {
                    values[i] = reader.readFieldObject(keys[i]);
                }
            }
            else
            {
                JArray jar = (JArray)jobj["array"];

                for (int i = 0; i < cfg.FieldRepeat; i++)
                {
                    EexFieldReader reader = new EexFieldReader((JArray)jar[i]);

                    Object[] objs = new Object[FieldCount];
                    for (int j = 0; j < FieldCount; j++)
                    {
                        objs[j] = reader.readFieldObject(keys[j]);
                    }
                    arrayValues[i] = objs;
                }
            }

            return true;
        }
    }
}
