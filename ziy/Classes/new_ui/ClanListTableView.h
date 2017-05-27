#ifndef __TRGame_CLANLIST_TableView__
#define __TRGame_CLANLIST_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "ClanApplyListLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClanListTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static ClanListTableView* create(map<int, ClanListElem*>* ptrElemList,EventDelegate* ptrDelegate);
	virtual bool init(map<int, ClanListElem*>* ptrElemList, EventDelegate* ptrDelegate);

	ClanListTableView();
	~ClanListTableView();
    
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

	//点击cell回调
	void RunFunction(int iNomal);
	//申请加入公会回调
	void updateRTDate(int iClanId);

	void UpdataElemData(map<int, ClanListElem*>* ptrElemList,int iType = 0);

	int m_iSelectDataIndex;
private:

	map<int, ClanListElem*>* mElemDataList;
	map<int, int> mClanIdIndex;
	EventDelegate* mDelegate;
};

class ClanListCell :public TableViewCell
{
public:
	static ClanListCell* create(ClanListElem* ptrElem, EventDelegate* ptrDelegate);
	bool init(ClanListElem* ptrElem, EventDelegate* ptrDelegate);
	ClanListCell();
	~ClanListCell(){}
	void UpDateCell(ClanListElem* ptrElem);

	void SetSelect(bool bIsSelect);
	int GetClanIndex();
	ClanListElem* GetElemData();
private:
	void initTouches();

	Sprite* mbg;
	Label* mClanName;
	Label* mClanLv;
	Label* mElderName;
	Label* mHeroNum;
	Sprite* mIsApply;

	bool m_bIsSelect;
	ClanListElem* mDataElem;
	EventDelegate* mDelegate;

	bool m_bIsClickSelect;
};

#endif /* __TRGame_CLANLIST_TableView__ */
