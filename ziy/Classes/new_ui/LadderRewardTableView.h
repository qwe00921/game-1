#ifndef __TRGame_LADDERREWARD_TableView__
#define __TRGame_LADDERREWARD_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LadderRewardTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	CREATE_FUNC(LadderRewardTableView);
	virtual bool init();

	LadderRewardTableView();
	~LadderRewardTableView();
    
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
    
    TableView* tableView;
};

class LadderRewardCell :public TableViewCell
{
public:
	CREATE_FUNC(LadderRewardCell);
	bool init();
	LadderRewardCell();
	~LadderRewardCell() {};
};

#endif /* __TRGame_LADDERREWARD_TableView__ */
