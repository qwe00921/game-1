//
//  DailySignLayer.h
//  TRGame
//
//  Created by Jimmy on 16/09/06.
//
//

#ifndef _DailySignLayer_H_
#define _DailySignLayer_H_

#include "Defines.h"
#include "GoodsSprite.h"

class DailySignLayer : public BTLayer
{
public:
	CREATE_FUNC(DailySignLayer);
	virtual bool init();
	DailySignLayer();
	~DailySignLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void callBackConfirm();
private:
	void initControl();
	void initGoodsItem();
	void updateShow();
	void updateItem();
	void playGoodsNotice(float fDelay);

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);

	void callBackSign(Ref *sender);
	void callBackResign(Ref *sender);
	void callBackClose(Ref *sender);
private:
	map<int, GoodsSprite*> m_mGoodsItem;
	MenuItemSprite* m_btnResign;
	Label* m_labTitle;
	Label* m_labResign;
	Sprite* m_spSelect;
	int m_iNoticeIndex;
	map<int, string> m_strNotice;
};

#endif //_DailySignLayer_H_