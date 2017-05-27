#ifndef __TREASUREBOX_TABLEVIEW__
#define __TREASUREBOX_TABLEVIEW__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TreasureBoxTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static TreasureBoxTableView* create(int iMapId,EventDelegate* ptrDelegate, int iBattleType);
	virtual bool init(int iMapId,EventDelegate* ptrDelegate, int iBattleType);

	TreasureBoxTableView();
	~TreasureBoxTableView();
    
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

public:
	void GetElemList();

	TableView* tableView;
	EventDelegate* mDelegate;
	map<int, NetMapChestPtr> mDataElemList;
	int mCellNum;
	int mMapId;
	int mBattleType;
};

class TreasureBoxElem
{
	CC_SYNTHESIZE(Sprite*, mBg, Bg);
	CC_SYNTHESIZE(Sprite*, mGoodsBg, GoodsBg);
	CC_SYNTHESIZE(Menu*, mReceive, Receive);
	CC_SYNTHESIZE(Sprite*, mAReceive, AReceive);
	CC_SYNTHESIZE(Sprite*, mNReceive, NReceive);
	CC_SYNTHESIZE(Label*, mHdkq,Hdkq);
};

class TreasureBoxECell :public TableViewCell
{
public:
	static TreasureBoxECell* create(NetMapChestPtr ptrNetMc,EventDelegate* ptrDelegate, int iBattleType);
	bool init(NetMapChestPtr ptrNetMc,EventDelegate* ptrDelegate, int iBattleType);
	TreasureBoxECell();
	~TreasureBoxECell(){}
	void UpDateCell(NetMapChestPtr ptrNetMc);
	void onExit();
	void onEnter();

	void AddGoodsListSp(NetMapChestPtr ptrNetMc,Sprite* ptrElemBg);

	void UpdateShow();
	void ClickReceiveMenu(Ref* sender);

	void SendNetDate();
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void setNetDate(Json::Value jsonValue);

private:
	EventDelegate* mDelegate;
	int mBattleType;
	TreasureBoxElem mElemBox;
	NetMapChestPtr mNetMapChest;
};

#endif /* __TREASUREBOX_TABLEVIEW__ */
