using ICSharpCode.SharpZipLib.Zip;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace TRGameRepack.Repack
{
    class AndroidApp : BaseApp
    {

        public AndroidApp(RepackConfig cfg)
            : base(cfg)
        {

        }

        #region 覆盖基类函数

        public override void runDecompile()
        {
            unpackApk(FileBackupName, Path.GetFileNameWithoutExtension(FileBackupName));
        }
        public class AndroidManifestData
        {
            public string ApkFileName { get; set; }
            public bool CompressionType { get; set; }
            public List<string> DoNotCompress { get; set; }
            public bool IsFrameworkApk { get; set; }
            public PackageInfo PackageInfo { get; set; }
            public Dictionary<string, string> SdkInfo { get; set; }
            public bool SharedLibrary { get; set; }
            public Dictionary<char,char> UnknownFiles { get; set; }
            public UsesFramework UsesFramework { get; set; }
            public string Version { get; set; }
            public VersionInfo VersionInfo { get; set; }
        }
        public class UsesFramework
        {
            public List<System.Int32> ids { get; set; }
            public String tag { get; set; }
        }

        public class PackageInfo
        {
            public string forcedPackageId { get; set; }
            public string renameManifestPackage { get; set; }
        }


        public class VersionInfo
        {
            public string versionCode { get; set; }
            public string versionName { get; set; }
        }


        public override void readVersion(out string verName, out int verCode)
        {
          
            var fileData = File.ReadAllText(getTempPath("apktool.yml"));
            
            int index = fileData.IndexOf('\n');

            var subFileData = fileData.Substring(index, fileData.Length - index);
 
            var input = new StringReader(subFileData);

            var deserializer = new Deserializer(namingConvention: new CamelCaseNamingConvention());

            var manifestData = deserializer.Deserialize<AndroidManifestData>(input);
            
            verName = manifestData.VersionInfo.versionName;

            verCode = Int32.Parse(manifestData.VersionInfo.versionCode);
            

#if false
            XmlDocument xd = new XmlDocument();
            xd.Load(manifestFile);

            string versionName = xd.SelectSingleNode("manifest").Attributes["android:versionName"].Value;
            verName = versionName;

            int versionCode = int.Parse(xd.SelectSingleNode("manifest").Attributes["android:versionCode"].Value);
            verCode = versionCode; 
#endif
        }

        public override string getAssetPath(string relativePath)
        {
            return CombinePath(getTempPath("assets"), relativePath);
        }

        #endregion

        public void unzipFileList(string[] fileNameArray)
        {
            string destFileBak = getOutputPath(FileBackupName);

            using (ZipFile zf = new ZipFile(destFileBak))
            {
                foreach (string fileName in fileNameArray)
                {
                    logText("copy " + fileName + " ...");

                    ZipEntry ze = zf.GetEntry(fileName);
                    using (Stream st = zf.GetInputStream(ze))
                    {
                        using (FileStream fs = new FileStream(getTempPath(fileName), FileMode.Create))
                        {
                            byte[] buffer = new byte[16 * 1024];
                            int readLen = st.Read(buffer, 0, buffer.Length);
                            while (readLen > 0)
                            {
                                fs.Write(buffer, 0, readLen);
                                readLen = st.Read(buffer, 0, buffer.Length);
                            }
                        }
                    }

                }

            }
        }


        private void CommandDataReceived(object sender, DataReceivedEventArgs e)
        {
            logText(e.Data);
        }

        private Process initProcess()
        {
            Process process = new Process();
            //设定程序名 
            process.StartInfo.FileName = "cmd.exe";

            process.StartInfo.WorkingDirectory = Path.Combine(Application.StartupPath, "apktool");
            //关闭Shell的使用 
            process.StartInfo.UseShellExecute = false;
            //重定向标准输入 
            process.StartInfo.RedirectStandardInput = true;
            //重定向标准输出 
            process.StartInfo.RedirectStandardOutput = true;
            process.OutputDataReceived += CommandDataReceived;
            //重定向错误输出 
            process.StartInfo.RedirectStandardError = true;
            process.ErrorDataReceived += CommandDataReceived;
            //设置不显示窗口 
            process.StartInfo.CreateNoWindow = true;

            return process;
        }

        private void unpackApk(string fileName, string outDir)
        {
            logText("start unpack...");
            Process pro = initProcess();
            //执行反编译命令 
            pro.Start();
            pro.BeginOutputReadLine();
            pro.BeginErrorReadLine();
            pro.StandardInput.WriteLine(String.Format(@"apktool d ..\{0}\{1} -o ..\{0}\{2}", OutputDirName, fileName, outDir));
            pro.StandardInput.WriteLine("exit");
            pro.WaitForExit();
            pro.Close();
            logText("unpack finished!");
        }        

        public void editAppName(string appName)
        {
            string stringsFile = getTempPath(@"res\values\strings.xml");
            XmlDocument xd = new XmlDocument();
            xd.Load(stringsFile);

            XmlNode xn = findXmlNode(xd, "resources/string", "name", "app_name");
            xn.InnerText = appName;

            xd.Save(stringsFile);

            logText("edit app_name finished!");
        }        

        public void editAndroidManifest(ChannelItem channel, PackageItem packageItem, List<string> permissionDelete,
            string versionName, int versionCode)
        {
            string manifestFile = getTempPath(@"AndroidManifest.xml");
            XmlDocument xd = new XmlDocument();
            xd.Load(manifestFile);

            XmlNode nodeManifest = xd.SelectSingleNode("manifest");
            nodeManifest.Attributes["package"].Value = packageItem.PackageName;

            string apkFile = getTempPath(@"apktool.yml");           

            var fileData = File.ReadAllText(getTempPath("apktool.yml"));

            int index = fileData.IndexOf('\n');

            var subFileData = fileData.Substring(index, fileData.Length - index);//头不明类型的引用

            var titleFileData = fileData.Substring(0, index);

            var input = new StringReader(subFileData);

            var deserializer = new Deserializer(namingConvention: new CamelCaseNamingConvention());

            var manifestData = deserializer.Deserialize<AndroidManifestData>(input);

           


            if (!String.IsNullOrEmpty(versionName))
            {
                manifestData.VersionInfo.versionName = versionName;
            }
            if (versionCode > 0)
            {
                manifestData.VersionInfo.versionCode = versionCode.ToString();
            }

            var serializer = new Serializer(namingConvention: new CamelCaseNamingConvention());
           
            StringWriter textWriter = new StringWriter();
            textWriter.ToString();
            serializer.Serialize(textWriter, manifestData);


            string directorypath = titleFileData + "\n" + textWriter;
            System.IO.File.WriteAllText(apkFile, directorypath, Encoding.Default);
            System.Console.ReadLine();


            if (permissionDelete.Count != 0)
            {
                XmlNodeList permissionList = xd.SelectNodes("manifest/uses-permission");
                foreach (XmlNode item in permissionList)
                {
                    if (item.Attributes["android:name"] != null && item.Attributes["android:name"].Value != null
                        && permissionDelete.Contains(item.Attributes["android:name"].Value))
                    {
                        item.ParentNode.RemoveChild(item);
                        logText("remove permission=" + item.Attributes["android:name"].Value);
                    }
                }
            }

            XmlNodeList xnList = xd.SelectNodes("manifest/application/meta-data");
            foreach (XmlNode metaItem in xnList)
            {
                string androidName = metaItem.Attributes["android:name"].Value;
                if (packageItem.MetaData.ContainsKey(androidName))
                {
                    metaItem.Attributes["android:value"].Value = packageItem.MetaData[androidName];
                }
            }

            if (Config.EnableMM)
            {
                bool edited = false;
                XmlNodeList nodeActionList = findXmlNode(xd, "manifest/application/service", "android:name", "mm.purchasesdk.iapservice.PurchaseService")
                    .SelectNodes("intent-filter/action");

                foreach (XmlNode nodeItem in nodeActionList)
                {
                    string attrName = nodeItem.Attributes["android:name"].Value;
                    if (attrName.StartsWith("com.") && attrName.EndsWith(".purchaseservice.BIND"))
                    {
                        nodeItem.Attributes["android:name"].Value = packageItem.PackageName + ".purchaseservice.BIND";
                        edited = true;
                        break;
                    }
                }

                if (!edited)
                {
                    throw new ApplicationException("mm.purchasesdk.iapservice.PurchaseService edit error");
                }

                edited = false;
                nodeActionList = findXmlNode(xd, "manifest/application/activity", "android:name", "mm.purchasesdk.iapservice.BillingLayoutActivity")
                    .SelectNodes("intent-filter/action");

                foreach (XmlNode nodeItem in nodeActionList)
                {
                    string attrName = nodeItem.Attributes["android:name"].Value;
                    if (attrName.StartsWith("com.") && attrName.EndsWith(".com.mmiap.activity"))
                    {
                        nodeItem.Attributes["android:name"].Value = packageItem.PackageName + ".com.mmiap.activity";
                        edited = true;
                        break;
                    }
                }

                if (!edited)
                {
                    throw new ApplicationException("mm.purchasesdk.iapservice.BillingLayoutActivity edit error");
                }
            }

            //微信支付相关配置信息
            XmlNode xmlWeChat = findXmlNode(xd, "manifest/application/activity", "android:name", "com.bantu.fsyjz.wxapi.WXPayEntryActivity");
            if (xmlWeChat != null)
            {
                xmlWeChat.Attributes["android:name"].Value = packageItem.PackageName + ".wxapi.WXPayEntryActivity";
            }
            XmlNode xmlWXLogin = findXmlNode(xd, "manifest/application/activity", "android:name", "com.bantu.fsyjz.wxapi.WXEntryActivity");
            if (xmlWXLogin != null)
            {
                xmlWXLogin.Attributes["android:name"].Value = packageItem.PackageName + ".wxapi.WXEntryActivity";
            }

            //个推相关配置信息
            XmlNode xmlUsesPermission = findXmlNode(xd, "manifest/uses-permission", "android:name", "getui.permission.GetuiService.com.bantu.fsyjz");
            if (xmlUsesPermission != null)
            {
                xmlUsesPermission.Attributes["android:name"].Value = "getui.permission.GetuiService." + packageItem.PackageName;
            }
            XmlNode xmlPermission = findXmlNode(xd, "manifest/permission", "android:name", "getui.permission.GetuiService.com.bantu.fsyjz");
            if (xmlPermission != null)
            {
                xmlPermission.Attributes["android:name"].Value = "getui.permission.GetuiService." + packageItem.PackageName;
            }
            XmlNode xmlDownload = findXmlNode(xd, "manifest/application/provider", "android:name", "com.igexin.download.DownloadProvider");
            if (xmlDownload != null)
            {
                xmlDownload.Attributes["android:authorities"].Value = "downloads." + packageItem.PackageName;
            }
            XmlNode xmlReceiver = findXmlNode(xd, "manifest/application/receiver", "android:name", "com.igexin.getuiext.service.PayloadReceiver");
            if(xmlReceiver != null)
            {
                XmlNodeList xmlReceiverList = xmlReceiver.SelectNodes("intent-filter/action");
                foreach (XmlNode nodeItem in xmlReceiverList)
                {
                    string attrName = nodeItem.Attributes["android:name"].Value;
                    if (attrName.StartsWith("com.igexin.sdk.action.") && !attrName.EndsWith("7fjUl2Z3LH6xYy7NQK4ni4"))
                    {
                        if (packageItem.MetaData.ContainsKey("PUSH_APPID"))
                        {
                            nodeItem.Attributes["android:name"].Value = "com.igexin.sdk.action." + packageItem.MetaData["PUSH_APPID"];
                            break;
                        }
                    }
                }
            }
            xd.Save(manifestFile);
        }

        public void packApk(string channelId, string packageName, string versionName)
        {
            logText("start process " + channelId + " ...");

            Process pro = initProcess();
            pro.Start();
            pro.BeginOutputReadLine();
            pro.BeginErrorReadLine();
            pro.StandardInput.WriteLine(@"set");
            if (Config.EnableCopyFile)
            {
                pro.StandardInput.WriteLine(String.Format(@"apktool b ..\{0}\{1} -o ..\{0}\unsigned.apk", OutputDirName, Path.GetFileNameWithoutExtension(FileBackupName)));
                Console.Write(String.Format(@"apktool b ..\{0}\{1} -o ..\{0}\unsigned.apk", OutputDirName, Path.GetFileNameWithoutExtension(FileBackupName)));
                pro.StandardInput.WriteLine("exit");
                pro.WaitForExit();
                pro.Close();

                using (ZipFile zf = new ZipFile(getOutputPath("unsigned.apk")))
                {
                    zf.BeginUpdate();

                    foreach (string fileName in Config.CopyFileList)
                    {
                        logText("copy back " + fileName + " ...");
                        zf.Add(getTempPath(fileName), fileName);
                    }

                    zf.CommitUpdate();
                }

                pro = initProcess();
                pro.Start();
                pro.BeginOutputReadLine();
                pro.BeginErrorReadLine();

                logText("continue pack ...");
            }
            else
            {
                Console.Write(String.Format(@"apktool b ..\{0}\{1} -o ..\{0}\unsigned.apk", OutputDirName, Path.GetFileNameWithoutExtension(FileBackupName)));
                pro.StandardInput.WriteLine(String.Format(@"del ..\{0}\temp\res\drawable-xxhdpi-v4\icon.png", OutputDirName));
                pro.StandardInput.WriteLine(String.Format(@"apktool b ..\{0}\{1} -o ..\{0}\unsigned.apk", OutputDirName, Path.GetFileNameWithoutExtension(FileBackupName)));
            }
            pro.StandardInput.WriteLine(String.Format(@"rd/s/q ..\{0}\temp\build", OutputDirName));
            pro.StandardInput.WriteLine(String.Format(@"rd/s/q ..\{0}\temp\original", OutputDirName));
            //pro.StandardInput.WriteLine(String.Format(@"jarsigner -verbose -keystore bantus.keystore -storepass bantus123 -signedjar ..\{0}\unalign.apk ..\{0}\unsigned.apk bantus", outputDirName));
            //pro.StandardInput.WriteLine(String.Format(@"jarsigner -keystore bantus.keystore -storepass bantus123 -signedjar ..\{0}\unalign.apk ..\{0}\unsigned.apk bantus", outputDirName));
            pro.StandardInput.WriteLine(String.Format(@"jarsigner -digestalg SHA1 -sigalg MD5withRSA -tsa http://timestamp.digicert.com -keystore bantus.keystore -storepass bantus123 -signedjar ..\{0}\unalign.apk ..\{0}\unsigned.apk bantus", OutputDirName));         
            //pro.StandardInput.WriteLine(String.Format(@"zipalign -v 4 ..\{0}\unalign.apk ..\{0}\release_{1}_{2}_{3}.apk", outputDirName, packageName, versionName, channelId));
            pro.StandardInput.WriteLine(String.Format(@"zipalign 4 ..\{0}\unalign.apk ..\{0}\release_{1}_{2}_{3}.apk", OutputDirName, packageName, versionName, channelId));
            pro.StandardInput.WriteLine(String.Format(@"del ..\{0}\unsigned.apk", OutputDirName));
            pro.StandardInput.WriteLine(String.Format(@"del ..\{0}\unalign.apk", OutputDirName));
            pro.StandardInput.WriteLine("exit");
            pro.WaitForExit();
            pro.Close();
            logText("process " + channelId + " finished!");
        }
    }
}
