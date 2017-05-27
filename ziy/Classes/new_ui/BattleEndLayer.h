#ifndef __TRGAME_BattleEndLayer__
#define  __TRGAME_BattleEndLayer__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BattleEndLayer :public BTLayer
{
public:
	static BattleEndLayer* create(bool is_fail = false);
	bool init(bool is_fail);
	BattleEndLayer();
	~BattleEndLayer();
	void SetWinShow();
	void SetFailShow();
	void AddActorElem();
	void AddGoodsElem();
	void clickBackMenu(Ref* sender);
	void clickGetStarMenu(Ref* sender);

	int get_star_num;
	int reach_conditions_num;
private:
	void InitSendDate();
	void SendWinDate();
	void SendClanFBWinDate();
	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void* resultdata);
	void onPVPResultSucceed(Json::Value valRes);
	void LoadBattleWinDate(Json::Value JsonDate);
	void StarActionCallF(Node* sender);
	void AddOneGoods(Json::Value JsonDate);

	Json::Value JsonSendDate;
	float mScore;
	map<int, int> mActorLvUp;
	bool mEnableGetExp;
	bool mEnableGetGold;
	map<int, int> mDropGoods;
	bool m_bIsFail;
};
#endif //(__TRGAME_BattleEndLayer__)