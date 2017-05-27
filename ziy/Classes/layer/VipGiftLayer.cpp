//
//  VipGiftLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/13.
//
//

#include "VipGiftLayer.h"
#include "BuyJadeLayer.h"

VipGiftLayer* VipGiftLayer::create(EventDelegate* ptrDelegate)
{
	VipGiftLayer* pRet = new VipGiftLayer();
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

bool VipGiftLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;

	initControl();
	updateVipLayer();
	vipDateHttpRequest();
	return true;
}

bool VipGiftLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	return true;
}

void VipGiftLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void VipGiftLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void VipGiftLayer::updateMoneyShow()
{
	vipDateHttpRequest();
}

void VipGiftLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/img_global/vip_bg1.png");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	//会员特权
	Label* labInfo = LabelSystemFont(trData->m_mVipInfo[1]->strInfo.c_str(), 18);
	labInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
	labInfo->setPosition(point + Vec2(-168, 90));
	labInfo->setDimensions(340, 0);
	labInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	labInfo->setColor(Color3B::YELLOW);
	this->addChild(labInfo);

	//礼包内容
	map<int, int> mGoodsMap = trData->m_mVipInfo[1]->mapGifts;
	int iIndex = 0;
	for(map<int, int>::iterator it = mGoodsMap.begin(); it != mGoodsMap.end(); it++, iIndex++)
	{
		int iGoodsId = it->first;
		int iCount = it->second;

		HistoryGoods hisGoods(iGoodsId);
		Label* labGoods = LabelSystemFont(hisGoods.GetGoodsName(iCount), 18);
		labGoods->setAnchorPoint(Vec2(0.0f, 1.0f));
		labGoods->setPosition(point + Vec2(-168 + 105 * (iIndex % 3), 90 - labInfo->getContentSize().height - 12 - 22 * (iIndex / 3)));
		this->addChild(labGoods);
	}

	//更多特权
	Label* labNotice = LabelSystemFont(LL("vipnotice1"), 18);
	labNotice->setColor(Color3B(102, 51, 0));
	labNotice->setAnchorPoint(Vec2(0.0f, 1.0f));
	labNotice->setPosition(point + Vec2(-168, -27));
	this->addChild(labNotice);

	//时间&未开启
	m_labText = LabelSystemFont("", 16);
	m_labText->setColor(Color3B::YELLOW);
	this->addChild(m_labText);

	//日期
	m_labDate = LabelSystemFont("2016-00-00", 16);
	m_labDate->setAnchorPoint(Vec2(0.0f, 0.5f));
	this->addChild(m_labDate);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//购买
	Sprite* spBuyN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spBuyTextN = ReadSpriteName("new_ui/img_text/vip_txt1.png");
	spBuyTextN->setPosition(Vec2(spBuyN->getContentSize().width / 2, spBuyN->getContentSize().height / 2));
	spBuyN->addChild(spBuyTextN);
	Sprite* spBuyS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spBuyTextS = ReadSpriteName("new_ui/img_text/vip_txt2.png");
	spBuyTextS->setPosition(Vec2(spBuyS->getContentSize().width / 2, spBuyS->getContentSize().height / 2));
	spBuyS->addChild(spBuyTextS);
	m_mbtnBuy[0] = MenuItemSprite::create(spBuyN, spBuyS, CC_CALLBACK_1(VipGiftLayer::callBackBuy, this));
	m_mbtnBuy[0]->setPosition(point + Vec2(0, -127));
	ptrMenu->addChild(m_mbtnBuy[0]);

	//续费
	Sprite* spRenewN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spRenTextN = ReadSpriteName("new_ui/img_text/vip_txt3.png");
	spRenTextN->setPosition(Vec2(spRenewN->getContentSize().width / 2, spRenewN->getContentSize().height / 2));
	spRenewN->addChild(spRenTextN);
	Sprite* spRenewS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spRenTextS = ReadSpriteName("new_ui/img_text/vip_txt4.png");
	spRenTextS->setPosition(Vec2(spRenewS->getContentSize().width / 2, spRenewS->getContentSize().height / 2));
	spRenewS->addChild(spRenTextS);
	m_mbtnBuy[1] = MenuItemSprite::create(spRenewN, spRenewS, CC_CALLBACK_1(VipGiftLayer::callBackBuy, this));
	m_mbtnBuy[1]->setPosition(point + Vec2(0, -127+8));
	ptrMenu->addChild(m_mbtnBuy[1]);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(VipGiftLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(202, 152-15));
	ptrMenu->addChild(btnClose);
}

void VipGiftLayer::callBackBuy(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this));
}

void VipGiftLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void VipGiftLayer::vipDateHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("getCurVIPDate",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(VipGiftLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void VipGiftLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		onVipDateSucceed(value["result"]);
	}
}

bool VipGiftLayer::onVipDateSucceed(Json::Value valRes)
{
	if(!valRes["dueTime"].isNull())
	{
		double iTime1 = valRes["dueTime"].asDouble();

		time_t time;
		struct tm *pTime;
		time = iTime1 / 1000 + 3600 * 8;
		pTime = gmtime(&time);
		char chDate[80];
		strftime(chDate, 80, "%Y-%m-%d", pTime);
		m_labDate->setString(chDate);
		if(history->m_iVIP == 0) //首次开通刷新状态
		{
			history->m_iVIP = 1;
			history->m_iVIPGift = 1;
			updateVipLayer();

			//刷新标题栏金币勾玉数据
			if(history->m_layTitleBar)
			{
				history->m_layTitleBar->updateShow(0);
			}

			if(history->m_layHome)
			{
				history->m_layHome->updateState();
			}
		}
		m_pDelegate->updateState();
	}

	return true;
}

void VipGiftLayer::updateVipLayer()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	bool bVisible = history->m_iVIP ? true : false;
	m_mbtnBuy[0]->setVisible(!bVisible);
	m_mbtnBuy[1]->setVisible(bVisible);
	m_labDate->setVisible(bVisible);

	if (history->m_iVIP)
	{
		m_labText->setString(LL("vipendtime"));
		m_labText->setAnchorPoint(Vec2(0.0f, 0.5f));

		int iLab1 = m_labText->getContentSize().width;
		int iLab2 = m_labDate->getContentSize().width;
		int iOffsetX = (iLab1 + iLab2) / 2;
		m_labText->setPosition(point + Vec2(-iOffsetX, -68+5));
		m_labDate->setPosition(point + Vec2(-iOffsetX + iLab1, -68+5));
	}
	else
	{
		m_labText->setString(LL("vipnotice2"));
		m_labText->setAnchorPoint(Vec2(0.5f, 0.5f));
		m_labText->setPosition(point + Vec2(0, -68));
	}
}