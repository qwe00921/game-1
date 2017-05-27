//
//  ActivityInfoLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/14.
//
//

#ifndef _ACTIVITYINFOLAYER_H_
#define _ACTIVITYINFOLAYER_H_

#include "Defines.h"
#include "MailLayer.h"

class ActExchLayer : public Layer, public  ui::EditBoxDelegate
{
public:
	ActExchLayer();
	~ActExchLayer();
	static ActExchLayer* create(string strContent, Rect rcDraw, EventDelegate* ptrDelegate);
	virtual bool init(string strContent, Rect rcDraw, EventDelegate* ptrDelegate);
	virtual void editBoxReturn(ui::EditBox* editBox);

	float getOffsetY();
private:
	void initControl();

	void callBackConfirm(Ref *sender);
public:
	static string ms_strCode;
private:
	string m_strContent;
	Rect m_rcDraw;
	float m_fOffsetY;
	EventDelegate* m_pDelegate;
	ui::EditBox* m_pEditBox;
};

class ActGiftLayer : public BTLayer
{
public:
	static ActGiftLayer* create(int iActId, EventDelegate* ptrDelegate);
	virtual bool init(int iActId, EventDelegate* ptrDelegate);
	ActGiftLayer();
	~ActGiftLayer();
	virtual void setVisible(bool visible);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateState();
	virtual void RunFunction(int iNomal);
	virtual void updateParentShow();
	void updateRTDate(map<int, std::string> mString);
private:
	void initControl();
	void updateDisplay();
	void playGoodsNotice(float fDelay);

	void callBackRmbGift(Ref *sender);
private:
	int m_iActId;
	EventDelegate* m_pDelegate;
	MenuItemSprite* m_btnGift;
	Layer* m_ptrLayer;
	int m_iOffsetY;
	bool m_bIsPress;
	float m_fTouchY;
	bool m_bIsDrag;
	Rect m_rcDraw;

	map<int, string> m_strNotice;
	int m_iNoticeIndex;
};

class ActWXGiftLayer : public BTLayer
{
public:
	static ActWXGiftLayer* create(int iActId, EventDelegate* ptrDelegate);
	virtual bool init(int iActId, EventDelegate* ptrDelegate);
	ActWXGiftLayer();
	~ActWXGiftLayer();
	virtual void setVisible(bool visible);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onHttpRequest(int iAid,int iGid);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
private:
	void initControl();
	void updateDisplay();
	void playGoodsNotice(float fDelay);

	void callBackRmbGift(Ref *sender);
private:
	int m_iActId;
	EventDelegate* m_pDelegate;
	MenuItemSprite* m_btnGift;
	Layer* m_ptrLayer;
	int m_iOffsetY;
	bool m_bIsPress;
	float m_fTouchY;
	bool m_bIsDrag;
	Rect m_rcDraw;
	Sprite* m_spReceived;

	map<int, string> m_strNotice;
	int m_iNoticeIndex;
};
class ActivityElem;
class ActivityInfoLayer : public BTLayer
{
public:
	static ActivityInfoLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	ActivityInfoLayer();
	~ActivityInfoLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void callBackConfirm();
	virtual void updateState();

	void updateActLayer(int iActId);
private:
	void initControl();
	Label* updateLabNum(int iGoodsId, int iCount);
	void updateExchLayer();
	void updateRecharge(); //购买新手礼包后刷新充值活动界面
	void updataActState(int iActId, int iElemId, int iState);
	void playGoodsNotice(float fDelay);

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onActivitySucceed(Json::Value valRes);

	void callBackClose(Ref *sender);
	void callBackTake(Ref *sender);
	void callBackExchange(Ref *sender);
	void callBackModel(Ref *sender);
	void callBackCard(Ref *sender);
	void callBackToPay(Ref *sender);
	Menu* createToBuyBtn(ActivityElem* ptrActivity);
private:
	int m_iActId;
	int m_iClickIndex;

	EventDelegate* m_pDelegate;
	map<int, string> m_strNotice;
	int m_iNoticeIndex;
	bool m_bIsPress;
	float m_fTouchY;

	map<int, Layer*> m_mLayer;
	map<int, map<int, map<int, Node*> > > m_layElem;
	map<int, map<int, map<int, Label*> > > m_mLabNum;
	map<int, float> m_mOffsetY;
	map<int, bool> m_mIsDrag;
	Rect m_rcDraw;
};

#endif //_ACTIVITYINFOLAYER_H_