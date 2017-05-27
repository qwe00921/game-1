#ifndef __GREAT_BATTLE_SCROLLVIEW__
#define  __GREAT_BATTLE_SCROLLVIEW__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class GreatBattleScrollview :public BTLayer, public ScrollViewDelegate
{
public:
	static GreatBattleScrollview* create(EventDelegate* ptrDelegate);
	bool init(EventDelegate* ptrDelegate);
	GreatBattleScrollview();
	~GreatBattleScrollview();

	void scrollViewDidScroll(ScrollView* view);
	void scrollViewDidZoom(ScrollView* view);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

private:
	void InitDateList();
	void setArroeState();

	ScrollView* mScrollView;

	int m_iCurPage;
	int m_iMaxPage;
	int mViewWidth;
	int mViewHeight;
	bool m_bEnableMove;
	Vec2 m_vStarPoint;
	EventDelegate* mDelegate;
	map<int, int> mElemData;
	Sprite* mRightArrowSp;
	Sprite* mLeftArrowSp;
};


class GreatBattleScrollviewLayer :public BTLayer
{
public:
	static GreatBattleScrollviewLayer* create(int iGreatId,EventDelegate* ptrDelegate);
	bool init(int iGreatId, EventDelegate* ptrDelegate);
	GreatBattleScrollviewLayer();
	~GreatBattleScrollviewLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void ClickAttackMenu(Ref* sender);
private:
	int m_iGreatId;
	EventDelegate* mDelegate;
};

#endif //(__GREAT_BATTLE_SCROLLVIEW__)