using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Text;

namespace TRGameCheck.Command.Scene
{
    public abstract class ScriptCommand
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
                        return ret;
                    }
            }
        }

        protected int id = 0;
        protected int extValue = -1;
        protected int[] keys = new int[0];
        protected Object[] values = new Object[0];
        protected Object[][] arrayValues = new Object[0][];
        protected int fieldCount = 0;

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

        public abstract bool loadReader(EexBinaryReader reader);

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

        public static int parseCommandId(JObject obj)
        {
            return Convert.ToInt32(obj[JSON_KEY_CMD_ID].Value<string>(), 16);
        }

        public static int parseExtValue(JObject obj)
        {
            return obj[JSON_KEY_EXT_VALUE].Value<int>();
        }

        public String getNodeText()
        {
             CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(id);
            return String.Format("{0:x}:{1}", id, cfg.Name);
        }
    }
}
