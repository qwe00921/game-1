#include "WolfCityScrollview.h"
#include "Defines.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "BattleScreen.h"
#include "DispatchLayer.h"
#include "PopLayer.h"
#include "GoodsSprite.h"
#include "GuardLayer.h"

WolfCityScrollview* WolfCityScrollview::create(EventDelegate* ptrDelegate)
{
	WolfCityScrollview* pRet = new WolfCityScrollview();
	if (pRet&&pRet->init(ptrDelegate))
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

bool WolfCityScrollview::init(EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	m_iCurPage = 0;
	m_bEnableMove = true;
	mDelegate = ptrDelegate;
	InitDateList();
	mViewWidth = size.width;
	mViewHeight = size.height;

	mScrollView = ScrollView::create(Size(mViewWidth, mViewHeight), NULL);

	mScrollView->setDelegate(this);

	mScrollView->setContentSize(Size(mViewWidth * m_iMaxPage, mViewHeight));

	mScrollView->setContentOffset(Vec2::ZERO);

	mScrollView->setDirection(ScrollView::Direction::HORIZONTAL);

	mScrollView->setPosition(Vec2::ZERO);

	mScrollView->setAnchorPoint(Vec2(0,0));

	mScrollView->setBounceable(true);

	mScrollView->setTouchEnabled(false);

	this->addChild(mScrollView);

	mRightArrowSp = ReadSpriteName("new_ui/img_global/turn_page.png");
	mRightArrowSp->setPosition(Vec2(36, size.height/2));
	this->addChild(mRightArrowSp,1);

	mLeftArrowSp = ReadSpriteName("new_ui/img_global/turn_page.png");
	mLeftArrowSp->setFlippedX(true);
	mLeftArrowSp->setPosition(size.width - 36, size.height / 2);
	this->addChild(mLeftArrowSp,1);

	WolfCityScrollviewLayer* ptrRSVLayer = NULL;

	for (int i = 0; i < m_iMaxPage; i++)
	{
		ptrRSVLayer = WolfCityScrollviewLayer::create(GetElemList(i), mDelegate);
		ptrRSVLayer->setPosition(Vec2(mViewWidth*i, 0));
		ptrRSVLayer->setAnchorPoint(Vec2(0, 0));
		mScrollView->addChild(ptrRSVLayer);
	}

	setArroeState();

	return true;
}

bool WolfCityScrollview::onTouchBegan(Touch *touch, Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_bEnableMove = true;
	m_vStarPoint = touch->getLocation();
	return true;
}

void WolfCityScrollview::onTouchMoved(Touch *touch, Event *unused_event)
{
	float fDir = touch->getLocation().x - m_vStarPoint.x;
	if (m_bEnableMove && (fDir > 70.0f && m_iCurPage || fDir< -70.0f && m_iCurPage < m_iMaxPage-1))
	{
		if (fDir > 0)
			m_iCurPage--;
		else
			m_iCurPage++;
		mScrollView->setContentOffsetInDuration(Vec2(-mViewWidth * m_iCurPage, 0), 0.3f);
		m_bEnableMove = false;
		setArroeState();
	}
}

void WolfCityScrollview::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_bEnableMove = true;
}

void WolfCityScrollview::InitDateList()
{
	mElemData.clear();

	map<int, int> mMapWarList = history->mMapDateInfo->GetBattleWolfData()->GetWolfMapWarList();

	int iElemIndex = 0;
	for (map<int, int>::iterator eIt = mMapWarList.begin(); eIt != mMapWarList.end(); eIt++)
	{
		mElemData[iElemIndex / 9][eIt->first] = 1;
		iElemIndex++;
	}
	m_iMaxPage = mElemData.size();
}

map<int, int> WolfCityScrollview::GetElemList(int iPage)
{
	return mElemData[iPage];
}

void WolfCityScrollview::setArroeState()
{
	if (!m_iCurPage)
		mRightArrowSp->setVisible(false);
	else
		mRightArrowSp->setVisible(true);
	if (m_iCurPage >= m_iMaxPage-1)
		mLeftArrowSp->setVisible(false);
	else
		mLeftArrowSp->setVisible(true);
}

WolfCityScrollview::WolfCityScrollview()
{

}

WolfCityScrollview::~WolfCityScrollview()
{

}

void WolfCityScrollview::scrollViewDidScroll(ScrollView* view)
{

}

void WolfCityScrollview::scrollViewDidZoom(ScrollView* view)
{

}


//RomanceScrollviewLayer

WolfCityScrollviewLayer* WolfCityScrollviewLayer::create(map<int, int> ElemData,EventDelegate* ptrDelegate)
{
	WolfCityScrollviewLayer* pRet = new WolfCityScrollviewLayer();
	if (pRet&&pRet->init(ElemData,ptrDelegate))
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

bool WolfCityScrollviewLayer::init(map<int, int> ElemData,EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mClickIndex = -1;
	mBattleMapId = -1;
	mClickChestindex = -1;
	mEnableClick = true;
	mReceiveChestIndex = -1;
	this->setSwallow(false);
	mElemData = ElemData;
	mDelegate = ptrDelegate;

	mWolfCity = history->mMapDateInfo->GetBattleWolfData();

	DSize size = ChessViewTool::getInstance()->getFullSize();

	if (ElemData.size() > 0)
		mBattleMapId = trData->m_mNetWarInfo[ElemData.begin()->first]->iMapIndex;

	//背景
	Sprite* bg = ReadSpriteName("new_ui/glc_war/glc_bg1.jpg");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	bg->setOpacity(0);

	//line
	Sprite* gkLineSp = ReadSpriteName("new_ui/glc_war/glc_line1.png");
	gkLineSp->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
	bg->addChild(gkLineSp);

	int iElemBgWidth = bg->getContentSize().width;
	int iElemBgHeight = bg->getContentSize().height;

	int iShowIndex = 0;

	for (map<int,int>::iterator eIt = ElemData.begin();eIt!= ElemData.end();eIt++)
	{
		NetWarInfo* ptrNetWarInfo = trData->m_mNetWarInfo[eIt->first];

		//关卡开启情况
		int iPassBattle = mWolfCity->GetWolfBattleState(eIt->first);
		bool iGuard = false;
		if (mWolfCity->GetWolfGuardData(eIt->first).size())
			iGuard = true;
		
		__String* GqbgImgpath = __String::createWithFormat("new_ui/glc_war/glc_icon%d_%d.png", ptrNetWarInfo->iImgId, iPassBattle ?2:1);
		Sprite* elemGq = ReadSpriteName(GqbgImgpath->getCString());
		elemGq->setPosition(Vec2(ptrNetWarInfo->iPosX + elemGq->getContentSize().width/2, iElemBgHeight - ptrNetWarInfo->iPosY - elemGq->getContentSize().height/2));
		bg->addChild(elemGq);

		Sprite* passedSp = NULL;
		if (iPassBattle&&iGuard)
		{
			passedSp = ReadSpriteName("new_ui/glc_war/glc_txt4.png");
		}
		else if (iPassBattle)
		{
			passedSp = ReadSpriteName("new_ui/glc_war/glc_txt3.png");
		}

		if (!iPassBattle)
		{
			//关卡开启情况
			map<int, int> mGqKq = mWolfCity->WolfIsOpen(eIt->first);
			if (mGqKq[0] != 1 && mGqKq[0] != 3)
			{
				addSpriteGray(elemGq);
			}
		}

		if (passedSp)
		{
			passedSp->setPosition(Vec2(elemGq->getContentSize().width / 2, 0));
			passedSp->setAnchorPoint(Vec2(0.5, 0));
			elemGq->addChild(passedSp);
		}


		WolfCityShowElem* ptrWCShowElem = new WolfCityShowElem();
		ptrWCShowElem->seteBgSp(elemGq);
		ptrWCShowElem->seteBISBATTLED(iPassBattle ? true : false);
		ptrWCShowElem->seteWarId(eIt->first);
		ptrWCShowElem->seteBISGURAD(iGuard);
		ptrWCShowElem->setePassSp(passedSp);
		mShowElem[iShowIndex++] = ptrWCShowElem;
	}

	//箱子
	if (mBattleMapId > 0)
	{
		Sprite* chestBoxBg = ReadSpriteName("new_ui/glc_war/glc_bg3.png");
		chestBoxBg->setPosition(Vec2(size.width / 2, chestBoxBg->getContentSize().height / 2));
		this->addChild(chestBoxBg, 1);
		chestBoxBg->setOpacity(0);

		int iStarNum = 0,iChestBoxIndex = 0;
		bool bEnableGray = false,bReceived = false,bEnableRec = false;
		std::string chestPathS = "";
		int iMapTotalStar = mWolfCity->getWolfCityWinGq();
		map<int, int> MapDate = mWolfCity->GetWolfTreasures();

		for (map<int, NetMapChestPtr>::iterator it = trData->net_map_chest[mBattleMapId].begin(); it != trData->net_map_chest[mBattleMapId].end(); it++)
		{
			bEnableGray = false;
			bReceived = false;
			bEnableRec = false;
			iStarNum = it->second->star;

			if (MapDate.count(iStarNum) > 0 && MapDate[iStarNum] > 0)
			{
				chestPathS = "new_ui/glc_war/glc_xz99.png";
				bReceived = true;
			}
			else if (iStarNum <= iMapTotalStar)
			{
				chestPathS = StringUtils::format("new_ui/glc_war/glc_xz%d.png", it->second->chest_img);
				bEnableRec = true;
			}
			else
			{
				chestPathS = StringUtils::format("new_ui/glc_war/glc_xz%d.png", it->second->chest_img);
				bEnableGray = true;
			}

			Sprite* chestImg = ReadSpriteName(chestPathS.c_str());
			chestImg->setAnchorPoint(Vec2(0.5,0));
			chestBoxBg->addChild(chestImg);
			if (bEnableGray)
				addSpriteGray(chestImg);

			//102,0,0
			Label* gqLab = LabelSystemFont(StringUtils::format(LL("didguan"), iStarNum).c_str(),16);
			gqLab->setPosition(Vec2(chestImg->getContentSize().width/2, chestImg->getContentSize().height));
			chestImg->addChild(gqLab);

			if (bReceived)
			{
				gqLab->setVisible(false);
			}
			else if (bEnableRec)
			{
				gqLab->enableOutline(Color4B(102, 0, 0, 255), 1);
			}
			else
			{
				gqLab->enableOutline(Color4B::BLACK, 1);
			}

			WolfCityChestShow* ptrWCChestShow = new WolfCityChestShow();
			ptrWCChestShow->seteBGSP(chestImg);
			ptrWCChestShow->seteISRECEIVE(bReceived);
			ptrWCChestShow->seteMAPID(mBattleMapId);
			ptrWCChestShow->seteStarNum(iStarNum);
			ptrWCChestShow->seteENABLERECEIVE(bEnableRec);
			ptrWCChestShow->seteGQNAME(gqLab);

			mChestElem[iChestBoxIndex++] = ptrWCChestShow;
		}

		if (mChestElem.size())
		{
			int chestBgWidth = chestBoxBg->getContentSize().width;
			int iChestSpace = 75;
			int iPointX = (chestBgWidth - iChestSpace * (mChestElem.size() - 1)) / 2;

			for (size_t i = 0; i < mChestElem.size(); i++)
			{
				mChestElem[i]->geteBGSP()->setPosition(Vec2(iPointX+i*iChestSpace,0));
			}
		}
	}

	return true;
}

bool WolfCityScrollviewLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (!mEnableClick) return true;

	mClickIndex = -1;
	mClickChestindex = -1;
	mMoveStartPoint = touch->getLocation();
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		Rect elemBox = mShowElem[i]->geteBgSp()->getBoundingBox();
		if (Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(mShowElem[i]->geteBgSp()->convertToNodeSpace(touch->getLocation())))
		{
			mClickIndex = i;
			ScaleTo* ptrScale = ScaleTo::create(0.1f,0.9f);
			mShowElem[i]->geteBgSp()->runAction(ptrScale);
			return true;
		}
	}

	for (size_t i = 0; i < mChestElem.size(); i++)
	{
		Rect elemBox = mChestElem[i]->geteBGSP()->getBoundingBox();
		if (!mChestElem[i]->geteISRECEIVE()&&Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(mChestElem[i]->geteBGSP()->convertToNodeSpace(touch->getLocation())))
		{
			mClickChestindex = i;
			ScaleTo* ptrScale = ScaleTo::create(0.1f, 0.9f);
			mChestElem[i]->geteBGSP()->runAction(ptrScale);
			return true;
		}
	}

	return true;
}

void WolfCityScrollviewLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (mEnableClick&&(mClickIndex > -1|| mClickChestindex>-1))
	{
		float offsetX = touch->getLocation().x - mMoveStartPoint.x;
		float offsetY = touch->getLocation().y - mMoveStartPoint.y;
		if (abs((int)offsetX) > 1 || abs((int)offsetY) > 1)
		{
			if (mClickIndex > -1)
			{
				ScaleTo* ptrScale = ScaleTo::create(0.1f, 1);
				mShowElem[mClickIndex]->geteBgSp()->runAction(ptrScale);
				mClickIndex = -1;
			}
			if (mClickChestindex>-1)
			{
				ScaleTo* ptrScale = ScaleTo::create(0.1f, 1);
				mChestElem[mClickChestindex]->geteBGSP()->runAction(ptrScale);
				mClickChestindex = -1;
			}
			
		}
	}
}

void WolfCityScrollviewLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (!mEnableClick) return;
	if (mClickIndex > -1)
	{
		mEnableClick = false;
		ScaleTo* ptrScale = ScaleTo::create(0.1f, 1);
		mShowElem[mClickIndex]->geteBgSp()->runAction(ptrScale);

		if (!mShowElem[mClickIndex]->geteBISBATTLED())
		{
			map<int, int> mWarOpenState = mWolfCity->WolfIsOpen(mShowElem[mClickIndex]->geteWarId());

			if (mWarOpenState[0] == 1)
			{
				//出兵
				history->m_iBattleType = BATTLE_WOLFCITY;
				history->m_iCurWarId = mShowElem[mClickIndex]->geteWarId();
				history->m_iBattleStage = 0;
				ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
				this->scheduleOnce(schedule_selector(WolfCityScrollviewLayer::setEnableClick), 0.5f);
			}
			else if (mWarOpenState[0] == 3)
			{
				ITools::getInstance()->createGlobalPrompt(LL("qpqwjzsqyg"), false);
				setEnableClick(0);
			}
			else
			{
				ITools::getInstance()->createGlobalPrompt(LL("gqwkq"), false);
				setEnableClick(0);
			}
		}
		else
		{
			ChessApplication::getInstance()->AddFunctionLayer(GuardLayer::create(mShowElem[mClickIndex]->geteWarId(),this));
			this->scheduleOnce(schedule_selector(WolfCityScrollviewLayer::setEnableClick), 0.5f);
		}

	}
	else if (mClickChestindex > -1)
	{
		mEnableClick = false;
		ScaleTo* ptrScale = ScaleTo::create(0.1f, 1);
		mChestElem[mClickChestindex]->geteBGSP()->runAction(ptrScale);
		
		int iChestState = 0;
		if (mChestElem[mClickChestindex]->geteENABLERECEIVE())
			iChestState = 1;
		ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_GLC_CHEST, this, iChestState));
		this->scheduleOnce(schedule_selector(WolfCityScrollviewLayer::setEnableClick), 0.5f);
	}
}

void WolfCityScrollviewLayer::ChildCloseLayer(int iNomal)
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

WolfCityScrollviewLayer::WolfCityScrollviewLayer()
{

}
WolfCityScrollviewLayer::~WolfCityScrollviewLayer()
{
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		delete mShowElem[i];
	}
	for (size_t i = 0; i < mChestElem.size(); i++)
	{
		delete mChestElem[i];
	}
}

void WolfCityScrollviewLayer::setEnableClick(float f)
{
	mEnableClick = true;
}

void WolfCityScrollviewLayer::SetPopLayerElement(Sprite* bgSp)
{
	int iBgWidth = bgSp->getContentSize().width;
	int iBgHeight = bgSp->getContentSize().height;

	Sprite* tgjlSp = ReadSpriteName("new_ui/daily/stalker_txt4.png");
	tgjlSp->setAnchorPoint(Vec2(0.5, 1));
	tgjlSp->setPosition(Vec2(iBgWidth / 2, iBgHeight));
	bgSp->addChild(tgjlSp);

	if (mClickChestindex > -1)
	{
		WolfCityChestShow* ptrWCCS = mChestElem[mClickChestindex];
		NetMapChestPtr ptrNMC = trData->net_map_chest[ptrWCCS->geteMAPID()][ptrWCCS->geteStarNum()];
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
		int iPointX = (iBgWidth - ieGoodsBgWidth*eGoodsListIndex - 10 * (eGoodsListIndex - 1)) / 2 + ieGoodsBgWidth/2;

		for (int i = 0; i < eGoodsListIndex; i++)
		{
			iGoodsId = eGoodsIdList[i][0];
			iCount = eGoodsIdList[i][1];

			Sprite* GoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
			GoodsBG->setPosition(Vec2(iPointX+i*(ieGoodsBgWidth+10), iBgHeight / 2 + 23));
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

void WolfCityScrollviewLayer::RunFunction(int iNomal)
{
	if (iNomal == 1&& mClickChestindex>-1)
	{
		mReceiveChestIndex = mClickChestindex;
		SendNetDate(mChestElem[mReceiveChestIndex]->geteMAPID(), mChestElem[mReceiveChestIndex]->geteStarNum());
	}
}

void WolfCityScrollviewLayer::updateRTDate(int iWarId)
{
	if (mClickIndex > -1 && mShowElem[mClickIndex]->geteWarId() == iWarId)
	{
		mShowElem[mClickIndex]->seteBISGURAD(true);
		if (mShowElem[mClickIndex]->getePassSp())
		{
			mShowElem[mClickIndex]->getePassSp()->setTexture(ReadSpriteTexture("new_ui/glc_war/glc_txt4.png"));
		}
	}
	else
	{
		for (size_t i = 0; i < mShowElem.size(); i++)
		{
			if (mShowElem[i]->geteWarId() == iWarId)
			{
				mShowElem[i]->seteBISGURAD(true);
				if (mShowElem[mClickIndex]->getePassSp())
				{
					mShowElem[mClickIndex]->getePassSp()->setTexture(ReadSpriteTexture("new_ui/glc_war/glc_txt4.png"));
				}
				break;
			}
		}
	}
}

void WolfCityScrollviewLayer::SendNetDate(int iMapId,int iStar)
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
		callfuncND_selector(WolfCityScrollviewLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);
}

void WolfCityScrollviewLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
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

void WolfCityScrollviewLayer::setNetDate(Json::Value jsonValue)
{
	mPromptTxt.clear();
	mPromptTxt = history->onHttpGoodsAdd(jsonValue);
	mPromptTxtIndex = 0;
	this->schedule(schedule_selector(WolfCityScrollviewLayer::showPrompt), 0.8f, mPromptTxt.size() - 1, 0);

	mWolfCity->SetWolfTreasures(mChestElem[mReceiveChestIndex]->geteStarNum(), 1);
	UpdataChestShow();
}

void WolfCityScrollviewLayer::showPrompt(float f)
{
	ITools::getInstance()->createGlobalPrompt(mPromptTxt[mPromptTxtIndex++], true);
}

void WolfCityScrollviewLayer::UpdataChestShow()
{
	mChestElem[mReceiveChestIndex]->geteBGSP()->setTexture(ReadSpriteTexture("new_ui/glc_war/glc_xz99.png"));
	mChestElem[mReceiveChestIndex]->seteENABLERECEIVE(false);
	mChestElem[mReceiveChestIndex]->seteISRECEIVE(true);
	mChestElem[mReceiveChestIndex]->geteGQNAME()->setVisible(false);
}