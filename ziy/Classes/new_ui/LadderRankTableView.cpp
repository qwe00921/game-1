#include "LadderRankTableView.h"
#include "Defines.h"
#include "LadderRewardTableView.h"

LadderRankTableView* LadderRankTableView::create(Json::Value JsonValue, int iType)
{
	LadderRankTableView* pRet = new LadderRankTableView();
	if (pRet&&pRet->init(JsonValue,iType))
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

LadderRankTableView* LadderRankTableView::create(Json::Value JsonValue, int iCellType , int iType)
{
	LadderRankTableView* pRet = new LadderRankTableView();
	if (pRet&&pRet->init(JsonValue, iType , iCellType))
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

bool LadderRankTableView::init(Json::Value JsonValue, int iType ,int iCellType)
{
    if ( !Layer::init() )
    {
        return false;
    }
	mScore = 0;
	mRank = 0;
	mType = iType;
	mSelectMenu = NULL;
	m_iCellType = iCellType;

	getElemData(JsonValue, m_iCellType);
	addShowElem();
	DSize size = ChessViewTool::getInstance()->getFullSize();
	
	this->cell_num = mElemData.size();
	if (m_iCellType == TYPE_CHONGZHI)
		this->cell_num = mCZElemData.size();

	int iTBWidth = 430;

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(iTBWidth,248));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2((mBg->getContentSize().width - iTBWidth)/2,56));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    mBg->addChild(tableView);
    //加载tableview
    //tableView->reloadData();
    
    return true;
}

void LadderRankTableView::addShowElem()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	mBg = ReadSpriteName("new_ui/img_global/global_ph_bg1.png");
	mBg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(mBg);

	//积分榜
	if (mType == LADDER_RANK)
	{
		rankTabSp = ReadSpriteName("new_ui/img_global/global_ph_txt2.png");
		rankInfoSp = ReadSpriteName("new_ui/img_global/global_ph_txt1.png");
	}
	else
	{
		if (m_iCellType == TYPE_CHONGZHI)
		{
			rankTabSp = ReadSpriteName("new_ui/img_global/global_ph_txt10.png");
			rankInfoSp = ReadSpriteName("new_ui/img_global/global_ph_txt11.png");
		}
		else
		{
			rankTabSp = ReadSpriteName("new_ui/img_global/global_ph_txt4.png");
			rankInfoSp = ReadSpriteName("new_ui/img_global/global_ph_txt5.png");
		}

	}
	rankTabSp->setPosition(Vec2(296,353));
	mBg->addChild(rankTabSp);

	//info
	rankInfoSp->setAnchorPoint(Vec2(0,0.5));
	rankInfoSp->setPosition(Vec2((mBg->getContentSize().width - rankInfoSp->getContentSize().width)/ 2 - 14,315));
	mBg->addChild(rankInfoSp);


	if (mType == LADDER_RANK)
	{
		//76.36
		Label* selfLab = LabelSystemFont(LL("wodepaiming"), 16);
		selfLab->setAnchorPoint(Vec2(0, 0.5));
		selfLab->setPosition(Vec2(74, 41));
		selfLab->enableOutline(Color4B(180, 136, 80, 255));
		mBg->addChild(selfLab);

		//未上榜

		if (mRank > 0)
		{
			rankLab = LabelSystemFont(StringUtils::format(LL("didming"), mRank).c_str(), 16);
		}
		else
		{
			rankLab = LabelSystemFont(LL("yqmyw"), 16);
		}
		rankLab->setAnchorPoint(Vec2(0, 0.5));
		rankLab->setPosition(Vec2(78 + selfLab->getContentSize().width, 41));
		rankLab->setColor(Color3B(255, 255, 0));
		rankLab->enableOutline(Color4B(180, 136, 80, 255));
		mBg->addChild(rankLab);

		//奖励
		Menu* ptrjlMenu = Menu::create();
		ptrjlMenu->setPosition(Vec2(492, 351));
		mBg->addChild(ptrjlMenu);

		Sprite* jlMenuNSp = ReadSpriteName("new_ui/img_global/global_ph_icon5.png");
		Sprite* jlMenuSSp = ReadSpriteName("new_ui/img_global/global_ph_icon5.png");
		jlMenuSSp->setScale(0.9f);
		MenuItemSprite* jl_menu = MenuItemSprite::create(jlMenuNSp, jlMenuSSp, CC_CALLBACK_1(LadderRankTableView::clickRewardMenu, this));
		ptrjlMenu->addChild(jl_menu);
		jlMenuSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		jlMenuSSp->setPosition(Vec2(jlMenuSSp->getContentSize().width / 2, jlMenuSSp->getContentSize().height / 2));
	}
	else if (mType == WORLD_RANK)
	{
		Menu* ptrflMenu = Menu::create();
		ptrflMenu->setPosition(Vec2(mBg->getContentSize().width - 16, mBg->getContentSize().height/2));
		mBg->addChild(ptrflMenu);

		//战力
		Sprite* zlMenuNSp = ReadSpriteName("new_ui/img_global/global_ban19.png");
		Sprite* zlMenuSSp = ReadSpriteName("new_ui/img_global/global_ban20.png");
		Sprite* zlMenuDsSp = ReadSpriteName("new_ui/img_global/global_ban20.png");

		Sprite* zlMenuNtSp = ReadSpriteName("new_ui/img_text/imgtxt1_5.png");
		zlMenuNtSp->setPosition(Vec2(zlMenuNSp->getContentSize().width/2, zlMenuNSp->getContentSize().height/2));
		Sprite* zlMenuStSp = ReadSpriteName("new_ui/img_text/imgtxt1_6.png");
		zlMenuStSp->setPosition(Vec2(zlMenuSSp->getContentSize().width / 2, zlMenuSSp->getContentSize().height / 2));
		Sprite* zlMenuDtsSp = ReadSpriteName("new_ui/img_text/imgtxt1_6.png");
		zlMenuDtsSp->setPosition(Vec2(zlMenuDsSp->getContentSize().width / 2, zlMenuDsSp->getContentSize().height / 2));

		zlMenuNSp->addChild(zlMenuNtSp);
		zlMenuSSp->addChild(zlMenuStSp);
		zlMenuDsSp->addChild(zlMenuDtsSp);

		MenuItemSprite* zl_menu = MenuItemSprite::create(zlMenuNSp, zlMenuSSp, zlMenuDsSp,CC_CALLBACK_1(LadderRankTableView::clickZLRankMenu, this));
		zl_menu->setPosition(Vec2(0,60));
		ptrflMenu->addChild(zl_menu);
		zl_menu->setEnabled(false);
		mSelectMenu = zl_menu;

		//财力
		Sprite* clMenuNSp = ReadSpriteName("new_ui/img_global/global_ban19.png");
		Sprite* clMenuSSp = ReadSpriteName("new_ui/img_global/global_ban20.png");
		Sprite* clMenuDsSp = ReadSpriteName("new_ui/img_global/global_ban20.png");

		Sprite* clMenuNtSp = ReadSpriteName("new_ui/img_text/imgtxt1_7.png");
		clMenuNtSp->setPosition(Vec2(clMenuNSp->getContentSize().width / 2, clMenuNSp->getContentSize().height / 2));
		Sprite* clMenuStSp = ReadSpriteName("new_ui/img_text/imgtxt1_8.png");
		clMenuStSp->setPosition(Vec2(clMenuSSp->getContentSize().width / 2, clMenuSSp->getContentSize().height / 2));
		Sprite* clMenuDtsSp = ReadSpriteName("new_ui/img_text/imgtxt1_8.png");
		clMenuDtsSp->setPosition(Vec2(clMenuDsSp->getContentSize().width / 2, clMenuDsSp->getContentSize().height / 2));

		clMenuNSp->addChild(clMenuNtSp);
		clMenuSSp->addChild(clMenuStSp);
		clMenuDsSp->addChild(clMenuDtsSp);

		MenuItemSprite* cl_menu = MenuItemSprite::create(clMenuNSp, clMenuSSp, clMenuDsSp, CC_CALLBACK_1(LadderRankTableView::clickCLRankMenu, this));
		//cl_menu->setPosition(Vec2(0,-30));
		ptrflMenu->addChild(cl_menu);

		//通关
		Sprite* tgMenuNSp = ReadSpriteName("new_ui/img_global/global_ban19.png");
		Sprite* tgMenuSSp = ReadSpriteName("new_ui/img_global/global_ban20.png");
		Sprite* tgMenuDsSp = ReadSpriteName("new_ui/img_global/global_ban20.png");

		Sprite* tgMenuNtSp = ReadSpriteName("new_ui/img_text/imgtxt1_9.png");
		tgMenuNtSp->setPosition(Vec2(tgMenuNSp->getContentSize().width / 2, tgMenuNSp->getContentSize().height / 2));
		Sprite* tgMenuStSp = ReadSpriteName("new_ui/img_text/imgtxt1_10.png");
		tgMenuStSp->setPosition(Vec2(tgMenuSSp->getContentSize().width / 2, tgMenuSSp->getContentSize().height / 2));
		Sprite* tgMenuDtsSp = ReadSpriteName("new_ui/img_text/imgtxt1_10.png");
		tgMenuDtsSp->setPosition(Vec2(tgMenuDsSp->getContentSize().width / 2, tgMenuDsSp->getContentSize().height / 2));

		tgMenuNSp->addChild(tgMenuNtSp);
		tgMenuSSp->addChild(tgMenuStSp);
		tgMenuDsSp->addChild(tgMenuDtsSp);

		MenuItemSprite* tg_menu = MenuItemSprite::create(tgMenuNSp, tgMenuSSp, tgMenuDsSp, CC_CALLBACK_1(LadderRankTableView::clickTGRankMenu, this));
		tg_menu->setPosition(Vec2(0,-60));
		ptrflMenu->addChild(tg_menu);

		//未上榜
		if (mRankList[0] > 0)
		{
			rankLab = LabelSystemFont(StringUtils::format(LL("didming"), mRankList[0]).c_str(), 16);
		}
		else
		{
			rankLab = LabelSystemFont(__String::createWithFormat("100%s", LL("dmyw"))->getCString(), 16);
		}

		Label* selfLab = LabelSystemFont(LL("wodepaiming"), 16);
		selfLab->setAnchorPoint(Vec2(0, 0.5));
		selfLab->setPosition(Vec2(74, 38));
		selfLab->enableOutline(Color4B(180, 136, 80, 255));
		mBg->addChild(selfLab);

		rankLab->setAnchorPoint(Vec2(0, 0.5));
		rankLab->setPosition(Vec2(78 + selfLab->getContentSize().width, 38));
		rankLab->setColor(Color3B(255, 255, 0));
		rankLab->enableOutline(Color4B(180, 136, 80, 255));
		mBg->addChild(rankLab);

		//我的战力
		m_labCP = LabelSystemFont(LL("mytotalcp"), 16);
		m_labCP->setAnchorPoint(Vec2(0.0f, 0.5f));
		m_labCP->setPosition(Vec2(378, 38));
		m_labCP->enableOutline(Color4B(180, 136, 80, 255));
		mBg->addChild(m_labCP);

		auto labCPVal = LabelSystemFont(StringUtils::format("%d", history->m_pPlayerInfo->getTotalCP()), 16);
		labCPVal->setAnchorPoint(Vec2(0.0f, 0.5f));
		labCPVal->setPosition(Vec2(5 + m_labCP->getContentSize().width, m_labCP->getContentSize().height / 2));
		labCPVal->setColor(Color3B::YELLOW);
		labCPVal->enableOutline(Color4B(180, 136, 80, 255));
		m_labCP->addChild(labCPVal);

		if (m_iCellType == TYPE_CHONGZHI)
		{
			selfLab->setVisible(false);
			rankLab->setVisible(false);
			m_labCP->setVisible(false);
			labCPVal->setVisible(false);
			ptrflMenu->setEnabled(false);
			ptrflMenu->setVisible(false);

			auto labInfo = LabelSystemFont(LL("cjczxx"), 16);
			labInfo->setPosition(Vec2(mBg->getContentSize().width/2, 38));
			labInfo->setColor(Color3B::YELLOW);
			mBg->addChild(labInfo);
		}
	}

	//550,367
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(552, 367));
	mBg->addChild(ptrMenu);

	Sprite* closeMenuNSp = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeMenuSSp = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeMenuNSp, closeMenuSSp, CC_CALLBACK_1(LadderRankTableView::clickCloseMenu, this));
	ptrMenu->addChild(close_menu);
}

void LadderRankTableView::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void LadderRankTableView::clickRewardMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(LadderRewardTableView::create());
}

void LadderRankTableView::clickZLRankMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	mSelectMenu->setEnabled(true);
	mSelectMenu = (MenuItemSprite*)sender;
	mSelectMenu->setEnabled(false);

	rankTabSp->setTexture(ReadSpriteTexture("new_ui/img_global/global_ph_txt4.png"));
	rankInfoSp->setTexture(ReadSpriteTexture("new_ui/img_global/global_ph_txt5.png"));

	mRankOffset[m_iCellType] = tableView->getContentOffset().y;
	ReLoadTableView(TYPE_NOMAL);
}

void LadderRankTableView::clickCLRankMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	mSelectMenu->setEnabled(true);
	mSelectMenu = (MenuItemSprite*)sender;
	mSelectMenu->setEnabled(false);

	rankTabSp->setTexture(ReadSpriteTexture("new_ui/img_global/global_ph_txt6.png"));
	rankInfoSp->setTexture(ReadSpriteTexture("new_ui/img_global/global_ph_txt7.png"));

	mRankOffset[m_iCellType] = tableView->getContentOffset().y;

	if (!mCLElemData.size())
	{
		SendRankData(TYPE_CAILI);
	}
	else
	{
		ReLoadTableView(TYPE_CAILI);
	}
}
void LadderRankTableView::clickTGRankMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	mSelectMenu->setEnabled(true);
	mSelectMenu = (MenuItemSprite*)sender;
	mSelectMenu->setEnabled(false);

	rankTabSp->setTexture(ReadSpriteTexture("new_ui/img_global/global_ph_txt8.png"));
	rankInfoSp->setTexture(ReadSpriteTexture("new_ui/img_global/global_ph_txt9.png"));

	mRankOffset[m_iCellType] = tableView->getContentOffset().y;

	if (!mTGElemData.size())
	{
		SendRankData(TYPE_TONGGUAN);
	}
	else
	{
		ReLoadTableView(TYPE_TONGGUAN);
	}
}

void LadderRankTableView::getElemData(Json::Value JsonValue, int iType)
{
	Json::Value JsonRank = JsonValue["ranks"];

	if (JsonRank.isArray())
	{
		string heroNameS = history->m_pPlayerInfo->getName();
		int iGender = 0;
		map<int, int> meFashion;
		for (UInt i = 0; i < JsonRank.size(); i++)
		{
			meFashion.clear();
			int iGender = 0, iFashion1 = 0, iFashion2 = 0, iFashion3 = 0, iFashion4 = 0,iRank = 0,iScore = 0, iIsVip = 0,iLevel;
			string nameS = "";

			ParseInt(iGender, JsonRank[i]["gender"]);
			ParseInt(iFashion1, JsonRank[i]["Fashion1"]);
			ParseInt(iFashion2, JsonRank[i]["Fashion2"]);
			ParseInt(iFashion3, JsonRank[i]["Fashion3"]);
			ParseInt(iFashion4, JsonRank[i]["Fashion4"]);
			ParseInt(iRank, JsonRank[i]["rank"]);
			ParseInt(iScore, JsonRank[i]["score"]);
			ParseInt(iIsVip, JsonRank[i]["isVip"]);
			ParseInt(iLevel, JsonRank[i]["lv"]);
			ParseString(nameS, JsonRank[i]["name"]);

			meFashion[1] = iGender + 1;
			meFashion[2] = iFashion1;
			meFashion[3] = 100;
			meFashion[4] = iFashion2;
			if (iFashion3 > 0)
				meFashion[2] = iFashion3;
			if (iFashion4 > 0)
				meFashion[4] = iFashion4;


			LadderRankData* ptrLadderRankData = new LadderRankData();
			ptrLadderRankData->seteLEVEL(iLevel);
			ptrLadderRankData->seteNAME(nameS.c_str());
			ptrLadderRankData->seteRANK(iRank);
			ptrLadderRankData->seteSCORE(iScore);
			ptrLadderRankData->seteISVIP(iIsVip);
			ptrLadderRankData->mFashion = meFashion;

			if (iType == TYPE_CAILI)
				mCLElemData[i] = ptrLadderRankData;
			else if (iType == TYPE_TONGGUAN)
				mTGElemData[i] = ptrLadderRankData;
			else if (iType == TYPE_CHONGZHI)
				mCZElemData[i] = ptrLadderRankData;
			else
				mElemData[i] = ptrLadderRankData;

			if (mType == WORLD_RANK&&nameS == heroNameS)
			{
				mRankList[iType] = iRank;
			}
		}
	}

	if (JsonValue["myScore"].isInt())
	{
		mScore = JsonValue["myScore"].asInt();
	}
	if (JsonValue["myRank"].isInt())
	{
		mRank = JsonValue["myRank"].asInt();
	}
}

LadderRankData* LadderRankTableView::GetCellData(int iDx)
{
	if (m_iCellType == TYPE_CAILI)
		return mCLElemData[iDx];
	else if(m_iCellType == TYPE_TONGGUAN)
		return mTGElemData[iDx];
	else if (m_iCellType == TYPE_CHONGZHI)
		return mCZElemData[iDx];
	else
		return mElemData[iDx];
}

void LadderRankTableView::ReLoadTableView(int iType)
{
	m_iCellType = iType;
	map<int, LadderRankData*>* ptrElemDate = NULL;

	if (iType == TYPE_CAILI)
		ptrElemDate = &mCLElemData;
	else if (iType == TYPE_TONGGUAN)
		ptrElemDate = &mTGElemData;
	else if (iType == TYPE_CHONGZHI)
		ptrElemDate = &mCZElemData;
	else
		ptrElemDate = &mElemData;
	this->cell_num = ptrElemDate->size();

	if (mRankList[iType] > 0)
	{
		rankLab->setString(StringUtils::format(LL("didming"), mRankList[iType]).c_str());
	}
	else
	{
		rankLab->setString(__String::createWithFormat("100%s", LL("dmyw"))->getCString());
	}

	m_labCP->setVisible(iType == TYPE_NOMAL);

	tableView->reloadData();
	if(mRankOffset.count(iType))
		tableView->setContentOffset(Vec2(0, mRankOffset[iType]));
}

void LadderRankTableView::SendRankData(int iType)
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	if (iType == TYPE_CAILI)
	{
		ITools::getInstance()->connectingNetwork("getJSRank",
			HttpRequest::Type::POST,
			this,
			callfuncND_selector(LadderRankTableView::onHttpRequestCompleted),
			data->getCString(),
			ITools::getInstance()->mHead,
			"Post_SendJSRankData");
		ITools::getInstance()->addWaitLayer();
	}
	else if (iType == TYPE_TONGGUAN)
	{
		ITools::getInstance()->connectingNetwork("getWCRank",
			HttpRequest::Type::POST,
			this,
			callfuncND_selector(LadderRankTableView::onHttpRequestCompleted),
			data->getCString(),
			ITools::getInstance()->mHead,
			"Post_SendWCRankData");

		ITools::getInstance()->addWaitLayer();
	}
}

void LadderRankTableView::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (strTag == "Post_SendJSRankData")
		{
			getElemData(value["result"],TYPE_CAILI);
			ReLoadTableView(TYPE_CAILI);
		}
		else if(strTag == "Post_SendWCRankData")
		{
			getElemData(value["result"], TYPE_TONGGUAN);
			ReLoadTableView(TYPE_TONGGUAN);
		}
	}
}

LadderRankTableView::LadderRankTableView()
{
	cell_num = 1;
}

LadderRankTableView::~LadderRankTableView()
{
	for (map<int, LadderRankData*>::iterator eIt = mElemData.begin(); eIt != mElemData.end(); eIt++)
	{
		delete eIt->second;
	}
	for (map<int, LadderRankData*>::iterator eIt = mCLElemData.begin(); eIt != mCLElemData.end(); eIt++)
	{
		delete eIt->second;
	}
	for (map<int, LadderRankData*>::iterator eIt = mTGElemData.begin(); eIt != mTGElemData.end(); eIt++)
	{
		delete eIt->second;
	}
	for (map<int, LadderRankData*>::iterator eIt = mCZElemData.begin(); eIt != mCZElemData.end(); eIt++)
	{
		delete eIt->second;
	}
}

void LadderRankTableView::scrollViewDidScroll(ScrollView* view)
{

}

void LadderRankTableView::scrollViewDidZoom(ScrollView* view)
{

}

void LadderRankTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void LadderRankTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void LadderRankTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size LadderRankTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(430,62);
}

TableViewCell* LadderRankTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = LadderRankCell::create(GetCellData(idx));
    }
    else
    {
		LadderRankCell* c = (LadderRankCell*)cell;
		c->UpDateCell(GetCellData(idx));
    }
    return cell;
}

ssize_t LadderRankTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

/////////////////////////////////////////////////////////////
LadderRankCell* LadderRankCell::create(LadderRankData* ptrLRData)
{
	LadderRankCell* pRet = new LadderRankCell();
	if (pRet&&pRet->init(ptrLRData))
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

bool LadderRankCell::init(LadderRankData* ptrLRData)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	m_iCellWidth = 430;
	m_iCellHeight = 62;
	mNumSp = NULL;

	bg = ReadSpriteName("new_ui/img_global/global_ph_bg2.png");
	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;
	bg->setPosition(Vec2(m_iCellWidth / 2, m_iCellHeight / 2));
	this->addChild(bg);

	GetNumSprite(bg, ptrLRData->geteRANK());

	//78,
	vipSp = ReadSpriteName("new_ui/main/vip1.png");
	vipSp->setPosition(Vec2(78, bg->getContentSize().height/2));
	bg->addChild(vipSp);

	int iFashion1 = ptrLRData->mFashion[2];
	int iFashion2 = ptrLRData->mFashion[4];
	int iGender = ptrLRData->mFashion[1];

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	int iOrder = trData->net_hero_dress[iFashion2]->mMov[6];

	spEquip = ReadSpriteNormal(strEquip->getCString());
	Size weaponSize = spEquip->getContentSize();

	spEquip->setPosition(Vec2(130, bg->getContentSize().height/2));
	spEquip->setTextureRect(Rect(0, weaponSize.width * 6, weaponSize.width, weaponSize.width));
	bg->addChild(spEquip, iOrder);

	spHero = ReadSpriteNormal(strHero->getCString());
	Size heroSize = spHero->getContentSize();
	spHero->setPosition(spEquip->getPosition());
	spHero->setTextureRect(Rect(0, heroSize.width * 6, heroSize.width, heroSize.width));
	bg->addChild(spHero,1);

	//姓名,等级
	nameLab = LabelSystemFont(ptrLRData->geteNAME().c_str(),16);
	nameLab->setColor(Color3B(102,0,0));
	nameLab->setAnchorPoint(Vec2(0,0));
	nameLab->setPosition(Vec2(158, bg->getContentSize().height / 2));
	bg->addChild(nameLab);

	levelLab = LabelSystemFont(StringUtils::format("%s%d",LL("dengji"),ptrLRData->geteLEVEL()).c_str(), 16);
	levelLab->setColor(Color3B(102, 51, 0));
	levelLab->setAnchorPoint(Vec2(0, 1));
	levelLab->setPosition(Vec2(158, bg->getContentSize().height / 2));
	bg->addChild(levelLab);

	//分数
	scoreLab = LabelSystemFont(IntToString(ptrLRData->geteSCORE()),16);
	scoreLab->setPosition(Vec2(366, bg->getContentSize().height / 2));
	scoreLab->setColor(Color3B(102, 0, 0));
	bg->addChild(scoreLab);
	
	UpDateCell(ptrLRData);
	return true;
}
LadderRankCell::LadderRankCell()
{

}

void LadderRankCell::GetNumSprite(Sprite* bgSp,int iNum)
{
	if (mNumSp)
		mNumSp->removeFromParent();
	mNumSp = NULL;
	int iNumWidth = 0;
	if (iNum > 0 && iNum < 4)
	{
		string rankS = StringUtils::format("new_ui/img_global/global_ph_icon%d.png", iNum);
		mNumSp = ReadSpriteName(rankS.c_str());
	}
	else
	{
		map<int, int> numarr;
		int iNumIndex = 0;
		do
		{
			numarr[iNumIndex++] = iNum % 10;
			iNum = iNum / 10;

		} while (iNum > 0);

		for (int i = iNumIndex - 1; i >= 0; i--)
		{
			if (!mNumSp)
				mNumSp = ReadSpriteName(StringUtils::format("new_ui/img_number/num10_%d.png", numarr[i]).c_str());
			else
			{
				Sprite* numSp = ReadSpriteName(StringUtils::format("new_ui/img_number/num10_%d.png", numarr[i]).c_str());
				iNumWidth += numSp->getContentSize().width;
				numSp->setAnchorPoint(Vec2(0,0));
				numSp->setPosition(Vec2(iNumWidth,0));
				mNumSp->addChild(numSp);
			}
		}
	}

	int iNumPX = 35 - (mNumSp->getContentSize().width + iNumWidth)/2;
	mNumSp->setAnchorPoint(Vec2(0,0.5));
	mNumSp->setPosition(Vec2(iNumPX,bgSp->getContentSize().height/2));
	bgSp->addChild(mNumSp);
}

void LadderRankCell::UpDateCell(LadderRankData* ptrLRData)
{
	GetNumSprite(bg, ptrLRData->geteRANK());

	int iFashion1 = ptrLRData->mFashion[2];
	int iFashion2 = ptrLRData->mFashion[4];
	int iGender = ptrLRData->mFashion[1];

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	int iOrder = 0;
	if (trData->net_hero_dress[iFashion2]->mMov[6])
	{
		iOrder = 2;
	}

	Texture2D* ptrTexture2D = GetSpriteTexture(strEquip->getCString());
	Size weaponSize = ptrTexture2D->getContentSize();
	spEquip->setTexture(ptrTexture2D);
	spEquip->setTextureRect(Rect(0, weaponSize.width * 6, weaponSize.width, weaponSize.width));
	spEquip->setLocalZOrder(iOrder);

	ptrTexture2D = GetSpriteTexture(strHero->getCString());
	Size heroSize = ptrTexture2D->getContentSize();
	spHero->setTexture(ptrTexture2D);
	spHero->setTextureRect(Rect(0, heroSize.width * 6, heroSize.width, heroSize.width));

	nameLab->setString(ptrLRData->geteNAME().c_str());

	levelLab->setString(StringUtils::format("%s%d", LL("dengji"), ptrLRData->geteLEVEL()).c_str());

	scoreLab->setString(IntToString(ptrLRData->geteSCORE()));

	if (ptrLRData->geteISVIP())
		vipSp->setVisible(true);
	else
		vipSp->setVisible(false);
}
