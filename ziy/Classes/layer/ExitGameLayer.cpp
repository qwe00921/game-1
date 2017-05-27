//
//  ExitGameLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/18.
//
//

#include "ExitGameLayer.h"

ExitGameLayer* ExitGameLayer::m_pInstance = NULL;

ExitGameLayer::ExitGameLayer()
{

}

ExitGameLayer::~ExitGameLayer()
{

}

ExitGameLayer* ExitGameLayer::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = ExitGameLayer::create();
	}
	return m_pInstance;
}

bool ExitGameLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();
	return true;
}

void ExitGameLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩层
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(layBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/img_global/pop_bg1.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//中华文化
	Sprite* spMark = ReadSpriteName("new_ui/img_global/icon_zhwh.png");
	spMark->setPosition(Vec2(78, 132));
	spBG->addChild(spMark);

	//标题
	Label* labTitle = LabelSystemFont(LL("keyexitgame"), 18);
	labTitle->setPosition(Vec2(spBG->getContentSize().width / 2, 132));
	spBG->addChild(labTitle);

	//取消
	Menu* menuCancel = Menu::create();
	menuCancel->setPosition(Vec2(spBG->getContentSize().width / 2 - 70, 47));
	Sprite* spCancelN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spCanTextN = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	spCanTextN->setPosition(Vec2(40, 18));
	spCancelN->addChild(spCanTextN);
	Sprite* spCancelS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spCanTextS = ReadSpriteName("new_ui/img_text/imgtxt4_4.png");
	spCanTextS->setPosition(Vec2(40, 18));
	spCancelS->addChild(spCanTextS);
	MenuItemSprite* btnCancel = MenuItemSprite::create(spCancelN, spCancelS, CC_CALLBACK_1(ExitGameLayer::callBackCancel, this));
	menuCancel->addChild(btnCancel);
	spBG->addChild(menuCancel);

	//确定
	Menu* menuConfirm = Menu::create();
	menuConfirm->setPosition(Vec2(spBG->getContentSize().width / 2 + 70, 47));
	Sprite* spConfirmN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spConTextN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	spConTextN->setPosition(Vec2(40, 18));
	spConfirmN->addChild(spConTextN);
	Sprite* spConfirmS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spConTextS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	spConTextS->setPosition(Vec2(40, 18));
	spConfirmS->addChild(spConTextS);
	MenuItemSprite* btnConfirm = MenuItemSprite::create(spConfirmN, spConfirmS, CC_CALLBACK_1(ExitGameLayer::callBackConfirm, this));
	menuConfirm->addChild(btnConfirm);
	spBG->addChild(menuConfirm);
}

void ExitGameLayer::callBackCancel(Ref* node)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_pInstance = NULL;
	this->removeFromParent();
}

void ExitGameLayer::callBackConfirm(Ref* node)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessExit();
}