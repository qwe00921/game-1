//
//  BattleLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/07/14.
//
//

#include "Common.h"
#include "main.h"
#include "BattleLayer.h"
#include "BattleSprite.h"
#include "BattleScreen.h"

BattleLayer::BattleLayer()
{
	setSwallow(false);
}

BattleLayer::~BattleLayer()
{

}

bool BattleLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (history->m_iBattleType != BATTLE_PVP)
	{
		scheduleOnce(schedule_selector(BattleLayer::gameSpeedUp), 0.2f);
	}
	return true;
}

void BattleLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void BattleLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bSpeedUp)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		m_bSpeedUp = false;
	}
	else
	{
		unschedule(schedule_selector(BattleLayer::gameSpeedUp));
	}
}

bool BattleLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_bSpeedUp = false;

	initControl();

	return true;
}

void BattleLayer::initControl()
{

}

void BattleLayer::gameSpeedUp(float fDelay)
{
	Director::getInstance()->getScheduler()->setTimeScale(5.0f);
	BattleScreen* ptrScreen = (BattleScreen*)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
	if(ptrScreen)
	{
		//ptrScreen->updateWeatherScale(1.0f);
	}
	m_bSpeedUp = true;
}
