//
//  GoodsStoreLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/11/04.
//
//

#include "GoodsStoreLayer.h"
#include "GoodsDealLayer.h"
#include "BuyJadeLayer.h"
#include "ActivityInfo.h"

enum 
{
	STORE_XHP = 0, //消耗品
	STORE_CBG = 1 //藏宝阁
};


enum
{
	SHOP_STROE_INIT = 0,
	SHOP_STROE_BUY = 1
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StoreItem::StoreItem()
{
	m_iGoodsId = 0;
	m_iCount = 0;
	m_iType = 0;
	m_iPrice = 0;
	m_iDiscount = 0;
	m_strName = "";
	m_strStart = "";
	m_strEnd = "";
}

StoreItem::~StoreItem()
{

}

bool StoreItem::loadJson(Json::Value valJson)
{
	ParseInt(m_iGoodsId, valJson["goodID"]);
	ParseInt(m_iCount, valJson["count"]);
	ParseInt(m_iType, valJson["payType"]);
	ParseInt(m_iPrice, valJson["price"]);
	ParseInt(m_iDiscount, valJson["discount"]);
	ParseString(m_strName, valJson["name"]);
	ParseString(m_strStart, valJson["startTime"]);
	ParseString(m_strEnd, valJson["endTime"]);

	if (m_iType == 1)
	{
		m_iType = PAY_TYPE_JADE;
	}
	else if (m_iType == 2)
	{
		m_iType = PAY_TYPE_LONG;
	}

	return true;
}

string StoreItem::getIconPath()
{
	string strPath = "";
	switch(m_iType)
	{
	case PAY_TYPE_JADE:
		strPath = "new_ui/img_global/icon_gy.png";
		break;
	case PAY_TYPE_LONG:
		strPath = "new_ui/img_global/icon_gld.png";
		break;
	default:
		break;
	}
	return strPath;
}

int StoreItem::getIntTime(string strTime)
{
	string strTemp = "";
	map<int, string> mstrTime = trData->getStringMap(strTime, '-');
	for(UInt i = 0; i < mstrTime.size(); i++)
	{
		strTemp += mstrTime[i];
	}
	return StringToInt(strTemp.c_str());
}

void StoreItem::calcuDiscount(int iCurTime)
{
	if(m_iDiscount)
	{
		int iStartTime = getIntTime(m_strStart);
		int iEndTime = getIntTime(m_strEnd);
		if(iCurTime < iStartTime || iCurTime > iEndTime)
		{
			m_iDiscount = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StoreInfo::StoreInfo()
{
	m_dCurTime = 0.0;
	m_strContent = "";
}

StoreInfo::~StoreInfo()
{
	clearJson();
}

bool StoreInfo::loadJson(Json::Value valJson)
{
	clearJson();

	if(valJson["time"].isDouble())
	{
		m_dCurTime = valJson["time"].asDouble();
	}
	ParseString(m_strContent, valJson["explain"]);

	//时间戳转为整形时间
	time_t time;
	struct tm *pTime;
	time = m_dCurTime / 1000 + 3600 * 8;
	pTime = gmtime(&time);
	char chDate[80];
	strftime(chDate, 80, "%Y%m%d", pTime);
	int iCurTime = StringToInt(chDate);

	Json::Value valGoods = valJson["goodsMan"];
	for(Json::Value::iterator itGoods = valGoods.begin(); itGoods != valGoods.end(); itGoods++)
	{
		string strKeyId = itGoods.key().asString();
		Json::Value valStore = valGoods[strKeyId.c_str()];
		map<int, StoreItem*> mStoreItem;
		for(Json::Value::iterator it = valStore.begin(); it != valStore.end(); it++)
		{
			string strItemId = it.key().asString();
			int iItemId = StringToInt(strItemId.c_str());
			Json::Value valItem = valStore[strItemId.c_str()];
			StoreItem* ptrStore = new StoreItem();
			ptrStore->loadJson(valItem);
			ptrStore->calcuDiscount(iCurTime);
			mStoreItem[iItemId] = ptrStore;
		}
		m_mStoreItem[strKeyId] = mStoreItem;
	}
	return true;
}

void StoreInfo::clearJson()
{
	m_dCurTime = 0.0;
	m_strContent = "";

	for (map<string, map<int, StoreItem*> >::iterator it = m_mStoreItem.begin(); it != m_mStoreItem.end(); it++)
	{
		map<int, StoreItem*> mStoreItem = it->second;
		for(map<int, StoreItem*>::iterator itStore = mStoreItem.begin(); itStore != mStoreItem.end(); itStore++)
		{
			SAFE_DELETE(itStore->second);
		}
		mStoreItem.clear();
	}
	m_mStoreItem.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GoodsStoreLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_iCurIndex = STORE_XHP;

	initControl();
	onHttpRequest(SHOP_STROE_INIT);

	return true;
}

GoodsStoreLayer::GoodsStoreLayer()
{
	m_ptrStore = NULL;
}

GoodsStoreLayer::~GoodsStoreLayer()
{
	SAFE_DELETE(m_ptrStore);
	for(UInt i = 0; i < m_mSellGoods.size(); i++)
	{
		for (UInt j = 0; j < m_mSellGoods[i].size(); j++)
		{
			SAFE_DELETE(m_mSellGoods[i][j]);
		}
		m_mSellGoods[i].clear();
	}
	m_mSellGoods.clear();
}

void GoodsStoreLayer::updateShow(int index)
{
	m_labJade->setString(StringUtils::format("%d", history->m_iJade));
	m_labLong->setString(StringUtils::format("%d", history->m_iJadeSeal));

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

void GoodsStoreLayer::callBackListClick(int iIndex)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->addChild(GoodsDealLayer::create(m_mSellGoods[m_iCurIndex][iIndex], this));
}

void GoodsStoreLayer::updateMoneyShow()
{
	updateShow(0);
}

void GoodsStoreLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* spBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	spBlack->setPosition(Vec2(0, 0));
	this->addChild(spBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/shop/shop_bg10.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	Sprite* spTitle = ReadSpriteName("new_ui/img_text/shop_txt3.png");
	spTitle->setPosition(Vec2(305, 369));
	spBG->addChild(spTitle);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//魂玉充值
	Sprite* spJadeN = ReadSpriteName("new_ui/main/main_bg2.png");
	Sprite* spJadeS = ReadSpriteName("new_ui/main/main_bg2.png");
	m_btnJade = MenuItemSprite::create(spJadeN, spJadeS, CC_CALLBACK_1(GoodsStoreLayer::callBackJade, this));
	m_btnJade->setPosition(point + Vec2(-200, 116));
	ptrMenu->addChild(m_btnJade);

	//魂玉
	m_labJade = LabelSystemFont(StringUtils::format("%d", history->m_iJade), 16);
	m_labJade->setColor(Color3B::GREEN);
	m_labJade->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labJade->setPosition(Vec2(98, 16));
	m_btnJade->addChild(m_labJade);

	Sprite* spJadeIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJadeIcon->setPosition(Vec2(115, 16));
	m_btnJade->addChild(spJadeIcon);
	m_btnJade->setVisible(false);

	//骨龙币背景
	m_spLongBG = ReadSpriteName("new_ui/shop/shop_bg3.png");
	m_spLongBG->setPosition(point + Vec2(-192, 113));
	//m_spLongBG->setVisible(false);
	this->addChild(m_spLongBG);

	//骨龙币ICON
	Sprite* spLong = ReadSpriteName("new_ui/img_global/icon_gld.png");
	spLong->setPosition(Vec2(15, 15));
	m_spLongBG->addChild(spLong);

	//骨龙币库存
	m_labLong = LabelSystemFont(StringUtils::format("%d", history->m_iJadeSeal), 16);
	m_labLong->setColor(Color3B::YELLOW);
	m_labLong->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labLong->setPosition(Vec2(40, 15));
	m_spLongBG->addChild(m_labLong);

	//提示
	m_labNotice = LabelSystemFont("", 18);
	m_labNotice->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labNotice->setColor(Color3B(153, 51, 0));
	m_labNotice->setPosition(point + Vec2(250, 113));
	this->addChild(m_labNotice);

	//消耗品
	Sprite* spXHPN = ReadSpriteName("new_ui/img_global/global_ban15.png");
	Sprite* spXHPTextN = ReadSpriteName("new_ui/shop/shop_ban7.png");
	spXHPTextN->setPosition(Vec2(spXHPN->getContentSize().width / 2, spXHPN->getContentSize().height / 2));
	spXHPN->addChild(spXHPTextN);
	Sprite* spXHPS = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spXHPTextS = ReadSpriteName("new_ui/shop/shop_ban8.png");
	spXHPTextS->setPosition(Vec2(spXHPS->getContentSize().width / 2, spXHPS->getContentSize().height / 2));
	spXHPS->addChild(spXHPTextS);
	Sprite* spXHPD = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spXHPTextD = ReadSpriteName("new_ui/shop/shop_ban8.png");
	spXHPTextD->setPosition(Vec2(spXHPD->getContentSize().width / 2, spXHPD->getContentSize().height / 2));
	spXHPD->addChild(spXHPTextD);
	m_mBtnType[0] = MenuItemSprite::create(spXHPN, spXHPS, spXHPD, CC_CALLBACK_1(GoodsStoreLayer::callBackSwitch, this));
	m_mBtnType[0]->setTag(STORE_XHP);
	m_mBtnType[0]->setPosition(point + Vec2(293, 62));
	ptrMenu->addChild(m_mBtnType[0]);

	//藏宝阁
	Sprite* spCBGN = ReadSpriteName("new_ui/img_global/global_ban15.png");
	Sprite* spCBGTextN = ReadSpriteName("new_ui/shop/shop_ban9.png");
	spCBGTextN->setPosition(Vec2(spCBGN->getContentSize().width / 2, spCBGN->getContentSize().height / 2));
	spCBGN->addChild(spCBGTextN);
	Sprite* spCBGS = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spCBGTextS = ReadSpriteName("new_ui/shop/shop_ban10.png");
	spCBGTextS->setPosition(Vec2(spCBGS->getContentSize().width / 2, spCBGS->getContentSize().height / 2));
	spCBGS->addChild(spCBGTextS);
	Sprite* spCBGD = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spCBGTextD = ReadSpriteName("new_ui/shop/shop_ban10.png");
	spCBGTextD->setPosition(Vec2(spCBGD->getContentSize().width / 2, spCBGD->getContentSize().height / 2));
	spCBGD->addChild(spCBGTextD);
	m_mBtnType[1] = MenuItemSprite::create(spCBGN, spCBGS, spCBGD, CC_CALLBACK_1(GoodsStoreLayer::callBackSwitch, this));
	m_mBtnType[1]->setTag(STORE_CBG);
	m_mBtnType[1]->setPosition(point + Vec2(293, -28));
	ptrMenu->addChild(m_mBtnType[1]);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(GoodsStoreLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(280, 172));
	ptrMenu->addChild(btnClose);
}

void GoodsStoreLayer::initGoodsMap()
{
	//提示
	m_labNotice->setString(m_ptrStore->getContent());

	int iTypeIndex = 0;
	for(map<string, map<int, StoreItem*> >::iterator it = m_ptrStore->m_mStoreItem.begin(); it != m_ptrStore->m_mStoreItem.end(); it++)
	{
		map<int, StoreItem*> mStoreItem = it->second;
		int iIndex = 0;
		map<int, SellGoods*> mSellGoods;
		for(map<int, StoreItem*>::iterator itStore = mStoreItem.begin(); itStore != mStoreItem.end(); itStore++)
		{
			StoreItem* ptrStore = itStore->second;
			SellGoods* ptrItem = new SellGoods();
			ptrItem->setIndex(iIndex);
			ptrItem->setKeyId(itStore->first);
			ptrItem->setGoodsId(ptrStore->getGoodsId());
			ptrItem->setCount(ptrStore->getCount());
			ptrItem->setPrice(ptrStore->getPrice());
			ptrItem->setDiscount(ptrStore->getDiscount());
			ptrItem->setPayType(ptrStore->getPayType());
			ptrItem->setDealType(DEAL_STORE);
			ptrItem->setIsSell(0);
			ptrItem->setIconPath(ptrStore->getIconPath());
			ptrItem->setStrType(it->first);
			mSellGoods[iIndex++] = ptrItem;
		}
		m_mSellGoods[iTypeIndex++] = mSellGoods;
	}

	//TableView 获取到数据再创建TableView
	m_mTVGoods = GoodsTableView::create(this, m_mSellGoods[m_iCurIndex]);
	this->addChild(m_mTVGoods);

	updateDisplay();
}

void GoodsStoreLayer::updateDisplay()
{
	for(UInt i = 0; i < m_mBtnType.size(); i++)
	{
		m_mBtnType[i]->setEnabled(m_iCurIndex != i);
	}

	m_mTVGoods->updateTableView(m_mSellGoods[m_iCurIndex]);
}

void GoodsStoreLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case SHOP_STROE_INIT:
	{
		strURL = "getGoodsMan";
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s", strToken.c_str()));
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SHOP_STROE_INIT";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(GoodsStoreLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void GoodsStoreLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		if (strTag == "SHOP_STROE_INIT")
		{
			onStoreInitSucceed(value["result"]);
		}
	}
}

void GoodsStoreLayer::onStoreInitSucceed(Json::Value valRes)
{
	m_ptrStore = new StoreInfo();
	m_ptrStore->loadJson(valRes);
	initGoodsMap();
}

void GoodsStoreLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	removeFromParent();
}

void GoodsStoreLayer::callBackSwitch(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* pItem = (MenuItemSprite*)sender;
	//if (pItem->getTag() == STORE_XHP)
	//{
	//	ITools::getInstance()->createGlobalPrompt(LL("jingqingqidai"), false);
	//	return;
	//}
	m_iCurIndex = pItem->getTag();
	updateDisplay();
}

void GoodsStoreLayer::callBackJade(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this));
}