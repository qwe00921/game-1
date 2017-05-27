#ifndef _HISTORYGOOD_H_
#define _HISTORYGOOD_H_

#include "PlayerInfo.h"

enum
{
	A_POWER = 0,        //武力
	A_DEFENCE = 1,      //统帅
	A_INTELLEGENCE =2,	//智力
	A_EXPLOSION = 3,    //敏捷
	A_FORTURN = 4,		//运气
	A_SPEED = 5,		//移动
	A_UPHP = 6,         //HP
	A_UPMP = 7          //MP
};

class HistoryGoods
{
public:
	HistoryGoods();
	HistoryGoods(int iGoodId);
	HistoryGoods(int iGoodId, int level);
	HistoryGoods(HistoryGoodsPtr ptrGood);
	HistoryGoods(GoodsElem* ptrGoods);
	void setGood(HistoryGoodsPtr good);
	void setGood(GoodsElem* ptrGoods);
	void init(int iGoodId, int level);
	
	void setGoodsPower(int iSet = -1, int iAdd = 0);//物品初始基础属性

	bool isSpecial();		//物品是否有特性
	bool isNormalGood();	//物品是否存在
	int getInductNum();		//物品特性类型109或者29的效果范围数量
	int GetGoodsPower();	//物品基础属性power（加密）
	GoodsItem* getGoodAttr();	//通过good_id获取物品（trData->m_mGoodsItem）
public:
	int GetGoodsType();			//获取物品种类，消耗品、武器、防具、辅助<==>GOOD_TYPE_WEAPON GOOD_TYPE_COAT 等
	int GetGoodsAbilityType();	//获取物品加成效果，攻击力、精神力、防御力=附带属性GOODS_POWER GOODS_INTEL GOODS_DEFENCE等
	char* GetAttributeInfo();	//获取物品类别名称
	char* GetSpecialEffectInfo(); //获取物品第一个特殊效果文字
	char* GetGoodsName(int iNum = 1); //获取物品的名字，大于1则返回名字 x Num
	char* GetGoodsAbilityInfo(); //获取物品加成效果文字，如攻击力+40

	char* GetGoodsLevel(); //获取物品等级文字，辅助则显示"--"
	int GetGoodsPrice(); //获取物品的金币价格
	short GetRealLevel(); //获取物品真实等级(另加特性加成)
	void setGoodAddLevel(short goodAddLevel);
	int getGoodsSeType();//获得物品强化type
	int GetOrSetQuality(int iAdd, bool is_get);
	std::map<int, int> GetStrengEffect();
	void AddStrengEffect(int iEffect);
	void EraseStrengEffect(int index);
	void UpdateStrengEffect(int index,int iNum);
	std::map<int,int> getGoodsAddPower();//1攻击力，2防御力，3精神力，4爆发，5士气，6移动
	void setStrAddEffect();

	char* getSpecialEffectName();//
	char* getSpecialEffectInfo();
	int getReLevelUpExp();
	int* getStrAddAbility();//获得强化增加的五维
	int* getStrengEffectIndex();//获得强化字典索引表
	int getStoreNum(); //获取物品的库存
	bool isTreasure(); //判断是不是宝物
	int getImageId();
	int getItId();
	int getUeId();
	int getSeSeid();
	int getPrice();
	int getUpVal();
	int getMaxStar();
	int getSpeAttrNum(); //获取特殊属性的个数
	map<int, int> getSeid();
	string getInfo();
	string getAttributeText();
	string getAllStrengEffextInfo();
	int getStarAddVal(int iQuality = -1); //获取星级的属性加成，不传参默认取物品的当前星级
	int getStrengNeedMoney(int iQuality = -1);//强化到下一级别所需金币
	int getStrengNeedStore(int iQuality = -1);//强化到下一级别所需强化石
	bool isConsume();
	bool isForgeEnable(); //是否支持锻造
	bool isEquipEnable(int iActorId); //检测武将是否可以装备
	
	bool isSoul(); //是否是武魂
	bool isPiece(); //是否是碎片
	bool isExpFruit(); //是否是经验果
	bool isGift(); //是否是礼包
	bool isAssist(); //是否是辅助物品
	bool isPreWarConsume(); //是否是战前消耗品 itid 998
	bool isPlotGoods();//是否剧情物品 itid 997
	bool isSuit();//是否套装
	bool isHero();//是否是武将
	bool isFashion();//是否时装
	bool isEquip(); //是否是装备，包括武器防具和辅助品
	bool isNewGoods();//是否是新装备
	bool isGetEnable();//是否还可以获得
	bool isOnly();//是否具有唯一性
	bool isFmEnable(); //是否可以附魔
	bool EnableEquip(int iActorId);//
	bool isAddAbilityGoods();//是否是永久增加属性物品

	map<int, int> getFmGoodsMap(); //获取该物品可以附魔的宝石集合
public:
	unsigned short good_id; //物品ID
	unsigned short level;	//物品等级
	unsigned short exp;		//物品经验
	unsigned short special_power;
	int iQuality;			//物品品质
	int equipment_actorId;	//使用武将ID
	bool is_press;
	bool iNewGoods;//新增加的物品
	std::map<int, int> StrengEffect;	//强化属性ID(GoodsElem-m_mProperty)
	CC_SYNTHESIZE(int, m_iNetId, NetID); //网络ID
	CC_SYNTHESIZE(int, m_iStone, StoneID); //附魔宝石ID
private:
	GoodsItem* m_pGoods; 
	int power;
	int m_EncryptKey;

	short goodAddLevel;			
	short reLevelUpExp;			//升级减少经验百分比
	int StrAddAbility[6];		//强化增加的五维和移动
	int StrengEffectIndex[10];	//强化特性的索引表
};

#endif //_HISTORYGOOD_H_