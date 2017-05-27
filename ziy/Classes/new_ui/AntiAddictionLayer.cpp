#include "AntiAddictionLayer.h"
#include "Defines.h"
#include "auto_update/AutoUpdateScene.hpp"

bool AntiAddictionLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 255));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	Sprite* text = ReadSpriteName("new_ui/main/gdj100.jpg");
	text->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(text);

	FadeIn* FadeIn = FadeIn::create(1.5f);
	DelayTime* ptrDelayTime = DelayTime::create(2);
	FadeOut* FadeOut = FadeOut::create(0.5f);

	Sequence* ptrSequence = Sequence::create(FadeIn, ptrDelayTime, FadeOut,NULL);
	text->runAction(ptrSequence);

	this->scheduleOnce(schedule_selector(AntiAddictionLayer::ShowLogoLayer),4);
	return true;
}

void AntiAddictionLayer::onExit()
{
	Layer::onExit();
}

AntiAddictionLayer::AntiAddictionLayer()
{

}

AntiAddictionLayer::~AntiAddictionLayer()
{

}

void AntiAddictionLayer::ShowLogoLayer(float f)
{
	ChessApplication::getInstance()->AddFunctionLayer(AutoUpdateScene::create());
	this->removeFromParent();
}

bool AntiAddictionLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}