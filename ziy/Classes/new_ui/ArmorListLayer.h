#ifndef __TRGAME_ARMORLISTLAYER__
#define  __TRGAME_ARMORLISTLAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ArmorListLayer :public BTLayer
{
public:
	CREATE_FUNC(ArmorListLayer);
	bool init();
	ArmorListLayer();
	~ArmorListLayer();
	void onExit();
	void onEnter();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void clickCloseLayer(Ref* sender);
	void RunFunction(int iNomal);
	void updateMoneyShow();
	void RemoveAllParentLayer();
public:
	EventDelegate* mDelegate;
	Label* wjs_num;
	Menu* mCloseMenu;
};
#endif //(__TRGAME_DISPATCH_LAYER__)