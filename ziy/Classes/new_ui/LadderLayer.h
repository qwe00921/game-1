#ifndef __TRGAME_LADDER_LAYER__
#define  __TRGAME_LADDER_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LadderShowElemData
{
	CC_SYNTHESIZE(int,iWarId,eWarId);
	CC_SYNTHESIZE(bool,bIsOpen,eIsOpen);
	CC_SYNTHESIZE(bool, bPassBattle, ePassBattle);
	CC_SYNTHESIZE(string,NameS,eNameS);
	CC_SYNTHESIZE(int,iGold,eGold);
	CC_SYNTHESIZE(int, iJade, eJade);
};

class LadderShowElem
{
	CC_SYNTHESIZE(Label*, DayLab, eDayLab);
	CC_SYNTHESIZE(Label*, Name1Lab, eName1Lab);
	CC_SYNTHESIZE(Label*, Name2Lab, eName2Lab);
	CC_SYNTHESIZE(Sprite*, Elem1BgSp, eElem1BgSp);
	CC_SYNTHESIZE(Sprite*, Elem2BgSp, eElem2BgSp);
	CC_SYNTHESIZE(Sprite*, Win1Sp, eWin1Sp);
	CC_SYNTHESIZE(Sprite*, Win2Sp, eWin2Sp);
	CC_SYNTHESIZE(Sprite*, Box1Sp, eBox1Sp);
	CC_SYNTHESIZE(Sprite*, Box2Sp, eBox2Sp);
};

class LadderLayer :public BTLayer
{
public:
	CREATE_FUNC(LadderLayer);
	bool init();
	LadderLayer();
	~LadderLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void cliskCloseMenu(Ref* sender);
	void clickProMenu(Ref* sender);
	void clickNextMenu(Ref* sender);
	void clickRankMenu(Ref* sender);
	void clickExchMenu(Ref* sender);

	void updataElemShow();

	void GetElemData();

	void ChildCloseLayer(int iNomal);

	void SetPopLayerElement(Sprite* ptrBg);
private:
	Sprite* mCurDocSp;
	int mClickBoxIndex;
	int mCurDayIndex;
	int mMaxDayIndex;

	LadderShowElem mLadderShowElem;

	map<int, Sprite*> mSpBoxList;
	map<int, Sprite*> mElemDoc;
	map<int, LadderShowElemData*> mLadderElemData;
	map<int, int> mBoxCanClick;
	Menu* proMenu;
	Menu* NextMenu;
};
#endif //(__TRGAME_ACTION_LAYER__)