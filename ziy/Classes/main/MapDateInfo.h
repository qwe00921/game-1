#ifndef __TRGAME_MAPDATEINFO_
#define __TRGAME_MAPDATEINFO_

#include "Common.h"
#include "jsons.h"
#include "cocos2d.h"
#include "EventDelegate.h"

using namespace Json;
using namespace std;

class BattleWolfCity:public EventDelegate
{
public:
	BattleWolfCity();
	~BattleWolfCity();

	void SetWolfBattleData(int iWarId);
	int GetWolfBattleState(int iWarid);

	void SetWolfTreasures(int iSatr,int iCount);
	map<int,int> GetWolfTreasures();

	void SetWolfGuardData(int iWarId,map<int,int> m_guard);
	map<int,int> GetWolfGuardData(int iWarid);

	void LoadJsonValue(Json::Value jsonData);
	map<int,int> WolfIsOpen(int iWarid);

	map<int, int> GetWolfMapWarList();
	int getWolfCityWinGq();

	map<int, int> getAllGuardActor();

	void getRewardGoods(Json::Value jsonData);
	void SetPopLayerElement(Sprite* bgSp);
	void RunFunction(int iNomal);

private:
	map<int, int> mBattleData;//过关数据
	map<int, int> mTreasures;//宝箱数据
	map<int, map<int, int> > mGuard;//守将数据
	map<int, int> mMapWolfWarList;//高狼城关卡,warid下标
	map<int, int> mAllGuardActor;//所有镇守武将
	map<int, map<int,int> > mAddReward;//镇守奖励
};

class BattleImageWar
{
public:
	BattleImageWar();
	~BattleImageWar();

	int GetEnemyImageId(int iEnemyId);
	void ClearActorList();
	void SetImageActorList(map<int, int> mActorList);

	void SetBattleData(int iWarId);
	int GetBattleState(int iWarid);
	map<int, int> GetAllBattleData();

	void SetTreasures(int iSatr, int iCount);
	map<int, int> GetTreasures();
	void LoadJsonValue(Json::Value jsonData);

	int GetCurBattleWar(int iStage);
	int GetXMTWarStar(int iWarId, int iStage);
	int GetBattleStar(int iWarId,int iStage);
	int GetStageNum();
private:
	map<int, int> mActorList;//出战武将

	map<int, int> mBattleData;//过关数据
	map<int, int> mTreasures;//宝箱数据

	map<int, map<int, int> > eAllDegreeWarList;
};

class BattleGreatWar
{
public:
	BattleGreatWar();
	~BattleGreatWar();

	void SetBattleData(int iWarId);
	int GetBattleState(int iWarid);
	map<int, int> GetAllBattleData();

	bool GreatBattleIsOpen();

	void LoadJsonValue(Json::Value jsonData);
private:
	map<int, int> mBattleData;//过关数据
	bool m_bIsOpen;
};

class MapDateElem
{
public:
	void loadJson(Json::Value jsonVal);
	MapDateElem()
	{
		mMapId = 0;
		mWarId = 0;
		mStar = 0;
		mScore = 0;
		mAttackNum = 0;
		mRefNum = 0;
	}

	CC_SYNTHESIZE(int,mMapId,MAPID);
	CC_SYNTHESIZE(int, mWarId, WARID);
	CC_SYNTHESIZE(int, mStar, STAR);
	CC_SYNTHESIZE(int, mScore, SCORE);
	CC_SYNTHESIZE(int, mAttackNum, ATTACKNUM);
	CC_SYNTHESIZE(int, mRefNum, RefNum);
};

class MapDateInfo
{
public:
	MapDateInfo();
	~MapDateInfo();
	void loadJson(Json::Value jsonVal);
	int getMaxWarId(int iModel = NORMAL_MODEL);
	int getNextWarId(int iModel = NORMAL_MODEL); //获取MaxWarId的下一个
	int getCurWarId(int iModel = NORMAL_MODEL);
	void setCurWarId(int iWarId);
	MapDateElem* getMapDateElem(int iWarId);
	MapDateElem* getHeroicMapDateElem(int iWarId);
	void AddBattleWinDate(int iMapid, int iWarid, int iStar, int iScore,int iBattleNum = 1);
	void AddHeroicBattleWinDate(int iWarid,int iBattleNum = 1);
	void AddHeroicBattleRefNum(int iWarid, int iBattleNum = 1);

	map<int, int> getMapTreasure(int iMapId);
	void addMapTreasure(int iMapId,int iStar,int iCount);
	void setMapTreasure(map<int, map<int, int> > mMapTrea);
	int getMapTotalStar(int iMapId);
	int getRomanceWinGq(int iMapId);
	BattleWolfCity* GetBattleWolfData();
	BattleImageWar* GetBattleImageData();
	BattleGreatWar* GetBattleGreatData();
	bool isPassed(int iWarId); //是否已通关，用于判断是否开启扫荡功能
private:
	void clearMapDate();
	void setMaxWarId(int iWarId);
	map<int, MapDateElem*> m_MapInfo;
	map<int, MapDateElem*> m_HeroicMapInfo;
	map<int, map<int, int> > m_MapTreasure;
	int mMaxWarid;
	int mCurWarid;
	int mJyMaxWarid;
	int mJyCurWarid;
	BattleWolfCity mBattleWolfCity;
	BattleImageWar mBattleImageWar;
	BattleGreatWar mBattleGreatWar;
};

class ArmyFuBenBattle
{
public:
	ArmyFuBenBattle();
	~ArmyFuBenBattle();

	void ClearBossGainHp();
	void SetEndBossGainHp(int iWarid);
	int GetBossGainHp(int iBossId);
	int GetAllEnemyGainHp();
private:
	void SetBossGainHp(int iBossId, int iSet);
	map<int, map<int, int> > mClanBossGainHp;
};
#endif /* __TRGAME_MAPDATEINFO_ */
