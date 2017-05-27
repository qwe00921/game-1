#ifndef __TRGame_LADDERRANK_TableView__
#define __TRGame_LADDERRANK_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum
{
	LADDER_RANK,//高狼城排行榜
	WORLD_RANK,//世界排行榜
};

enum
{
	TYPE_NOMAL,
	TYPE_CAILI,
	TYPE_TONGGUAN,
	TYPE_CHONGZHI
};

class LadderRankData
{
	CC_SYNTHESIZE(int,iRank,eRANK);
	CC_SYNTHESIZE(int,iLevel,eLEVEL);
	CC_SYNTHESIZE(int,iScore,eSCORE);
	CC_SYNTHESIZE(string,nameS,eNAME);
	CC_SYNTHESIZE(int, iIsVip, eISVIP);
public:
	map<int, int> mFashion;
};

class LadderRankTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static LadderRankTableView* create(Json::Value JsonValue,int iType = LADDER_RANK);
	virtual bool init(Json::Value JsonValue, int iType, int iCellType = TYPE_NOMAL);

	static LadderRankTableView* create(Json::Value JsonValue, int iCellType , int iType);

	LadderRankTableView();
	~LadderRankTableView();
    
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

	void addShowElem();
	void clickCloseMenu(Ref* sender);
	void clickRewardMenu(Ref* sender);

	void clickZLRankMenu(Ref* sender);
	void clickCLRankMenu(Ref* sender);
	void clickTGRankMenu(Ref* sender);

	LadderRankData* GetCellData(int iDx);
	void getElemData(Json::Value JsonValue,int iType = TYPE_NOMAL);
	void ReLoadTableView(int iType);
	void SendRankData(int iType);
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);
    
    TableView* tableView;
	int cell_num;

	Sprite* mBg;
	map<int, LadderRankData*> mElemData;
	map<int, LadderRankData*> mCLElemData;
	map<int, LadderRankData*> mTGElemData;
	map<int, LadderRankData*> mCZElemData;
	int mScore;
	int mRank;
	int mType;
	int m_iCellType;

	MenuItemSprite* mSelectMenu;
	Sprite* rankTabSp;
	Sprite* rankInfoSp;
	Label* rankLab;
	Label* m_labCP;

	map<int, int> mRankList;
	map<int, float> mRankOffset;
};

class LadderRankCell :public TableViewCell
{
public:
	static LadderRankCell* create(LadderRankData* ptrLRData);
	bool init(LadderRankData* ptrLRData);
	LadderRankCell();
	~LadderRankCell() {};
	void UpDateCell(LadderRankData* ptrLRData);
	void GetNumSprite(Sprite* bgSp,int iNum);

	int m_iCellWidth;
	int m_iCellHeight;

	Sprite* bg;
	Sprite* mNumSp;
	Sprite* spEquip;
	Sprite* spHero;
	Label* nameLab;
	Label* levelLab;
	Label* scoreLab;
	Sprite* vipSp;
};

#endif /* __TRGame_LADDERRANK_TableView__ */
