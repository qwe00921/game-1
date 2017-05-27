using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace ChessLmTool.Data
{
    public class GameData
    {
        private int hero_num = 0;
        private DicHeroAttr[] heros = { };
        private int job_num = 0;
        private DicJobAttr[] jobs = { };
        private int good_num = 0;
        private DicItemAttr[] goods = { };
        private int skill_num = 0;
        private DicSkillAttr[] skills = { };
        private int land_num = 0;
        private LandformAttr[] lands = { };
        private int goodtype_num = 0;
        private GoodTypeAttr[] goodtypes = { };
        private byte[][] job_skill_info = { };
        private byte[][] job_terrain_info = { };
        private byte[][] job_walk_info = { };
        private byte[][] job_good_info = { };
        private const int MAX_BATTLE_NUM = 58;
        private readonly ShopAttr[] shop_info = new ShopAttr[MAX_BATTLE_NUM];
        private byte[][] skill_land_info = new byte[5][];
        private int gk_num = 0;
        private DicGkAttr[] gks = { };
        private byte[][] job_pk_info = { };

        public bool parseLM(MemoryReader mr)
        {
            hero_num = mr.readInt32();
            heros = new DicHeroAttr[hero_num];
            for (int i = 0; i < hero_num; i++ )
            {
                heros[i] = new DicHeroAttr();
                heros[i].parseLM(mr);
            }

            job_num = mr.readInt32();
            jobs = new DicJobAttr[job_num];
            for (int i = 0; i < job_num; i++)
            {
                jobs[i] = new DicJobAttr();
                jobs[i].parseLM(mr);
            }

            good_num = mr.readInt32();
            goods = new DicItemAttr[good_num];
            for (int i = 0; i < good_num; i++)
            {
                goods[i] = new DicItemAttr();
                goods[i].parseLM(mr);
            }

            skill_num = mr.readInt32();
            skills = new DicSkillAttr[skill_num];
            for (int i = 0; i < skill_num; i++)
            {
                skills[i] = new DicSkillAttr();
                skills[i].parseLM(mr);
            }

            land_num = mr.readInt32();
            lands = new LandformAttr[land_num];
            for (int i = 0; i < land_num; i++)
            {
                lands[i] = new LandformAttr();
                lands[i].parseLM(mr);
            }

            goodtype_num = mr.readInt32();
            goodtypes = new GoodTypeAttr[goodtype_num];
            for (int i = 0; i < goodtype_num; i++)
            {
                goodtypes[i] = new GoodTypeAttr();
                goodtypes[i].parseLM(mr);
            }

            job_skill_info = new byte[job_num][];
            for (int i = 0; i < job_num; i++)
            {
                job_skill_info[i] = new byte[skill_num];
                mr.readBytes(job_skill_info[i]);
            }

            job_terrain_info = new byte[job_num][];
            for (int i = 0; i < job_num; i++)
            {
                job_terrain_info[i] = new byte[land_num];
                mr.readBytes(job_terrain_info[i]);
            }

            job_walk_info = new byte[job_num][];
            for (int i = 0; i < job_num; i++)
            {
                job_walk_info[i] = new byte[land_num];
                mr.readBytes(job_walk_info[i]);
            }

            job_good_info = new byte[job_num][];
            for (int i = 0; i < job_num; i++)
            {
                job_good_info[i] = new byte[good_num];
                mr.readBytes(job_good_info[i]);
            }

            for (int i=0; i<shop_info.Length; i++)
            {
                shop_info[i] = new ShopAttr();
                shop_info[i].parseLM(mr);
            }

            for (int i = 0; i < skill_land_info.Length; i++)
            {
                skill_land_info[i] = new byte[land_num];
            }

            //
            skill_land_info[0][0] = 1;
            skill_land_info[0][1] = 1;
            skill_land_info[0][2] = 1;
            skill_land_info[0][8] = 1;
            skill_land_info[0][16] = 1;
            skill_land_info[0][18] = 1;
            skill_land_info[0][19] = 1;
            skill_land_info[0][20] = 1;
            skill_land_info[0][21] = 1;
            skill_land_info[0][22] = 1;
            skill_land_info[0][23] = 1;
            skill_land_info[0][24] = 1;
            skill_land_info[0][25] = 1;
            //
            skill_land_info[1][7] = 1;
            skill_land_info[1][8] = 1;
            skill_land_info[1][9] = 1;
            skill_land_info[1][10] = 1;
            skill_land_info[1][11] = 1;
            skill_land_info[1][12] = 1;
            skill_land_info[1][13] = 1;
            //
            skill_land_info[2][3] = 1;
            skill_land_info[2][4] = 1;
            skill_land_info[2][5] = 1;
            skill_land_info[2][6] = 1;
            //
            skill_land_info[3][0] = 1;
            skill_land_info[3][1] = 1;
            skill_land_info[3][2] = 1;
            skill_land_info[3][3] = 1;
            skill_land_info[3][4] = 1;
            skill_land_info[3][5] = 1;
            skill_land_info[3][6] = 1;
            skill_land_info[3][7] = 1;
            skill_land_info[3][8] = 1;
            skill_land_info[3][9] = 1;
            skill_land_info[3][10] = 1;
            skill_land_info[3][11] = 1;
            skill_land_info[3][12] = 1;
            skill_land_info[3][13] = 1;                      

            return true;
        }

        public bool parseE5(MemoryReader mr)
        {
            GameItem item = ToolConfig.Instance.Current;

            hero_num = 0x400;
            heros = new DicHeroAttr[hero_num];
            for (int i = 0; i < hero_num; i++)
            {
                mr.setIndex(0x18c + i * 0x20);
                heros[i] = new DicHeroAttr();
                heros[i].hid = (ushort)i;
                heros[i].parseE5(mr);
            }

            job_num = item.e5_job_num;
            jobs = new DicJobAttr[job_num];
            for (int i = 0; i < job_num; i++)
            {
                mr.setIndex(item.e5_job_attr_offset + i * item.e5_job_attr_size);
                jobs[i] = new DicJobAttr();
                jobs[i].jobid = (ushort)i;
                jobs[i].parseE5(mr);
            }

            mr.setIndex(0x5973);
            land_num = 0x1e;
            lands = new LandformAttr[land_num];
            for (int i = 0; i < land_num; i++ )
            {
                lands[i] = new LandformAttr();
                lands[i].landform_id = (ushort)i;
                lands[i].parseE5(mr);
            }

            int jobLandLen = item.e5_jobLandLen;
            job_terrain_info = new byte[jobLandLen][];
            job_walk_info = new byte[jobLandLen][];
            mr.setIndex(item.e5_job_land_info_offset);
            for (int i = 0; i < jobLandLen; i++)
            {
                job_terrain_info[i] = new byte[land_num];
                job_walk_info[i] = new byte[land_num];
                mr.readBytes(job_terrain_info[i]);
                mr.readBytes(job_walk_info[i]);
            }

            good_num = item.e5_item_num + item.star_e5_item_num;
            goods = new DicItemAttr[good_num];
            for (int i = 0; i < item.e5_item_num; i++ )
            {
                mr.setIndex(item.e5_item_attr_offset + 0x19 * i);
                goods[i] = new DicItemAttr();
                goods[i].good_id = (ushort)i;
                goods[i].parseE5(mr);
            }

            job_good_info = new byte[job_num][];
            for (int i = 0; i < job_num; i++ )
            {
                job_good_info[i] = new byte[good_num];
            }

            for (int i = 0; i < 53; i++)
            {
                mr.setIndex(0x53dc + 0x09 + goods[i].itid);
                for (int j = 0; j < job_num; j++)
                {
                    mr.setIndex(0x53dc + 0x09 + goods[i].itid + j * 0x1b);
                    job_good_info[j][i] = mr.readByte();
                }
            }

            for (int i = 53; i < 87; i++)
            {
                DicItemAttr good = goods[i];
                if (good.up_val > 10000)
                {
                    for (int j = 0; j < job_num; j++)
                    {
                        job_good_info[j][i] = 1;
                    }
                }
                else 
                {
                    for (int j = 0; j < job_num; j++)
                    {
                        job_good_info[j][i] = 0;
                    }
                    //TODO upgrade_value out of range
                    //occupation_good_info[good.upgrade_value * 3][i] = 1;
                    //occupation_good_info[good.upgrade_value * 3 + 1][i] = 1;
                    //occupation_good_info[good.upgrade_value * 3 + 2][i] = 1;
                }
                good.up_val = 0;
            }

            for (int i = 87; i < 104; i++)
            {
                for (int j = 0; j < job_num; j++)
                {
                    job_good_info[j][i] = 1;
                }
            }

            skill_num = item.e5_skill_num;
            skills = new DicSkillAttr[skill_num];
            job_skill_info = new byte[job_num][];
            for (int i=0; i<job_num; i++)
            {
                job_skill_info[i] = new byte[skill_num];
            }
            for (int i = 0; i < skill_num; i++ )
            {
                mr.setIndex(item.e5_skill_attr_offset + item.e5_skill_attr_size * i);
                skills[i] = new DicSkillAttr();
                skills[i].skid = (ushort)i;
                skills[i].parseE5(mr);

                for(int j = 0;j<job_num; j++)
			    {
                    job_skill_info[j][i] = mr.readByte();
			    }
            }

            mr.setIndex(0x4bb4);
            for (int i = 0; i < shop_info.Length; i++)
            {
                shop_info[i] = new ShopAttr();
                shop_info[i].parseE5(mr);
            }

            gk_num = item.e5_gk_num;
            gks = new DicGkAttr[gk_num];
            mr.setIndex(item.e5_gk_attr_offset);
            for (int i = 0; i < gks.Length; i++)
            {
                gks[i] = new DicGkAttr();
                gks[i].gkid = (ushort)i;
                gks[i].parseE5(mr);
            }

            return true;
        }

        public bool parseExe(MemoryReader mr)
        {
            mr.setIndex(0xa3280);
            int occupPkLen = 40;
            job_pk_info = new byte[occupPkLen][];
            for (int i = 0; i < occupPkLen; i++ )
            {
                job_pk_info[i] = new byte[occupPkLen];
                mr.readBytes(job_pk_info[i]);
            }

            return true;
        }

        private static readonly int[] mingjiangbiao = {0,1,2,3,6,7,9,13,15,16,17,33,34,36,57,84,95,108,109,119,32};

        public bool parseImsg(MemoryReader mr)
        {
            GameItem item = ToolConfig.Instance.Current;

            mr.setIndex(0);
            for (int i = 0; i < good_num; i++)//200
            {
                goods[i].info = mr.readString(0xc8);
            }

            mr.setIndex(item.imsg_skill_info_offset);
            for (int i = 0; i < skill_num; i++)
            {
                skills[i].info = mr.readString(0xc8);
            }

            mr.setIndex(0x11170);
            for (int i = 0; i < job_num; i++)
            {
                jobs[i].info = mr.readString(0xc8);
            }

            mr.setIndex(item.imsg_hero_info_offset);
            for (int i = 0; i<heros.Length && (item.imsg_hero_info_num<0 || i<item.imsg_hero_info_num); i++)
            {
                heros[i].info = mr.readString(0xc8);
            }

            mr.setIndex(0x1fbd0);
            for (int i = 0; i < 26; i++)
            {
                heros[i].retreat = mr.readString(0xc8);
            }

            mr.setIndex(0x222e0);
            for (int i = 0; i < 21; i++)
            {
                int mjId = mingjiangbiao[i];
                heros[mjId].kill = mr.readString(0xc8);
            }

            mr.setIndex(0xc350);
            for (int i = 0; i < gks.Length; i++)
            {
                gks[i].gkname = mr.readString(0xc8);
            }

            return true;
        }

        public bool parseStarE5(MemoryReader mr)
        {
            GameItem item = ToolConfig.Instance.Current;

            mr.setIndex(0);

            for (int i = 0; i < item.star_e5_item_num; i++)
            {
                mr.setIndex(0x19 * i);
                goods[i + item.e5_item_num] = new DicItemAttr();
                goods[i + item.e5_item_num].good_id = (ushort)(i + item.e5_item_num);
                goods[i + item.e5_item_num].parseE5(mr);
            }
            

            return true;
        }

        public void saveJson()
        {
            saveArrayJson(heros, "dic_hero.json");
            saveArrayJson(jobs, "dic_job.json");
            saveArrayJson(goods, "dic_item.json");
            saveArrayJson(skills, "dic_skill.json");
            saveArrayJson(gks, "dic_gk.json");
            saveTypeArrayJson(jobs, DicJobAttr.TYPE_JOB_ITEM, "dic_jobItem.json");

            saveByteArrayJson(job_terrain_info, "dic_jobTerrain.json");
            saveByteArrayJson(job_walk_info, "dic_jobWalk.json");

            saveJobSkillJson(job_skill_info, "dic_jobSkill.json");
            saveJobPkJson(job_pk_info, "dic_job_pk.json");
        }

        private static void saveArrayJson(Array itemArray, string fileName)
        {
            JArray content = new JArray();
            for (int i = 0; i < itemArray.Length; i++)
            {
                BaseParse bp = (BaseParse)itemArray.GetValue(i);                
                content.Add(bp.toJsonObject());
            }

            saveJson2File(content, fileName);
        }

        private static void saveTypeArrayJson(Array itemArray, int type, string fileName)
        {
            JArray content = new JArray();
            for (int i = 0; i < itemArray.Length; i++)
            {
                BaseParse bp = (BaseParse)itemArray.GetValue(i);
                content.Add(bp.toTypeJsonObject(type));
            }

            saveJson2File(content, fileName);
        }

        private static void saveByteArrayJson(byte[][] byteArray, string fileName)
        {
            JArray content = new JArray();

            for (int i = 0; i < byteArray.Length; i++)
            {
                JObject jo = new JObject();
                jo.Add("id", i + 1);
                for (int j = 0; j < byteArray[i].Length; j++)
                {
                    jo.Add((j + 1).ToString(), byteArray[i][j]);
                }
                content.Add(jo);
            }

            saveJson2File(content, fileName);
        }

        private static void saveJobSkillJson(byte[][] byteArray, string fileName)
        {
            JArray content = new JArray();

            for (int i = 0; i < byteArray.Length; i++)
            {
                JObject jo = new JObject();
                jo.Add("jobid", i + 1);

                StringBuilder sbSkill = new StringBuilder();
                StringBuilder sbLevel = new StringBuilder();

                for (int j = 0; j < byteArray[i].Length; j++)
                {
                    if (byteArray[i][j] == 0)
                    {
                        continue;
                    }

                    if (sbSkill.Length != 0)
                    {
                        sbSkill.Append('&');
                        sbLevel.Append('&');
                    }

                    sbSkill.Append(j + 1);
                    sbLevel.Append(byteArray[i][j]);
                }

                jo.Add("skid", sbSkill.ToString());
                jo.Add("lv_up", sbLevel.ToString());

                content.Add(jo);
            }

            saveJson2File(content, fileName);
        }

        private static void saveJobPkJson(byte[][] jobPkInfo, String fileName)
        {
            JArray content = new JArray();

            for (int i = 0; i < jobPkInfo.Length; i++)
            {
                for (int j = 0; j < jobPkInfo[i].Length; j++)
                {
                    JObject jo = new JObject();
                    jo.Add("cid", i + 1);
                    jo.Add("cidpk", j + 1);
                    jo.Add("hart", jobPkInfo[i][j]);

                    content.Add(jo);
                }                
            }

            saveJson2File(content, fileName);
        }

        private static void saveJson2File(JContainer jc, String fileName)
        {
            String saveFile = Path.Combine(Application.StartupPath, "json_save/" + fileName);
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

            ExcelJson.ConvertJson2Excel(saveFile);
        }
    }
    
}
