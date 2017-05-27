/************************************************************************/
/* 物品<==>dic_item.json                                        */
/************************************************************************/
#ifndef _GOODATTR_H_
#define _GOODATTR_H_

//物品属性
#define MAX_GOOD_NAME_LEN 50
#define MAX_GOOD_INFO_LEN 400

using namespace std;

enum 
{
	GOOD_TYPE_WEAPON = 0x01,
	GOOD_TYPE_COAT = 0x02,
	GOOD_TYPE_ASSIST = 0x04,
	GOOD_TYPE_ALL = 0x07,
	GOOD_TYPE_CONSUME = 0x10,
	GOOD_TYPE_PIECE = 0x20,
	GOOD_TYPE_FASHING = 0x40,
	GOOD_TYPE_WAP_FASHING = 0x80,
	GOOD_TYPE_ALLCLASS = 9999,
	GOOD_TYPE_MAX
};

enum
{
	GOODS_POWER = 0x01,       //攻击力
	GOODS_INTEL = 0x02,       //精神力
	GOODS_DEFENCE = 0x04,     //防御力
	GOOD_EFFECT_MAX
};

class GoodsItem
{
public:
	GoodsItem()
	{
		iGoodsId = 0;
		iImgId = 0;
		iItId = 0;
		iTypeId = 0;
		iSeSeid = 0;
		iPower = 0;
		iPrice = 0;
		iUpVal = 0;
		iUeid = 0;
		iIsOnly = 0;
		strName = "";
		strInfo = "";
		strUeid = "";
		bIsConsume = false;
	}
public:
	int iGoodsId;		//物品ID<==>good_id	
	int iImgId;			//物品IconID<==>image_id
	int iItId;			//物品类型（对应于dic_itemType.json）<==>itid 
	int iTypeId;		//物品可强化次数<==>typeid
	int iSeSeid;		//物品最终强化制定特性ID(对应dic_strengthen_property)<==>se_seid
	int iPower;			//物品初始属性<==>power
	int iPrice;			//物品价格<==>price
	int iUpVal;			//物品成长属性<==>up_val
	int iUeid;			//消耗物品的效果ID<==>ueid
	int iIsOnly;       //是否具有唯一性<==>isOnlyOne
	int iSpePower;		//？？？？？没有用到
	int m_iGoodType;	//物品类型枚举值 GOOD_TYPE_WEAPON GOOD_TYPE_COAT 等	
	string strName;		//物品名称<==>name
	string strInfo;		//物品介绍<==>info
	string strUeid;		//消耗物品的效果说明<==>ueid_txt
	map<int, int> mSeid;	//物品自带特性（对应dic_seid）格式<0,512>,<1,288><==>seid
	map<int, int> mSuitList;	//<==>suit_id
	map<int, int> mHid;			//时装物品可装备武将id
	map<int, int> mFmItid; //支持的附魔装备类型
	map<int, int> mNoHid;			//物品不可装备武将id
	bool bIsConsume;        //物品是否为消耗品
	void InitGoodsType();	//m_iGoodType和bIsConsume赋值
	int GetGoodsAbilityType();	//附带属性GOODS_POWER GOODS_INTEL GOODS_DEFENCE等
	char* GetAttributeInfo();	//物品类别名称 刀 剑 弓等	
	char* GetSpecialEffectInfo(int level);//物品第一个特性信息
	char* GetSpecialEffectName(int level);//物品第一个特性名称
	char* GetGoodsAbilityInfo(int level); //物品的基础属性信息
	string GetJobItem();		//物品适用于某职业
	string GetAllSeidInfo();	//物品所有特性信息
public:
	/// 特殊物品
	bool isSpecial()
	{
		return !isGeneral();
	}
	/// 代表普通物品
	bool isGeneral()
	{
		return mSeid[0] < 1;
	}
	/// 代表辅助物品
	bool isAssist()
	{
		return m_iGoodType == GOOD_TYPE_ASSIST;
	}
	bool isEquip() //是装备，包括武器和防具，有等级特性
	{
		return m_iGoodType == GOOD_TYPE_WEAPON || m_iGoodType == GOOD_TYPE_COAT;
	}
	bool isFashion()//是时装衣服
	{
		return m_iGoodType == GOOD_TYPE_FASHING;
	}
	bool isWapFashion()//是时装武器
	{
		return m_iGoodType == GOOD_TYPE_WAP_FASHING;
	}
};

#endif //_GOODATTR_H_