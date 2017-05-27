using ICSharpCode.SharpZipLib.Zip;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace TRGameRepack.Repack
{
    public class CommandProcess
    {
        private RepackConfig Config = null;

        public CommandProcess(RepackConfig cfg)
        {
            Config = cfg;
        }

        public bool DeleteGameResDir = false;
        public string ResourceApkPath = "";
        public string SourceFile = "";
        public PackageItem Package = null;
        public List<ChannelItem> ChannelList = new List<ChannelItem>();
        public List<string> DeletePath = new List<string>();
        public List<string> EncryptOverwrite = new List<string>();
        public List<string> NormalOverwrite = new List<string>();
        public List<string> PermissionDelete = new List<string>();
        public List<string> OtherJsonFile = new List<string>();
        public List<string> GlobalresOverwrite = new List<string>();
        public string VersionName = "";
        public int VersionCode = 0;
        private string outputDirName = "";

        public event Action<String> OutputDataReceived;

        private void DataReceived(object sender, DataReceivedEventArgs e)
        {
            if (OutputDataReceived != null)
            {
                //OutputDataReceived(e.Data);
            }
        }

        private void logText(String text)
        {
            if (OutputDataReceived != null)
            {
                OutputDataReceived(text);
            }
        }

        #region Android 打包

        public void runAndroidCommand()
        {
            AndroidApp resourceApp = null;
            AndroidApp androidApp = new AndroidApp(Config);
            androidApp.OutputDataReceived += logText;

            outputDirName = buildOutputDir();
            androidApp.OutputDirName = outputDirName;
            Directory.CreateDirectory(androidApp.getOutputPath(""));
            logText("build " + outputDirName + " finished!");
            androidApp.FileBackupName = "temp.bak";
            androidApp.loadFile(SourceFile);
            logText("copy to " + androidApp.FileBackupName + " finished!");
            androidApp.runDecompile();
            logText("decompile " + androidApp.FileBackupName + " finished!");

            if (Config.EnableCopyFile)
            {
                androidApp.unzipFileList(Config.CopyFileList.ToArray());
            }

            androidApp.deleteFileAndDir(DeletePath.ToArray());

            if (DeleteGameResDir)
            {
                androidApp.deleteGameResDir();
            }

            if (!String.IsNullOrEmpty(ResourceApkPath))
            {
                resourceApp = new AndroidApp(Config);
                resourceApp.OutputDataReceived += logText;

                resourceApp.OutputDirName = outputDirName;
                resourceApp.FileBackupName = "resource.bak";
                resourceApp.loadFile(ResourceApkPath);
                logText("copy to " + resourceApp.FileBackupName + " finished!");
                resourceApp.runDecompile();
                logText("decompile " + resourceApp.FileBackupName + " finished!");

                logText("start copy...");

                JObject funJson = resourceApp.readFunctionTxt();
                string gameDir = funJson["GamePath"].ToObject<string>();
                bool resEncry = funJson["EncryptResource"].ToObject<bool>();

                foreach (string itemName in Config.ResourceFileList)
                {
                    string pathName = itemName;
                    bool needEncry = false;
                    if (itemName.Contains("%gamedir%"))
                    {
                        pathName = itemName.Replace("%gamedir%", gameDir);
                        needEncry = !resEncry;
                    }

                    if (Directory.Exists(resourceApp.getTempPath(pathName)))
                    {
                        resourceApp.DirectoryCopy(resourceApp.getTempPath(pathName), androidApp.getTempPath(pathName) + "GameResources\\", needEncry);
                    }
                    else if (File.Exists(resourceApp.getTempPath(pathName)))
                    {
                        resourceApp.FileCopy(resourceApp.getTempPath(pathName), androidApp.getTempPath(pathName) +"GameResources\\", needEncry);
                    }
                }

                logText("copy from " + resourceApp.FileBackupName + " finished!");
            }

            foreach (string strPath in EncryptOverwrite)
            {
                if (Directory.Exists(strPath))
                {
                    androidApp.DirectoryCopy(strPath, androidApp.getAssetPath("GameResources"), true);
                    logText("copy " + strPath + " finished!");
                }
            }
            foreach(string strPath in EncryptOverwrite)
            {
                if (Directory.Exists(strPath))
                {
                    string GameResources = "GameResources\\";
                    string globalres = "\\globalres";
                    JObject funJson = androidApp.readFunctionTxt();
                    string gameDir = funJson["GamePath"].ToObject<string>();
                    string localGameResGlobal = androidApp.getAssetPath(GameResources + gameDir + globalres);
                    if (Directory.Exists(localGameResGlobal))
                    {
                      //  androidApp.DirectoryCopy(localGameResGlobal, androidApp.getAssetPath("GameResources" + globalres), false);//copy globalres
                    }
                    androidApp.DirectoryCopy(strPath + "\\" + gameDir, androidApp.getAssetPath(GameResources + gameDir), true);
                    if (Directory.Exists(localGameResGlobal))
                    {
                       // Directory.Delete(localGameResGlobal, true);
                       // logText("delete" + localGameResGlobal + "finished");
                    }
                }
            }

            foreach (string strPath in NormalOverwrite)
            {
                if (Directory.Exists(strPath))
                {
                    androidApp.DirectoryCopy(strPath, androidApp.getTempPath(""), false);
                    logText("copy " + strPath + " finished!");
                }
            }
            {
                JObject funJson = androidApp.readFunctionTxt();
                string gameDir = funJson["GamePath"].ToObject<string>();
                foreach (string strPath in OtherJsonFile)
                {
                    string fileName = Path.GetFileName(strPath);
                    androidApp.FileCopy(strPath, androidApp.getJsonPath(gameDir, fileName), true);
                }
            }
            foreach (string strPath in GlobalresOverwrite)
            {
                if (Directory.Exists(strPath))
                {
                    androidApp.DirectoryCopy(strPath, androidApp.getAssetPath(""), false);
                    logText("copy " + strPath + " finished!");
                }
            }

            //修改AppName
            androidApp.editAppName(Package.AppName);

            foreach (ChannelItem channel in ChannelList)
            {
                logText("pack " + channel.ChannelId);

                int appVerCode = 0;
                string appVerName = "";
                androidApp.readVersion(out appVerName, out appVerCode);

                if (String.IsNullOrEmpty(VersionName))
                {
                    VersionName = appVerName;
                }
                if (VersionCode <= 0)
                {
                    VersionCode = appVerCode;
                }
                //修改AndroidManifest文件
                androidApp.editAndroidManifest(channel, Package, PermissionDelete, VersionName, VersionCode);
                logText("editAndroidManifest finished!");

                androidApp.editFunctionTxt(channel.ChannelId, true, Package.PackageName);
                logText("editFunctionTxt finished!");

                androidApp.packApk(channel.ChannelId, Package.PackageName, VersionName);
            }

            if (Config.DeleteTemp)
            {
                androidApp.deleteTempDir();
                if (resourceApp!=null)
                { 
                    resourceApp.deleteTempDir();
                }
            }
            logText("all finished!");
        }

        public void runAndroidDecompile()
        {
            AndroidApp androidApp = new AndroidApp(Config);
            androidApp.OutputDataReceived += logText;

            outputDirName = buildOutputDir();
            androidApp.OutputDirName = outputDirName;
            Directory.CreateDirectory(androidApp.getOutputPath(""));
            logText("build " + outputDirName + " finished!");
            androidApp.FileBackupName = "temp.bak";
            androidApp.loadFile(SourceFile);
            logText("copy to " + androidApp.FileBackupName + " finished!");
            androidApp.runDecompile();
            logText("decompile finished!");
            androidApp.readVersion(out VersionName, out VersionCode);
        }

        #endregion

        #region WinPhone 打包

        public void runWinPhoneCommand()
        {
            WinPhoneApp resourceApp = null;
            WinPhoneApp wpApp = new WinPhoneApp(Config);
            wpApp.OutputDataReceived += logText;

            outputDirName = buildOutputDir();
            wpApp.OutputDirName = outputDirName;
            Directory.CreateDirectory(wpApp.getOutputPath(""));
            logText("build " + outputDirName + " finished!");
            wpApp.FileBackupName = "temp.bak";
            wpApp.loadFile(SourceFile);
            logText("copy to " + wpApp.FileBackupName + " finished!");
            wpApp.runDecompile();
            logText("decompile " + wpApp.FileBackupName + " finished!");

            wpApp.deleteFileAndDir(DeletePath.ToArray());

            if (DeleteGameResDir)
            {
                wpApp.deleteGameResDir();
            }

            if (!String.IsNullOrEmpty(ResourceApkPath))
            {
                resourceApp = new WinPhoneApp(Config);
                resourceApp.OutputDataReceived += logText;

                resourceApp.OutputDirName = outputDirName;
                resourceApp.FileBackupName = "resource.bak";
                resourceApp.loadFile(ResourceApkPath);
                logText("copy to " + resourceApp.FileBackupName + " finished!");
                resourceApp.runDecompile();
                logText("decompile " + resourceApp.FileBackupName + " finished!");

                logText("start copy...");
                JObject funJson = resourceApp.readFunctionTxt();
                string gameDir = funJson["GamePath"].ToObject<string>();
                bool resEncry = funJson["EncryptResource"].ToObject<bool>();
                foreach (string itemName in Config.ResourceFileList)
                {
                    string pathName = itemName;
                    bool needEncry = false;
                    if (itemName.Contains("%gamedir%"))
                    {
                        pathName = itemName.Replace("%gamedir%", gameDir);
                        needEncry = !resEncry;
                    }

                    if (Directory.Exists(resourceApp.getTempPath(pathName)))
                    {
                        resourceApp.DirectoryCopy(resourceApp.getTempPath(pathName), wpApp.getTempPath(pathName), needEncry);
                    }
                    else if (File.Exists(resourceApp.getTempPath(pathName)))
                    {
                        resourceApp.FileCopy(resourceApp.getTempPath(pathName), wpApp.getTempPath(pathName), needEncry);
                    }
                }
                logText("copy from " + resourceApp.FileBackupName + " finished!");
            }

            foreach (string strPath in EncryptOverwrite)
            {
                if (Directory.Exists(strPath))
                {
                    wpApp.DirectoryCopy(strPath, wpApp.getAssetPath(""), true);
                    logText("copy " + strPath + " finished!");
                }
            }

            foreach (string strPath in NormalOverwrite)
            {
                if (Directory.Exists(strPath))
                {
                    wpApp.DirectoryCopy(strPath, wpApp.getTempPath(""), false);
                    logText("copy " + strPath + " finished!");
                }
            }
            {
                JObject funJson = wpApp.readFunctionTxt();
                string gameDir = funJson["GamePath"].ToObject<string>();
                foreach (string strPath in OtherJsonFile)
                {
                    string fileName = Path.GetFileName(strPath);
                    wpApp.FileCopy(strPath, wpApp.getJsonPath(gameDir, fileName), true);
                }
            }
            //修改AppName

            foreach (ChannelItem channel in ChannelList)
            {
                logText("pack " + channel.ChannelId);

                int appVerCode = 0;
                string appVerName = "";
                wpApp.readVersion(out appVerName, out appVerCode);

                if (String.IsNullOrEmpty(VersionName))
                {
                    VersionName = appVerName;
                }
                if (VersionCode <= 0)
                {
                    VersionCode = appVerCode;
                }
                //修改Manifest文件
                wpApp.editManifest(channel, Package, VersionName);
                logText("editManifest finished!");

                wpApp.editFunctionTxt(channel.ChannelId, true, Package.PackageName);
                logText("editFunctionTxt finished!");

                wpApp.packXap(channel.ChannelId, Package.PackageName, VersionName);
            }

            if (Config.DeleteTemp)
            {
                wpApp.deleteTempDir();
                if (resourceApp != null)
                {
                    resourceApp.deleteTempDir();
                }
            }
            logText("all finished!");
        }
        
        public void runWinPhoneDecompile()
        {
            WinPhoneApp wpApp = new WinPhoneApp(Config);
            wpApp.OutputDataReceived += logText;

            outputDirName = buildOutputDir();
            wpApp.OutputDirName = outputDirName;
            Directory.CreateDirectory(wpApp.getOutputPath(""));
            logText("build " + outputDirName + " finished!");
            wpApp.FileBackupName = "temp.bak";
            wpApp.loadFile(SourceFile);
            logText("copy to " + wpApp.FileBackupName + " finished!");
            wpApp.runDecompile();
            logText("decompile finished!");
            wpApp.readVersion(out VersionName, out VersionCode);
        }

        #endregion

        private string buildOutputDir()
        {
            DateTime dt = DateTime.Now;
            string dirName = dt.ToString("yyyyMMdd_HHmmss");
            return dirName;
        }
    }
}
