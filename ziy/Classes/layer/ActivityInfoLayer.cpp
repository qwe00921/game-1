//
//  ActivityInfoLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/14.
//
//

#include "ActivityInfoLayer.h"
#include "GoodsSprite.h"
#include "ActivityInfo.h"
#include "WebViewLayer.h"
#include "PaymentTool.h"
#include "BuyJadeLayer.h"
#include "Net_HomePage.h"

#define MAX_ACT_NUM 4
#define EDIT_MAX_NUM 12
#define GIFT_ITEM_NUM 5

enum 
{
	REQ_ACT_TAKE = 0, //领取
	REQ_ACT_EXCHANGE = 1, //兑换
	REQ_EXCHCODE = 2 //兑换码
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string ActExchLayer::ms_strCode = "";

ActExchLayer::ActExchLayer()
{

}

ActExchLayer::~ActExchLayer()
{

}

ActExchLayer* ActExchLayer::create(string strContent, Rect rcDraw, EventDelegate* ptrDelegate)
{
	ActExchLayer* pRet = new ActExchLayer();
	if(pRet&&pRet->init(strContent, rcDraw, ptrDelegate))
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

bool ActExchLayer::init(string strContent, Rect rcDraw, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_strContent = strContent;
	m_rcDraw = rcDraw;
	m_pDelegate = ptrDelegate;

	initControl();
	return true;
}

void ActExchLayer::editBoxReturn(ui::EditBox* editBox)
{

}

float ActExchLayer::getOffsetY()
{
	return m_fOffsetY;
}

void ActExchLayer::initControl()
{
	float fOffsetY = 3 - m_rcDraw.size.height;

	//活动内容
	Label* labContent = LabelSystemFont(m_strContent.c_str(), 16);
	labContent->setAnchorPoint(Vec2(0.0f, 1.0f));
	labContent->setDimensions(316, 0);
	labContent->setHorizontalAlignment(TextHAlignment::LEFT);
	labContent->setPosition(Vec2(6, -fOffsetY));
	this->addChild(labContent);
	fOffsetY += labContent->getContentSize().height + 40;

	//提示
	Sprite* spNotice = ReadSpriteName("new_ui/activity/activity_txt1.png");
	spNotice->setPosition(Vec2(m_rcDraw.size.width / 2, -fOffsetY));
	this->addChild(spNotice);
	fOffsetY += 40;

	//输入框背景
	auto spInput = ReadSpriteName("new_ui/activity/activity_bg6.png");
	spInput->setPosition(Vec2(m_rcDraw.size.width / 2, -fOffsetY));
	this->addChild(spInput);

	//兑换码输入框
	auto spExchBG = ui::Scale9Sprite::create(getRespath("new_ui/hyaline/92x30.png", 0, 1));
	m_pEditBox = ui::EditBox::create(spExchBG->getContentSize(), spExchBG);
	m_pEditBox->setPosition(Vec2(m_rcDraw.size.width / 2 + 8, -fOffsetY));
	m_pEditBox->setFontSize(16);
	m_pEditBox->setFontColor(Color3B::YELLOW);
	m_pEditBox->setMaxLength(EDIT_MAX_NUM);
	m_pEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_pEditBox->setDelegate(this);
	this->addChild(m_pEditBox);
	fOffsetY += 55;

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//确定
	Sprite* spConfirmN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spConTextN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	spConfirmN->addChild(spConTextN);
	Sprite* spConfirmS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spConTextS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	spConfirmS->addChild(spConTextS);
	MenuItemSprite* btnConfirm = MenuItemSprite::create(spConfirmN, spConfirmS, CC_CALLBACK_1(ActExchLayer::callBackConfirm, this));
	btnConfirm->setPosition(Vec2(m_rcDraw.size.width / 2, -fOffsetY));
	spConTextN->setPosition(Vec2(spConfirmN->getContentSize().width / 2, spConfirmN->getContentSize().height / 2));
	spConTextS->setPosition(Vec2(spConfirmS->getContentSize().width / 2, spConfirmS->getContentSize().height / 2));
	ptrMenu->addChild(btnConfirm);
	fOffsetY += 30;

	m_fOffsetY = fOffsetY;
}

void ActExchLayer::callBackConfirm(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	ms_strCode = m_pEditBox->getText();
	if(ms_strCode == "")
	{
		ITools::getInstance()->createGlobalPrompt(LL("exchcodeisnull"), false);
		return;
	}

	m_pDelegate->callBackConfirm();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActGiftLayer* ActGiftLayer::create(int iActId, EventDelegate* ptrDelegate)
{
	ActGiftLayer* pRet = new ActGiftLayer();
	if(pRet&&pRet->init(iActId, ptrDelegate))
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

bool ActGiftLayer::init(int iActId, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_bIsPress = false;
	m_fTouchY = 0;
	m_iActId = iActId;
	m_pDelegate = ptrDelegate;
	m_iNoticeIndex = 0;

	initControl();
	updateDisplay();
	return true;
}

ActGiftLayer::ActGiftLayer()
{
	setSwallow(false);
}

ActGiftLayer::~ActGiftLayer()
{

}

void ActGiftLayer::setVisible(bool visible)
{
	Layer::setVisible(visible);
	m_ptrLayer->setVisible(visible);
}

bool ActGiftLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsDrag && isVisible() && m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_fTouchY = touch->getLocation().y;
		m_bIsPress = true;
	}
	return true;
}

void ActGiftLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsPress)
	{
		if(m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			Vec2 pos = m_ptrLayer->getPosition();
			pos.y += touch->getLocation().y - m_fTouchY;
			m_fTouchY = touch->getLocation().y;
			if(pos.y > 0 && pos.y < m_iOffsetY)
			{
				m_ptrLayer->setPosition(pos);
			}
		}
		else
		{
			m_fTouchY = 0;
			m_bIsPress = false;
		}
	}
}

void ActGiftLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsPress)
	{
		m_fTouchY = 0;
		m_bIsPress = false;
	}
}

void ActGiftLayer::updateState()
{
	m_pDelegate->updateState();
}

void ActGiftLayer::RunFunction(int iNomal)
{
	ITools::getInstance()->removeWaitLayer();
	if(iNomal == 1)//支付成功
	{
		ITools::getInstance()->createGlobalPrompt(LL("zhifuchenggong"), true);

		//重置购买状态
		ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
		ptrActivity->m_mGiftGoods.begin()->second->setState(ACT_RECEIVED);
		m_btnGift->setEnabled(false);

		//刷新活动列表充值状态
		BTActivity::getInstance()->updateActByType(ACT_RECHARGE, this);

		//刷新标题栏金币勾玉数据
		if(history->m_layTitleBar)
		{
			history->m_layTitleBar->updateShow(0);
		}
	}
	else if(iNomal == -1)//支付未完成
	{
		ITools::getInstance()->createGlobalPrompt(LL("zhifuweiwancheng"), false);
	}
}

void ActGiftLayer::updateParentShow()
{
	ITools::getInstance()->removeWaitLayer();
}

void ActGiftLayer::updateRTDate(map<int, std::string> mString)
{
	m_iNoticeIndex = 0;
	m_strNotice = mString;
	this->schedule(schedule_selector(ActGiftLayer::playGoodsNotice), 0.8f);
}

void ActGiftLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//绘制裁剪区域
	DrawNode* dnShap = DrawNode::create();
	Vec2 rect[4] = {Vec2(0, 220), Vec2(330, 220), Vec2(330, 0), Vec2(0, 0)};
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	ClippingNode* clipNode = ClippingNode::create();
	clipNode->setStencil(dnShap);
	clipNode->setPosition(Vec2(0, 50));
	m_rcDraw = clipNode->getBoundingBox();
	m_rcDraw.size.width = 330;
	m_rcDraw.size.height = 220;
	this->addChild(clipNode);

	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];

	Layer* ptrLayer = Layer::create();
	ptrLayer->setPosition(Vec2(0, 0));
	clipNode->addChild(ptrLayer);

	float fOffsetY = 3 - m_rcDraw.size.height;

	//活动内容
	Label* labContent = LabelSystemFont(ptrActivity->getContent().c_str(), 16);
	labContent->setAnchorPoint(Vec2(0.0f, 1.0f));
	labContent->setDimensions(316, 0);
	labContent->setHorizontalAlignment(TextHAlignment::LEFT);
	labContent->setPosition(Vec2(6, -fOffsetY));
	ptrLayer->addChild(labContent);
	fOffsetY += labContent->getContentSize().height + 15;

	for(map<int, ActGiftElem*>::iterator itGift = ptrActivity->m_mGiftGoods.begin(); itGift != ptrActivity->m_mGiftGoods.end(); itGift++)
	{
		ActGiftElem* ptrGift = itGift->second;

		//达成条件
		Label* labMark1 = LabelSystemFont(LL("rmbgiftmark1"), 16);
		labMark1->setAnchorPoint(Vec2(0.0f, 1.0f));
		labMark1->setPosition(Vec2(6, -fOffsetY));
		labMark1->setColor(Color3B::YELLOW);
		ptrLayer->addChild(labMark1);

		Label* labMark2 = LabelSystemFont(StringUtils::format(LL("rmbgiftmark2"), history->m_pPlayerInfo->getLoginNum()), 16);
		labMark2->setAnchorPoint(Vec2(0.0f, 1.0f));
		labMark2->setPosition(Vec2(6 + labMark1->getContentSize().width, -fOffsetY));
		ptrLayer->addChild(labMark2);

		fOffsetY += labMark1->getContentSize().height + 10;

		//礼包物品
		map<int, int> mGoodsMap = ptrGift->m_mGoods;
		int iIndex = 0;
		for(map<int, int>::iterator it = mGoodsMap.begin(); it != mGoodsMap.end(); it++, iIndex++)
		{
			int iGoodsId = it->first;
			int iCount = it->second;

			if(trData->m_mGoodsItem.count(iGoodsId))
			{
				GoodsSprite* spItem = GoodsSprite::create(iGoodsId, iCount);
				spItem->setSupDrag(true);
				spItem->setPosition(Vec2(31 + 61 * (iIndex % GIFT_ITEM_NUM), -fOffsetY - 25 - 61 * (iIndex / GIFT_ITEM_NUM)));
				ptrLayer->addChild(spItem);
			}
		}

		fOffsetY += 61 * ((mGoodsMap.size() - 1) / GIFT_ITEM_NUM + 1);
	}

	m_ptrLayer = ptrLayer;
	m_iOffsetY = fOffsetY;
	m_bIsDrag = fOffsetY > 0 ? true : false;

	//购买
	Sprite* spBuyN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Label* labBuyN = LabelSystemFont(StringUtils::format(LL("rmbgiftbuy"), ptrActivity->getRMBPrice()), 18);
	labBuyN->setPosition(Vec2(spBuyN->getContentSize().width / 2, spBuyN->getContentSize().height / 2));
	spBuyN->addChild(labBuyN);
	Sprite* spBuyS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Label* labBuyS = LabelSystemFont(StringUtils::format(LL("rmbgiftbuy"), ptrActivity->getRMBPrice()), 18);
	labBuyS->setPosition(Vec2(spBuyS->getContentSize().width / 2, spBuyS->getContentSize().height / 2));
	spBuyS->addChild(labBuyS);
	Sprite* spBuyD = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Label* labBuyD = LabelSystemFont(LL("rmbgiftfinish"), 18);
	labBuyD->setPosition(Vec2(spBuyD->getContentSize().width / 2, spBuyD->getContentSize().height / 2));
	spBuyD->addChild(labBuyD);
	addSpriteGray(spBuyD);
	m_btnGift = MenuItemSprite::create(spBuyN, spBuyS, spBuyD, CC_CALLBACK_1(ActGiftLayer::callBackRmbGift, this));
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(160, 15));
	ptrMenu->addChild(m_btnGift);
	this->addChild(ptrMenu);
}

void ActGiftLayer::updateDisplay()
{
	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
	bool bFinish = ptrActivity->m_mGiftGoods.begin()->second->getState() != ACT_UNFINISH;
	m_btnGift->setEnabled(!bFinish);
}

void ActGiftLayer::playGoodsNotice(float fDelay)
{
	if(m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->unschedule(schedule_selector(ActGiftLayer::playGoodsNotice));
	}
}

void ActGiftLayer::callBackRmbGift(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
	int iPrice = ptrActivity->getRMBPrice();
	if(iPrice)
	{
		NetJadeCharge* ptrJade = trData->getJadeCharge(iPrice);
		if(ptrJade)
		{
			ITools::getInstance()->addWaitLayer(false);
			PaymentTool::getInstance()->startPay(ptrJade->mId, ptrJade->mChargeId, ptrActivity->getTitle(), this, m_iActId);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActWXGiftLayer* ActWXGiftLayer::create(int iActId, EventDelegate* ptrDelegate)
{
	ActWXGiftLayer* pRet = new ActWXGiftLayer();
	if (pRet&&pRet->init(iActId, ptrDelegate))
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

bool ActWXGiftLayer::init(int iActId, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	m_bIsPress = false;
	m_fTouchY = 0;
	m_iActId = iActId;
	m_pDelegate = ptrDelegate;
	m_iNoticeIndex = 0;

	initControl();
	return true;
}

ActWXGiftLayer::ActWXGiftLayer()
{
	setSwallow(false);
}

ActWXGiftLayer::~ActWXGiftLayer()
{

}

void ActWXGiftLayer::setVisible(bool visible)
{
	Layer::setVisible(visible);
	m_ptrLayer->setVisible(visible);
}

bool ActWXGiftLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsDrag && isVisible() && m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_fTouchY = touch->getLocation().y;
		m_bIsPress = true;
	}
	return true;
}

void ActWXGiftLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsPress)
	{
		if (m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			Vec2 pos = m_ptrLayer->getPosition();
			pos.y += touch->getLocation().y - m_fTouchY;
			m_fTouchY = touch->getLocation().y;
			if (pos.y > 0 && pos.y < m_iOffsetY)
			{
				m_ptrLayer->setPosition(pos);
			}
		}
		else
		{
			m_fTouchY = 0;
			m_bIsPress = false;
		}
	}
}

void ActWXGiftLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsPress)
	{
		m_fTouchY = 0;
		m_bIsPress = false;
	}
}

void ActWXGiftLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];

	int iHeight = 0;
	int iPosY = 0;
	if (ptrActivity->getTypeId() == ACT_NOTICE)
	{
		iHeight = 250;
		iPosY = 20;
	}
	else if (ptrActivity->getTypeId() == ACT_WXGIFT)
	{
		iHeight = 220;
		iPosY = 50;
	}

	//绘制裁剪区域
	DrawNode* dnShap = DrawNode::create();
	Vec2 rect[4] = { Vec2(0, iHeight), Vec2(330, iHeight), Vec2(330, 0), Vec2(0, 0) };
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	ClippingNode* clipNode = ClippingNode::create();
	clipNode->setStencil(dnShap);
	clipNode->setPosition(Vec2(0, iPosY));
	m_rcDraw = clipNode->getBoundingBox();
	m_rcDraw.size.width = 330;
	m_rcDraw.size.height = iHeight;
	this->addChild(clipNode);

	Layer* ptrLayer = Layer::create();
	ptrLayer->setPosition(Vec2(0, 0));
	clipNode->addChild(ptrLayer);

	float fOffsetY = 3 - m_rcDraw.size.height;

	//活动内容
	Label* labContent = LabelSystemFont(ptrActivity->getContent().c_str(), 16);
	labContent->setAnchorPoint(Vec2(0.0f, 1.0f));
	labContent->setDimensions(316, 0);
	labContent->setHorizontalAlignment(TextHAlignment::LEFT);
	labContent->setPosition(Vec2(6, -fOffsetY));
	ptrLayer->addChild(labContent);
	fOffsetY += labContent->getContentSize().height + 15;

	for (map<int, ActGiftElem*>::iterator itGift = ptrActivity->m_mGiftGoods.begin(); itGift != ptrActivity->m_mGiftGoods.end(); itGift++)
	{
		ActGiftElem* ptrGift = itGift->second;

		//达成条件
		Label* labMark1 = LabelSystemFont(LL("jianglineirong"), 16);
		labMark1->setAnchorPoint(Vec2(0.0f, 1.0f));
		labMark1->setPosition(Vec2(6, -fOffsetY));
		labMark1->setColor(Color3B(102,0,0));
		ptrLayer->addChild(labMark1);

		fOffsetY += labMark1->getContentSize().height + 10;

		//礼包物品
		map<int, int> mGoodsMap = ptrGift->m_mGoods;
		int iIndex = 0;
		for (map<int, int>::iterator it = mGoodsMap.begin(); it != mGoodsMap.end(); it++, iIndex++)
		{
			int iGoodsId = it->first;
			int iCount = it->second;

			if (trData->m_mGoodsItem.count(iGoodsId))
			{
				GoodsSprite* spItem = GoodsSprite::create(iGoodsId, iCount);
				spItem->setSupDrag(true);
				spItem->setPosition(Vec2(31 + 61 * (iIndex % GIFT_ITEM_NUM), -fOffsetY - 25 - 61 * (iIndex / GIFT_ITEM_NUM)));
				ptrLayer->addChild(spItem);
			}
		}

		fOffsetY += 61 * ((mGoodsMap.size() - 1) / GIFT_ITEM_NUM + 1);
	}

	m_ptrLayer = ptrLayer;
	m_iOffsetY = fOffsetY;
	m_bIsDrag = fOffsetY > 0 ? true : false;

	if (ptrActivity->getTypeId() == ACT_WXGIFT)
	{
		// 	Sprite* spBuyN = ReadSpriteName("new_ui/img_global/global_ban1.png");
		// 	Label* labBuyN = LabelSystemFont(StringUtils::format(LL("rmbgiftbuy"), ptrActivity->getRMBPrice()), 18);
		// 	labBuyN->setPosition(Vec2(spBuyN->getContentSize().width / 2, spBuyN->getContentSize().height / 2));
		// 	spBuyN->addChild(labBuyN);
		// 	Sprite* spBuyS = ReadSpriteName("new_ui/img_global/global_ban2.png");
		// 	Label* labBuyS = LabelSystemFont(StringUtils::format(LL("rmbgiftbuy"), ptrActivity->getRMBPrice()), 18);
		// 	labBuyS->setPosition(Vec2(spBuyS->getContentSize().width / 2, spBuyS->getContentSize().height / 2));
		// 	spBuyS->addChild(labBuyS);
		// 	Sprite* spBuyD = ReadSpriteName("new_ui/img_global/global_ban1.png");
		// 	Label* labBuyD = LabelSystemFont(LL("rmbgiftfinish"), 18);
		// 	labBuyD->setPosition(Vec2(spBuyD->getContentSize().width / 2, spBuyD->getContentSize().height / 2));
		// 	spBuyD->addChild(labBuyD);
		// 	addSpriteGray(spBuyD);

		//领取
		Sprite* spTakeN = ReadSpriteName("new_ui/img_global/global_ban3.png");
		Sprite* spTakeTextN = ReadSpriteName("new_ui/img_text/imgtxt4_5.png");
		spTakeTextN->setPosition(Vec2(spTakeN->getContentSize().width / 2, spTakeN->getContentSize().height / 2));
		spTakeN->addChild(spTakeTextN);
		Sprite* spTakeS = ReadSpriteName("new_ui/img_global/global_ban4.png");
		Sprite* spTakeTextS = ReadSpriteName("new_ui/img_text/imgtxt4_5.png");
		spTakeTextS->setPosition(Vec2(spTakeS->getContentSize().width / 2, spTakeS->getContentSize().height / 2));
		spTakeS->addChild(spTakeTextS);

		m_btnGift = MenuItemSprite::create(spTakeN, spTakeS, CC_CALLBACK_1(ActWXGiftLayer::callBackRmbGift, this));
		Menu* ptrMenu = Menu::create();
		ptrMenu->setPosition(Vec2(160, 15));
		ptrMenu->addChild(m_btnGift);
		this->addChild(ptrMenu);


		//已领取
		m_spReceived = ReadSpriteName("new_ui/battle/battle_txt1.png");
		m_spReceived->setPosition(Vec2(160, 15));
		this->addChild(m_spReceived);
		m_spReceived->setVisible(false);

		updateDisplay();
	}
}

void ActWXGiftLayer::updateDisplay()
{
	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
	bool bFinish = ptrActivity->m_mGiftGoods.begin()->second->getState() != ACT_UNFINISH;
	m_btnGift->setEnabled(!bFinish);
	m_btnGift->setVisible(!bFinish);
	m_spReceived->setVisible(bFinish);
}

void ActWXGiftLayer::playGoodsNotice(float fDelay)
{
	if (m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->unschedule(schedule_selector(ActWXGiftLayer::playGoodsNotice));
	}
}

void ActWXGiftLayer::callBackRmbGift(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
	onHttpRequest(ptrActivity->getActId(),1);
}

void ActWXGiftLayer::onHttpRequest(int iAid, int iGid)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "wechatLoginGift";
	CocosMD5 md5;

	string data = StringUtils::format("aid=%d&gid=%d&gameToken=%s", iAid , iGid ,strToken.c_str());
	string validCode = md5.ToMD5(data);
	string strData = StringUtils::format("{\"aid\":%d,\"gid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}",iAid,iGid,strToken.c_str(),validCode.c_str());

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(ActWXGiftLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void ActWXGiftLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (value["result"]["goods"].isArray())
		{
			ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
			ptrActivity->m_mGiftGoods.begin()->second->setState(ACT_RECEIVED);

			m_iNoticeIndex = 0;
			m_strNotice.clear();
			m_strNotice = history->onHttpGoodsAdd(value["result"]);
			this->schedule(schedule_selector(ActWXGiftLayer::playGoodsNotice), 0.8f);

			updateDisplay();

			if (history->m_layTitleBar)
			{
				history->m_layTitleBar->updateShow(0);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActivityInfoLayer::ActivityInfoLayer()
{
	this->setSwallow(false);
}

ActivityInfoLayer::~ActivityInfoLayer()
{

}

bool ActivityInfoLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if(m_mIsDrag[m_iActId] && m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_fTouchY = touch->getLocation().y;
		m_bIsPress = true;
	}
	return true;
}

void ActivityInfoLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (m_bIsPress)
	{
		if(m_rcDraw.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			Vec2 pos = m_mLayer[m_iActId]->getPosition();
			pos.y += touch->getLocation().y - m_fTouchY;
			m_fTouchY = touch->getLocation().y;
			if(pos.y > 0 && pos.y < m_mOffsetY[m_iActId])
			{
				m_mLayer[m_iActId]->setPosition(pos);
			}
		}
		else
		{
			m_fTouchY = 0;
			m_bIsPress = false;
		}
	}
}

void ActivityInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsPress)
	{
		m_fTouchY = 0;
		m_bIsPress = false;
	}
}

void ActivityInfoLayer::callBackConfirm()
{
	onHttpRequest(REQ_EXCHCODE);
}

void ActivityInfoLayer::updateState()
{
	updateRecharge();
}

ActivityInfoLayer* ActivityInfoLayer::create(EventDelegate* ptrDelegate)
{
	ActivityInfoLayer* pRet = new ActivityInfoLayer();
	if(pRet&&pRet->init(ptrDelegate))
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

bool ActivityInfoLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_iNoticeIndex = 0;
	m_bIsPress = false;
	m_fTouchY = 0;
	m_iClickIndex = 1;

	initControl();
	updateActLayer(BTActivity::getInstance()->m_mActivity.begin()->first);

	auto _listener = EventListenerCustom::create("GAME_LAYER_CLOSE", [=](EventCustom* event) {
		std::string str("BuyJadeLayer");
		const char* buf = static_cast<const char*>(event->getUserData());
		if (str.compare(buf) == 0) {
			this->updateRecharge();
		}

	});

	getEventDispatcher()->addEventListenerWithFixedPriority(_listener, 1);
	return true;
}

void ActivityInfoLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//绘制裁剪区域
	DrawNode* dnShap = DrawNode::create();
	Vec2 rect[4] = {Vec2(0, 270), Vec2(330, 270), Vec2(330, 0), Vec2(0, 0)};
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	ClippingNode* clipNode = ClippingNode::create();
	clipNode->setStencil(dnShap);
	clipNode->setPosition(point + Vec2(-58, -138));
	m_rcDraw = clipNode->getBoundingBox();
	m_rcDraw.size.width = 330;
	m_rcDraw.size.height = 270;
	this->addChild(clipNode);

	map<int, ActivityElem*> mActivity = BTActivity::getInstance()->m_mActivity;
	for(map<int, ActivityElem*>::iterator it = mActivity.begin(); it != mActivity.end(); it++)
	{
		int iActId = it->first;
		ActivityElem* ptrActElem = it->second;

		Layer* ptrLayer = NULL;
		float fOffsetY = 0.0f;
		if (ptrActElem->getTypeId() == ACT_EXCHCODE)
		{
			ActExchLayer* layExch = ActExchLayer::create(ptrActElem->getContent(), m_rcDraw, this);
			layExch->setPosition(Vec2(0, 0));
			clipNode->addChild(layExch);

			fOffsetY = layExch->getOffsetY();
			ptrLayer = layExch;
		}
		else if (ptrActElem->getTypeId() == ACT_RMBGIFT)
		{
			ActGiftLayer* layGift = ActGiftLayer::create(ptrActElem->getActId(), this);
			layGift->setPosition(Vec2(0, 0));
			clipNode->addChild(layGift);

			fOffsetY = 0.0f;
			ptrLayer = layGift;
		}
		else if (ptrActElem->getTypeId() == ACT_NOTICE || ptrActElem->getTypeId() == ACT_WXGIFT)
		{
			ActWXGiftLayer* layWxGift = ActWXGiftLayer::create(ptrActElem->getActId(), this);
			layWxGift->setPosition(Vec2(0, 0));
			clipNode->addChild(layWxGift);

			fOffsetY = 0.0f;
			ptrLayer = layWxGift;
		}
		else
		{
			ptrLayer = Layer::create();
			ptrLayer->setPosition(Vec2(0, 0));
			clipNode->addChild(ptrLayer);

			fOffsetY = 3 - m_rcDraw.size.height;

			//活动内容
			Label* labContent = LabelSystemFont(ptrActElem->getContent().c_str(), 16);
			labContent->setAnchorPoint(Vec2(0.0f, 1.0f));
			labContent->setDimensions(316, 0);
			labContent->setHorizontalAlignment(TextHAlignment::LEFT);
			labContent->setPosition(Vec2(6, -fOffsetY));
			ptrLayer->addChild(labContent);
			fOffsetY += labContent->getContentSize().height + 15;

			if(ptrActElem->getTypeId() == ACT_EXCHENGE)
			{
				for(map<int, ActExchElem*>::iterator itExch = ptrActElem->m_mExchGoods.begin(); itExch != ptrActElem->m_mExchGoods.end(); itExch++)
				{
					ActExchElem* ptrExch = itExch->second;
					map<int, int> mUseGoods = ptrExch->m_mUseGoods;
					map<int, int> mGetGoods = ptrExch->m_mGetGoods;

					string strUseName = "";
					string strGetName = "";
					int iOffsetX = 0;

					//需要物品
					int iIndex = 0;
					for(map<int, int>::iterator it = mUseGoods.begin(); it != mUseGoods.end(); it++, iIndex++)
					{
						if(iIndex < MAX_ACT_NUM)
						{
							int iGoodsId = it->first;
							int iCount = it->second;

							GoodsSprite* spItem = GoodsSprite::create(iGoodsId);
							spItem->setSupDrag(true);
							spItem->setPosition(Vec2(31 + 56 * iIndex, -fOffsetY - 25 - 21));
							ptrLayer->addChild(spItem);

							//数目
							Label* labNum = updateLabNum(iGoodsId, iCount);
							spItem->addChild(labNum);
							m_mLabNum[iActId][itExch->first][iIndex] = labNum;

							HistoryGoods hisGoods(iGoodsId);
							strUseName += hisGoods.GetGoodsName();
							if(iIndex < (int)mUseGoods.size() - 1 && iIndex < MAX_ACT_NUM - 1)
							{
								strUseName += LL("dunhao");
							}
							iOffsetX++;
						}
					}

					//箭头
					Sprite* spArrow = ReadSpriteName("new_ui/img_global/global_sj.png");
					spArrow->setPosition(Vec2(31 + 56 * iOffsetX++, -fOffsetY - 25 - 21));
					ptrLayer->addChild(spArrow);

					//获取物品
					iIndex = 0;
					for(map<int, int>::iterator it = mGetGoods.begin(); it != mGetGoods.end(); it++, iIndex++)
					{
						if(iIndex < MAX_ACT_NUM)
						{
							int iGoodsId = it->first;
							int iCount = it->second;

							GoodsSprite* spItem = GoodsSprite::create(iGoodsId);
							spItem->setSupDrag(true);
							spItem->setPosition(Vec2(31 + 56 * iOffsetX++, -fOffsetY - 25 - 21));
							ptrLayer->addChild(spItem);

							Label* labNum = LabelSystemFont(StringUtils::format("%d", iCount), 14);
							labNum->setAnchorPoint(Vec2(1.0f, 0.0f));
							labNum->setPosition(Vec2(49, 0));
							labNum->enableOutline(Color4B::BLACK, 1);
							spItem->addChild(labNum);

							HistoryGoods hisGoods(iGoodsId);
							strGetName += hisGoods.GetGoodsName();
							if(iIndex < (int)mGetGoods.size() - 1 && iIndex < MAX_ACT_NUM - 1)
							{
								strGetName += LL("dunhao");
							}
						}
					}

					//达成条件
					__String* strText = __String::createWithFormat("actcondition%d", ptrActElem->getTypeId());
					__String* strCondition = __String::createWithFormat(LL(strText->getCString()), strUseName.c_str(), strGetName.c_str());
					Label* labDay = LabelSystemFont(strCondition->getCString(), 16);
					labDay->setAnchorPoint(Vec2(0.0f, 1.0f));
					labDay->setPosition(Vec2(6, -fOffsetY));
					labDay->setColor(Color3B(102, 0, 0));
					ptrLayer->addChild(labDay);
					fOffsetY += labDay->getContentSize().height + 5;

					//领取状态
					//未完成
					Sprite* spUnfinish = ReadSpriteName("new_ui/battle/battle_txt2.png");
					spUnfinish->setPosition(Vec2(290, -fOffsetY - 25));
					ptrLayer->addChild(spUnfinish);

					//兑换
					Sprite* spExchN = ReadSpriteName("new_ui/img_global/global_ban11.png");
					Sprite* spExchTextN = ReadSpriteName("new_ui/img_text/imgtxt4_7.png");
					spExchTextN->setPosition(Vec2(spExchN->getContentSize().width / 2, spExchN->getContentSize().height / 2));
					spExchN->addChild(spExchTextN);
					Sprite* spExchS = ReadSpriteName("new_ui/img_global/global_ban12.png");
					Sprite* spExchTextS = ReadSpriteName("new_ui/img_text/imgtxt4_8.png");
					spExchTextS->setPosition(Vec2(spExchS->getContentSize().width / 2, spExchS->getContentSize().height / 2));
					spExchS->addChild(spExchTextS);
					MenuItemSprite* btnExch = MenuItemSprite::create(spExchN, spExchS, CC_CALLBACK_1(ActivityInfoLayer::callBackExchange, this));
					btnExch->setTag(ptrExch->getElemId());
					Menu* ptrMenu = Menu::create();
					ptrMenu->setPosition(Vec2(290, -fOffsetY - 25));
					ptrMenu->addChild(btnExch);
					ptrLayer->addChild(ptrMenu);

					//已领取
					Sprite* spReceived = ReadSpriteName("new_ui/battle/battle_txt1.png");
					spReceived->setPosition(Vec2(290, -fOffsetY - 25));
					ptrLayer->addChild(spReceived);

					m_layElem[iActId][ptrExch->getElemId()][ACT_UNFINISH] = spUnfinish;
					m_layElem[iActId][ptrExch->getElemId()][ACT_FINISH] = ptrMenu;
					m_layElem[iActId][ptrExch->getElemId()][ACT_RECEIVED] = spReceived;
					updataActState(iActId, ptrExch->getElemId(), ptrExch->getState());

					fOffsetY += 60;
				}
			}
			else if (ptrActElem->getTypeId() == ACT_CREDITCARD)
			{
				fOffsetY += 8;

				//礼包标签
				Label* labGifts = LabelSystemFont(LL("creditcardgifts"), 16);
				labGifts->setAnchorPoint(Vec2(0.0f, 1.0f));
				labGifts->setPosition(Vec2(6, -fOffsetY));
				labGifts->setColor(Color3B(102, 0, 0));
				ptrLayer->addChild(labGifts);
				fOffsetY += labGifts->getContentSize().height + 5;

				//礼包物品
				map<int, int> mGoodsMap = ptrActElem->m_mCardGifts;
				int iIndex = 0;
				for(map<int, int>::iterator it = mGoodsMap.begin(); it != mGoodsMap.end(); it++, iIndex++)
				{
					if(iIndex < MAX_ACT_NUM)
					{
						int iGoodsId = it->first;
						int iCount = it->second;

						GoodsSprite* spItem = GoodsSprite::create(iGoodsId, iCount);
						spItem->setSupDrag(true);
						spItem->setPosition(Vec2(31 + 56 * iIndex, -fOffsetY - 25));
						ptrLayer->addChild(spItem);
					}
				}
				fOffsetY += 65;

				//按钮菜单
				Menu* ptrMenu = Menu::create();
				ptrMenu->setPosition(Vec2(0, 0));
				ptrLayer->addChild(ptrMenu, 3);

				//模板图片
				Sprite* spModelN = ReadSpriteName("new_ui/img_global/global_ban1.png");
				Sprite* spModTextN = ReadSpriteName("new_ui/img_text/imgtxt6_1.png");
				spModTextN->setPosition(Vec2(spModelN->getContentSize().width / 2, spModelN->getContentSize().height / 2));
				spModelN->addChild(spModTextN);
				Sprite* spModelS = ReadSpriteName("new_ui/img_global/global_ban2.png");
				Sprite* spModTextS = ReadSpriteName("new_ui/img_text/imgtxt6_2.png");
				spModTextS->setPosition(Vec2(spModelS->getContentSize().width / 2, spModelS->getContentSize().height / 2));
				spModelS->addChild(spModTextS);
				MenuItemSprite* btnModel = MenuItemSprite::create(spModelN, spModelS, CC_CALLBACK_1(ActivityInfoLayer::callBackModel, this));
				Menu* ptrModel = Menu::create();
				ptrModel->setPosition(Vec2(m_rcDraw.size.width / 2 - 80, -fOffsetY - 25));
				ptrModel->addChild(btnModel);
				ptrLayer->addChild(ptrModel);

				//申请信用卡
				Sprite* spCardN = ReadSpriteName("new_ui/img_global/global_ban1.png");
				Sprite* spCardTextN = ReadSpriteName("new_ui/img_text/imgtxt6_3.png");
				spCardTextN->setPosition(Vec2(spCardN->getContentSize().width / 2, spCardN->getContentSize().height / 2));
				spCardN->addChild(spCardTextN);
				Sprite* spCardS = ReadSpriteName("new_ui/img_global/global_ban2.png");
				Sprite* spCardTextS = ReadSpriteName("new_ui/img_text/imgtxt6_4.png");
				spCardTextS->setPosition(Vec2(spCardS->getContentSize().width / 2, spCardS->getContentSize().height / 2));
				spCardS->addChild(spCardTextS);
				MenuItemSprite* btnCard = MenuItemSprite::create(spCardN, spCardS, CC_CALLBACK_1(ActivityInfoLayer::callBackCard, this));
				Menu* ptrCard = Menu::create();
				ptrCard->setPosition(Vec2(m_rcDraw.size.width / 2 + 80, -fOffsetY - 25));
				ptrCard->addChild(btnCard);
				ptrLayer->addChild(ptrCard);

				fOffsetY += 45;
			}
			else if (ptrActElem->getTypeId() == ACT_LOGIN || ptrActElem->getTypeId() == ACT_RECHARGE || ptrActElem->getTypeId() == ACT_LEVEL)
			{
				for(map<int, ActGiftElem*>::iterator itGift = ptrActElem->m_mGiftGoods.begin(); itGift != ptrActElem->m_mGiftGoods.end(); itGift++)
				{
					ActGiftElem* ptrGift = itGift->second;

					//达成条件
					__String* strText = __String::createWithFormat("actcondition%d", ptrActElem->getTypeId());
					__String* strCondition = __String::createWithFormat(LL(strText->getCString()), ptrGift->getCondition());
					Label* labDay = LabelSystemFont(strCondition->getCString(), 16);
					labDay->setAnchorPoint(Vec2(0.0f, 1.0f));
					labDay->setPosition(Vec2(6, -fOffsetY));
					labDay->setColor(Color3B(102, 0, 0));
					ptrLayer->addChild(labDay);
					fOffsetY += labDay->getContentSize().height + 5;

					//礼包物品
					map<int, int> mGoodsMap = ptrGift->m_mGoods;
					int iIndex = 0;
					for(map<int, int>::iterator it = mGoodsMap.begin(); it != mGoodsMap.end(); it++, iIndex++)
					{
						if(iIndex < MAX_ACT_NUM)
						{
							int iGoodsId = it->first;
							int iCount = it->second;

							GoodsSprite* spItem = GoodsSprite::create(iGoodsId, iCount);
							spItem->setSupDrag(true);
							spItem->setPosition(Vec2(31 + 56 * iIndex, -fOffsetY - 25));
							ptrLayer->addChild(spItem);
						}
					}

					//领取状态
					//未完成
					Node* spUnfinish;
					if (ptrActElem->getTypeId() == ACT_RECHARGE) {
						spUnfinish = createToBuyBtn(ptrActElem);
						spUnfinish->setPosition(Vec2(290, -fOffsetY - 25));
						ptrLayer->addChild(spUnfinish);
					}
					else {
						spUnfinish = ReadSpriteName("new_ui/battle/battle_txt2.png");
						spUnfinish->setPosition(Vec2(290, -fOffsetY - 25));
						ptrLayer->addChild(spUnfinish);
					}


					//领取
					Sprite* spTakeN = ReadSpriteName("new_ui/img_global/global_ban11.png");
					Sprite* spTakeTextN = ReadSpriteName("new_ui/img_text/imgtxt4_5.png");
					spTakeTextN->setPosition(Vec2(spTakeN->getContentSize().width / 2, spTakeN->getContentSize().height / 2));
					spTakeN->addChild(spTakeTextN);
					Sprite* spTakeS = ReadSpriteName("new_ui/img_global/global_ban12.png");
					Sprite* spTakeTextS = ReadSpriteName("new_ui/img_text/imgtxt4_5.png");
					spTakeTextS->setPosition(Vec2(spTakeS->getContentSize().width / 2, spTakeS->getContentSize().height / 2));
					spTakeS->addChild(spTakeTextS);
					MenuItemSprite* btnTake = MenuItemSprite::create(spTakeN, spTakeS, CC_CALLBACK_1(ActivityInfoLayer::callBackTake, this));
					btnTake->setTag(ptrGift->getElemId());
					Menu* ptrMenu = Menu::create();
					ptrMenu->setPosition(Vec2(290, -fOffsetY - 25));
					ptrMenu->addChild(btnTake);
					ptrLayer->addChild(ptrMenu);

					//已领取
					Sprite* spReceived = ReadSpriteName("new_ui/battle/battle_txt1.png");
					spReceived->setPosition(Vec2(290, -fOffsetY - 25));
					ptrLayer->addChild(spReceived);

					m_layElem[iActId][ptrGift->getElemId()][ACT_UNFINISH] = spUnfinish;
					m_layElem[iActId][ptrGift->getElemId()][ACT_FINISH] = ptrMenu;
					m_layElem[iActId][ptrGift->getElemId()][ACT_RECEIVED] = spReceived;
					updataActState(iActId, ptrGift->getElemId(), ptrGift->getState());

					fOffsetY += 60;
				}
			}
		}

		m_mLayer[iActId] = ptrLayer;
		m_mOffsetY[iActId] = fOffsetY;
		m_mIsDrag[iActId] = fOffsetY > 0 ? true : false;
	}
}

Menu* ActivityInfoLayer::createToBuyBtn(ActivityElem* ptrActivity)
{
	//购买
	Sprite* spBuyN = ReadSpriteName("new_ui/img_global/global_ban11.png");
	Label* labBuyN = LabelSystemFont(LL("gotopay"), 18);
	labBuyN->setPosition(Vec2(spBuyN->getContentSize().width / 2, spBuyN->getContentSize().height / 2));
	spBuyN->addChild(labBuyN);
	Sprite* spBuyS = ReadSpriteName("new_ui/img_global/global_ban12.png");
	Label* labBuyS = LabelSystemFont(LL("gotopay"), 18);
	labBuyS->setPosition(Vec2(spBuyS->getContentSize().width / 2, spBuyS->getContentSize().height / 2));
	spBuyS->addChild(labBuyS);
	Sprite* spBuyD = ReadSpriteName("new_ui/img_global/global_ban11.png");
	Label* labBuyD = LabelSystemFont(LL("rmbgiftfinish"), 18);
	labBuyD->setPosition(Vec2(spBuyD->getContentSize().width / 2, spBuyD->getContentSize().height / 2));
	spBuyD->addChild(labBuyD);
	addSpriteGray(spBuyD);
	MenuItemSprite* m_btnGift = MenuItemSprite::create(spBuyN, spBuyS, spBuyD, CC_CALLBACK_1(ActivityInfoLayer::callBackToPay, this));
	Menu* ptrMenu = Menu::create();

	ptrMenu->addChild(m_btnGift);
	return ptrMenu;

}

void ActivityInfoLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
	HongDian::getInstence()->autoUpdateWuJiang(WUJIANG_JIEMIAN);
}

void ActivityInfoLayer::callBackTake(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* ptrMenu = (MenuItemSprite*)sender;
	m_iClickIndex = ptrMenu->getTag();
	onHttpRequest(REQ_ACT_TAKE);
}

void ActivityInfoLayer::callBackExchange(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* ptrMenu = (MenuItemSprite*)sender;
	m_iClickIndex = ptrMenu->getTag();

	bool bIsExch = true; //是否只能兑换一次
	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
	if(ptrActivity->getTypeId() == ACT_EXCHENGE)
	{
		map<int, int> mGetGoods = ptrActivity->m_mExchGoods[m_iClickIndex]->m_mGetGoods;
		for(map<int, int>::iterator it = mGetGoods.begin(); it != mGetGoods.end(); it++)
		{
			HistoryGoods hisGoods(it->first);
			if(!hisGoods.isGetEnable())
			{
				bIsExch = false;
				break;
			}
		}
	}

	if (bIsExch)
	{
		onHttpRequest(REQ_ACT_EXCHANGE);
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("wufaduihuan"), false);
	}
}

void ActivityInfoLayer::callBackModel(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(WebViewLayer::create(BTActivity::getInstance()->m_mActivity[m_iActId]->getCardUrl1()));
}

void ActivityInfoLayer::callBackCard(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(WebViewLayer::create(BTActivity::getInstance()->m_mActivity[m_iActId]->getCardUrl2()));
}

void ActivityInfoLayer::callBackToPay(Ref *sender) {
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (history->m_layTitleBar) {
		TitleBarLayer* titleBar = dynamic_cast<TitleBarLayer*>(history->m_layTitleBar);
		if (titleBar){
			titleBar->fireToJade();
			return;
		}
	}
	ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this,true));
}

void ActivityInfoLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case REQ_ACT_TAKE:
	{
		strURL = "activitiesItem";
		string data = StringUtils::format("aid=%d&gid=%d&gameToken=%s", m_iActId, m_iClickIndex, strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"aid\":%d,\"gid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iActId, m_iClickIndex, strToken.c_str(), validCode.c_str());
		strTag = "REQ_ACT_TAKE";
	}
		break;
	case REQ_ACT_EXCHANGE:
	{
		strURL = "activitiesItem2";
		string data = StringUtils::format("aid=%d&gid=%d&gameToken=%s", m_iActId, m_iClickIndex, strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"aid\":%d,\"gid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iActId, m_iClickIndex, strToken.c_str(), validCode.c_str());
		strTag = "REQ_ACT_EXCHANGE";
	}
		break;
	case REQ_EXCHCODE:
	{
		strURL = "duihuan";
		string data = StringUtils::format("gameToken=%s&code=%s", strToken.c_str(), ActExchLayer::ms_strCode.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"code\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), ActExchLayer::ms_strCode.c_str(), validCode.c_str());
		strTag = "REQ_EXCHCODE";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(ActivityInfoLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void ActivityInfoLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		onActivitySucceed(value["result"]);
	}
}

bool ActivityInfoLayer::onActivitySucceed(Json::Value valRes)
{
	m_iNoticeIndex = 0;
	m_strNotice.clear();
	m_strNotice = history->onHttpGoodsAdd(valRes);
	this->schedule(schedule_selector(ActivityInfoLayer::playGoodsNotice), 0.8f);

	//更新显示状态
	ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
	if (ptrActivity->getTypeId() == ACT_EXCHENGE)
	{
		//除去兑换消耗
		map<int, int> mUseGoods = ptrActivity->m_mExchGoods[m_iClickIndex]->m_mUseGoods;
		for(map<int, int>::iterator it = mUseGoods.begin(); it != mUseGoods.end(); it++)
		{
			int iGoodsId = it->first;
			int iCount = it->second;
			history->packGoodsReduce(iGoodsId, history->m_mConsuNetId[iGoodsId], iCount);
		}
	}
	else if (ptrActivity->getTypeId() == ACT_EXCHCODE)
	{
		//兑换码
	}
	else
	{
		ptrActivity->m_mGiftGoods[m_iClickIndex]->setState(ACT_RECEIVED);
		updataActState(m_iActId, m_iClickIndex, ACT_RECEIVED);
	}
	updateExchLayer(); //领取后刷新兑换页面

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}

	return true;
}

void ActivityInfoLayer::playGoodsNotice(float fDelay)
{
	if(m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->unschedule(schedule_selector(ActivityInfoLayer::playGoodsNotice));
	}
}

void ActivityInfoLayer::updateActLayer(int iActId)
{
	m_iActId = iActId;
	for(map<int, Layer*>::iterator it = m_mLayer.begin(); it != m_mLayer.end(); it++)
	{
		it->second->setVisible(it->first == m_iActId);
	}
}

void ActivityInfoLayer::updataActState(int iActId, int iElemId, int iState)
{
	map<int, Node*> mLayNode = m_layElem[iActId][iElemId];
	for(map<int, Node*>::iterator it = mLayNode.begin(); it != mLayNode.end(); it++)
	{
		it->second->setVisible(it->first == iState);
	}
}

void ActivityInfoLayer::updateExchLayer()
{
	BTActivity::getInstance()->updateActByType(ACT_EXCHENGE);
	map<int, ActivityElem*> mActivity = BTActivity::getInstance()->m_mActivity;
	for(map<int, ActivityElem*>::iterator it = mActivity.begin(); it != mActivity.end(); it++)
	{
		ActivityElem* ptrExch = it->second;
		if(ptrExch->getTypeId() == ACT_EXCHENGE)
		{
			for(map<int, ActExchElem*>::iterator itExch = ptrExch->m_mExchGoods.begin(); itExch != ptrExch->m_mExchGoods.end(); itExch++)
			{
				ActExchElem* ptrChange = itExch->second;
				map<int, int> mUseGoods = ptrChange->m_mUseGoods;
				int iIndex = 0;
				for(map<int, int>::iterator itGoods = mUseGoods.begin(); itGoods != mUseGoods.end(); itGoods++, iIndex++)
				{
					int iGoodsId = itGoods->first;
					int iCount = itGoods->second;
					Label* labNum = m_mLabNum[ptrExch->getActId()][ptrChange->getElemId()][iIndex];
					Label* labNew = updateLabNum(iGoodsId, iCount);
					m_mLabNum[ptrExch->getActId()][ptrChange->getElemId()][iIndex] = labNew;
					labNum->getParent()->addChild(labNew);
					labNum->removeFromParent();
				}
				updataActState(ptrExch->getActId(), ptrChange->getElemId(), ptrChange->getState());
			}
		}
	}
}

void ActivityInfoLayer::updateRecharge()
{
	map<int, ActivityElem*> mActivity = BTActivity::getInstance()->m_mActivity;
	for(map<int, ActivityElem*>::iterator it = mActivity.begin(); it != mActivity.end(); it++)
	{
		ActivityElem* ptrElem = it->second;
		if(ptrElem->getTypeId() == ACT_RECHARGE)
		{
			for(map<int, ActGiftElem*>::iterator itGift = ptrElem->m_mGiftGoods.begin(); itGift != ptrElem->m_mGiftGoods.end(); itGift++)
			{
				ActGiftElem* ptrGift = itGift->second;
				updataActState(ptrElem->getActId(), ptrGift->getElemId(), ptrGift->getState());
			}
		}
	}
}

Label* ActivityInfoLayer::updateLabNum(int iGoodsId, int iCount)
{
	HistoryGoods hisGoods(iGoodsId);
	int iStore = hisGoods.getStoreNum();
	Label* labNum = LabelSystemFont(StringUtils::format("%d", iStore), 14);
	labNum->setAnchorPoint(Vec2(0.0f, 0.0f));
	labNum->setColor(iStore >= iCount ? Color3B::GREEN : Color3B::RED);
	labNum->setPosition(Vec2(1, 0));
	labNum->enableOutline(Color4B::BLACK, 1);

	Label* labUsed = LabelSystemFont(StringUtils::format("/%d", iCount), 14);
	labUsed->setAnchorPoint(Vec2(0.0f, 0.0f));
	labUsed->setPosition(Vec2(1 + labNum->getContentSize().width, 0));
	labUsed->enableOutline(Color4B::BLACK, 1);
	labNum->addChild(labUsed);

	return labNum;
}