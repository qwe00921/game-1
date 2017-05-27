//
//  PayRestoreLayer.cpp
//  TRGame
//
//  Created by Jimmy on 17/02/15.
//
//

#include "PayRestoreLayer.h"
#include "LogoLayer.h"
#include "NetPromptTool.h"

PayRestoreLayer::PayRestoreLayer()
{

}

PayRestoreLayer::~PayRestoreLayer()
{

}

PayRestoreLayer* PayRestoreLayer::create(string strTitle, string strContent)
{
	PayRestoreLayer* pRet = new PayRestoreLayer();
	if(pRet&&pRet->init(strTitle, strContent))
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

bool PayRestoreLayer::init(string strTitle, string strContent)
{
	if(!Layer::init())
	{
		return false;
	}

	m_strTitle = strTitle;
	m_strContent = strContent;

	initControl();
	return true;
}

void PayRestoreLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	auto spBG = ReadSpriteName("new_ui/img_global/cz_ui1.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	auto labTitle = LabelSystemFont(m_strTitle, 18);
	labTitle->setPosition(point + Vec2(0, 125));
	labTitle->setColor(Color3B::YELLOW);
	this->addChild(labTitle);
	
	//礼包内容
	if (m_strContent != "")
	{
		Label* labContent = LabelSystemFont(m_strContent, 16);
		labContent->setAnchorPoint(Vec2(0.5f, 1.0f));
		labContent->setDimensions(292, 0);
		labContent->setHorizontalAlignment(TextHAlignment::LEFT);
		labContent->setPosition(point + Vec2(0, 100));
		this->addChild(labContent);
	}

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//确定
	Sprite* spConfirmN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spConTextN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	spConfirmN->addChild(spConTextN);
	spConTextN->setPosition(Vec2(spConfirmN->getContentSize().width / 2, spConfirmN->getContentSize().height / 2));
	Sprite* spConfirmS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spConTextS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	spConfirmS->addChild(spConTextS);
	spConTextS->setPosition(Vec2(spConfirmS->getContentSize().width / 2, spConfirmS->getContentSize().height / 2));
	MenuItemSprite* btnConfirm = MenuItemSprite::create(spConfirmN, spConfirmS, CC_CALLBACK_1(PayRestoreLayer::callBackConfirm, this));
	btnConfirm->setPosition(point + Vec2(0, -142));
	ptrMenu->addChild(btnConfirm);
}

void PayRestoreLayer::callBackConfirm(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessViewTool::getInstance()->setScreenDefaultSize();
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	NetPromptTool::getInstance()->StopGetNetPrompt();
	ChessApplication::getInstance()->AddFunctionLayer(LogoLayer::create(true));
}