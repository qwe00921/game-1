//
//  LoginLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/4/18.
//
//

#include "LoginLayer.h"
#include "Common.h"
#include "main.h"
#include "PasswordLib.h"
#include "otherfunction.h"
#include "StatConfig.h"
#include "CocoMd5.h"
#include "Defines.h"

#define TEXT_MIN_NUM 6
#define TEXT_MAX_NUM 16

enum 
{
	PAGE_LOGIN = 0,
	PAGE_REGISTER = 1,
	PAGE_BIND = 2
};

LoginLayer::LoginLayer()
{

}

LoginLayer::~LoginLayer()
{

}

LoginLayer* LoginLayer::create(EventDelegate* ptrDelegate, bool bIsBind /*= false*/)
{
	LoginLayer* pRet = new LoginLayer();
	if(pRet&&pRet->init(ptrDelegate, bIsBind))
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

bool LoginLayer::init(EventDelegate* ptrDelegate, bool bIsBind)
{
    if ( !Layer::init() )
    {
        return false;
    }

	m_pDelegate = ptrDelegate;
	m_iPage = PAGE_LOGIN;
	m_iCurLogin = LOGIN_NONE;
	initControl();

	if(bIsBind)
	{
		updateBind();
	}
	else
	{
		LoginElem* ptrLogin = history->m_pLoginInfo->getLoginElem(LOGIN_USER);
		if(ptrLogin)
		{
			m_mEditBox[0]->setText(ptrLogin->getUserName().c_str());
			m_mEditBox[1]->setText(ptrLogin->getPassword().c_str());
		}
		updateBtnClean();
	}

    return true;
}

void LoginLayer::initControl()
{
	this->setTag(LOGINLAYER);
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/sign/sign_bg2.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//帐号输入框
	auto spUserBG = ui::Scale9Sprite::create(getRespath("new_ui/sign/sign_box1.png", 0, 1));
	m_mEditBox[0] = ui::EditBox::create(spUserBG->getContentSize(), spUserBG);
	m_mEditBox[0]->setPosition(point + Vec2(12, 89));
	m_mEditBox[0]->setFontSize(18);
	m_mEditBox[0]->setPlaceholderFontSize(18);
	m_mEditBox[0]->setPlaceHolder(LL("inputuser"));
	m_mEditBox[0]->setPlaceholderFontColor(Color3B(97, 121, 146));
	m_mEditBox[0]->setFontColor(Color3B::WHITE);
	m_mEditBox[0]->setMaxLength(TEXT_MAX_NUM);
	m_mEditBox[0]->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_mEditBox[0]->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_mEditBox[0]->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_mEditBox[0]->setDelegate(this);
	this->addChild(m_mEditBox[0]);

	//密码输入框
	auto spPassBG = ui::Scale9Sprite::create(getRespath("new_ui/sign/sign_box1.png", 0, 1));
	m_mEditBox[1] = ui::EditBox::create(spPassBG->getContentSize(), spPassBG);
	m_mEditBox[1]->setPosition(point + Vec2(12, 33));
	m_mEditBox[1]->setFontSize(18);
	m_mEditBox[1]->setPlaceholderFontSize(18);
	m_mEditBox[1]->setPlaceHolder(LL("inputpassword"));
	m_mEditBox[1]->setPlaceholderFontColor(Color3B(97, 121, 146));
	m_mEditBox[1]->setFontColor(Color3B::WHITE);
	m_mEditBox[1]->setMaxLength(TEXT_MAX_NUM);
	m_mEditBox[1]->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_mEditBox[1]->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_mEditBox[1]->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_mEditBox[1]->setDelegate(this);
	this->addChild(m_mEditBox[1]);

	//其他方式
	m_spOther = ReadSpriteName("new_ui/sign/sign_txt1.png");
	m_spOther->setPosition(point + Vec2(0, -95));
	this->addChild(m_spOther);

	//提示
	m_spNotice = ReadSpriteName("new_ui/sign/sign_txt3.png");
	m_spNotice->setPosition(point + Vec2(0, -95));
	m_spNotice->setVisible(false);
	this->addChild(m_spNotice);

	//绑定提示
	m_spBind = ReadSpriteName("new_ui/sign/sign_txt4.png");
	m_spBind->setPosition(point + Vec2(0, -95));
	m_spBind->setVisible(false);
	this->addChild(m_spBind);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//用户清除
	Sprite* spUserN = ReadSpriteName("new_ui/sign/sign_close1.png");
	Sprite* spUserS = ReadSpriteName("new_ui/sign/sign_close1.png");
	spUserS->setScale(fScale);
	m_mClean[0] = MenuItemSprite::create(spUserN, spUserS, CC_CALLBACK_1(LoginLayer::callBackUserClean, this));
	spUserS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spUserS->setPosition(Vec2(spUserS->getContentSize().width / 2, spUserS->getContentSize().height / 2));
	m_mClean[0]->setPosition(point + Vec2(96, 89));
	ptrMenu->addChild(m_mClean[0]);

	//密码清除 
	Sprite* spPassN = ReadSpriteName("new_ui/sign/sign_close1.png");
	Sprite* spPassS = ReadSpriteName("new_ui/sign/sign_close1.png");
	spPassS->setScale(fScale);
	m_mClean[1] = MenuItemSprite::create(spPassN, spPassS, CC_CALLBACK_1(LoginLayer::callBackPassClean, this));
	spPassS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spPassS->setPosition(Vec2(spPassS->getContentSize().width / 2, spPassS->getContentSize().height / 2));
	m_mClean[1]->setPosition(point + Vec2(96, 33));
	ptrMenu->addChild(m_mClean[1]);

	//注册
	Sprite* spRegisterN = ReadSpriteName("new_ui/sign/sign_bnt1.png");
	Sprite* spRegisterS = ReadSpriteName("new_ui/sign/sign_bnt1.png");
	spRegisterS->setScale(fScale);
	m_btnRegister = MenuItemSprite::create(spRegisterN, spRegisterS, CC_CALLBACK_1(LoginLayer::callBackRegister, this));
	spRegisterS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRegisterS->setPosition(Vec2(spRegisterS->getContentSize().width / 2, spRegisterS->getContentSize().height / 2));
	m_btnRegister->setPosition(point + Vec2(-105, -29));
	ptrMenu->addChild(m_btnRegister);

	//登陆
	Sprite* spLoginN = ReadSpriteName("new_ui/sign/sign_bnt2.png");
	Sprite* spLoginS = ReadSpriteName("new_ui/sign/sign_bnt2.png");
	spLoginS->setScale(fScale);
	m_btnLogin = MenuItemSprite::create(spLoginN, spLoginS, CC_CALLBACK_1(LoginLayer::callBackRequest, this));
	spLoginS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLoginS->setPosition(Vec2(spLoginS->getContentSize().width / 2, spLoginS->getContentSize().height / 2));
	m_btnLogin->setPosition(point + Vec2(45, -29));
	m_btnLogin->setTag(PAGE_LOGIN);
	ptrMenu->addChild(m_btnLogin);

	//注册帐号
	Sprite* spRegIdN = ReadSpriteName("new_ui/sign/sign_bnt3.png");
	Sprite* spRegIdS = ReadSpriteName("new_ui/sign/sign_bnt3.png");
	spRegIdS->setScale(fScale);
	m_btnRegId = MenuItemSprite::create(spRegIdN, spRegIdS, CC_CALLBACK_1(LoginLayer::callBackRequest, this));
	spRegIdS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRegIdS->setPosition(Vec2(spRegIdS->getContentSize().width / 2, spRegIdS->getContentSize().height / 2));
	m_btnRegId->setPosition(point + Vec2(5, -29));
	m_btnRegId->setVisible(false);
	m_btnRegId->setTag(PAGE_REGISTER);
	ptrMenu->addChild(m_btnRegId);

	//绑定帐号
	Sprite* spBindN = ReadSpriteName("new_ui/sign/sign_bnt5.png");
	Sprite* spBindS = ReadSpriteName("new_ui/sign/sign_bnt5.png");
	spBindS->setScale(fScale);
	m_btnBind = MenuItemSprite::create(spBindN, spBindS, CC_CALLBACK_1(LoginLayer::callBackRequest, this));
	spBindS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spBindS->setPosition(Vec2(spBindS->getContentSize().width / 2, spBindS->getContentSize().height / 2));
	m_btnBind->setPosition(point + Vec2(5, -29));
	m_btnBind->setVisible(false);
	m_btnBind->setTag(PAGE_BIND);
	ptrMenu->addChild(m_btnBind);

	//QQ登陆
	Sprite* spQQN = ReadSpriteName("new_ui/sign/icon_qq.png");
	Sprite* spQQS = ReadSpriteName("new_ui/sign/icon_qq.png");
	spQQS->setScale(fScale);
	m_btnQQ = MenuItemSprite::create(spQQN, spQQS, CC_CALLBACK_1(LoginLayer::callBackQQLogin, this));
	spQQS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spQQS->setPosition(Vec2(spQQS->getContentSize().width / 2, spQQS->getContentSize().height / 2));
	m_btnQQ->setPosition(point + Vec2(-15, -92));
	m_btnQQ->setVisible(false);
	ptrMenu->addChild(m_btnQQ);

	//微信登陆
	Sprite* spWXN = ReadSpriteName("new_ui/sign/icon_wx.png");
	Sprite* spWXS = ReadSpriteName("new_ui/sign/icon_wx.png");
	spWXS->setScale(fScale);
	m_btnWX = MenuItemSprite::create(spWXN, spWXS, CC_CALLBACK_1(LoginLayer::callBackWXLogin, this));
	spWXS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spWXS->setPosition(Vec2(spWXS->getContentSize().width / 2, spWXS->getContentSize().height / 2));
	m_btnWX->setPosition(point + Vec2(65, -92));
	m_btnWX->setVisible(false);
	ptrMenu->addChild(m_btnWX);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(LoginLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(165, 116));
	ptrMenu->addChild(btnClose);
}

void LoginLayer::editBoxReturn(ui::EditBox* editBox)
{
	updateBtnClean();
}

bool LoginLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}

void LoginLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void LoginLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void LoginLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	removeWaitLayer(0);

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
		Json::Value result = value["result"];
		if(strTag == "POST_REGISTER")
		{
			if(result["gameToken"].isString() && result["markCode"].isInt())
			{
				history->m_pLoginInfo->setGameToken(result["gameToken"].asString());
				history->m_pLoginInfo->setMarkCode(result["markCode"].asInt());
				history->m_pLoginInfo->addLoginElem(LOGIN_USER, LL("tongrenyonghu"), m_mEditBox[0]->getText(), m_mEditBox[1]->getText(), "");
				history->m_pPlayerInfo->loadJson(result["playerInfo"]);
				ITools::getInstance()->createGlobalPrompt(LL("registersucceed"), true);
				Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), m_mEditBox[0]->getText(), 2);
			}
		}
		else if(strTag == "POST_WXQQ")
		{
			if(result["gameToken"].isString() && result["markCode"].isInt() && result["openid"].isString())
			{
				string strType = m_iCurLogin == LOGIN_WX ? LL("wxyonghu") : LL("qqyonghu");
				history->m_pLoginInfo->setGameToken(result["gameToken"].asString());
				history->m_pLoginInfo->setMarkCode(result["markCode"].asInt());
				history->m_pLoginInfo->addLoginElem(m_iCurLogin, strType, "", "", result["openid"].asString());
				history->m_pPlayerInfo->loadJson(result["playerInfo"]);
				Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), "", 3);
			}
		}
		else if(strTag == "POST_USER_LOGIN")
		{
			if(result["gameToken"].isString() && result["markCode"].isInt())
			{
				history->m_pLoginInfo->setGameToken(result["gameToken"].asString());
				history->m_pLoginInfo->setMarkCode(result["markCode"].asInt());
				history->m_pLoginInfo->addLoginElem(LOGIN_USER, LL("tongrenyonghu"), m_mEditBox[0]->getText(), m_mEditBox[1]->getText(), "");
				history->m_pPlayerInfo->loadJson(result["playerInfo"]);
				Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), m_mEditBox[0]->getText(), 2);
			}
		}
		else if(strTag == "POST_BINDING")
		{
			if(result["isTrue"].isInt() && result["isTrue"].asInt())
			{
				history->m_pLoginInfo->removeLoginElem(LOGIN_GUEST);
				history->m_pLoginInfo->addLoginElem(LOGIN_USER, LL("tongrenyonghu"), m_mEditBox[0]->getText(), m_mEditBox[1]->getText(), "");
			}
		}
		m_pDelegate->updateState();
		this->removeFromParent();
	}
}

void LoginLayer::onHttpReqWXQQLogin(string loginno, string accounttype)
{
	LoginLayer* ptrLogin = (LoginLayer*)ChessApplication::getInstance()->getFounctionLayerByTag(LOGINLAYER);
	if (ptrLogin == NULL)
	{
		return;
	}

	int iType = LOGIN_WX;
	if(accounttype == "QQ")
	{
		iType = LOGIN_QQ;
	}
	ptrLogin->m_iCurLogin = iType;

	__String* strData = __String::createWithFormat("openid=%s&registerType=%d", loginno.c_str(), iType);
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData->getCString());
	__String* data = __String::createWithFormat("{\"openid\":\"%s\",\"registerType\":%d,\"validateCode\":\"%s\"}", loginno.c_str(), iType, validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("otherLogin",
		HttpRequest::Type::POST,
		ptrLogin,
		callfuncND_selector(LoginLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"POST_WXQQ");
}

void LoginLayer::showLoginWarn(string strMSG)
{
	ITools::getInstance()->removeWaitLayer();
	ITools::getInstance()->createGlobalPrompt(strMSG.c_str(), false);
}

void LoginLayer::removeWaitLayer(float fDelay)
{
	unschedule(schedule_selector(LoginLayer::removeWaitLayer));
	ITools::getInstance()->removeWaitLayer();
}

void LoginLayer::PlatRemoveWaitSprite()
{
	ITools::getInstance()->removeWaitLayer();
}

void LoginLayer::callBackUserClean(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_mEditBox[0]->setText("");
	if (m_iPage == PAGE_LOGIN)
	{
		m_mEditBox[0]->setPlaceHolder(LL("inputuser"));
	}
	else if(m_iPage == PAGE_REGISTER)
	{
		m_mEditBox[0]->setPlaceHolder(LL("registernotice"));
	}
	updateBtnClean();
}

void LoginLayer::callBackPassClean(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_mEditBox[1]->setText("");
	if(m_iPage == PAGE_LOGIN)
	{
		m_mEditBox[1]->setPlaceHolder(LL("inputpassword"));
	}
	else if(m_iPage == PAGE_REGISTER)
	{
		m_mEditBox[1]->setPlaceHolder(LL("registernotice"));
	}
	updateBtnClean();
}

void LoginLayer::callBackRegister(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_btnRegister->setVisible(false);
	m_btnLogin->setVisible(false);
	m_btnQQ->setVisible(false);
	m_btnWX->setVisible(false);
	m_spOther->setVisible(false);
	m_spNotice->setVisible(true);
	m_btnRegId->setVisible(true);
	m_iPage = PAGE_REGISTER;

	m_mEditBox[0]->setText("");
	m_mEditBox[1]->setText("");
	m_mEditBox[0]->setPlaceHolder(LL("registernotice"));
	m_mEditBox[1]->setPlaceHolder(LL("registernotice"));
	updateBtnClean();
}

void LoginLayer::callBackRequest(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	//检查数据合法性
	string strUser = m_mEditBox[0]->getText();
	string strPass = m_mEditBox[1]->getText();

	if(strUser.length() < TEXT_MIN_NUM || !checkDataUsable(strUser))
	{
		ITools::getInstance()->createGlobalPrompt(LL("zhanghaocuowu"), false);
		return;
	}
	else if(strPass.length() < TEXT_MIN_NUM || !checkDataUsable(strPass))
	{
		ITools::getInstance()->createGlobalPrompt(LL("mimacuowu"), false);
		return;
	}

	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	MenuItemSprite* item = (MenuItemSprite*)sender;
	switch(item->getTag())
	{
	case PAGE_LOGIN:
	{
		strURL = "login";
		string data = StringUtils::format("username=%s&password=%s", strUser.c_str(), strPass.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"username\":\"%s\",\"password\":\"%s\",\"validateCode\":\"%s\"}", strUser.c_str(), strPass.c_str(), validCode.c_str());
		strTag = "POST_USER_LOGIN";
	}
		break;
	case PAGE_REGISTER:
	{
		strURL = "register";
		string data = StringUtils::format("registerType=%d&username=%s&password=%s", LOGIN_USER, strUser.c_str(), strPass.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"registerType\":%d,\"username\":\"%s\",\"password\":\"%s\",\"validateCode\":\"%s\"}", LOGIN_USER, strUser.c_str(), strPass.c_str(), validCode.c_str());
		strTag = "POST_REGISTER";
	}
		break;
	case PAGE_BIND:
	{
		strURL = "bindingUser";
		string strToken = history->m_pLoginInfo->getGameToken();
		string data = StringUtils::format("gameToken=%s&username=%s&password=%s", strToken.c_str(), strUser.c_str(), strPass.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"username\":\"%s\",\"password\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), strUser.c_str(), strPass.c_str(), validCode.c_str());
		strTag = "POST_BINDING";
	}
		break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(LoginLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void LoginLayer::callBackQQLogin(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	//ITools::getInstance()->addWaitLayer();
	//scheduleOnce(schedule_selector(LoginLayer::removeWaitLayer), 8.0f); 	//8S后自动清除
	//sendQQAuthRequest();
}

void LoginLayer::callBackWXLogin(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	//ITools::getInstance()->addWaitLayer();
	//scheduleOnce(schedule_selector(LoginLayer::removeWaitLayer), 8.0f); 	//8S后自动清除
	//sendWXAuthRequest();
}

void LoginLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

bool LoginLayer::checkDataUsable(string strData)
{
	bool bUsable = true;
	if (strData == "")
	{
		return false;
	}

	for(UInt i = 0; i < strData.length(); i++)
	{
		char ch = strData.at(i);
		if (ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'z')
		{
		}
		else
		{
			bUsable = false;
			break;
		}
	}
	return bUsable;
}

void LoginLayer::updateBtnClean()
{
	for(UInt i = 0; i < m_mEditBox.size(); i++)
	{
		string strText = m_mEditBox[i]->getText();
		m_mClean[i]->setVisible(strText != "");
	}
}

void LoginLayer::updateBind()
{
	m_btnRegister->setVisible(false);
	m_btnLogin->setVisible(false);
	m_btnQQ->setVisible(false);
	m_btnWX->setVisible(false);
	m_spOther->setVisible(false);
	m_spNotice->setVisible(false);

	m_spBind->setVisible(true);
	m_btnBind->setVisible(true);
	m_iPage = PAGE_REGISTER;

	m_mEditBox[0]->setText("");
	m_mEditBox[1]->setText("");
	m_mEditBox[0]->setPlaceHolder(LL("registernotice"));
	m_mEditBox[1]->setPlaceHolder(LL("registernotice"));
	updateBtnClean();
}
