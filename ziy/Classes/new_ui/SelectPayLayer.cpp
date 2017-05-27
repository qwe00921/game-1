#include "SelectPayLayer.h"
#include "Defines.h"
#include "StatConfig.h"
#include "PaymentTool.h"

SelectPayLayer* SelectPayLayer::create(int buyId, string chargeId, EventDelegate* ptrDelegate)
{
	SelectPayLayer* pRet = new SelectPayLayer();
	if (pRet&&pRet->init(buyId, chargeId,ptrDelegate))
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

bool SelectPayLayer::init(int buyId, string chargeId, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	mChargeId = chargeId;
	mBuyId = buyId;
	mDelegate = ptrDelegate;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* bg = LayerColor::create(Color4B(255, 255, 255, 255));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	Sprite* bgHT = ReadSpriteName("new_ui/pay/pay_hongtiao.png");
	bgHT->setPosition(Vec2(size.width / 2, size.height*7/8));
	this->addChild(bgHT);

	Sprite* bgHUIT = ReadSpriteName("new_ui/pay/pay_huitiao.png");
	bgHUIT->setPosition(Vec2(size.width / 2, size.height*4 / 9));
	this->addChild(bgHUIT);

	int point_x = size.width / 2 - bgHT->getContentSize().width / 2 + 20;
	int point_y = size.height * 7 / 8 - bgHT->getContentSize().height-20;
	order = LabelSystemFont("ORDER : 123456789",17);
	order->setPosition(Vec2(point_x, point_y));
	order->setColor(Color3B::BLACK);
	order->setAnchorPoint(Vec2(0, 1));
	this->addChild(order);

	point_y -= order->getContentSize().height + 5;
	price = LabelSystemFont("PRICE : 0.1", 17);
	price->setPosition(Vec2(point_x, point_y));
	price->setColor(Color3B::BLACK);
	price->setAnchorPoint(Vec2(0, 1));
	this->addChild(price);

	Sprite* zfb_n = ReadSpriteName("new_ui/pay/pay_zhifubao.png");
	Sprite* zfb_s = ReadSpriteName("new_ui/pay/pay_zhifubao.png");
	zfb_s->setScale(0.95f);
	MenuItemSprite* zfbS = MenuItemSprite::create(zfb_n, zfb_s, CC_CALLBACK_1(SelectPayLayer::clickZFBMenu, this));

	point_x += zfb_n->getContentSize().width/2;
	point_y = size.height * 4 / 9 - bgHUIT->getContentSize().height - 10 - zfb_n->getContentSize().height / 2;
	zfb_menu = Menu::create();
	zfb_menu->setPosition(Vec2(point_x, point_y));
	zfb_menu->addChild(zfbS);
	zfb_s->setAnchorPoint(Vec2(0.5, 0.5));
	zfb_s->setPosition(Vec2(zfb_s->getContentSize().width / 2, zfb_s->getContentSize().height/2));
	this->addChild(zfb_menu);

	Sprite* wx_n = ReadSpriteName("new_ui/pay/pay_wechat.png");
	Sprite* wx_s = ReadSpriteName("new_ui/pay/pay_wechat.png");
	wx_s->setScale(0.95f);
	MenuItemSprite* wxS = MenuItemSprite::create(wx_n, wx_s, CC_CALLBACK_1(SelectPayLayer::clickWXMenu, this));
	point_x += zfb_n->getContentSize().width + 20;
	wx_menu = Menu::create();
	wx_menu->setPosition(Vec2(point_x, point_y));
	wx_menu->addChild(wxS);
	wx_s->setAnchorPoint(Vec2(0.5, 0.5));
	wx_s->setPosition(Vec2(wx_s->getContentSize().width / 2, wx_s->getContentSize().height / 2));
	this->addChild(wx_menu);

	close_M = Menu::create();
	close_M->setPosition(Vec2(bgHT->getContentSize().width - 25, bgHT->getContentSize().height / 2));
	bgHT->addChild(close_M);

	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_MIt = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(SelectPayLayer::ClickCloseMenu, this));
	close_M->addChild(close_MIt);

	UpdateDate();
	return true;
}

void SelectPayLayer::UpdateDate()
{
	TrPayInfo::TrChargeItem* mChargeItem = trData->trGamePay->getPayInfo()->findChargeItem(mChargeId.c_str());
	order->setString(__String::createWithFormat(LL("payGood"), mChargeItem->subject->getChar())->getCString());
	price->setString(__String::createWithFormat(LL("payPrice"), mChargeItem->price / 100)->getCString());

	if (mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_ALIPAY) == NULL)
	{
		zfb_menu->setVisible(false);
	}

	if(mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_ALIPAY) == NULL && mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_WXPAY) == NULL)
	{
		wx_menu->setVisible(false);
	}
}

void SelectPayLayer::clickZFBMenu(Ref* sender)
{
	if (mDelegate)
		mDelegate->SelectPayRequest(PAY_TYPE_ALIPAY);
	this->removeFromParent();
}
void SelectPayLayer::clickWXMenu(Ref* sender)
{
	if (mDelegate)
		mDelegate->SelectPayRequest(PAY_TYPE_WXPAY);
	this->removeFromParent();
}
void SelectPayLayer::ClickCloseMenu(Ref* sender)
{
	if (mDelegate)
		mDelegate->ChildCloseLayer(0);
	this->removeFromParent();
}
bool SelectPayLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}
SelectPayLayer::SelectPayLayer()
{

}
SelectPayLayer::~SelectPayLayer()
{

}
void SelectPayLayer::onExit()
{
	Layer::onExit();
}
void SelectPayLayer::onEnter()
{
	Layer::onEnter();
}
