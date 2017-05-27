#ifndef __TRGame_ClanLog_TableView__
#define __TRGame_ClanLog_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClanLogElem
{
public:
	void LoadJson(Json::Value vContent);
	map<int,string> mContentlist;
	map<string, string> mInfoElem;
};

class ClanLogTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static ClanLogTableView* create(Json::Value vLogData);
	virtual bool init(Json::Value vLogData);

	ClanLogTableView();
	~ClanLogTableView();
    
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
    
    TableView* tableView;
	int cell_num;

private:
	void getLogLost(Json::Value vLogData);
	map<int, ClanLogElem*> mLogList;
};

class ClanLogCell :public TableViewCell
{
public:
	static ClanLogCell* create(ClanLogElem* ptrLogElem);
	bool init(ClanLogElem* ptrLogElem);
	ClanLogCell();
	~ClanLogCell(){}
	void UpDateCell(ClanLogElem* ptrLogElem);

private:
	void addString(ClanLogElem* ptrLogElem);
	Sprite* mLogBg;
};

#endif /* __TRGame_ClanLog_TableView__ */
