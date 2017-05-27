//
//  BaseClass.cpp
//  TRGame
//
//  Created by Jimmy on 16/07/09.
//
//

#include "BaseClass.h"
#include "ExitGameLayer.h"
#include "otherfunction.h"
#include "LogoLayer.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BTSprite::BTSprite()
{
	m_ptrLisTouch = nullptr;

	EventListenerTouchOneByOne* ptrLisTouch = EventListenerTouchOneByOne::create();
	ptrLisTouch->setSwallowTouches(false);
	ptrLisTouch->onTouchBegan = CC_CALLBACK_2(BTSprite::onTouchBegan, this);
	ptrLisTouch->onTouchMoved = CC_CALLBACK_2(BTSprite::onTouchMoved, this);
	ptrLisTouch->onTouchEnded = CC_CALLBACK_2(BTSprite::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ptrLisTouch, this);
	m_ptrLisTouch = ptrLisTouch;
}

BTSprite::~BTSprite()
{
	if(m_ptrLisTouch)
	{
		_eventDispatcher->removeEventListener(m_ptrLisTouch);
	}
}

void BTSprite::onEnter()
{
	Sprite::onEnter();
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(m_ptrLisTouch, this);
}

void BTSprite::onExit()
{
	Sprite::onExit();
	//_eventDispatcher->removeEventListener(m_ptrLisTouch);
}

bool BTSprite::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void BTSprite::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void BTSprite::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void BTSprite::setSwallow(bool bSwallow)
{
	if(m_ptrLisTouch)
	{
		EventListenerTouchOneByOne* ptrLisTouch = (EventListenerTouchOneByOne*)m_ptrLisTouch;
		ptrLisTouch->setSwallowTouches(bSwallow);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BTLayer::BTLayer(bool bListener /*= true*/)
{
	m_ptrLisTouch = nullptr;

	if (bListener)
	{
		EventListenerTouchOneByOne* ptrLisTouch = EventListenerTouchOneByOne::create();
		ptrLisTouch->setSwallowTouches(true);
		ptrLisTouch->onTouchBegan = CC_CALLBACK_2(Layer::onTouchBegan, this);
		ptrLisTouch->onTouchMoved = CC_CALLBACK_2(Layer::onTouchMoved, this);
		ptrLisTouch->onTouchEnded = CC_CALLBACK_2(Layer::onTouchEnded, this);
		ptrLisTouch->onTouchCancelled = CC_CALLBACK_2(Layer::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(ptrLisTouch, this);
		m_ptrLisTouch = ptrLisTouch;
	}

	_keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(Layer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 1);
}

BTLayer::~BTLayer()
{
	if (m_ptrLisTouch)
	{
		_eventDispatcher->removeEventListener(m_ptrLisTouch);
	}

	if (_keyboardListener)
	{
		_eventDispatcher->removeEventListener(_keyboardListener);
	}
}

void BTLayer::onEnter()
{
	Layer::onEnter();
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(m_ptrLisTouch, this);
	//_eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 1);
}

void BTLayer::onExit()
{
	Layer::onExit();
	//_eventDispatcher->removeEventListener(m_ptrLisTouch);
	//_eventDispatcher->removeEventListener(_keyboardListener);
}

bool BTLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void BTLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void BTLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void BTLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		int iLoginId = history->m_pLoginInfo->getLoginId();
		if (iLoginId >= LOGIN_UC && (iLoginId!= 11)) {//11.huawei
			if (LogoLayer::getBoolV("SDKDialogExit") == false)
			{
				Json::Value callStr;
				callStr["name"] = "startSDKQuitGame";
				cppBridgeCall(callStr.toStyledString().c_str());
				LogoLayer::setBoolV("SDKDialogExit", true);
			}
		}
		else {

			if( !ExitGameLayer::getInstance()->isRunning())
				Director::getInstance()->getRunningScene()->addChild(ExitGameLayer::getInstance(), 999);
		}
	}

	Layer::onKeyReleased(keyCode, event);
}

void BTLayer::setSwallow(bool bSwallow)
{
	if (m_ptrLisTouch)
	{
		EventListenerTouchOneByOne* ptrLisTouch = (EventListenerTouchOneByOne*)m_ptrLisTouch;
		ptrLisTouch->setSwallowTouches(bSwallow);
	}
}
