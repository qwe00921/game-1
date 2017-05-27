#ifndef __TRGame_ClanActiveTask_TableView__
#define __TRGame_ClanActiveTask_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClanActiveTaskElem
{
	CC_SYNTHESIZE(int,iIndex,EiIndex);
	CC_SYNTHESIZE(int,iId,EiId);
	CC_SYNTHESIZE(int,iType,EiType);
	CC_SYNTHESIZE(string, sTypeName,EsTypeName);
	CC_SYNTHESIZE(int,iWarid,EiWarid);
	CC_SYNTHESIZE(int,iNum,EiNum);
	CC_SYNTHESIZE(int,iActive,EiActive);
	CC_SYNTHESIZE(int,iStatus, EiStatus);
	CC_SYNTHESIZE(int, iFinishNum, EiFinishNum);
public:
	void LoadJsonValue(Json::Value vElemJson,int iIndex);
	map<int, map<int, int> > mGoods;
};

class ClanActiveTaskTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static ClanActiveTaskTableView* create(Json::Value vLogData,EventDelegate* ptrDelegate);
	virtual bool init(Json::Value vLogData, EventDelegate* ptrDelegate);

	ClanActiveTaskTableView();
	~ClanActiveTaskTableView();
    
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

	void updateRTDate(int iNomal,int iType);

private:
	void SendLingQuNetData();
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);

	void clickCloseMenu(Ref* sender);
	void showPrompt(float f);

	void ClanTaskJump(int iIndex);

	void getActiveData(Json::Value vLogData);
	map<int, ClanActiveTaskElem*> mActiveData;

	EventDelegate* mDelegate;
	bool bEnableUpdataFatherShow;

	int m_iLingQuIndex;

	int mPromptTxtIndex;
	map<int, string> mPromptTxt;
};

class ClanActiveTaskCell :public TableViewCell
{
public:
	static ClanActiveTaskCell* create(ClanActiveTaskElem* ptrATE,EventDelegate* ptrDelegate);
	bool init(ClanActiveTaskElem* ptrATE, EventDelegate* ptrDelegate);
	ClanActiveTaskCell();
	~ClanActiveTaskCell(){}
	void UpDateCell(ClanActiveTaskElem* ptrATE);
private:
	ClanActiveTaskElem* mClanActiveTaskElem;
	string GetGoodsString();

	void clickLingQuMenu(Ref* sender);
	void clickQianWangMenu(Ref* sender);

	EventDelegate* mDelegate;
	Sprite* mImg;
	Label* mActiveName;
	Label* mGoodsLab;
	Label* mActiveInfo;

	Sprite* mYilingquSp;
	//Sprite* mWeiWanCheng;
	MenuItemSprite* mLingQuMI;
	MenuItemSprite* mQianWangMI;
};

#endif /* __TRGame_ClanActiveTask_TableView__ */
