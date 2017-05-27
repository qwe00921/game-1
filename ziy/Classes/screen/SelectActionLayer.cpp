#include "Common.h"
#include "main.h"
#include "SelectActionLayer.h"
#include "BattleControler.h"
#include "BattleScreen.h"
#include "MiniMap.h"
#include "Defines.h"


SelectActionLayer::SelectActionLayer()
{

}
SelectActionLayer* SelectActionLayer::create(BattleControlerPtr handler)
{ 
	SelectActionLayer *pRet = new SelectActionLayer();
	if (pRet && pRet->init(handler))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
bool SelectActionLayer::init(BattleControlerPtr ptrHandler)
{
	if (!Layer::init())
	{
		return false;
	}
	this->handler = ptrHandler;
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	float fScale = 0.90f;
	int iOffset = 90;
	int iHeight = 65;

	Sprite* skill = ReadSpriteName("new_ui/icon_ui/icon9_2.png");
	skill->setScale(fScale);
	menuSkill = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon9_2.png"), skill, ReadSpriteName("new_ui/icon_ui/icon9_3.png"), CC_CALLBACK_1(SelectActionLayer::SkillAction, this));
	skill->setAnchorPoint(Vec2(0.5f, 0.5f));
	skill->setPosition(skill->getPosition() + Vec2(skill->getContentSize().width/ 2, skill->getContentSize().height / 2));
	menuSkill->setPosition(Vec2(0, size.height - iOffset - iHeight));

	Sprite* goods = ReadSpriteName("new_ui/icon_ui/icon9_4.png");
	goods->setScale(fScale);
	menuGoods = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon9_4.png"), goods, ReadSpriteName("new_ui/icon_ui/icon9_5.png"), CC_CALLBACK_1(SelectActionLayer::GoodsAction, this));
	goods->setAnchorPoint(Vec2(0.5f, 0.5f));
	goods->setPosition(goods->getPosition() + Vec2(goods->getContentSize().width / 2, goods->getContentSize().height / 2));
	menuGoods->setPosition(Vec2(0, size.height - iOffset - iHeight * 2));

	Sprite* stand = ReadSpriteName("new_ui/icon_ui/icon9_6.png");
	stand->setScale(fScale);
	menuStand = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon9_6.png"), stand, CC_CALLBACK_1(SelectActionLayer::StandAction, this));
	stand->setAnchorPoint(Vec2(0.5f, 0.5f));
	stand->setPosition(stand->getPosition() + Vec2(stand->getContentSize().width / 2, stand->getContentSize().height / 2));
	menuStand->setPosition(Vec2(0, size.height - iOffset - iHeight * 3));

	Sprite* close = ReadSpriteName("new_ui/icon_ui/icon9_1.png");
	close->setScale(fScale);
	menuClose = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon9_1.png"), close, CC_CALLBACK_1(SelectActionLayer::CloseAction, this));
	close->setAnchorPoint(Vec2(0.5f, 0.5f));
	close->setPosition(close->getPosition() + Vec2(close->getContentSize().width / 2, close->getContentSize().height / 2));
	menuClose->setPosition(Vec2(0, size.height - iOffset));

	menuSkill->setEnabled(false);
	menuGoods->setEnabled(false);

	ptrMenu = Menu::create(menuSkill, menuGoods, menuStand, menuClose,NULL);
	ptrMenu->setPosition(Vec2(size.width - 45, 0));
	this->addChild(ptrMenu);
	ptrMenu->setVisible(false);
	return true;
}
void SelectActionLayer::onExit()
{
	Layer::onExit();
}
bool SelectActionLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return false;
}
void SelectActionLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}
void SelectActionLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}
void SelectActionLayer::SkillAction(Ref *sender)
{
	if (!ptrMenu->isVisible())
		return;
	handler->SelectAction(1);
	setVisible(false);
}
void SelectActionLayer::GoodsAction(Ref *sender)
{
	if (!ptrMenu->isVisible())
		return;
	handler->SelectAction(2);
	setVisible(false);
}

void SelectActionLayer::StandAction(Ref *sender)
{
	if (!ptrMenu->isVisible())
		return;

	handler->ai.ClearGainList();
	handler->SelectAction(3);
	setVisible(false);
}
void SelectActionLayer::CloseAction(Ref *sender)
{
	if (!ptrMenu->isVisible())
		return;
	setVisible(false);
}
void SelectActionLayer::setActionButton()
{
	menuSkill->setEnabled(false);
	menuGoods->setEnabled(false);
	if (history->cur_actor->action_finished || history->cur_actor->state_puzzle || history->cur_actor->state_obfuscation || history->cur_actor->relation != 0)
		return;
	for (int i = 0; i < trData->skill_num; i++)
	{
		if (!history->cur_actor->CurSkillLevelIsNull(i + 1))
		{
			SkillAttrPtr skill = &trData->skills[i];
			menuSkill->setEnabled(true);
			break;
		}
	}

	//禁咒
	if (history->cur_actor->state_stun)
	{
		menuSkill->setEnabled(false);
	}

	map<int, GoodsItem*>::iterator itGoods;
	for(itGoods = trData->m_mGoodsItem.begin(); itGoods != trData->m_mGoodsItem.end(); itGoods++)
	{
		GoodsItem* ptrGoods = itGoods->second;
		if(history->m_mConsuNum[ptrGoods->iGoodsId] != 0 && ptrGoods->iUeid != 28 && !(ptrGoods->iUeid >= 36 && ptrGoods->iUeid <= 40) && ptrGoods->iUeid != 42)
		{
			menuGoods->setEnabled(true);
			break;
		}
	}

	if (history->cur_actor->actor_only_move == 2)
	{
		menuGoods->setEnabled(false);
		menuSkill->setEnabled(false);
	}
	setVisible(true);
}
void SelectActionLayer::setVisible(bool iVisible)
{
	if (!BattleControler::auto_run)
	{
		ptrMenu->setVisible(iVisible);
		handler->scr->setMiniMapVisible(!iVisible);
	}
}