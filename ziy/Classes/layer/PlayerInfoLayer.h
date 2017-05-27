//
//  PlayerInfoLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/01.
//
//

#ifndef _PLAYERINFOLAYER_H_
#define _PLAYERINFOLAYER_H_

#include "Defines.h"

class PlayerInfoLayer : public BTLayer
{
public:
	PlayerInfoLayer();
	~PlayerInfoLayer();
	static PlayerInfoLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateState();
private:
	void initControl();

	void callBackRename(Ref *sender);
	void callBackVip(Ref *sender);
	void callBackSysSet(Ref *sender);
	void callBackBind(Ref *sender);
	void callBackSwitch(Ref *sender);
	void callBackClose(Ref *sender);
private:
	EventDelegate* m_pDelegate;
	bool m_bIsBind;
	Sprite* m_spVIP;
	Label* m_labType;
	Label* m_labName;
	MenuItemSprite* m_btnBind;
	MenuItemSprite* m_btnSwitch;
};

class RenameLayer : public BTLayer, public ui::EditBoxDelegate
{
public:
	RenameLayer();
	~RenameLayer();
	static RenameLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual void editBoxReturn(ui::EditBox* editBox);
private:
	void initControl();

	void onHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onRequestSucceed(Json::Value valRes);

	void callBackRand(Ref *sender);
	void callBackConfirm(Ref *sender);
	void callBackClose(Ref *sender);
private:
	EventDelegate* m_pDelegate;
	ui::EditBox* m_pEditBox;
};

#endif //_PLAYERINFOLAYER_H_
