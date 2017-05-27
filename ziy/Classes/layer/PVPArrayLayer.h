//
//  PVPArrayLayer.h
//  TRGame
//
//  Created by Jimmy on 17/03/14.
//
//

#ifndef _PVPArrayLayer_H_
#define  _PVPArrayLayer_H_

#include "Defines.h"
#include "PVPHomeLayer.h"
#include "ArmorListTableView.h"

class ArraySprite : public BTSprite
{
public:
	ArraySprite();
	~ArraySprite();
	static ArraySprite* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void updateActor(int iActorId);
	void updateOrder(int iOrderId);
	void updateSprite(PVPArrayItem* ptrData, string strSeid);
private:
	void initControl();
private:
	EventDelegate* m_ptrDel;
	Sprite* m_spBGNull;
	Sprite* m_spBGHero;
	Sprite* m_spImage;
	Sprite* m_spWeapon;
	Sprite* m_spPointId;
	Sprite* m_spOrderId;
	Label* m_labSeid;

	bool m_bIsSelect;
	bool m_bIsOrder; //排序中
	int m_iIndex;
};

class PVPActorCell;
class PVPActorTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	PVPActorTableView();
	~PVPActorTableView();
	static PVPActorTableView* create(map<int, int> mActorId, EventDelegate* ptrDelegate);
	virtual bool init(map<int, int> mActorId, EventDelegate* ptrDelegate);
	virtual void callBackListClick(int iIndex);
	/* 必须实现的函数 */
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	ArmorListDate* GetCellElem(int iIndex);
	void removeActor(int iActorId);
private:
	void InitElemList(map<int, int> mActorId);
	void SortElemList();
	void updateElemList(); //选中的排前面
private:
	EventDelegate* m_ptrDel;
	TableView* tableView;
	int cell_num;
	map<int, ArmorListDate*> armor_list;
	map<int, PVPActorCell*> m_mPtrCell;
	CC_SYNTHESIZE(bool, m_bIsOrder, IsOrder); //是否在排序
};

class PVPActorCell :public TableViewCell
{
public:
	static PVPActorCell* create(PVPActorTableView* ptrALTB, int iIndex);
	bool init(int iIndex);
	PVPActorCell();
	~PVPActorCell() {}
	void UpDateCell(int iIndex);
public:
	map<int, ArmorListInfo*> elem;
	PVPActorTableView* mALTB;
	int click_elem_index;
private:
	void initTouches();
};

class PVPArrayLayer : public BTLayer
{
public:
	PVPArrayLayer();
	~PVPArrayLayer();
	static PVPArrayLayer* create(int iType, bool bIsInit = true);
	virtual bool init(int iType, bool bIsInit);
	virtual void updateShow(int iIndex, int iType);
private:
	void initControl();
	void initArrayData();
	void updateArrayType();
	void updateBtnEnabled();
	void preEntryBattle();
	string getArrayData();

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onSaveDefSucceed(Json::Value valRes);
	void onPreEntrySucceed(Json::Value valRes);

	void callBackLast(Ref *sender);
	void callBackNext(Ref *sender);
	void callBackOrder(Ref *sender);
	void callBackSave(Ref *sender);
	void callBackConfirm(Ref *sender);
	void callBackCancel(Ref *sender);
	void callBackFight(Ref *sender);
	void callBackClose(Ref *sender);
private:
	int m_iType;
	bool m_bIsInit;
	int m_iArrayIndex;

	Sprite* m_spBG;
	Label* m_labName;

	MenuItemSprite* m_btnOrder;
	MenuItemSprite* m_btnSave;
	MenuItemSprite* m_btnFight;
	MenuItemSprite* m_btnConfirm;
	MenuItemSprite* m_btnCancel;

	PVPActorTableView* m_tvActorList;
	map<int, ArraySprite*> m_mSpArray;

	PVPArrayInfo* m_pArrayInfo;
	map<int, int> m_mOrderId;
};

#endif // _PVPArrayLayer_H_