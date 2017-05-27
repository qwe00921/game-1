#ifndef __DISPATCHWAIT_TABLEVIEW__
#define __DISPATCHWAIT_TABLEVIEW__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class DisPatchWaitTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static DisPatchWaitTableView* create(int cell_num, EventDelegate* ptrDelegate);
	virtual bool init();

	DisPatchWaitTableView();
	~DisPatchWaitTableView();
    
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

	void UpdateTableView();

    TableView* tableView;
	EventDelegate* mDelegate;
	int m_cell_num;
};

class DisPatchWaitInfo
{
	CC_SYNTHESIZE(Sprite*, mBg, BG);
	CC_SYNTHESIZE(Sprite*,mImg,IMG);
	CC_SYNTHESIZE(Sprite*, mFashion, FASHION);
	CC_SYNTHESIZE(Label*, mName, TNAME);
	CC_SYNTHESIZE(Label*, mLv, LV);
	CC_SYNTHESIZE(Sprite*, mButton, BUTTON);
	CC_SYNTHESIZE(Sprite*, mQuality, Quality);
	CC_SYNTHESIZE(Sprite*, mOccupation,Occupation);
	CC_SYNTHESIZE(int, mActorId, ACTORID);
	CC_SYNTHESIZE(int, mOut, IOUT);
	CC_SYNTHESIZE(int, mForceout, FORCEOUT);
	CC_SYNTHESIZE(int, mForcein, FORCEIN);
	CC_SYNTHESIZE(int, mIndex, MINDEX);
};

class DisPatchWaitCell :public TableViewCell, public EventDelegate
{
public:
	static DisPatchWaitCell* create(map<int, map<int, int> > elem_slit,int iIndex);
	bool init(map<int, map<int, int> > elem_slit, int iIndex);
	void onEnter();
	void onExit();
	DisPatchWaitCell();
	~DisPatchWaitCell(){}

	void UpDateCell(map<int, map<int, int> > elem_slit, int iIndex);

	EventDelegate* mDelegate;
	EventDelegate* tbvDelegate;
private:
	map<int, DisPatchWaitInfo*> elem;
	int click_img_index;
    void initTouches();
};

#endif /* __DISPATCHWAIT_TABLEVIEW__ */
