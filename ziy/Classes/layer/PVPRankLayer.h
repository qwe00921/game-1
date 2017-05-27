//
//  PVPRankLayer.h
//  TRGame
//
//  Created by zwy on 17/03/31.
//
//

#ifndef _PVPRankLayer_H_
#define  _PVPRankLayer_H_

#include "Defines.h"

class PVPRankData
{
	CC_SYNTHESIZE(int, iRank, eRANK);
	CC_SYNTHESIZE(int, iLevel, eLEVEL);
	CC_SYNTHESIZE(int, iScore, eSCORE);
	CC_SYNTHESIZE(string, nameS, eNAME);
	CC_SYNTHESIZE(int, iIsVip, eISVIP);
public:
	map<int, int> mFashion;
};

class PVPRankTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static PVPRankTableView* create(Json::Value JsonValue);
	virtual bool init(Json::Value JsonValue);
	PVPRankTableView() {}
	~PVPRankTableView() {}
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
	PVPRankData* GetCellData(int iDx);
	void getElemData(Json::Value JsonValue);
public:
	TableView* tableView;
	int cell_num;
	map<int, PVPRankData*> mElemData;
	int mScore;
	int mRank;
};

class PVPRankCell :public TableViewCell
{
public:
	
	static PVPRankCell* create(PVPRankData* ptrLRData);
	bool init(PVPRankData* ptrLRData);
	PVPRankCell();
	~PVPRankCell() {};
	void UpDateCell(PVPRankData* ptrLRData);
	void GetNumSprite(Sprite* bgSp, int iNum);
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

class PVPRankLayer : public BTLayer
{
public:
	PVPRankLayer();
	~PVPRankLayer();
	static PVPRankLayer* create(Json::Value JsonVal);
	virtual bool init(Json::Value JsonVal);
private:
	void initControl();
	void callBackClose(Ref *sender);
private:
	Json::Value m_JsonVal;
};

#endif // _PVPRankLayer_H_