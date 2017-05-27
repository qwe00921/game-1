#ifndef __TRGAME_BATTLEWINCONDITIONLAYER__
#define  __TRGAME_BATTLEWINCONDITIONLAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "BattleScreen.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BattleWinConditionLayer :public BTLayer
{
public:
	static BattleWinConditionLayer* create(BattleScreenPtr ptrBs, bool bOpenSchedule = true);
	bool init(BattleScreenPtr ptrBs, bool bOpenSchedule);
	void onExit();
	BattleWinConditionLayer();
	~BattleWinConditionLayer();
	void RemoveThis(float f);

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	BattleScreenPtr m_pBattleScreen;
	bool m_bOpenSchedule;
};
#endif //(__TRGAME_ACTION_LAYER__)