//
//  ActivityLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/14.
//
//

#ifndef _ACTIVITYLAYER_H_
#define _ACTIVITYLAYER_H_

#include "Defines.h"
#include "ActivityInfoLayer.h"

class ActivityCell : public TableViewCell
{
public:
	CREATE_FUNC(ActivityCell);
	bool init();
	void updateCell(int iActId);
private:
	void initControl();
public:
	Sprite* m_spBGN;

	static int ms_iSelect;
private:
	Sprite* m_spIcon;
	Sprite* m_spSelect;
	Label* m_labTitle;
	Label* m_labMark;
};

class ActivityTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static ActivityTableView* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	ActivityTableView();
	~ActivityTableView();
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
private:
	void showActivityInfo();
	void updateArrowShow();
public:
	TableView* m_pTableView;
private:
	EventDelegate* m_pDelegate;

	Sprite* m_spArrowUp;
	Sprite* m_spArrowDown;
	int m_iCellNum;
	map<int, int> m_mActId;
};

class ActivityLayer : public BTLayer
{
public:
	CREATE_FUNC(ActivityLayer);
	ActivityLayer();
	~ActivityLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	virtual void callBackListClick(int iIndex);
private:
	void initControl();
	void callBackClose(Ref *sender);
	void initCustomEvents();
	ActivityInfoLayer* m_layActInfo;
};

#endif //_ACTIVITYLAYER_H_
