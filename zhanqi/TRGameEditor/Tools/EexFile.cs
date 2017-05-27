using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using NPOI.XSSF.UserModel;
using TRGameEditor.Scenes;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace TRGameEditor.Tools
{
    public static class EexFile
    {
        public enum EexFileType
        {
            TYPE_EEX,
            TYPE_EEX_NEW,
            TYPE_EEXJS,
            TYPE_UNKNOW
        }

        public static EexFileType getFileType(String filePath)
        {
            switch (Path.GetExtension(filePath).ToLower())
            {
                case ".eex":
                    return EexFileType.TYPE_EEX;
                case ".eex_new":
                    return EexFileType.TYPE_EEX_NEW;
                case ".eexjs":
                    return EexFileType.TYPE_EEXJS;
                default:
                    return EexFileType.TYPE_UNKNOW;
            }
        }

        public static string getFileExtension(EexFileType fileType)
        {
            switch (fileType)
            {
                case EexFileType.TYPE_EEX:
                    return ".eex";
                case EexFileType.TYPE_EEX_NEW:
                    return ".eex_new";
                case EexFileType.TYPE_EEXJS:
                    return ".eexjs";
                default:
                    return ".dat";
            }
        }

        public static ScriptRoot loadFile(String path)
        {
            switch (getFileType(path))
            {
                case EexFileType.TYPE_EEX:
                    return loadEexFile(path);
                case EexFileType.TYPE_EEX_NEW:
                    return loadEexNewFile(path);
                case EexFileType.TYPE_EEXJS:
                    return loadJsonFile(path);
                default:
                    throw new EexReaderException(
                        String.Format("{0} 扩展名不认识，无法解析", path));
            }
        }

        public static ScriptRoot loadEexFile(String path)
        {
            using (FileStream fs = new FileStream(path, FileMode.Open))
            {
                byte[] buffer = new byte[fs.Length];
                int readLen = fs.Read(buffer, 0, buffer.Length);
                if (readLen != buffer.Length)
                {
                    throw new EexReaderException(
                        String.Format("读取字节数={0},总字节数={1}", readLen, buffer.Length));
                }
                fs.Close();
                ScriptRoot root = new ScriptRoot(path);
                root.loadReader(new EexBinaryReader(buffer));
                return root;
            }
        }

        public static ScriptRoot loadEexNewFile(String path)
        {
            using (FileStream fs = new FileStream(path, FileMode.Open))
            {
                byte[] buffer = new byte[fs.Length];
                int readLen = fs.Read(buffer, 0, buffer.Length);
                if (readLen != buffer.Length)
                {
                    throw new EexReaderException(
                        String.Format("读取字节数={0},总字节数={1}", readLen, buffer.Length));
                }
                fs.Close();
                ScriptRoot root = new ScriptRoot(path);
                root.loadNewReader(new EexBinaryReader(buffer, EexBinaryReader.ENCODING_UTF8));
                return root;
            }
        }

        public static ScriptRoot loadJsonFile(String path)
        {
            using (StreamReader sr = new StreamReader(path, Encoding.UTF8))
            {
                using (JsonTextReader jr = new JsonTextReader(sr))
                {
                    JObject jo = JObject.Load(jr);
                    ScriptRoot root = new ScriptRoot(path);
                    root.loadJson(jo);
                    return root;
                }
            }
        }

        public static ScriptRoot jsonText2ScriptRoot(String text)
        {
            JObject job = JObject.Parse(text);
            ScriptRoot root = new ScriptRoot();
            root.loadJson(job);
            return root;
        }

        public static String scriptRoot2JsonText(ScriptRoot sr)
        {
            JObject jo = sr.toJsonObject();
            String text = jo.ToString(Newtonsoft.Json.Formatting.Indented);
            return text;
        }

        public static void saveEexFile(ScriptRoot root, String path)
        {
            EexBinaryWriter writer = new EexBinaryWriter();
            root.saveWriter(writer);
            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                fs.Write(writer.getContent(), 0, writer.getIndex());
            }
        }

        public static void saveJsonFile(ScriptRoot root, String path)
        {
            using (StreamWriter sw = new StreamWriter(path, false, Encoding.UTF8))
            {
                String text = scriptRoot2JsonText(root);
                sw.Write(text);
            }
        }

        public static void saveEexNewFile(ScriptRoot root, String path)
        {
            EexBinaryWriter writer = new EexBinaryWriter(EexBinaryWriter.ENCODING_UTF8);
            root.saveNewWriter(writer);
            using (FileStream fs = new FileStream(path, FileMode.Create))
            {
                fs.Write(writer.getContent(), 0, writer.getIndex());
            }
        }

        public static void saveFile(ScriptRoot root, String path)
        {
            switch (getFileType(path))
            {
                case EexFileType.TYPE_EEX:
                    saveEexFile(root, path);
                    break;
                case EexFileType.TYPE_EEX_NEW:
                    saveEexNewFile(root, path);
                    break;
                case EexFileType.TYPE_EEXJS:
                    saveJsonFile(root, path);
                    break;
                default:
                    throw new EexReaderException(
                        String.Format("{0} 扩展名不认识，无法保存", path));
            }
        }

        public static void saveStringToExcel(ScriptRoot root, String path)
        {
            List<string> textList = new List<string>();
            foreach (ScriptScene scene in root.SceneList)
            {
                foreach (ScriptSection section in scene.SectionList)
                {
                    foreach (ScriptCommand cmd in section.CommandList)
                    {
                        if (cmd.ArrayValues.GetLength(0) > 0)
                        {
                            for (int j = 0; j < cmd.ArrayValues.GetLength(0); j++)
                            {
                                for (int i = 0; i < cmd.Keys.Length; i++)
                                {
                                    int fieldId = cmd.Keys[i];
                                    if (ScriptConfig.Instance.getEexType(fieldId) == EexType.EexString)
                                    {
                                        textList.Add(cmd.ArrayValues[j][i].ToString());
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < cmd.Keys.Length; i++)
                            {
                                int fieldId = cmd.Keys[i];
                                if (ScriptConfig.Instance.getEexType(fieldId) == EexType.EexString)
                                {
                                    textList.Add(cmd.Values[i].ToString());
                                }
                            }
                        }
                    }
                }
            }

            if (textList.Count > 0)
            {
                XSSFWorkbook workbook = new XSSFWorkbook();
                XSSFSheet sheet = (XSSFSheet)workbook.CreateSheet("Sheet1");
                for (int i = 0; i < textList.Count; i++)
                {
                    XSSFRow row = (XSSFRow)sheet.CreateRow(i);
                    row.CreateCell(0).SetCellValue(textList[i]);
                }
                using (FileStream fs = new FileStream(path, FileMode.Create))
                {
                    workbook.Write(fs);
                }
            }
        }

        public static bool loadStringFromExcel(ScriptRoot root, String path)
        {
            List<string> textList = new List<string>();
            using (FileStream fs = new FileStream(path, FileMode.Open))
            {
                XSSFWorkbook workbook = new XSSFWorkbook(fs);
                XSSFSheet sheet = (XSSFSheet)workbook.GetSheetAt(0);

                for (int i = sheet.FirstRowNum; i <= sheet.LastRowNum; i++)
                {
                    XSSFRow row = (XSSFRow)sheet.GetRow(i);
                    if (row == null || row.GetCell(0) == null)
                    {
                        textList.Add("");
                        continue;
                    }
                    textList.Add(row.GetCell(0).ToString());
                }
            }

            int textIndex = 0;

            foreach (ScriptScene scene in root.SceneList)
            {
                foreach (ScriptSection section in scene.SectionList)
                {
                    foreach (ScriptCommand cmd in section.CommandList)
                    {
                        if (cmd.ArrayValues.GetLength(0) > 0)
                        {
                            for (int j = 0; j < cmd.ArrayValues.GetLength(0); j++)
                            {
                                for (int i = 0; i < cmd.Keys.Length; i++)
                                {
                                    int fieldId = cmd.Keys[i];
                                    if (ScriptConfig.Instance.getEexType(fieldId) == EexType.EexString)
                                    {
                                        if (textIndex >= textList.Count)
                                        {
                                            //cmd.ArrayValues[j][i] = "";
                                        }
                                        else
                                        {
                                            cmd.ArrayValues[j][i] = textList[textIndex];
                                        }
                                        textIndex++;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < cmd.Keys.Length; i++)
                            {
                                int fieldId = cmd.Keys[i];
                                if (ScriptConfig.Instance.getEexType(fieldId) == EexType.EexString)
                                {
                                    if (textIndex >= textList.Count)
                                    {
                                        //cmd.Values[i] = "";
                                    }
                                    else
                                    {
                                        cmd.Values[i] = textList[textIndex];
                                    }
                                    textIndex++;
                                }
                            }
                        }
                    }
                }
            }
            return true;
        }
    }
}
