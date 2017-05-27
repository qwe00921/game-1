using Newtonsoft.Json.Linq;
using TRGameEditor.Scenes.Commands;
using TRGameEditor.Tools;
using TRGameEditor.Datas;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameEditor.Scenes
{
    public class ScriptCommand
    {
        private const String JSON_KEY_CMD_ID = "cmdId";
        private const String JSON_KEY_EXT_VALUE = "ext";

        public static ScriptCommand createCommand(int cmdId)
        {
            switch (cmdId)
            {
                default:
                    {
                        if (!ScriptConfig.Instance.containsCommand(cmdId))
                        {
                            throw new ApplicationException(String.Format("cmd=0x{0:x2},{1} not find", cmdId, ((CommandId)cmdId).ToString()));
                        }
                        CommandObjectArray ret = new CommandObjectArray();
                        ret.Id = cmdId;
                        ret.init();
                        return ret;
                    }
            }
        }

        public void Clone(ScriptCommand cmd)
        {
            id = cmd.id;
            values = new Object[cmd.values.Length];
            for (int i = 0; i < cmd.values.Length; i++)
            {
                values[i] = cmd.values[i];
            }
        }


        public int id = 0;
        public int extValue = -1;
        public int nullValue = 0xffff;
        public int[] keys = new int[0];
        public int[] Keys
        {
            get
            {
                return keys;
            }
        }

        public Object[] values = new Object[0];

        public Object[] Values
        {
            get
            {
                return values;
            }
        }
        protected Object[][] arrayValues = new Object[0][];
        public Object[][] ArrayValues
        {
            get
            {
                return arrayValues;
            }
        }
        public int FieldCount
        {
            get
            {
                return keys.Length;
            }
        }

        public virtual int Id
        {
            get
            {
                return id;
            }
            set
            {
                id = value;
            }
        }

        public ScriptCommand()
        {
            id = -1;
        }
        public virtual void init()
        {

        }
        public virtual bool loadReader(EexBinaryReader reader)
        {
            return true;
        }

        public virtual void saveWriter(EexBinaryWriter writer)
        {
        }

        public void setExtValue(int val)
        {
            extValue = val;
        }
        public bool isExtValueAvailable()
        {
            return extValue != -1;

        }
        public int getExtValue()
        {
            return extValue;
        }

        public virtual JObject toJsonObject()
        {
            JObject ret = new JObject();

            ret.Add(JSON_KEY_CMD_ID, Util.int2HexString(id));
            ret.Add("cmdName", ScriptConfig.Instance.getCommandConfig((int)id).Name);
            if (isExtValueAvailable())
            {
                ret.Add(JSON_KEY_EXT_VALUE, extValue);
            }

            return ret;
        }

        public static int parseCommandId(JObject obj)
        {
            return Convert.ToInt32(obj[JSON_KEY_CMD_ID].Value<string>(), 16);
        }

        public static int parseExtValue(JObject obj)
        {
            return obj[JSON_KEY_EXT_VALUE].Value<int>();
        }

        public virtual bool loadJson(JObject jobj)
        {
            return true;
        }
    }
}