#ifndef __TRGAME_HEROICAMBITION_LAYER__
#define  __TRGAME_HEROICAMBITION_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class HeroicDateElem
{
	CC_SYNTHESIZE(int, mWarId, WarId);
	CC_SYNTHESIZE(int, mIsOpen, IsOpen);
	CC_SYNTHESIZE(int, mOpenLevel, OpenLevel);
	CC_SYNTHESIZE(int,mMapId,MapId);
	CC_SYNTHESIZE(int, mCurNum, CurNum);
	CC_SYNTHESIZE(int, mMaxNum, MaxNum);
	CC_SYNTHESIZE(int, mIsPass, IsPass);
public:
	map<int, map<int, int> >GoodsList;
	HeroicDateElem()
	{
		mWarId = 0;
		mIsOpen = 0;
		mOpenLevel = 0;
		mMapId = 0;
		mCurNum = 0;
		mMaxNum = 0;
		mIsPass = 0;
	}
};
typedef HeroicDateElem* HeroicDateElemPtr;

class HeroicShowElem
{
	CC_SYNTHESIZE(int,mWarId,WarId);
	CC_SYNTHESIZE(int, mDateIndex, DateIndex);
	CC_SYNTHESIZE(Sprite*,mBg,Bg);

	CC_SYNTHESIZE(Sprite*, mPromptBg, PromptBg);
	CC_SYNTHESIZE(Label*, mOpenPrompt, OpenPrompt);

	CC_SYNTHESIZE(Sprite*, mActorIcon, ActorIcon);
	CC_SYNTHESIZE(Sprite*, mActorName, ActorName);
	CC_SYNTHESIZE(Sprite*, mNameBg, NameBg);
	CC_SYNTHESIZE(Sprite*, mPromptImg, PromptImg);

	CC_SYNTHESIZE(Sprite*, mBarrierBg, BarrierBg);
	CC_SYNTHESIZE(Label*, mSurplusNum, SurplusNum);
	CC_SYNTHESIZE(Menu*, mAddNum, AddNum);
	CC_SYNTHESIZE(Sprite*, mGoodsBg1, GoodsBg1);
	CC_SYNTHESIZE(Sprite*, mGoodsBg2, GoodsBg2);
	CC_SYNTHESIZE(Sprite*, mGoodsBg3, GoodsBg3);
};
typedef HeroicShowElem* HeroicShowElemPtr;

class HeroicAmbitionLayer :public BTLayer
{
public:
	static HeroicAmbitionLayer* create(int iCurWarId=-1);
	bool init(int iCurWarId);
	void onExit();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void updateShow(int index);
	void updateMoneyShow();
	void GetDateElem();
	void GetShowElem();
	void ShowAction(bool bIsNext);
	void UpdateNextHeroicShow(int iDateEIndex,int iShowEIndex);
	void AddGoodsElem(Sprite* GoodsBg,int iGoodsId,int iCount);
	void ChildCloseLayer(int iNomal);
	void setEnableClick(float f);
	void updateRTDate(int iNomal,int iType);
	void SendNetDateResult(bool bIsSuccess, Json::Value jsonResult,int iType);
	void SetPopLayerElement(Sprite* ptrBg);
	void RunFunction(int iNomal);

	void clickMenuBack(Ref* sender);
	void clickAddNumMenu(Ref* sender);
	void clickDispatchMenu(Ref* sender);
	void clickSweepMenu(Ref* sender);
	void clickOrdinaryMenu(Ref* sender);
	void clickDifficultyMenu(Ref* sender);

	HeroicAmbitionLayer();
	~HeroicAmbitionLayer();
private:
	void updateMenuState();
private:
	int mShowElemIndex;
	int mDateElemIndex;
	bool mEnableClick;
	int mSweepType;
	int mCurWarId;
	bool mEnableMove;

	DSize mSize;
	Sprite* bg;
	map<int, HeroicDateElemPtr> mDateElem;
	map<int, HeroicShowElemPtr> mShowElem;
	MenuItemSprite* SweepItem;
	MenuItemSprite* DispatchItem;
	Label* SweepPrompt;
};
#endif //(__TRGAME_HEROICAMBITION_LAYER__)