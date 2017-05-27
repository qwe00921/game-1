//
//  DailySceneLayer.h
//  TRGame
//
//  Created by Jimmy on 16/08/18.
//
//

#ifndef _DailySceneLayer_H_
#define _DailySceneLayer_H_

#include "Defines.h"

class DailyDate
{
	CC_SYNTHESIZE(int, m_iMapId, MapId);
	CC_SYNTHESIZE(int, m_iWarId, WarId);
	CC_SYNTHESIZE(int, m_iTimes, Times);
	CC_SYNTHESIZE(bool, m_bIsOpen, IsOpen);
};

class DailySceneCell : public TableViewCell
{
public:
	CREATE_FUNC(DailySceneCell);
	bool init();
	void updateCell(DailyDate* ptrDaily);
private:
	void initControl();
public:
	Sprite* m_spScene;
};

class DailySceneTableView : public Layer, TableViewDataSource, TableViewDelegate
{
public:
	static DailySceneTableView* create(map<int, DailyDate*> mDaily);
	virtual bool init(map<int, DailyDate*> mDaily);
	DailySceneTableView();
	~DailySceneTableView();

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
	void showSelectLayer();
	void updateArrow();
private:
	TableView* m_pTableView;
	map<int, DailyDate*> m_mDaily;
	int m_iClickIndex;
	Sprite* m_spLArrow;
	Sprite* m_spRArrow;
};

class DailySceneLayer : public BTLayer
{
public:
	static DailySceneLayer* create(int iIsGLCBattled = 0);
	virtual bool init(int iIsGLCBattled = 0);
	DailySceneLayer();
	~DailySceneLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void initDailyDate();
	string getWarIdData();

	void onHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onDailySucceed(Json::Value valRes);
	bool GetBattleState(int iMapId);

	void callBackExch(Ref *sender);
	void callBackClose(Ref *sender);
private:
	map<int, DailyDate*> m_mDaily;
	int m_iIsGLC;
};

#endif //_DailySceneLayer_H_