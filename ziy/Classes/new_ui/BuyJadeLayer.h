#ifndef __TRGAME_BUYJADELAYER__
#define  __TRGAME_BUYJADELAYER__

#include "Defines.h"

class JadeSprite : public BTSprite
{
public:
	CREATE_FUNC(JadeSprite);
	virtual bool init();
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchMoved(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event* unused_event);

	void updateSprite(int iIndex);
private:
	void initControl();
private:
	Sprite* m_spBG;
	Sprite* m_spJade;
	Sprite* m_spGift;
	Label* m_labPrice;
	Label* m_labGift;

	bool m_bIsSelect;
	int m_iIndex;
};

class JadeCell : public TableViewCell
{
public:
	CREATE_FUNC(JadeCell);
	virtual bool init();
	void updateCell(map<int, int> mIndex);
	Sprite* getJadeSprite();
private:
	void initControl();
private:
	map<int, JadeSprite*> m_mSpJade;
};

class JadeTableView : public Layer, TableViewDataSource, TableViewDelegate
{
public:
	static JadeTableView* create(map<int, int> mIndex, EventDelegate* ptrDelegate);
	virtual bool init(map<int, int> mIndex, EventDelegate* ptrDelegate);

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
	void updateTableView();
private:
	void jadeSpriteClick();
	void updateArrowShow();
public:
	static int ms_iIndex;
private:
	map<int, int> m_mIndex;
	EventDelegate* m_pDelegate;
	TableView* m_pTableView;

	Sprite* m_spArrowUp;
	Sprite* m_spArrowDw;
};

class BuyJadeLayer : public BTLayer
{
public:
	static BuyJadeLayer* create(EventDelegate* ptrDelegate);
	static BuyJadeLayer* create(EventDelegate* ptrDelegate,bool fireCloseEvent);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate, bool fireCloseEvent);
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchMoved(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event* unused_event);
	virtual void callBackListClick(int iIndex);
	virtual void RunFunction(int iNomal);
	virtual void updateParentShow();
private:
	void initControl();
	void updateVipLayer();
	void updateVipState(float fDelay);

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onHttpReqSucceed(Json::Value valRes, string strTag);

	void callBackClose(Ref* sender);
private:
	EventDelegate* m_pDelegate;
	JadeTableView* m_tvJade;
	bool m_fireCloseEvent;
	Sprite* m_spVipBG;
	Label* m_labJade;
	Label* m_labVip1;
	Label* m_labVip2;
	Label* m_labTime;
	Label* m_labRech;

	bool m_bIsClick;
	bool m_bIsVip;
	bool m_bIsContinue; //是否继续请求，只有请求收到结果后才会继续
};
#endif //(__TRGAME_BUYJADELAYER__)