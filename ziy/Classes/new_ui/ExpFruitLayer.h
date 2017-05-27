//
//  ExpFruitLayer.h
//  TRGame
//
//  Created by Jimmy on 16/5/29.
//
//

#ifndef _EXPFRUITLAYER_H_
#define  _EXPFRUITLAYER_H_

#include "Defines.h"
#include "editor-support/cocostudio/CCArmature.h"

using namespace cocostudio;
class AddExpHero
{
	CC_SYNTHESIZE(int, iIndex, Index); //元素索引
	CC_SYNTHESIZE(int, iHeroId, HeroId); //武将ID
	CC_SYNTHESIZE(int, iAddVal, AddVal); //经验果增加经验值
	CC_SYNTHESIZE(int, iAddNum, AddNum); //使用经验果数
};

class ExpFruitSprite : public Sprite
{
public:
	CREATE_FUNC(ExpFruitSprite);
	bool init();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void updateSprite(AddExpHero* ptrAddHero, EventDelegate* ptrDelegate);
	void updateExpBar(float fDelay);
private:
	void initControl();
	bool updateLvState(); //返回值标识是否升级
	void getAddLv(int iCount, int& iLv, int& iExp); //使用定量经验果得到的等级
private:
	Sprite* m_spCellBG;
	Sprite* m_spJob;
	Sprite* m_spFashion1;
	Sprite* m_spFashion2;
	Sprite* m_spQuality;
	Sprite* m_spExpBar;
	Sprite* m_spExpFull;

	Label* m_labLv;
	Label* m_labName;

	bool m_bIsSelect;
	AddExpHero* m_pAddHero;
	EventDelegate* m_pDelegate;
};


class ExpFruitCell : public TableViewCell
{
public:
	static ExpFruitCell* create(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero);
	bool init(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero);
	ExpFruitCell();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void updateCell(map<int, AddExpHero*> mAddHero);
	void updateSprite(int iIndex);
private:
	void initControl();
	void showGoodsDeal();
private:
	EventDelegate* m_pDelegate;
	map<int, ExpFruitSprite*> m_mAddHero;
};

class ExpFruitTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static ExpFruitTableView* create(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero);
	virtual bool init(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero);
	ExpFruitTableView();
	~ExpFruitTableView();
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
	void updateTableView(map<int, AddExpHero*> mShopGoods);
public:
	TableView* m_pTableView;
	EventDelegate* m_pDelegate;
	static int ms_iSelectIndex;
private:
	int m_iSelectIndex;
	map<int, AddExpHero*> m_mAddHero;
	map<int, ExpFruitCell*> m_mCell;
};

class ExpFruitLayer : public BTLayer
{
public:
	static ExpFruitLayer* create(int iGoodsId, EventDelegate* ptrDelegate);
	bool init(int iGoodsId, EventDelegate* ptrDelegate);
	ExpFruitLayer();
	~ExpFruitLayer();
	void onEnter();
	void onExit();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	virtual void updateShow(int index);
public:
	static int ms_iTotalNum;
	static int ms_iGoodsId;
private:
	void initControl();
	string getActorListData();
	void ExpAddHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onExpAddSucceed(Json::Value valRes);

	void callBackClose(Ref *sender);
private:
	EventDelegate* m_pDelegate;
	Menu* m_pBtnMenu;
	Label* m_labTitle;

	map<int, AddExpHero*> m_mAddHero;
};

#endif // _EXPFRUITLAYER_H_