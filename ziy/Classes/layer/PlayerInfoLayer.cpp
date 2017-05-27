//
//  PlayerInfoLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/01.
//
//

#include "PlayerInfoLayer.h"
#include "SysSetLayer.h"
#include "LogoLayer.h"
#include "VipGiftLayer.h"
#include "LoginLayer.h"
#include "NetPromptTool.h"

#define EDIT_MAX_NUM 8
#define RENAME_PRICE 150

#define TEXT_MIN_NUM 2
#define TEXT_MAX_NUM 15

PlayerInfoLayer::PlayerInfoLayer()
{

}

PlayerInfoLayer::~PlayerInfoLayer()
{

}

PlayerInfoLayer* PlayerInfoLayer::create(EventDelegate* ptrDelegate)
{
	PlayerInfoLayer* pRet = new PlayerInfoLayer();
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

bool PlayerInfoLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_bIsBind = false;
	initControl();
	return true;
}

void PlayerInfoLayer::onEnter()
{
	Layer::onEnter();
}

void PlayerInfoLayer::onExit()
{
	Layer::onExit();
}

bool PlayerInfoLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	return true;
}

void PlayerInfoLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void PlayerInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void PlayerInfoLayer::updateState()
{
	//VIP图标
	string strPath = StringUtils::format("new_ui/main/vip%d.png", history->m_iVIP);
	m_spVIP->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
	if (m_pDelegate)
	{
		m_pDelegate->updateMoneyShow();
	}

	//绑定
	if (m_bIsBind)
	{
		//用户类型
		int iLoginId = history->m_pLoginInfo->getLoginId();
		m_labType->setString(history->m_pLoginInfo->m_mLogInfo[iLoginId]->getTypeName());

		//绑定按钮
		m_btnBind->setVisible(false);
		m_btnSwitch->setPosition(m_btnSwitch->getPosition() + Vec2(-85, 0));
		m_bIsBind = false;
		ITools::getInstance()->createGlobalPrompt(LL("bindsucceed"), true);
	}

	//刷新名字
	m_labName->setString(history->m_pPlayerInfo->getName());
}

void PlayerInfoLayer::initControl()
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
	//Sprite* spTitle = ReadSpriteName("new_ui/img_text/xi_txt1.png");
	//spTitle->setPosition(point + Vec2(0, 133));
	//this->addChild(spTitle);

	//按钮菜单
	auto btnMenu = Menu::create();
	btnMenu->setPosition(Vec2(0, 0));
	this->addChild(btnMenu);

	//角色背景
	Sprite* spHeroBG = ReadSpriteName("new_ui/img_global/mw_bg1.png");
	spHeroBG->setPosition(point + Vec2(-110, 65));
	this->addChild(spHeroBG);

	//时装或形象
	BgAddFashionSImg(1, history->actors[0].mFashion, point + Vec2(-110, 78), 6, this, NULL, NULL, "mov");

	//VIP图标
	string strVIP = StringUtils::format("new_ui/main/vip%d.png", history->m_iVIP);
	m_spVIP = ReadSpriteName(strVIP.c_str());
	m_spVIP->setPosition(point + Vec2(-110, 48));
	this->addChild(m_spVIP);

	//角色名字
	m_labName = LabelSystemFont(history->m_pPlayerInfo->getName().c_str(), 18);
	m_labName->setColor(Color3B(0x66, 0, 0));
	m_labName->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labName->setPosition(point + Vec2(-70, 85));
	this->addChild(m_labName);

	//改名
	Sprite* spRenameN = ReadSpriteName("new_ui/img_global/name_img1.png");
	Sprite* spRenameS = ReadSpriteName("new_ui/img_global/name_img1.png");
	spRenameS->setScale(fScale);
	auto btnRename = MenuItemSprite::create(spRenameN, spRenameS, CC_CALLBACK_1(PlayerInfoLayer::callBackRename, this));
	spRenameS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRenameS->setPosition(Vec2(spRenameS->getContentSize().width / 2, spRenameS->getContentSize().height / 2));
	btnRename->setPosition(point + Vec2(150, 85));
	btnMenu->addChild(btnRename);

	//登陆方式
	Label* labType = LabelSystemFont(LL("usertype"), 18);
	labType->setColor(Color3B::YELLOW);
	labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	labType->setPosition(point + Vec2(-70, 55));
	this->addChild(labType);

	int iLoginId = history->m_pLoginInfo->getLoginId();
	string strType = history->m_pLoginInfo->m_mLogInfo[iLoginId]->getTypeName();
	m_labType = LabelSystemFont(strType.c_str(), 18);
	m_labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labType->setPosition(point + Vec2(-70 + labType->getContentSize().width + 10, 55));
	this->addChild(m_labType);

	//内核编号
	Label* labMark = LabelSystemFont(LL("neiheid"), 18);
	labMark->setColor(Color3B::YELLOW);
	labMark->setAnchorPoint(Vec2(0.0f, 0.5f));
	labMark->setPosition(point + Vec2(-70, 30));
	this->addChild(labMark);

	Label* labMarkVal = LabelSystemFont(IntToString(history->m_pLoginInfo->getMarkCode()), 18);
	labMarkVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labMarkVal->setPosition(point + Vec2(-70 + labMark->getContentSize().width + 10, 30));
	this->addChild(labMarkVal);

	//会员特权
	Sprite* spVipN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spVipTextN = ReadSpriteName("new_ui/img_text/xitxt1_1.png");
	spVipTextN->setPosition(Vec2(spVipN->getContentSize().width / 2, spVipN->getContentSize().height / 2));
	spVipN->addChild(spVipTextN);
	Sprite* spVipS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spVipTextS = ReadSpriteName("new_ui/img_text/xitxt1_2.png");
	spVipTextS->setPosition(Vec2(spVipS->getContentSize().width / 2, spVipS->getContentSize().height / 2));
	spVipS->addChild(spVipTextS);
	MenuItemSprite* btnVip = MenuItemSprite::create(spVipN, spVipS, CC_CALLBACK_1(PlayerInfoLayer::callBackVip, this));
	btnVip->setPosition(point + Vec2(-85, -36));
	btnMenu->addChild(btnVip);

	//系统设置
	Sprite* spSetN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spSetTextN = ReadSpriteName("new_ui/img_text/xitxt1_3.png");
	spSetTextN->setPosition(Vec2(spSetN->getContentSize().width / 2, spSetN->getContentSize().height / 2));
	spSetN->addChild(spSetTextN);
	Sprite* spSetS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spSetTextS = ReadSpriteName("new_ui/img_text/xitxt1_4.png");
	spSetTextS->setPosition(Vec2(spSetS->getContentSize().width / 2, spSetS->getContentSize().height / 2));
	spSetS->addChild(spSetTextS);
	MenuItemSprite* btnSet = MenuItemSprite::create(spSetN, spSetS, CC_CALLBACK_1(PlayerInfoLayer::callBackSysSet, this));
	btnSet->setPosition(point + Vec2(85, -36));
	btnMenu->addChild(btnSet);

	int iBtnOffset = 0;
	bool bVisible = false;
	if(history->m_pLoginInfo->m_mLogInfo[iLoginId]->getTypeId() == LOGIN_GUEST)
	{
		iBtnOffset = 85;
		bVisible = true;
	}

	//绑定帐号
	Sprite* spBindN = ReadSpriteName("new_ui/main/tb_icon3.png");
	Sprite* spBindS = ReadSpriteName("new_ui/main/tb_icon3.png");
	spBindS->setScale(fScale);
	m_btnBind = MenuItemSprite::create(spBindN, spBindS, CC_CALLBACK_1(PlayerInfoLayer::callBackBind, this));
	spBindS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spBindS->setPosition(Vec2(spBindS->getContentSize().width / 2, spBindS->getContentSize().height / 2));
	m_btnBind->setPosition(point + Vec2(-iBtnOffset, -112));
	m_btnBind->setVisible(bVisible);
	btnMenu->addChild(m_btnBind);

	//切换帐号
	Sprite* spSwitchN = ReadSpriteName("new_ui/main/tb_icon4.png");
	Sprite* spSwitchS = ReadSpriteName("new_ui/main/tb_icon4.png");
	spSwitchS->setScale(fScale);
	m_btnSwitch = MenuItemSprite::create(spSwitchN, spSwitchS, CC_CALLBACK_1(PlayerInfoLayer::callBackSwitch, this));
	spSwitchS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSwitchS->setPosition(Vec2(spSwitchS->getContentSize().width / 2, spSwitchS->getContentSize().height / 2));
	m_btnSwitch->setPosition(point + Vec2(iBtnOffset, -112));
	btnMenu->addChild(m_btnSwitch);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PlayerInfoLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(203, 152-15));
	btnMenu->addChild(btnClose);
}

void PlayerInfoLayer::callBackRename(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(RenameLayer::create(this));
}

void PlayerInfoLayer::callBackVip(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(VipGiftLayer::create(this));
}

void PlayerInfoLayer::callBackSysSet(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(SysSetLayer::create());
}

void PlayerInfoLayer::callBackBind(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_bIsBind = true;
	ChessApplication::getInstance()->AddFunctionLayer(LoginLayer::create(this, m_bIsBind));
}

void PlayerInfoLayer::callBackSwitch(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessViewTool::getInstance()->setScreenDefaultSize();
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	NetPromptTool::getInstance()->StopGetNetPrompt();
	ChessApplication::getInstance()->AddFunctionLayer(LogoLayer::create(true));
}

void PlayerInfoLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenameLayer::RenameLayer()
{

}

RenameLayer::~RenameLayer()
{

}

RenameLayer* RenameLayer::create(EventDelegate* ptrDelegate)
{
	RenameLayer* pRet = new RenameLayer();
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

bool RenameLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;

	initControl();
	return true;
}

void RenameLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/img_global/pop_bg1.png");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	//标题
	Sprite* spTitle = ReadSpriteName("new_ui/img_global/name_img2.png");
	spTitle->setPosition(point + Vec2(0, 93));
	this->addChild(spTitle);

	//名字背景
	auto spName = ReadSpriteName("new_ui/main/main_bg5.png");
	spName->setPosition(point + Vec2(10, 32));
	this->addChild(spName);

	//名字输入框
	auto spUserBG = ui::Scale9Sprite::create(getRespath("new_ui/hyaline/92x30.png", 0, 1));
	m_pEditBox = ui::EditBox::create(spUserBG->getContentSize(), spUserBG);
	m_pEditBox->setPosition(point + Vec2(10, 32));
	m_pEditBox->setFontSize(16);
	m_pEditBox->setFontColor(Color3B::WHITE);
	m_pEditBox->setMaxLength(EDIT_MAX_NUM);
	m_pEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_pEditBox->setDelegate(this);
	m_pEditBox->setText(history->m_pPlayerInfo->getName().c_str());
	this->addChild(m_pEditBox);

	//勾玉ICON
	Sprite* spJade = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJade->setPosition(point + Vec2(-20, -5));
	this->addChild(spJade);

	auto labJade = LabelSystemFont(StringUtils::format("%d", RENAME_PRICE), 16);
	labJade->setPosition(point + Vec2(15, -5));
	labJade->setColor(Color3B::GREEN);
	this->addChild(labJade);

	//按钮菜单
	auto btnMenu = Menu::create();
	btnMenu->setPosition(Vec2(0, 0));
	this->addChild(btnMenu);

	//随机名字
	Sprite* spRandN = ReadSpriteName("new_ui/main/icon_dice.png");
	Sprite* spRandS = ReadSpriteName("new_ui/main/icon_dice.png");
	spRandS->setScale(fScale);
	MenuItemSprite* btnRand = MenuItemSprite::create(spRandN, spRandS, CC_CALLBACK_1(RenameLayer::callBackRand, this));
	spRandS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRandS->setPosition(Vec2(spRandS->getContentSize().width / 2, spRandS->getContentSize().height / 2));
	btnRand->setPosition(point + Vec2(95, 32));
	btnMenu->addChild(btnRand);

	//确定
	Sprite* spConfirmN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spConTextN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	spConfirmN->addChild(spConTextN);
	Sprite* spConfirmS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spConTextS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	spConfirmS->addChild(spConTextS);
	MenuItemSprite* btnConfirm = MenuItemSprite::create(spConfirmN, spConfirmS, CC_CALLBACK_1(RenameLayer::callBackConfirm, this));
	btnConfirm->setPosition(point + Vec2(0, -60));
	spConTextN->setPosition(Vec2(spConfirmN->getContentSize().width / 2, spConfirmN->getContentSize().height / 2));
	spConTextS->setPosition(Vec2(spConfirmS->getContentSize().width / 2, spConfirmS->getContentSize().height / 2));
	btnMenu->addChild(btnConfirm);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(RenameLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(175, 88));
	btnMenu->addChild(btnClose);
}

void RenameLayer::callBackRand(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int index1 = rand() % trData->m_mFirstName.size();
	int index2 = rand() % trData->m_mSecName.size();
	string strName = trData->m_mFirstName[index1] + trData->m_mSecName[index2];
	m_pEditBox->setText(strName.c_str());
}

void RenameLayer::callBackConfirm(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	//检查昵称是否合法
	string strName = m_pEditBox->getText();
	if(strName.length() < TEXT_MIN_NUM)
	{
		ITools::getInstance()->createGlobalPrompt(LL("nichengcuowu"), false);
		return;
	}
	else if(strName.length() > TEXT_MAX_NUM)
	{
		ITools::getInstance()->createGlobalPrompt(LL("nichengguochang"), false);
		return;
	}
	else
	{
		DString dsName = strName.c_str();
		if(dsName->indexOf(' ') != -1)
		{
			ITools::getInstance()->createGlobalPrompt(LL("feifanicheng"), false);
			return;
		}

		for(UInt i = 0; i < trData->m_mNameForbid.size(); i++)
		{
			if(dsName->indexOf(trData->m_mNameForbid[i].c_str()) != -1)
			{
				ITools::getInstance()->createGlobalPrompt(LL("feifanicheng"), false);
				return;
			}
		}
	}

	if(history->m_iJade < RENAME_PRICE)
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
	}
	else
	{
		onHttpRequest();
	}
}

void RenameLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void RenameLayer::editBoxReturn(ui::EditBox* editBox)
{

}

void RenameLayer::onHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strData = StringUtils::format("gameToken=%s&name=%s", strToken.c_str(), m_pEditBox->getText());
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData);
	string data = StringUtils::format("{\"gameToken\":\"%s\",\"name\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), m_pEditBox->getText(), validateCode.c_str());
	CCLOG("%s", data.c_str());

	ITools::getInstance()->connectingNetwork("rename",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(RenameLayer::onHttpRequestCompleted),
		data,
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void RenameLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string reseData(buffer->begin(), buffer->end());
	CCLOG("%s...", reseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(reseData, value) && value.size() > 0)
	{
		onRequestSucceed(value["result"]);
	}
}

bool RenameLayer::onRequestSucceed(Json::Value valRes)
{
	if(valRes["isTrue"].isInt() && valRes["isTrue"].asInt() == 1)
	{
		if(valRes["jade"].isInt())
		{
			history->m_iJade = valRes["jade"].asInt();
		}

		history->m_pPlayerInfo->setName(m_pEditBox->getText());
		m_pDelegate->updateState();
		ITools::getInstance()->createGlobalPrompt(LL("renamesucceed"), true);

		//刷新标题栏金币勾玉数据
		if(history->m_layTitleBar)
		{
			history->m_layTitleBar->updateShow(0);
		}
		this->removeFromParent();
	}
	return true;
}
