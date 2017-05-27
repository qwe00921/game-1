#include "BuyJadeLayer.h"
#include "Defines.h"
#include "PaymentTool.h"
#include "ActivityInfo.h"

#define JADE_NUM 6
#define JADE_LIST_NUM 2

#define JADE_VIEW_W 356
#define JADE_VIEW_H 274
#define JADE_ITEM_W 178
#define JADE_ITEM_H 137

#define VIP_INDEX 7
#define TOTAL_RECH_ACTID 2

enum
{
	REQ_VIPTIME = 0, //VIP时间
	REQ_RECHARGE = 1 //累充金额
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool JadeSprite::init()
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

bool JadeSprite::onTouchBegan(Touch* touch, Event* unused_event)
{
	if(m_spBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		JadeTableView::ms_iIndex = m_iIndex;
		m_bIsSelect = true;
	}
	return true;
}

void JadeSprite::onTouchMoved(Touch* touch, Event* unused_event)
{
	if(m_bIsSelect)
	{
		if(!m_spBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			JadeTableView::ms_iIndex = -1;
			m_bIsSelect = false;
		}
	}
}

void JadeSprite::onTouchEnded(Touch* touch, Event* unused_event)
{
	m_bIsSelect = false;
}

void JadeSprite::updateSprite(int iIndex)
{
	if(trData->net_jade_charge.count(iIndex))
	{
		NetJadeCharge* ptrJade = trData->net_jade_charge[iIndex];
		m_iIndex = iIndex;
		this->setTag(m_iIndex);

		//ICON
		string strPath = StringUtils::format("new_ui/shop/icon_%d.png", iIndex);
		m_spJade->setTexture(ReadSpriteTexture(strPath.c_str()));

		//价格
		m_labPrice->setString(StringUtils::format("%s%d", LL("rmb"), ptrJade->mRmb));

		string strNotice = "";
		if (ptrJade->mFirstJade && !history->m_pPlayerInfo->getBuyJadeState(ptrJade->mRmb))
		{
			strNotice = StringUtils::format("%s %d", LL("shouchongsong"), ptrJade->mFirstJade);
		}
		else if (ptrJade->mGiveJade)
		{
			strNotice = StringUtils::format("%s %d", LL("zengsong"), ptrJade->mGiveJade);
		}

		m_labGift->setString(strNotice);
		m_spGift->setVisible(strNotice != "");
	}
}

void JadeSprite::initControl()
{
	//背景
	m_spBG = ReadSpriteName("new_ui/shop/shop_bg11.png");
	this->addChild(m_spBG);

	//ICON
	m_spJade = ReadSpriteName("new_ui/shop/icon_1.png");
	m_spJade->setPosition(Vec2(78, 70));
	m_spBG->addChild(m_spJade);

	//价格
	m_labPrice = LabelSystemFont("", 16);
	m_labPrice->setPosition(Vec2(78, 19));
	m_labPrice->enableOutline(Color4B::BLACK, 1);
	m_spBG->addChild(m_labPrice);

	//福利标签
	m_spGift = ReadSpriteName("new_ui/shop/shop_bg4.png");
	m_spGift->setPosition(Vec2(83, 113));
	m_spBG->addChild(m_spGift);

	//福利内容
	m_labGift = LabelSystemFont("", 16);
	m_labGift->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labGift->setColor(Color3B::YELLOW);
	m_labGift->setPosition(Vec2(10, 29));
	m_spGift->addChild(m_labGift);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool JadeCell::init()
{
	if(!TableViewCell::init())
	{
		return false;
	}

	initControl();
	return true;
}

void JadeCell::updateCell(map<int, int> mIndex)
{
	for(int i = 0; i < JADE_LIST_NUM; i++)
	{
		m_mSpJade[i]->updateSprite(mIndex[i]);
	}
}

Sprite* JadeCell::getJadeSprite()
{
	Sprite* spRet = nullptr;
	for(UInt i = 0; i < m_mSpJade.size(); i++)
	{
		if(JadeTableView::ms_iIndex == m_mSpJade[i]->getTag())
		{
			spRet = m_mSpJade[i];
			break;
		}
	}
	return spRet;
}

void JadeCell::initControl()
{
	for(int i = 0; i < JADE_LIST_NUM; i++)
	{
		m_mSpJade[i] = JadeSprite::create();
		m_mSpJade[i]->setPosition(Vec2(JADE_ITEM_W / 2 + JADE_ITEM_W * i, JADE_ITEM_H / 2));
		this->addChild(m_mSpJade[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int JadeTableView::ms_iIndex = -1;

JadeTableView* JadeTableView::create(map<int, int> mIndex, EventDelegate* ptrDelegate)
{
	JadeTableView* pRet = new JadeTableView();
	if(pRet&&pRet->init(mIndex, ptrDelegate))
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

bool JadeTableView::init(map<int, int> mIndex, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_mIndex = mIndex;
	m_pDelegate = ptrDelegate;
	m_spArrowUp = NULL;
	m_spArrowDw = NULL;
	
	//创建一个talbleview 将datasource设置为当前的类的对象
	m_pTableView = TableView::create(this, Size(JADE_VIEW_W, JADE_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - JADE_VIEW_W / 2 + 84, size.height / 2 - JADE_VIEW_H / 2 - 22));
	//设置代理对象
	m_pTableView->setDelegate(this);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);

	//箭头上
	m_spArrowUp = ReadSpriteName("new_ui/activity/activity_jt1.png");
	m_spArrowUp->setPosition(Vec2(size.width / 2 + 84, size.height / 2 + JADE_VIEW_H / 2 - 14));
	this->addChild(m_spArrowUp);

	//箭头下
	m_spArrowDw = ReadSpriteName("new_ui/activity/activity_jt2.png");
	m_spArrowDw->setPosition(Vec2(size.width / 2 + 84, size.height / 2 - JADE_VIEW_H / 2 - 30));
	this->addChild(m_spArrowDw);

	updateArrowShow();

	return true;
}

void JadeTableView::scrollViewDidScroll(ScrollView* view)
{
	updateArrowShow();
}

void JadeTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if(ms_iIndex > -1)
	{
		JadeCell* ptrCell = (JadeCell*)cell;

		ITools::getInstance()->addWaitLayer(false);
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(JadeTableView::jadeSpriteClick, this));
		ptrCell->getJadeSprite()->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
	}
}

cocos2d::Size JadeTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(JADE_ITEM_W, JADE_ITEM_H);
}

TableViewCell* JadeTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	JadeCell* cell = NULL;
	if(ptrCell)
	{
		cell = (JadeCell*)ptrCell;
	}
	else
	{
		cell = JadeCell::create();
	}

	map<int, int> mIndex;
	for (int i = 0; i < JADE_LIST_NUM; i++)
	{
		mIndex[i] = m_mIndex[idx * JADE_LIST_NUM + i];
	}
	cell->updateCell(mIndex);
	return cell;
}

ssize_t JadeTableView::numberOfCellsInTableView(TableView *table)
{
	return (m_mIndex.size() - 1) / JADE_LIST_NUM + 1;
}

void JadeTableView::updateTableView()
{
	Vec2 point = m_pTableView->getContentOffset();
	m_pTableView->reloadData();
	m_pTableView->setContentOffset(point);
}

void JadeTableView::jadeSpriteClick()
{
	if(ms_iIndex > -1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_pDelegate->callBackListClick(ms_iIndex);
		ms_iIndex = -1;
	}
}

void JadeTableView::updateArrowShow()
{
	if(m_spArrowUp)
	{
		Vec2 point = m_pTableView->getContentOffset();
		m_spArrowUp->setVisible(point.y - JADE_VIEW_H + JADE_ITEM_H * JADE_NUM / 2 > 0);
		m_spArrowDw->setVisible(point.y < 0);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BuyJadeLayer* BuyJadeLayer::create(EventDelegate* ptrDelegate)
{
	BuyJadeLayer* pRet = new BuyJadeLayer();
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

BuyJadeLayer* BuyJadeLayer::create(EventDelegate* ptrDelegate, bool fireCloseEvent)
{
	BuyJadeLayer* pRet = new BuyJadeLayer();
	if (pRet&&pRet->init(ptrDelegate, fireCloseEvent))
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

bool BuyJadeLayer::init(EventDelegate* ptrDelegate, bool fireCloseEvent)
{
	bool rs = BuyJadeLayer::init(ptrDelegate);
	m_fireCloseEvent = fireCloseEvent;
	return rs;
}

bool BuyJadeLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}
	m_fireCloseEvent = false;
	m_pDelegate = ptrDelegate;
	m_bIsClick = false;
	m_bIsVip = false;
	m_bIsContinue = false;

	initControl();
	updateVipLayer();
	onHttpRequest(REQ_VIPTIME);
	onHttpRequest(REQ_RECHARGE);
	return true;
}

bool BuyJadeLayer::onTouchBegan(Touch* touch, Event* unused_event)
{
	if(m_spVipBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_spVipBG->setScale(0.9f);
		m_bIsClick = true;
	}
	return true;
}

void BuyJadeLayer::onTouchMoved(Touch* touch, Event* unused_event)
{
	if(m_bIsClick)
	{
		if(!m_spVipBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			m_spVipBG->setScale(1.0f);
			m_bIsClick = false;
		}
	}
}

void BuyJadeLayer::onTouchEnded(Touch* touch, Event* unused_event)
{
	if (m_bIsClick)
	{
		m_spVipBG->setScale(1.0f);
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ITools::getInstance()->addWaitLayer(false);
		callBackListClick(VIP_INDEX);
		m_bIsClick = false;
	}
}

void BuyJadeLayer::callBackListClick(int iIndex)
{
	m_bIsVip = iIndex == VIP_INDEX;
	NetJadeChargePtr ptrJade = trData->net_jade_charge[iIndex];
	PaymentTool::getInstance()->startPay(ptrJade->mId, ptrJade->mChargeId, "", this);
}

void BuyJadeLayer::callBackClose(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_fireCloseEvent)
		getEventDispatcher()->dispatchCustomEvent("GAME_LAYER_CLOSE", (void*)"BuyJadeLayer");
	this->removeFromParent();
}

void BuyJadeLayer::RunFunction(int iNomal)
{
	ITools::getInstance()->removeWaitLayer();
	if(iNomal == 1)//支付成功
	{
		if (m_bIsVip)
		{
			m_bIsContinue = true;
			schedule(schedule_selector(BuyJadeLayer::updateVipState), 1.0f);
		}
		else
		{
			m_labJade->setString(IntToString(history->m_iJade));
			m_pDelegate->updateMoneyShow();
			m_tvJade->updateTableView();
		}

		onHttpRequest(REQ_RECHARGE);
		ITools::getInstance()->createGlobalPrompt(LL("zhifuchenggong"), true);
		BTActivity::getInstance()->updateActByType(ACT_RECHARGE);
	}
	else if(iNomal == -1)//支付未完成
	{
		ITools::getInstance()->createGlobalPrompt(LL("zhifuweiwancheng"), false);
	}
}

void BuyJadeLayer::updateParentShow()
{
	ITools::getInstance()->removeWaitLayer();
}

void BuyJadeLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/img_global/global_bg1.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	Sprite* spTitle = ReadSpriteName("new_ui/img_text/bt5_1.png");
	spTitle->setPosition(Vec2(spBG->getContentSize().width / 2, 363));
	spBG->addChild(spTitle);

	//勾玉背景
	Sprite* spJade = ReadSpriteName("new_ui/img_global/hero_bg4.png");
	spJade->setAnchorPoint(Vec2(0, 1));
	spJade->setPosition(Vec2(31+45, 352));
	spBG->addChild(spJade);

	//ICON
	Sprite* spIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spIcon->setPosition(Vec2(spIcon->getContentSize().width / 2 + 3, spJade->getContentSize().height / 2));
	spJade->addChild(spIcon);

	//勾玉数
	m_labJade = LabelSystemFont(IntToString(history->m_iJade), 16);
	m_labJade->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labJade->setColor(Color3B::GREEN);
	m_labJade->setPosition(Vec2(spIcon->getContentSize().width + 7, spJade->getContentSize().height / 2));
	spJade->addChild(m_labJade);

	//累充金额
	auto labRech = LabelSystemFont(LL("totalrecharge"), 16);
	labRech->setAnchorPoint(Vec2(0.0f, 0.5f));
	labRech->setPosition(Vec2(440, 335));
	labRech->enableOutline(Color4B(167, 134, 89, 255), 1);
	spBG->addChild(labRech);

	m_labRech = LabelSystemFont("", 16);
	m_labRech->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labRech->setPosition(Vec2(440 + labRech->getContentSize().width, 335));
	m_labRech->setColor(Color3B::YELLOW);
	m_labRech->enableOutline(Color4B(167, 134, 89, 255), 1);
	spBG->addChild(m_labRech);

	//商品背景
	Sprite* spJadeBG = ReadSpriteName("new_ui/shop/shop_bg1.png");
	spJadeBG->setAnchorPoint(Vec2(0.5f, 0));
	spJadeBG->setPosition(Vec2(spBG->getContentSize().width / 2, 22));
	spBG->addChild(spJadeBG);

	//VIP背景
	m_spVipBG = ReadSpriteName("new_ui/shop/shop_bg7.png");
	m_spVipBG->setPosition(point + Vec2(-178, -22));
	this->addChild(m_spVipBG);

	//VIP提示
	m_labVip1 = LabelSystemFont(LL("viprecharge1"), 16);
	m_labVip1->setPosition(Vec2(78, 155));
	m_labVip1->setColor(Color3B::YELLOW);
	m_labVip1->enableOutline(Color4B::BLACK, 1);
	m_labVip1->setVisible(false);
	m_spVipBG->addChild(m_labVip1);

	m_labVip2 = LabelSystemFont(LL("viprecharge2"), 16);
	m_labVip2->setPosition(Vec2(78, 143));
	m_labVip2->setColor(Color3B::YELLOW);
	m_labVip2->enableOutline(Color4B::BLACK, 1);
	m_spVipBG->addChild(m_labVip2);

	auto labVip3 = LabelSystemFont(LL("viprecharge3"), 16);
	labVip3->setAnchorPoint(Vec2(0.5f, 1.0f));
	labVip3->setPosition(Vec2(78, 110));
	labVip3->setDimensions(132, 0);
	labVip3->setHorizontalAlignment(TextHAlignment::LEFT);
	labVip3->enableOutline(Color4B::BLACK, 1);
	m_spVipBG->addChild(labVip3);

	//VIP时间
	m_labTime = LabelSystemFont("", 16);
	m_labTime->setPosition(Vec2(78, 131));
	m_labTime->enableOutline(Color4B::BLACK, 1);
	m_spVipBG->addChild(m_labTime);

	//VIP价格
	NetJadeCharge* ptrJade = trData->net_jade_charge[VIP_INDEX];
	auto labVipPrice = LabelSystemFont(StringUtils::format("%s%d", LL("rmb"), ptrJade->mRmb), 16);
	labVipPrice->setPosition(Vec2(78, 19));
	labVipPrice->enableOutline(Color4B::BLACK, 1);
	m_spVipBG->addChild(labVipPrice);

	//TableView
	map<int, int> mIndex;
	for(int i = 0; i < JADE_NUM; i++)
	{
		mIndex[i] = JADE_NUM - i;
	}
	m_tvJade = JadeTableView::create(mIndex, this);
	this->addChild(m_tvJade);

	//关闭
	auto ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(600+33, 358+9));
	spBG->addChild(ptrMenu);
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(BuyJadeLayer::callBackClose, this));
	ptrMenu->addChild(btnClose);
}

void BuyJadeLayer::updateVipLayer()
{
	if (history->m_iVIP)
	{
		m_labVip1->setVisible(true);
		m_labVip2->setVisible(false);
		m_labTime->setVisible(true);
	}
	else
	{
		m_labVip1->setVisible(false);
		m_labVip2->setVisible(true);
		m_labTime->setVisible(false);
	}
}

void BuyJadeLayer::updateVipState(float fDelay)
{
	if (m_bIsContinue)
	{
		m_bIsContinue = false;
		onHttpRequest(REQ_VIPTIME);
	}
}

void BuyJadeLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case REQ_VIPTIME:
	{
		strURL = "getCurVIPDate";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_VIPTIME";
	}
	break;
	case REQ_RECHARGE:
	{
		strURL = "getActivityMoney";
		string data = StringUtils::format("aid=%d&gameToken=%s", TOTAL_RECH_ACTID, strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"aid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", TOTAL_RECH_ACTID, strToken.c_str(), validCode.c_str());
		strTag = "REQ_RECHARGE";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(BuyJadeLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void BuyJadeLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		onHttpReqSucceed(value["result"], strTag);
	}
}

bool BuyJadeLayer::onHttpReqSucceed(Json::Value valRes, string strTag)
{
	if (strTag == "REQ_VIPTIME")
	{
		m_bIsContinue = true;
		if(!valRes["dueTime"].isNull())
		{
			double iTime1 = valRes["dueTime"].asDouble();
			if(iTime1 - history->m_pPlayerInfo->getVIPTime() > 100)
			{
				history->m_pPlayerInfo->setVIPTime(iTime1);
				unschedule(schedule_selector(BuyJadeLayer::updateVipState));
				m_pDelegate->updateMoneyShow(); //获取到更新数据后通知VIP层刷新
			}

			time_t time;
			struct tm *pTime;
			time = iTime1 / 1000 + 3600 * 8;
			pTime = gmtime(&time);
			char chDate[80];
			strftime(chDate, 80, "%Y-%m-%d", pTime);
			m_labTime->setString(chDate);
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
		}
	}
	else if (strTag == "REQ_RECHARGE")
	{
		int iMoney = valRes["money"].asInt();
		m_labRech->setString(StringUtils::format("%d", iMoney));
	}
	return true;
}
