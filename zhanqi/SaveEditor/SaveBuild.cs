using System;
using System.Collections.Generic;
using System.Text;

namespace SaveEditor
{
    class SaveBuild
    {
        private int version;
        private int mainLevel;
        private int curRound;
        private string filePath;
        private string eventName;
        private string sceneName;
        private string battleName;
        private string storeTime;

        public SaveBuild()
        {
            version = 0;
            mainLevel = 0;
            curRound = 0;
            filePath = null;
            eventName = null;
            sceneName = null;
            battleName = null;
            storeTime = null;
        }

        public void setVersionCode(int versionCode)
        {
            version = versionCode;
        }

        public void setFilePath(string filePathText)
        {
            filePath = filePathText;
        }

        public void setEventName(string eventNameText)
        {
            eventName = eventNameText;
        }

        public void setSceneName(string sceneNameText)
        {
            sceneName = sceneNameText;
        }

        public void setBattleName(string battleNameText)
        {
            battleName = battleNameText;
        }

        public void setMainLevel(int mainLevelNum)
        {
            mainLevel = mainLevelNum;
        }

        public void setCurRound(int curRoundNum)
        {
            curRound = curRoundNum;
        }

        public void setStoreTime(string storeTimeStr)
        {
            storeTime = storeTimeStr;
        }
    }
}
