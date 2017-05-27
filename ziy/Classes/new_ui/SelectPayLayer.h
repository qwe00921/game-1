#ifndef __TRGAME_SELECTPAY_LAYER__
#define  __TRGAME_SELECTPAY_LAYER__

#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class SelectPayLayer :public BTLayer
{
public:
	static SelectPayLayer* create(int buyId, string chargeId, EventDelegate* ptrDelegate);
	bool init(int buyId, string chargeId,EventDelegate* ptrDelegate);
	SelectPayLayer();
	~SelectPayLayer();
	void onExit();
	void onEnter();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void clickZFBMenu(Ref* sender);
	void clickWXMenu(Ref* sender);
	void ClickCloseMenu(Ref* sender);

	void UpdateDate();

public:
	Label* order;
	Label* price;
	string mChargeId;
	int mBuyId;
	EventDelegate* mDelegate;
	Menu* zfb_menu;
	Menu* wx_menu;
	Menu* close_M;
};
#endif //(__TRGAME_SELECTPAY_LAYER__)