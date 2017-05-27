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

namespace TRGameRepack.Repack
{
    public class WinPhoneApp : BaseApp
    {
        public WinPhoneApp(RepackConfig cfg) : base(cfg)
        {
            
        }

        #region 覆盖基类函数

        public override void runDecompile()
        {
            unpackXap(FileBackupName, Path.GetFileNameWithoutExtension(FileBackupName));
        }

        public override void readVersion(out string verName, out int verCode)
        {
            string manifestFile = getTempPath("WMAppManifest.xml");
            XmlDocument xd = new XmlDocument();
            xd.Load(manifestFile);

            XmlNode xn = xd.DocumentElement.SelectSingleNode("App");
            string versionName = xn.Attributes["Version"].Value;
            verName = versionName;

            verCode = 0;
        }

        public override string getAssetPath(string relativePath)
        {
            return CombinePath(getTempPath("Assets/Resources"), relativePath);
        }

        #endregion


        public void packXap(string channelId, string packageName, string versionName)
        {
            logText("start pack...");
            FastZip fz = new FastZip();
            string fileName = String.Format("release_{0}_{1}_{2}.xap", packageName, versionName, channelId);
            fz.CreateZip(getOutputPath(fileName), getOutputPath(Path.GetFileNameWithoutExtension(FileBackupName)), true, null);
            logText("pack finished!");
        }

        public void editManifest(ChannelItem channel, PackageItem packageItem, string versionName)
        {
            string manifestFile = getTempPath("WMAppManifest.xml");
            XmlDocument xd = new XmlDocument();
            xd.Load(manifestFile);

            XmlNode xnApp = xd.DocumentElement.SelectSingleNode("App");
            xnApp.Attributes["Version"].Value = versionName;
            xnApp.Attributes["Title"].Value = packageItem.AppName;
            xnApp.Attributes["Publisher"].Value = packageItem.AppName;
            xnApp.Attributes["Description"].Value = packageItem.WinPhoneDescription;
            xnApp.Attributes["ProductID"].Value = packageItem.WinPhoneProductID;

            XmlNode xnTitle = xnApp.SelectSingleNode("Tokens/PrimaryToken/TemplateFlip/Title");
            xnTitle.InnerText = packageItem.AppName;

            xd.Save(manifestFile);
        }        

        private void unpackXap(string fileName, string outDir)
        {
            logText("start unpack...");
            FastZip fz = new FastZip();
            fz.ExtractZip(getOutputPath(fileName), getOutputPath(outDir), null);
            logText("unpack finished!");
        }
    }
}
