//
//  MailInfoLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/07.
//
//

#include "MailInfoLayer.h"
#include "GoodsSprite.h"
#include "GuideLayer.h"

#define MAX_GOODS_NUM 6

MailInfoLayer::MailInfoLayer()
{

}

MailInfoLayer::~MailInfoLayer()
{

}

void MailInfoLayer::onEnter()
{
	Layer::onEnter();
}

void MailInfoLayer::onExit()
{
	Layer::onExit();
}

bool MailInfoLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if(m_bSupDrag && m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_fOffsetY = touch->getLocation().y;
		m_bIsPress = true;
	}
	return true;
}

void MailInfoLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (m_bIsPress)
	{
		if(m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			Vec2 pos1 = m_labContent->getPosition();
			Vec2 pos2 = m_labSender->getPosition();
			pos1.y += touch->getLocation().y - m_fOffsetY;
			pos2.y += touch->getLocation().y - m_fOffsetY;
			m_fOffsetY = touch->getLocation().y;
			if(pos1.y > m_rcDraw.size.height && pos2.y < m_labSender->getContentSize().height)
			{
				m_labContent->setPosition(pos1);
				m_labSender->setPosition(pos2);
			}
		}
		else
		{
			m_fOffsetY = 0;
			m_bIsPress = false;
		}
	}
}

void MailInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsPress)
	{
		m_fOffsetY = 0;
		m_bIsPress = false;
	}
}

MailInfoLayer* MailInfoLayer::create(MailElem* ptrMail, EventDelegate* ptrDelegate)
{
	MailInfoLayer* pRet = new MailInfoLayer();
	if(pRet&&pRet->init(ptrMail, ptrDelegate))
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

bool MailInfoLayer::init(MailElem* ptrMail, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrMail = ptrMail;
	m_pDelegate = ptrDelegate;
	m_iNoticeIndex = 0;
	m_bSupDrag = false;
	m_bIsPress = false;
	m_fOffsetY = 0;

	initControl();
	history->updateGuide(EMIL_GUIDE);

	if(m_ptrMail->getType() == MAIL_MSG && !m_ptrMail->getIsRead())
	{
		mailHttpRequest();
	}
	return true;
}

void MailInfoLayer::initControl()
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
	//spTitle->setPosition(point + Vec2(0, 140+30));
	//this->addChild(spTitle);

	//标题
	Label* labTitle = LabelSystemFont(m_ptrMail->getTitle().c_str(), 18);
	labTitle->setPosition(point + Vec2(0, 140+26));
	labTitle->setColor(Color3B::YELLOW);
	this->addChild(labTitle);

	//绘制裁剪区域
	DrawNode* dnShap = DrawNode::create();
	int iHeight = m_ptrMail->getType() == MAIL_MSG ? 225 : 130;
	Vec2 rect[4] = {Vec2(0, iHeight), Vec2(330, iHeight), Vec2(330, 0), Vec2(0, 0)};
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	ClippingNode* clipNode = ClippingNode::create();
	clipNode->setStencil(dnShap);
	clipNode->setPosition(point + Vec2(-163, 115 - iHeight));
	m_rcDraw = clipNode->getBoundingBox();
	m_rcDraw.size.width = 330;
	m_rcDraw.size.height = iHeight;
	this->addChild(clipNode);

	//邮件正文
	m_labContent = LabelSystemFont(m_ptrMail->getContent().c_str(), 16);
	m_labContent->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_labContent->setPosition(Vec2(0, iHeight));
	m_labContent->setDimensions(330, 0);
	m_labContent->setHorizontalAlignment(TextHAlignment::LEFT);
	clipNode->addChild(m_labContent);

	//发件人
	__String* strSender = __String::createWithFormat(LL("pozhehao"), m_ptrMail->getSender().c_str());
	m_labSender = LabelSystemFont(strSender->getCString(), 16);
	m_labSender->setAnchorPoint(Vec2(1.0f, 1.0f));
	int iTotalHeight = m_labContent->getContentSize().height + m_labSender->getContentSize().height;
	m_labSender->setPosition(Vec2(330, iHeight - iTotalHeight));
	clipNode->addChild(m_labSender);
	m_bSupDrag = iTotalHeight + m_labSender->getContentSize().height > iHeight ? true : false;

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	if(m_ptrMail->getType() == MAIL_MSG)
	{
		//关闭
		Sprite* spCloseN = ReadSpriteName("new_ui/img_global/global_ban1.png");
		Sprite* spCloseTextN = ReadSpriteName("new_ui/img_text/mail_txt3.png");
		spCloseTextN->setPosition(Vec2(spCloseN->getContentSize().width / 2, spCloseN->getContentSize().height / 2));
		spCloseN->addChild(spCloseTextN);
		Sprite* spCloseS = ReadSpriteName("new_ui/img_global/global_ban2.png");
		Sprite* spCloseTextS = ReadSpriteName("new_ui/img_text/mail_txt4.png");
		spCloseTextS->setPosition(Vec2(spCloseS->getContentSize().width / 2, spCloseS->getContentSize().height / 2));
		spCloseS->addChild(spCloseTextS);
		MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(MailInfoLayer::callBackClose, this));
		btnClose->setPosition(point + Vec2(0, -158));
		m_pBtnMenu->addChild(btnClose);
	}
	else if (m_ptrMail->getType() == MAIL_GOODS)
	{
		//分割线
		Sprite* spLine = ReadSpriteName("new_ui/img_global/mail_line1.png");
		spLine->setPosition(point + Vec2(0, -30));
		this->addChild(spLine);

		//附件
		Label* labAppend = LabelSystemFont(LL("mailappend"), 16);
		labAppend->setAnchorPoint(Vec2(0.0f, 1.0f));
		labAppend->setPosition(point + Vec2(-163, -40+30));
		labAppend->setColor(Color3B::YELLOW);
		this->addChild(labAppend);

		map<int, int> mGoodsMap = m_ptrMail->m_mGoodsMap;
		int iIndex = 0;
		for(map<int, int>::iterator it = mGoodsMap.begin(); it != mGoodsMap.end(); it++)
		{
			if(iIndex < MAX_GOODS_NUM)
			{
				int iGoodsId = it->first;
				int iCount = it->second;
				Vec2 pos = point + Vec2(-138 + 56 * iIndex++, -90+30);

				GoodsSprite* spItem = GoodsSprite::create(iGoodsId, iCount);
				spItem->setPosition(pos);
				this->addChild(spItem);
			}
		}

		//领取
		Sprite* spTakeN = ReadSpriteName("new_ui/img_global/global_ban1.png");
		Sprite* spTakeTextN = ReadSpriteName("new_ui/img_text/mail_txt1.png");
		spTakeTextN->setPosition(Vec2(spTakeN->getContentSize().width / 2, spTakeN->getContentSize().height / 2));
		spTakeN->addChild(spTakeTextN);
		Sprite* spTakeS = ReadSpriteName("new_ui/img_global/global_ban2.png");
		Sprite* spTakeTextS = ReadSpriteName("new_ui/img_text/mail_txt2.png");
		spTakeTextS->setPosition(Vec2(spTakeS->getContentSize().width / 2, spTakeS->getContentSize().height / 2));
		spTakeS->addChild(spTakeTextS);
		MenuItemSprite* btnTake = MenuItemSprite::create(spTakeN, spTakeS, CC_CALLBACK_1(MailInfoLayer::callBackTake, this));
		btnTake->setPosition(point + Vec2(0, -158+30));
		m_pBtnMenu->addChild(btnTake);
	}

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(MailInfoLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(200+13, 175-25));
	m_pBtnMenu->addChild(btnClose);
}

void MailInfoLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void MailInfoLayer::callBackTake(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	mailHttpRequest();
}

void MailInfoLayer::mailHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("emailStatuID=%s&gameToken=%s", m_ptrMail->getMailId().c_str(), strToken.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"emailStatuID\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrMail->getMailId().c_str(), strToken.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("setEmailStatu",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(MailInfoLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void MailInfoLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		onMailSucceed(value["result"]);
	}
}

bool MailInfoLayer::onMailSucceed(Json::Value valRes)
{
	history->m_iMailNum--;
	if(m_ptrMail->getType() == MAIL_MSG)
	{
		int iRead = valRes["isTrue"].asInt();
		m_ptrMail->setIsRead(iRead);
		m_pDelegate->updateShow(0);
	}
	else	if(m_ptrMail->getType() == MAIL_GOODS)
	{
		m_strNotice = history->onHttpGoodsAdd(valRes);
		this->schedule(schedule_selector(MailInfoLayer::playGoodsNotice), 0.8f);

		m_ptrMail->setIsRead(1);
		m_pDelegate->updateShow(0);
		//刷新标题栏金币勾玉数据
		if(history->m_layTitleBar)
		{
			history->m_layTitleBar->updateShow(0);
		}
		this->setVisible(false);
		this->setSwallow(false);
	}

	return true;
}

void MailInfoLayer::playGoodsNotice(float fDelay)
{
	if(m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->removeFromParent();
		history->updateGuide(EMIL_GUIDE);
	}
}
