//
//  PVPSelectLayer.h
//  TRGame
//
//  Created by zwy on 17/03/31.
//
//

#ifndef _PVPSelectLayer_H_
#define _PVPSelectLayer_H_

#include "Defines.h"

class PVPSelectCell : public TableViewCell
{
public:
	CREATE_FUNC(PVPSelectCell);
	bool init();
	void upDateCell(int iIndex);
private:
	void initControl();
public:
	Sprite* m_spScene;
	PVPSelectCell* ptrCell;
};

class PVPSelectTableView : public Layer, TableViewDataSource, TableViewDelegate
{
public:
	static PVPSelectTableView* create();
	virtual bool init();
	PVPSelectTableView();
	~PVPSelectTableView();
	//重写
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
	//每一个cell选择
	void showSelectLayer();
	void upDateArrow();
private:
	TableView* m_pTableView;
	int m_iCellNum;
	Sprite* m_spLArrow;
	Sprite* m_spRArrow;
	int m_iClickIndex;
};

class PVPSelectLayer : public BTLayer
{
public:
	static PVPSelectLayer* create();
	virtual bool init();
	PVPSelectLayer();
	~PVPSelectLayer();
private:
	void initControl();
	void callBackClose(Ref *sender);
};

#endif //_PVPSelectLayer_H_