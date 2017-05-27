#ifndef __TRGAME_CLAN_ARMY_BATTLE_LAYER__
#define  __TRGAME_CLAN_ARMY_BATTLE_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClanArmyBattleElem
{
public:
	ClanArmyBattleElem()
	{
		iIndex = -1;
		iWarId = 0;
		iBossMaxHp = 1;
		iSelfJob = 0;
	}
	map<int,int> mGoodsList;
	CC_SYNTHESIZE(int, iIndex, EiIndex);
	CC_SYNTHESIZE(int,iWarId,EiWarId);
	CC_SYNTHESIZE(int, iIsOpen, EiIsOpen);
	CC_SYNTHESIZE(int, iBossCurHp, EiBossCurHp);
	CC_SYNTHESIZE(int, iBossMaxHp, EiBossMaxHp);
	CC_SYNTHESIZE(int, iBgImg, EiBgImg);
	CC_SYNTHESIZE(int, iSelfJob, EiSelfJob);
};

class ClanArmyBattleLayer :public BTLayer
{
public:
	static ClanArmyBattleLayer* create(Json::Value vArmyData,EventDelegate* ptrDelegate,int iSelfJob,string guildIdS);
	bool init(Json::Value vArmyData,EventDelegate* ptrDelegate,int iSelfJob, string guildIdS);
	ClanArmyBattleLayer();
	~ClanArmyBattleLayer();

	void SetPopLayerElement(Sprite* bgSp);

private:
	void GetBattleData(Json::Value vArmyData);
	void clickCloseMenu(Ref* sender);
	void updateShow(int iNomal);

	map<int, ClanArmyBattleElem*> mArmyBattleData;
	int mSelfJob;
	EventDelegate* mDelegate;

	bool m_bEnableUpdataFatherShow;

	map<int, map<int, int> > mAddReward;
	map<int, int> mGoodsIndex;
	string mGuildIdS;

};
#endif //(__TRGAME_CLAN_ARMY_BATTLE_LAYER__)