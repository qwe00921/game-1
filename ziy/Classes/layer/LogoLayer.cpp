//
//  LogoLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/11.
//
//

#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "LogoLayer.h"
#include "otherfunction.h"
#include "NewLoginLayer.h"
#ifdef WIN32
	#include "LoginLayer.h"
#endif
#include<iostream>
#include "CocoMd5.h"
#include "LoginRequest.h"
#include "RoleCreateLayer.h"
#include "Net_HomePage.h"
#include "NetNoticelayer.h"
#include "StatConfig.h"
#include "DestroySixCountriesLayer.h"
#include "PaymentTool.h"
#include "History.h"
#include "NetPromptTool.h"

#define GUIDE_INIT_LEVEL 94

static Json::Value s_loginInfoJson = Json::Value::null;
static bool s_loginOKOccour = false;

static Json::Value s_globalBoolValue = Json::Value::null;

LogoLayer::LogoLayer()
{
	//uc login test
	//s_loginInfoJson["openId"] = "ssh1game204ac053169c41ca97c392392dc06919173158";//right
	//s_loginInfoJson["openId"] = "ssh1game204ac056139c41ca97c392392dc06919170000";//wrong
	//s_loginInfoJson["sdk"] = "uc";
	//s_loginOKOccour = true;

	//s_loginInfoJson["openId"] = "{\"n\":\"DC8FA60F\",\"v\":\"97AEC1D5AEF376FA67FFA29B5B5B33CB7F647095\",\"h\":\"A5FE67CA2A354ACAAF91F91C3B399A0F\",\"t\":\"1492068338\"}";//18621506463
	//s_loginInfoJson["sdk"] = "am";
	//s_loginOKOccour = true;
}

LogoLayer::~LogoLayer()
{

}

void LogoLayer::onEnter()
{
	Layer::onEnter();
}

void LogoLayer::onExit()
{
	Layer::onExit();
}

bool LogoLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (m_bSucceed)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		if(history->m_pPlayerInfo->m_mHero.size())
		{
			for (int i= GUIDE_INIT_LEVEL;i<(GUIDE_INIT_LEVEL +3);i++)
			{
				if (history->mMapDateInfo->isPassed(i)) {

				}
				else {
					ITools::getInstance()->startFight(i, BATTLE_SIXWAR, 0);
					return true;
				}
			}

			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create());
			this->removeFromParent();
		}
		else
		{
			ChessApplication::getInstance()->AddFunctionLayer(RoleCreateLayer::create(this));
		}
	}

	return true;
}


void LogoLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void LogoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

LogoLayer* LogoLayer::create(bool bIsSwitch /*= false*/)
{
	LogoLayer* pRet = new LogoLayer();
	if(pRet&&pRet->init(bIsSwitch))
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

bool LogoLayer::getBoolV(const char* key) {
	if (s_globalBoolValue.empty()) {
		return false;
	}
	if (s_globalBoolValue[key].isBool()) {
		return s_globalBoolValue[key].asBool();
	}
	return false;
}

void LogoLayer::setBoolV(const char* key, bool v) {
	s_globalBoolValue[key] = v;
}

void LogoLayer::javaCallCppBridge(const char* jsonStr) {
	Json::Reader reader;
	Json::Value infoJson;
	reader.parse(jsonStr, infoJson);
	if (!infoJson.isNull()) {
		CCLOG("%s", infoJson.toStyledString().c_str());
		if (!infoJson["name"].isNull()) {
			std::string funcName = infoJson["name"].asString();
			if (funcName.compare("SDKExitCancel") == 0) {
				LogoLayer::setBoolV("SDKDialogExit", false);
			}
			else if (funcName.compare("SDKChangeAccount") == 0) {
				ChessViewTool::getInstance()->setScreenDefaultSize();
				Chess::getInstance()->removeAllScreen();
				ChessApplication::getInstance()->removeAllFunctionLayer();
				NetPromptTool::getInstance()->StopGetNetPrompt();
				ChessApplication::getInstance()->AddFunctionLayer(LogoLayer::create(true));
			}
		}
	}
}

int LogoLayer::getIntValue(const char* name) {
	if (strcmp(name, "jade") == 0) {
		return history->m_iJade;
	}
	if (strcmp(name, "vip") == 0) {
		return history->m_iVIP;
	}
	if (strcmp(name, "guildId") == 0) {
		return history->m_pPlayerInfo->getGuildId();
	}
	if (strcmp(name, "isPatriarch") == 0) {
		if (history->m_pPlayerInfo->getGuildId() != 0) {
			std::string patriarch = history->m_pPlayerInfo->getpatriarch();
			if (!patriarch.empty() && !history->m_pPlayerInfo->getName().empty()) {
				if (strcmp(patriarch.c_str(), history->m_pPlayerInfo->getName().c_str()) == 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

std::string LogoLayer::getStrValue(const char* name) {
	CCLOG("xxoo getStrValue %s", name);
	std::string rs = "";
	if (strcmp(name, "guildName") == 0) {
		rs = history->m_pPlayerInfo->getGuildName();
	}
	else if (strcmp(name, "getDataForSDKAnalys") == 0) {
		rs = getDataForSDKAnalys("getDataForSDKAnalys");
	}
	else if (strcmp(name, "patriarch") == 0) {
		rs = history->m_pPlayerInfo->getpatriarch();
	}
	return rs;
}


std::string LogoLayer::getDataForSDKAnalys(const char* flag, int level) {
	//CCLOG("xxoo getDataForSDKAnalys %s",flag);
	PlayerInfo* pInfo = history->m_pPlayerInfo;
	LoginInfo* pLoginInfo = history->m_pLoginInfo;
	Json::Value callStr;
	callStr["name"] = "submitExtendData";
	callStr["markCode"] = IntToString(pLoginInfo->getMarkCode());
	callStr["accountId"] = pLoginInfo->getLoginElem(pLoginInfo->getLoginId())->getOpenId();
	callStr["roleId"] = pLoginInfo->getMarkCode();
	callStr["roleName"] = pInfo->getName().c_str();
	callStr["totalCP"] = pInfo->getTotalCP();
	callStr["vip"] = history->m_iVIP;
	HeroElem* mainHero = pInfo->getMainHero();
	if (mainHero) {
		callStr["level"] = mainHero->getLV();
		string gender = StringUtils::format("gender%d", mainHero->getGender());
		callStr["gender"] = __String::createWithFormat("%s", LL(gender.c_str()))->getCString();
	}
	else {
		callStr["gender"] =  __String::createWithFormat("%s", LL("wu"))->getCString();
		callStr["level"] = level;
	}
	callStr["flag"] = flag;
	callStr["createdAt"] = pInfo->getRoleCreateTime();
	//CCLOG("rrrr:%s", callStr.toStyledString().data());
	return callStr.toStyledString();
}

void LogoLayer::setLoginInfo(const char* jsonStr) {
	Json::Reader reader;
	reader.parse(jsonStr, s_loginInfoJson);
	if (!s_loginInfoJson.isNull()) {
		//CCLOG("%s", s_loginInfoJson.toStyledString().c_str());
		if (!s_loginInfoJson["sdkPayCallName"].isNull()) {
			PaymentTool::getInstance()->setmSdkPayCallName(s_loginInfoJson["sdkPayCallName"].asCString());
		}
		s_loginOKOccour = true;
	}
}



void LogoLayer::checkLoginState(float fDelay) {
	if (s_loginOKOccour){
		s_loginOKOccour = false;
		CCLOG("occour to updateLoginState");
		updateLoginState();
	}
}

bool LogoLayer::init(bool bIsSwitch)
{
	if(!Layer::init())
	{
		return false;
	}
	if (bIsSwitch==false)
	{
		Director::getInstance()->getRunningScene()->addChild(LogoLayerWaiting::getInstance(), 1000);

		LogoLayerWaiting::getInstance()->m_Progress->runAction(ProgressTo::create(2, 100));
	}
	else if (bIsSwitch==true)
	{

	}

	trData->loadSettingFlag();
	ChessPlayMp3(MUSIC_LOGO);
	history->setting_flag[6] = 0; //移动速度默认快

	m_bSucceed = false;
	m_bGuestSwitch = false;
	m_spHeroImg = NULL;
	m_spWeapon = NULL;

	initControl();
	if (bIsSwitch)
	{
		updateShow(0);
	}
	else
	{
		scheduleOnce(schedule_selector(LogoLayer::getServerState), 0.2f);
	}
	schedule(schedule_selector(LogoLayer::checkLoginState), 0.2f);
	if (bIsSwitch==false)
	{
		scheduleOnce(schedule_selector(LogoLayer::runHeroAdd), 2.2f);
		scheduleOnce(schedule_selector(LogoLayer::runHeroGo), 3.5f);
	}
	else if(bIsSwitch==true){
		scheduleOnce(schedule_selector(LogoLayer::runHeroAdd), 0.2f);
		scheduleOnce(schedule_selector(LogoLayer::runHeroGo), 1.5f);
	}

	return true;
}

void LogoLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/logo_eff.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("new_ui/bg/loding/NewProject.ExportJson", 0, 1));

	////背景
	//Sprite* spLayerBG = ReadSpriteName("new_ui/main/logo_bg1.jpg");
	//spLayerBG->setPosition(point);
	//this->addChild(spLayerBG, -10);

	armBG = Armature::create("NewProject");
	armBG->setAnchorPoint(Vec2(0.5, 0.5));
	armBG->setPosition(point);
	this->addChild(armBG, -10);


	////LOGO动画
	//Armature* armLogo = Armature::create("logo_eff");
	//armLogo->getAnimation()->play("logo");
	//armLogo->setScale(0.66f);
	//Size szLogo = armLogo->getContentSize() * 0.66f;
	//armLogo->setPosition(Vec2(szLogo.width / 2 + 10, size.height - szLogo.height / 2 - 3));
	//this->addChild(armLogo);

	////LOGO动画1
	//Armature* armLogo1 = Armature::create("logo_eff");
	//armLogo1->getAnimation()->play("logo1");
	//armLogo1->setPosition(point);
	//this->addChild(armLogo1, -10);
	//armLogo1->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	//{
	//	if (type == COMPLETE)
	//	{
	//		//LOGO动画2
	//		Armature* armLogo2 = Armature::create("logo_eff");
	//		armLogo2->getAnimation()->play("logo2");
	//		armLogo2->setPosition(point);
	//		this->addChild(armLogo2, -10);
	//		armLogo1->removeFromParent();
	//	}
	//});

	//广电总局版号
	Label* labGDNum = LabelSystemFont(LL("gdzjnum"), 14);
	labGDNum->setAnchorPoint(Vec2(0.0f, 0.0f));
	labGDNum->setPosition(Vec2(5, 5));
	labGDNum->enableOutline(Color4B::BLACK, 1);
	this->addChild(labGDNum);

	//版本号
	DString strAppVer = statGetDeviceParams(StatParamVersion);
	string strVersion = StringUtils::format("%s%s", LL("banbenhao"), strAppVer->getChar());
	if(history->m_iPatchVersion)
	{
		strVersion = StringUtils::format("%s.%d", strVersion.c_str(), history->m_iPatchVersion);
	}
	Label* labVersion = LabelSystemFont(strVersion, 16);
	labVersion->setAnchorPoint(Vec2(1.0f, 0.0f));
	labVersion->setPosition(Vec2(size.width - 5, 5));
	labVersion->enableOutline(Color4B::BLACK, 1);
	this->addChild(labVersion);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//游客登录
#ifdef WIN32
	Sprite* spGuestN = ReadSpriteName("new_ui/sign/accounts_guest.png");
	Sprite* spGuestS = ReadSpriteName("new_ui/sign/accounts_guest.png");
	spGuestS->setScale(fScale);
	m_pBtnGuest = MenuItemSprite::create(spGuestN, spGuestS, CC_CALLBACK_1(LogoLayer::callBackGuest, this));
	spGuestS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spGuestS->setPosition(Vec2(spGuestS->getContentSize().width / 2, spGuestS->getContentSize().height / 2));
	m_pBtnGuest->setPosition(point + Vec2(-120, -115));
	m_pBtnMenu->addChild(m_pBtnGuest);
#endif
	//用户登录
	Sprite* spUserN = ReadSpriteName("new_ui/sign/accounts_member.png");
	Sprite* spUserS = ReadSpriteName("new_ui/sign/accounts_member.png");
	spUserS->setScale(fScale);
	m_pBtnUser = MenuItemSprite::create(spUserN, spUserS, CC_CALLBACK_1(LogoLayer::callBackUser, this));
	spUserS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spUserS->setPosition(Vec2(spUserS->getContentSize().width / 2, spUserS->getContentSize().height / 2));
#ifdef WIN32
	m_pBtnUser->setPosition(point + Vec2(120, -115));
#else
	m_pBtnUser->setPosition(point + Vec2(0, -115));
#endif
	m_pBtnMenu->addChild(m_pBtnUser);

	//登陆
	Sprite* spLoginN = ReadSpriteName("new_ui/sign/member_info.png");
	Sprite* spLoginS = ReadSpriteName("new_ui/sign/member_info.png");
	spLoginS->setScale(fScale);
	MenuItemSprite* btnLogin = MenuItemSprite::create(spLoginN, spLoginS, CC_CALLBACK_1(LogoLayer::callBackLogin, this));
	spLoginS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLoginS->setPosition(Vec2(spLoginS->getContentSize().width / 2, spLoginS->getContentSize().height / 2));
	btnLogin->setPosition(point + Vec2(size.width / 2 - 30, size.height / 2 - 32));
	m_pBtnMenu->addChild(btnLogin);

	//公告
	Sprite* spNoticeN = ReadSpriteName("new_ui/sign/bulletin_info.png");
	Sprite* spNoticeS = ReadSpriteName("new_ui/sign/bulletin_info.png");
	spNoticeS->setScale(fScale);
	MenuItemSprite* btnNotice = MenuItemSprite::create(spNoticeN, spNoticeS, CC_CALLBACK_1(LogoLayer::callBackNotice, this));
	spNoticeS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNoticeS->setPosition(Vec2(spNoticeS->getContentSize().width / 2, spNoticeS->getContentSize().height / 2));
	btnNotice->setPosition(point + Vec2(size.width / 2 - 30, size.height / 2 - 85));
	m_pBtnMenu->addChild(btnNotice);

	//创建角色
	m_spCreate = ReadSpriteName("new_ui/main/main_bg6.png");
	m_spCreate->setPosition(point + Vec2(0, -105));
	m_spCreate->setVisible(false);
	this->addChild(m_spCreate);

	Sprite* spHero = ReadSpriteName("new_ui/main/main_bg7.png");
	spHero->setPosition(Vec2(79, m_spCreate->getContentSize().height / 2));
	m_spCreate->addChild(spHero);

	Label* labCreate = LabelSystemFont(LL("createhero"), 16);
	labCreate->setAnchorPoint(Vec2(0.0f, 0.5f));
	labCreate->setPosition(Vec2(115, m_spCreate->getContentSize().height / 2));
	labCreate->enableOutline(Color4B::BLACK, 1);
	m_spCreate->addChild(labCreate);

	//人物信息
	m_spHeroInfo = ReadSpriteName("new_ui/main/main_bg6.png");
	m_spHeroInfo->setPosition(point + Vec2(0, -90));
	m_spHeroInfo->setVisible(false);
	this->addChild(m_spHeroInfo);

	m_labInfo = LabelSystemFont("", 14);
	m_labInfo->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labInfo->setPosition(Vec2(115, m_spHeroInfo->getContentSize().height / 2));
	m_labInfo->setDimensions(100, 0);
	m_labInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	m_labInfo->enableOutline(Color4B::BLACK, 1);
	m_spHeroInfo->addChild(m_labInfo);

	//开始游戏
	m_spStart = ReadSpriteName("new_ui/sign/accounts_ok.png");
	m_spStart->setPosition(point + Vec2(0, -135));
	m_spStart->setVisible(false);
	this->addChild(m_spStart);
}

void LogoLayer::callBackGuest(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	
	LoginElem* ptrLogin = history->m_pLoginInfo->getLoginElem(LOGIN_GUEST);
	if (ptrLogin)
	{
		string strOpenId = history->m_pLoginInfo->m_mLogInfo[LOGIN_GUEST]->getOpenId();
		__String* strData = __String::createWithFormat("openid=%s&registerType=%d", strOpenId.c_str(), LOGIN_GUEST);
		CocosMD5 md5;
		string validateCode = md5.ToMD5(strData->getCString());
		__String* data = __String::createWithFormat("{\"openid\":\"%s\",\"registerType\":%d,\"validateCode\":\"%s\"}", strOpenId.c_str(), LOGIN_GUEST, validateCode.c_str());
		CCLOG("%s", data->getCString());

		ITools::getInstance()->connectingNetwork("otherLogin",
			HttpRequest::Type::POST,
			this,
			callfuncND_selector(LogoLayer::onHttpRequestCompleted),
			data->getCString(),
			ITools::getInstance()->mHead);

		m_bGuestSwitch = true;
		ITools::getInstance()->addWaitLayer();
	}
	else
	{
		ChessApplication::getInstance()->AddFunctionLayer(GuestLayer::create(this));
	}
}

void LogoLayer::callBackUser(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
#ifdef WIN32
	ChessApplication::getInstance()->AddFunctionLayer(LoginLayer::create(this));
#else
	Json::Value callStr;
	callStr["name"] = "startSDKLogin";
	cppBridgeCall(callStr.toStyledString().c_str());
#endif
}

void LogoLayer::callBackLogin(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_bSucceed)
	{
		ChessApplication::getInstance()->AddFunctionLayer(LoginSwitchLayer::create(this));
	}
	else
	{
#ifdef WIN32
		ChessApplication::getInstance()->AddFunctionLayer(LoginLayer::create(this));
#else
		Json::Value callStr;
		callStr["name"] = "startSDKLogin";
		cppBridgeCall(callStr.toStyledString().c_str());
#endif
	}
}

void LogoLayer::callBackNotice(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(trData->m_pNetNotice)
	{
		ChessApplication::getInstance()->AddFunctionLayer(NetNoticelayer::create());
	}
}

void LogoLayer::updateState()
{
#ifdef WIN32
	m_pBtnGuest->setVisible(false);
#endif
	m_pBtnUser->setVisible(false);
	if (history->m_pPlayerInfo->m_mHero.size())
	{
		m_spCreate->setVisible(false);
		m_spHeroInfo->setVisible(true);
		m_spStart->setVisible(true);
		updateHeroInfo();
	}
	else
	{
		m_spCreate->setVisible(true);
		m_spHeroInfo->setVisible(false);
		m_spStart->setVisible(false);
		if (m_spHeroImg)
		{
			m_spHeroImg->setVisible(false);
		}
		if (m_spWeapon)
		{
			m_spWeapon->setVisible(false);
		}
	}
	if (!m_bSucceed) {

		Net_HomePage::InitHistory();
	}
	m_bSucceed = true;
	m_bGuestSwitch = false;
}

void LogoLayer::updateShow(int index)
{
#ifdef WIN32
	m_pBtnGuest->setVisible(true);
#endif
	m_pBtnUser->setVisible(true);
	m_spCreate->setVisible(false);
	m_spHeroInfo->setVisible(false);
	m_spStart->setVisible(false);
	if(m_spHeroImg)
	{
		m_spHeroImg->setVisible(false);
	}
	if(m_spWeapon)
	{
		m_spWeapon->setVisible(false);
	}
	m_bSucceed = false;
}

void LogoLayer::closeLayer(Ref *sender)
{
	this->removeFromParent();
}

void LogoLayer::updateHeroInfo()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	HeroElem* ptrHero = history->m_pPlayerInfo->getMainHero();
	int iOffsetIndex = 6;
	int iFashion1 = ptrHero->getFashion1();
	int iFashion2 = ptrHero->getFashion2();
	int iFashion3 = history->m_pPlayerInfo->geteLeadFashion();
	int iFashion4 = history->m_pPlayerInfo->geteLeadWapFashion();
	int iGender = ptrHero->getGender();
	if (iGender < 1) iGender = 1;
	if (iFashion3 > 0)
		iFashion1 = iFashion3;
	if (iFashion4 > 0)
		iFashion2 = iFashion4;

	__String* heroPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* weaponPath = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	m_spWeapon = ReadSpriteNormal(weaponPath->getCString());
	m_spWeapon->setPosition(point + Vec2(-50, -85));
	Size weaponSize = m_spWeapon->getContentSize();
	m_spWeapon->setTextureRect(Rect(0, weaponSize.width * iOffsetIndex, weaponSize.width, weaponSize.width));
	this->addChild(m_spWeapon);

	m_spHeroImg = ReadSpriteNormal(heroPath->getCString());
	m_spHeroImg->setPosition(point + Vec2(-50, -85));
	Size heroSize = m_spHeroImg->getContentSize();
	m_spHeroImg->setTextureRect(Rect(0, heroSize.width * iOffsetIndex, heroSize.width, heroSize.width));
	this->addChild(m_spHeroImg);

	__String* heroName = __String::createWithFormat("%s\n%s%d", history->m_pPlayerInfo->getName().c_str(),LL("dengji"),ptrHero->getLV());
	m_labInfo->setString(heroName->getCString());
}

void LogoLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	HttpResponse* response = (HttpResponse*)resultdata;

	ITools::getInstance()->removeWaitLayer();

	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("LogoLayer %s...", strTag.c_str());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if(strTag == "REQ_SERVER")
		{
			getNetNotice();
			if (!s_loginInfoJson.isNull()) {
			}
			else {
				updateLoginState();
			}
			
		}
		else if(strTag == "REQ_GET_NOTICE")
		{
			Json::Value result = value["result"];
			NetNoticeInfo* ptrNotice = new NetNoticeInfo();
			ptrNotice->strTitle = result["title"].asString();
			ptrNotice->strContent = result["content"].asString();
			trData->m_pNetNotice = ptrNotice;
			ChessApplication::getInstance()->AddFunctionLayer(NetNoticelayer::create());
		}
		else
		{
			Json::Value result = value["result"];

			if(result["gameToken"].isString() && result["markCode"].isInt())
			{
				history->m_pLoginInfo->setGameToken(result["gameToken"].asString());
				history->m_pLoginInfo->setMarkCode(result["markCode"].asInt());

				history->m_pPlayerInfo->loadJson(result["playerInfo"]);

				int iLoginId = history->m_pLoginInfo->getLoginId();

				if (iLoginId == LOGIN_USER)
					Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), history->m_pLoginInfo->m_mLogInfo[iLoginId]->getUserName(), 2);
				else if (iLoginId == LOGIN_GUEST)
				{
					if (m_bGuestSwitch && result["openid"].isString())
					{
						history->m_pLoginInfo->addLoginElem(LOGIN_GUEST, LL("youke"), "", "", result["openid"].asString());
					}
					Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), "", 1);
				}
				else if (iLoginId == LOGIN_WX || iLoginId == LOGIN_QQ)
				{
					//TODO  QQ WX登录
					Chess::getInstance()->getStatConfig()->SetAccountConfig(IntToString(history->m_pLoginInfo->getMarkCode()), "", 3);

				}
				else
				{
					if (result["sdkAccountData"].isObject()) {
						Json::Value callStr;
						callStr["name"] = "setExtUserInfo";
						callStr["sdkAccountData"] = result["sdkAccountData"].toStyledString().c_str();
						cppBridgeCall(callStr.toStyledString().c_str());
					}

					const char* accountId = result["openid"].asCString();
					std::string strType;
					__String* keyStr = __String::createWithFormat("iLoginId%d", iLoginId);
					if (LL(keyStr->getCString(), false, true))
						strType = LL(keyStr->getCString(), false, true);
					else
						strType = keyStr->getCString();

					history->m_pLoginInfo->addLoginElem(iLoginId, strType.c_str(), "", "", accountId);


					StatConfig::submitExtendData(LogoLayer::getDataForSDKAnalys("login").data());
				}
				updateState();
			}
		}
	}

	m_bGuestSwitch = false;
}

void LogoLayer::updateLoginState()
{
	if (!s_loginInfoJson.isNull()) {
		if (strncmp(s_loginInfoJson["sdk"].asCString() , "uc",2) == 0) {
			history->m_pLoginInfo->setLoginId(LOGIN_UC);
			CCLOG("UC");
		}else if (strncmp(s_loginInfoJson["sdk"].asCString() , "am",2) == 0) {
			history->m_pLoginInfo->setLoginId(LOGIN_AM);
			CCLOG("AM");
		}
		else {
			if (s_loginInfoJson["iLoginId"].isNumeric()) {
				int aiLoginId = s_loginInfoJson["iLoginId"].asInt();
				history->m_pLoginInfo->setLoginId(aiLoginId);
			}
		}
	}
	int iLoginId = history->m_pLoginInfo->getLoginId();
	if (iLoginId == LOGIN_NONE)
	{
	}
	else if(iLoginId == LOGIN_USER)
	{
		string strUser = history->m_pLoginInfo->m_mLogInfo[iLoginId]->getUserName();
		string strPass = history->m_pLoginInfo->m_mLogInfo[iLoginId]->getPassword();
		CocosMD5 md5;
		__String* strData = __String::createWithFormat("username=%s&password=%s", strUser.c_str(), strPass.c_str());
		string validateCode = md5.ToMD5(strData->getCString());
		__String* data = __String::createWithFormat("{\"username\":\"%s\",\"password\":\"%s\",\"validateCode\":\"%s\"}", strUser.c_str(), strPass.c_str(), validateCode.c_str());
		CCLOG("%s", data->getCString());

		ITools::getInstance()->connectingNetwork("login",
			HttpRequest::Type::POST,
			this,
			callfuncND_selector(LogoLayer::onHttpRequestCompleted),
			data->getCString(),
			ITools::getInstance()->mHead);

		ITools::getInstance()->addWaitLayer();
	}
	else if(iLoginId == LOGIN_GUEST || iLoginId == LOGIN_WX || iLoginId == LOGIN_QQ || iLoginId == LOGIN_UC || iLoginId == LOGIN_AM || (!s_loginInfoJson.isNull()))
	{
		string strOpenId;
		if (!s_loginInfoJson.isNull()) {
			strOpenId = s_loginInfoJson["openId"].asCString();
		}
		if (strOpenId.empty()) {
			if (history->m_pLoginInfo->m_mLogInfo[iLoginId])
				strOpenId = history->m_pLoginInfo->m_mLogInfo[iLoginId]->getOpenId();
		}
		__String* data;
		if (iLoginId == LOGIN_AM)
		{ 
			char *encodedData = 0;

			base64Encode((const unsigned char *)strOpenId.c_str(), strOpenId.length(), &encodedData);

			__String* strData = __String::createWithFormat("openid=%s&registerType=%d", encodedData, iLoginId);
	
			CocosMD5 md5;
			string validateCode = md5.ToMD5(strData->getCString());
			data = __String::createWithFormat("{\"openid\":\"%s\",\"registerType\":%d,\"validateCode\":\"%s\"}", encodedData, iLoginId, validateCode.c_str());
			if (encodedData)
				free(encodedData);
		}
		else {
			__String* strData = __String::createWithFormat("openid=%s&registerType=%d", strOpenId.c_str(), iLoginId);

			CocosMD5 md5;
			string validateCode = md5.ToMD5(strData->getCString());
			if (!s_loginInfoJson.isNull()) {
				char *encodedData = 0;
				std::string sdkData = s_loginInfoJson.toStyledString();
				base64Encode((const unsigned char *)sdkData.data(), sdkData.length(), &encodedData);
				data = __String::createWithFormat("{\"openid\":\"%s\",\"loginData\":\"%s\",\"registerType\":%d,\"validateCode\":\"%s\"}", strOpenId.c_str(), encodedData, iLoginId, validateCode.c_str());
				if (encodedData)
					free(encodedData);
			}
			else {
				data = __String::createWithFormat("{\"openid\":\"%s\",\"registerType\":%d,\"validateCode\":\"%s\"}", strOpenId.c_str(), iLoginId, validateCode.c_str());

			}
		}


		ITools::getInstance()->connectingNetwork("otherLogin",
			HttpRequest::Type::POST,
			this,
			callfuncND_selector(LogoLayer::onHttpRequestCompleted),
			data->getCString(),
			ITools::getInstance()->mHead);

		ITools::getInstance()->addWaitLayer();
	}
}

void LogoLayer::getServerState(float fDelay)
{
	string strVersion = statGetDeviceParams(StatParamVersion)->getChar();
	__String* data = __String::createWithFormat("version=%s", strVersion.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"version\":\"%s\",\"validateCode\":\"%s\"}", strVersion.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("hello",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(LogoLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead,
		"REQ_SERVER");

	ITools::getInstance()->addWaitLayer();
}

void LogoLayer::getNetNotice()
{
	ITools::getInstance()->connectingNetwork("getNotice",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(LogoLayer::onHttpRequestCompleted),
		"{\"aaa\":111}",
		ITools::getInstance()->mHead,
		"REQ_GET_NOTICE");

	ITools::getInstance()->addWaitLayer();
}
void LogoLayer::runHeroAdd(float fDelay)
{
	
	armBG->getAnimation()->play("add_hero", -1, 0);
	//armLogo->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(BattleTitleLayer::framCallback));

	
}
void LogoLayer::runHeroGo(float fDelay)
{
	armBG->getAnimation()->play("hero_go", -1, 1);
}
LogoLayerWaiting* LogoLayerWaiting::m_LogoWaiting = NULL;
LogoLayerWaiting * LogoLayerWaiting::getInstance()
{
	if (m_LogoWaiting == NULL)
	{
		m_LogoWaiting = LogoLayerWaiting::create();
	}
	return m_LogoWaiting;
}

bool LogoLayerWaiting::init()
{
	if (!Layer::init())
	{
		return false;
	}
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("new_ui/logo_loding/backgroud/NewProject.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("new_ui/logo_loding/xiaoma/xiaoma.ExportJson", 0, 1));
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//DSizeF size = ChessViewTool::getInstance()->getFullSize();
	float m_fPosX = 355;
	float m_fPosY = 200;
	m_fPosX = m_fPosX - 710 / 2 + size.width / 2;
	m_fPosY = m_fPosY - 400 / 2 + size.height / 2;
	

	Armature* armLogo = Armature::create("NewProject");
	armLogo->getAnimation()->play("Animation1");
	armLogo->setAnchorPoint(Vec2(0.5, 0.5));
	armLogo->setPosition(point);
	this->addChild(armLogo);



	Sprite* _pro= ReadSpriteName("new_ui/logo_loding/progress.png");
	m_Progress= ProgressTimer::create(_pro);
	m_Progress->setScale(0.5f);
	m_Progress->setType(kCCProgressTimerTypeBar);
	m_Progress->setPercentage(0);
	m_Progress->setBarChangeRate(ccp(1, 0));
	m_Progress->setMidpoint(ccp(0, 0));

	m_LogoProgressNum = LabelSystemFont("", 32);
	//m_LogoProgressNum->setColor(Color3B(0,0,255));
	m_Progress->setPosition(Vec2(m_fPosX, m_fPosY-70));
	m_Progress->addChild(m_LogoProgressNum);
	m_LogoProgressNum->setPosition(Vec2(_pro->getContentSize().width / 2, _pro->getContentSize().height / 2));

	Armature* armLogo2 = Armature::create("xiaoma");
	armLogo2->getAnimation()->play("Animation1");
	armLogo2->setAnchorPoint(Vec2(0.5, 0.5));
	armLogo2->setPosition(Vec2(m_fPosX, m_fPosY+20));
	this->addChild(armLogo2);

	this->addChild(m_Progress);

	this->scheduleUpdate();
	return true;
}

void LogoLayerWaiting::update(float dt)
{
	int num = m_Progress->getPercentage();

	if (num==100)
	{
		m_LogoWaiting = NULL;
		this->removeFromParent();
		return;
	}


	stringstream ss;
	string sNum;
	ss << num;
	ss >> sNum;
	std::cout << sNum << endl;

	m_LogoProgressNum->setString(sNum+"%");

}


