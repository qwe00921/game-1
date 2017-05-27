//
//  PVPRewardLayer.h
//  TRGame
//
//  Created by Jimmy on 17/03/22.
//
//

#ifndef _PVPRewardLayer_H_
#define  _PVPRewardLayer_H_

#include "Defines.h"

class PVPRewardCell : public TableViewCell
{
public:
	PVPRewardCell();
	~PVPRewardCell();
	static PVPRewardCell* create();
	virtual bool init();
	void UpDateCell(int iIndex);
private:
	void initControl();
	CC_SYNTHESIZE(int, m_iHeight, CellHeight);//单元高度
};

class PVPRewardTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	PVPRewardTableView();
	~PVPRewardTableView();
	static PVPRewardTableView* create();
	virtual bool init();
	/* 必须实现的函数 */
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);
private:
	TableView* tableView;
	int m_iCellHeight;
};

class PVPRewardLayer : public BTLayer
{
public:
	PVPRewardLayer();
	~PVPRewardLayer();
	static PVPRewardLayer* create();
	virtual bool init();
private:
	void initControl();
	void callBackClose(Ref *sender);
};

#endif // _PVPRewardLayer_H_