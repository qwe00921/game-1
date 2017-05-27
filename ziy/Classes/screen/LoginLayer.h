//
//  LoginLayer.h
//  TRGame
//
//  Created by Jimmy on 16/4/18.
//
//

#ifndef _LOGINLAYER_H_
#define _LOGINLAYER_H_

#include "Defines.h"
#include "DString.h"

class LoginLayer : public BTLayer, public ui::EditBoxDelegate
{
public:
	static LoginLayer* create(EventDelegate* ptrDelegate, bool bIsBind = false);
	virtual bool init(EventDelegate* ptrDelegate, bool bIsBind);
    LoginLayer();
    ~LoginLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void editBoxReturn(ui::EditBox* editBox);

	static void onHttpReqWXQQLogin(string loginno, string accounttype);
	static void showLoginWarn(string strMSG); //提示第三方登陆错误
	static void PlatRemoveWaitSprite();//移除第三方登陆等待图标
	void onHttpRequestCompleted(cocos2d::Node *sender, void *data);
	void removeWaitLayer(float fDelay);
private:
	void initControl();
	bool checkDataUsable(string strData);
	void updateBtnClean();
	void updateBind();

	void callBackUserClean(Ref *sender);
	void callBackPassClean(Ref *sender);
	void callBackRegister(Ref *sender);
	void callBackRequest(Ref *sender);
	void callBackQQLogin(Ref *sender);
	void callBackWXLogin(Ref *sender);
	void callBackClose(Ref *sender);
private:
	EventDelegate* m_pDelegate;
	int m_iCurLogin;
	MenuItemSprite* m_btnRegister;
	MenuItemSprite* m_btnLogin;
	MenuItemSprite* m_btnRegId;
	MenuItemSprite* m_btnBind;
	MenuItemSprite* m_btnQQ;
	MenuItemSprite* m_btnWX;
	Sprite* m_spOther;
	Sprite* m_spNotice;
	Sprite* m_spBind;
	int m_iPage;
	map<int, ui::EditBox*> m_mEditBox;
	map<int, MenuItemSprite*> m_mClean;
};

#endif //_LOGINLAYER_H_
