//
//  PlayerInfo.h
//  TRGame
//
//  Created by Jimmy on 16/04/26.
//
//

#ifndef _PLAYERINFO_H_
#define _PLAYERINFO_H_

#include "Common.h"

using namespace std;

/************************************************************************/
/* 服务器武将信息数据                                                   */
/************************************************************************/
class HeroElem
{
public:
	HeroElem();
    void loadJson(Json::Value jsonVal);
	CC_SYNTHESIZE(int, m_iHero, HeroID);		//武将ID
	CC_SYNTHESIZE(int, m_iLv, LV);				//等级
	CC_SYNTHESIZE(int, m_iExp, EXP);			//经验值
	CC_SYNTHESIZE(int, m_iJob, JobID);			//职业ID
	CC_SYNTHESIZE(int, m_iFashion1, Fashion1);	//时装1
	CC_SYNTHESIZE(int, m_iFashion2, Fashion2);	//时装2
	CC_SYNTHESIZE(int, m_iGender, Gender);		//性别
	CC_SYNTHESIZE(int, m_iQuality, Quality);	//品质 
	CC_SYNTHESIZE(int, m_iGrade, Grade);		//品阶（绿、蓝、紫、橙、金）
	CC_SYNTHESIZE(int, m_iIndex, eIndex);		   //排序
	CC_SYNTHESIZE(int, m_iSoulBox, eSoulBox);		   //魂匣
	CC_SYNTHESIZE(int, m_iHeroCP, HeroCP);     //战斗力
	map<int, int> mSkillLv;						   //技能等级key 技能;value 等级
	map<int, double> m_mPractice;          //武将修炼的成长值
	map<int, int> mSpecialItem;				//吃的特殊消耗品
};
/************************************************************************/
/* 服务器物品（主要是装备）信息数据										*/
/************************************************************************/
class GoodsElem
{
public:
	GoodsElem();
	void loadJson(Json::Value jsonVal);	
	CC_SYNTHESIZE(int, m_iNetId, NetID);	//网络ID
	CC_SYNTHESIZE(int, m_iGood, GoodID);	//物品ID
	CC_SYNTHESIZE(int, m_iStarLv, StarLV);	//星级
	CC_SYNTHESIZE(int, m_iHero, HeroID);	//持有武将ID
	CC_SYNTHESIZE(int, m_iStone, StoneID); //附魔宝石ID
	map<int, int> m_mProperty;				//特性key 索引0 value ID
};
/************************************************************************/
/* 服务器物品（主要是道具）信息数据										*/
/************************************************************************/
class ConsumeElem
{
public:
	ConsumeElem();
	void loadJson(Json::Value jsonVal);
	CC_SYNTHESIZE(int, m_iNetId, NetID);	//网络ID
	CC_SYNTHESIZE(int, m_iGoodId, GoodID);  //物品ID
	CC_SYNTHESIZE(int, m_iCount, Count);	//库存
};
/************************************************************************/
/* 服务器玩家信息数据													*/
/************************************************************************/
class PlayerInfo
{
public:
	PlayerInfo();
	bool loadJson(Json::Value jsonVal);

	void loadPlayer(Json::Value jsonVal);	//加载用户其他信息
	void loadHero(Json::Value jsonVal);		//加载武将
	void loadGoods(Json::Value jsonVal);	//加载物品
	void loadConsumes(Json::Value jsonVal);	//加载消耗物品
	void loadHeroChip(Json::Value jsonVal);	//加载武将碎片
	void loadGoodsChip(Json::Value jsonVal);//加载装备碎片
	void loadSpecialGoods(Json::Value jsonVal);	//加载特殊道具
	void loadScriptGoods(Json::Value jsonVal);	//加载剧本物品
	void loadMapCity(Json::Value jsonVal);		//加载地图城市的状态
	void loadMapTreasure(Json::Value jsonVal);	//加载地图宝箱领取情况
	void loadMapAttackNum(Json::Value jsonVal);	//加载关卡攻击次数
	void loadScriptCode(Json::Value jsonVal);	//加载全局变量
	void loadGuideStatus(Json::Value jsonVal);//加载用户引导状态信息
	HeroElem* getMainHero();
	void setLeadFashion(map<int, int> eGoodsList);
	int getBuyJadeState(int iPrice); //获取对应金额充值的次数
	void setBuyJadeState(int iPrice);
	void setGuideStatus(const char* key, int v);
	int getGuideStatus(const char* key);
public:
	map<string, int> m_guideStatus;
	map<int, HeroElem*> m_mHero;	//key heroID
	map<int, GoodsElem*> m_mGoods;	//key 索引0
	map<int, ConsumeElem*> m_mConsume;	  //key goodID
	map<int, ConsumeElem*> m_mHeroSoul;	  //key goodID
	map<int, ConsumeElem*> m_mEquipChip;  //key goodID
	map<int, ConsumeElem*> m_mGifts;	  //key goodID
	map<int, ConsumeElem*> m_mScriptGoods;//key goodID

	map<int, map<int,int> > m_mMapAttackNum;
	map<int, map<int, int> >m_mScriptCode;
	map<int, map<int, int> > m_MapTreasure;
	map<int, int >m_mMapCity;

	Json::Value m_valActivity; //活动数据领取状态


	CC_SYNTHESIZE(int, m_iRmb6, Rmb6);
	CC_SYNTHESIZE(int, m_iRmb12, Rmb12);
	CC_SYNTHESIZE(int, m_iRmb30, Rmb30);
	CC_SYNTHESIZE(int, m_iRmb98, Rmb98);
	CC_SYNTHESIZE(int, m_iRmb328, Rmb328);
	CC_SYNTHESIZE(int, m_iRmb648, Rmb648);

	CC_SYNTHESIZE(int, m_iGold, Gold);	
	CC_SYNTHESIZE(int, m_iJade, Jade);	
	CC_SYNTHESIZE(int, m_iImprovedStone, ImprovedStone);
	CC_SYNTHESIZE(int, m_iJadeSeal, JadeSeal);
	

	CC_SYNTHESIZE(int, m_iVIP, VIP);				//会员
	CC_SYNTHESIZE(int, m_iVIPGift, VIPGift);		//会员礼包
	CC_SYNTHESIZE(double, m_dTime, VIPTime);		//会员到期时间	

	CC_SYNTHESIZE(string, m_strName, Name);			//玩家名称
	CC_SYNTHESIZE(int, m_iHead, PlayerHead);		//玩家头像
	CC_SYNTHESIZE(int, m_iVIPExp, VIPExp);			//玩家VIP等级
	CC_SYNTHESIZE(int, m_iVIPNextExp, VIPNextExp);	//玩家VIP下一个等级需要的经验
	CC_SYNTHESIZE(int, m_iLV, PlayerLV);			//玩家等级
	CC_SYNTHESIZE(int, m_iExp, PlayerExp);			//玩家经验
	CC_SYNTHESIZE(int, m_iNextExp, PlayerNextExp);  //玩家下一个等级需要的经验
	CC_SYNTHESIZE(int, m_iHasUnion, HasUnion);		//玩家加入公会
	CC_SYNTHESIZE(int, m_iLastMapID, LastMapID);	//上一次地图ID
	CC_SYNTHESIZE(int, m_iLastWarID, LastWarID);	//上一次战场ID
	CC_SYNTHESIZE(int, m_iLoginNum, LoginNum);		//玩家登录次数
	CC_SYNTHESIZE(int, m_iSweepJadeSum, SweepJadeSum);		//勾玉扫荡次数

	CC_SYNTHESIZE(int,m_LeadFashion,eLeadFashion);			//主角装备时装
	CC_SYNTHESIZE(int, m_LeadWapFashion, eLeadWapFashion);  //主角装备时装武器
	CC_SYNTHESIZE(int, m_iTotalCP, TotalCP);  //综合战斗力

	CC_SYNTHESIZE(Json::UInt, m_roleCreateTime, RoleCreateTime);//角色创建时间
	CC_SYNTHESIZE(int, m_guildId, GuildId);
	CC_SYNTHESIZE(string, m_guildName, GuildName);//玩家所在公会
	CC_SYNTHESIZE(string, m_patriarch, patriarch);//玩家所在公会族长
	
private:
	void reset();//数据重置
};

#endif //_PLAYERINFO_H_