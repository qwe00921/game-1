#include "WolfCityLayer.h"
#include "Defines.h"
#include "WolfCityScrollview.h"
#include "Net_HomePage.h"

WolfCityLayer* WolfCityLayer::create()
{
	WolfCityLayer* pRet = new WolfCityLayer();
	if (pRet&&pRet->init())
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

bool WolfCityLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	//背景
	Sprite* bg = ReadSpriteName("new_ui/glc_war/glc_bg1.jpg");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	//提示
	Sprite* timeSp = ReadSpriteName("new_ui/img_global/global_ph_txt3.png");
	timeSp->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height/2+180));
	bg->addChild(timeSp);

	Sprite* chestBoxBg = ReadSpriteName("new_ui/glc_war/glc_bg3.png");
	chestBoxBg->setPosition(Vec2(size.width / 2, chestBoxBg->getContentSize().height / 2));
	this->addChild(chestBoxBg);

	this->addChild(WolfCityScrollview::create(this));

	Sprite* titlebgSp = ReadSpriteName("new_ui/glc_war/glc_bg2.png");
	titlebgSp->setPosition(Vec2(0, size.height - 2));
	titlebgSp->setAnchorPoint(Vec2(0, 1));
	this->addChild(titlebgSp, 1);

	Sprite* wolfTitleSp = ReadSpriteName("new_ui/glc_war/glc_txt1.png");
	wolfTitleSp->setPosition(Vec2(12, titlebgSp->getContentSize().height/2+4));
	wolfTitleSp->setAnchorPoint(Vec2(0,0.5));
	titlebgSp->addChild(wolfTitleSp);


	//返回
	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(WolfCityLayer::clickMenuBack, this));

	Menu* mBack = Menu::create();
	mBack->setPosition(Vec2(size.width - 32, size.height - 32));
	this->addChild(mBack, 1);
	mBack->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5, 0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width / 2, mb_select->getContentSize().height / 2));

	return true;
}

void WolfCityLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (history->m_layTitleBar)
		history->m_layTitleBar->updateState();
	this->removeFromParent();
}

void WolfCityLayer::ChildCloseLayer(int iNomal)
{
	this->removeFromParent();
}

WolfCityLayer::WolfCityLayer() :BTLayer(false)
{

}

WolfCityLayer::~WolfCityLayer()
{

}