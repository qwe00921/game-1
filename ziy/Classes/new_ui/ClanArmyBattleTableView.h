#ifndef __TRGame_ClanArmyBattle_TableView__
#define __TRGame_ClanArmyBattle_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
#include "GoodsSprite.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClanArmyBattleElem;

class ClanArmyBattleTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static ClanArmyBattleTableView* create(map<int, ClanArmyBattleElem*>* ptrArmyBattleData,EventDelegate* ptrDelegate, string guildIdS);
	virtual bool init(map<int, ClanArmyBattleElem*>* ptrArmyBattleData, EventDelegate* ptrDelegate, string guildIdS);

	ClanArmyBattleTableView();
	~ClanArmyBattleTableView();
    
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

	//出兵回调
	void ChildCloseLayer(int iNomal);
	//cell点击回调
	void RunFunction(int iNomal);
	void SetPopLayerElement(Sprite* bg);

	//开启副本回调
	void callBackConfirm();

	//cell点击回调
	int GetOrSetDate(int iDate, bool is_get = true);

	//伤害按钮回调
	void updateRTDate(int iIndex);
private:
	void SendOpenWarNetData();
	void SendGainHpLogNetData();
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);

	map<int, ClanArmyBattleElem*>* mArmyBattleData;
	int m_iSelectCellIndex;

	EventDelegate* mDelegate;

	Sprite* mLeftArrow;
	Sprite* mRightArrow;

	float fOffsetMaxX;

	int m_iEnableClickCell;

	string mGuildIdS;
};

class ClanArmyBattleCell :public TableViewCell
{
public:
	static ClanArmyBattleCell* create(ClanArmyBattleElem* ptrArmyBattleElem,EventDelegate* ptrDelegate);
	bool init(ClanArmyBattleElem* ptrArmyBattleElem, EventDelegate* ptrDelegate);
	ClanArmyBattleCell();
	~ClanArmyBattleCell();
	void UpDateCell(ClanArmyBattleElem* ptrArmyBattleElem);

	void clickGoodsMenu(Ref* sender);
private:
	void initTouches();
	void callBack();
	EventListenerTouchOneByOne* m_ptrLisTouch;
	ClanArmyBattleElem* mArmyBattleElem;
	Sprite* mbg;
	Sprite* mLockSp;
	Sprite* mTfInfo;
	Sprite* mHpBarBg;
	Sprite* mHpBarSp;
	Label* mHpBarPre;
	Sprite* mGainHpLog;

	map<int, GoodsSprite*> mGoodsList;
	EventDelegate* mDelegate;

	int m_iIsClick;
	int m_iIsClickGoods;
};

#endif /* __TRGame_ClanLog_TableView__ */
