//
//  DailySelectLayer.h
//  TRGame
//
//  Created by Jimmy on 16/08/18.
//
//

#ifndef _DailySelectLayer_H_
#define _DailySelectLayer_H_

#include "Defines.h"
#include "DailySceneLayer.h"

class DailySelectCell : public TableViewCell
{
public:
	CREATE_FUNC(DailySelectCell);
	bool init();
	void updateCell(int iWarId);
private:
	void initControl();
public:
	Sprite* m_spSelect;
	Sprite* m_spPassed;
};

class DailySelectTableView : public Layer, TableViewDataSource, TableViewDelegate
{
public:
	static DailySelectTableView* create(map<int, int> mWarId, EventDelegate* pDelegate);
	virtual bool init(map<int, int> mWarId, EventDelegate* pDelegate);
	DailySelectTableView();
	~DailySelectTableView();

	// 当滑动tableview时触发该方法 参数为当前的tableview对象
	virtual void scrollViewDidScroll(ScrollView* view);
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);
private:
	void showDispatch();
	void updateArrow();
private:
	TableView* m_pTableView;
	map<int, int> m_mWarId;
	EventDelegate* m_pDelegate;
	int m_iClickIndex;
	Sprite* m_spLArrow;
	Sprite* m_spRArrow;
};

class DailySelectLayer : public BTLayer
{
public:
	static DailySelectLayer* create(DailyDate* ptrDaily);
	virtual bool init(DailyDate* ptrDaily);
	DailySelectLayer();
	~DailySelectLayer();
	virtual void callBackListClick(int iIndex);
	virtual void ChildCloseLayer(int iNomal);
	virtual void updateRTDate(int iNomal, int iType);
	virtual void SendNetDateResult(bool bIsSuccess, Json::Value jsonResult, int iType);
private:
	void initControl();
	void updateDisplay();

	void callBackSweep(Ref *sender);
	void callBackClose(Ref *sender);
private:
	DailyDate* m_ptrDaily;
	int m_iSweepId; //可以扫荡关卡的ID
	int m_iSweepType;

	map<int, MenuItemSprite*> m_mMenuItem;
	MenuItemSprite* m_btnSweep;
	Label* m_labTimes;
	Label* m_labOpen;
};

#endif //_DailySelectLayer_H_