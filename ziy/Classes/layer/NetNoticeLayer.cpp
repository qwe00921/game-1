//
//  NetNoticelayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/23.
//
//

#include "NetNoticelayer.h"

NetNoticelayer::NetNoticelayer()
{

}

NetNoticelayer::~NetNoticelayer()
{

}

void NetNoticelayer::onEnter()
{
	Layer::onEnter();
}

void NetNoticelayer::onExit()
{
	Layer::onExit();
}

bool NetNoticelayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if(m_bSupDrag && m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_fOffsetY = touch->getLocation().y;
		m_bIsPress = true;
	}
	return true;
}

void NetNoticelayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (m_bIsPress)
	{
		if(m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			Vec2 pos = m_labContent->getPosition();
			pos.y += touch->getLocation().y - m_fOffsetY;
			m_fOffsetY = touch->getLocation().y;
			if(pos.y > m_rcDraw.size.height && pos.y < m_labContent->getContentSize().height)
			{
				m_labContent->setPosition(pos);
			}
		}
		else
		{
			m_fOffsetY = 0;
			m_bIsPress = false;
		}
	}
}

void NetNoticelayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsPress)
	{
		m_fOffsetY = 0;
		m_bIsPress = false;
	}
}

NetNoticelayer* NetNoticelayer::create()
{
	NetNoticelayer* pRet = new NetNoticelayer();
	if(pRet&&pRet->init())
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

bool NetNoticelayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_bSupDrag = false;
	m_bIsPress = false;
	m_fOffsetY = 0;

	initControl();
	return true;
}

void NetNoticelayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/img_global/mail_bg4.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题背景
	//Sprite* spTitle = ReadSpriteName("new_ui/img_global/mail_bg5.png");
	//spTitle->setPosition(point + Vec2(0, 170));
	//this->addChild(spTitle);

	//标题
	Label* labTitle = LabelSystemFont(trData->m_pNetNotice->strTitle.c_str(), 18);
	labTitle->setPosition(point + Vec2(0, 170));
	labTitle->setColor(Color3B::YELLOW);
	this->addChild(labTitle);

	//绘制裁剪区域
	DrawNode* dnShap = DrawNode::create();
	Vec2 rect[4] = {Vec2(0, 225), Vec2(330, 225), Vec2(330, 0), Vec2(0, 0)};
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	ClippingNode* clipNode = ClippingNode::create();
	clipNode->setStencil(dnShap);
	clipNode->setPosition(point + Vec2(-163, 115 - 225+11));
	m_rcDraw = clipNode->getBoundingBox();
	m_rcDraw.size.width = 340;
	m_rcDraw.size.height = 200;
	this->addChild(clipNode);

	//邮件正文
	m_labContent = LabelSystemFont(trData->m_pNetNotice->strContent.c_str(), 16);
	m_labContent->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_labContent->setPosition(Vec2(0, 225));
	m_labContent->setDimensions(330, 0);
	m_labContent->setHorizontalAlignment(TextHAlignment::LEFT);
	clipNode->addChild(m_labContent);
	m_bSupDrag = m_labContent->getContentSize().height > m_rcDraw.size.height ? true : false;

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spCloseTextN = ReadSpriteName("new_ui/img_text/mail_txt3.png");
	spCloseTextN->setPosition(Vec2(spCloseN->getContentSize().width / 2, spCloseN->getContentSize().height / 2));
	spCloseN->addChild(spCloseTextN);
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spCloseTextS = ReadSpriteName("new_ui/img_text/mail_txt4.png");
	spCloseTextS->setPosition(Vec2(spCloseS->getContentSize().width / 2, spCloseS->getContentSize().height / 2));
	spCloseS->addChild(spCloseTextS);
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(NetNoticelayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(0, -158+30-8));
	m_pBtnMenu->addChild(btnClose);

	//关闭
	Sprite* spCancelN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCancelS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnCancel = MenuItemSprite::create(spCancelN, spCancelS, CC_CALLBACK_1(NetNoticelayer::callBackClose, this));
	btnCancel->setPosition(point + Vec2(210, 168));
	m_pBtnMenu->addChild(btnCancel);
}

void NetNoticelayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}