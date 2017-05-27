#include "RomanceLayer.h"
#include "Defines.h"
#include "RomanceScrollview.h"
#include "Net_HomePage.h"
#include "GuideLayer.h"

RomanceLayer* RomanceLayer::create(bool bEnablePlayMp3 /*= true*/)
{
	RomanceLayer* pRet = new RomanceLayer();
	if (pRet&&pRet->init(bEnablePlayMp3))
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

bool RomanceLayer::init(bool bEnablePlayMp3)
{
	if (!Layer::init())
	{
		return false;
	}
	if (bEnablePlayMp3)
		ChessPlayMp3(MUSIC_ROMANCE);
	DSize size = ChessViewTool::getInstance()->getFullSize();

	Sprite* bg = ReadSpriteNormal("img/script/imgs_bg1.jpg");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	this->addChild(RomanceScrollview::create(this));

	//返回
	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(RomanceLayer::clickMenuBack, this));

	Menu* mBack = Menu::create();
	mBack->setPosition(Vec2(size.width - (mb_nomal->getContentSize().width / 2 + 8), size.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mBack, 1);
	mBack->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5, 0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width / 2, mb_select->getContentSize().height / 2));

	mTitleBar = TitleBarLayer::create(TITLE_SHOW_GY_YX);
	this->addChild(mTitleBar,1);
	history->updateGuide(EMIL_GUIDE);
	return true;
}

void RomanceLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
	this->removeFromParent();
}

void RomanceLayer::ChildCloseLayer(int iNomal)
{
	this->removeFromParent();
}

RomanceLayer::RomanceLayer() :BTLayer(false)
{

}

RomanceLayer::~RomanceLayer()
{

}