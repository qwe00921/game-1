//
//  GoldExchangeLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/05/30.
//
//

#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "GoldExchangeLayer.h"

enum
{
	EXCHANGE_INIT = 0,
	EXCHANGE_BUY
};

GoldExchangeLayer::GoldExchangeLayer()
{

}

GoldExchangeLayer::~GoldExchangeLayer()
{

}

void GoldExchangeLayer::onEnter()
{
	Layer::onEnter();
}

void GoldExchangeLayer::onExit()
{
	Layer::onExit();
}

bool GoldExchangeLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void GoldExchangeLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void GoldExchangeLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

GoldExchangeLayer* GoldExchangeLayer::create(EventDelegate* ptrDelegate)
{
	GoldExchangeLayer* pRet = new GoldExchangeLayer();
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

bool GoldExchangeLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;

	m_iMaxTimes = trData->m_mGoldExchange.size();
	m_iCurTimes = 0;
	initControl();
	ExchangHttpRequest(EXCHANGE_INIT);
	return true;
}

void GoldExchangeLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spInfoBG = ReadSpriteName("new_ui/sign/accounts_sm2.png");
	spInfoBG->setPosition(point);
	this->addChild(spInfoBG);

	//充值图标
	Sprite* spIcon = ReadSpriteName("new_ui/sign/accounts_gold.png");
	spIcon->setPosition(point + Vec2(-96, 63));
	this->addChild(spIcon);

	//兑换金币
	Label* labExchange = LabelSystemFont(LL("duihuanjinbi"), 20);
	labExchange->setAnchorPoint(Vec2(0.0f, 0.5f));
	labExchange->setPosition(point + Vec2(-58, 80));
	labExchange->setColor(Color3B::YELLOW);
	this->addChild(labExchange);

	//今日可用
	Label* labTimes = LabelSystemFont(LL("jinrikeyong"), 16);
	labTimes->setAnchorPoint(Vec2(0.0f, 0.5f));
	labTimes->setPosition(point + Vec2(-58, 48));
	labTimes->setColor(Color3B(102, 0, 0));
	this->addChild(labTimes);

	//剩余次数
	m_labTimes = LabelSystemFont("0/0", 16);
	m_labTimes->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labTimes->setPosition(point + Vec2(15, 48));
	m_labTimes->setColor(Color3B::YELLOW);
	this->addChild(m_labTimes);

	//兑换提示
	Label* labNotice = LabelSystemFont(LL("exchangenotice"), 18);
	labNotice->setAnchorPoint(Vec2(0.0f, 0.5f));
	labNotice->setPosition(point + Vec2(-126, 12));
	this->addChild(labNotice);

	//勾玉ICON
	Sprite* spJadeIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJadeIcon->setPosition(point + Vec2(-95, -21+3));
	this->addChild(spJadeIcon);

	//勾玉
	m_labJade = LabelSystemFont("0", 18);
	m_labJade->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labJade->setPosition(point + Vec2(-80, -21 + 3));
	m_labJade->setColor(Color3B::GREEN);
	this->addChild(m_labJade);

	//箭头
	Sprite* spArrow = ReadSpriteName("new_ui/img_global/global_sj.png");
	spArrow->setPosition(point + Vec2(-15, -21 + 3));
	this->addChild(spArrow);

	//金币ICON
	Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGoldIcon->setPosition(point + Vec2(35, -21 + 3));
	this->addChild(spGoldIcon);

	//金币
	m_labGold = LabelSystemFont("0", 18);
	m_labGold->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labGold->setPosition(point + Vec2(50, -21));
	m_labGold->setColor(Color3B::YELLOW);
	this->addChild(m_labGold);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//确认
	Sprite* spConN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spConTextN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	spConTextN->setPosition(Vec2(spConN->getContentSize().width / 2, spConN->getContentSize().height / 2));
	spConN->addChild(spConTextN);
	Sprite* spConS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spConTextS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	spConTextS->setPosition(Vec2(spConS->getContentSize().width / 2, spConS->getContentSize().height / 2));
	spConS->addChild(spConTextS);
	MenuItemSprite* btnConfirm = MenuItemSprite::create(spConN, spConS, CC_CALLBACK_1(GoldExchangeLayer::callBackConfirm, this));
	btnConfirm->setPosition(point + Vec2(75, -100));
	m_pBtnMenu->addChild(btnConfirm);

	//取消
	Sprite* spQuitN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spQuitTextN = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	spQuitTextN->setPosition(Vec2(spQuitN->getContentSize().width / 2, spQuitN->getContentSize().height / 2));
	spQuitN->addChild(spQuitTextN);
	Sprite* spQuitS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spQuitTextS = ReadSpriteName("new_ui/img_text/imgtxt4_4.png");
	spQuitTextS->setPosition(Vec2(spQuitS->getContentSize().width / 2, spQuitS->getContentSize().height / 2));
	spQuitS->addChild(spQuitTextS);
	MenuItemSprite* btnCancel = MenuItemSprite::create(spQuitN, spQuitS, CC_CALLBACK_1(GoldExchangeLayer::callBackCancel, this));
	btnCancel->setPosition(point + Vec2(-75, -100));
	m_pBtnMenu->addChild(btnCancel);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(GoldExchangeLayer::callBackCancel, this));
	btnClose->setPosition(point + Vec2(200, 125+17));
	m_pBtnMenu->addChild(btnClose);
}

void GoldExchangeLayer::callBackConfirm(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_iCurTimes < m_iMaxTimes)
	{
		if(history->m_iJade < trData->m_mGoldExchange[m_iCurTimes + 1]->iJade)
		{
			ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
			return;
		}
		ExchangHttpRequest(EXCHANGE_BUY);
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("goldbuytimesout"), false);
	}
}

void GoldExchangeLayer::callBackCancel(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void GoldExchangeLayer::updateExchange()
{
	__String* strTimes = __String::createWithFormat("%d/%d", m_iMaxTimes - m_iCurTimes, m_iMaxTimes);
	m_labTimes->setString(strTimes->getCString());
	int iCurIndex = m_iCurTimes < m_iMaxTimes ? m_iCurTimes + 1 : m_iMaxTimes;
	m_labJade->setString(IntToString(trData->m_mGoldExchange[iCurIndex]->iJade));
	m_labGold->setString(IntToString(trData->m_mGoldExchange[iCurIndex]->iGold));
	if (m_pDelegate)
		m_pDelegate->updateMoneyShow();
}

void GoldExchangeLayer::ExchangHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	__String* strData = NULL;
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case EXCHANGE_INIT:
	{
		strURL = "getBuyGldNum";
		__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data->getCString());
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "EXCHANGE_INIT";
	}
		break;
	case EXCHANGE_BUY:
	{
		strURL = "buyGold";
		__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data->getCString());
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "EXCHANGE_BUY";
	}
		break;
	default:
		break;
	}

	CCLOG("%s", strData->getCString());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(GoldExchangeLayer::onHttpRequestCompleted), strData->getCString(), ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void GoldExchangeLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		Json::Value valRes = value["result"];
		if(strTag == "EXCHANGE_INIT")
		{
			if(valRes["num"].isInt())
			{
				m_iCurTimes = valRes["num"].asInt();
				updateExchange();
			}
		}
		else if(strTag == "EXCHANGE_BUY")
		{
			onBuySucceed(value["result"]);
		}
	}
}

void GoldExchangeLayer::onBuySucceed(Json::Value valRes)
{
	int iAddGold = 0;
	if(valRes["gold"].isInt())
	{
		iAddGold = valRes["gold"].asInt() - history->money;
		history->money = valRes["gold"].asInt();
	}
	if(valRes["jade"].isInt())
	{
		history->m_iJade = valRes["jade"].asInt();
	}
	if(valRes["num"].isInt())
	{
		m_iCurTimes = valRes["num"].asInt();
	}
	updateExchange();

	__String* strNotice = __String::createWithFormat(LL("huodessdd"), LL("jinbi"), iAddGold);
	ITools::getInstance()->createGlobalPrompt(strNotice->getCString(), true);

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}

	m_pDelegate->updateShow(9999999);
}