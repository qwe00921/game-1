//
//  NewLoginLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/09.
//
//

#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "RedeScreen.h"
#include "NewLoginLayer.h"
#include "LoginLayer.h"
#include "RoleCreateLayer.h"
#include "LogoLayer.h"
#include "CocoMd5.h"
#include "LoginRequest.h"
#include "StatConfig.h"

enum
{
	MODE_SINGLE = 0,
	MODE_NETWORK,
	MODE_FUNNY
};

GuestLayer::GuestLayer()
{

}

GuestLayer::~GuestLayer()
{

}

void GuestLayer::onEnter()
{
	Layer::onEnter();
}

void GuestLayer::onExit()
{
	Layer::onExit();
}

bool GuestLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	return true;
}

void GuestLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void GuestLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

GuestLayer* GuestLayer::create(EventDelegate* ptrDelegate)
{
	GuestLayer* pRet = new GuestLayer();
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

bool GuestLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	initControl();
	return true;
}

void GuestLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/sign/accounts_sm.png");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	//标题
	Sprite* spTitle = ReadSpriteName("new_ui/img_text/xi_txt2.png");
	spTitle->setPosition(point + Vec2(0, 133));
	this->addChild(spTitle);

	//游客注意事项
	Label* labNotice = LabelSystemFont(LL("guestnotice"), 16);
	labNotice->setPosition(point + Vec2(0, 45));
	labNotice->setDimensions(340, 100);
	labNotice->setHorizontalAlignment(TextHAlignment::LEFT);
	labNotice->setColor(Color3B(0x66, 0, 0));
	this->addChild(labNotice);

	//登陆确认
	Label* labConfirm = LabelSystemFont(LL("guestconfirm"), 16);
	labConfirm->setPosition(point);
	this->addChild(labConfirm);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//游客登录
	Sprite* spGuestN = ReadSpriteName("new_ui/sign/sign_bnt4.png");
	Sprite* spGuestS = ReadSpriteName("new_ui/sign/sign_bnt4.png");
	spGuestS->setScale(fScale);
	MenuItemSprite* btnGuest = MenuItemSprite::create(spGuestN, spGuestS, CC_CALLBACK_1(GuestLayer::callBackGuest, this));
	spGuestS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spGuestS->setPosition(Vec2(spGuestS->getContentSize().width / 2, spGuestS->getContentSize().height / 2));
	btnGuest->setPosition(point + Vec2(0, -30));
	m_pBtnMenu->addChild(btnGuest);

	//用户登录
	Sprite* spUserN = ReadSpriteName("new_ui/sign/accounts_member.png");
	Sprite* spUserS = ReadSpriteName("new_ui/sign/accounts_member.png");
	spUserS->setScale(fScale);
	MenuItemSprite* btnUser = MenuItemSprite::create(spUserN, spUserS, CC_CALLBACK_1(GuestLayer::callBackUser, this));
	spUserS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spUserS->setPosition(Vec2(spUserS->getContentSize().width / 2, spUserS->getContentSize().height / 2));
	btnUser->setPosition(point + Vec2(10, -115));
	m_pBtnMenu->addChild(btnUser);

	//推荐标签
	Sprite* spRecommend = ReadSpriteName("new_ui/sign/sign_txt2.png");
	spRecommend->setPosition(point + Vec2(-7, -97));
	this->addChild(spRecommend);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(GuestLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(200, 150));
	m_pBtnMenu->addChild(btnClose);
}

void GuestLayer::callBackGuest(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	__String* strData = __String::createWithFormat("registerType=%d", LOGIN_GUEST);
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData->getCString());
	__String* data = __String::createWithFormat("{\"registerType\":%d,\"validateCode\":\"%s\"}", LOGIN_GUEST, validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("register",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GuestLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void GuestLayer::callBackUser(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(LoginLayer::create(m_pDelegate));
	this->removeFromParent();
}

void GuestLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void GuestLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		Json::Value result = value["result"];
		if(result["gameToken"].isString() && result["markCode"].isInt() && result["openid"].isString())
		{
			history->m_pLoginInfo->setGameToken(result["gameToken"].asString());
			history->m_pLoginInfo->setMarkCode(result["markCode"].asInt());
			history->m_pLoginInfo->addLoginElem(LOGIN_GUEST, LL("youke"), "", "", result["openid"].asString());
			history->m_pPlayerInfo->loadJson(result["playerInfo"]);
			m_pDelegate->updateState();
			Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), "", 1);
			this->removeFromParent();
		}
	}
}

LoginSwitchLayer::LoginSwitchLayer()
{

}

LoginSwitchLayer::~LoginSwitchLayer()
{

}

LoginSwitchLayer* LoginSwitchLayer::create(EventDelegate* ptrDelegate)
{
	LoginSwitchLayer* pRet = new LoginSwitchLayer();
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

bool LoginSwitchLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	initControl();
	return true;
}

void LoginSwitchLayer::onEnter()
{
	Layer::onEnter();
}

void LoginSwitchLayer::onExit()
{
	Layer::onExit();
}

bool LoginSwitchLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	return true;
}

void LoginSwitchLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void LoginSwitchLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void LoginSwitchLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	int iLoginId = history->m_pLoginInfo->getLoginId();
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/sign/accounts_sm1.png");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	//登陆方式
	Label* labType = LabelSystemFont(LL("usertype"), 18);
	labType->setColor(Color3B::YELLOW);
	labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	labType->setPosition(point + Vec2(-110, 65));
	this->addChild(labType);

	string strType;
	if (history->m_pLoginInfo->m_mLogInfo[iLoginId])
		strType = history->m_pLoginInfo->m_mLogInfo[iLoginId]->getTypeName();
	else {
		__String* keyStr = __String::createWithFormat("iLoginId%d", iLoginId);
		if (LL(keyStr->getCString(), false, true))
			strType = LL(keyStr->getCString(), false, true);
		else
			strType = keyStr->getCString();
	}
	CCLOG("strType : %s", strType.c_str());
	
	Label* labTypeVal = LabelSystemFont(strType.c_str(), 18);
	labTypeVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labTypeVal->setPosition(point + Vec2(-110 + labType->getContentSize().width + 10, 65));
	this->addChild(labTypeVal);

	//内核编号
	Label* labMark = LabelSystemFont(LL("neiheid"), 18);
	labMark->setColor(Color3B::YELLOW);
	labMark->setAnchorPoint(Vec2(0.0f, 0.5f));
	labMark->setPosition(point + Vec2(-110, 40));
	this->addChild(labMark);

	Label* labMarkVal = LabelSystemFont(IntToString(history->m_pLoginInfo->getMarkCode()), 18);
	labMarkVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labMarkVal->setPosition(point + Vec2(-110 + labMark->getContentSize().width + 10, 40));
	this->addChild(labMarkVal);

	//内核编号注意事项
	Label* labNotice = LabelSystemFont(LL("markcodenotice"), 16);
	labNotice->setAnchorPoint(Vec2(0.0f, 1.0f));
	labNotice->setPosition(point + Vec2(-110, 2));
	labNotice->setDimensions(230, 40);
	labNotice->setHorizontalAlignment(TextHAlignment::LEFT);
	labNotice->setColor(Color3B(0x66, 0, 0));
	this->addChild(labNotice);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//切换帐号
	Sprite* spSwitchN = ReadSpriteName("new_ui/main/tb_icon4.png");
	Sprite* spSwitchS = ReadSpriteName("new_ui/main/tb_icon4.png");
	spSwitchS->setScale(fScale);
	MenuItemSprite* btnSwitch = MenuItemSprite::create(spSwitchN, spSwitchS, CC_CALLBACK_1(LoginSwitchLayer::callBackSwitch, this));
	spSwitchS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSwitchS->setPosition(Vec2(spSwitchS->getContentSize().width / 2, spSwitchS->getContentSize().height / 2));
	btnSwitch->setPosition(point + Vec2(0, -95));
	m_pBtnMenu->addChild(btnSwitch);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(LoginSwitchLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(150, 135));
	m_pBtnMenu->addChild(btnClose);
}

void LoginSwitchLayer::callBackSwitch(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	Json::Value callStr;
	callStr["name"] = "startSDKLogout";
	cppBridgeCall(callStr.toStyledString().c_str());
	m_pDelegate->updateShow(0);
	this->removeFromParent();
}

void LoginSwitchLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}