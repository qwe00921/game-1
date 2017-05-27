#ifndef _REDEMENULAYER_H_
#define _REDEMENULAYER_H_

#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "DString.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum
{
	SHOW_NONE,
	SHOW_MENU,
	SHOW_ALL,
};

class RedeMenuLayer : public BTLayer
{
public:
	virtual bool init(ScreenPtr ptrScreen);
	static RedeMenuLayer* create(ScreenPtr ptrScreen);
	RedeMenuLayer();
	~RedeMenuLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onEnter();
	void onExit();
	void setLayerVisible(int iType);
	void setNewGoodsIconVisible(bool bVisible);
private:
	void callBackSend(Ref *sender);
	void callBackEquip(Ref *sender);
	void callBackForge(Ref *sender);
	void callBackFruit(Ref *sender);
	void callBackSell(Ref *sender);
	void callBackBuy(Ref *sender);
	void callBackMall(Ref *sender);
	void callBackMenu(Ref *sender);
public:
	Menu* m_pMain;
	Menu* m_pFunc;
private:
	ScreenPtr m_pScreen;
	Sprite* NewGoodsIcon;
};

#endif //_REDEMENULAYER_H_