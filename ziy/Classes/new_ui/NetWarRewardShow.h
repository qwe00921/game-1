#ifndef __NETWARREWARDSHOW_LAYER__
#define  __NETWARREWARDSHOW_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class NetWarRewardShow :public BTLayer
{
public:
	static NetWarRewardShow* create(int iWarid,EventDelegate* ptrDelegate);
	bool init(int iWarid,EventDelegate* ptrDelegate);

	void onExit();
	void onEnter();
	NetWarRewardShow();
	~NetWarRewardShow();

	void clickCloseMenu(Ref* sender);
	void UpdataStageShow();
	void clickProMenu(Ref* sender);
	void clickNextMenu(Ref* sender);
	void clickDispatchMenu(Ref* sender);
private:
	int mWarId;
	int mBattleStage;
	int mBattleMaxStage;
	EventDelegate* mDelegate;
	map<int, Sprite*> GoodsBgSp;
	Sprite* bg;
	Label* mEnemyLevel;
	Label* mStageGoodsT;
};
#endif //(__TRGAME_ACTION_LAYER__)