using Newtonsoft.Json.Linq;
using NPOI.SS.UserModel;
using NPOI.XSSF.UserModel;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace ChessLmTool
{
    public class ExcelJson
    {
        /// <summary>
        /// 由Excel导入DataTable
        /// </summary>
        /// <param name="excelFileStream">Excel文件流</param>
        /// <param name="sheetName">Excel工作表索引</param>
        /// <param name="headerRowIndex">Excel表头行索引</param>
        /// <returns>DataTable</returns>
        private static DataTable Excel2DataTable(Stream excelFileStream, int sheetIndex, int headerRowIndex)
        {
            XSSFWorkbook workbook = new XSSFWorkbook(excelFileStream);
            XSSFSheet sheet = (XSSFSheet)workbook.GetSheetAt(sheetIndex);
            DataTable table = new DataTable();
            table.TableName = sheet.SheetName;
            XSSFRow headerRow = (XSSFRow)sheet.GetRow(headerRowIndex);
            XSSFRow firstRow = null;
            if (headerRowIndex + 1 >= sheet.FirstRowNum && headerRowIndex + 1 <= sheet.LastRowNum)
            {
                firstRow = (XSSFRow)sheet.GetRow(headerRowIndex + 1);
            }
            int cellCount = headerRow.LastCellNum;
            for (int i = headerRow.FirstCellNum; i < cellCount; i++)
            {
                if (headerRow.GetCell(i) == null || headerRow.GetCell(i).ToString() == "")
                {
                    // 如果遇到第一个空列，则不再继续向后读取
                    cellCount = i + 1;
                    break;
                }
                Type columnType = typeof(string);
                if (firstRow != null && firstRow.GetCell(i) != null && firstRow.GetCell(i).CellType == CellType.Numeric)
                {
                    columnType = typeof(double);
                }
                DataColumn column = new DataColumn(headerRow.GetCell(i).ToString(), columnType);
                table.Columns.Add(column);
            }
            for (int i = (sheet.FirstRowNum + 1); i <= sheet.LastRowNum; i++)
            {
                XSSFRow row = (XSSFRow)sheet.GetRow(i);
                if (row == null || row.GetCell(0) == null || row.GetCell(0).ToString().Trim() == "")
                {
                    // 如果遇到第一个空行，则不再继续向后读取
                    break;
                }
                DataRow dataRow = table.NewRow();
                for (int j = row.FirstCellNum; j < cellCount; j++)
                {
                    ICell cell = row.GetCell(j);
                    if (cell == null)
                    {
                        continue;
                    }
                    else if (cell.CellType == CellType.Numeric)
                    {
                        dataRow[j] = row.GetCell(j).NumericCellValue;
                    }
                    else
                    {
                        dataRow[j] = row.GetCell(j).ToString();
                    }
                }
                table.Rows.Add(dataRow);
            }
            return table;
        }

        public static DataTable Excel2DataTable(string excelFilePath)
        {
            using (FileStream stream = System.IO.File.OpenRead(excelFilePath))
            {
                return Excel2DataTable(stream, 0, 0);
            }
        }

        public static JContainer DataTable2Json(DataTable dt)
        {
            JContainer ret = new JArray();
            foreach (DataRow dr in dt.Rows)
            {
                JObject obj = new JObject();

                for (int i = 0; i < dt.Columns.Count; i++)
                {
                    Object oi = dr[i];
                    if (oi is double || oi is int)
                    {
                        obj.Add(dt.Columns[i].ColumnName, Convert.ToInt32(oi));
                    }
                    else
                    {
                        obj.Add(dt.Columns[i].ColumnName, oi.ToString());
                    }
                }

                ret.Add(obj);
            }

            return ret;
        }

        public static DataTable Json2DataTable(JArray arr)
        {
            DataTable ret = new DataTable();

            ret.BeginLoadData();
            for(int i=0; i<arr.Count; i++)
            {
                JObject item = (JObject)arr[i];
                if (i==0)
                {
                    foreach (KeyValuePair<string, JToken> kv in item)
                    {
                        if (kv.Value.Type == JTokenType.Integer)
                        {
                            ret.Columns.Add(kv.Key, typeof(int));
                        }
                        else
                        {
                            ret.Columns.Add(kv.Key);
                        }
                    }
                }
                DataRow dr = ret.NewRow();
                for (int j=0; j<ret.Columns.Count; j++)
                {
                    if (ret.Columns[j].DataType == typeof(int))
                    {
                        dr[j] = item[ret.Columns[j].ColumnName].ToObject<int>();
                    }
                    else
                    {
                        dr[j] = item[ret.Columns[j].ColumnName].ToString();
                    }
                }
                ret.Rows.Add(dr);
            }
            ret.EndLoadData();

            return ret;
        }

        public static bool ConvertExcel2Json(string excelPath)
        {
            string jsonPath = "";
            return ConvertExcel2Json(excelPath, out jsonPath);
        }

        public static bool ConvertExcel2Json(string excelPath, out string jsonPath)
        {
            DataTable dt = Excel2DataTable(excelPath);
            JContainer jc = DataTable2Json(dt);

            String saveFile = Path.Combine(Application.StartupPath, "json_save/" + dt.TableName + ".json");
            jsonPath = saveFile;
            String saveDir = Path.GetDirectoryName(saveFile);
            if (!Directory.Exists(saveDir))
            {
                Directory.CreateDirectory(saveDir);
            }

            String fileContent = jc.ToString();
            using (StreamWriter tw = new StreamWriter(saveFile, false, Encoding.UTF8))
            {
                tw.Write(fileContent);
            }

            return true;
        }

        public static bool ConvertJson2Excel(string jsonPath)
        {
            string excelPath = "";
            return ConvertJson2Excel(jsonPath, out excelPath);
        }

        public static bool ConvertJson2Excel(string jsonPath, out string excelPath)
        {
            String text = "";
            using (StreamReader sr = new StreamReader(jsonPath, Encoding.UTF8))
            {
                text = sr.ReadToEnd();
            }

            String fileName = Path.GetFileNameWithoutExtension(jsonPath);

            JArray arr = JArray.Parse(text);
            DataTable dt = Json2DataTable(arr);
            byte[] fileContent = DataTable2Excel(dt, fileName);

            String saveFile = Path.Combine(Application.StartupPath, "excel_save/" + fileName + ".xlsx");
            excelPath = saveFile;
            String saveDir = Path.GetDirectoryName(saveFile);
            if (!Directory.Exists(saveDir))
            {
                Directory.CreateDirectory(saveDir);
            }

            using (FileStream fs = new FileStream(saveFile, FileMode.Create))
            {
                fs.Write(fileContent, 0, fileContent.Length);
            }

            return true;
        }

        /// <summary>
        /// 由DataTable导出Excel
        /// </summary>
        /// <param name="sourceTable">要导出数据的DataTable</param>
        /// <param name="sheetName">excel sheetName</param>
        /// <returns>文件内容</returns>
        private static byte[] DataTable2Excel(DataTable sourceTable, string sheetName)
        {
            XSSFWorkbook workbook = new XSSFWorkbook();
            MemoryStream ms = new MemoryStream();
            XSSFSheet sheet = (XSSFSheet)workbook.CreateSheet(sheetName);
            XSSFRow headerRow = (XSSFRow)sheet.CreateRow(0);
            // handling header.
            for (int i=0; i<sourceTable.Columns.Count; i++)
            {
                DataColumn column = sourceTable.Columns[i];
                headerRow.CreateCell(column.Ordinal).SetCellValue(column.ColumnName);
            }
            // handling value.
            int rowIndex = 1;
            foreach (DataRow row in sourceTable.Rows)
            {
                XSSFRow dataRow = (XSSFRow)sheet.CreateRow(rowIndex);
                for (int i = 0; i < sourceTable.Columns.Count; i++)
                {
                    DataColumn column = sourceTable.Columns[i];
                    if (row[i] is int || row[i] is double)
                    {
                        dataRow.CreateCell(column.Ordinal).SetCellValue(Convert.ToDouble(row[i]));
                    }
                    else
                    {
                        dataRow.CreateCell(column.Ordinal).SetCellValue(row[i].ToString());
                    }
                }
                rowIndex++;
            }
            workbook.Write(ms);
            ms.Flush();
            ms.Close();

            byte[] ret = ms.ToArray();
            
            return ret;
        }
    }
}
