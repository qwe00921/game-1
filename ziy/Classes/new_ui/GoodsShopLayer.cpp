//
//  GoodsShopLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/5/9.
//
//

#include "GoodsShopLayer.h"
#include "Defines.h"
#include "GoodsDealLayer.h"
#include "SkillAndGoodsInfoLayer.h"
#include "GoldExchangeLayer.h"
#include "CocoMd5.h"
#include "PopLayer.h"
#include "BuyJadeLayer.h"
#include "ActivityInfo.h"

#define GOODS_ROW_NUM 2

#define GOODS_VIEW_W 530
#define GOODS_VIEW_H 270
#define GOODS_ITEM_W 166
#define GOODS_ITEM_H 135

enum
{
	SHOP_SELL_INIT = 0,
	SHOP_SELL_REFRESH = 1,
	SHOP_SELL_BUY = 2,
	SHOP_EXCH_INIT = 3,
	SHOP_EXCH_CHANGE =4,
	SHOP_STROE_INIT = 5,
	SHOP_STROE_BUY =6
};

//ShopGoodsSprite////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ShopGoodsSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_bIsSelect = false;
	m_iIndex = -1;

	initControl();
	return true;
}

bool ShopGoodsSprite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_spCellBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())) && isVisible())
	{
		GoodsTableView::ms_iSelectIndex = m_iIndex;
		m_bIsSelect = true;
	}
	return true;
}

void ShopGoodsSprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsSelect)
	{
		if(!m_spCellBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			GoodsTableView::ms_iSelectIndex = -1;
			m_bIsSelect = false;
		}
	}
}

void ShopGoodsSprite::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	m_bIsSelect = false;
}

void ShopGoodsSprite::initControl()
{
	//Cell背景
	m_spCellBG = ReadSpriteName("new_ui/shop/shop_bg11.png");
	this->addChild(m_spCellBG);

	//名称
	m_labName = LabelSystemFont("", 18);
	m_labName->setColor(Color3B(153, 51, 0));
	m_labName->setPosition(Vec2(78, 115));
	m_spCellBG->addChild(m_labName);

	//Icon背景
	Sprite* spIconBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	spIconBG->setPosition(Vec2(78, 70));
	m_spCellBG->addChild(spIconBG);

	//Icon
	m_spIcon = ReadSpriteNormal("img/Items/1.png");
	m_spIcon->setPosition(Vec2(78, 70));
	m_spCellBG->addChild(m_spIcon);

	//武魂
	m_spSoul = ReadSpriteName("new_ui/hero/icon_soul.png");
	m_spSoul->setPosition(Vec2(56, 92));
	m_spCellBG->addChild(m_spSoul);

	//数目
	m_spNum = ITools::getInstance()->getNumSprite(1, true);
	m_spNum->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_spNum->setPosition(Vec2(106, 41));
	m_spCellBG->addChild(m_spNum);

	//价格Icon
	m_spPrice = ReadSpriteName("new_ui/img_global/icon_gb.png");
	m_spPrice->setPosition(Vec2(23, 23));
	m_spCellBG->addChild(m_spPrice);

	//价格
	m_labPrice = LabelSystemFont("100", 16);
	m_labPrice->setColor(Color3B::YELLOW);
	m_labPrice->setPosition(Vec2(78, 23));
	m_spCellBG->addChild(m_labPrice);

	//折扣
	m_spDisCount = ReadSpriteName("new_ui/shop/dz_1.png");
	m_spDisCount->setPosition(Vec2(23, 114));
	m_spCellBG->addChild(m_spDisCount);

	//折扣勾玉
	m_spDisIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	m_spDisIcon->setPosition(Vec2(62, 98));
	m_spCellBG->addChild(m_spDisIcon);

	//折扣线
	m_spDisLine = ReadSpriteName("new_ui/shop/dz_line1.png");
	m_spDisLine->setPosition(Vec2(78, 23));
	m_spCellBG->addChild(m_spDisLine);

	//已售卖
	m_spIsSell = ReadSpriteName("new_ui/shop/shop_bg12.png");
	m_spIsSell->setPosition(Vec2(78, 66));
	m_spCellBG->addChild(m_spIsSell, 9);
}

void ShopGoodsSprite::updateSprite(SellGoods* sellGoods)
{
	int iGoodsId = sellGoods->getGoodsId();
	int iCount = sellGoods->getCount();
	int iPrice = sellGoods->getPrice();
	int iDiscount = sellGoods->getDiscount();
	string strPath = sellGoods->getIconPath();
	m_iIndex = sellGoods->getIndex();

	HistoryGoods hisGoods(iGoodsId);
	if(hisGoods.isNormalGood())
	{
		//名称
		m_labName->setString(hisGoods.GetGoodsName());

		//Icon
		m_spIcon->setTexture(GetSpriteTexture(history->GetGoodsImagePath(hisGoods.good_id).c_str()));

		//武魂
		if(hisGoods.GetGoodsType() == GOOD_TYPE_PIECE)
		{
			string strSpPath = "";
			if (hisGoods.isSoul())
			{
				strSpPath = "new_ui/hero/icon_soul.png";
			}
			else if (hisGoods.isPiece())
			{
				strSpPath = "new_ui/img_global/icon_sp.png";
			}

			m_spSoul->setTexture(ReadSpriteTexture(strSpPath.c_str()));
			m_spSoul->setVisible(true);
		}
		else
		{
			m_spSoul->setVisible(false);
		}

		//数目
		if(iCount > 1)
		{
			m_spNum->removeFromParent();
			m_spNum = ITools::getInstance()->getNumSprite(iCount, true);
			m_spNum->setAnchorPoint(Vec2(1.0f, 0.0f));
			m_spNum->setPosition(Vec2(106, 41));
			m_spCellBG->addChild(m_spNum);
		}
		else
		{
			m_spNum->setVisible(false);
		}

		//价格Icon
		m_spPrice->setTexture(ReadSpriteTexture(strPath.c_str()));

		//价格
		m_labPrice->setString(IntToString(iPrice));

		//打折相关
		bool bIsDiscount = iDiscount != 0;
		m_spDisCount->setVisible(bIsDiscount);
		m_spDisIcon->setVisible(bIsDiscount);
		m_spDisLine->setVisible(bIsDiscount);

		if (bIsDiscount)
		{
			//折扣
			m_spDisCount->setTexture(ReadSpriteTexture(StringUtils::format("new_ui/shop/dz_%d.png", iDiscount).c_str()));

			//折扣勾玉
			m_spDisIcon->setTexture(ReadSpriteTexture(strPath.c_str()));
			int iDisPrice = iPrice * iDiscount / 10;
			m_spDisIcon->removeAllChildren();
			auto spPrice = ITools::getInstance()->getNumSprite(iDisPrice, false, 5);
			spPrice->setAnchorPoint(Vec2(0.0f, 0.5f));
			spPrice->setPosition(Vec2(m_spDisIcon->getContentSize().width, m_spDisIcon->getContentSize().height / 2));
			m_spDisIcon->addChild(spPrice);

			//更新位置
			string strNum = StringUtils::format("%d", iDisPrice);
			int iTotalWidth = m_spDisIcon->getContentSize().width + spPrice->getContentSize().width * strNum.length();
			m_spDisIcon->setPosition(Vec2(78 - iTotalWidth / 2 + m_spDisIcon->getContentSize().width / 2, 98));
		}

		//已售卖
		m_spIsSell->setVisible(sellGoods->getIsSell() ? true : false);
	}
}

//GoodsCell////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GoodsCell* GoodsCell::create(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods)
{
	GoodsCell* pRet = new GoodsCell();
	if(pRet&&pRet->init(ptrDelegate, mSellGoods))
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

bool GoodsCell::init(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods)
{
	if(!TableViewCell::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;

	initControl();
	updateCell(mSellGoods);
	return true;
}

GoodsCell::GoodsCell()
{

}

void GoodsCell::initControl()
{
	for(int i = 0; i < GOODS_ROW_NUM; i++)
	{
		m_mSellGoods[i] = ShopGoodsSprite::create();
		m_mSellGoods[i]->setPosition(Vec2(GOODS_ITEM_W / 2, GOODS_ITEM_H / 2 - GOODS_ITEM_H * i));
		this->addChild(m_mSellGoods[i]);
	}
}

void GoodsCell::showGoodsDeal()
{
	if(GoodsTableView::ms_iSelectIndex >= 0)
	{
		m_pDelegate->callBackListClick(GoodsTableView::ms_iSelectIndex);
		GoodsTableView::ms_iSelectIndex = -1;
	}
}

void GoodsCell::updateCell(map<int, SellGoods*> mSellGoods)
{
	for(int i = 0; i < GOODS_ROW_NUM; i++)
	{
		if (mSellGoods.count(i))
		{
			m_mSellGoods[i]->updateSprite(mSellGoods[i]);
			m_mSellGoods[i]->setVisible(true);
		}
		else
		{
			m_mSellGoods[i]->setVisible(false);
		}
	}
}

void GoodsCell::callBackClick(int iIndex)
{
	ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
	CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(GoodsCell::showGoodsDeal, this));
	m_mSellGoods[iIndex]->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
}

//GoodsTableView////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GoodsTableView::ms_iSelectIndex = -1;

GoodsTableView* GoodsTableView::create(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods)
{
	GoodsTableView* pRet = new GoodsTableView();
	if(pRet&&pRet->init(ptrDelegate, mSellGoods))
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

bool GoodsTableView::init(EventDelegate* ptrDelegate, map<int, SellGoods*> mSellGoods)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_mSellGoods = mSellGoods;

	//创建一个talbleview 将datasource设置为当前的类的对象
	m_pTableView = TableView::create(this, Size(GOODS_VIEW_W, GOODS_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::HORIZONTAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - GOODS_VIEW_W / 2, size.height / 2 - GOODS_VIEW_H / 2 - 37));
	//设置代理对象
	m_pTableView->setDelegate(this);
//GHB		m_pTableView->setTouchPriority(LAYER_BOTTOM);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);

	return true;
}

GoodsTableView::GoodsTableView()
{
	m_iSelectIndex = -1;
	m_pDelegate = NULL;
}

GoodsTableView::~GoodsTableView()
{

}

void GoodsTableView::scrollViewDidScroll(ScrollView* view)
{

}

void GoodsTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if(ms_iSelectIndex >= 0)
	{
		GoodsCell* ptrCell = (GoodsCell*)cell;
		ptrCell->callBackClick(ms_iSelectIndex % 2);
	}
}

Size GoodsTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(GOODS_ITEM_W, GOODS_ITEM_H);
}

TableViewCell* GoodsTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	m_mCell[idx] = NULL;

	map<int, SellGoods*> mSellGoods;
	for(int i = 0; i < GOODS_ROW_NUM; i++)
	{
		int iIndex = i + idx * GOODS_ROW_NUM;
		if (iIndex < (int)m_mSellGoods.size())
		{
			mSellGoods[i] = m_mSellGoods[i + idx * 2];
		}
	}
	if(ptrCell)
	{
		m_mCell[idx] = (GoodsCell*)ptrCell;
		m_mCell[idx]->updateCell(mSellGoods);
	}
	else
	{
		m_mCell[idx] = GoodsCell::create(m_pDelegate, mSellGoods);
	}
	return m_mCell[idx];
}

ssize_t GoodsTableView::numberOfCellsInTableView(TableView *table)
{
	return (m_mSellGoods.size() - 1) / GOODS_ROW_NUM + 1;
}

void GoodsTableView::updateTableView(map<int, SellGoods*> mSellGoods)
{
	m_mSellGoods = mSellGoods;
	m_pTableView->reloadData();
}

void GoodsTableView::updateGoodsCell(int iIndex)
{
	int idx = iIndex / 2;
	map<int, SellGoods*> mSellGoods;
	for(int i = 0; i < GOODS_ROW_NUM; i++)
	{
		int iIndex = i + idx * GOODS_ROW_NUM;
		if(iIndex < (int)m_mSellGoods.size())
		{
			mSellGoods[i] = m_mSellGoods[i + idx * 2];
		}
	}
	m_mCell[idx]->updateCell(mSellGoods);
}

bool GoodsShopLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_iRefresh = 1;
	m_iMaxRef = trData->m_mShopRefPrice.size();

	initControl();
	onHttpRequest(SHOP_SELL_INIT);
	return true;
}

void GoodsShopLayer::initControl()
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
	Sprite* spTitle = ReadSpriteName("new_ui/img_text/shop_txt1.png");
	spTitle->setPosition(Vec2(305, 369));
	spBG->addChild(spTitle);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//魂玉充值
	Sprite* spJadeN = ReadSpriteName("new_ui/main/main_bg2.png");
	Sprite* spJadeS = ReadSpriteName("new_ui/main/main_bg2.png");
	MenuItemSprite* btnJade = MenuItemSprite::create(spJadeN, spJadeS, CC_CALLBACK_1(GoodsShopLayer::callBackJade, this));
	btnJade->setPosition(point + Vec2(-200, 155));
	m_pBtnMenu->addChild(btnJade);

	//金币充值
	Sprite* spGoldN = ReadSpriteName("new_ui/main/main_bg2.png");
	Sprite* spGoldS = ReadSpriteName("new_ui/main/main_bg2.png");
	MenuItemSprite* btnGold = MenuItemSprite::create(spGoldN, spGoldS, CC_CALLBACK_1(GoodsShopLayer::callBackGold, this));
	btnGold->setPosition(point + Vec2(-200, 118));
	m_pBtnMenu->addChild(btnGold);

	//魂玉
	m_labJade = LabelSystemFont(IntToString(history->m_iJade), 16);
	m_labJade->setColor(Color3B(0xff, 0xff, 0));
	m_labJade->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labJade->setPosition(Vec2(90, 16));
	btnJade->addChild(m_labJade);

	Sprite* spJadeIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJadeIcon->setPosition(Vec2(115, 16));
	btnJade->addChild(spJadeIcon);

	//金币
	m_labGold = LabelSystemFont(IntToStringEx(history->money), 16);
	m_labGold->setColor(Color3B(0xff, 0xff, 0));
	m_labGold->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labGold->setPosition(Vec2(90, 16));
	btnGold->addChild(m_labGold);

	Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGoldIcon->setPosition(Vec2(115, 16));
	btnGold->addChild(spGoldIcon);

	//刷新时间
	m_labRefTime = LabelSystemFont(__String::createWithFormat(LL("nextrefreshtime"), 0)->getCString(), 18);
	m_labRefTime->setColor(Color3B(153, 51, 0));
	m_labRefTime->setPosition(point + Vec2(0, 115));
	this->addChild(m_labRefTime);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(GoodsShopLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(280, 172));
	m_pBtnMenu->addChild(btnClose);

	//刷新
	Sprite* spRefreshN = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* spRefreshTextN = ReadSpriteName("new_ui/img_text/imgtxt2_5.png");
	spRefreshTextN->setPosition(Vec2(spRefreshN->getContentSize().width / 2, spRefreshN->getContentSize().height / 2));
	spRefreshN->addChild(spRefreshTextN);
	Sprite* spRefreshS = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* spRefreshTextS = ReadSpriteName("new_ui/img_text/imgtxt2_6.png");
	spRefreshTextS->setPosition(Vec2(spRefreshS->getContentSize().width / 2, spRefreshS->getContentSize().height / 2));
	spRefreshS->addChild(spRefreshTextS);
	MenuItemSprite* btnRefresh = MenuItemSprite::create(spRefreshN, spRefreshS, CC_CALLBACK_1(GoodsShopLayer::callBackRefresh, this));
	btnRefresh->setPosition(point + Vec2(228, 115));
	m_pBtnMenu->addChild(btnRefresh);
}

GoodsShopLayer::GoodsShopLayer()
{

}
GoodsShopLayer::~GoodsShopLayer()
{
	for (UInt i = 0;  i < m_mSellGoods.size(); i++)
	{
		SAFE_DELETE(m_mSellGoods[i]);
	}
	m_mSellGoods.clear();
}

void GoodsShopLayer::updateShow(int index)
{
	if(index < (int)m_mSellGoods.size())
	{
		m_mSellGoods[index]->setIsSell(1);
		m_mTVGoods->updateGoodsCell(index);
	}

	m_labGold->setString(IntToStringEx(history->money));
	m_labJade->setString(IntToString(history->m_iJade));

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

void GoodsShopLayer::callBackGold(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
}

void GoodsShopLayer::callBackJade(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this));
}

void GoodsShopLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	removeFromParent();
}

void GoodsShopLayer::callBackRefresh(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iRefTime = (m_iRefresh + 1) < m_iMaxRef ? (m_iRefresh + 1) : m_iMaxRef;
	int iPrice = trData->m_mShopRefPrice[iRefTime];
	Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_SP_REF, this, iPrice));
}

void GoodsShopLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case SHOP_SELL_INIT:
	{
		strURL = "getShopData";
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s", strToken.c_str()));
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SHOP_SELL_INIT";
	}
	break;
	case SHOP_SELL_REFRESH:
	{
		strURL = "refreshShopData";
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s", strToken.c_str()));
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SHOP_SELL_REFRESH";
	}
	break;
	case SHOP_SELL_BUY:
		break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(GoodsShopLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void GoodsShopLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		Json::Value result = value["result"];
		if(strTag == "SHOP_SELL_INIT")
		{
			parseJson(result, SHOP_SELL_INIT);
			//TableView 获取到数据再创建TableView
			m_mTVGoods = GoodsTableView::create(this, m_mSellGoods);
			this->addChild(m_mTVGoods);
		}
		else if(strTag == "SHOP_SELL_REFRESH")
		{
			parseJson(result, SHOP_SELL_REFRESH);
			m_mTVGoods->updateTableView(m_mSellGoods);
			m_labJade->setString(IntToString(history->m_iJade));
		}
	}
}

bool GoodsShopLayer::parseJson(Json::Value val, int iReqType)
{
	Json::Value goodsList = val["goods"];
	for(UInt i = 0; i < goodsList.size(); i++)
	{
		string strKey = IntToString(i + 1);
		int iGoodsId = goodsList[strKey.c_str()]["id"].asInt();
		int iIsSell = goodsList[strKey.c_str()]["isBuy"].asInt();

		if (!m_mSellGoods[i])
		{
			m_mSellGoods[i] = new SellGoods();
		}

		NetShopItem* ptrShop = trData->m_mNetShop[iGoodsId];
		string strPath = ptrShop->iPayType == 1 ? "new_ui/img_global/icon_gb.png" : "new_ui/img_global/icon_gy.png";
		m_mSellGoods[i]->setIndex(i);
		m_mSellGoods[i]->setKeyId(i + 1);
		m_mSellGoods[i]->setGoodsId(ptrShop->iGoodsId);
		m_mSellGoods[i]->setCount(ptrShop->iNum);
		m_mSellGoods[i]->setPrice(ptrShop->iPrice);
		m_mSellGoods[i]->setDiscount(0);
		m_mSellGoods[i]->setPayType(ptrShop->iPayType);
		m_mSellGoods[i]->setDealType(DEAL_BUY);
		m_mSellGoods[i]->setIsSell(iIsSell);
		m_mSellGoods[i]->setIconPath(strPath);
		m_mSellGoods[i]->setStrType("");
	}
	m_iRefresh = val["refreshCount"].asInt();

	switch(iReqType)
	{
	case SHOP_SELL_INIT:
		if(val["nextRefTime"].isInt())
		{
			int iHour = val["nextRefTime"].asInt();
			m_labRefTime->setString(StringUtils::format(LL("nextrefreshtime"), iHour));
		}
		break;
	case SHOP_SELL_REFRESH:
		history->m_iJade = val["jade"].asInt();
		break;
	default:
		break;
	}
	return true;
}

void GoodsShopLayer::callBackListClick(int iIndex)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_mSellGoods[iIndex]->getIsSell())
	{
		ITools::getInstance()->createGlobalPrompt(LL("goodssold"), false);
	}
	else
	{
		this->addChild(GoodsDealLayer::create(m_mSellGoods[iIndex], this));
	}
}

void GoodsShopLayer::callBackConfirm()
{
	int iRefTime = (m_iRefresh + 1) < m_iMaxRef ? (m_iRefresh + 1) : m_iMaxRef;
	int iPrice = trData->m_mShopRefPrice[iRefTime];

	if(history->m_iJade < iPrice)
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
		return;
	}

	onHttpRequest(SHOP_SELL_REFRESH);
}

void GoodsShopLayer::updateMoneyShow()
{
	m_labJade->setString(IntToString(history->m_iJade));
	if (history->m_layTitleBar)
		history->m_layTitleBar->updateShow(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GoodsExchLayer::ms_iExchType = EXCH_JJL;

bool GoodsExchLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();
	updateToken();
	if(BTActivity::getInstance()->isExchInit())
	{
		initGoodsMap();
	}
	else
	{
		onHttpRequest(SHOP_EXCH_INIT);
	}

	return true;
}

void GoodsExchLayer::initControl()
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
	Sprite* spTitle = ReadSpriteName("new_ui/img_text/shop_txt2.png");
	spTitle->setPosition(Vec2(305, 369));
	spBG->addChild(spTitle);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//令牌背景
	Sprite* spTokenBG = ReadSpriteName("new_ui/shop/shop_bg3.png");
	spTokenBG->setPosition(point + Vec2(-192, 113));
	this->addChild(spTokenBG);

	//令牌ICON
	m_spToken = ReadSpriteName("new_ui/img_global/icon_pz1.png");
	m_spToken->setPosition(Vec2(15, 15));
	spTokenBG->addChild(m_spToken);

	//令牌库存
	m_labToken = LabelSystemFont("100", 16);
	m_labToken->setColor(Color3B(0xff, 0xff, 0));
	m_labToken->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labToken->setPosition(Vec2(40, 15));
	spTokenBG->addChild(m_labToken);

	//提示
	auto labNotice = LabelSystemFont(LL("shopexchnotice"), 18);
	labNotice->setAnchorPoint(Vec2(1.0f, 0.5f));
	labNotice->setColor(Color3B(153, 51, 0));
	labNotice->setPosition(point + Vec2(250, 113));
	this->addChild(labNotice);

	//将军令
	Sprite* spJJLN = ReadSpriteName("new_ui/img_global/global_ban15.png");
	Sprite* spJJLTextN = ReadSpriteName("new_ui/shop/shop_ban1.png");
	spJJLTextN->setPosition(Vec2(spJJLN->getContentSize().width / 2, spJJLN->getContentSize().height / 2));
	spJJLN->addChild(spJJLTextN);
	Sprite* spJJLS = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spJJLTextS = ReadSpriteName("new_ui/shop/shop_ban2.png");
	spJJLTextS->setPosition(Vec2(spJJLS->getContentSize().width / 2, spJJLS->getContentSize().height / 2));
	spJJLS->addChild(spJJLTextS);
	Sprite* spJJLD = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spJJLTextD = ReadSpriteName("new_ui/shop/shop_ban2.png");
	spJJLTextD->setPosition(Vec2(spJJLD->getContentSize().width / 2, spJJLD->getContentSize().height / 2));
	spJJLD->addChild(spJJLTextD);
	m_mBtnType[0] = MenuItemSprite::create(spJJLN, spJJLS, spJJLD, CC_CALLBACK_1(GoodsExchLayer::callBackSwitch, this));
	m_mBtnType[0]->setTag(EXCH_JJL);
	m_mBtnType[0]->setPosition(point + Vec2(293, 62));
	ptrMenu->addChild(m_mBtnType[0]);

	//英雄令
	Sprite* spYXLN = ReadSpriteName("new_ui/img_global/global_ban15.png");
	Sprite* spYXLTextN = ReadSpriteName("new_ui/shop/shop_ban3.png");
	spYXLTextN->setPosition(Vec2(spYXLN->getContentSize().width / 2, spYXLN->getContentSize().height / 2));
	spYXLN->addChild(spYXLTextN);
	Sprite* spYXLS = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spYXLTextS = ReadSpriteName("new_ui/shop/shop_ban4.png");
	spYXLTextS->setPosition(Vec2(spYXLS->getContentSize().width / 2, spYXLS->getContentSize().height / 2));
	spYXLS->addChild(spYXLTextS);
	Sprite* spYXLD = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spYXLTextD = ReadSpriteName("new_ui/shop/shop_ban4.png");
	spYXLTextD->setPosition(Vec2(spYXLD->getContentSize().width / 2, spYXLD->getContentSize().height / 2));
	spYXLD->addChild(spYXLTextD);
	m_mBtnType[1] = MenuItemSprite::create(spYXLN, spYXLS, spYXLD, CC_CALLBACK_1(GoodsExchLayer::callBackSwitch, this));
	m_mBtnType[1]->setTag(EXCH_YXL);
	m_mBtnType[1]->setPosition(point + Vec2(293, -28));
	ptrMenu->addChild(m_mBtnType[1]);

	//高狼令
	Sprite* spGLLN = ReadSpriteName("new_ui/img_global/global_ban15.png");
	Sprite* spGLLTextN = ReadSpriteName("new_ui/shop/shop_ban5.png");
	spGLLTextN->setPosition(Vec2(spGLLN->getContentSize().width / 2, spGLLN->getContentSize().height / 2));
	spGLLN->addChild(spGLLTextN);
	Sprite* spGLLS = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spGLLTextS = ReadSpriteName("new_ui/shop/shop_ban6.png");
	spGLLTextS->setPosition(Vec2(spGLLS->getContentSize().width / 2, spGLLS->getContentSize().height / 2));
	spGLLS->addChild(spGLLTextS);
	Sprite* spGLLD = ReadSpriteName("new_ui/img_global/global_ban16.png");
	Sprite* spGLLTextD = ReadSpriteName("new_ui/shop/shop_ban6.png");
	spGLLTextD->setPosition(Vec2(spGLLD->getContentSize().width / 2, spGLLD->getContentSize().height / 2));
	spGLLD->addChild(spGLLTextD);
	m_mBtnType[2] = MenuItemSprite::create(spGLLN, spGLLS, spGLLD, CC_CALLBACK_1(GoodsExchLayer::callBackSwitch, this));
	m_mBtnType[2]->setTag(EXCH_GLL);
	m_mBtnType[2]->setPosition(point + Vec2(293, -118));
	ptrMenu->addChild(m_mBtnType[2]);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(GoodsExchLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(280, 172));
	ptrMenu->addChild(btnClose);
}

void GoodsExchLayer::initGoodsMap()
{
	map<int, map<int, ActExchElem*> > mExchGoods = BTActivity::getInstance()->m_mExchGoods;
	for (UInt i = 0; i < mExchGoods.size(); i++)
	{
		map<int, SellGoods*> mSellGoods;
		int iIndex = 0;
		for (map<int, ActExchElem*>::iterator it = mExchGoods[i].begin(); it != mExchGoods[i].end(); it++)
		{
			ActExchElem* ptrExch = it->second;
			SellGoods* ptrItem = new SellGoods();
			int iGoodsId = ptrExch->m_mGetGoods.begin()->first;
			map<int, map<int, ActExchElem*> > mExchGoods = BTActivity::getInstance()->m_mExchGoods;
			ptrItem->setIndex(iIndex);
			ptrItem->setKeyId(ptrExch->getElemId());
			ptrItem->setGoodsId(iGoodsId);
			ptrItem->setCount(ptrExch->m_mGetGoods[iGoodsId]);
			ptrItem->setPrice(ptrExch->m_mUseGoods.begin()->second);
			ptrItem->setDiscount(0);
			ptrItem->setPayType(PAY_TYPE_OTHER);
			ptrItem->setDealType(DEAL_EXCH);
			ptrItem->setIsSell(0);
			ptrItem->setIconPath(StringUtils::format("new_ui/img_global/icon_pz%d.png", i + 1));
			ptrItem->setStrType("");
			mSellGoods[iIndex++] = ptrItem;
		}
		m_mExchGoods[i] = mSellGoods;
	}

	for(UInt i = 0; i < m_mBtnType.size(); i++)
	{
		m_mBtnType[i]->setEnabled(ms_iExchType != i);
	}

	//TableView 获取到数据再创建TableView
	m_mTVGoods = GoodsTableView::create(this, m_mExchGoods[ms_iExchType]);
	this->addChild(m_mTVGoods);
}

void GoodsExchLayer::updateDisplay()
{
	m_mTVGoods->updateTableView(m_mExchGoods[ms_iExchType]);
	updateToken();
}

void GoodsExchLayer::updateToken()
{
	string strPath = StringUtils::format("new_ui/img_global/icon_pz%d.png", ms_iExchType + 1);
	m_spToken->setTexture(ReadSpriteTexture(strPath.c_str()));

	int iGoodsId = BTActivity::getInstance()->getMaterialId(ms_iExchType);
	HistoryGoods hisGoods(iGoodsId);
	int iCount = hisGoods.getStoreNum();
	m_labToken->setString(StringUtils::format("%d", iCount));
}

GoodsExchLayer::GoodsExchLayer()
{

}

GoodsExchLayer::~GoodsExchLayer()
{
	for (UInt i = 0; i < m_mExchGoods.size(); i++)
	{
		for (UInt j = 0; j < m_mExchGoods[i].size(); j++)
		{
			SAFE_DELETE(m_mExchGoods[i][j]);
		}
		m_mExchGoods[i].clear();
	}
	m_mExchGoods.clear();
}

void GoodsExchLayer::updateShow(int index)
{
	updateToken();

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

void GoodsExchLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	removeFromParent();
}

void GoodsExchLayer::callBackSwitch(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	MenuItemSprite* pItem = (MenuItemSprite*)sender;
	int iTag = pItem->getTag();

	for(UInt i = 0; i < m_mBtnType.size(); i++)
	{
		m_mBtnType[i]->setEnabled(iTag != i);
	}

	ms_iExchType = iTag;
	updateDisplay();
}

void GoodsExchLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case SHOP_EXCH_INIT:
	{
		strURL = "getExChangeGoods";
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s", strToken.c_str()));
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SHOP_EXCH_INIT";
	}
	break;
	case SHOP_EXCH_CHANGE:
		break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(GoodsExchLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void GoodsExchLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		if(strTag == "SHOP_EXCH_INIT")
		{
			BTActivity::getInstance()->loadExchJson(value["result"]);
			initGoodsMap();
		}
	}
}

void GoodsExchLayer::callBackListClick(int iIndex)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->addChild(GoodsDealLayer::create(m_mExchGoods[ms_iExchType][iIndex], this));
}