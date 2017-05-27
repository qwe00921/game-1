#ifndef __TRGAME_SHORTCUTFUNCTION_LAYER__
#define  __TRGAME_SHORTCUTFUNCTION_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum
{
	SHORTCUT_TYPE_JSD,//扫荡卷扫荡
	SHORTCUT_TYPE_GYSD,//勾玉扫荡
	SHORTCUT_TYPE_JSZD,//结束战斗
	SHORTCUT_TYPE_CCZC,//撤出战场
	SHORTCUT_TYPE_JSYX//结束游戏
};

class ShortcutFunctionLayer :public BTLayer
{
public:
	static ShortcutFunctionLayer* create(EventDelegate* ptrDelegate,int iSType,int iDate,int iGoodsNum);
	bool init(EventDelegate* ptrDelegate, int iSType, int iDate, int iGoodsNum);
	void onExit();
	void onEnter();
	ShortcutFunctionLayer();
	~ShortcutFunctionLayer();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);

	void ClickCloseMenu(Ref* sender);
	void ClickLeftMenu(Ref* sender);
	void ClickRightMenu(Ref* sender);

private:
	EventDelegate* mDelegate;
	int mSType;
	int mDateNum;
	int mGoodsNum;

	Menu* mMenuClose;
	Menu* mMenuLeft;
	Menu* mMenuRight;
};
#endif //(__TRGAME_SHORTCUTFUNCTION_LAYER__)