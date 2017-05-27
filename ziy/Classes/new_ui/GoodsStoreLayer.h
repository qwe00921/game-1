//
//  GoodsStoreLayer.h
//  TRGame
//
//  Created by Jimmy on 16/11/04.
//
//

#ifndef _GoodsStoreLayer_H_
#define  _GoodsStoreLayer_H_

#include "Defines.h"
#include "GoodsShopLayer.h"

class StoreItem
{
public:
	StoreItem();
	~StoreItem();
	bool loadJson(Json::Value valJson);
	string getIconPath();
	int getIntTime(string strTime);
	void calcuDiscount(int iCurTime); //计算折扣

	CC_SYNTHESIZE(int, m_iGoodsId, GoodsId); //物品ID
	CC_SYNTHESIZE(int, m_iCount, Count); //数目
	CC_SYNTHESIZE(int, m_iType, PayType); //支付类型
	CC_SYNTHESIZE(int, m_iPrice, Price); //支付类型
	CC_SYNTHESIZE(int, m_iDiscount, Discount); //折扣
	CC_SYNTHESIZE(string, m_strName, Name); //物品名
	CC_SYNTHESIZE(string, m_strStart, StartTime); //开始时间
	CC_SYNTHESIZE(string, m_strEnd, EndTime); //结束时间
};

class StoreInfo
{
public:
	StoreInfo();
	~StoreInfo();
	bool loadJson(Json::Value valJson);
	void clearJson();

	CC_SYNTHESIZE(double, m_dCurTime, CurTime); //物品ID
	CC_SYNTHESIZE(string, m_strContent, Content); //提示信息
	map<string, map<int, StoreItem*> > m_mStoreItem;
};

class GoodsStoreLayer : public BTLayer
{
public:
	GoodsStoreLayer();
	~GoodsStoreLayer();
	CREATE_FUNC(GoodsStoreLayer);
	virtual bool init();
	virtual void updateShow(int index);
	virtual void callBackListClick(int iIndex);
	virtual void updateMoneyShow();
private:
	void initControl();
	void initGoodsMap();
	void updateDisplay();

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onStoreInitSucceed(Json::Value valRes);

	void callBackClose(Ref *sender);
	void callBackSwitch(Ref* sender);
	void callBackJade(Ref *sender);
private:
	StoreInfo* m_ptrStore;
	int m_iCurIndex;

	Sprite* m_spLongBG;
	Label* m_labJade;
	Label* m_labLong;
	Label* m_labNotice;
	GoodsTableView* m_mTVGoods;
	MenuItemSprite* m_btnJade;
	map<int, MenuItemSprite*> m_mBtnType;
	map<int, map<int, SellGoods*> > m_mSellGoods;
};

#endif // _GoodsStoreLayer_H_