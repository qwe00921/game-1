#ifndef __TRGame__SelectActionLayer__
#define __TRGame__SelectActionLayer__

#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "DString.h"

USING_NS_CC;
USING_NS_CC_EXT;
class SelectActionLayer:public BTLayer
{
public:
	virtual bool init(BattleControlerPtr handler);
	static SelectActionLayer* create(BattleControlerPtr handler);
	SelectActionLayer();
	~SelectActionLayer(){}
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void SkillAction(Ref *sender);
	void GoodsAction(Ref *sender);
	void StandAction(Ref *sender);
	void CloseAction(Ref *sender);
	void onExit();
	void setVisible(bool iVisible);
	void setActionButton();
private:
	BattleControlerPtr handler;
	MenuItemSprite* menuClose;
	MenuItemSprite* menuSkill;
	MenuItemSprite* menuGoods;
	MenuItemSprite* menuStand;
	Menu* ptrMenu;
};
#endif