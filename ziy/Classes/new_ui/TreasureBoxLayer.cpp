#include "TreasureBoxLayer.h"
#include "Defines.h"
#include "GoodsSprite.h"
#include "TreasureBoxTableView.h"

TreasureBoxLayer* TreasureBoxLayer::create(int iMapId, EventDelegate* ptrDelegate,int iBattleType)
{
	TreasureBoxLayer* pRet = new TreasureBoxLayer();
	pRet->mMapId = iMapId;
	if (pRet&&pRet->init(ptrDelegate,iBattleType))
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

bool TreasureBoxLayer::init(EventDelegate* ptrDelegate, int iBattleType)
{
	if (!Layer::init())
	{
		return false;
	}
	mPromptTxtIndex = 0;
	mDelegate = ptrDelegate;
	this->mBattleType = iBattleType;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	bg = ReadSpriteName("new_ui/battle/battle_sbg2.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	bg->addChild(TreasureBoxTableView::create(mMapId,this,iBattleType));

	c_menu = Menu::create();
	c_menu->setPosition(Vec2(496, 369-12));
	bg->addChild(c_menu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close2.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press2.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(TreasureBoxLayer::clickCloseLayer, this));
	c_menu->addChild(close_menu);

	return true;
}

bool TreasureBoxLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void TreasureBoxLayer::onTouchMoved(Touch *pTouches, Event *pEvent)
{

}
void TreasureBoxLayer::onTouchEnded(Touch *pTouches, Event *pEvent)
{

}

void TreasureBoxLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void TreasureBoxLayer::onExit()
{
	Layer::onExit();
}

void TreasureBoxLayer::onEnter()
{
	Layer::onEnter();
}

TreasureBoxLayer::TreasureBoxLayer()
{

}

TreasureBoxLayer::~TreasureBoxLayer()
{

}

void TreasureBoxLayer::updateRTDate(map<int, string> mString)
{
	mPromptTxt.clear();
	mPromptTxt = mString;
	mPromptTxtIndex = 0;
	this->schedule(schedule_selector(TreasureBoxLayer::showPrompt), 0.8f, mPromptTxt.size() - 1, 0);
	if (mDelegate)
		mDelegate->updateMoneyShow();
}

void TreasureBoxLayer::showPrompt(float f)
{
	ITools::getInstance()->createGlobalPrompt(mPromptTxt[mPromptTxtIndex++], true);
}