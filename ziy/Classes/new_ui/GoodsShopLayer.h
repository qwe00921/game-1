//
//  GoodsShopLayer.h
//  TRGame
//
//  Created by Jimmy on 16/5/9.
//
//

#ifndef _GOODSSHOPLAYER_H_
#define  _GOODSSHOPLAYER_H_

#include "Defines.h"

enum
{
	DEAL_BUY = 0, //商店
	DEAL_SELL = 1, //背包出售
	DEAL_MAKE = 2, //制作
	DEAL_EXCH = 3,  //兑换
	DEAL_STORE = 4, //货郎
	DEAL_ENCHANT = 5, //附魔
};

enum 
{
	PAY_TYPE_GOLD = 1, //金币
	PAY_TYPE_JADE = 2, //勾玉
	PAY_TYPE_LONG = 3, //骨龙币
	PAY_TYPE_OTHER = 4 //其他
};

class SellGoods
{
	CC_SYNTHESIZE(int, iIndex, Index);//物品索引
	CC_SYNTHESIZE(int, iKeyId, KeyId);//物品Key值
	CC_SYNTHESIZE(int, iGoodsId, GoodsId);//物品ID
	CC_SYNTHESIZE(int, iCount, Count);//数目
	CC_SYNTHESIZE(int, iPrice, Price);//价格
	CC_SYNTHESIZE(int, iDiscount, Discount); //折扣
	CC_SYNTHESIZE(int, iPayType, PayType);//支付类型
	CC_SYNTHESIZE(int, iDealType, DealType); //商铺类型
	CC_SYNTHESIZE(int, iIsSell, IsSell);//是否已卖
	CC_SYNTHESIZE(string, strPath, IconPath); //图标路径
	CC_SYNTHESIZE(string, strType, StrType); //支付类型货郎需要
};

class ShopGoodsSprite : public BTSprite
{
public:
	CREATE_FUNC(ShopGoodsSprite);
	virtual bool init();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void updateSprite(SellGoods* shopGoods);
private:
	void initControl();
private:
	Label* m_labName;
	Label* m_labPrice;
	Sprite* m_spCellBG;
	Sprite* m_spIcon;
	Sprite* m_spSoul;
	Sprite* m_spPrice;
	Sprite* m_spIsSell;
	Sprite* m_spNum;
	Sprite* m_spDisCount;
	Sprite* m_spDisIcon;
	Sprite* m_spDisLine;

	bool m_bIsSelect;
	int m_iIndex;
};

class GoodsCell : public TableViewCell
{
public:
	static GoodsCell* create(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods);
	bool init(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods);
	GoodsCell();
	void updateCell(map<int, SellGoods*> mSellGoods);
	void callBackClick(int iIndex);
private:
	void initControl();
	void showGoodsDeal();
private:
	EventDelegate* m_pDelegate;
	map<int, ShopGoodsSprite*> m_mSellGoods;
};

class GoodsTableView : public Layer, TableViewDataSource, TableViewDelegate
{
public:
	static GoodsTableView* create(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods);
	virtual bool init(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods);
	GoodsTableView();
	~GoodsTableView();

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

	void updateTableView(map<int, SellGoods*> mSellGoods);
	void updateGoodsCell(int iIndex);
public:
	TableView* m_pTableView;
	EventDelegate* m_pDelegate;
	static int ms_iSelectIndex;
private:
	int m_iSelectIndex;
	map<int, SellGoods*> m_mSellGoods;
	map<int, GoodsCell*> m_mCell;
};

class GoodsShopLayer : public BTLayer
{
public:
	CREATE_FUNC(GoodsShopLayer);
	virtual bool init();
	GoodsShopLayer();
	~GoodsShopLayer();

	virtual void updateShow(int index);
	virtual void callBackListClick(int iIndex);
	virtual void callBackConfirm();
	virtual void updateMoneyShow();
private:
	void initControl();
	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool parseJson(Json::Value val, int iReqType);

	void callBackGold(Ref *sender);
	void callBackJade(Ref *sender);
	void callBackClose(Ref *sender);
	void callBackRefresh(Ref *sender);
private:
	Menu* m_pBtnMenu;
	Label* m_labGold;
	Label* m_labJade;
	Label* m_labRefTime;
	GoodsTableView* m_mTVGoods;

	map<int, SellGoods*> m_mSellGoods;
	int m_iRefresh; //手动刷新次数
	int m_iMaxRef; //刷新价格表里的最大次数
};

class GoodsExchLayer : public BTLayer
{
public:
	CREATE_FUNC(GoodsExchLayer);
	virtual bool init();
	GoodsExchLayer();
	~GoodsExchLayer();

	virtual void updateShow(int index);
	virtual void callBackListClick(int iIndex);
private:
	void initControl();
	void initGoodsMap();
	void updateDisplay();
	void updateToken(); //更新令牌显示

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);

	void callBackClose(Ref *sender);
	void callBackSwitch(Ref* sender);
public:
	static int ms_iExchType;
private:
	Sprite* m_spToken; //令牌ICON
	Label* m_labToken;
	GoodsTableView* m_mTVGoods;
	map<int, MenuItemSprite*> m_mBtnType;
	map<int, map<int, SellGoods*> > m_mExchGoods;
};

#endif // _GOODSSHOPLAYER_H_