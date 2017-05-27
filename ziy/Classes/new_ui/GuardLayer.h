#ifndef __TRGAME_GUARD_LAYER__
#define  __TRGAME_GUARD_LAYER__
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
	GUARD_SELECT_ACTOR,//高狼城镇守
	ARRAY_SELECT_ACTOR,//城战布阵
};

class GuardShowElem
{
	CC_SYNTHESIZE(Sprite*, mBg, BG);
	CC_SYNTHESIZE(Sprite*, mImg, IMG);
	CC_SYNTHESIZE(Sprite*, mFashion, FASHION);
	CC_SYNTHESIZE(Label*, mName, TNAME);
	CC_SYNTHESIZE(Label*, mLv, LV);
	CC_SYNTHESIZE(Sprite*, mQuality, QUALITY);
	CC_SYNTHESIZE(int, mActorId, ACTORID);
	CC_SYNTHESIZE(Sprite*, mOccupation, OCCUPATION);
	CC_SYNTHESIZE(Sprite*,mAddActor,ADDACTOR);
};

class GuardLayer :public BTLayer
{
public:
	static GuardLayer* create(int iWarId,EventDelegate* ptrDelegate);
	bool init(int iWarId, EventDelegate* ptrDelegate);
	GuardLayer();
	~GuardLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchesCancelled(Touch *touch, Event *unused_event);

	void UpdataElemShow();

	void clickSureMenu(Ref* sender);
	void clickCloseMenu(Ref* sender);
	void RunFunction(int iNomal);

	void SendNetDate();
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);

	int m_iWarId;
	int m_iClickSIndex;

	map<int, GuardShowElem*> mShowElem;
	map<int, int> mActorIdList;
	map<int, Label*> mGuardReward;
	EventDelegate* mDelegate;
	bool m_bEnableDispatch;
};

class SelectGuradActorTableView:public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static SelectGuradActorTableView* create(map<int,int> mGuradList,int iWarId,EventDelegate* ptrDelegate,int iType = GUARD_SELECT_ACTOR);
	virtual bool init(map<int, int> mGuradList,int iWarId, EventDelegate* ptrDelegate, int iType);

	SelectGuradActorTableView();
	~SelectGuradActorTableView();

	/* 必须实现的函数 */
	//    // 当滑动tableview时触发该方法 参数为当前的tableview对象
	virtual void scrollViewDidScroll(ScrollView* view);
	//    // 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
	virtual void scrollViewDidZoom(ScrollView* view);
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);

	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	void clickCloseMenu(Ref* sender);
	map<int,int> getCellElem(int iIdx);
	void GetElemData(map<int, int> mGuradList);
	void RunFunction(int iNomal);

	int cell_num;
	int m_iWarId;
	TableView* tableView;
	map<int, int> mActorList;
	EventDelegate* mDelegate;
	int m_iType;
};

class  SelectGuradActorCell :public TableViewCell
{
public:
	static SelectGuradActorCell* create(map<int,int> mElemData,EventDelegate* ptrDelegate);
	bool init(map<int, int> mElemData, EventDelegate* ptrEventDelegate);
	SelectGuradActorCell();
	~SelectGuradActorCell();
	void UpDateCell(map<int, int> mElemData, EventDelegate* ptrDelegate);

private:
	void initTouches();

	map<int, GuardShowElem*> mShowElem;
	EventDelegate* mDelegate;
	int m_iClickShowindex;
};

#endif //(__TRGAME_GUARD_LAYER__)