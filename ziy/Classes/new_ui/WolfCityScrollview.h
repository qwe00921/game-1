#ifndef __TRGAME_WOLFCITY_SCROLLVIEW__
#define  __TRGAME_WOLFCITY_SCROLLVIEW__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class WolfCityScrollview :public BTLayer, public ScrollViewDelegate
{
public:
	static WolfCityScrollview* create(EventDelegate* ptrDelegate);
	bool init(EventDelegate* ptrDelegate);
	WolfCityScrollview();
	~WolfCityScrollview();

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
	map<int, map<int,int> > mElemData;
	Sprite* mRightArrowSp;
	Sprite* mLeftArrowSp;
};

class WolfCityChestShow
{
	CC_SYNTHESIZE(Sprite*,bgSp,eBGSP);
	CC_SYNTHESIZE(Label*,gqName,eGQNAME);
	CC_SYNTHESIZE(bool,bIsReceive,eISRECEIVE);
	CC_SYNTHESIZE(int,iStarNum,eStarNum);
	CC_SYNTHESIZE(int,iMapId,eMAPID);
	CC_SYNTHESIZE(bool,bEnableReceive,eENABLERECEIVE);
};

class WolfCityShowElem
{
	CC_SYNTHESIZE(int,iWarId,eWarId);
	CC_SYNTHESIZE(bool, bIsBattled, eBISBATTLED);
	CC_SYNTHESIZE(bool,bIsGurad,eBISGURAD);
	CC_SYNTHESIZE(Sprite*,BgSp,eBgSp);
	CC_SYNTHESIZE(Sprite*, PassSp, ePassSp);
};

class WolfCityScrollviewLayer :public BTLayer
{
public:
	static WolfCityScrollviewLayer* create(map<int, int> ElemData,EventDelegate* ptrDelegate);
	bool init(map<int, int> ElemData, EventDelegate* ptrDelegate);
	WolfCityScrollviewLayer();
	~WolfCityScrollviewLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void ChildCloseLayer(int iNomal);

	void SetPopLayerElement(Sprite* bgSp);
	void RunFunction(int iNomal);
	void setEnableClick(float f);
	void updateRTDate(int iNomal);


	void SendNetDate(int iMapId,int iStar);
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void setNetDate(Json::Value jsonValue);
	void showPrompt(float f);

	void UpdataChestShow();
private:
	int mClickIndex;
	int mClickChestindex;
	int mBattleMapId;
	bool mEnableClick;

	Vec2 mMoveStartPoint;
	map<int, int> mElemData;
	EventDelegate* mDelegate;
	map<int, WolfCityShowElem*> mShowElem;
	map<int, WolfCityChestShow*> mChestElem;

	int mPromptTxtIndex;
	map<int, string> mPromptTxt;
	int mReceiveChestIndex;
	BattleWolfCity* mWolfCity;
};

#endif //(__TRGAME_WOLFCITY_SCROLLVIEW__)