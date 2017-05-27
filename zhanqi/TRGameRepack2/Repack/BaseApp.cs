using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace TRGameRepack.Repack
{
    public abstract class BaseApp
    {
        public BaseApp(RepackConfig cfg)
        {
            Config = cfg;
        }

        protected RepackConfig Config;

        #region 日志输出
        public event Action<String> OutputDataReceived;

        protected void OnOutputDataReceived(String text)
        {
            if (OutputDataReceived != null)
            {
                OutputDataReceived(text);
            }
        }

        protected void logText(String text)
        {
            OnOutputDataReceived(text);
        }
        #endregion

        #region 编译和打包功能

        public abstract void runDecompile();

        public string readGameDirName()
        {
            JObject funJson = readFunctionTxt();
            string gameDir = funJson["GamePath"].ToObject<string>();
            return gameDir;
        }

        public void deleteGameResDir()
        {
            string destDir = getAssetPath("GameResources\\"+readGameDirName());
            if (Directory.Exists(destDir))
            {
                Directory.Delete(destDir, true);
                logText("delete " + destDir + " finished!");
            }
        }

        public JObject readFunctionTxt()
        {
            return readJsonFile(getAssetPath("GameResources\\" + "function.txt"));
        }

        public string getJsonPath(string jsonName)
        {
            return CombinePath(getAssetPath(readGameDirName()), "json", jsonName);
        }

        public abstract void readVersion(out string verName, out int verCode);

        public void editFunctionTxt(string channelId, bool encryptRes, string packageName)
        {
            JObject funJson = readFunctionTxt();
            
            funJson["TalkingDataChannelId"] = channelId;
            if (encryptRes)
            {
                funJson["EncryptResource"] = true;
            }

            funJson["PackageName"] = packageName;

            String fileContent = funJson.ToString(Newtonsoft.Json.Formatting.Indented);
            using (StreamWriter tw = new StreamWriter(getAssetPath("GameResources\\" + "function.txt"), false, Encoding.UTF8))
            {
                tw.Write(fileContent);
            }
        }

        #endregion

        #region 文件和目录功能

        public void deleteTempDir()
        {
            if (Directory.Exists(getTempPath("")))
            {
                Directory.Delete(getTempPath(""), true);
            }
        }

        public void deleteFileAndDir(string[] fileNameArray)
        {
            foreach (string strPath in fileNameArray)
            {
                string destPath = getTempPath(strPath);
                if (File.Exists(destPath))
                {
                    File.Delete(destPath);
                    logText("delete " + strPath + " finished!");
                }
                else if (Directory.Exists(destPath))
                {
                    Directory.Delete(destPath, true);
                    logText("delete " + strPath + " finished!");
                }
            }
        }        

        public String OutputDirName = "";
        public String FileBackupName = "";

        public string getOutputPath(string relativePath)
        {
            return CombinePath(Application.StartupPath, OutputDirName, relativePath);
        }

        public string getTempPath(string relativePath)
        {
            return CombinePath(getOutputPath(Path.GetFileNameWithoutExtension(FileBackupName)), relativePath);
        }

        public abstract string getAssetPath(string relativePath);

        public string getJsonPath(string gameDir, string jsonName)
        {
            return CombinePath(getAssetPath(gameDir), "json", jsonName);
        }        

        public void loadFile(string fromFilePath)
        {
            File.Copy(fromFilePath, getOutputPath(FileBackupName), true);
        }

        #endregion               
        
        #region 通用功能

        protected static JObject readJsonFile(string jsPath)
        {
            using (StreamReader sr = new StreamReader(jsPath, Encoding.UTF8))
            {
                using (JsonTextReader jr = new JsonTextReader(sr))
                {
                    JObject funJson = JObject.Load(jr);
                    return funJson;
                }
            }
        }

        protected static XmlNode findXmlNode(XmlNode node, string path, string attrName, string attrValue)
        {
            XmlNodeList xnList = node.SelectNodes(path);
            foreach (XmlNode item in xnList)
            {
                if (item.Attributes[attrName] != null && item.Attributes[attrName].Value != null && item.Attributes[attrName].Value.Equals(attrValue))
                {
                    return item;
                }
            }

            return null;
        }

        protected static String CombinePath(params string[] pathArr)
        {
            switch (pathArr.Length)
            {
                case 1:
                    return pathArr[0];
                case 2:
                    return Path.Combine(pathArr[0], pathArr[1]);
                default:
                    {
                        String result = pathArr[0];
                        for (int i = 1; i < pathArr.Length; i++)
                        {
                            result = Path.Combine(result, pathArr[i]);
                        }
                        return result;
                    }
            }
        }

        public void DirectoryCopy(string sourceDirName, string destDirName , bool encrypt)
        {
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);
            DirectoryInfo[] dirs = dir.GetDirectories();

            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }

            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(destDirName, file.Name);

                if (encrypt && (Config.EncryptCfg.ContainsExt(file.Extension)))
                {
                    FileCopy(file.FullName, temppath, true);
                }
                else
                {
                    FileCopy(file.FullName, temppath, false);
                }
            }


            foreach (DirectoryInfo subdir in dirs)
            {
                string temppath = Path.Combine(destDirName, subdir.Name);
                DirectoryCopy(subdir.FullName, temppath, encrypt);
            }
        }

        public void FileCopy(string sourceFile, string destFile, bool encrypt)
        {
            if (!encrypt)
            {
                File.Copy(sourceFile,  destFile , true);
                return;
            }

            int encLength = Config.EncryptCfg.getFileEncLength(sourceFile);
            if (encLength == -1)
            {
                if (!sourceFile.Equals(destFile))
                {
                    File.Copy(sourceFile, destFile, true);
                }
                return;
            }

            byte[] fileBuffer = null;
            //读取文件
            using (FileStream fs = new FileStream(sourceFile, FileMode.Open))
            {
                int readIndex = 0;
                fileBuffer = new byte[fs.Length];
                int readLen = fs.Read(fileBuffer, readIndex, fileBuffer.Length - readIndex);
                while (readLen > 0 && readLen + readIndex < fileBuffer.Length)
                {
                    readIndex += readLen;
                    readLen = fs.Read(fileBuffer, readIndex, fileBuffer.Length - readIndex);
                }
            }
            SecretFile secretFile = new SecretFile();
            secretFile.setVersionCode(1);
            secretFile.setCodeKey("gYjhBMzjNtSaVYDV");
            secretFile.saveFile(fileBuffer, destFile, encLength);
        }

        #endregion
    }
}
