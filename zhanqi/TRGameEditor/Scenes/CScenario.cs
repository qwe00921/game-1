using System;
using System.Collections.Generic;
using System.Text;

using TRGameEditor.Tools;
using TRGameEditor.Datas;
namespace TRGameEditor.Scenes
{
    public class ParaTypeInfo
    {
        public int NameNum;
        public int MaxNameLen;
        public int DefIndex;
        public bool IsUseCombo;
        public bool isAllowNULL;
    }
    class CScenario
    {

        public static String getNodeText(ScriptCommand cmd)
        {

             CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(cmd.id);
             String ret = String.Format("{0:x}:{1}", cmd.id, cfg.Name);
             switch (cmd.id)
             {
                 case 0x5:
                     {
                         ushort[] true_list = (ushort[])cmd.values[0];
                         ushort[] false_list = (ushort[])cmd.values[1];
                         int n = true_list.Length;
                         ret += "   ";
                         for (int i = 0; i < n; i++)
                         {
                             ret += "Var" + true_list[i];
                             ret += " ";
                         }
                         ret += "; ";
                         n = false_list.Length;
                         for (int i = 0; i < n; i++)
                         {
                             ret += "Var" + false_list[i];
                             ret += " ";
                         }
                     }
                     break;
                 case 0x46:
                 case 0x47:
                     break;
                 default:
                     {
                         for (int i = 0; i < cmd.keys.Length; i++)
                         {
                             int key = cmd.keys[i];
                             ret += (" ");
                             //EexType etype = ScriptConfig.Instance.getEexType(key);
                             switch (key)
                             {
                                 case 4:
                                     //TempStr += ReadLong_Str();
                                     ret += cmd.values[i].ToString();
                                     break;
                                 case 0x102:
                                     ret += "Var" + cmd.values[i].ToString();
                                     //tt.Format(("Var{0}"), ReadLong());
                                     //TempStr += tt;
                                     break;
                                 case 5:
                                     ret += cmd.values[i].ToString();
                                     //tt = ReadStr(true);
                                     //tt.Replace(("\r\n"), ("\\n"));
                                     //TempStr += tt;
                                     break;
                                 case 0x100:
                                     ret += cmd.values[i].ToString();
                                     //TempStr += ReadPos(true);
                                     break;
                                 case 0x101:
                                     ret += cmd.values[i].ToString();
                                     //TempStr += ReadArea(true);
                                     break;
                                 case 0x103:
                                     ret += cmd.values[i].ToString();
                                     //TempStr += GetParameterName_Int(ParameterInfo[i], ReadInt(3), Code, i - CodeInfo[Code].ParameterIndex);
                                     break;

                                 default:
                                     string aaa = cmd.values[i].ToString();
                                     ret += GetParameterName_Int(key, Convert.ToInt32(cmd.values[i]), cmd.id, 0/*i - CodeInfo[Code].ParameterIndex*/);
                                     break;
                             }
                         }

                     }
                     break;
             }
             
             return ret;
        }
        const int nullValue = 0xFFFF;
        public static string GetStrFromStrArray(string[] StrArray,int Index,int StrNum,bool IsAllowNULL = false,string NULLStr = null)
	    {
		    //string String;
            if (Index == nullValue || Index >= StrNum)
            {
			    return "无";
		    } else  {
			    return StrArray[Index];
		    }
//             else {
// 			    String.Format(_T("?:{0}"),Index);
// 			    return String;
// 		    }
	    }
        public static string GetParaName(int Para,int Code = -1,int ParaIndex = -1)
        {
	        string temp;
	        string t=null;
	        if (Para<ARRAYSIZE(ParameterName)) t=ParameterName[Para];
	        if (t==null)
	        {
		        switch (Para)
		        {
		        case 2:
			        switch (Code)
			        {
			        case 0x45:
				        if (ParaIndex==5) return ("敌方主将");
				        if (ParaIndex==7) return ("我方主将");
				        break;
			        default:
				        return ("武将");
			        }
			        break;
		        case 4:
			        switch (Code)
			        {
			        case 0x39:
				        return ("升级级数");
			        case 0x45:
				        return ("最大回合数");
			        case 0x59:
				        return ("额外获得金钱");
			        case 0x67:
				        return ("章数");
			        }
			        break;
		        case 5:
			        switch (Code)
			        {
			        case 0x67:
				        return ("章名");
			        default:
				        return ("文字");
			        }
			        //break;
		        case 0x100:
			        return ("坐标");
		        case 0x101:
			        return ("区域");
		        case 0x102:
			        return ("变量");
		        case 0x103:
			        return ("兵种");
		        }
		        temp = String.Format(("Para{0}"),Para);
		        return temp;
	        } else {
		        return t;
	        }
        }
        public static string GetParameterName_Int(int ParaCode,int t,int Code = -1,int ParaIndex = -1)
        {
	        switch (ParaCode)
	        {
	            case 2:
                case 0x38:
                case 0x39:
                    if (t == nullValue)
                    {
                        return "无";
		            }
                    else if (t < GameData.Instance.data_hero.Rows.Count)
                    {
                        return (string)GameData.Instance.data_hero.Rows[t]["name"];
			        
		            } 
                    else if(t<0x400)
                    {
                        return "无";
                    }
                    else if (t==0x400) {
			            return ("任何部队");
		            } 
                    else if (t==0x401) {
			            return ("我军或友军");
		            } 
                    else if (t==0x402) {
			            return ("敌军"); //s_18(Code25)
		            } 
                    else if (t==0x403) {
			            return ("我军当前人物");
		            }
		            break;
                case 3:
                    return GetStrFromStrArray(strPara3, t, strPara3.Length, true, strPara3[6]);
                case 7:
                    return GetStrFromStrArray(strPara7, t, strPara7.Length);
                case 9:
                    if (t == 0xff)
                    {
                        return "无";
                    }
                    else
                    {
                        return String.Format(("Track{0}"), t + 2);
                    }
	            case 0xc:
		            if (t==nullValue) {
			            return "无";
		            } else  {
                        return String.Format(("MMap-{0}"), t + 1); 
		            }
                case 0xd:
                    return GetStrFromStrArray(strParad, t, strParad.Length, true);
                case 0xe:
                    if (t == 0)
                    {
                        return ("加入");
                    }
                    else if (t == 0xff)
                    {
                        return ("离开");
                    }
                    break;
                case 0x10:
                    return GetStrFromStrArray(strPara10, t, (strPara10.Length));
                case 0x12:
                    return GetStrFromStrArray(strPara12, t, (strPara12.Length));
                case 0x13:
                    return String.Format(("PMapObj-{0}"), t + t + 1);
	            case 0x15:
		            if (t==nullValue) {
			            return "无";
		            } else{
                        return String.Format(("HM{0:D2}"), t); 
		            }
	            case 0x17:
		            if (t==nullValue) {
			            return "无";
		            } else if (t<gItemMax) {
                        return (string)GameData.Instance.data_item.Rows[t]["name"];
		            }
                    else
                    {
                        return "无"; 
                    }
	            case 0x18:
		            return GetStrFromStrArray(strPara18,t,strPara18.Length,true);
	            case 0x1a:
		            if (t==nullValue) {
			            return "无";
		            } else if (t==0) {
			            return ("中国地图");
		            }
		            break;
	            case 0x1b:
		            if (IsBetween(t,0,(strPara1b.Length)-1)) {
			            return GetStrFromStrArray(strPara1b,t,(strPara1b.Length));
		            } else {
			            if (t==nullValue) {
				            return "无";
			            } else {
				            return String.Format(("动画{0}"),t);
			            }
		            }
	            case 0x1c:
		            if (t==nullValue) {
			            return "无";
		            } else /*if (t<gMMap2Max)*/ {
			            
			            return String.Format(("MMap-{0}"),t/*+gMMap1Max*/+1);
		            }
	            case 0x1e:
		            if (t<100) {
                        return String.Format(("SE{0:D2}.WAV"), t);
		            } else if (t<200) {
                        return String.Format(("SE_M_{0:D2}.WAV"), t - 100);
		            } else {
                        return String.Format(("SE_E_{0:D2}.WAV"), t - 200);
		            }
	            case 0x22:
		            return GetStrFromStrArray(strPara22,t,ARRAYSIZE(strPara22),true,strPara22[1]);
	            case 0x23:
		            return GetStrFromStrArray(strPara23,t,ARRAYSIZE(strPara23));
	            case 0x24:
		            return GetStrFromStrArray(strPara24,t,ARRAYSIZE(strPara24));
	            case 0x26:
		            switch (Code)
		            {
		            case 0x4:
			            return (t==1)?("选是"):("选否");
		            case 0x21:
			            if (ParaIndex==2) return (t==1)?("未知1"):("未知0");
			            if (ParaIndex==3) return (t==1)?("未知1"):("未知0");
			            break;
		            case 0x2c:
			            if (ParaIndex==1) return (t==1)?("不换页"):("换页");
			            if (ParaIndex==2) return (t==1)?("换行"):("不换行");
			            if (ParaIndex==3) return (t==1)?("等待"):("不等待");
			            break;
		            case 0x2e:
			            return (t==0)?("相邻(可攻击)"):("相邻");
		            case 0x3d:
			            return (t==1)?("显示动作"):("不显示动作");
		            case 0x45:
			            if (ParaIndex==0) return (t==1)?("未知1"):("未知0");
			            if (ParaIndex==1) return (t==1)?("未知1"):("未知0");
			            break;
		            case 0x46:
			            return (t==1)?("隐藏"):("不隐藏");
		            case 0x47:
			            if (ParaIndex==1) return (t==1)?("援军"):("敌军");
			            if (ParaIndex==2) return (t==1)?("隐藏"):("不隐藏");
			            break;
		            case 0x4b:
			            return (t==1)?("隐藏"):("不隐藏");
		            case 0x4f:
			            if (ParaIndex==3) return (t==1)?("不转向"):("转向");		
			            if (ParaIndex==4) return (t==1)?("动作前延迟"):("无延迟");
			            if (ParaIndex==5) return (t==1)?("动作后延迟"):("无延迟");
			            break;
		            case 0x50:
			            if (ParaIndex==2) return (t==1)?("动作前延迟"):("无延迟");
			            if (ParaIndex==3) return (t==1)?("动作后延迟"):("无延迟");
			            break;
		            case 0x53:
			            return (t==1)?("死亡"):("撤退");
		            case 0x58:
			            if (ParaIndex==4) return (t==1)?("未知1"):("未知0");
			            if (ParaIndex==5) return (t==1)?("未知1"):("未知0");
			            break;
		            case 0x59:
			            return (t==1)?("结局"):("平时");
		            case 0x5b:
			            return (t==1)?("战斗中"):("胜利条件");
		            case 0x62:
			            return (t==1)?("敌方武将"):("我方武将");
		            case 0x60:case 0x63:case 0x64:case 0x65:case 0x66:
			            if (ParaIndex==0) {
				            return (t==1)?("我方武将"):("敌方武将");
			            } else {
				            if (Code==0x63) return (t==1)?("延时"):("不延时");
				            if (Code==0x65) return (t==1)?("致命一击"):("普通攻击");
				            if (Code==0x66) return (t!=1)?("命中"):("未命中");
			            }
			            break;
		            case 0x6b:
			            return (t==1)?("未知1"):("未知0");
                    case 0x6d:
                        return (t == 1) ? ("复活") : ("移动");
		            default:
			            return ("未知");
		            }
		            break;
	            case 0x27:
		            return GetStrFromStrArray(strPara27,t,ARRAYSIZE(strPara27),true,strPara27[1]);
	            case 0x28:
		            return GetStrFromStrArray(strPara28,t,ARRAYSIZE(strPara28));
	            case 0x2b:
		            return GetStrFromStrArray(strPara2b,t,ARRAYSIZE(strPara2b),true,("默认方向"));
	            case 0x2c:
		            return GetStrFromStrArray(strPara2c,t,ARRAYSIZE(strPara2c),true,strPara2c[0]);
	            case 0x2d:
		            return GetStrFromStrArray(strPara2d,t,ARRAYSIZE(strPara2d));
	            case 0x2e:
		            return GetStrFromStrArray(strPara2e,t,ARRAYSIZE(strPara2e));
	            case 0x2f:
		            return GetStrFromStrArray(strPara2f,t,ARRAYSIZE(strPara2f),true);
	            case 0x30:
		            if (t==nullValue) {
			            return "无";
		            } else if (IsBetween(t,2,255)) {
			            String aaa="";
			            if ((t & 0x2)!=0) aaa+=("麻痹+");
                        if ((t & 0x4) != 0) aaa += ("封咒+");
                        if ((t & 0x8) != 0) aaa += ("混乱+");
                        if ((t & 0x10) != 0) aaa += ("中毒+");
                        if ((t & 0x80) != 0)
                        {
				            if (aaa=="") aaa=("+");
				            aaa=("取消")+aaa;
			            }
			            //aaa=aaa.Left(aaa.GetLength()-1);
			            return aaa;
		            }
		            break;
	            case 0x34:
		            return GetStrFromStrArray(strPara34,t,ARRAYSIZE(strPara34));
	            case 0x37:
		            if (t>=gSceMax+gSceMax) {
			            return "无";
		            } else 
                    if (t%2!=0) {
                        return String.Format(("R_{0:D2}.eex"), (t >> 1) + 1);
		            } else {
                        return String.Format(("S_{0:D2}.eex"), t >> 1);

		            }
	            case 0x3a:
		            return GetStrFromStrArray(strPara3a,t,ARRAYSIZE(strPara3a));
	            case 0x3b:
		            if (t==nullValue || t==0) {
			            return ("默认装备");
		            } else if (t==1) {
			            return ("卸去装备");
		            } else if (t<gItemMax && t>=2) {
			            return (string)GameData.Instance.data_item.Rows[t+gItemWeaponID-2]["name"];
		            }
		            break;
	            case 0x3c:
		            if (t==nullValue || t==0) {
			            return ("默认装备");
		            } else if (t==1) {
			            return ("卸去装备");
		            } else if (t<gItemMax && t>=2) {
                        return (string)GameData.Instance.data_item.Rows[t + gItemDefenseID - 2]["name"];
		            }
		            break;
	            case 0x3d:
		            if (t==nullValue || t==0) {
			            return ("默认装备");
		            } else if (t==1) {
			            return ("卸去装备");
		            } else if (t<gItemMax && t>=2) {
                        return (string)GameData.Instance.data_item.Rows[t + gItemAssistID - 2]["name"];
		            }
		            break;
	            case 0x3e:
		            if (t==nullValue || t==0) {
			            return ("默认等级");
		            } else if (t>gCharLvMax) {
			            return String.Format(("-{0}级"),t-gCharLvMax);
		            } else {
                        return String.Format(("+{0}级"), t);
		            }

	            case 0x3f:
		            return GetStrFromStrArray(strPara3f,t,ARRAYSIZE(strPara3f));
	            case 0x40:
		            return GetStrFromStrArray(strPara40,t,ARRAYSIZE(strPara40),true,strPara40[0]);
	            case 0x41:
		            return GetStrFromStrArray(strPara41,t,ARRAYSIZE(strPara41),true,strPara41[2]);
	            case 0x42:
		            if (IsBetween(t,0,3)) {
			            return GetStrFromStrArray(strPara42,t,ARRAYSIZE(strPara42));
		            } else {
                        return String.Format(("Gate-{0}"), 2 * (t - 4) + 1); ;
		            }
	            case 0x43:
		            return GetStrFromStrArray(strPara43,t,ARRAYSIZE(strPara43));
	            case 0x44:
		            return GetStrFromStrArray(strPara44,t,ARRAYSIZE(strPara44));
	            case 0x45:
		            return GetStrFromStrArray(strPara45,t,ARRAYSIZE(strPara45),true,strPara45[2]);
	            case 0x46:
		            return GetStrFromStrArray(strPara46,t,ARRAYSIZE(strPara46),true);
	            case 0x47:
		            return GetStrFromStrArray(strPara47,t,ARRAYSIZE(strPara47),true,strPara47[0]);
	            case 0x48:
		            return GetStrFromStrArray(strPara48,t,ARRAYSIZE(strPara48));
	            case 0x49:
		            if (t==nullValue || t==0) {
			            return ("DefLv");
		            } else {
                        return String.Format(("Lv{0}"), t);
		            }
	            case 0x4a:
		            switch (t)
		            {
		            case 0:
			            return ("曹操-普通");
		            case 1:
			            return ("曹操-惊讶");
		            case 2:
			            return ("曹操-愤怒");
		            case 3:
			            return ("曹操-欣喜");
		            case 4:
			            return ("夏侯惇-蒙目");
		            case 0x10:
			            return ("孔明-邪恶");
		            case 0x20:
			            return ("曹丕-称帝");
		            case 0x80:
			            return ("夏侯惇-独目");
		            case 0xff:
			            return ("孔明-正常");
		            }
		            break;
	            case 0x4b:
		            if (t==nullValue) {
			            return "未知111";
		            } else if (t<gMEffMax) {
			            return String.Format(("MEff-{0}"),t+1);
		            } else if (IsBetween(t,100,100+gMCallMax-1)) { //100~108
                        return String.Format(("MCall{0:D2}.e5"), t - 100);
		            } else if (t==100+gMCallMax) {
			            return ("未知");
		            }
		            break;
	            case 0x4c:
		            return GetStrFromStrArray(strPara4c,t,ARRAYSIZE(strPara4c));
	            case 0x4d:
		            return GetStrFromStrArray(strPara4d,t,ARRAYSIZE(strPara4d));
	            case 0x4e:
		            return GetStrFromStrArray(strPara4e,t,ARRAYSIZE(strPara4e));
// 	            case 0x100:
// 		            {
// 			            string r=GetCodeName(t);
// 			            if (!r) r=null;
// 			            return r;
// 		            }
// 	            case 0x103:
// 		            return GetStrFromStrArray(TroopName,t,gTroopNum);
	        }
	        //String.Format(("Para{0}-{0}"),ParaCode,t);
	        //return String;
            return "未知222";
        }
        static string[] ParameterName=
	    {
		    null,null,null,("战斗方"), //0
		    null,null,null,("AI"),
		    null,("音轨"),null,null,
		    ("外场景"),("动作"),("加入状态"),null,
		    null,null,("结局"),("地图形象"), //0x10
		    null,("战场地图"),null,("物品"),
		    ("变化"),null,("中国地图"),("动画"),
		    ("内场景"),null,("音效"),null,
		    null,null,("起始天气"),("属性"), //0x20
		    ("关系"),null,null,("赋值"),
		    null,null,null,("朝向"),
		    ("范围"),("场景类型"),("TF"),("属性"),
		    ("状态"),null,("无用"),null, //0x30 //31,33无
		    ("运算"),null,null,("剧本"), //35无
		    ("强制出场"),("强制不出场"),("关系"),("武器"),
		    ("护具"),("辅助"),("等级加成"),("区域"),
		    ("CharOrBF"),("武将"),("物体"),("是否显示"), //0x40
		    ("地形"),("兵种级别限制"),("战场动作"),("天气类别"),
		    ("回合阶段"),("等级"),("头像状态"),("法术"),
		    ("单挑动作"),("单挑攻击1"),("单挑攻击2"),
		    null,
	    };

	    //Para:2(400~403),26(Code21,45,58,6b),30(&80)(Code4d),4b(109)(Code6b)
	    //未知：strParad(Code30),strPara42(Code58)
	    static string[] strPara3={
		    ("我军"),("友军"),("敌军"),("援军"),
		    ("我军及友军"),("敌军及援军"),("所有部队")
	    };
	    static string[] strPara7={
		    ("被动出击"),("主动出击"),("坚守原地"),("攻击武将"),
		    ("到指定点"),("跟随武将"),("逃至指定点")
	    };
	    static string[] strParad={
		    ("普通"),("下跪"),("脸红"),("举手"),("哭"),
		    ("伸手"),("作揖"),("盘坐脸红"),("盘坐举手"),("盘坐哭"),
		    ("倒下"),("单膝跪地"),("被缚"),("挥剑扬起"),("挥剑劈下"),
		    ("活埋"),("起身"),("单手举起"),("未知")
	    };
	    static string[] strPara10={
		    ("放火"),("恢复")
	    };
	    static string[] strPara12={
		    ("曹操统一"),("曹丕称帝"),("魔王孔明")
	    };
	    static string[] strPara18={
		    ("下降"),("正常"),("上升")
	    };
	    static string[] strPara1b={
		    ("LOGO.AVI"),("OPEN.AVI"),("END.AVI"),("PRESS.AVI")
	    };
	    static string[] strPara22={
		    ("晴"),("晴/晴/阴/晴/阴"),("晴/晴/雨/阴/雪"),("阴/晴/雨/阴/雪"),("雨/阴/豪雨/雪/雪"),("豪雨/雨/豪雨/雪/雪")
	    };
        static string[] strPara23 ={
		    ("Att"),("Def"),("Spr"),("Cri"),("Mor"),("HP"),("MP"),("HPCur"),("MPCur"),("Lv"),
		    ("武力"),("统率"),("智力"),("敏捷"),("运气"),("头像")
	    };
        static string[] strPara24 ={
		    (">="),("<"),("=")
	    };
        static string[] strPara27 ={
		    ("false"),("true")
	    };
        static string[] strPara28 ={
		    ("钱"),("SceIndex"),("野心")
	    };
        static string[] strPara2b ={
		    ("朝北"),("朝东"),("朝南"),("朝西")
	    };
        static string[] strPara2c ={
		    ("单人"),("区域")
	    };
        static string[] strPara2d ={
		    ("外场景"),("中国地图"),("内场景"),("战场地图")
	    };
        static string[] strPara2e ={
		    ("false"),("true")
	    };
        static string[] strPara2f ={
		    ("Att"),("Def"),("Spr"),("Cri"),("Mor"),("Speed")
	    };
        static string[] strPara34 ={
		    ("="),("+"),("-")
	    };
        static string[] strPara3a ={
		    ("!="),("=")
	    };
        static string[] strPara3f ={
		    ("整个战场"),("指定区域")
	    };
        static string[] strPara40 ={
		    ("CharIndex"),("BFIndex")
	    };
        static string[] strPara41 ={
		    ("CharIndex"),("BFIndex"),("区域")
	    };
        static string[] strPara42 ={
		    ("火"),("船"),("起火船"),("未知")
	    };
        static string[] strPara43 ={
		    ("显示"),("消失")
	    };
        static string[] strPara44 ={
		    ("平原"),("草原"),("树林"),("荒地"),("山地"),
		    ("岩山"),("山崖"),("雪原"),("桥梁"),("浅滩"),
		    ("沼泽"),("池塘"),("小河"),("大河"),("栅栏"),
		    ("城墙"),("城内"),("城门"),("城池"),("关隘"),
		    ("鹿砦"),("树木"),("兵营"),("民居"),("宝物库"),
		    ("水池"),("火"),("船"),("祭坛"),("地下")
	    };
        static string[] strPara45 ={
		    ("初级"),("中级"),("高级")
	    };
        static string[] strPara46 ={
		    null,("举起武器"),("防御"),("受攻击"),("虚弱"),
		    ("攻击预备"),("攻击"),("二次攻击"),("慢速转圈"),("喘气"),
		    ("晕倒"),("快速转圈"),("中速转圈")
	    };
        static string[] strPara47 ={
		    ("普通"),("晴好"),("阴雨"),("小雪"),("大雪")
	    };
        static string[] strPara48 ={
		    //("敌方回合"),("我方回合"),("回合结束")
		    ("我军阶段"),("友军阶段"),("敌军阶段")
		    //("我军行动前"),("友军行动前"),("敌军行动前")
	    };
	    static string[] strPara4c={
		    null,("后转"),("前移"),("小步前移"),
		    ("小步后退"),("举起武器"),("防御"),("受攻击"),
		    ("攻击预备"),("攻击"),("二次攻击"),("晕倒"),
		    ("喘气"),("撤退"),("跳舞1"),("跳舞2")
	    };
	    static string[] strPara4d={
		    ("命中"),("格挡"),("格档后退"),("后退"),("闪躲绕前")
	    };
        static string[] strPara4e ={
		    ("原地攻击"),("移动攻击"),("互相冲锋")
	    };
        static bool IsBetween(int Value,int LBound,int UBound) { 
		    return (Value>=LBound && Value<=UBound);
        }
        static int ARRAYSIZE(string[] aa)
        {
            return aa.Length;
        }
        
	    static int gCharMax=0x200;
	    //static int gMagicMax=73; //~256
	    static int gItemMax=104; //~256
	    static int gTroopNum=53;
//	    static int gTroopNameAddr=0x8a8a8+0x401600;

	    static int gItemWeaponID=0;
	    static int gItemDefenseID=38;
	    static int gItemAssistID=53;
	    static int gItemOtherID=104; //~256

        static int gSceMax=58; //<100
        static int gAVIMax=4; //~128
        static int gTrackMax=21; //<256 Para9
        static int gSEMax=43; //~100
        static int gSEMMax=37; //~100
        static int gSEEMax=9; //~100
        static int gMEffMax=58; //<=100
        static int gMMap1Max=40;
        static int gMMap2Max=74;
        static int gCharLvMax=50; //50~127
        static int gItemLvMax=9; //~16
        static int gPMapObjMax=206;
        static int gMCallMax=9; //~100
        static int gGateMax=97;
        public static bool[] gIsShowChar = new bool[0x400]; //for MFC only
        static public int GetParaTypeNameIndex(int ParaCode,int TypeIndex)
        {
	        ParaTypeInfo TypeInfo = GetParaTypeInfo(ParaCode);
	        if (TypeInfo.isAllowNULL && TypeInfo.DefIndex==0x10000 && TypeIndex==TypeInfo.NameNum-1) return nullValue;
	        switch (ParaCode)
	        {
	        case 2:
		        if (TypeIndex>=gCharMax) TypeIndex+=0x400-gCharMax;
		        break;
	        case 9:
		        if (TypeIndex==gTrackMax) TypeIndex=0xff;
		        break;
	        case 0xe:
		        if (TypeIndex==1) TypeIndex=0xff;
		        break;
	        case 0x1e:
		        if (TypeIndex>=gSEMax+gSEMMax) {
			        TypeIndex+=200-gSEMax-gSEMMax;
		        } else if (TypeIndex>=gSEMax) {
			        TypeIndex+=100-gSEMax;
		        }
		        break;
	        case 0x30:
		        {
			        int t=0;
			        if (TypeIndex>=15) ++TypeIndex;
			        if (((TypeIndex+1) & 1)!=0) t|=2;
			        if (((TypeIndex+1) & 2)!=0) t|=4;
			        if (((TypeIndex+1) & 4)!=0) t|=8;
			        if (((TypeIndex+1) & 8)!=0) t|=0x10;
                    if (((TypeIndex + 1) & 16) != 0) t |= 0x80;
			        TypeIndex=t;
			        break;
		        }
	        case 0x4a:
		        if (TypeIndex==8) {
			        TypeIndex=0xff;
		        } else if (TypeIndex==7) {
			        TypeIndex=0x80;
		        } else if (TypeIndex==6) {
			        TypeIndex=0x20;
		        } else if (TypeIndex==5) {
			        TypeIndex=0x10;
		        }
		        break;
	        case 0x4b:
		        if (TypeIndex>=gMEffMax) TypeIndex+=100-gMEffMax;
		        break;
	        }
	        return TypeIndex;
        }
//        static int gLogoMax=13; //~0x400
        
        public static ParaTypeInfo GetParaTypeInfo(int ParaCode)
        {
            ParaTypeInfo TypeInfo = new ParaTypeInfo();
            TypeInfo.IsUseCombo = true;
            TypeInfo.isAllowNULL = false;
            TypeInfo.MaxNameLen = 10;
            TypeInfo.DefIndex = 0x10000;
            switch (ParaCode)
            {
                case 2:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 12 + 5;
                    TypeInfo.NameNum = gCharMax + 4 + 1;
                    break;
                case 3:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 6;
                    TypeInfo.MaxNameLen = 10;
                    TypeInfo.NameNum = ARRAYSIZE(strPara3) + 1;
                    break;
                case 7:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 1;
                    TypeInfo.MaxNameLen = 10;
                    TypeInfo.NameNum = ARRAYSIZE(strPara7) + 1;
                    break;
                case 9:
                    TypeInfo.MaxNameLen = 7;
                    TypeInfo.NameNum = gTrackMax + 1;
                    break;
                case 0xc:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 7;
                    TypeInfo.NameNum = gMMap1Max + 1;
                    break;
                case 0xd:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strParad) + 1;
                    break;
                case 0xe:
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = 2;
                    break;
                case 0x10:
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = ARRAYSIZE(strPara10);
                    break;
                case 0x12:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara12);
                    break;
                case 0x13:
                    TypeInfo.MaxNameLen = 11;
                    TypeInfo.NameNum = gPMapObjMax;
                    break;
                case 0x15:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = gSceMax + 1;
                    break;
                case 0x17:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 16;
                    TypeInfo.NameNum = gItemMax + 1;
                    break;
                case 0x18:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = ARRAYSIZE(strPara18) + 1;
                    break;
                case 0x1a:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = 1 + 1;
                    break;
                case 0x1b:
                    TypeInfo.MaxNameLen = 9;
                    TypeInfo.NameNum = gAVIMax;
                    break;
                case 0x1c:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = gMMap2Max + 1;
                    break;
                case 0x1e:
                    TypeInfo.MaxNameLen = 11;
                    TypeInfo.NameNum = gSEMax + gSEMMax + gSEEMax;
                    break;
                case 0x22:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 1;
                    TypeInfo.MaxNameLen = 18;
                    TypeInfo.NameNum = ARRAYSIZE(strPara22) + 1;
                    break;
                case 0x23:
                    TypeInfo.MaxNameLen = 5;
                    TypeInfo.NameNum = ARRAYSIZE(strPara23);
                    break;
                case 0x24:
                    TypeInfo.MaxNameLen = 2;
                    TypeInfo.NameNum = ARRAYSIZE(strPara24);
                    break;
                case 0x27:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 1;
                    TypeInfo.MaxNameLen = 6;
                    TypeInfo.NameNum = ARRAYSIZE(strPara27) + 1;
                    break;
                case 0x28:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara28);
                    break;
                case 0x2b:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara2b) + 1;
                    break;
                case 0x2c:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = ARRAYSIZE(strPara2c) + 1;
                    break;
                case 0x2d:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara2d);
                    break;
                case 0x2e:
                    TypeInfo.MaxNameLen = 5;
                    TypeInfo.NameNum = ARRAYSIZE(strPara2e);
                    break;
                case 0x2f:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 5;
                    TypeInfo.NameNum = ARRAYSIZE(strPara2f) + 1;
                    break;
                case 0x30:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 24;
                    TypeInfo.NameNum = 30 + 1;
                    break;
                case 0x34:
                    TypeInfo.MaxNameLen = 1;
                    TypeInfo.NameNum = ARRAYSIZE(strPara34);
                    break;
                case 0x37:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = gSceMax + gSceMax;
                    break;
                case 0x38:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 12;
                    TypeInfo.NameNum = gCharMax + 1;
                    break;
                case 0x39:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 12;
                    TypeInfo.NameNum = gCharMax + 1;
                    break;
                case 0x3a:
                    TypeInfo.MaxNameLen = 2;
                    TypeInfo.NameNum = ARRAYSIZE(strPara3a);
                    break;
                case 0x3b:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 16;
                    TypeInfo.NameNum = gItemDefenseID - gItemWeaponID + 3;
                    break;
                case 0x3c:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 16;
                    TypeInfo.NameNum = gItemAssistID - gItemDefenseID + 3;
                    break;
                case 0x3d:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 16;
                    TypeInfo.NameNum = gItemOtherID - gItemAssistID + 3;
                    break;
                case 0x3e:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = gCharLvMax - 0 + 1 + gCharLvMax + 1;
                    break;
                case 0x3f:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara3f);
                    break;
                case 0x40:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 9;
                    TypeInfo.NameNum = ARRAYSIZE(strPara40) + 1;
                    break;
                case 0x41:
                    TypeInfo.DefIndex = 2;
                    TypeInfo.MaxNameLen = 9;
                    TypeInfo.NameNum = ARRAYSIZE(strPara41);
                    break;
                case 0x42:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = gGateMax;
                    break;
                case 0x43:
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = ARRAYSIZE(strPara43);
                    break;
                case 0x44:
                    TypeInfo.MaxNameLen = 6;
                    TypeInfo.NameNum = ARRAYSIZE(strPara44);
                    break;
                case 0x45:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 2;
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = ARRAYSIZE(strPara45) + 1;
                    break;
                case 0x46:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara46) + 1;
                    break;
                case 0x47:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 4;
                    TypeInfo.NameNum = ARRAYSIZE(strPara47) + 1;
                    break;
                case 0x48:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara48);
                    break;
                case 0x49:
                    TypeInfo.isAllowNULL = true;
                    TypeInfo.DefIndex = 0;
                    TypeInfo.MaxNameLen = 5;
                    TypeInfo.NameNum = 1 + gItemLvMax + 1;
                    break;
                case 0x4a:
                    TypeInfo.MaxNameLen = 11;
                    TypeInfo.NameNum = 9;
                    break;
                case 0x4b:
                    TypeInfo.MaxNameLen = 10;
                    TypeInfo.NameNum = gMEffMax + gMCallMax + 1;
                    break;
                case 0x4c:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara4c);
                    break;
                case 0x4d:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara4d);
                    break;
                case 0x4e:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = ARRAYSIZE(strPara4e);
                    break;
                case 0x100:
                    TypeInfo.MaxNameLen = 25;
                    TypeInfo.NameNum = (int)CommandId.CMD_MAX;
                    break;
                case 0x103:
                    TypeInfo.MaxNameLen = 8;
                    TypeInfo.NameNum = gTroopNum;
                    break;
                default:
                    TypeInfo.IsUseCombo = false;
                    break;
            }
            if (TypeInfo.isAllowNULL && TypeInfo.DefIndex != 0x10000) --TypeInfo.NameNum;
            return TypeInfo;
        }
        public static string GetCodeName(int Code)
        {
            
            if (Code<(int)CommandId.CMD_MAX)
            {
                CommandConfig cfg = ScriptConfig.Instance.getCommandConfig(Code);
                return cfg.Name;
            }
            else if (Code == 0x200)
            {
                return ("Section");
            }
            else if (Code == 0x100)
            {
                return ("Scene");
            }
            else
            {
                return null;
            }
        }
    }

}
