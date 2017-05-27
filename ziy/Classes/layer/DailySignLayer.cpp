//
//  DailySignLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/09/06.
//
//

#include "DailySignLayer.h"
#include "ActivityInfo.h"
#include "PopLayer.h"
#include "Net_HomePage.h"

enum
{
	SIGN_INIT = 0, //获取数据
	SIGN_DAILY = 1, //日常签到
	SIGN_SUPPLE = 2 //补签
};

#define RESIGN_PRICE 50

DailySignLayer::DailySignLayer()
{

}

DailySignLayer::~DailySignLayer()
{

}

bool DailySignLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_spSelect = nullptr;
	m_iNoticeIndex = 0;

	initControl();
	if(BTActivity::getInstance()->isSignInit())
	{
		initGoodsItem();
	}
	else
	{
		onHttpRequest(SIGN_INIT);
	}

	return true;
}

bool DailySignLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void DailySignLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void DailySignLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void DailySignLayer::callBackConfirm()
{
	if(history->m_iJade < RESIGN_PRICE)
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
		return;
	}

	onHttpRequest(SIGN_SUPPLE);
}

void DailySignLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spLayBG = ReadSpriteName("new_ui/activity/activity_bg5.png");
	spLayBG->setPosition(point);
	this->addChild(spLayBG);

	//标题
	m_labTitle = LabelSystemFont("", 18);
	m_labTitle->setPosition(point + Vec2(0, 118));
	m_labTitle->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(m_labTitle);

	//补签次数
	m_labResign = LabelSystemFont("", 16);
	m_labResign->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labResign->setPosition(point + Vec2(190, 118));
	m_labResign->setColor(Color3B(102, 50, 0));
	this->addChild(m_labResign);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//补签
	Sprite* spResignN = ReadSpriteName("new_ui/img_global/global_ban11.png");
	Sprite* spResTextN = ReadSpriteName("new_ui/img_text/imgtxt6_5.png");
	spResTextN->setPosition(Vec2(spResignN->getContentSize().width / 2, spResignN->getContentSize().height / 2));
	spResignN->addChild(spResTextN);
	Sprite* spResignS = ReadSpriteName("new_ui/img_global/global_ban12.png");
	Sprite* spResTextS = ReadSpriteName("new_ui/img_text/imgtxt6_6.png");
	spResTextS->setPosition(Vec2(spResignS->getContentSize().width / 2, spResignS->getContentSize().height / 2));
	spResignS->addChild(spResTextS);
	m_btnResign = MenuItemSprite::create(spResignN, spResignS, CC_CALLBACK_1(DailySignLayer::callBackResign, this));
	m_btnResign->setPosition(point + Vec2(230, 118));
	m_btnResign->setVisible(false);
	ptrMenu->addChild(m_btnResign);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(DailySignLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(297, 171));
	ptrMenu->addChild(btnClose);
}

void DailySignLayer::initGoodsItem()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	updateShow();
	//物品表格
	for(int i = 0; i < (int)BTActivity::getInstance()->m_mSignGoods.size(); i++)
	{
		SignGoods* ptrGoods = BTActivity::getInstance()->m_mSignGoods[i];

		//物品ICON
		auto spGoods = GoodsSprite::create(ptrGoods->getGoodsId(), ptrGoods->getCount());
		spGoods->setPosition(point + Vec2(-230 + 66 * (i % 8), 70 - 60 * (i / 8)));
		this->addChild(spGoods);

		//王族双倍
		if(ptrGoods->getIsDouble())
		{
			auto spDouble = ReadSpriteName("new_ui/activity/activity_xz2.png");
			spDouble->setPosition(Vec2(25, 43));
			spGoods->addChild(spDouble);
		}

		//已签到
		if(i < BTActivity::getInstance()->getTotalTimes())
		{
			auto spSigned = ReadSpriteName("new_ui/activity/activity_xz1.png");
			spSigned->setPosition(Vec2(25, 25));
			spGoods->addChild(spSigned);
		}
		else if(i == BTActivity::getInstance()->getTotalTimes() && BTActivity::getInstance()->getIsSign()) //当天未签到
		{
			//选中框
			m_spSelect = ReadSpriteName("new_ui/img_global/selected_box1.png");
			m_spSelect->setPosition(Vec2(25, 25));
			spGoods->addChild(m_spSelect);

			//设置单击委托
			spGoods->setCallback(CC_CALLBACK_1(DailySignLayer::callBackSign, this));
		}

		m_mGoodsItem[i] = spGoods;
	}
}

void DailySignLayer::updateShow()
{
	//标题
	string strTitle = StringUtils::format(LL("signtotaltimes"), BTActivity::getInstance()->getSignMonth(), BTActivity::getInstance()->getTotalTimes());
	m_labTitle->setString(strTitle);

	//补签
	if(!BTActivity::getInstance()->getIsSign() && BTActivity::getInstance()->getMissTimes())
	{
		string strResign = StringUtils::format(LL("resigntimes"), BTActivity::getInstance()->getMissTimes());
		m_labResign->setString(strResign);
		m_labResign->setVisible(true);
		m_btnResign->setVisible(true);
	}
	else
	{
		m_labResign->setVisible(false);
		m_btnResign->setVisible(false);
	}
}

void DailySignLayer::updateItem()
{
	int iIndex = BTActivity::getInstance()->getTotalTimes() - 1;

	//选中框
	if(m_spSelect)
	{
		m_mGoodsItem[iIndex]->setCallback(nullptr);
		m_spSelect->removeFromParent();
		m_spSelect = nullptr;
	}

	//已签到
	auto spSigned = ReadSpriteName("new_ui/activity/activity_xz1.png");
	spSigned->setPosition(Vec2(25, 25));
	m_mGoodsItem[iIndex]->addChild(spSigned);
}

void DailySignLayer::playGoodsNotice(float fDelay)
{
	if(m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->unschedule(schedule_selector(DailySignLayer::playGoodsNotice));
	}
}

void DailySignLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case SIGN_INIT:
	{
		strURL = "getSignData";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SIGN_INIT";
	}
	break;
	case SIGN_DAILY:
	{
		strURL = "sign";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SIGN_DAILY";
	}
	break;
	case SIGN_SUPPLE:
	{
		strURL = "repairSign";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SIGN_SUPPLE";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(DailySignLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void DailySignLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if(strTag == "SIGN_INIT")
		{
			BTActivity::getInstance()->loadSignJson(value["result"]);
			initGoodsItem();
		}
		else
		{
			if (strTag == "SIGN_DAILY")
			{
				if(value["result"]["isSign"].isInt())
				{
					BTActivity::getInstance()->setIsSign(value["result"]["isSign"].asInt());
				}
			}
			else if (strTag == "SIGN_SUPPLE")
			{
				history->m_iJade -= RESIGN_PRICE;
				if(value["result"]["repairSignCount"].isInt())
				{
					BTActivity::getInstance()->setMissTimes(value["result"]["repairSignCount"].asInt());
				}
			}

			m_iNoticeIndex = 0;
			m_strNotice = history->onHttpGoodsAdd(value["result"]);
			this->schedule(schedule_selector(DailySignLayer::playGoodsNotice), 0.8f);

			BTActivity::getInstance()->setTotalTimes(BTActivity::getInstance()->getTotalTimes() + 1);
			updateShow();
			updateItem();

			//刷新标题栏金币勾玉数据
			if(history->m_layTitleBar)
			{
				history->m_layTitleBar->updateShow(0);
			}
		}
		HongDian::getInstence()->autoUpdate();
	}
}

void DailySignLayer::callBackSign(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	onHttpRequest(SIGN_DAILY);
}

void DailySignLayer::callBackResign(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_RESIGN, this, RESIGN_PRICE));
}

void DailySignLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
	HongDian::getInstence()->autoUpdate();
}
