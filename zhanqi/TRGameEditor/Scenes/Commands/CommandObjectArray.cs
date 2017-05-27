using Newtonsoft.Json.Linq;
using TRGameEditor.Tools;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TRGameEditor.Scenes.Commands
{
    class CommandObjectArray : ScriptCommand
    {
        public CommandObjectArray()
        {
        }

        private void initKVArray()
        {
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(Id);

            keys = new int[cfg.FieldCount];
            Array.Copy(cfg.FieldIds, keys, FieldCount);
            values = new Object[FieldCount];

            if (cfg.FieldRepeat > 1)
            {
                arrayValues = new Object[cfg.FieldRepeat][];
            }
        }
        public Object newFieldValue(int fieldId)
        {
            ParaTypeInfo TypeInfo = CScenario.GetParaTypeInfo(fieldId);
            EexType etype = ScriptConfig.Instance.getEexType(fieldId);
            switch (etype)
            {
                case EexType.EexString:
                    {
                        string ret = "";
                        return ret;
                    }
                case EexType.EexBoolean:
                    {
                        return false;
                    }
                case EexType.EexInteger:
                    if (!TypeInfo.isAllowNULL)
                        return 0;
                    else
                    {
                        return TypeInfo.DefIndex == 0x10000 ? 0xFFFF : TypeInfo.DefIndex;
                    }                    
                case EexType.EexUShort:
                    if (!TypeInfo.isAllowNULL)
                        return (ushort)0;
                    else
                    {
                        return TypeInfo.DefIndex == 0x10000 ? (ushort)0xFFFF : (ushort)TypeInfo.DefIndex;
                    }
                case EexType.EexUShortArray:
                    {
                        ushort[] result = new ushort[0];

                        return result;
                    }
                default:
                    throw new EexReaderException(String.Format("unknow type={0}", etype.ToString()));
            }
        }
        public override void init()
        {
            initKVArray();
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(Id);

            if (cfg.FieldRepeat <= 1)
            {
                for (int i = 0; i < FieldCount; i++)
                {
                    values[i] = newFieldValue(keys[i]);
                }
            }
            else
            {
                for (int i = 0; i < cfg.FieldRepeat; i++)
                {
                    Object[] objs = new Object[FieldCount];
                    for (int j = 0; j < FieldCount; j++)
                    {
                        objs[j] = newFieldValue(keys[j]);
                    }
                    arrayValues[i] = objs;
                }
            }
        }

        public override bool loadReader(EexBinaryReader reader)
        {
            initKVArray();
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(Id);

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
            
            return true;
        }

        public override void saveWriter(EexBinaryWriter writer)
        {
            CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(id);

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
            initKVArray();
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
