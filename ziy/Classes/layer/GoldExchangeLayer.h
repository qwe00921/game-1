//
//  GoldExchangeLayer.h
//  TRGame
//
//  Created by Jimmy on 16/05/30.
//
//

#ifndef _GOLDEXCHANGELAYER_H_
#define _GOLDEXCHANGELAYER_H_

#include "Defines.h"

class GoldExchangeLayer : public BTLayer
{
public:
	static GoldExchangeLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	GoldExchangeLayer();
	~GoldExchangeLayer();
	virtual void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void updateExchange();
	void callBackConfirm(Ref *sender);
	void callBackCancel(Ref *sender);

	void ExchangHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onBuySucceed(Json::Value valRes);
private:
	EventDelegate* m_pDelegate;
	Menu* m_pBtnMenu;
	Label* m_labTimes;
	Label* m_labJade;
	Label* m_labGold;

	int m_iMaxTimes;
	int m_iCurTimes;
};

#endif //_GOLDEXCHANGELAYER_H_