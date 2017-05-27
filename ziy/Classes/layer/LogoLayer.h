//
//  LogoLayer.h
//  TRGame
//
//  Created by Jimmy on 16/04/11.
//
//

#ifndef _LOGOLAYER_H_
#define _LOGOLAYER_H_

#include "LoginRequest.h"
#include "WaitingLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LogoLayer : public BTLayer
{
public:
	static LogoLayer* create(bool bIsSwitch = false);
	static void setLoginInfo(const char* jsonStr);
	static void javaCallCppBridge(const char* jsonStr);
	static bool getBoolV(const char* key);
	static void setBoolV(const char* key,bool v);
	static int getIntValue(const char* name);
	static std::string getStrValue(const char* name);
	static std::string getDataForSDKAnalys(const char* flag, int level = -1);
	virtual bool init(bool bIsSwitch);
	LogoLayer();
	~LogoLayer();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateState();
	virtual void updateShow(int index);
	virtual void closeLayer(Ref *sender);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void callBackGuest(Ref *sender);
	void callBackUser(Ref *sender);
	void callBackLogin(Ref *sender);
	void callBackNotice(Ref *sender);
private:
	void initControl();
	void updateHeroInfo();
	void getServerState(float fDelay);
	void checkLoginState(float fDelay);
	void updateLoginState();
	void getNetNotice();
	void runHeroAdd(float fDelay);
	void runHeroGo(float fDelay);
private:
	Menu* m_pBtnMenu;
#ifdef WIN32
	MenuItemSprite* m_pBtnGuest;
#endif // WIN32

	
	MenuItemSprite* m_pBtnUser;

	Sprite* m_spCreate;
	Sprite* m_spHeroInfo;
	Sprite* m_spStart;
	Sprite* m_spHeroImg;
	Sprite* m_spWeapon;
	Label* m_labInfo;
	Armature* armBG;

	bool m_bSucceed;
	bool m_bGuestSwitch; //切换为游客登陆
};

class LogoLayerWaiting : public BTLayer
{
public:
	static LogoLayerWaiting *getInstance();
	bool init();
	CREATE_FUNC(LogoLayerWaiting);
	ProgressTimer *m_Progress;
	virtual void update(float dt);
private:
	static LogoLayerWaiting *m_LogoWaiting;
	Menu* m_pFunc;
	Label* m_LogoProgressNum;
	
	
};

#endif //_SGLogoLayer_H_
