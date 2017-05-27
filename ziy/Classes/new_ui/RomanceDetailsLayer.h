#ifndef __TRGAME_ROMANCEDETAILS_LAYER__
#define  __TRGAME_ROMANCEDETAILS_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class RomanceDetailsInfo
{
	CC_SYNTHESIZE(int,iWarID,EWarId);
	CC_SYNTHESIZE(bool,bIsOpen,EIsOpen);
	CC_SYNTHESIZE(Sprite*,BgSp,EBgSp);
	CC_SYNTHESIZE(Sprite*, NameBgSp, ENameBgSp);
	CC_SYNTHESIZE(Sprite*,LockSp,ELockSp);
	CC_SYNTHESIZE(Label*,NameLab,EName);
};

class RomanceDetailsLayer :public BTLayer
{
public:
	static RomanceDetailsLayer* create(int iMapId,bool bEnablePlayMp3 = true,bool bShowTitle = false);
	bool init(int iMapId, bool bEnablePlayMp3, bool bShowTitle);
	RomanceDetailsLayer();
	~RomanceDetailsLayer();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void GetWarList();
	void AddWarShowElem();
	void AddShowElemLine();
	void ChildCloseLayer(int iNomal);
	void clickCloseMenu(Ref* sender);
	void clickTreasureMenu(Ref* sender);
	void clickJuBenMenu(Ref* sender);
private:
	int mMapId;
	int mClickIndex;

	map<int, int> mWarList;
	map<int, int> mWarOpenList;
	Layer* mShowElemLayer;
	int mStartWarId;
	map<int, int> mMapBoundary;//0:x-min,1:x-max,2:y-min,3:y-max
	map<int, RomanceDetailsInfo*> mShowElem;
	Vec2 mShowLayerVec2;
	Vec2 mClickStartVec2;
	DSize mFullSize;
	Label* mjinduLab;
};

class RomanceDetailsWarLayer :public BTLayer
{
public:
	static RomanceDetailsWarLayer* create(int iWarId,bool bIsOpen,EventDelegate* ptrDelegate);
	bool init(int iWarId, bool bIsOpen, EventDelegate* ptrDelegate);
	RomanceDetailsWarLayer();
	~RomanceDetailsWarLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

private:
	void addWinCondition(Sprite* BgSp);
	void addSpoilsElem(Sprite* BgSp);
	void clickCloseMenu(Ref* sender);
	void clickDispatchMenu(Ref* sender);
private:
	int m_iWarId;
	bool m_bIsOpen;
	EventDelegate* mDelegate;
};
#endif //(__TRGAME_ROMANCEDETAILS_LAYER__)