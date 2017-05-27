#ifndef _MENUBARLAYER_H_
#define _MENUBARLAYER_H_

#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "DString.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MenuBarLayer : public BTLayer
{
public:
	virtual bool init(ScreenPtr ptrScreen);
	static MenuBarLayer* create(ScreenPtr ptrScreen);
	MenuBarLayer();
	~MenuBarLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onEnter();
	void onExit();
	static void SetMenuEnabled(Menu* ptrMenu, bool bEnabled);
	static bool getMenuEnabled(Menu* ptrMenu);
private:
	void menuAction(float fDelay);

	void callBackSet(Ref *sender);
	void callBackBack(Ref *sender);
	void callBackCondition(Ref *sender);
	void callBackCollect(Ref *sender);
	void callBackFinish(Ref *sender);
	void callBackAutoRun(Ref *sender);
	void callBackMenu(Ref *sender);
	void callBackWarEnd(Ref* sender);
	void callBackSpeed(Ref *sender);
public:
	Menu* m_pMain;
	Menu* m_pFunc;
private:
	ScreenPtr m_pScreen;
	MenuItemSprite* m_pBtnMenu;
	Sprite* m_spSpeedN;
	Sprite* m_spSpeedS;

	Vec2 m_pMenuPos;
	int m_iMenuWidth;
	int m_iSpeed;
};

#endif //_MENUBARLAYER_H_