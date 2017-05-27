#include "RomanceScrollview.h"
#include "Defines.h"
#include "RomanceInfoLayer.h"
#include "RomanceDetailsLayer.h"

RomanceScrollview* RomanceScrollview::create(EventDelegate* ptrDelegate)
{
	RomanceScrollview* pRet = new RomanceScrollview();
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

bool RomanceScrollview::init(EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	m_iCurPage = 0;
	m_bEnableMove = true;
	mDelegate = ptrDelegate;
	InitDateList();
	mViewWidth = 510;
	mViewHeight = 316;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	mScrollView = ScrollView::create(Size(mViewWidth, mViewHeight), NULL);

	mScrollView->setDelegate(this);

	mScrollView->setContentSize(Size(mViewWidth * m_iMaxPage, mViewHeight));

	mScrollView->setContentOffset(Vec2::ZERO);

	mScrollView->setDirection(ScrollView::Direction::HORIZONTAL);

	mScrollView->setPosition(Vec2((size.width - mViewWidth) / 2, 30));

	mScrollView->setAnchorPoint(Vec2(0,0));

	mScrollView->setBounceable(true);

	mScrollView->setTouchEnabled(false);

	this->addChild(mScrollView);

	mRightArrowSp = ReadSpriteName("new_ui/img_global/turn_page.png");
	mRightArrowSp->setPosition(Vec2((size.width - mViewWidth) / 2 -11, 188));
	this->addChild(mRightArrowSp);

	mLeftArrowSp = ReadSpriteName("new_ui/img_global/turn_page.png");
	mLeftArrowSp->setFlippedX(true);
	mLeftArrowSp->setPosition((size.width - mViewWidth) / 2 + mViewWidth +11, 188);
	this->addChild(mLeftArrowSp);

	RomanceScrollviewLayer* ptrRSVLayer = NULL;
	for (int i = 0; i < m_iMaxPage; i++)
	{
		ptrRSVLayer = RomanceScrollviewLayer::create(GetElemList(i), mDelegate);
		ptrRSVLayer->setPosition(Vec2(mViewWidth*i, 0));
		ptrRSVLayer->setAnchorPoint(Vec2(0, 0));
		mScrollView->addChild(ptrRSVLayer);
	}

	setArroeState();
	return true;
}

bool RomanceScrollview::onTouchBegan(Touch *touch, Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_bEnableMove = true;
	m_vStarPoint = touch->getLocation();
	return true;
}

void RomanceScrollview::onTouchMoved(Touch *touch, Event *unused_event)
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

void RomanceScrollview::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_bEnableMove = true;
}

void RomanceScrollview::InitDateList()
{
	mElemData.clear();
	int iElemIndex = 0, iElemIndex1 = 0, iElemIndex2 = 0;
	map<int, int> mOpenNetStory;
	map<int, int> mNotOpenNetStory;
	map<int, map<int, int> > mOpenWatList;
	for (map<int, NetStory*>::iterator NsIt = trData->net_story.begin(); NsIt != trData->net_story.end(); NsIt++)
	{
		if (history->mRomanceOpenData.count(NsIt->first))
		{
			mOpenNetStory[iElemIndex1++] = NsIt->first;
			mOpenWatList[NsIt->first][1] = 0;
		}
		else
		{
			mNotOpenNetStory[iElemIndex2++] = NsIt->first;
		}

	}

	for (int i = 0; i < iElemIndex1; i++)
	{
		int iMapId = mOpenNetStory[i];
		if (trData->map_total_war.count(iMapId))
		{
			for (map<int, int>::iterator mIt = trData->map_total_war[iMapId].begin(); mIt != trData->map_total_war[iMapId].end(); mIt++)
			{
				if (history->mMapDateInfo->getHeroicMapDateElem(mIt->first))
				{
					mOpenWatList[iMapId][0] += 1;
				}
				mOpenWatList[iMapId][1] += 1;
			}
		}
	}

	

	map<int, map<int, float> > mOpenWarPre;
	float fPre = 0;
	int mOpenWarPreIndex = 0;
	for (map<int, map<int, int> >::iterator mIt = mOpenWatList.begin(); mIt != mOpenWatList.end(); mIt++)
	{
		fPre = 0;
		if (mIt->second[1])
		{
			fPre = 1.0*mIt->second[0] / mIt->second[1];
		}

		mOpenWarPre[mOpenWarPreIndex][0] = mIt->first;
		mOpenWarPre[mOpenWarPreIndex][1] = fPre;
		mOpenWarPreIndex++;
	}

	float fNum1 = 0, fNum2 = 0;
	for (int i = 0; i < mOpenWarPreIndex; i++)
	{
		for (int j = mOpenWarPreIndex - 1; j > i; j--)
		{
			if ((mOpenWarPre[j][1] < mOpenWarPre[i][1])||(mOpenWarPre[j][1] == mOpenWarPre[i][1] && mOpenWarPre[j][0] < mOpenWarPre[i][0]))
			{
				fNum1 = mOpenWarPre[i][0];
				fNum2 = mOpenWarPre[i][1];

				mOpenWarPre[i][0] = mOpenWarPre[j][0];
				mOpenWarPre[i][1] = mOpenWarPre[j][1];

				mOpenWarPre[j][0] = fNum1;
				mOpenWarPre[j][1] = fNum2;
			}
		}
	}

	for (int i = 0; i < mOpenWarPreIndex; i++)
	{
		mElemData[iElemIndex++] = mOpenWarPre[i][0];
	}
	for (int i = 0; i < iElemIndex2; i++)
	{
		mElemData[iElemIndex++] = mNotOpenNetStory[i];
	}

	m_iMaxPage = ceil(iElemIndex / 6.0f);
}

map<int, int> RomanceScrollview::GetElemList(int iPage)
{
	map<int, int> mRet;
	int iRetIndex = 0;
	for (size_t i = iPage * 6; i < mElemData.size() && i<(size_t)((iPage+1) * 6); i++)
	{
		mRet[iRetIndex++] = mElemData[i];
	}
	return mRet;
}

void RomanceScrollview::setArroeState()
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

RomanceScrollview::RomanceScrollview()
{

}

RomanceScrollview::~RomanceScrollview()
{

}

void RomanceScrollview::scrollViewDidScroll(ScrollView* view)
{

}

void RomanceScrollview::scrollViewDidZoom(ScrollView* view)
{

}


//RomanceScrollviewLayer

RomanceScrollviewLayer* RomanceScrollviewLayer::create(map<int, int> ElemData,EventDelegate* ptrDelegate)
{
	RomanceScrollviewLayer* pRet = new RomanceScrollviewLayer();
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

bool RomanceScrollviewLayer::init(map<int, int> ElemData,EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mClickIndex = -1;
	this->setSwallow(false);
	mElemData = ElemData;
	mDelegate = ptrDelegate;
	NetStory* ptrNetStory = NULL;

	int iElemBgWidth = 0;
	int iElemBgHeight = 0;
	for (size_t i = 0; i < ElemData.size(); i++)
	{
		ptrNetStory = trData->net_story[ElemData[i]];

		string bg_path = StringUtils::format("img/script/imgs_%d.png", ptrNetStory->mapid);

		Sprite* bg = ReadSpriteNormal(bg_path.c_str());
		bg->setAnchorPoint(Vec2(0,0));
		bg->setPosition(Vec2((i%3)*175,168 - i/3*148));
		this->addChild(bg);

		iElemBgWidth = bg->getContentSize().width;
		iElemBgHeight = bg->getContentSize().height;

		Sprite* numBg = ReadSpriteName("new_ui/img_global/imgs_bg2.png");
		numBg->setPosition(Vec2(iElemBgWidth / 2, 56));
		bg->addChild(numBg);

		int iNumBgWidth = numBg->getContentSize().width;
		int iNumBgHeight = numBg->getContentSize().height;

		if (ptrNetStory->story_type == 1)
		{
			Label* freeLab = LabelSystemFont(LL("mianfeikaiqi"),16);
			freeLab->setPosition(Vec2(iNumBgWidth / 2, iNumBgHeight / 2));
			freeLab->enableOutline(Color4B::BLACK, 1);
			numBg->addChild(freeLab);
		}
		else if (ptrNetStory->story_type == 2)
		{
			Sprite* sealSp = ReadSpriteName("new_ui/img_global/icon_yx.png");
			sealSp->setAnchorPoint(Vec2(0,0.5));
			sealSp->setPosition(Vec2(5, iNumBgHeight/ 2));
			numBg->addChild(sealSp);

			Label* sealLab = LabelSystemFont(IntToString(ptrNetStory->jadeSeal),16);
			sealLab->setAnchorPoint(Vec2(0,0.5));
			sealLab->setPosition(Vec2(5 + sealSp->getContentSize().width, iNumBgHeight / 2));
			sealLab->enableOutline(Color4B::BLACK, 1);
			sealLab->setColor(Color3B::GREEN);
			numBg->addChild(sealLab);

			Label* jadeLab = LabelSystemFont(IntToString(ptrNetStory->jade), 16);
			jadeLab->setAnchorPoint(Vec2(1, 0.5));
			jadeLab->setPosition(Vec2(iNumBgWidth - 5, iNumBgHeight / 2));
			jadeLab->enableOutline(Color4B::BLACK, 1);
			jadeLab->setColor(Color3B::GREEN);
			numBg->addChild(jadeLab);

			Sprite* jadeSp = ReadSpriteName("new_ui/img_global/icon_gy.png");
			jadeSp->setAnchorPoint(Vec2(1, 0.5));
			jadeSp->setPosition(Vec2(iNumBgWidth - 8 - jadeLab->getContentSize().width, iNumBgHeight / 2));
			numBg->addChild(jadeSp);
		}
		else if (ptrNetStory->story_type == 3)
		{
			Sprite* vipSp = ReadSpriteName("new_ui/main/vip1.png");
			vipSp->setAnchorPoint(Vec2(0,0.5));
			vipSp->setPosition(Vec2(5, iNumBgHeight / 2));
			numBg->addChild(vipSp);

			if (ptrNetStory->jade)
			{
				Label* jadeLab = LabelSystemFont(IntToString(ptrNetStory->jade), 16);
				jadeLab->setAnchorPoint(Vec2(1, 0.5));
				jadeLab->setPosition(Vec2(iNumBgWidth - 5, iNumBgHeight / 2));
				jadeLab->enableOutline(Color4B::BLACK, 1);
				jadeLab->setColor(Color3B::GREEN);
				numBg->addChild(jadeLab);

				Sprite* jadeSp = ReadSpriteName("new_ui/img_global/icon_gy.png");
				jadeSp->setAnchorPoint(Vec2(1, 0.5));
				jadeSp->setPosition(Vec2(iNumBgWidth - 8 - jadeLab->getContentSize().width, iNumBgHeight / 2));
				numBg->addChild(jadeSp);
			}
			else
			{
				Label* freeLab = LabelSystemFont(LL("mianfeikaiqi"), 16);
				freeLab->setAnchorPoint(Vec2(1,0.5));
				freeLab->setPosition(Vec2(iNumBgWidth - 8, iNumBgHeight / 2));
				freeLab->enableOutline(Color4B::BLACK, 1);
				numBg->addChild(freeLab);
			}
		}
		else if (ptrNetStory->story_type == 4&& ptrNetStory->condition)
		{
			int iHeroId = ptrNetStory->condition;
			NetHeroNoDress* ptrNHND = NULL;
			Sprite* arms_fashion = NULL;
			__String* img_path = NULL;

			if (trData->net_hero_no_dress.count(iHeroId))
			{
				ptrNHND = trData->net_hero_no_dress[iHeroId];
				__String* f_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", 100, ptrNHND->fashion2);
				img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrNHND->gender, ptrNHND->fashion1);
				arms_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
				int fashion_width = arms_fashion->getContentSize().width;
				arms_fashion->setTextureRect(Rect(0, fashion_width * 4, fashion_width, fashion_width));
				int iZorder = trData->net_hero_dress[ptrNHND->fashion2]->mSpc[4];
				if (iZorder) iZorder++;
				arms_fashion->setPosition(20, numBg->getContentSize().height / 2+12);
				numBg->addChild(arms_fashion, iZorder);
			}
			else
			{
				img_path = __String::create(history->actors[iHeroId - 1].getActorSImgPath("spc", 0));
			}

			//武将
			Sprite* actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
			int img_width = actor_img->getContentSize().width;
			//actor_img->setAnchorPoint(Vec2(0.5, 0));
			actor_img->setTextureRect(Rect(0, img_width * 4, img_width, img_width));
			actor_img->setPosition(Vec2(20, numBg->getContentSize().height / 2+12));
			numBg->addChild(actor_img);

			Label* nameLab = LabelSystemFont(__String::createWithFormat("%s %s",trData->actors[iHeroId-1].getActorName(),LL("kekaiqi"))->getCString(),16);
			nameLab->setAnchorPoint(Vec2(0, 0.5));
			nameLab->setPosition(Vec2(45, numBg->getContentSize().height/2));
			nameLab->enableOutline(Color4B::BLACK, 1);
			numBg->addChild(nameLab);

		}

		Sprite* LockSp = ReadSpriteName("new_ui/img_global/imgs_lock.png");
		LockSp->setPosition(Vec2(iElemBgWidth / 2, 91));
		bg->addChild(LockSp);

		RomanceShowElem* ptrRSE = new RomanceShowElem();
		ptrRSE->setEMapId(ptrNetStory->mapid);
		ptrRSE->setELockSp(LockSp);
		ptrRSE->setEPricebg(numBg);
		ptrRSE->setEBgSp(bg);
		mShowElem[i] = ptrRSE;

		if (history->mRomanceOpenData.count(ptrNetStory->mapid))
		{
			ptrRSE->setEIsOpen(true);
			LockSp->setVisible(false);
			numBg->setVisible(false);

			int iIsOpenWarNum = 0,iTotalWarNum = 0;
			if (trData->map_total_war.count(ptrNetStory->mapid))
			{
				for (map<int, int>::iterator mIt = trData->map_total_war[ptrNetStory->mapid].begin(); mIt != trData->map_total_war[ptrNetStory->mapid].end(); mIt++)
				{
					if (history->mMapDateInfo->getHeroicMapDateElem(mIt->first))
					{
						iIsOpenWarNum += 1;
					}
					iTotalWarNum += 1;
				}
			}

			Label* preLab = LabelSystemFont(__String::createWithFormat("%d%%", iIsOpenWarNum * 100 / iTotalWarNum)->getCString(),14);
			preLab->setPosition(Vec2(4,50));
			preLab->setAnchorPoint(Vec2(0,0.5));
			preLab->enableOutline(Color4B::BLACK, 1);
			bg->addChild(preLab);

		}
		else
		{
			ptrRSE->setEIsOpen(false);
			addSpriteGray(bg);
			LockSp->setVisible(true);
			numBg->setVisible(true);
		}
	}

	return true;
}

bool RomanceScrollviewLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	mClickIndex = -1;
	mMoveStartPoint = touch->getLocation();
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		Rect elemBox = mShowElem[i]->getEBgSp()->getBoundingBox();
		if (Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(mShowElem[i]->getEBgSp()->convertToNodeSpace(touch->getLocation())))
		{
			mClickIndex = i;
			return true;
		}
	}
	return true;
}

void RomanceScrollviewLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (mClickIndex > -1)
	{
		float offsetX = touch->getLocation().x - mMoveStartPoint.x;
		float offsetY = touch->getLocation().y - mMoveStartPoint.y;
		if (abs((int)offsetX) > 1 || abs((int)offsetY) > 1)
		{
			mClickIndex = -1;
		}
	}
}

void RomanceScrollviewLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (mClickIndex > -1)
	{
		if (!mShowElem[mClickIndex]->getEIsOpen())
		{
			ChessApplication::getInstance()->AddFunctionLayer(RomanceInfoLayer::create(mShowElem[mClickIndex]->getEMapId(),mDelegate));
		}
		else
		{
			ChessApplication::getInstance()->AddFunctionLayer(RomanceDetailsLayer::create(mShowElem[mClickIndex]->getEMapId(),false));
			if (mDelegate)
				mDelegate->ChildCloseLayer(0);
		}
	}
}

RomanceScrollviewLayer::RomanceScrollviewLayer()
{

}
RomanceScrollviewLayer::~RomanceScrollviewLayer()
{
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		delete mShowElem[i];
	}
}