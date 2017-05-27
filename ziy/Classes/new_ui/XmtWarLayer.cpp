#include "XmtWarLayer.h"
#include "Defines.h"
#include "Net_HomePage.h"
#include "DispatchLayer.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "BattleTitleLayer.h"
#include "PopLayer.h"

XmtWarElemLayer* XmtWarElemLayer::create(int iCurWar, int iCurStage, EventDelegate* ptrEventDelegate)
{
	XmtWarElemLayer* pRet = new XmtWarElemLayer();
	if (pRet&&pRet->init(iCurWar,iCurStage,ptrEventDelegate))
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

bool XmtWarElemLayer::init(int iCurWar, int iCurStage, EventDelegate* ptrEventDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	this->setSwallow(false);
	
	mSize = ChessViewTool::getInstance()->getFullSize();

	xmtNode = Node::create();
	xmtNode->setPosition(Vec2(190, 0));
	this->addChild(xmtNode);

	m_iStage = iCurStage;
	m_iSignWarId = -1;
	m_iXmtOffsetY = 0;
	m_iSelectIndex = -1;
	m_iFloorHeight = 95;
	m_iClickIndex = -1;
	m_iClickChestIndex = -1;
	mPromptTxtIndex = -1;
	mReceiveChestIndex = -1;

	if(!m_iStage)
		GetCurStage(iCurWar);

	GetElemData(iCurWar);

	int iLWidth = 269 + m_iFloorHeight* mShowList.size();
	if (iLWidth < mSize.height)
		iLWidth = mSize.height;
	this->setContentSize(Size(mSize.width, iLWidth));

	if (iLWidth > mSize.height && m_iSelectIndex > -1&&ptrEventDelegate)
	{
		int iOffsetY = mSize.height / 2 - (137 + m_iSelectIndex*m_iFloorHeight + 32);
		if (iOffsetY < 0)
		{
			if (iOffsetY < mSize.height - iLWidth)
			{
				iOffsetY = mSize.height - iLWidth;
			}

			ptrEventDelegate->RunFunction(iOffsetY);
		}
	}

	auto ptrArmaBottom = Armature::create("xmt_ani");
	ptrArmaBottom->getAnimation()->play("xmt3");
	ptrArmaBottom->setPosition(Vec2(0,0));
	xmtNode->addChild(ptrArmaBottom);

	int iPointX = ptrArmaBottom->getContentSize().width/2;
	int iPointY = 137;

	auto ptrArmaTop = Armature::create("xmt_ani");
	ptrArmaTop->getAnimation()->play("xmt1");
	ptrArmaTop->setPosition(Vec2(0, iPointY + m_iFloorHeight* mShowList.size()));
	xmtNode->addChild(ptrArmaTop);

	return true;
}


XmtWarElemLayer::XmtWarElemLayer()
{

}

XmtWarElemLayer::~XmtWarElemLayer()
{
	for (map<int, XmtWarElem*>::iterator eIt = mShowList.begin(); eIt != mShowList.end(); eIt++)
	{
		delete eIt->second;
	}
}

void XmtWarElemLayer::GetCurStage(int iCurWar)
{
	m_iStage = 0;
	map<int, int> mBattleWarList = history->mMapDateInfo->GetBattleImageData()->GetAllBattleData();

	if (mBattleWarList.size()&&trData->m_mNetWarInfo.count(mBattleWarList.begin()->first))
	{
		m_iStage = trData->m_mNetWarInfo[mBattleWarList.begin()->first]->iDegree;
	}
}

void XmtWarElemLayer::GetElemData(int iCurWar)
{
	map<int, map<int, int> > eAllWarList;
	for (map<int, NetWarInfo*>::iterator eIt = trData->m_mNetWarInfo.begin(); eIt != trData->m_mNetWarInfo.end(); eIt++)
	{
		if (eIt->second->iType == BATTLE_IMAGE)
		{
			eAllWarList[eIt->second->iDegree][eIt->second->iWarId] = 1;
		}
	}

	map<int, int> eWarList;
	if (eAllWarList.count(m_iStage))
	{
		eWarList = eAllWarList[m_iStage];
	}
	else
	{
		eWarList = eAllWarList.begin()->second;
		m_iStage = eAllWarList.begin()->first;
	}


	int iPointY = 137;
	int iShowIndex = 0;
	int iCurSelectIndex = -1;
	NetWarInfo* ptrNetWarInfo = NULL;

	map<int, int> mBattleWarList = history->mMapDateInfo->GetBattleImageData()->GetAllBattleData();
	map<int, int> mChestList = history->mMapDateInfo->GetBattleImageData()->GetTreasures();
	int iCurBattleWar = history->mMapDateInfo->GetBattleImageData()->GetCurBattleWar(m_iStage);

	for (map<int, int>::iterator eIt = eWarList.begin(); eIt != eWarList.end(); eIt++)
	{
		XmtWarElem* ptrXmtWarElem = new XmtWarElem();

		ptrNetWarInfo = trData->m_mNetWarInfo[eIt->first];

		auto ptrArmature = Armature::create("xmt_ani");
		ptrArmature->getAnimation()->play("xmt2");
		ptrArmature->setPosition(Vec2(0, iPointY));
		xmtNode->addChild(ptrArmature);

		Sprite* eBgSp = ReadSpriteName("new_ui/xmt/xmt_png3.png");
		eBgSp->setPosition(Vec2(0, iPointY));
		eBgSp->setAnchorPoint(Vec2(0.5,0));
		xmtNode->addChild(eBgSp);

		Sprite* eHeadImg = ReadSpriteName(StringUtils::format("new_ui/xmt/xmt_icon%d.png", ptrNetWarInfo->iImgId).c_str());
		eHeadImg->setAnchorPoint(Vec2(0,0.5));
		eHeadImg->setPosition(Vec2(2,eBgSp->getContentSize().height/2));
		eBgSp->addChild(eHeadImg);

		Label* eName = LabelSystemFont(ptrNetWarInfo->strName.c_str(),18);
		eName->setColor(Color3B::YELLOW);
		eName->setPosition(Vec2(88, eBgSp->getContentSize().height / 2));
		eName->enableOutline(Color4B(102,51,0,255),1);
		eBgSp->addChild(eName);

		Sprite* LockSp = ReadSpriteName("new_ui/xmt/xmt_png2.png");
		LockSp->setPosition(Vec2(eBgSp->getContentSize().width/2, eBgSp->getContentSize().height/2));
		eBgSp->addChild(LockSp);
		LockSp->setVisible(false);

		Sprite* isPassSp = ReadSpriteName("new_ui/xmt/xmt_png1.png");
		isPassSp->setPosition(Vec2(eBgSp->getContentSize().width / 2, eBgSp->getContentSize().height / 2));
		eBgSp->addChild(isPassSp);
		isPassSp->setVisible(false);

		Sprite* isSelect = ReadSpriteName("new_ui/xmt/xmt_png4.png");
		isSelect->setPosition(Vec2(eBgSp->getContentSize().width / 2, eBgSp->getContentSize().height / 2));
		eBgSp->addChild(isSelect);
		isSelect->setVisible(false);

		int iStar = history->mMapDateInfo->GetBattleImageData()->GetXMTWarStar(eIt->first, m_iStage);
		Sprite* chestSp = NULL;
		if (mChestList.count(iStar))
			chestSp = ReadSpriteName("new_ui/xmt/xmt_xz2.png");
		else
			chestSp = ReadSpriteName("new_ui/xmt/xmt_xz1.png");

		chestSp->setPosition(Vec2(203, eBgSp->getContentSize().height / 2 - 7));
		eBgSp->addChild(chestSp);
		addSpriteGray(chestSp);

		ptrXmtWarElem->setEARMATURE(ptrArmature);
		ptrXmtWarElem->setEBG(eBgSp);
		ptrXmtWarElem->setESELECT(isSelect);
		ptrXmtWarElem->setEWARID(eIt->first);
		ptrXmtWarElem->setECHEST(chestSp);
		ptrXmtWarElem->setESTARNUM(iStar);
		ptrXmtWarElem->setEISPASS(false);
		ptrXmtWarElem->setEISENABLE(false);

		if (mBattleWarList.count(eIt->first))
		{
			ptrXmtWarElem->setEISPASS(true);
			isPassSp->setVisible(true);
			removeSpriteGray(chestSp);
		}
		else if (iCurBattleWar == eIt->first)
		{
			ptrXmtWarElem->setEISENABLE(true);

			if(iCurWar<=0)  iCurSelectIndex = iShowIndex;
		}
		else
		{
			LockSp->setVisible(true);
		}

		if (iCurWar == eIt->first&&iCurSelectIndex < 0)
		{
			iCurSelectIndex = iShowIndex;
			isSelect->setVisible(true);
		}


		iPointY += m_iFloorHeight;

		mShowList[iShowIndex++] = ptrXmtWarElem;
	}


	m_iSelectIndex = iCurSelectIndex;
	if (m_iSelectIndex > -1)
		mShowList[m_iSelectIndex]->getESELECT()->setVisible(true);
}

bool XmtWarElemLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_iClickIndex = -1;
	m_iClickChestIndex = -1;

	playSound(SOUND_LEFT_MOUSE_CLICK);

	for (size_t i = 0; i < mShowList.size(); i++)
	{
		Rect eBgBox = mShowList[i]->getEBG()->getBoundingBox();
		Rect eChestBox = mShowList[i]->getECHEST()->getBoundingBox();
		if (Rect(0, 0, eBgBox.size.width, eBgBox.size.height).containsPoint(mShowList[i]->getEBG()->convertToNodeSpace(touch->getLocation())))
		{
			m_iClickIndex = i;

			m_iSelectIndex = i;
			return true;
		}
		else if (Rect(0, 0, eChestBox.size.width, eChestBox.size.height).containsPoint(mShowList[i]->getECHEST()->convertToNodeSpace(touch->getLocation())))
		{
			m_iClickChestIndex = i;

			ScaleTo* ptrScaleTo = ScaleTo::create(0.1f, 0.9f);
			mShowList[i]->getECHEST()->runAction(ptrScaleTo);
			return true;
		}
	}

	return true;
}

void XmtWarElemLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void XmtWarElemLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if ((m_iClickIndex > -1|| m_iClickChestIndex > -1) && abs((int)(touch->getLocation().y - touch->getStartLocation().y))<3)
	{
		if (m_iClickIndex > -1&&mShowList[m_iSelectIndex]->getEISENABLE())
		{
			history->m_iBattleType = BATTLE_IMAGE;
			history->m_iCurWarId = mShowList[m_iSelectIndex]->getEWARID();
			history->m_iBattleStage = m_iStage;
			ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
		}

		if (m_iClickChestIndex > -1)
		{
			int iChestState = 0;
			int iStarNum = mShowList[m_iClickChestIndex]->getESTARNUM();
			map<int, int> eTreasuresList = history->mMapDateInfo->GetBattleImageData()->GetTreasures();
			if (mShowList[m_iClickChestIndex]->getEISPASS() && !eTreasuresList.count(iStarNum))
				iChestState = 1;
			ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_GLC_CHEST, this, iChestState));
		}
	}

	if (m_iClickChestIndex > -1)
	{
		ScaleTo* ptrScaleTo = ScaleTo::create(0.1f, 1);
		mShowList[m_iClickChestIndex]->getECHEST()->runAction(ptrScaleTo);
	}
}

void XmtWarElemLayer::SetPopLayerElement(Sprite* bgSp)
{
	int iBgWidth = bgSp->getContentSize().width;
	int iBgHeight = bgSp->getContentSize().height;

	Sprite* tgjlSp = ReadSpriteName("new_ui/daily/stalker_txt4.png");
	tgjlSp->setAnchorPoint(Vec2(0.5, 1));
	tgjlSp->setPosition(Vec2(iBgWidth / 2, iBgHeight));
	bgSp->addChild(tgjlSp);

	if (m_iClickChestIndex > -1)
	{
		NetMapChestPtr ptrNMC = trData->net_map_chest[MAPID_SCENE_IMAGE][mShowList[m_iClickChestIndex]->getESTARNUM()];
		map<int, map<int, int> > eGoodsIdList;

		int eGoodsListIndex = 0;
		int item_num = 0;
		for (int i = 0; i < 3; i++)
		{
			if (i == 0)
				item_num = ptrNMC->jade;
			else if (i == 1)
				item_num = ptrNMC->gold;
			else
				item_num = ptrNMC->improvedStone;

			if (item_num == 0) continue;

			eGoodsIdList[eGoodsListIndex][0] = 10000 + i;
			eGoodsIdList[eGoodsListIndex][1] = item_num;
			eGoodsListIndex++;
		}

		int iGoodsId = 0, iCount = 0;
		for (map<int, int>::iterator ititem = ptrNMC->itemid.begin(); ititem != ptrNMC->itemid.end(); ititem++)
		{
			iGoodsId = ititem->first;
			iCount = ititem->second;

			if (!iGoodsId)  continue;

			eGoodsIdList[eGoodsListIndex][0] = iGoodsId;
			eGoodsIdList[eGoodsListIndex][1] = iCount;
			eGoodsListIndex++;
		}

		if (eGoodsListIndex > 4) eGoodsListIndex = 4;
		Sprite* eGoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		int ieGoodsBgWidth = eGoodsBG->getContentSize().width;
		int iPointX = (iBgWidth - ieGoodsBgWidth*eGoodsListIndex - 10 * (eGoodsListIndex - 1)) / 2 + ieGoodsBgWidth / 2;

		for (int i = 0; i < eGoodsListIndex; i++)
		{
			iGoodsId = eGoodsIdList[i][0];
			iCount = eGoodsIdList[i][1];

			Sprite* GoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
			GoodsBG->setPosition(Vec2(iPointX + i*(ieGoodsBgWidth + 10), iBgHeight / 2 + 23));
			bgSp->addChild(GoodsBG);

			//物品ICON
			Sprite* spIcon = history->getGoodsSprite(iGoodsId);
			spIcon->setPosition(Vec2(33, 33));
			GoodsBG->addChild(spIcon);

			//数目
			if (iCount > 1)
			{
				Sprite* spNum = ITools::getInstance()->getNumSprite(iCount, true);
				spNum->setAnchorPoint(Vec2(1, 0));
				spNum->setPosition(Vec2(62, 4));
				GoodsBG->addChild(spNum);
			}
		}
	}
}

void XmtWarElemLayer::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if (trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
	{
		history->game_state = GAME_STATE_REDE;
		Chess::getInstance()->appendScreen(new RedeScreen());
	}
	else
	{
		history->game_state = GAME_STATE_BATTLE;
		history->mTitleLayer = BattleTitleLayer::create();
		ChessApplication::getInstance()->AddFunctionLayer(history->mTitleLayer);
		Chess::getInstance()->appendScreen(new BattleScreen());
	}
}

void XmtWarElemLayer::RunFunction(int iNomal)
{
	if (m_iClickChestIndex > -1)
	{
		mReceiveChestIndex = m_iClickChestIndex;
		SendNetDate(MAPID_SCENE_IMAGE, mShowList[mReceiveChestIndex]->getESTARNUM());
	}
}

void XmtWarElemLayer::SendNetDate(int iMapId, int iStar)
{
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("mapID=%d&star=%d&gameToken=%s", iMapId, iStar, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"mapID\":%d,\"star\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iMapId, iStar, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("openTreasures",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(XmtWarElemLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);
}

void XmtWarElemLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (value["result"].isObject())
		{
			setNetDate(value["result"]);
		}
	}
}

void XmtWarElemLayer::setNetDate(Json::Value jsonValue)
{
	mPromptTxt.clear();
	mPromptTxt = history->onHttpGoodsAdd(jsonValue);
	mPromptTxtIndex = 0;
	this->schedule(schedule_selector(XmtWarElemLayer::showPrompt), 0.8f, mPromptTxt.size() - 1, 0);

	history->mMapDateInfo->GetBattleImageData()->SetTreasures(mShowList[mReceiveChestIndex]->getESTARNUM(), 1);
	mShowList[mReceiveChestIndex]->getECHEST()->setTexture(ReadSpriteTexture("new_ui/xmt/xmt_xz2.png"));
}

void XmtWarElemLayer::showPrompt(float f)
{
	ITools::getInstance()->createGlobalPrompt(mPromptTxt[mPromptTxtIndex++], true);
}

///////////////////////////////////////////

XmtWarLayer* XmtWarLayer::create(int iCurWar, int iCurStage)
{
	XmtWarLayer* pRet = new XmtWarLayer();
	if (pRet&&pRet->init(iCurWar, iCurStage))
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

bool XmtWarLayer::init(int iCurWar, int iCurStage)
{
	if (!Layer::init())
	{
		return false;
	}

	ChessPlayMp3(104);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/xmt_ani.ExportJson", 0, 1));
	DSize mSize = ChessViewTool::getInstance()->getFullSize();
	m_iOffsetY = 0;

	Sprite* bg = ReadSpriteName("new_ui/xmt/xmt_bgs.jpg");
	bg->setAnchorPoint(Vec2(0.5f, 0.5f));
	bg->setPosition(Vec2(mSize.width / 2, mSize.height / 2));
	this->addChild(bg);

	auto ptrArmaBg = Armature::create("xmt_ani");
	ptrArmaBg->getAnimation()->play("xmt_txt1");
	ptrArmaBg->setPosition(Vec2(mSize.width / 2, mSize.height / 2));
	this->addChild(ptrArmaBg);

	ScrollView* ptrScrollView = ScrollView::create(Size(mSize.width, mSize.height), XmtWarElemLayer::create(iCurWar, iCurStage,this));
	ptrScrollView->setBounceable(false);
	ptrScrollView->setDirection(ScrollView::Direction::VERTICAL);
	ptrScrollView->setDelegate(this);
	ptrScrollView->setTouchEnabled(true);
	ptrScrollView->setAnchorPoint(Vec2(0, 0));
	ptrScrollView->setPosition(Vec2(0, 0));
	this->addChild(ptrScrollView);

	if (m_iOffsetY)
		ptrScrollView->setContentOffset(Vec2(0,m_iOffsetY));

	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(XmtWarLayer::clickMenuBack, this));

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8), mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mMenu, 1);
	mMenu->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5, 0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width / 2, mb_select->getContentSize().height / 2));

	Sprite* titleSp = ReadSpriteName("new_ui/img_global/global_ph_txt3.png");
	titleSp->setAnchorPoint(Vec2(1, 0));
	titleSp->setPosition(Vec2(mSize.width - 4,2));
	this->addChild(titleSp);

	return true;
}

void XmtWarLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessPlayMp3(MUSIC_HOME);
	this->removeFromParent();
}

void XmtWarLayer::RunFunction(int iNomal)
{
	m_iOffsetY = iNomal;
}

XmtWarLayer::XmtWarLayer()
{

}

XmtWarLayer::~XmtWarLayer()
{

}

//////////////////////////////////


#define DAILY_VIEW_W 408
#define DAILY_VIEW_H 100
#define DAILY_CELL_W 136
#define DAILY_CELL_H 100

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool XmtWarCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}
	m_spStage = NULL;
	return true;
}

void XmtWarCell::updateCell(int iStage, int iEnable)
{
	string strPath = StringUtils::format("new_ui/daily/xmt_icon%d.png", iStage);
	if (!m_spStage)
	{
		m_spStage = ReadSpriteName(strPath.c_str());
		m_spStage->setPosition(Vec2(DAILY_CELL_W / 2, DAILY_CELL_H / 2));
		this->addChild(m_spStage);
	}
	else
	{
		m_spStage->setTexture(ReadSpriteTexture(strPath.c_str()));
	}
	

	if (iEnable)
		removeSpriteGray(m_spStage);
	else
		addSpriteGray(m_spStage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool XmtWarTableView::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_iClickIndex = -1;
	m_spLArrow = NULL;
	m_spRArrow = NULL;

	DSizeF size = ChessViewTool::getInstance()->getFullSize();

	GetAllStage();
	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/daily/daily_bg1.png");
	spBG->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(spBG);

	//标题
	Sprite* spTitle = ReadSpriteName("new_ui/daily/daily_txt5.png");
	spTitle->setPosition(Vec2(size.width / 2, size.height / 2 + 124));
	this->addChild(spTitle);

	Label* title1Lab = LabelSystemFont(LL("xmjmxfjf"),16);
	title1Lab->setPosition(Vec2(spBG->getContentSize().width/2,230));
	title1Lab->enableOutline(Color4B(102, 50, 0, 255), 1);
	spBG->addChild(title1Lab);

	Label* title2Lab = LabelSystemFont(LL("mzkfskxztzms"), 16);
	title2Lab->setPosition(Vec2(spBG->getContentSize().width / 2, 46));
	title2Lab->enableOutline(Color4B(102, 50, 0, 255), 1);
	spBG->addChild(title2Lab);

	//创建一个talbleview 将datasource设置为当前的类的对象
	m_pTableView = TableView::create(this, Size(DAILY_VIEW_W, DAILY_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::HORIZONTAL);
	//设置位置
	m_pTableView->setPosition(Vec2(spBG->getContentSize().width / 2 - DAILY_VIEW_W / 2, spBG->getContentSize().height / 2 - DAILY_VIEW_H / 2 - 10));
	//设置代理对象
	m_pTableView->setDelegate(this);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	spBG->addChild(m_pTableView);

	//箭头左
	m_spLArrow = ReadSpriteName("new_ui/battle/battle_arrow2.png");
	m_spLArrow->setPosition(Vec2(size.width / 2 - DAILY_VIEW_W / 2 - m_spLArrow->getContentSize().width, size.height / 2 - 10));
	this->addChild(m_spLArrow);

	//箭头右
	m_spRArrow = ReadSpriteName("new_ui/battle/battle_arrow2.png");
	m_spRArrow->setFlippedX(true);
	m_spRArrow->setPosition(Vec2(size.width / 2 + DAILY_VIEW_W / 2 + m_spRArrow->getContentSize().width, size.height / 2 - 10));
	this->addChild(m_spRArrow);

	//关闭

	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(size.width / 2+235, size.height / 2 + 123));
	this->addChild(ptrMenu);

	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(XmtWarTableView::callBackClose, this));
	ptrMenu->addChild(btnClose);

	updateArrow();
	return true;
}

void XmtWarTableView::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

XmtWarTableView::XmtWarTableView()
{

}

XmtWarTableView::~XmtWarTableView()
{

}

void XmtWarTableView::scrollViewDidScroll(ScrollView* view)
{
	updateArrow();
}

void XmtWarTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	XmtWarCell* ptrCell = (XmtWarCell*)cell;
	m_iClickIndex = ptrCell->getIdx();
	if (m_mStage[m_iClickIndex][1] == 1)
	{
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(XmtWarTableView::showDispatch, this));
		ptrCell->m_spStage->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
	}
}

bool XmtWarTableView::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

Size XmtWarTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(DAILY_CELL_W, DAILY_CELL_H);
}

TableViewCell* XmtWarTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	XmtWarCell* cell = NULL;
	if (ptrCell)
	{
		cell = (XmtWarCell*)ptrCell;
	}
	else
	{
		cell = XmtWarCell::create();
	}
	cell->updateCell(m_mStage[idx][0], m_mStage[idx][1]);
	return cell;
}

ssize_t XmtWarTableView::numberOfCellsInTableView(TableView *table)
{
	return m_mStage.size();
}

void XmtWarTableView::showDispatch()
{
	if (m_iClickIndex > -1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);

		if (m_mStage[m_iClickIndex][1] == 1)
		{
			ChessApplication::getInstance()->AddFunctionLayer(XmtWarLayer::create(-1, m_mStage[m_iClickIndex][0]));
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("dailynotopen"),false);
		}

		m_iClickIndex = -1;
	}
}

void XmtWarTableView::updateArrow()
{
	if (m_spLArrow)
	{
		Vec2 point = m_pTableView->getContentOffset();
		m_spLArrow->setVisible(point.x < 0);
		m_spRArrow->setVisible(point.x - DAILY_VIEW_W + m_mStage.size() * DAILY_CELL_W > 0);
	}
}

void XmtWarTableView::GetAllStage()
{
	map<int, int> eAllWarList;
	for (map<int, NetWarInfo*>::iterator eIt = trData->m_mNetWarInfo.begin(); eIt != trData->m_mNetWarInfo.end(); eIt++)
	{
		if (eIt->second->iType == BATTLE_IMAGE)
		{
			eAllWarList[eIt->second->iDegree] = 1;
		}
	}

	int iStageIndex = 0,iCurStage = 0;
	map<int, int> xmtWar = history->mMapDateInfo->GetBattleImageData()->GetAllBattleData();
	if (xmtWar.size()&& trData->m_mNetWarInfo.count(xmtWar.begin()->first))
	{
		iCurStage = trData->m_mNetWarInfo[xmtWar.begin()->first]->iDegree;
	}

	for (map<int, int>::iterator eIt = eAllWarList.begin(); eIt != eAllWarList.end(); eIt++)
	{
		m_mStage[iStageIndex][0] = eIt->first;
		if (!iCurStage)
		{
			m_mStage[iStageIndex][1] = 1;
		}
		else
		{
			if(iCurStage == eIt->first)
				m_mStage[iStageIndex][1] = 1;
			else
				m_mStage[iStageIndex][1] = 0;
		}
		iStageIndex++;
	}
}
