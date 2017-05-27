#ifndef __TRGAME_BATTLELADDER_
#define __TRGAME_BATTLELADDER_

#include <stdio.h>
#include "Common.h"
#include "main.h"
#include "PlayerInfo.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum
{
	TYPE_ACTOR_NAME = 1,//武将名
	TYPE_PLAYER_NAME = 2,//玩家名
};

class LadderElemData
{
public:
	LadderElemData()
	{
		mName = "";
	}
	~LadderElemData()
	{
		for (map<int, HeroElem*>::iterator iIt = mHeroElem.begin(); iIt != mHeroElem.end(); iIt++)
		{
			delete iIt->second;
		}
		for (map<int, GoodsElem*>::iterator iIt = mGoodsItem.begin(); iIt != mGoodsItem.end(); iIt++)
		{
			delete iIt->second;
		}
		mHeroElem.clear();
		mGoodsItem.clear();
	}
public:
	map<int, HeroElem*> mHeroElem;
	map<int, GoodsElem*> mGoodsItem;
	string mName;
};

class BattleLadder:public Ref,EventDelegate
{
public:
	static BattleLadder* getInstance();
	~BattleLadder();
	void ClearAllData();
	void ClearHeroData();

	void SendNetData();//发送请求
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);

	void LoadBattleLadderData(Json::Value jsonValue);//加载一骑当关关卡敌军数据
	void SetLadderLevelData(int iWarId);//设置一骑当关关卡过关数据

	map<int, int> GetLadderLevelData();//获得过关数据
	map<int, int> GetLadderOpenWarData();//获得过关开启数据
	map<int, map<int, LadderElemData*> >* GetLadderHeroData();//活儿敌军信息

	bool isAllStar(int iWarId);//是不是全明星
	HeroElem* getIndexHeroElem(int iWarId,int iIndex,int& iElemIndex);
	string GetBattleActorName(int iActorId,int iType);//type 1:武将名 2：玩家名
	void SetBattleActorName(int iActorId, string NameS, int iType);//type 1:武将名 2：玩家名
	void ClearBattleActorName();
private:
	BattleLadder();

private:
	static BattleLadder* m_instance;

	int m_iCurWarId;

	map<int, int> mBattleData;
	map<int, int> mOpenWar;
	map<int, map<int, LadderElemData*> > m_mHeroData;
	map<int, map<int, string> > m_mActorName;
};
#endif /* __TRGAME_BATTLELADDER_ */
