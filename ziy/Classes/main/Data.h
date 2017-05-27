#ifndef _DATAHANDLER_H_
#define _DATAHANDLER_H_

#include "TrPayInfo.h"
#include "TrAccountBuy.h"
#include "GoodsItem.h"

using namespace std;

#define MAX_NAME_LEN 12
#define MAX_ACTOR_NAME_LEN 20
#define MAX_OCCUPATION_NAME_LEN 20
#define MAX_LANDFORM_NAME_LEN 12
#define MAX_SKILL_NAME_LEN 20
#define MAX_GOOD_TYPE_NAME_LEN 24
#define MAX_INFO_LEN 400
#define MAX_SKILL_EFFECT_LEN 40
#define MAX_SKILL_NUM modConst->NEW_MAX_SKILL_NUM
#define MAX_LL_NAME_LEN 40
#define MAX_MEFF_FRAME 40
#define MAX_GK_NAME_LEN 50

#define MAX_APP_NAME_LEN 100
#define MAX_APP_DESCRIBE_LEN 400
#define MAX_APP_URL_LEN 200

#define MAX_GAME_MENU_NUM 10
#define MAX_GAME_MENU_LEN 40
#define MAX_SEID_NUM 5000
#define MAX_CHAR_VAR_NUM 9999
#define MAX_INT_VAR_NUM 9999
#define MAX_SKILL_MIN_NUM 4
#define MAX_FORCE_LIMIT 16

enum
{
	AIR_TYPE_SUN = 1,//晴天
	AIR_TYPE_CLOUDY,//阴天
	AIR_TYPE_RAIN,//雨天
	AIR_TYPE_WIND,//风天
	AIR_TYPE_SNOW//雪天
};

typedef enum SystemOpenIDS
{
	E_SixCountries = 1,//游历天下
	E_Romance = 2,//演义堂
	E_GoodsShop = 3,//商店
	E_DailyScene = 4,//日常副本
	E_WorldRank = 5,//排行榜
	E_BattleLadder = 6,//过关斩将
	E_ItemMerge = 7,//道具合成
	E_Guild = 8,//氏族公会
	E_ZZCC = 9,//众志成城
	E_GoodsStore = 10,//货郎
	E_PubDrink=11,//酒馆
	E_ZhuMinZhanYi=12,//著名战役
	E_YOUJIAN=13,//邮件
	E_WUJIANG=14,//武将
	E_BEIBAO=15,//背包
	E_DUANZAO=16,//锻造
	E_QIANDAO=17,//签到
	E_HUODONG=18,//活动
	E_CHONGZHI=19,//充值
	E_GOUYU=20,//勾玉购买
	E_WANJIAXINXI=21,//玩家信息
	E_YOULIPUTONG=101,//游历普通
	E_YOULIJINGYING=102,//游历精英
	E_YOULIYINGXIONGZHI=103,//游历英雄志
	E_GAOLANGCHENG=401,//高狼林
	E_XINMOTA=402,//心魔塔
	E_YINGXIONGHUI=403,//英雄会
	E_SHUOGUOLEILIE=404,//硕果累累
	E_FUJIATIANXIA=405,//富甲天下
	E_GUOGUANZHANJIANG=601,//过关斩将
	E_YANWUCHANG=602,//演武场
	E_HECHENGXIAOHAO=701,//合成消耗品
	E_HECHENGSUIPIAN=702,//合成碎片
	E_HECHENGSHENQI=703,//合成神器
	E_HECHENGBAOWU,//合成宝物
	E_HECHENGFUMOSHI,//合成附魔石
	E_HECHENGLIANGYATAO,//合成狼牙套
	E_HECHENGHUWEITAO,//合成虎威套
	E_SHIZUHUODONG=801,//氏族活动
	E_SHIZIFUBEN=802,//氏族团队副本
	E_jiuguanjinbi=1101,//酒馆金币
	E_jiuguangouyu=1102,//酒馆勾玉
	E_1401=1401,//武将装备
	E_1402,//武将技能
	E_1403,//武将修炼
	E_1404,//武将时装
	E_1405,//武将进阶
	E_1601,//锻造强化
	E_1602,//锻造重铸
	E_1603,//锻造附魔



} SystemOpenIDS;

//////////////////////////////////////////////////////////////
//角色属性 <==> dic_hero.json
class ActorAttr
{
public:
	ActorAttr()
	{
		actor_id = 0;			//武将ID
		face_id = 0;			//对话时头像
		image_id = 0;			//形象
		image_fight_id = 0;
		occupation_id = 0;		//职业
		power = 0;				//武力
		defence = 0;			//防御力
		intellegence = 0;		//智力
		explosion = 0;			//敏捷（命中率、格档率、连击率）
		fortune = 0;			//士气（爆发率）
		level = 0;
		hp = 0;
		mp = 0;
		seid = NULL;			//人物特技效果数组
		seid_num = 0;			//人物特技效果数量
		tujian_id = 0;			//图鉴id
		level_max = 0;          //等级上限
		up_ex = 0;				//升级经验
		skill = 0;				//绝技
		actor_quality = 1;		//武将品质
		actor_grade = 1;		//武将品阶
		qid = 1;
		job_img = 0;
		iGender = 0;
		iHeroGrade = 0;

		memset(actor_name, 0, sizeof(actor_name));
		memset(actorname_nonum, 0, sizeof(actorname_nonum));
		memset(actor_info, 0, sizeof(actor_info));
		memset(rouse_word, 0, sizeof(rouse_word));
		memset(retreat_word, 0, sizeof(retreat_word));
	}
	char* getActorName();
public:
	unsigned short actor_id;				//武将ID<==>hid
	unsigned short face_id;					//对话时头像<==>face_id
	unsigned short image_id;				//形象
	unsigned short image_fight_id;
	unsigned short occupation_id;			//职业<==>jobid  
	unsigned short power;					//武力<==>atk
	unsigned short defence;					//防御力<==>def
	unsigned short intellegence;			//智力<==>ints
	unsigned short explosion;				//敏捷（命中率、格档率、连击率）<==>burst
	unsigned short fortune;					//士气（爆发率）<==>fortune
	unsigned short level;					//等级<==>level	
	unsigned short hp;						//<==>hp
	unsigned short mp;						//<==>mp
	unsigned short* seid;					//人物特技效果数组<==>seid
	unsigned short seid_num;				//人物特技效果数量
	unsigned short tujian_id;				//图鉴id
	unsigned short level_max;				//等级上限
	unsigned short up_ex;					//升级经验
	unsigned short skill;					//主动技能(net_hero_skill)<==>skill
	unsigned short qid;						//成长<==>qid
	unsigned short actor_quality;           //武将品质<==>quality
	unsigned short actor_grade;		 	    //武将品阶<==>border_color
	unsigned short job_img;			        //职业图标<==>job_img
	char actor_name[MAX_ACTOR_NAME_LEN];    //姓名<==>name
	char actor_info[MAX_INFO_LEN];		    //介绍<==>info
	char rouse_word[MAX_INFO_LEN];		    //奋力一击台词<==>kill
	char retreat_word[MAX_INFO_LEN];	    //撤退台词<==>retreat
	int iGender;							//性别
	int iHeroGrade;							//兵种类别
	int iRecruit;                           //是否可出现在我军
private:
	char actorname_nonum[MAX_ACTOR_NAME_LEN];
};
typedef ActorAttr* ActorAttrPtr;

///////////////////////////////////////////////////////////
//兵种类型 <==> dic_job_class.json
class OccupationType
{
public:
	OccupationType()
	{
		occupation_type_id = 0;
		cval = 0;
		occupation_start = 0;
		memset(occupation_type_name, 0, sizeof(occupation_type_name));
		atk_music = 0;
		job_type = 0;
	}
public:
	unsigned short occupation_type_id;	//<==>cid
	unsigned short cval;				//<==>cval
	unsigned short occupation_start;	//<==>jobid
	char occupation_type_name[MAX_OCCUPATION_NAME_LEN];//<==>name
	short atk_music;					// 攻击声音<==>atk_music
	unsigned short job_type;			//兵种种类<==>job_type
};

///////////////////////////////////////////////////////////
//兵种属性 <==> dic_job.json
class OccupationAttr
{
public:
	OccupationAttr()
	{
		occupation_id = 0;
		occupation_level = 0;
		attack_type_id = 0;
		attack_special_effect = 0;
		seid = NULL;
		seid_num = 0;
		image_fight_id = 0;
		moveId = 0;
		musicId = 0;
		base_speed = 0;
		memset(occupation_name, 0, sizeof(occupation_name));
		memset(occupation_info, 0, sizeof(occupation_info));
		changejob_imgid = 0;
		effect = "";
		is_robot = 0;
	}
public:
	unsigned short occupation_id;					//职业ID<==>jobid
	unsigned short occupation_level;
	unsigned short attack_type_id;					//<==>atkid
	unsigned short attack_special_effect;			// 该值对应原json中‘seid’，新json该值变为字符串
	unsigned short* seid;
	unsigned short seid_num;
	unsigned short image_fight_id;
	unsigned short moveId;							//<==>move
	unsigned short musicId;							//<==>music
	unsigned short base_speed;						//<==>spe
	char occupation_name[MAX_OCCUPATION_NAME_LEN];	//<==>name
	char occupation_info[MAX_INFO_LEN];				//<==>info	
	unsigned short changejob_imgid;					//<==>changejob_imgid
	string effect;									//<==>effect
	unsigned short is_robot;                        //特殊兵种，不参与胜利结算
};
typedef OccupationAttr* OccupationAttrPtr;

////////////////////////////////////////////////////////
//半身像截取小头像坐标<==>dic_h_img.json
class BustHImg
{
public:
	BustHImg()
	{
		face_id = -1;
		src_x = 0;
		src_y = 0;
	}
public:
	int face_id;
	int src_x;
	int src_y;
};
typedef BustHImg* BustHImgPtr;

////////////////////////////////////////////////////////
//特殊效果
class SpecialEffe
{
public:
	SpecialEffe()
	{
		seid = 0;
		memset(seLL, 0, sizeof(seLL));
		memset(sename, 0, sizeof(sename));
	}
public:
	unsigned short seid;
	char seLL[MAX_LL_NAME_LEN];
	char sename[MAX_LL_NAME_LEN];
};

////////////////////////////////////////////////////////
//装备普通效果<==>dic_itemType.json
class ItemType
{
public:
	ItemType()
	{
		itid = 0;
		typeId = 0;
		itfun = 0;
		itpos = 0;
		memset(itname, 0, sizeof(itname));
	}
public:
	unsigned short itid;			//唯一ID<==>itid
	unsigned short typeId;			//0 消耗 1 武器 2 法器 3 铠甲 4 衣服 5 辅助<==>typeid	
	unsigned short itfun;			// 附带属性 1攻击力 2精神力 3攻击力和精神力 4防御力 <==>itfun
	unsigned short itpos;			// 0 辅助 1 武器 2 防具 3 消耗 4 碎片（装备/武将）	<==>itpos
	char itname[MAX_LL_NAME_LEN];	//物品类别的名称 弓 剑 棍等j<==>itname 
};

////////////////////////////////////////////////////////
//宝物类型<==>dic_item_class.json
class ItemClass
{
public:
	ItemClass()
	{
		id = 0;
		memset(name, 0, sizeof(name));
	}
public:
	int id;
	char name[MAX_LL_NAME_LEN]; //1 武器 2 法器 3 铠甲 4 衣服 5 辅助
};

////////////////////////////////////////////////////////
//宝物信息<==>dic_good_info.json
class TreasureInfo
{
public:
	TreasureInfo()
	{
		iGoodId = 0;
		memset(name, 0, sizeof(name));
		memset(method, 0, sizeof(method));
		memset(condition, 0, sizeof(condition));
	}
public:
	int iGoodId;					//物品ID<==>good_id
	char name[MAX_LL_NAME_LEN];		//名称<==>good_name
	char method[MAX_INFO_LEN];		//获取方式（显示）<==>obtain_method
	char condition[MAX_INFO_LEN];	//达到条件（显示）<==>special_condition
};

////////////////////////////////////////////////////////
//套装效果<==>dic_suiteffect.json
class SuitEffect
{
public:
	SuitEffect()
	{
		seid = 0;
		seid_num = 0;
		effect_seid = NULL;
		effect_seid_num = 0;
	}
public:
	int seid;//触发套装的seid
	int seid_num;
	int* effect_seid;//套装实际效果seid
	int effect_seid_num;
};
typedef SuitEffect* SuitEffectPtr;

////////////////////////////////////////////////////
//地图单元属性<==>map_terrain.json
class LandformAttr
{
public:
	LandformAttr()
	{
		landform_id = 0;
		can_move = 0;
		skill_attr = 0;
		memset(landform_name, 0, sizeof(landform_name));

		music = 0;

		type = 0;
		seid = NULL;
		seid_num = 0;
	}
public:
	unsigned short landform_id;
	unsigned short can_move;
	unsigned short skill_attr;
	char landform_name[MAX_LANDFORM_NAME_LEN];

	map<int, int> atk_up;
	unsigned short music;

	unsigned short type;
	unsigned short* seid;
	unsigned short seid_num;
};
typedef LandformAttr* LandformAttrPtr;

////////////////////////////////////////////////////////////
//技能属性<==>dic_skill.json
class HistoryActor;
class SkillAttr
{
public:
	SkillAttr()
	{
		skill_id = 0;
		image_id = 0;
		skill_object = 0;
		type = 0;
		magic_type = 0;
		use_region = 0;
		effect_region_type = 0;
		mp_consume = 0;
		power = 0;
		hurt_num = 0;
		prop_num = 0;
		prop_per = 0;
		seid = 0;
		seid_round = 0;
		hit_per = 0;
		buff_hit_per = 0;
		skill_mp3 = 0;
		delay1 = 0;
		delay2 = 0;
		boss_limit = 0;
		full_anim = 0;
		skill_limit = 0;
		iNoCopy = 0;
		iNoCost = 0;
		iTimes = 0;
		iActorUseNum = 0;
		iControlAtkid = 0;
		animation = NULL;
		effect = "";
		explain = "";

		memset(skill_name, 0, sizeof(skill_name));
		memset(skill_info, 0, sizeof(skill_info));
		memset(skill_effect, 0, sizeof(skill_effect));
	}
public:
	int skill_id;		//技能ID<==>skid
	int image_id;		//技能ICON<==>imgid
	int skill_object;	//技能释放的目标对象 0敌方 1我方等<==>object
	int type;			//技能类型<==>type
	int magic_type;		//技能法术种类<==>skill_type
	int use_region;		//技能可攻击范围<==>atkid
	int effect_region_type;//技能产生的效果范围<==>mpid
	int mp_consume;		   //技能MP的消耗<==>mp_consume
	int power;			   //伤害系数<==>power
	int hurt_num;		   //法术伤害系数<==>hurt_num
	int prop_num;		   //seid特殊效果的加成数值<==>prop_num
	int prop_per;		   //seid特殊效果的加成百分比<==>prop_per
	int seid;			   //法术特技<==>seid
	int seid_round;		   //buff持续回合数<==>seid_round
	int hit_per;		   //命中系数<==>hit_per
	int buff_hit_per;	   //异常状态命中系数<==>buff_hit_per
	int skill_mp3;		   //<==>mp3
	int delay1;			   //<==>delay1
	int delay2;			   //<==>delay2
	int boss_limit;		   //<==>boss_limit
	int full_anim;		   //魔法动画<==>full_anim
	int skill_limit;//法术限制
	int iNoCopy; //禁止复制<==>no_copy
	int iNoCost;//移动前使用不消耗移动力
	int iTimes;//每回合使用次数
	int iActorUseNum;//对actor 一句使用次数限制
	int iControlAtkid;//机关控制范围
	int* animation;	//动编[0]动编播放名，[1]可能的方向,[2]动编帧数<==>animation
	char skill_name[MAX_SKILL_NAME_LEN];		//技能名称<==>name
	char skill_info[MAX_INFO_LEN];				//技能简介<==>info
	char skill_effect[MAX_SKILL_EFFECT_LEN];	//技能效果文本<==>eff_txt
	string effect;	//<==>effect
	string explain; //<==>explain
	
	bool BeingAbleToCastSpells(HistoryActor* curActor);	//判断当前能否使用该法术
	int LimitUseSkill();//限制使用的法术
	int GetMpConsume(HistoryActor* curActor,int iIsShow = 0);//获得mp消耗
	float GetExtraGainHp(HistoryActor* curActor,int iType = 1);//获得技能额外提升的HP,1:通用伤害  2: seid伤害数值  3:seid伤害百分比
	int GetCasterActorId(HistoryActor* curActor);//获得施法对象
};
typedef SkillAttr* SkillAttrPtr;

//////////////////////////////////////////////////////////////////////////
//特技<==>dic_seid.json
class Seid
{
public:
	Seid()
	{
		seid = 0;
		memset(sename, 0, sizeof(sename));
		type = 0;
		special_effe = NULL;
		special_effe_num = 0;
		memset(Info, 0, sizeof(Info));
		imgid = 0;
	}
public:
	unsigned short seid;
	char sename[MAX_LL_NAME_LEN];
	unsigned short type;
	short* special_effe;
	short special_effe_num;
	char Info[MAX_INFO_LEN];
	short imgid;
	map<int, int> mOpenCondition;
};
typedef Seid* SeidPtr;

//特技类型
class SeidType
{
public:
	SeidType()
	{
		iTypeId = 0;
		iAddType = 0;
		iAddIndex = 0;
		iClass = 0;
		iEffNum = 0;
	}
public:
	int iTypeId;
	int iAddType;
	int iAddIndex;
	int iClass;
	int iEffNum;
};

class HitType
{
public:
	HitType()
	{
		pointlist = NULL;
		length = 0;
		memset(cname, 0, MAX_LL_NAME_LEN);
	}
public:
    DPoint* pointlist;
    int length;
	char cname[MAX_LL_NAME_LEN];
};

//////////////////////////////////////////////////////////////////////////
//兵种克制伤害系数<==>dic_job_pk.json
class JobPK  
{
public:
	JobPK()
	{
		cid = 0;
		cidpk = 0;
		hart = 0;
	}
public:
	unsigned short cid;
	unsigned short cidpk;
	unsigned short hart;
};

//////////////////////////////////////////////////////////////////////////
//套装信息<==>dic_suitinfo.json
class SuitInfo
{
public:
	SuitInfo()
	{
		suitinfo_id = 0;
		suit_image_id = 0;
		suit_gold_id = 0;
		suitarr_num = 0;
		suitarr = NULL;
		memset(suit_name, 0, sizeof(suit_name));
		memset(suit_info, 0, sizeof(suit_info));
	}
public:
	unsigned short suitinfo_id;
	unsigned short suit_image_id;
	int suit_gold_id;
	int suitarr_num;
	int* suitarr;
	char suit_name[MAX_GK_NAME_LEN];
	char suit_info[MAX_INFO_LEN];
};
typedef SuitInfo* SuitInfoPtr;

//////////////////////////////////////////////////////////////////////////
//强化<==>dic_strengthen_consume.json
class StrengthenConsume
{
public:
	StrengthenConsume()
	{
		level = 0;
		money = 0;
		special_goods = 0;
		quality = 0;
		gem_num = 0;
		success_rate = 0;
		atk_num = 0;
		def_num = 0;
		iTotalMoney = 0;
		iTotalGem = 0;
	}
	~StrengthenConsume(){}
public:
	int level;			//等级<==>level
	int money;			//花费<==>money
	int special_goods;	//是否有强化属性<==>special_goods
	int quality;		//<==>quality
	int gem_num;		//<==>gem_num
	int success_rate;	//<==>success_rate
	int atk_num;		//<==>atk_num
	int def_num;		//<==>def_num
	int iTotalMoney;	//<==>money_sum
	int iTotalGem;		//<==>gem_sum
};
typedef StrengthenConsume* StrengthenConsumePtr;

//////////////////////////////////////////////////////////////////////////
//<==>dic_material_exchange.json
class MaterialExchange
{
public:
	MaterialExchange()
	{
		goods_id = 0;
		gem_num = 0;
		memset(name, 0, MAX_ACTOR_NAME_LEN);
	}
	~MaterialExchange(){}
public:
	int goods_id;
	int gem_num;
	char name[MAX_ACTOR_NAME_LEN];
};
typedef MaterialExchange* MaterialExchangePtr;

//////////////////////////////////////////////////////////////////////////
//<==>dic_strengthen_property.json
class StrengthenProperty
{
public:
	StrengthenProperty()
	{
		id = 0;
		se_type = 0;
		quality = 0;
		odds = 0;		
		share = 0;
		seid = 0;
		memset(name, 0, MAX_GK_NAME_LEN);
	}
	~StrengthenProperty(){}
public:
	int id;
	int se_type;
	int quality;
	int odds;	
	int share;
	int seid;
	char name[MAX_GK_NAME_LEN];
};
typedef StrengthenProperty* StrengthenPropertyPtr;

//////////////////////////////////////////////////////////////////////////
//<==>dic_turn.json
class ActorTurn
{
public:
	ActorTurn()
	{
		hid = -1;
		turn = 0;
	}
	~ActorTurn(){}
public:
	int hid;
	int turn;
};

//////////////////////////////////////////////////////////////////////////
//<==>dic_fits.json
class ActorFits
{
public:
	ActorFits()
	{
		id = -1;
		hid = NULL;
		hid_num = 0;
		fits = 0;
		atkid = 0;
		sounds = NULL;
		sounds_num = 0;
		memset(info, 0, MAX_APP_NAME_LEN);
	}
	~ActorFits(){}
public:
	int id;
	int* hid;
	int hid_num;
	int fits;
	int atkid;
	int* sounds;
	int sounds_num;
	char info[MAX_APP_NAME_LEN];
};

//////////////////////////////////////////////////////////////////////////
//动画信息<==>net_db_info.json
class AnimationInfo
{
public:
	AnimationInfo()
	{
		hid = 0;
		gender = 0;
		memset(name, 0, MAX_GK_NAME_LEN);
	}
	~AnimationInfo(){}
public:
	int hid;
	int gender;
	char name[MAX_GK_NAME_LEN];
};

//////////////////////////////////////////////////////////////////////////
//网络模式职业信息<==>net_job_info.json
class NetJobInfo
{
public:
	NetJobInfo()
	{
		jobId = 0;
		skillId = NULL;
		skillNum = 0;
		memset(jobInfo, 0, MAX_INFO_LEN);
	}
public:
	int jobId;
	short* skillId;
	short skillNum;
	char jobInfo[MAX_INFO_LEN];
};

//////////////////////////////////////////////////////////////////////////
//<==>net_map_name.json
class NetMapName
{
public:
	NetMapName()
	{
		mapid = 0;
		memset(name, 0, sizeof(name));
		mapBgId = 1;
		mModel = NORMAL_MODEL;
	}
public:
	int mapid;
	char name[MAX_ACTOR_NAME_LEN];
	int mapBgId;
	int mModel;
};
typedef NetMapName* NetMapNamePtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_war_conditions.json
class WinCondition
{
public:
	WinCondition()
	{
		warid = 0;
		c_type = 0;
		data = NULL;
		data_num = 0;
		describe = "";
	}
public:
	int warid;
	int c_type;
	int* data;
	int data_num;
	string describe;
};
typedef WinCondition* WinConditionPtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_external_equip.json
class NetExternalEquip
{
public:
	NetExternalEquip()
	{
		equipid = 0;
		memset(weapon, 0, 5 * sizeof(weapon[0]));
		memset(armour, 0, 5 * sizeof(armour[0]));
		memset(assist, 0, 5 * sizeof(assist[0]));
		skill_lv = 0;
		quality = 0;
		jobid = 0;
	}
public:
	int equipid;
	int weapon[5];
	int armour[5];
	int assist[5];
	int skill_lv;
	int quality;
	int jobid;
	map<int, int> boss;
	map<int,int> level_area;
};
typedef NetExternalEquip* NetExternalEquipPtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_hero_list.json
class NetHeroList
{
public:
	NetHeroList()
	{
		hid = 0;
		good_id = 0;
	}
public:
	int hid;
	int good_id;
	map<int, map<int, int> >obtain_method;
};
typedef NetHeroList* NetHeroListPtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_hero_quality.json
class NetHeroQuality
{
public:
	NetHeroQuality()
	{
		quality = 0;
		call_soul = 0;
		rank_soul = 0;
		gold = 0;
	}
public:
	int quality;
	int call_soul;
	int rank_soul;
	int gold;
};
typedef NetHeroQuality* NetHeroQualityPtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_hero_skill.json
class NetHeroSkill
{
public:
	NetHeroSkill()
	{
		skillid = 0;
		memset(info, 0, MAX_APP_NAME_LEN);
	}
public:
	int skillid;
	map<int, int> skill;
	char info[MAX_APP_NAME_LEN];
};
typedef NetHeroSkill* NetHeroSkillPtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_hero_grown.json
class GrownAbilityInfo
{
public:
	GrownAbilityInfo()
	{
		atk = 0;
		def = 0;
		ints = 0;
		burst = 0;
		fortune = 0;
		hp_up = 0;
		mp_up = 0;
	}
public:
	float atk;
	float def;
	float ints;
	float burst;
	float fortune;
	float hp_up;
	float mp_up;
};
typedef GrownAbilityInfo* GrownAbilityInfoPtr;

//////////////////////////////////////////////////////////////////////////

//<==>net_shop_item.json
class NetShopItem
{
public:
	NetShopItem()
	{
		iShopId = 0;
		iGoodsId = 0;
		iPayType = 0;
		iPrice = 0;
		iNum = 0;
	}
public:
	int iShopId;
	int iGoodsId;
	int iPayType;
	int iPrice;
	int iNum;
};

//////////////////////////////////////////////////////////////////////////
//<==>net_hero_exp.json
class NetHeroLvUpExp
{
public:
	NetHeroLvUpExp()
	{
		mExp = 0;
		mTotalExp = 0;
	}
public:
	int mExp;
	int mTotalExp;
};
typedef NetHeroLvUpExp* NetHeroLvUpExpPtr;

//////////////////////////////////////////////////////////////////////////
//<==>net_skill_upgrade.json
class NetSkillUpgrade
{
public:
	NetSkillUpgrade()
	{
		skid = 0;
		up_limit = 0;
		up_mp_consume = 0;
		up_hurt_num = 0;
		up_prop_num = 0;
		up_prop_per = 0;
	}
public:
	int skid;
	int up_limit;
	int up_mp_consume;
	float up_hurt_num;
	float up_prop_num;
	float up_prop_per;
};
typedef NetSkillUpgrade* NetSkillUpgradePtr;

//////////////////////////////////////////////////////////////////////////
//金币兑换数据<==>net_jade_of_gold.json
class NetGoldExchange
{
public:
	NetGoldExchange()
	{
		iId = 0;
		iJade = 0;
		iGold = 0;
	}
public:
	int iId;
	int iJade;
	int iGold;
};

//////////////////////////////////////////////////////////////////////////
//勾玉购买信息<==>net_jade_charge.json
class NetJadeCharge
{
public:
	NetJadeCharge()
	{
		mId = 0;
		mRmb = 0;
		mJade = 0;
		mGiveJade = 0;
		mFirstJade = 0;
		mChargeId = "";
	}
public:
	int mId;
	int mRmb;
	int mJade;
	int mGiveJade;
	int mFirstJade;
	string mChargeId;
};
typedef NetJadeCharge* NetJadeChargePtr;

//////////////////////////////////////////////////////////////////////////
//章节宝箱<==>net_chest.json
class NetMapChest
{
public:
	NetMapChest()
	{
		mapid = 0;
		star = 0;
		gold = 0;
		jade = 0;
		improvedStone = 0;
		chest_img = 0;
	}
public:
	int mapid;
	int star;
	int gold;
	int jade;
	int improvedStone;
	int chest_img;
	map<int,int> itemid;
};
typedef NetMapChest* NetMapChestPtr;

//////////////////////////////////////////////////////////////////////////
//酒馆数据<==>net_pub_qinju.json
class PubInvitePrice
{
public:
	PubInvitePrice()
	{
		iId = 0;
		iGold = 0;
		iJade = 0;
	}
public:
	int iId;
	int iGold;
	int iJade;
};
//////////////////////////////////////////////////////////////////////////
//<==>net_pub_position.json
class PubPosition
{
public:
	PubPosition()
	{
		iId = 0;
		iPointX = 0;
		iPointY = 0;
	}
public:
	int iId;
	int iPointX;
	int iPointY;
};

//////////////////////////////////////////////////////////////////////////
//<==>net_vip_info.json
class VipInfo
{
public:
	VipInfo()
	{
		iVipId = 0;
		strInfo = "";
	}

public:
	int iVipId;
	string strInfo;
	map<int, int> mapGifts;
};

//////////////////////////////////////////////////////////////////////////
//关卡副本数据<==>net_map_war.json
class NetWarInfo
{
public:
	NetWarInfo()
	{
		iId = 0;
		iWarId = 0;
		iType = BATTLE_NORMAL;
		iImgId = 0;
		iMaxTimes = 0;
		iEnemyLv = 0;
		iMapId = 0;
		iNumLimit = 0;
		iGold = 0;
		iJade = 0;
		iOpenWarId = 0;
		iExp = 0;
		iStore = 0;
		iPosX = 0;
		iPosY = 0;
		iLevel = 0;
		iRound = 0;
		iMapIndex = 0;
		iGkLv = 0;
		iModel = 0;
		iStage = 0;
		iDegree = 0;
		iActive = 0;
		iBossHp = 0;
		iPawnHp = 0;
        
		strName = "";
		strInfo = "";
		strStart = "";
		strEnd = "";
		strLv = "";
	}
public:
	int iId;
	int iWarId;
	int iType;
	int iModel;
	int iImgId;
	int iMaxTimes;
	int iEnemyLv;
	int iMapId;
	int iNumLimit;
	int iGold;
	int iJade;
	int iOpenWarId;
	int iExp;
	int iStore;
	int iPosX;
	int iPosY;
	int iLevel;
	int iRound;
	int iMapIndex;
	int iGkLv;//置1开启，敌将等级与我军出场人物最大等级持平
	int iStage;//难度个数
	int iDegree;//难度系数
	int iActive;//活跃
	int iBossHp;//boss总hp
	int iPawnHp;//团本结算小兵占据的hp百分比
	map<int,int> guarding;//镇守武将
	map<int,int> drop;//掉落
	map<int, int> mBossId;
	map<int, int> mStarLimit;
	map<int, map<int, int> > mEnemyMap;

	string strName;
	string strInfo;
	string strStart;
	string strEnd;
	string strLv;
	map<int, string> mstrScene;
	map<int, int> mReach;
};

//大地图城市数据<==>net_map_city.json
class NetMapCity
{
public:
    NetMapCity()
    {
        iId=0;
        iImgId=0;
        iPosX=0;
        iPosY=0;
        strName="";
        isLock=true;
    }
public:
    int     iId;
    int     iImgId;
    string  strName;
    int     iPosX;
    int     iPosY;
    bool    isLock;
};
//神秘商城表
class MysNetShop
{
public:
	MysNetShop()
	{
		iShopId = 0;
		iGoodsId = 0;
		iPayType = 0;
		iPrice = 0;
		iNum = 0;
	}
public:
	int iShopId;
	int iGoodsId;
	int iPayType;
	int iPrice;
	int iNum;
};
//关卡限制表<==>net_war_premise.json
class NetWarPermise
{
public:
    NetWarPermise()
    {
        iId=0;
    }
public:
    int   iId;
    map<int,int>  mP_warid;
    map<int,int>  mVar;
	map<int, string> mExplain;
    map<int,int>  mGoods;
    map<int,int>  mHid;
};
//商城刷新表
class NetRefresh
{
public:
	NetRefresh()
	{
		iVipLv = 0;
		iFree_special = 0;
		iFree_plain = 0;
		iSubst20 = 0;
		iSubst40 = 0;
		iSubst80 = 0;
		iMax_subst = 0;
	}
public:
	int   iVipLv;
	int   iFree_special;
	int   iFree_plain;
	int   iSubst20;
	int   iSubst40;
	int   iSubst80;
	int   iMax_subst;
};
//重铸金额表
class NetItemRecastPrice
{
public:
	NetItemRecastPrice()
	{
		iRecast = 0;
		iPrice = 0;
	}
public:
	int iRecast;
	int iPrice;
};
//元宝换粮草表
class NetJadeOfVitality
{
public:
	NetJadeOfVitality()
	{
		iId = 0;
		iJade = 0;
		iVitality = 0;
	}
public:
	int iId;
	int iJade;
	int iVitality;
};
//vip等级对应表
class VipIncome
{
public:
	VipIncome()
	{
		iVIP = 0;
		iMAX = 0;
		iRate = 0;
		iSweep = 0;
		iVipexp = 0;
		iTradings = 0;
		iRemission = 0;
	}
public:
	int iVIP;
	int iMAX;
	int iRate;
	int iSweep;
	int iVipexp;
	int iTradings;
	int iRemission;
};
//////////////////////////////////////////////////////////////////////////
//副本活动开启提示<==>net_prompt.json
class NoticeActOpen
{
public:
    NoticeActOpen()
	{
		iWarId = 0;
		iOpenType = 0;
		iImgId = 0;
		strText1 = "";
		strText2 = "";
	}
public:
	int iWarId;
	int iOpenType;
	int iImgId;
	string strText1;
	string strText2;
};

//////////////////////////////////////////////////////////////////////////
//网络公告
class NetNoticeInfo
{
public:
	NetNoticeInfo()
	{
		strTitle = "";
		strContent = "";
	}
public:
	string strTitle;
	string strContent;
};

//////////////////////////////////////////////////////////////////////////
//地图半透明图层<==>net_map_clear.json
class NetMapClear
{
public:
	NetMapClear()
	{
		warid = 0;
		clear_img = "";
	}
public:
	int warid;
	map<int,int> position;
	string clear_img;
};

//////////////////////////////////////////////////////////////////////////
//演义堂数据<==>net_story.json
class NetStory
{
public:
	NetStory()
	{
		mapid = 0;
		name = "";
		story_type = 0;
		jade = 0;
		jadeSeal = 0;
		describe = "";
		author = 0;
		map_img = 0;
		condition = 0;
	}
public:
	int mapid;
	string name;
	int story_type;
	int jade;
	int jadeSeal;
	map<int, int> hid;
	map<int, int> good_id;
	string describe;
	int author;
	map<int, map<int, int> > txt_color;
	int map_img;
	int condition;
};

//////////////////////////////////////////////////////////////////////////
//演义堂关卡数据
class NetStoryWar
{
public:
	NetStoryWar()
	{
		mapid = 0;
		warid = 0;
		img = 0;
		x = 0;
		y = 0;
		name = "";
		describe = "";
	}
public:
	int mapid;
	int warid;
	int img;
	int x;
	int y;
	map<int, int> reach;
	string name;
	string describe;
};

//////////////////////////////////////////////////////////////////////////
//演义堂过关条件和战利品<==>net_story_condition.json
class NetStoryCondition
{
public:
	NetStoryCondition()
	{
		iWarId = 0;
		iType = 0;
		iVariable = 0;
		iGqId = 0;
		strDescribe = "";
	}
public:
	int iWarId;
	int iType;
	int iVariable;
	int iGqId;
	string strDescribe;
};

//////////////////////////////////////////////////////////////////////////
//<==>net_story_goods.json
class NetStoryGoods
{
public:
	NetStoryGoods()
	{
		iWarId = 0;
		iVariable = 0;
	}
public:
	int iWarId;
	int iVariable;
	map<int, int> mHid;
	map<int, int> mGoods;
};

//////////////////////////////////////////////////////////////////////////
//BUFF数据<==>net_buff.json
class BuffElem
{
public:
	BuffElem()
	{
		iId = 0;
		iImgId = 0;
		iType = 0;
		iPercent = 0;
		iReset = 0;
		iEffectType = 0;
		iRound = 0;
		iIsPlay = 0;
		iSound = 0;
		strName = "";
		strAnima1 = "";
		strAnima2 = "";
	}
public:
	int iId;
	int iImgId;
	int iType;
	int iPercent;
	int iReset;
	int iEffectType;
	int iRound;
	int iIsPlay;
	int iSound;
	string strName;
	string strAnima1;
	string strAnima2;
};

//////////////////////////////////////////////////////////////////////////
//武将出场数据<==>net_war_hero.json
class NetWarHero
{
public:
	NetWarHero()
	{
		iWarId = 0;
		iDispatchActorMinNum = 0;
		allow_lv = 0;
	}
public:
	int iWarId;
	map<int, int> fix_hid;
	map<int, int> avoid_hid;
	map<int, int> allow_hid;
	int  iDispatchActorMinNum;
	int allow_lv;
};

//////////////////////////////////////////////////////////////////////////
//套装<==>net_suit.json
class NetSuitData
{
public:
	NetSuitData()
	{
		id = 0;
		name = "";
		info = "";
	}
public:
	int id;
	string name;
	map<int, int> item_id;
	map<int, int> seid1;
	map<int, int> hid;
	map<int, int> seid2;
	string info;
};
typedef NetSuitData* NetSuitDataPtr;

//////////////////////////////////////////////////////////////////////////
//日常副本<==>net_daily.json
class DailyWar
{
public:
	DailyWar()
	{
		iId = 0;
		iMapId = 0;
		iImgId = 0;
		iMaxTimes = 0;
		strName = "";
		strInfo = "";
	}
public:
	int iId;
	int iMapId;
	int iImgId;
	int iMaxTimes;
	map<int, int> mWarId;
	string strName;
	string strInfo;
};

//////////////////////////////////////////////////////////////////////////
//S形象时装<==>net_hero_dress.json
class NetHeroDress
{
public:
	NetHeroDress() {}
public:
	map<int, int> mAtk;
	map<int, int> mMov;
	map<int, int> mSpc;
};

//////////////////////////////////////////////////////////////////////////
//物品合成类别<==>net_make_type.json
class NetMergeType
{
public:
	NetMergeType()
	{
		iId = 0;
		iImgId = 0;
		strName = "";
	}
public:
	int iId;
	int iImgId;
	string strName;
};

//////////////////////////////////////////////////////////////////////////
//物品合成<==>net_make_data.json
class NetMergeData
{
public:
	NetMergeData()
	{
		iGoodsId = 0;
		iType = 0;
		iCount = 0;
		iGold = 0;
		strName = "";
		barrage = 0;
	}
public:
	int iGoodsId;
	int iType;
	int iCount;
	int iGold;
	int barrage;
	map<int, int> mMaterial;
	string strName;
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//物品合成<==>net_stalker_reward.json
class NetStalkerReward
{
public:
	NetStalkerReward()
	{
		id = 0;
		name = "";
	}
public:
	int id;
	string name;
	map<int, map<int,int> > itemid;
};

//////////////////////////////////////////////////////////////////////////
//敌军时装<==> net_hero_no_dress
class NetHeroNoDress
{
public:
	NetHeroNoDress()
	{
		hid = 0;
		gender = 0;
		fashion1 = 0;
		fashion2 = 0;
	}
public:
	int hid;
	int gender;
	int fashion1;
	int fashion2;
};

//////////////////////////////////////////////////////////////////////////
//工会任务<==> net_active_task
class NetActiveTask
{
public:
	NetActiveTask()
	{
		id = 0;
		active_type = 0;
		condition = 0;
		warid = 0;
		active = 0;
		img = 0;
		explain = "";
		nameS = "";
	}
public:
	int id;
	int active_type;
	int condition;
	int warid;
	int active;
	map<int, int> itemid;
	int img;
	string explain;
	string nameS;
};

//////////////////////////////////////////////////////////////////////////
//附魔宝石套装<==> net_bw_suit.json
class FmStoneSuit
{
public:
	FmStoneSuit()
	{
		iSuitId = 0;
	}
public:
	int iSuitId;
	map<int, int> mNeedId;
	map<int, int> mNorSeid;
	map<int, int> mHeroId;
	map<int, int> mHeroDel;
	map<int, int> mHeroAdd;
	map<int, int> mHeroExcl;
};
//////////////////////////////////////////////////////////////////////////
//系统开启配置表<==> system_open_control.json
class SystemOpenItem
{
public:
	SystemOpenItem(const Json::Value& json)
	{
		id = json["id"].asInt();
		lv_limit = json["lv_limit"].asInt();
		hero_num = json["hero_num"].asInt();
		war_id = json["war_id"].asInt();
		vip = json["vip"].asInt();
		player_lv = json["player_lv"].asInt();
		strName = json["name"].asCString();
	}
public:
	int id;
	int lv_limit;
	int hero_num;
	std::map<int,int> hero;
	std::map<int,int> goods;
	int war_id;
	int vip;
	int player_lv;
	std::string strName;


};
//////////////////////////////////////////////////////////////////////////
//大战役<==> net_zmzy_map.json
class NetZmzyMap
{
public:
	NetZmzyMap()
	{
		id = 0;
		name = "";
		bgimg = 0;
		number_battle = 0;
		explain = "";
	}
public:
	int id;
	string name;
	int bgimg;
	int number_battle;
	map<int,int> warid;
	map<int, int> itemid;
	string explain;
};
//////////////////////////////////////////////////////////////////////////
//副将系统 <==> dic_ratio_buff.json
class NetRatioBuff
{
public:
	NetRatioBuff()
	{
		ratio = 0;
		level = 0;
	}
public:
	int ratio;
	int level;
	map<int, int> seid;
};

//////////////////////////////////////////////////////////////////////////
//PVP对战坐标系<==> net_group_xy.json
class PVPPoint
{
public:
	PVPPoint()
	{
		iPointId = 0;
		iPosX = 0;
		iPosY = 0;
	}
public:
	int iPointId;
	int iPosX;
	int iPosY;
};

//////////////////////////////////////////////////////////////////////////
//PVP阵形<==> net_group.json
class PVPArray
{
public:
	PVPArray()
	{
		iArrayId = 0;
		strName = "";
		strInfo = "";
	}
public:
	int iArrayId;
	string strName;
	string strInfo;
	map<int, int> mPosGroup;
	map<int, map<int, int> > mSeidGroup;
};

//////////////////////////////////////////////////////////////////////////
//PVP奖励<==> net_autopvp_reward.json
class PVPReward
{
public:
	PVPReward()
	{
		iRewId = 0;
		strName = "";
	}
public:
	int iRewId;
	string strName;
	map<int, map<int, int> > mReward;
};

//////////////////////////////////////////////////////////////////////////
class ChessData
{
public:
	ChessData()
	{
		hint_num = 0;
		scene_hint = NULL;

		actor_num = 0;
		actors = NULL;
		killword = NULL;

		occupation_type_num = 0;
		occu_types = NULL;

		occupation_num = 0;
		occupations = NULL;

		trGamePay = NULL;
		trAccountBuy = NULL;

		skill_num = 0;
		skills = NULL;

		land_num = 0;
		lands = NULL;

		jobpk_num = 0;
		jobpk = NULL;

		bustHImg_num = 0;
		bustHImg = NULL;

		suiteffect_num = 0;
		suiteffect = NULL;

		suitinfo_num = 0;
		suitinfo = NULL;

		strengthen_consume_num = 0;
		strengthen_consume = NULL;

		material_exchange_num = 0;
		material_exchange = NULL;

		strengthen_property_num = 0;
		strengthen_property = NULL;

		actor_turn_num = 0;
		actor_turn = NULL;

		actor_fits = NULL;
		actor_fits_num = 0;

		animation_info_num = 0;
		animation_info = NULL;

		win_condition_num = 0;
		win_condition = NULL;

		occupation_land_info = NULL;

		occupation_landmove_info = NULL;

		occupation_good_info = NULL;

		skill_land_info = NULL;

		occupation_special_ability = NULL;

		occupation_image_id = NULL;

		memset(kill_words, 0, sizeof(kill_words));
		isLoadItunesPriceSuccess = 0;

		iWeaponNum = 0;
		iCoatNum = 0;
		ptrDefWeapon = NULL;
		ptrDefCoat = NULL;

		m_pNetNotice = NULL;
	}
public:

	int isLoadItunesPriceSuccess;

	//小贴士数目
	int hint_num;
	//小贴士
	char **scene_hint;

	int actor_num;//所有武将数量
	ActorAttr* actors;//所有武将
	int* killword;
	
	int occupation_type_num;
	OccupationType* occu_types;

	int occupation_num;
	OccupationAttr* occupations;
    
	//道具类型
	map<int, ItemType*> m_mItemType;

	//道具大类型
	map<int, ItemClass*> m_mItemClass;
	//宝物信息
	map<int, TreasureInfo*> m_mTreasureInfo;
	//特技
	map<int, Seid*> m_mSeid;
	//特技TYPE
	map<int, SeidType*> m_mSeidType;
	//计费信息
	TrGamePay* trGamePay;
	//已购计费信息
	TrAccountBuy* trAccountBuy;

	int skill_num;
	SkillAttr* skills;
	
	int land_num;
	LandformAttr* lands;

	int jobpk_num;
	JobPK* jobpk;

	//半身像截取小头像
	int bustHImg_num;
	BustHImgPtr bustHImg;

	int suiteffect_num;
	SuitEffectPtr suiteffect;

	int suitinfo_num;
	SuitInfoPtr suitinfo;

	int strengthen_consume_num;
	StrengthenConsumePtr strengthen_consume;

	int material_exchange_num;
	MaterialExchangePtr material_exchange;

	int strengthen_property_num;
	StrengthenPropertyPtr strengthen_property;

	map<int, map<int,int> > map_total_war;

	map<int, NetMapNamePtr> net_map_name;

	int actor_turn_num;
	ActorTurn* actor_turn;

	int actor_fits_num;
	ActorFits* actor_fits;

	int animation_info_num;
	AnimationInfo* animation_info;

	map<short, NetExternalEquipPtr> net_external_equip;
	map<int, map<int, NetExternalEquipPtr> > net_automatic_equip;
	map<int, map<int, NetExternalEquipPtr> > net_elite_equip;

	map<int, map<int, int> > skill_weather_addition;

	//网络模式职业表
	map<int, NetJobInfo*> m_mNetJob;
	map<int, string> m_mFirstName;
	map<int, string> m_mSecName;
	map<int, string> m_mNameForbid;

	map<int, GoodsItem*> m_mGoodsItem;
	map<int, int> m_mFmStoneId; //附魔宝石物品组合

	map<int, NetHeroSkillPtr> net_hero_skill;

	int win_condition_num;
	WinConditionPtr win_condition;

	map<int, NetHeroListPtr> net_hero_list;

	map<int, NetHeroQualityPtr> net_hero_quality;

	map<int, NetHeroLvUpExpPtr> net_hero_exp;

	map<int, map<int, GrownAbilityInfoPtr> > net_hero_grown;

	map<int, NetSkillUpgradePtr> net_skill_upgrade;

	map<int, int> net_skill_gold;

	map<int, NetShopItem*> m_mNetShop;
	map<int, MysNetShop*>  m_mMysNetShop;
	map<int, int> m_mShopRefPrice;

	map<int, int> m_mGoodsItemItid; //需要判定是否能够装备的物品ITID集合，主要是武器和防具的ITID

	map<int, map<int, int> > net_war_goods;

	map<int, NetGoldExchange*> m_mGoldExchange;

	map<int, map<int, NetMapChestPtr> > net_map_chest;

	//酒馆数据
	map<int, int> m_mPubRefPrice;
	map<int, PubPosition*> m_mPubPosition;
	map<int, PubInvitePrice*> m_mPubInvite;

	map<int, NetJadeChargePtr> net_jade_charge;

	map<int, VipInfo*> m_mVipInfo;

	map<int, int> net_heroz_price;

	map<int, NetWarInfo*> m_mNetWarInfo; //战场ID做为下标
	map<int, NoticeActOpen*> m_mNoticeOpen; //活动副本和英雄志开启提示框
	map<int, map<int, NetMapClear*> > net_map_clear;

	map<int, NetMapCity*> m_mNetMapCity;//战棋三国城市，城市ID作为下标
	map<int, NetItemRecastPrice*> m_mNetRecastPrice;//战棋三国重铸表，重铸特性顺位作为下标
	map<int, NetJadeOfVitality*> m_mNetJadeOfVitality;//战棋三国元宝换粮草
	map<int, NetRefresh*> m_mNetShopRef;//战棋三国商城，vip等级作为下标
	map<int, VipIncome*>  m_mVipIncome;//战棋三国，vip等级作为下标

	NetNoticeInfo* m_pNetNotice;

	map<int, NetStory*> net_story;

	map<int, map<int, NetStoryCondition*> > net_story_condition;

	map<int, map<int, NetStoryGoods*> > net_story_goods;
	
	map<int, BuffElem*> m_mDataBuff;	

	map<int, NetWarHero*> net_war_hero;
	map<int, NetWarPermise*> net_war_permise;//关卡条件解析

	map<int, string> m_mErrorMSG; //网络错误警告

	map<int, NetSuitDataPtr> net_suit_data;

	map<int, DailyWar*> m_mDailyWar; //日常副本

	map<int, NetStalkerReward*> net_stalker_reward;

	map<int, NetZmzyMap*> net_zmzy_map;//大战役

	map<int, NetRatioBuff*> net_ratio_buff;
	
	UUCHAR** occupation_land_info;
	
	UUCHAR** occupation_landmove_info;	

	UUCHAR** occupation_good_info;

	UUCHAR** skill_land_info;
	
	int *occupation_special_ability;

	unsigned short* occupation_image_id;
    
    HitType hitArea[100];
    HitType effectArea[100];

	char kill_words[100][3][100];

	int iWeaponNum;
	int iCoatNum;
	int* ptrDefWeapon;
	int* ptrDefCoat;

	map<int,int> mFashionList;

	map<int,NetHeroDress*> net_hero_dress;

	map<int, NetMergeType*> m_mMergeType; //物品合成类型
	map<int, NetMergeData*> m_mMergeData; //物品合成数据

	map<int, map<int, int> > net_cloud_xy;

	map<int, NetHeroNoDress*> net_hero_no_dress;

	map<int, NetActiveTask*> net_active_task;

	map<int, FmStoneSuit*> m_mFmStone; //附魔宝石套装数据

	map<int, SystemOpenItem*> m_mSystemOpen; //系统开启配置表
	map<int, PVPPoint*> m_mPVPPoint; //PVP坐标系
	map<int, PVPArray*> m_mPVPArray; //PVP阵形数据
	map<int, PVPReward*> m_mPVPReward; //PVP奖励
	map<int, DailyWar*> m_mPKMap; //巅峰对决集合
public:
	int ChessInitData();	
	void saveSettingFlag();//保存设置信息
	void loadSettingFlag();//加载设置信息
	void initIOSPrice();//刷新iTunes价格信息
    bool canRangeEffect(int areaid, int x1, int y1, int x2, int y2);
	bool isConsume(int iGoodsId);
	int getOpenHeroicId(int iWarId); //通过当前章节获取可开启的英雄志ID，如果该章节不会开启英雄志则返回0；

	map<int, string> getStringMap(string chData, char chSplit); //以chSplit为分割符，将chData分割为若干子串
	map<int, int> getSeidData(const char* chData); //读取'&'字符分割的数据
	map<int, int> getMapData(const char* chData); //读取'&'字符分割的数据,元素作为下标
	map<int, int> getGoodsMapData(const char* chData); //读取'&'和'|'字符分割的数据
	map<int, map<int,int> > getMapJsonData(const char* chData); //读取'&'和'|'字符分割的数据
	void CopyActor2toActor1(ActorAttr* ptrActor1, ActorAttr* ptractor2);
	NetJadeCharge* getJadeCharge(int iPrice); //根据价格获取相应的计费点
	int getMakeGoodsId(int iMaterial); //根据合成表获取材料合成的最终物品
	bool isSystemOpen(const SystemOpenIDS& id, bool popTips);
private:
	/*
	* 解析数据
	*/
	void ParseData();	
	/*
	* 特技
	*/
	void getdicseid();
	void getItemType();
	void getDicJobClass();
	void getDicJobPk();
	void getDicJobItem();
	void getDicJobTerrain();
	void getDicJobWalk();
	void getDicKillTxt();
	void getAtkSe(const char* sData, int index);
	void getDicAtkSe();
	void getMpSe(const char* sData, int index);
	void getDicMpSe();
	void getDicTurn();
	void getTips();
	void getBustHImg();
	void getDicSuitEffect();
	void getDicSuitInfo();
	//解析json中string字段（如seid：1&2&3&）,return 数字个数
	int getjson_stringeffect(int**arr, const char* json_test);
	//json数组获取
	void ArrGetJsonInfo(int* arr, const char* json_test,int arrLen);
	//i ：actors下标
	//type : 1.人物特技（seid）
	void getherodata(const char* json_test, int i, int type);
	void getDicHero();
	//i ：occupations下标
	//type : 1.兵种特技（seid）
	void getjobdata(const char* json_test, int i, int type);
	void getDicJob();
	// i:Lands下标
	// type :1.可用效果类型 （0：火类，1：水类，2：地类，3：风类，&：没有任何效果）注意 0 是有用的\2.seid (type)
	void getLandsdata(const char* json_test, int i, int type);
	void getDicLands();
	// i ：goods下标
	void getDicItem();
	void getTrGamePay();
	void getTrAccountBuy();
	void getDicSkill();
	void GetSeidData(const char* json_test, short& seid_num, short*& seid);
	void getDicStrengthenConsume();//获取强化消耗信息
	void getDicMaterialExchange();//获取材料兑换强化石
	void getDicFits();//获得合体技字典表
	void getDicItemClass(); //获取宝物的大类型
	void getDicTreasureInfo(); //获取宝物信息
	void getAnimationInfo();//获取动编信息
	void getNetJobInfo(); //获取网络模式职业列表
	void getNetMapWar();//获取游历天下关卡数据
	void getNetMapWarJy();//获取游历天下精英关卡数据
	void getNetMapName();//获取网络关卡名
	void getNetWarCondition();//获取过关条件
	void getNetExternalEquip();//获取外部装备信息
	void getNetAutomaticEquip();//获取自动外部装备信息
	void getNetEliteEquip();//获取精英武将自动外部装备信息
	void getNetHeroList();//获取可得到武将
	void getNetHeroListElem(int iIndex, const char* json_test);//获取可得到武将碎片的出处信息
	void getNetHeroQuality();//获取武将合成、升阶对应的碎片数
	void getNetHeroSkillElem(int iIndex, const char* json_skill, const char* json_level);
	void getNetHeroSkill();//获取武将技能
	void getNetHeroExp();//获取武将经验
	void getNetHeroGrown();//获取武将升级加成
	void getNetSkillGold();//获取技能升级金币消耗
	void getNetSkillUpgrade();//获取技能升级
	void getNetWarGoods();//获得关卡掉落
	void getNetMapChest();//获得章节宝箱
	void getNetHerozWar();//获得英雄志信息
	void getNetHerozPrice();// 获得副本重置勾玉数
	void getMapWeather();//获得天气加成
	void getNetStory();//获得演义堂数据
	void getNetStoryWar();//获得演义堂战场数据
	void getNetStoryTxtColor(int iMapId, const char* json_test);//获得剧本介绍字的颜色
	void getNetStoryCondition();//获得演义堂关卡开启条件
	void getNetStoryGoods();//获得演义堂关卡掉落
	void getNetWarHero();//获得武将出场数据
	void getNetSuit();//获得套装数据
	void getNetStalkerWar();//获得远征关卡数据
	void getNetJadeCharge();
	void getNetChestElem(int iMapId, int iStar, const char* json_item);
	void getNetHeroName();
	void getNetHeroNameForbid();
	void getNetShopInfo();
	void getMysNetShop();
	void getNetGoldExchange();
	void getNetPubInfo();
	void getNetVipInfo();
	void getNetFbWar();
	void getNetPrompt();
    void getNetMapCity();//战棋三国城市数据
	void getNetRecastPrice();//战棋三国重铸数据
	void getNetJadeOfVitality();//战棋三国元宝换取粮草
	void getVipIncome();//战棋三国vip等级表
	void getNetShopRefresh();//战棋三国城市数据    
	void getNetMapPremise();//战棋三国关卡条件
	void getNetMapClear();//读取战场半透明地图
	void getNetBuff();
	void getNetErrorMSG();
	void getNetHeroDress();//获取武将时装武器位置
	void getNetGlcWar();//获取高狼城关卡数据
	void getNetStalkerReward();//获得过关斩将奖励
	void getNetCloudXy();//获得大地图是否被云遮挡
	void getNetHeroNoDress();//获得敌军初始化时装
	void getNetXmtWar();//获取心魔塔战场数据
	void getNetActiveTask();//获取工会日常任务
	void getNetWarGhFb();//获取工会副本
	void getNetZmzyMap();//获取大战役
	void getNetZmzyWar();//获取大战役战场数据
	void getNetRatioBuff();//获取副将系统数据
private:
	void parseJsonData();
	void reloadData(); //热更新后需要重新加载的文件
	void initSkillLandInfo(); //初始化技能地形限制信息
	void InitGoodAttrInfo(); //初始化兵种装备相关信息
	int GetNumDigit(int iNum);
};

//////////////////////////////////////////////////////////////////////////
class ModConst
{
public:
	ModConst();
	void loadConfig();
	void InitConfig();
public:
	//json读取的新的全局变量
	int NEW_MAX_FRIEND_ARMY_NUM;
	int NEW_MAX_ENEMY_ARMY_NUM;
	int NEW_MAX_SKILL_NUM;
	int m_iSkillImg; //技能图片最大索引
    int beginning_city;//三国战棋初始进入游戏城市ID
	bool mIsTestService;//是否测试服
	bool OpenIosAliPay;//开启iosAlipay
	bool m_bGuideSwitch; //新手引导开关，默认false；
	bool m_bRestoreClose; //关闭订单回复，默认false，打开
};

//////////////////////////////////////////////////////////////////////////
class FunctionConst
{
public:
	FunctionConst();
	DString getChannelId();
	static void initInstance();
private:
	void loadConfig();
public:
	// 游戏名称
	DString GameName;
	// 游戏路径
	DString GamePath;
	// 开启TalkingData
	bool EnableTalkingData;
	DString TalkingDataAppId;
	//DString TalkingDataChannelId;
	// 是否加密
	bool EncryptResource;
	// 显示帧数
	bool ShowFrame;
	// 项目包名
	DString strPackageName;

	//微信相关配置
	DString strWXAppid;
	DString strWXSecret;
    //QQ相关配置
    DString strQQAppid;
private:
	DString TalkingDataChannelId;
};

//////////////////////////////////////////////////////////////////////////
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)

class Win32Const
{
public:
	Win32Const()
	{
		downloadJson = 0;
		packageName = "com.bantu.fsyjz";
		versionName = "0.0.1";
		platform = "ios";
		yiDaoMiao = 0;
		mpEmpty = 0;
		iSingleSeckill = 0;
		iNolimitMove = 0;
		iMapElemNum = 0;
		ProtagonistLv = 1;
	}
private:
	void loadConfig();
public:
	static void initInstance();
public:
	// 服务器同步
	int downloadJson;
	// 游戏包名
	DString packageName;
	// 版本名
	DString versionName;
	// 平台
	DString platform;
	// 一刀秒
	int yiDaoMiao;
	// MP空
	int mpEmpty;
	//单人一刀秒
	int iSingleSeckill;
	//无限移动
	int iNolimitMove;
	//e5地图数
	int iMapElemNum;
	//主角等级
	int ProtagonistLv;
};
extern Win32Const* win32Const;
#endif

extern ModConst* modConst;
extern FunctionConst* funConst;
extern ChessData* trData;


#endif //#ifndef _DATAHANDLER_H_
