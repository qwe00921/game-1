#include "PicturePopLayer.h"
#include "Defines.h"

PicturePopLayer* PicturePopLayer::create(string BgPathS, bool bIsGlobal)
{
	PicturePopLayer* pRet = new PicturePopLayer();
	if (pRet&&pRet->init(BgPathS, bIsGlobal))
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

bool PicturePopLayer::init(string BgPathS, bool bIsGlobal)
{
	if (!Layer::init())
	{
		return false;
	}
	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);

	Sprite* bg = NULL;
	if (bIsGlobal)
		bg = ReadSpriteName(BgPathS.c_str());
	else
		bg = ReadSpriteNormal(BgPathS.c_str());

	bg->setPosition(Vec2(size.width/2,size.height/2));

	this->addChild(bg);

	return true;
}
bool PicturePopLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}
void PicturePopLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	this->removeFromParent();
}