#ifndef __SWEEP_TABLEVIEW__
#define __SWEEP_TABLEVIEW__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class SweepTableView;
class SweepMaskLayer :public BTLayer
{
public:
	static SweepMaskLayer* create(SweepTableView* ptrSweepTb);
	bool init(SweepTableView* ptrSweepTb);

	SweepMaskLayer() {};
	~SweepMaskLayer(){};

	bool onTouchBegan(Touch *touch, Event *unused_event);

	SweepTableView* mSweepTableView;
};

class SweepDateElem
{
	CC_SYNTHESIZE(int,mIndex,INDEX);
	CC_SYNTHESIZE(int, mGold, GOLD);
	map<int, int> GoodsList;
};

class SweepTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
    static SweepTableView* create(Json::Value jsonResult,int iGoldNum);
	virtual bool init(Json::Value jsonResult, int iGoldNum);

	SweepTableView();
	~SweepTableView();
	void onExit();
	void onEnter();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
    
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

	void GetElemDate(Json::Value jsonResult, int iGoldNum);
	void TbOffsetRunAction(float f);
	void TBRunAction(float f);
	void TbElemRunAction();
	void ElemActionCallF(Node* sender);
	void clickSureMenu(Ref* sender);
	void setStopAction();
public:
	TableView* tableView;
	Menu* sureM;
private:
	int mCellNum;
	int mCellWidth;
	int mCellHeight;
	int mCurElemIndex;
	float mRunActionInterval;
	float mTbActionInterval;
	float mElemActionInterval;
	int mTbRunActionNum;

	bool mStopAllAction;

	map<int, SweepDateElem*> elem_list;

	SweepMaskLayer* mSweepMaskLayer;
};

class SweepDateCell :public TableViewCell
{
public:
	static SweepDateCell* create(SweepDateElem* ptrSweepDate, bool bRunAction);
	bool init(SweepDateElem* ptrSweepDate, bool bRunAction);
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	SweepDateCell();
	~SweepDateCell(){}
	void UpDateCell(SweepDateElem* ptrSweepDate, bool bRunAction);

	Sprite* title_bg;
	map<int, Sprite*> eGoodsList;
private:
	void AddGoodsElem(SweepDateElem* ptrSweepDate, bool bRunAction);
};

#endif /* __SWEEP_TABLEVIEW__ */
