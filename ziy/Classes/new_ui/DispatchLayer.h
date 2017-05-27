#ifndef __TRGAME_DISPATCH_LAYER__
#define  __TRGAME_DISPATCH_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "DisPatchOutTableView.h"
#include "DisPatchWaitTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class RatioActorElem
{
	CC_SYNTHESIZE(float,fRatio,Ratio);
	CC_SYNTHESIZE(Sprite*,spfBg,fBg);
	CC_SYNTHESIZE(Label*,labfScore, fScore);
	CC_SYNTHESIZE(Label*,labfLevel,fLevel);
	CC_SYNTHESIZE(Label*,labfInfo,fInfo);
};

class DispatchLayer :public BTLayer
{
public:
	//CREATE_FUNC(DispatchLayer);
	static DispatchLayer* create(EventDelegate* ptrDelegate = NULL);
	bool init();
	DispatchLayer();
	~DispatchLayer();
	void onExit();
	void onEnter();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void clickCloseLayer(Ref* sender);
	void clickPreBattleMenu(Ref* sender);
	void clickDispatchMenu(Ref* sender);
	void clickSortMenu(Ref* sender);
	void clickRatioMenu(Ref* sender);

	void updateShow(int index, int type);
	void initShow();
	void SetActorList();
	void SortActorList(int iType);
	map<int, map<int, int> > GetElem(int iIndex);
	int GetElemInt(int iIndex);
	void setBattleState();
	void GetFitsEffect();
	void updateMoneyShow();
	void RemoveAllParentLayer();
	void SortDispatchActorLiat();
private:
	void dispatchFunction();
	void onHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void setRatioData();
	void preDispatch(float fDelay);
public:
	map<int, int> actor_list;
	map<int, int> out_list;
	map<int, int> in_list;
	map<int, int> sort_list;
	int min_num;
	int max_num;
	int m_actor_id_list[MAX_OWN_ARMY_NUM];
	int out_actor_num;
	int sort_type;
	float m_fRatioId;

	DisPatchWaitTableView* wait_tbv;
	int actor_cell_num;
	Label* num_date;
	MenuItemSprite* dispatch_mitem;
	MenuItemSprite* spr_sort;
	Menu* menu_up;
	EventDelegate* mDelegate;

	Menu* mMenuSort;
	Menu* mMenuPreB;
	Menu* mMenuDispatch;
	Menu* mMenuClose;
private:
	RatioActorElem mRatioActorElem;
};
#endif //(__TRGAME_DISPATCH_LAYER__)