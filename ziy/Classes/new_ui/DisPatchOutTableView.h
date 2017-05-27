#ifndef __DISPATCHOUT_TABLEVIEW__
#define __DISPATCHOUT_TABLEVIEW__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class DisPatchOutTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static DisPatchOutTableView* create(EventDelegate* ptrDelegate, int mCellNum);
	virtual bool init();

	DisPatchOutTableView();
	~DisPatchOutTableView();
    
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

	void UpdateTableView(int iCellNum,int iActionId);

public:
	TableView* tableView;
	EventDelegate* mDelegate;
	int mCellNum;
	int mActionId;
};

class DisPatchOutCell :public TableViewCell
{
public:
	static DisPatchOutCell* create(int iActorId,int iActionId);
	bool init();
	DisPatchOutCell();
	~DisPatchOutCell(){}
	void UpDateCell(int iActorId, int iActionId);
	void runAction(float fNum);
	void onExit();
	void onEnter();

private:
	int actor_id;
	int img_index;
	Sprite* actor_img;
	Sprite* actor_wep;
	Sprite* bg;
	int mActionId;
};

#endif /* __DISPATCHOUT_TABLEVIEW__ */
