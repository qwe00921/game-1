#ifndef __NETLEVELINFOLAYER_LAYER__
#define __NETLEVELINFOLAYER_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class NetLevelPromptLayer :public BTLayer
{
public:
	CREATE_FUNC(NetLevelPromptLayer);
	bool init();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	NetLevelPromptLayer();
	~NetLevelPromptLayer(){};
};

class NetLevelInfoLayer :public BTLayer
{
public:
	static NetLevelInfoLayer* create(int iIndex, EventDelegate* ptrDelegate);
	bool init();
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchesMoved(Touch *pTouches, Event *pEvent);
	void onTouchesEnded(Touch *pTouches, Event *pEvent);
	NetLevelInfoLayer();
	~NetLevelInfoLayer();

	void clickCloseLayer(Ref* sender);
	void clickBattleM(Ref* sender);
	void clickCkjlMenu(Ref* sender);
	void clickRankList(Ref* sender);
	void clickSweepM(Ref* sender);
	void clickPromptM(Ref* sender);

	void addWarDropGoodsElem(Sprite* ptrBg);
	void getWinCondition();
	void updateShow(int iNomal);
	void updateRTDate(int iNomal, int iType);
	void SendNetDateResult(bool bIsSuccess, Json::Value jsonResult, int iType);
	void updateMenuState();

	int mIndex;
	int mSweepType;
	Menu* mClose;
	Menu* mBattle;
	Menu* mCkjl;
	Menu* mSweep;
	MenuItemSprite* mSweepItem;
	Menu* mPrompt;
	Label* mSurplusTtf;
	Label* mSweepTtf;
	ScriptHandler handler;
	EventDelegate* mDelegate;
};
#endif //(__TRGAME_ACTION_LAYER__)