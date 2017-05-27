//
//  MailInfoLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/07.
//
//

#ifndef _MAILINFOLAYER_H_
#define _MAILINFOLAYER_H_

#include "Defines.h"
#include "MailLayer.h"

class MailInfoLayer : public BTLayer
{
public:
	static MailInfoLayer* create(MailElem* ptrMail, EventDelegate* ptrDelegate);
	virtual bool init(MailElem* ptrMail, EventDelegate* ptrDelegate);
	MailInfoLayer();
	~MailInfoLayer();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();

	void callBackClose(Ref *sender);
	void callBackTake(Ref *sender);

	void mailHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onMailSucceed(Json::Value valRes);
	void playGoodsNotice(float fDelay);
private:
	MailElem* m_ptrMail;
	EventDelegate* m_pDelegate;
	Menu* m_pBtnMenu;
	Label* m_labContent;
	Label* m_labSender;

	map<int, string> m_strNotice;
	int m_iNoticeIndex;
	Rect m_rcDraw;
	bool m_bSupDrag; //文字较长支持拖动
	bool m_bIsPress;
	float m_fOffsetY;
};

#endif //_MAILINFOLAYER_H_