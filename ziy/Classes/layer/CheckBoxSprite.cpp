//
//  CheckBoxSprite.cpp
//  TRGame
//
//  Created by Jimmy on 16/5/31.
//
//

#include "CheckBoxSprite.h"

CheckBoxSprite* CheckBoxSprite::create(Label* labText, Ref* target, SEL_MenuHandler selector)
{
	CheckBoxSprite *pRet = new CheckBoxSprite();
	if(pRet && pRet->init(labText, target, selector))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool CheckBoxSprite::init(Label* labText, Ref* target, SEL_MenuHandler selector)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_spCheckN = ReadSpriteName("new_ui/battle/box_no.png");
	m_spCheckS = ReadSpriteName("new_ui/battle/box_ok.png");
	m_labText = labText;
	m_pListener = target;
	m_pfnSelector = selector;

	initControl();
	return true;
}

CheckBoxSprite* CheckBoxSprite::create(Sprite* spCheckN, Sprite* spCheckS, Label* labText, Ref* target, SEL_MenuHandler selector)
{
	CheckBoxSprite *pRet = new CheckBoxSprite();
	if(pRet && pRet->init(spCheckN, spCheckS, labText, target, selector))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool CheckBoxSprite::init(Sprite* spCheckN, Sprite* spCheckS, Label* labText, Ref* target, SEL_MenuHandler selector)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_spCheckN = spCheckN;
	m_spCheckS = spCheckS;
	m_labText = labText;
	m_pListener = target;
	m_pfnSelector = selector;

	initControl();
	return true;
}


void CheckBoxSprite::onEnter()
{
	Sprite::onEnter();
    initTouches();
}

void CheckBoxSprite::onExit()
{
	Sprite::onExit();
}

void CheckBoxSprite::initControl()
{
	m_bIsClick = false;
	m_bChecked = false;

	//常态
	this->addChild(m_spCheckN);

	//选中态
	this->addChild(m_spCheckS);

	//文本
	if (m_labText)
	{
		m_labText->setAnchorPoint(Vec2(0.0f, 0.5f));
		m_labText->setPosition(Vec2(28, 0));
		m_labText->setColor(Color3B(102, 0, 0));
		this->addChild(m_labText);
	}

	updateCheck();
}

void CheckBoxSprite::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        Rect rect = m_spCheckN->getBoundingBox();
        Vec2 point = convertToNodeSpace(touch->getLocation());

		if(rect.containsPoint(point) && this->isVisible())
        {
            m_bIsClick = true;
        }
        return true;
    };
    listener->onTouchMoved = [&](Touch* touch, Event* event) {
        if (m_bIsClick)
        {
            Rect rect = m_spCheckN->getBoundingBox();
            if(!rect.containsPoint(convertToNodeSpace(touch->getLocation())))
            {
                m_bIsClick = false;
            }
        }
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event) {
        if (m_bIsClick)
        {
            m_bChecked = !m_bChecked;
            updateCheck();
            if(m_pListener && m_pfnSelector)
            {
                (m_pListener->*m_pfnSelector)(this);
            }
            m_bIsClick = false;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CheckBoxSprite::updateCheck()
{
	m_spCheckN->setVisible(!m_bChecked);
	m_spCheckS->setVisible(m_bChecked);
}

void CheckBoxSprite::setTextColor(const Color3B& color3)
{
	m_labText->setColor(color3);
}

void CheckBoxSprite::setChecked(bool bChecked)
{
	m_bChecked = bChecked;
	updateCheck();
}

bool CheckBoxSprite::getChecked()
{
	return m_bChecked;
}
