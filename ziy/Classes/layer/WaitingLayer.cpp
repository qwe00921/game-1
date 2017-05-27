//
//  WaitingLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/18.
//
//


#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "WaitingLayer.h"
#include "GuideLayer.h"

WaitingLayer::WaitingLayer()
{

}

WaitingLayer::~WaitingLayer()
{

}

void WaitingLayer::onEnter()
{
	Layer::onEnter();
}

void WaitingLayer::onExit()
{
	Layer::onExit();
	
}

bool WaitingLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void WaitingLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void WaitingLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

WaitingLayer* WaitingLayer::create(bool bIsDelay /*= true*/)
{
	WaitingLayer* pRet = new WaitingLayer();
	if (pRet&&pRet->init(bIsDelay))
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

bool WaitingLayer::init(bool bIsDelay)
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();

	if (bIsDelay)
	{
		scheduleOnce(schedule_selector(WaitingLayer::addWaitLayer), 1.0f);
	}
	else
	{
		addWaitLayer(0);
	}

	return true;
}

void WaitingLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();

	//Touch屏蔽层
	MenuItem* btnNull = MenuItem::create(CC_CALLBACK_1(WaitingLayer::callBackNull, this));
	btnNull->setContentSize(size);
	btnNull->setPosition(Vec2(size.width / 2, size.height / 2));
	m_menuNull = Menu::create(btnNull, NULL);
	m_menuNull->setPosition(Vec2(0, 0));
	this->addChild(m_menuNull, 8);
}

void WaitingLayer::addWaitLayer(float fDelay)
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//连接中
	Sprite* spConnect = ReadSpriteName("new_ui/loading1.png");
	spConnect->setPosition(point);
	this->addChild(spConnect);

	//等待
	Sprite* spWait = ReadSpriteName("new_ui/loading2.png");
	spWait->setPosition(point);
	this->addChild(spWait);

	//等待动作
	RotateBy* action = RotateBy::create(1.5f, 360.0f);
	spWait->runAction(RepeatForever::create(action));
}

void WaitingLayer::callBackNull(Ref *sender)
{
	//屏蔽点击事件，不做任何处理
}
