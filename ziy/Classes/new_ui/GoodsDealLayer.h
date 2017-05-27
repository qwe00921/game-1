//
//  GoodsDealLayer.h
//  TRGame
//
//  Created by Jimmy on 16/3/23.
//
//

#ifndef _GOODSDEALLAYER_H_
#define  _GOODSDEALLAYER_H_

#include "Defines.h"
#include "GoodsShopLayer.h"

class GoodsDealLayer : public BTLayer
{
public:
	static GoodsDealLayer* create(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate, int iType = DEAL_SELL);
	bool init(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate, int iType);
	static GoodsDealLayer* create(SellGoods* pShopGoods, EventDelegate* ptrDelegate);
	bool init(SellGoods* pShopGoods, EventDelegate* ptrDelegate);
	GoodsDealLayer();
	~GoodsDealLayer();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void dealHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onBuySucceed(Json::Value valRes);
	void onUseSucceed(Json::Value valRes);
	void onUseSSucceed(Json::Value valRes);
	void onMergeSucceed(Json::Value valRes);
	void onExchSucceed(Json::Value valRes);
	void onUseAddAbilityGoodsSuccess(Json::Value valRes);
	void playGoodsNotice(float fDelay);

	void callBackBuy(Ref* sender);
	void callBackSell(Ref* sender);
	void callBackUse(Ref* sender);
	void callBackUses(Ref* sender);
	void callBackMerge(Ref* sender);
	void callBackBreak(Ref* sender);
	void callBackMake(Ref* sender);
	void callBackExch(Ref* sender);
	void callBackSuit(Ref* sender);
private:
	HistoryGoods* m_ptrGoods;
	SellGoods* m_ptrSell;
	EventDelegate* m_pDelegate;
	int m_iKeyId;
	int m_iDealType;
	int m_iTotalNum;
	int m_iPayType;
	int m_iDealPrice;
	int m_iNoticeIndex;
	map<int, string> m_strNotice;
	string m_strPath;

	Sprite* m_pDealBG;
	Menu* m_pMenu;
	MenuItemSprite* m_pBtnPlus;
	MenuItemSprite* m_pBtnMinus;
	Label* m_pNumLab;
	Label* m_pPriceLab;
};

class SellPopupLayer : public BTLayer
{
public:
	SellPopupLayer();
	~SellPopupLayer();
	static SellPopupLayer* create(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate);
	bool init(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate);
private:
	void initControl();
	void updateMenuState();
	void sellHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onSellSucceed(Json::Value valRes);

	void callBackClose(Ref *sender);
	void callBackPlus(Ref* sender);
	void callBackMinus(Ref* sender);
	void callBackMax(Ref* sender);
	void callBackSell(Ref* sender);
private:
	HistoryGoods* m_ptrGoods;
	EventDelegate* m_pDelegate;
	int m_iTotalNum;
	int m_iCurNum;

	Menu* m_pMenu;
	MenuItemSprite* m_btnPlus;
	MenuItemSprite* m_btnMinus;
	MenuItemSprite* m_btnMax;
	Label* m_labNum;
	Label* m_labPrice;
};

class BreakupLayer : public BTLayer
{
public:
	BreakupLayer();
	~BreakupLayer();
	static BreakupLayer* create(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate);
	bool init(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate);
private:
	void initControl();
	void breakHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onBreakSucceed(Json::Value valRes);

	void callBackClose(Ref *sender);
	void callBackBreak(Ref* sender);
private:
	HistoryGoods* m_ptrGoods;
	EventDelegate* m_pDelegate;
};

#endif //_GOODSDEALLAYER_H_