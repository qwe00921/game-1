#include "ClanArmyBattleTableView.h"
#include "Defines.h"
#include "ClanArmyBattleLayer.h"
#include "DispatchLayer.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "BattleTitleLayer.h"
#include "PopLayer.h"
#include "ClanTBHpLogTableView.h"

#define LOG_CELL_WIDTH 208
#define LOG_CELL_HEIGHT 280

ClanArmyBattleTableView* ClanArmyBattleTableView::create(map<int, ClanArmyBattleElem*>* ptrArmyBattleData, EventDelegate* ptrDelegate, string guildIdS)
{
	ClanArmyBattleTableView *pRet = new ClanArmyBattleTableView();
	if (pRet && pRet->init(ptrArmyBattleData,ptrDelegate,guildIdS))
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

bool ClanArmyBattleTableView::init(map<int, ClanArmyBattleElem*>* ptrArmyBattleData, EventDelegate* ptrDelegate, string guildIdS)
{
    if ( !Layer::init() )
    {
        return false;
    }
	m_iEnableClickCell = 1;
	mDelegate = ptrDelegate;
	mArmyBattleData = ptrArmyBattleData;
	mGuildIdS = guildIdS;

	mLeftArrow = ReadSpriteName("new_ui/img_global/turn_page.png");
	mLeftArrow->setPosition(Vec2(20, 193));
	this->addChild(mLeftArrow);


	mRightArrow = ReadSpriteName("new_ui/img_global/turn_page.png");
	mRightArrow->setPosition(Vec2(580, 193));
	mRightArrow->setFlippedX(true);
	this->addChild(mRightArrow);

	this->cell_num = mArmyBattleData->size();
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(2.5f * LOG_CELL_WIDTH,LOG_CELL_HEIGHT));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    //设置位置
    tableView->setPosition(Vec2(40,53));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();
	m_iSelectCellIndex = -1;

	fOffsetMaxX = -(cell_num - 2.5f) * LOG_CELL_WIDTH;

    return true;
}

void ClanArmyBattleTableView::ChildCloseLayer(int iNomal)
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

void ClanArmyBattleTableView::RunFunction(int iIndex)
{
	m_iSelectCellIndex = iIndex;

	if (mArmyBattleData->at(iIndex)->getEiIsOpen())
	{
		int iWarId = mArmyBattleData->at(iIndex)->getEiWarId();
		int iBattleNum = 0;
		int iBattlemaxNum = trData->m_mNetWarInfo[iWarId]->iMaxTimes;
		if (history->m_pPlayerInfo->m_mMapAttackNum.count(iWarId))
			iBattleNum = history->m_pPlayerInfo->m_mMapAttackNum[iWarId][0];

		if (iBattleNum < iBattlemaxNum)
		{
			history->m_iBattleType = BATTLE_CLANARMYBATTLE;
			history->m_iCurWarId = iWarId;
			history->m_iBattleStage = 0;
			ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("gongjicsbz"), false);
		}
	}
	else if(mArmyBattleData->at(iIndex)->getEiSelfJob() == 1)
	{
		ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_OPEN_CLANFB,this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("zhiweibuzu"), false);
	}
}

void ClanArmyBattleTableView::updateRTDate(int iIndex)
{
	m_iSelectCellIndex = iIndex;

	SendGainHpLogNetData();
}

int ClanArmyBattleTableView::GetOrSetDate(int iDate, bool is_get)
{
	int iRet = 0;

	if (is_get)
		iRet = m_iEnableClickCell;
	else
		m_iEnableClickCell = iDate;

	return iRet;
}

void ClanArmyBattleTableView::SetPopLayerElement(Sprite* bg)
{
	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	Sprite* bginfo = ReadSpriteName("new_ui/gh/gh_til14.png");
	bginfo->setPosition(Vec2(iBgWidth/2, iBgHeight - 15));
	bg->addChild(bginfo);

	int iWarId = mArmyBattleData->at(m_iSelectCellIndex)->getEiWarId();

	Label* explainLab = LabelSystemFont(trData->m_mNetWarInfo[iWarId]->strInfo.c_str(),18);
	explainLab->setDimensions(330, 0);
	explainLab->setPosition(Vec2((iBgWidth-330)/2,234));
	explainLab->setAnchorPoint(Vec2(0,1));
	bg->addChild(explainLab);

	Sprite* activeBg = ReadSpriteName("new_ui/img_global/hero_bg4.png");
	activeBg->setPosition(Vec2(iBgWidth/2,107));
	bg->addChild(activeBg);

	Label* activeLab = LabelSystemFont(__String::createWithFormat("%s %d",LL("huoyuedu"), trData->m_mNetWarInfo[iWarId]->iActive)->getCString(),16);
	activeLab->setColor(Color3B::YELLOW);
	activeLab->setPosition(Vec2(activeBg->getContentSize().width/2, activeBg->getContentSize().height/2));
	activeBg->addChild(activeLab);
}

void ClanArmyBattleTableView::callBackConfirm()
{
	SendOpenWarNetData();
}

void ClanArmyBattleTableView::SendOpenWarNetData()
{
	int iWarId = mArmyBattleData->at(m_iSelectCellIndex)->getEiWarId();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("warID=%d&gameToken=%s", iWarId, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"warID\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iWarId,GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("openGuildWar",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanArmyBattleTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendOpenWarNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanArmyBattleTableView::SendGainHpLogNetData()
{
	int iWarId = mArmyBattleData->at(m_iSelectCellIndex)->getEiWarId();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&warID=%d&gameToken=%s", mGuildIdS.c_str(),iWarId, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"warID\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", mGuildIdS.c_str(), iWarId, GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getUserGuildBossHurt",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanArmyBattleTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendGainHpLogNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanArmyBattleTableView::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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
		if (strTag == "SendOpenWarNetData")
		{
			if (value["result"]["active"].isInt())
			{
				mArmyBattleData->at(m_iSelectCellIndex)->setEiIsOpen(1);
				mArmyBattleData->at(m_iSelectCellIndex)->mGoodsList.clear();

				ClanArmyBattleCell* ptrClanArmyBattleCell = (ClanArmyBattleCell*)tableView->cellAtIndex(m_iSelectCellIndex);
				if (ptrClanArmyBattleCell)
				{
					ptrClanArmyBattleCell->UpDateCell(mArmyBattleData->at(m_iSelectCellIndex));
				}
				if (mDelegate)
					mDelegate->updateShow(0);
			}
		}
		else if (strTag == "SendGainHpLogNetData")
		{
			if (value["result"]["atkBossData"].isArray())
			{
				ChessApplication::getInstance()->AddFunctionLayer(ClanTBHpLogTableView::create(value["result"]["atkBossData"], mArmyBattleData->at(m_iSelectCellIndex)->getEiWarId()));
			}
		}

	}
}

ClanArmyBattleTableView::ClanArmyBattleTableView()
{
	cell_num = 0;
	mRightArrow = NULL;
	mLeftArrow = NULL;
}

ClanArmyBattleTableView::~ClanArmyBattleTableView()
{

}

void ClanArmyBattleTableView::scrollViewDidScroll(ScrollView* view)
{

	float fOffsetx = tableView->getContentOffset().x;
	if (fOffsetx < 0 && fOffsetx > fOffsetMaxX)
	{
		mRightArrow->setVisible(true);
		mLeftArrow->setVisible(true);
	}
	else if (fOffsetx <= fOffsetMaxX)
	{
		//mLeftArrow->setVisible(false);
		mRightArrow->setVisible(false);
	}
	else
	{
		//mRightArrow->setVisible(false);
		mLeftArrow->setVisible(false);
	}
}

void ClanArmyBattleTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ClanArmyBattleTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("tableCellTouched");
}

void ClanArmyBattleTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ClanArmyBattleTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ClanArmyBattleTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(LOG_CELL_WIDTH, LOG_CELL_HEIGHT);
}

TableViewCell* ClanArmyBattleTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ClanArmyBattleCell::create(mArmyBattleData->at(idx),this);
    }
    else
    {
		ClanArmyBattleCell* c = (ClanArmyBattleCell*)cell;
		c->UpDateCell(mArmyBattleData->at(idx));
    }
    return cell;
}

ssize_t ClanArmyBattleTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ClanArmyBattleCell* ClanArmyBattleCell::create(ClanArmyBattleElem* ptrArmyBattleElem, EventDelegate* ptrDelegate)
{
	ClanArmyBattleCell* pRet = new ClanArmyBattleCell();
	if (pRet&&pRet->init(ptrArmyBattleElem,ptrDelegate))
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

bool ClanArmyBattleCell::init(ClanArmyBattleElem* ptrArmyBattleElem, EventDelegate* ptrDelegate)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	mDelegate = ptrDelegate;

	mbg = ReadSpriteName(__String::createWithFormat("new_ui/gh/gh_boss%d.png", ptrArmyBattleElem->getEiBgImg())->getCString());
	mbg->setPosition(Vec2(LOG_CELL_WIDTH/2, LOG_CELL_HEIGHT/2));
	this->addChild(mbg);

	int iBgWight = mbg->getContentSize().width;
	int iBgHeight = mbg->getContentSize().height;

	mLockSp = ReadSpriteName("new_ui/gh/gh_boss_ks.png");
	mLockSp->setPosition(Vec2(iBgWight/2, iBgHeight/2));
	mbg->addChild(mLockSp);

	mTfInfo = ReadSpriteName("new_ui/gh/gh_txt10.png");
	mTfInfo->setPosition(Vec2(iBgWight/2,130));
	mbg->addChild(mTfInfo);

	mHpBarBg = ReadSpriteName("new_ui/gh/gh_boss_hpbg.png");
	mHpBarBg->setPosition(Vec2(iBgWight / 2,110));
	mbg->addChild(mHpBarBg);

	mHpBarSp = ReadSpriteName("new_ui/gh/gh_boss_hp1.png");
	mHpBarSp->setAnchorPoint(Vec2(0,0.5));
	mHpBarSp->setPosition(Vec2(6,mHpBarBg->getContentSize().height / 2));
	mHpBarBg->addChild(mHpBarSp);

	mHpBarPre = LabelSystemFont(__String::createWithFormat("%d%%",100)->getCString(),16);
	mHpBarPre->setPosition(Vec2(mHpBarBg->getContentSize().width / 2,mHpBarBg->getContentSize().height / 2));
	mHpBarBg->addChild(mHpBarPre);

	mGainHpLog = ReadSpriteName("new_ui/gh/gh_icon50.png");
	mGainHpLog->setPosition(mGainHpLog->getContentSize().width/2, mbg->getContentSize().height - mGainHpLog->getContentSize().height / 2);
	mbg->addChild(mGainHpLog);

	UpDateCell(ptrArmyBattleElem);

	initTouches();
	return true;
}
ClanArmyBattleCell::ClanArmyBattleCell()
{
	m_ptrLisTouch = NULL;
	m_iIsClickGoods = -1;
}

ClanArmyBattleCell::~ClanArmyBattleCell()
{
	if (!m_ptrLisTouch)
		_eventDispatcher->removeEventListener(m_ptrLisTouch);
}

void ClanArmyBattleCell::UpDateCell(ClanArmyBattleElem* ptrArmyBattleElem)
{
	mArmyBattleElem = ptrArmyBattleElem;

	int iWarId = ptrArmyBattleElem->getEiWarId();
	for (size_t i = 0; i < mGoodsList.size(); i++)
	{
		mGoodsList[i]->removeFromParent();
	}
	mGoodsList.clear();

	if (iWarId > 0)
	{
		NetWarInfo* ptrWarInfo = trData->m_mNetWarInfo[iWarId];
		map<int, int> mClanGoodsList;
		if (!ptrArmyBattleElem->getEiIsOpen())
		{
			mLockSp->setVisible(true);
			addSpriteGray(mbg);
			mTfInfo->setVisible(false);
			mHpBarBg->setVisible(false);
			mGainHpLog->setVisible(false);

			mClanGoodsList = ptrWarInfo->drop;
		}
		else
		{
			mLockSp->setVisible(false);
			removeSpriteGray(mbg);
			mTfInfo->setVisible(true);
			mHpBarBg->setVisible(true);
			mGainHpLog->setVisible(true);

			if(ptrArmyBattleElem->mGoodsList.size())
				mClanGoodsList = ptrArmyBattleElem->mGoodsList;
			else
				mClanGoodsList = ptrWarInfo->drop;

			float fPre = ptrArmyBattleElem->getEiBossCurHp() / (1.0*ptrArmyBattleElem->getEiBossMaxHp());
			int iPre = ceil(fPre*100);
			mHpBarSp->setScaleX(fPre);
			mHpBarPre->setString(__String::createWithFormat("%d%%", iPre)->getCString());
		}

		int iGoodsIndex = 0;
		for (map<int, int>::iterator eIt = mClanGoodsList.begin(); eIt != mClanGoodsList.end(); eIt++)
		{
			mGoodsList[iGoodsIndex] = GoodsSprite::create(eIt->first, eIt->second);
			mGoodsList[iGoodsIndex]->setCallback(CC_CALLBACK_1(ClanArmyBattleCell::clickGoodsMenu, this));
			mGoodsList[iGoodsIndex]->setPosition(Vec2(56 + iGoodsIndex * 62, 40));
			mbg->addChild(mGoodsList[iGoodsIndex]);
			iGoodsIndex++;
		}
	}
	else
	{
		mLockSp->setVisible(false);
		removeSpriteGray(mbg);
		mTfInfo->setVisible(false);
		mHpBarBg->setVisible(false);
		mGainHpLog->setVisible(false);
	}

	mbg->setTexture(ReadSpriteTexture(__String::createWithFormat("new_ui/gh/gh_boss%d.png", ptrArmyBattleElem->getEiBgImg())->getCString()));
}

void ClanArmyBattleCell::initTouches()
{
	m_ptrLisTouch = EventListenerTouchOneByOne::create();

	m_ptrLisTouch->onTouchBegan = [&](Touch* touch, Event* event)
	{
		m_iIsClick = -1;
		if (mDelegate->GetOrSetDate(0))
		{
			Rect bgBox = mbg->getBoundingBox();
			Rect logBox = mGainHpLog->getBoundingBox();
			if (mGainHpLog->isVisible()&&Rect(0, 0, logBox.size.width, logBox.size.height).containsPoint(mGainHpLog->convertToNodeSpace(touch->getLocation())))
			{
				m_iIsClick = 2;
				mGainHpLog->runAction(ScaleTo::create(0.1f,0.9f));
				return true;
			}
			else if (Rect(0, 0, bgBox.size.width, bgBox.size.height).containsPoint(mbg->convertToNodeSpace(touch->getLocation())))
			{
				m_iIsClick = 1;
				mDelegate->GetOrSetDate(0, false);
				return true;
			}
		}
		return true;
	};

	m_ptrLisTouch->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (m_iIsClick == 2)
		{
			Rect logBox = mGainHpLog->getBoundingBox();
			if (!Rect(0, 0, logBox.size.width, logBox.size.height).containsPoint(mGainHpLog->convertToNodeSpace(touch->getLocation())))
			{
				m_iIsClick = -1;
				mGainHpLog->runAction(ScaleTo::create(0.1f, 1));
				return;
			}
		}
	};

	m_ptrLisTouch->onTouchEnded = [&](Touch* touch, Event* event)
	{
		if (m_iIsClickGoods < 0 && m_iIsClick > 0)
		{
			Vec2 vOffset = touch->getLocation() - touch->getStartLocation();
			if (vOffset.x > -5 && vOffset.x < 5 && vOffset.y>-5 && vOffset.y < 5)
			{
				playSound(SOUND_LEFT_MOUSE_CLICK);
				if (m_iIsClick == 1)
				{
					mbg->runAction(Sequence::create(ScaleTo::create(0.1f, 0.9f), ScaleTo::create(0.1f, 1), CallFunc::create(CC_CALLBACK_0(ClanArmyBattleCell::callBack, this)), NULL));
				}
				else if (m_iIsClick == 2)
				{
					if (mArmyBattleElem->getEiWarId() > 0)
						mDelegate->updateRTDate(mArmyBattleElem->getEiIndex());
				}
			}
			else
				mDelegate->GetOrSetDate(1, false);
		}
		else if(m_iIsClick > 0)
			mDelegate->GetOrSetDate(1, false);

		if (m_iIsClick == 2)
		{
			mGainHpLog->runAction(ScaleTo::create(0.1f, 1));
		}

		m_iIsClickGoods = -1;
	};
	m_ptrLisTouch->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_ptrLisTouch,this);
}

void ClanArmyBattleCell::callBack()
{
	if (mArmyBattleElem->getEiWarId() > 0)
		mDelegate->RunFunction(mArmyBattleElem->getEiIndex());

	mDelegate->GetOrSetDate(1, false);
}

void ClanArmyBattleCell::clickGoodsMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	GoodsSprite* ptrGoodsSprite = (GoodsSprite*)sender;
	ptrGoodsSprite->AddGoodsLayer();
	m_iIsClickGoods = 1;
}