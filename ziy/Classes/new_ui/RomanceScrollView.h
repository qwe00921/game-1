#ifndef __TRGAME_ROMANCE_SCROLLVIEW__
#define  __TRGAME_ROMANCE_SCROLLVIEW__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class RomanceScrollview :public BTLayer, public ScrollViewDelegate
{
public:
	static RomanceScrollview* create(EventDelegate* ptrDelegate);
	bool init(EventDelegate* ptrDelegate);
	RomanceScrollview();
	~RomanceScrollview();

	void scrollViewDidScroll(ScrollView* view);
	void scrollViewDidZoom(ScrollView* view);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

private:
	void InitDateList();
	map<int, int> GetElemList(int iPage);
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

class RomanceShowElem
{
	CC_SYNTHESIZE(int,iMapId,EMapId);
	CC_SYNTHESIZE(bool, bIsOpen, EIsOpen);
	CC_SYNTHESIZE(Sprite*,BgSp,EBgSp);
	CC_SYNTHESIZE(Sprite*,LockSp,ELockSp);
	CC_SYNTHESIZE(Sprite*,PriceBg,EPricebg);
};

class RomanceScrollviewLayer :public BTLayer
{
public:
	static RomanceScrollviewLayer* create(map<int, int> ElemData,EventDelegate* ptrDelegate);
	bool init(map<int, int> ElemData, EventDelegate* ptrDelegate);
	RomanceScrollviewLayer();
	~RomanceScrollviewLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

private:
	int mClickIndex;

	Vec2 mMoveStartPoint;
	map<int, int> mElemData;
	EventDelegate* mDelegate;
	map<int, RomanceShowElem*> mShowElem;
};

#endif //(__TRGAME_ROMANCE_SCROLLVIEW__)