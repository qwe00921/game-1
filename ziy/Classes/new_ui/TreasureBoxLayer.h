#ifndef __TREASUREBOX_LAYER__
#define  __TREASUREBOX_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TreasureBoxLayer :public BTLayer
{
public:
	static TreasureBoxLayer* create(int iMapId, EventDelegate* ptrDelegate = NULL,int iBattleType = BATTLE_SIXWAR);
	bool init(EventDelegate* ptrDelegate, int iBattleType);
	TreasureBoxLayer();
	~TreasureBoxLayer();
	void onExit();
	void onEnter();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouches, Event *pEvent);
	void onTouchEnded(Touch *pTouches, Event *pEvent);
	void showPrompt(float f);
	void clickCloseLayer(Ref* sender);
	void updateRTDate(map<int, std::string> mString);

	int mMapId;
	int mPromptTxtIndex;
	int mBattleType;
	Sprite* bg;
	Menu* c_menu;
	map<int, string> mPromptTxt;
	EventDelegate* mDelegate;


};
#endif //(__TREASUREBOX_LAYER__)