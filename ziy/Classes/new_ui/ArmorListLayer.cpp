#include "ArmorListLayer.h"
#include "Defines.h"
#include "ArmorListTableView.h"
#include "GuideLayer.h"
#include "Net_HomePage.h"

bool ArmorListLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/img_global/global_bg.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	Sprite* bg_info = ReadSpriteName("new_ui/img_text/bt1_1.png");
	bg_info->setPosition(Vec2(bg->getContentSize().width / 2-40, 364+8));
	bg->addChild(bg_info);

	Sprite* hero_bg = ReadSpriteName("new_ui/img_global/hero_bg1.png");
	hero_bg->setPosition(Vec2(28,24));
	hero_bg->setAnchorPoint(Vec2(0, 0));
	bg->addChild(hero_bg);

	Sprite* wj_num_bg = ReadSpriteName("new_ui/img_global/hero_bg4.png");
	wj_num_bg->setPosition(Vec2(89,336));
	bg->addChild(wj_num_bg);

	Label* wjs_txt = LabelSystemFont(LL("wujiangshu"),16);
	wjs_txt->setAnchorPoint(Vec2(0, 0.5));
	wjs_txt->setPosition(Vec2(18,wj_num_bg->getContentSize().height/2));
	wj_num_bg->addChild(wjs_txt);

	wjs_num = LabelSystemFont(IntToString(history->ownArmyNum), 16);
	wjs_num->setAnchorPoint(Vec2(0, 0.5));
	wjs_num->setColor(Color3B::YELLOW);
	wjs_num->setPosition(Vec2(18 + wjs_txt->getContentSize().width, wj_num_bg->getContentSize().height / 2));
	wj_num_bg->addChild(wjs_num);

	bg->addChild(ArmorListTableView::create(this));


	//关闭
	mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(600, 358+14));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ArmorListLayer::clickCloseLayer, this));
	mCloseMenu->addChild(close_menu);
	history->updateGuide(EMIL_GUIDE);
	HongDian::getInstence()->autoUpdateWuJiang(WUJIANG_JIEMIAN);
	HongDian::getInstence()->HongDianNodeisVisibol[1001] = 1;

	for (auto offset: bg->getChildren())
	{
		offset->setPosition(Vec2(offset->getPositionX()+40,offset->getPositionY()+8));
	}

	return true;
}

bool ArmorListLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void ArmorListLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void ArmorListLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	CCLOG("Layer touch Ended");
}

void ArmorListLayer::RunFunction(int iNomal)
{
	wjs_num->setString(IntToString(history->ownArmyNum));
	updateMoneyShow();
}

void ArmorListLayer::updateMoneyShow()
{
	history->m_layTitleBar->updateShow(0);
}

void ArmorListLayer::RemoveAllParentLayer()
{
	this->removeFromParent();
}

void ArmorListLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->updateGuide(EMIL_GUIDE);
	this->removeFromParent();
	HongDian::getInstence()->autoUpdate();
}

ArmorListLayer::ArmorListLayer()
{

}
ArmorListLayer::~ArmorListLayer()
{

}
void ArmorListLayer::onExit()
{
	Layer::onExit();
}

void ArmorListLayer::onEnter()
{
	Layer::onEnter();
}