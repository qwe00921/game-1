using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace TRGameRepack.Repack
{

    public class Set<T>:ICollection<T>
    {
        private Dictionary<T, object> dict = new Dictionary<T,object>();
        private object objValue = new object();

        public void Add(T item)
        {
            dict.Add(item, objValue);
        }

        public void Clear()
        {
            dict.Clear();
        }

        public bool Contains(T item)
        {
            return dict.ContainsKey(item);
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            dict.Keys.CopyTo(array, arrayIndex);
        }

        public int Count
        {
            get { return dict.Count; }
        }

        public bool IsReadOnly
        {
            get { return false; }
        }

        public bool Remove(T item)
        {
            return dict.Remove(item);
        }

        public IEnumerator<T> GetEnumerator()
        {
            return dict.Keys.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return dict.Keys.GetEnumerator();
        }
    }

    public class PackageItem
    {
        public PackageItem()
        {
            Title = String.Empty;
            AppName = String.Empty;
            PackageName = String.Empty;
            MetaData = new Dictionary<string, string>();
            ResourceEncrypt = String.Empty;
            ResourceNormal = String.Empty;
            ResourceDelete = String.Empty;
        }

        public string Title
        {
            get;
            set;
        }

        public string AppName { get; set; }

        public string PackageName
        {
            get;
            set;
        }

        public string ResourceDelete
        {
            get;
            set;
        }
        public string ResourceEncrypt
        {
            get;
            set;
        }

        public string ResourceNormal
        {
            get;
            set;
        }

        public string WinPhoneDescription = "";
        public string WinPhoneProductID = "";

        public Dictionary<string, string> MetaData
        {
            get;
            private set;
        }
    }

    public class ChannelItem
    {
        public ChannelItem()
        {
            ChannelId = "";
            ChannelName = "";
        }

        public string ChannelId
        {
            get;
            set;
        }

        public string ChannelName
        {
            get;
            set;
        }
    }

    public class EncryptConfig
    {
        public EncryptConfig()
        {
            AllExts = new Set<string>();
            PartExts = new Set<string>();
            PartLength = 0;
        }

        public Set<string> AllExts
        {
            get;
            private set;
        }
        public Set<string> PartExts
        {
            get;
            private set;
        }

        public bool ContainsExt(string ext)
        {
            return AllExts.Contains(ext) || PartExts.Contains(ext);
        }

        public int getFileEncLength(string sourcePath)
        {
            String extName = Path.GetExtension(sourcePath);
            if (PartExts.Contains(extName))
            {
                return PartLength;
            }

            if (AllExts.Contains(extName))
            {
                FileInfo fi = new FileInfo(sourcePath);
                return (int)fi.Length;
            }

            return -1;
        }

        public int PartLength
        {
            get;
            set;
        }
    }

    public class RepackConfig
    {
        public RepackConfig()
        {
            ChannelList = new List<ChannelItem>();
            PackageList = new List<PackageItem>();
            EnableMM = false;
            CopyFileList = new List<string>();
            ResourceFileList = new List<string>();
            WinPhoneFileList = new List<string>();
            EnableCopyFile = false;
            EncryptCfg = new EncryptConfig();
            DeleteTemp = false;
        }

        public List<ChannelItem> ChannelList
        {
            get;
            private set;
        }
        
        public List<PackageItem> PackageList
        {
            get;
            private set;
        }

        public bool EnableMM
        {
            get;
            set;
        }

        public bool EnableCopyFile
        {
            get;
            set;
        }

        public bool DeleteTemp
        {
            get;
            set;
        }

        public List<string> CopyFileList
        {
            get;
            private set;
        }

        public List<string> ResourceFileList
        {
            get;
            private set;
        }

        public List<string> WinPhoneFileList
        {
            get;
            private set;
        }

        public EncryptConfig EncryptCfg
        {
            get;
            private set;
        }

        public void loadConfig()
        {
            {
                XmlDocument xd = new XmlDocument();
                xd.Load(Path.Combine(Application.StartupPath, "config/channelList.xml"));

                XmlNodeList xnList = xd.SelectNodes("config/channelList/channel");

                ChannelList.Clear();
                foreach (XmlNode xn in xnList)
                {
                    ChannelItem item = new ChannelItem();
                    item.ChannelName = xn.SelectSingleNode("channelName").InnerText;
                    item.ChannelId = xn.SelectSingleNode("channelId").InnerText;
                    ChannelList.Add(item);
                }
            }

            {
                XmlDocument xd = new XmlDocument();
                xd.Load(Path.Combine(Application.StartupPath, "config/packageList.xml"));

                XmlNodeList xnList = xd.SelectNodes("config/packageList/package");
                PackageList.Clear();
                foreach (XmlNode xn in xnList)
                {
                    PackageItem item = new PackageItem();
                    item.Title = xn.SelectSingleNode("pkgTitle").InnerText;
                    item.PackageName = xn.SelectSingleNode("pkgName").InnerText;
                    item.AppName = xn.SelectSingleNode("AppName").InnerText;
                    item.ResourceDelete = xn.SelectSingleNode("ResourceDelete").InnerText;
                    item.ResourceEncrypt = xn.SelectSingleNode("ResourceEncrypt").InnerText;
                    item.ResourceNormal = xn.SelectSingleNode("ResourceNormal").InnerText;
                    item.WinPhoneDescription = xn.SelectSingleNode("WinPhoneDescription").InnerText;
                    item.WinPhoneProductID = xn.SelectSingleNode("WinPhoneProductID").InnerText;

                    XmlNodeList metaDataList = xn.SelectNodes("metaData/item");
                    foreach (XmlNode metaData in metaDataList)
                    {
                        string k = metaData.Attributes["name"].Value;
                        string v = metaData.Attributes["value"].Value;
                        item.MetaData.Add(k, v);
                    }

                    PackageList.Add(item);
                }
            }


            {
                XmlDocument xd = new XmlDocument();
                xd.Load(Path.Combine(Application.StartupPath, "config/enableList.xml"));

                XmlNodeList xnList = xd.SelectNodes("config/enableList/item");
                foreach (XmlNode xn in xnList)
                {
                    switch (xn.Attributes["name"].Value)
                    {
                        case "EnableMM":
                            EnableMM = xn.Attributes["value"].Value.Equals("true");
                            break;
                        case "EnableCopyFile":
                            EnableCopyFile = xn.Attributes["value"].Value.Equals("true");
                            break;
                        case "DeleteTemp":
                            DeleteTemp = xn.Attributes["value"].Value.Equals("true");
                            break;
                        default:
                            break;
                    }
                }

                xnList = xd.SelectNodes("config/encryptConfig/item");
                foreach (XmlNode xn in xnList)
                {
                    switch (xn.Attributes["name"].Value)
                    {
                        case "AllExts":
                            {
                                EncryptCfg.AllExts.Clear();
                                string attrValue = xn.Attributes["value"].Value;
                                string[] strArr = attrValue.Split(';');
                                foreach (string item in strArr)
                                {
                                    EncryptCfg.AllExts.Add("." + item);
                                }
                            }
                            break;
                        case "PartExts":
                            {
                                EncryptCfg.PartExts.Clear();
                                string attrValue = xn.Attributes["value"].Value;
                                string[] strArr = attrValue.Split(';');
                                foreach (string item in strArr)
                                {
                                    EncryptCfg.PartExts.Add("." + item);
                                }
                            }
                            break;
                        case "PartLength":
                            EncryptCfg.PartLength = Convert.ToInt32(xn.Attributes["value"].Value);
                            break;
                        default:
                            break;
                    }
                }


                CopyFileList.Clear();
                xnList = xd.SelectNodes("config/copyFileList/item");
                foreach (XmlNode xn in xnList)
                {
                    CopyFileList.Add(xn.Attributes["name"].Value);
                }

                ResourceFileList.Clear();
                xnList = xd.SelectNodes("config/resourceFileList/item");
                foreach (XmlNode xn in xnList)
                {
                    ResourceFileList.Add(xn.Attributes["name"].Value);
                }

                WinPhoneFileList.Clear();
                xnList = xd.SelectNodes("config/winPhoneFileList/item");
                foreach (XmlNode xn in xnList)
                {
                    WinPhoneFileList.Add(xn.Attributes["name"].Value);
                }
            }
        }
    }
}
