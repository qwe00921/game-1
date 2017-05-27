//
//  NewLoginLayer.h
//  TRGame
//
//  Created by Jimmy on 16/04/09.
//
//

#ifndef _NEWLOGINLAYER_H_
#define _NEWLOGINLAYER_H_

USING_NS_CC;
USING_NS_CC_EXT;

class GuestLayer : public BTLayer
{
public:
	static GuestLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	GuestLayer();
	~GuestLayer();
	virtual void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onHttpRequestCompleted(Node *sender, void *resultdata);
private:
	void initControl();
	void callBackGuest(Ref *sender);
	void callBackUser(Ref *sender);
	void callBackClose(Ref *sender);
private:
	EventDelegate* m_pDelegate;
	Menu* m_pBtnMenu;
};

class LoginSwitchLayer : public BTLayer
{
public:
	LoginSwitchLayer();
	~LoginSwitchLayer();
	static LoginSwitchLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void callBackSwitch(Ref *sender);
	void callBackClose(Ref *sender);
private:
	EventDelegate* m_pDelegate;
	Menu* m_pBtnMenu;
};

#endif //_NEWLOGINLAYER_H_
