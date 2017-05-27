//
//  PVPData.h
//  TRGame
//
//  Created by Jimmy on 17/03/21.
//
//

#ifndef _PVPData_H_
#define  _PVPData_H_

#include "Defines.h"

enum
{
	ARRAY_ATTACK = 1,
	ARRAY_DEFEND = 2,
	ARRAY_ALL = 3,
};

class PVPArrayItem
{
public:
	void loadJson(Json::Value jsonVal);
	Json::Value saveJson();

	CC_SYNTHESIZE(int, iPointId, PointID); //位置ID
	CC_SYNTHESIZE(int, iOrderId, OrderID); //出战顺序
	CC_SYNTHESIZE(int, iActorId, ActorID); //武将ID
};

class PVPArrayInfo
{
public:
	PVPArrayInfo();
	~PVPArrayInfo();
	void loadJson(Json::Value jsonVal);
	Json::Value saveJson();
	void updateArray(PVPArrayInfo* ptrArray);
	void clearArray();

	CC_SYNTHESIZE(bool, bIsInit, IsInit); //是否初始化
	CC_SYNTHESIZE(int, iArrayId, ArrayID); //阵形ID
	map<int, PVPArrayItem*> m_mArray;
};

class PVPActorData
{
public:
	PVPActorData();
	~PVPActorData();
	void loadJson(Json::Value jsonVal);
	void initFashion();
	void initHeroList();
	void initGoodsList();
	void clearData();

	CC_SYNTHESIZE(int, m_iGender, Gender); //性别
	CC_SYNTHESIZE(int, m_iFashion1, Fashion1); //时装形象
	CC_SYNTHESIZE(int, m_iFashion2, Fashion2); //武器造型
	CC_SYNTHESIZE(int, m_iRank, RankID); //排名
	CC_SYNTHESIZE(int, m_iWinNum, WinNum); //成功次数
	CC_SYNTHESIZE(int, m_iLoseNum, LoseNum); //失败次数
	CC_SYNTHESIZE(bool, m_bIsVIP, IsVIP); //王族
	CC_SYNTHESIZE(string, m_strName, Name); //主角名

	PVPArrayInfo* m_pDefend; //防守阵容
	map<int, HeroElem*> m_mHero;
	map<int, GoodsElem*> m_mGoods;
	map<int, int> m_mMirrorId; //武将镜像ID
};

class PVPData : public Ref
{
public:
	static PVPData* getInstance();
	~PVPData();
	void init();
	void startPVP();
	void updateAtcJson(PVPArrayInfo* ptrArray);
	void loadJson(Json::Value jsonVal);
	void clearData();
	int getPVPWarId();
private:
	PVPData();
	void loadAtkJson();

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onInitDataSucceed(Json::Value valRes);
public:
	map<int, PVPArrayItem*> m_mAttack;
	map<int, PVPArrayItem*> m_mDefend;
private:
	static PVPData* m_pInstance;

	CC_SYNTHESIZE(int, m_iRankId, RandID); //排名
	CC_SYNTHESIZE(int, m_iEntry, EntryTimes); //进入次数
	CC_SYNTHESIZE(int, m_iMax, MaxTimes); //最大次数
	CC_SYNTHESIZE(int, m_iMapId, MapID); //地图ID
	CC_SYNTHESIZE(int, m_iSelectRank, SelectRank); //挑战ID
	CC_SYNTHESIZE(int, m_iMidIndex, MidIndex); //中线位置，用于计算敌军位置
	CC_SYNTHESIZE(string, m_strMark, MarkID); //战场副本验证

	map<int, PVPArrayInfo*> m_mArrayInfo;
	map<int, PVPActorData*> m_mPVPActor;
	map<int, Json::Value> m_mJsAttack;
};

#endif // _PVPData_H_