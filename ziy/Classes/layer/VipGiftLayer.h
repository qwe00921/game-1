//
//  VipGiftLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/13.
//
//

#ifndef _VIPGIFTLAYER_H_
#define _VIPGIFTLAYER_H_

#include "Defines.h"

class VipGiftLayer : public BTLayer
{
public:
	static VipGiftLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateMoneyShow();
private:
	void initControl();
	void updateVipLayer();

	void callBackBuy(Ref *sender);
	void callBackClose(Ref *sender);

	void vipDateHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onVipDateSucceed(Json::Value valRes);
private:
	EventDelegate* m_pDelegate;
	Label* m_labText;
	Label* m_labDate;
	map<int, MenuItemSprite*> m_mbtnBuy;
};

#endif //_VIPGIFTLAYER_H_
