#include "RomanceDetailsLayer.h"
#include "Defines.h"
#include "GoodsSprite.h"
#include "DispatchLayer.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "BattleTitleLayer.h"
#include "Net_HomePage.h"
#include "TreasureBoxLayer.h"
#include "HeroSprite.h"
#include "RomanceLayer.h"
#include "RomanceInfoLayer.h"
#include "GuideLayer.h"

RomanceDetailsLayer* RomanceDetailsLayer::create(int iMapId, bool bEnablePlayMp3,bool bShowTitle)
{
	RomanceDetailsLayer* pRet = new RomanceDetailsLayer();
	if (pRet&&pRet->init(iMapId,bEnablePlayMp3,bShowTitle))
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

bool RomanceDetailsLayer::init(int iMapId, bool bEnablePlayMp3, bool bShowTitle)
{
	if (!Layer::init())
	{
		return false;
	}
	this->mMapId = iMapId;
	this->mStartWarId = -1;
	this->mShowElemLayer = NULL;
	this->mClickIndex = -1;

	if (bEnablePlayMp3)
		ChessPlayMp3(MUSIC_ROMANCE);

	mFullSize = ChessViewTool::getInstance()->getFullSize();
	NetStory* ptrNetStory = trData->net_story[iMapId];

	if (bShowTitle)
		ITools::getInstance()->createGlobalPrompt(__String::createWithFormat("%s %s", ptrNetStory->name.c_str(), LL("zyykq"))->getCString(), true);

	Sprite* bg = ReadSpriteNormal(StringUtils::format("img/script/script_map_%d.jpg", ptrNetStory->map_img).c_str());
	bg->setPosition(Vec2(mFullSize.width / 2, mFullSize.height / 2));
	this->addChild(bg);

	Sprite* chapterBgSp = ReadSpriteName("new_ui/img_global/imgs_bg4.png");
	chapterBgSp->setAnchorPoint(Vec2(0, 1));
	chapterBgSp->setPosition(Vec2(0, mFullSize.height));
	this->addChild(chapterBgSp,1);

	Sprite* chapterSp = ReadSpriteNormal(StringUtils::format("img/script/tit_%d.png", mMapId).c_str());
	chapterSp->setAnchorPoint(Vec2(0,0.5));
	chapterSp->setPosition(Vec2(10, chapterBgSp->getContentSize().height / 2 + 8));
	chapterBgSp->addChild(chapterSp);

	Label* jinduTxt = LabelSystemFont(LL("jindu"),16);
	jinduTxt->setAnchorPoint(Vec2(0, 0.5));
	jinduTxt->setPosition(Vec2(14 + chapterSp->getContentSize().width, chapterBgSp->getContentSize().height / 2+6));
	chapterBgSp->addChild(jinduTxt);

	mjinduLab = LabelSystemFont(__String::createWithFormat("%d%%", 0)->getCString(), 16);
	mjinduLab->setColor(Color3B::YELLOW);
	mjinduLab->setAnchorPoint(Vec2(0, 0.5));
	mjinduLab->setPosition(14 + chapterSp->getContentSize().width + jinduTxt->getContentSize().width, chapterBgSp->getContentSize().height / 2 + 6);
	chapterBgSp->addChild(mjinduLab);

	GetWarList();
	AddWarShowElem();
	AddShowElemLine();

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(0,0));
	this->addChild(mMenu);

	Sprite* closeNormalImage = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/battle/flippage3.png");
	closeSelectImage->setScale(0.9f);
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(RomanceDetailsLayer::clickCloseMenu, this));
	closeSelectImage->setAnchorPoint(Vec2(0.5,0.5));
	closeSelectImage->setPosition(Vec2(closeSelectImage->getContentSize().width / 2, closeSelectImage->getContentSize().height / 2));
	close_menu->setPosition(Vec2(mFullSize.width - (closeSelectImage->getContentSize().width / 2 + 8), mFullSize.height - (closeSelectImage->getContentSize().height / 2 + 8)));
	mMenu->addChild(close_menu);

	Sprite* Treasure_N = ReadSpriteName("new_ui/battle/battle_jiangl.png");
	Sprite* Treasure_S = ReadSpriteName("new_ui/battle/battle_jiangl.png");
	Treasure_S->setScale(0.9f);
	MenuItemSprite* Treasure_Item = MenuItemSprite::create(Treasure_N, Treasure_S, CC_CALLBACK_1(RomanceDetailsLayer::clickTreasureMenu, this));
	Treasure_S->setAnchorPoint(Vec2(0.5, 0.5));
	Treasure_S->setPosition(Vec2(Treasure_S->getContentSize().width / 2, Treasure_S->getContentSize().height / 2));
	Treasure_Item->setPosition(Vec2(35, 35));
	mMenu->addChild(Treasure_Item);

	Sprite* juben_N = ReadSpriteName("new_ui/battle/battle_jbeng.png");
	Sprite* juben_S = ReadSpriteName("new_ui/battle/battle_jbeng.png");
	juben_S->setScale(0.9f);
	MenuItemSprite* juben_Item = MenuItemSprite::create(juben_N, juben_S, CC_CALLBACK_1(RomanceDetailsLayer::clickJuBenMenu, this));
	juben_S->setAnchorPoint(Vec2(0.5, 0.5));
	juben_S->setPosition(Vec2(juben_S->getContentSize().width / 2, juben_S->getContentSize().height / 2));
	juben_Item->setPosition(Vec2(35, 105));
	mMenu->addChild(juben_Item);

	history->updateGuide(EMIL_GUIDE);
	return true;
}

void RomanceDetailsLayer::GetWarList()
{
	mWarList.clear();
	mWarOpenList.clear();
	int iOpenWarNum = 0;
	for (map<int, NetWarInfo*>::iterator mIt = trData->m_mNetWarInfo.begin(); mIt != trData->m_mNetWarInfo.end(); mIt++)
	{
		if (mIt->second->iMapIndex == mMapId&&mIt->second->iType == BATTLE_ROMANCE)
		{
			if (mStartWarId == -1 && !mIt->second->iPosX&&!mIt->second->iPosY)
				mStartWarId = mIt->first;

			if (history->mMapDateInfo->getHeroicMapDateElem(mIt->first))
			{
				mWarList[mIt->first] = 1;
				iOpenWarNum++;
				mWarOpenList[mIt->first] = 1;
			}
			else
			{
				mWarList[mIt->first] = -1;
			}
		}
	}

	if (mWarList.size())
	{
		mjinduLab->setString(__String::createWithFormat("%d%%", iOpenWarNum*100 / (int)mWarList.size())->getCString());
	}


	for (map<int, int>::iterator eIt = mWarList.begin(); eIt != mWarList.end(); eIt++)
	{
		if (eIt->second == -1)
		{
			if (trData->net_story_condition.count(eIt->first))
			{
				int iLabIndex = 0;
				bool bConIsOpen = true;
				mWarOpenList[eIt->first] = 1;
				for (map<int, NetStoryCondition*>::iterator mIt = trData->net_story_condition[eIt->first].begin(); mIt != trData->net_story_condition[eIt->first].end(); mIt++)
				{
					
					int iFrontWarId = mIt->second->iGqId;
					if (mIt->second->iType == 1 && !history->mMapDateInfo->getHeroicMapDateElem(iFrontWarId))
					{
						bConIsOpen = false;
					}
					else if (mIt->second->iType == 2 &&
						!((history->m_mScriptCode.count(iFrontWarId) && history->m_mScriptCode[iFrontWarId].count(mIt->second->iVariable))
							|| (history->m_mScriptCode.count(GLOBAL_VAR_MAPID) && history->m_mScriptCode[GLOBAL_VAR_MAPID].count(mIt->second->iVariable))))
					{
						bConIsOpen = false;
					}

					if (!bConIsOpen)
					{
						mWarOpenList[eIt->first] = -1;
						break;
					}
				}
			}
			else
			{
				mWarOpenList[eIt->first] = 1;
			}
		}

	}
}

void RomanceDetailsLayer::AddWarShowElem()
{
	mMapBoundary.clear();
	DSize size = ChessViewTool::getInstance()->getFullSize();
	int iCurWarId = -1;
	if (history->m_iBattleType == BATTLE_ROMANCE)
	{
		iCurWarId = history->m_iCurWarId;
	}

	mShowLayerVec2 = Vec2(0, 0);

	mShowElemLayer = Layer::create();
	mShowElemLayer->setAnchorPoint(Vec2(0,0));
	mShowElemLayer->setPosition(mShowLayerVec2);
	this->addChild(mShowElemLayer);

	int iMinX = size.width / 2, iMaxX = size.width / 2, iMinY = size.height / 2, iMaxY = size.height / 2;

	Vec2 vStartP = Vec2(size.width/2,size.height/2);
	NetWarInfo* ptrNWI = NULL;

	for (map<int, int>::iterator mIt = mWarList.begin(); mIt != mWarList.end(); mIt++)
	{
		ptrNWI = trData->m_mNetWarInfo[mIt->first];

		Vec2 vElemP = vStartP + Vec2(ptrNWI->iPosX, -ptrNWI->iPosY);
		Sprite* eBg = ReadSpriteNormal(StringUtils::format("img/script/s%d.png",ptrNWI->iImgId));
		eBg->setPosition(vElemP);
		mShowElemLayer->addChild(eBg,1);

		Sprite* nameBgSp = ReadSpriteName("new_ui/img_global/imgs_bg9.png");
		nameBgSp->setAnchorPoint(Vec2(0.5,0));
		nameBgSp->setPosition(Vec2(eBg->getContentSize().width/2,8));
		eBg->addChild(nameBgSp);

		Label* nameLab = LabelSystemFont(ptrNWI->strName,16);
		nameLab->setPosition(Vec2(nameBgSp->getContentSize().width / 2, nameBgSp->getContentSize().height/2));
		nameBgSp->addChild(nameLab);

		Sprite* LockSp = ReadSpriteName("new_ui/img_global/imgs_lock.png");
		LockSp->setAnchorPoint(Vec2(0.5,0));
		LockSp->setPosition(Vec2(10,1));
		nameBgSp->addChild(LockSp);

		bool iWarIsOpen = false;
		if (mWarOpenList[ptrNWI->iWarId]!= -1)
		{
			nameLab->setColor(Color3B(0, 255, 255));
			LockSp->setVisible(false);
			iWarIsOpen = true;
		}
		else
		{
			nameLab->setColor(Color3B::GRAY);
			addSpriteGray(eBg);
			addSpriteGray(nameBgSp);
		}

		if (iWarIsOpen&&mIt->second != -1)
		{
			Sprite* wanC = ReadSpriteName("new_ui/battle/icon_tuguan1.png");
			wanC->setPosition(Vec2(eBg->getContentSize().width - 5, eBg->getContentSize().height - 5));
			wanC->setAnchorPoint(Vec2(1,1));
			eBg->addChild(wanC);
		}

		RomanceDetailsInfo* ptrRDI = new RomanceDetailsInfo();
		ptrRDI->setEWarId(ptrNWI->iWarId);
		ptrRDI->setEIsOpen(iWarIsOpen);
		ptrRDI->setEBgSp(eBg);
		ptrRDI->setENameBgSp(nameBgSp);
		ptrRDI->setELockSp(LockSp);
		ptrRDI->setEName(nameLab);
		mShowElem[ptrNWI->iWarId] = ptrRDI;


		if (iMinX > vElemP.x)
			iMinX = vElemP.x;
		if (iMaxX < vElemP.x)
			iMaxX = vElemP.x;

		if (iMinY > vElemP.y)
			iMinY = vElemP.y;
		if (iMaxY < vElemP.y)
			iMaxY = vElemP.y;
	}

	mMapBoundary[0] = -(iMaxX - size.width / 2);
	mMapBoundary[1] = -(iMinX - size.width / 2);
	mMapBoundary[2] = -(iMaxY - size.height / 2);
	mMapBoundary[3] = -(iMinY - size.height / 2);


	if (iCurWarId > -1&&mShowElem.count(iCurWarId))
	{
		mShowLayerVec2 = Vec2(-(mShowElem[iCurWarId]->getEBgSp()->getPositionX() - size.width / 2), -(mShowElem[iCurWarId]->getEBgSp()->getPositionY() - size.height / 2));
		mShowElemLayer->setPosition(mShowLayerVec2);
	}
}

void RomanceDetailsLayer::AddShowElemLine()
{
	for (map<int, int>::iterator mIt = mWarList.begin(); mIt != mWarList.end(); mIt++)
	{
		Vec2 Start_P = mShowElem[mIt->first]->getEBgSp()->getPosition();
		CCLOG("spX;%.2f   spY:%0.2f",Start_P.x,Start_P.y);
		for (UInt i = 0; i < trData->m_mNetWarInfo[mIt->first]->mReach.size(); i++)
		{
			int iChildWarId = trData->m_mNetWarInfo[mIt->first]->mReach[i];
			if (mShowElem.count(iChildWarId))
			{
				Vec2 End_P = mShowElem[iChildWarId]->getEBgSp()->getPosition();
				CCLOG("epX;%.2f   epY:%0.2f", End_P.x, End_P.y);

				Vec2 Dircation_P = Start_P - End_P;
				float fLineLen = Start_P.distance(End_P);
				float fAngle = -CC_RADIANS_TO_DEGREES(Dircation_P.getAngle());
				fAngle += -180;
				CCLOG("angle%f",fAngle);

				Sprite* eLine = ReadSpriteName("new_ui/img_global/imgs_line.png");
				eLine->setAnchorPoint(Vec2(0, 0.5));
				eLine->setPosition(Vec2(Start_P));
				eLine->setRotation(fAngle);
				eLine->setScaleX(fLineLen / eLine->getContentSize().width);
				mShowElemLayer->addChild(eLine);
			}
		}
	}
}

bool RomanceDetailsLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	mClickIndex = -1;
	mClickStartVec2 = Vec2(0,0);

	for (map<int, RomanceDetailsInfo*>::iterator mIt = mShowElem.begin(); mIt != mShowElem.end();mIt++)
	{
		Rect elemBgBox = mIt->second->getEBgSp()->getBoundingBox();
		if (Rect(0, 0, elemBgBox.size.width, elemBgBox.size.height).containsPoint(mIt->second->getEBgSp()->convertToNodeSpace(touch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			mClickIndex = mIt->first;
			return true;
		}
	}
	return true;
}

void RomanceDetailsLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	Vec2 vDelta = touch->getDelta();
	mClickStartVec2 += vDelta;
	if (mClickIndex > -1 && (abs((int)mClickStartVec2.x)>1 || abs((int)mClickStartVec2.y)>1))
	{
		mClickIndex = -1;
	}

	mShowLayerVec2 += vDelta;

	if (mShowLayerVec2.x < mMapBoundary[0])
		mShowLayerVec2.x = mMapBoundary[0];
	else if (mShowLayerVec2.x>mMapBoundary[1])
		mShowLayerVec2.x = mMapBoundary[1];

	if (mShowLayerVec2.y < mMapBoundary[2])
		mShowLayerVec2.y = mMapBoundary[2];
	else if (mShowLayerVec2.y>mMapBoundary[3])
		mShowLayerVec2.y = mMapBoundary[3];

	mShowElemLayer->setPosition(mShowLayerVec2);
}

void RomanceDetailsLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (mClickIndex > -1)
	{
		ChessApplication::getInstance()->AddFunctionLayer(RomanceDetailsWarLayer::create(mShowElem[mClickIndex]->getEWarId(), mShowElem[mClickIndex]->getEIsOpen(),this));
	}
}

void RomanceDetailsLayer::ChildCloseLayer(int iNomal)
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

void RomanceDetailsLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(RomanceLayer::create(false));
	this->removeFromParent();
}

void RomanceDetailsLayer::clickTreasureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(TreasureBoxLayer::create(mMapId, NULL,BATTLE_ROMANCE));
}

void RomanceDetailsLayer::clickJuBenMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(RomanceInfoLayer::create(mMapId, this,false));
}

RomanceDetailsLayer::RomanceDetailsLayer()
{

}

RomanceDetailsLayer::~RomanceDetailsLayer()
{
	for (map<int, RomanceDetailsInfo*>::iterator mIt = mShowElem.begin(); mIt != mShowElem.end(); mIt++)
		delete mIt->second;
}


/////RomanceDetailsWarLayer

RomanceDetailsWarLayer* RomanceDetailsWarLayer::create(int iWarId, bool bIsOpen, EventDelegate* ptrDelegate)
{
	RomanceDetailsWarLayer* pRet = new RomanceDetailsWarLayer();
	if (pRet&&pRet->init(iWarId,bIsOpen,ptrDelegate))
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

bool RomanceDetailsWarLayer::init(int iWarId, bool bIsOpen, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	this->m_iWarId = iWarId;
	this->m_bIsOpen = bIsOpen;
	this->mDelegate = ptrDelegate;

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2());
	this->addChild(black_layer);

	DSize size = ChessViewTool::getInstance()->getFullSize();
	NetWarInfo* ptrNWI = trData->m_mNetWarInfo[iWarId];

	Sprite* bg = ReadSpriteName("new_ui/img_global/imgs_bg3.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	//312
	Label* nameLab = LabelSystemFont(ptrNWI->strName.c_str(),18);
	nameLab->setPosition(Vec2(iBgWidth/2,312));
	bg->addChild(nameLab);

	
	Label* infoLab = LabelSystemFont(__String::createWithFormat(ptrNWI->strInfo.c_str(),trData->actors[0].getActorName())->getCString(),16);
	infoLab->setDimensions(420,96);
	infoLab->setLineHeight(24);
	infoLab->setAnchorPoint(Vec2(0,1));
	infoLab->setPosition(Vec2(25, 286));
	infoLab->setColor(Color3B(102, 51, 0));
	bg->addChild(infoLab);

	//180
	Label* winCOrGetG = NULL;
	if (bIsOpen)
		winCOrGetG = LabelSystemFont(LL("bgkhd"), 18);
	else
		winCOrGetG = LabelSystemFont(LL("gqkqtj"), 18);
	winCOrGetG->setPosition(Vec2(iBgWidth/2,180));
	bg->addChild(winCOrGetG);

	if (bIsOpen)//获得奖励
	{
		addSpoilsElem(bg);
	}
	else//开启条件
	{
		addWinCondition(bg);
	}

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(0, 0));
	bg->addChild(mMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(RomanceDetailsWarLayer::clickCloseMenu, this));
	close_menu->setPosition(Vec2(iBgWidth - 15,iBgHeight -14));
	mMenu->addChild(close_menu);

	if (bIsOpen)
	{
		Sprite* DispatchSp_N = ReadSpriteName("new_ui/battle/battle_xyb.png");
		Sprite* DispatchSp_S = ReadSpriteName("new_ui/battle/battle_xyb.png");
		DispatchSp_S->setScale(0.9f);
		MenuItemSprite* DispatchMenuItem = MenuItemSprite::create(DispatchSp_N, DispatchSp_S, CC_CALLBACK_1(RomanceDetailsWarLayer::clickDispatchMenu, this));
		DispatchSp_S->setAnchorPoint(Vec2(0.5, 0.5));
		DispatchSp_S->setPosition(Vec2(DispatchSp_S->getContentSize().width / 2, DispatchSp_S->getContentSize().height / 2));
		DispatchMenuItem->setPosition(Vec2(iBgWidth - 57, 53));
		mMenu->addChild(DispatchMenuItem);
	}


	history->updateGuide(EMIL_GUIDE);
	return true;

}

void RomanceDetailsWarLayer::addWinCondition(Sprite* bg)
{
	if (trData->net_story_condition.count(m_iWarId))
	{
		int iLabIndex = 0;
		for (map<int, NetStoryCondition*>::iterator mIt = trData->net_story_condition[m_iWarId].begin(); mIt != trData->net_story_condition[m_iWarId].end(); mIt++)
		{
			bool bConIsOpen = false;
			int iFrontWarId = mIt->second->iGqId;
			if (mIt->second->iType == 1 && history->mMapDateInfo->getHeroicMapDateElem(iFrontWarId))
			{
				bConIsOpen = true;
			}
			else if (mIt->second->iType == 2 &&
				((history->m_mScriptCode.count(iFrontWarId) && history->m_mScriptCode[iFrontWarId].count(mIt->second->iVariable))
				|| (history->m_mScriptCode.count(GLOBAL_VAR_MAPID) && history->m_mScriptCode[GLOBAL_VAR_MAPID].count(mIt->second->iVariable))))
			{
				bConIsOpen = true;
			}
			//25,142
			int iPointY = 158 - iLabIndex * 26;
			Label* conLab = LabelSystemFont(mIt->second->strDescribe, 16);
			conLab->setAnchorPoint(Vec2(0, 1));
			conLab->setPosition(Vec2(25, iPointY));
			bg->addChild(conLab);
			iLabIndex++;

			if (bConIsOpen)
			{
				conLab->setColor(Color3B(0, 153, 0));
				Label* wancLab = LabelSystemFont(LL("wancheng"), 16);
				wancLab->setAnchorPoint(Vec2(0, 1));
				wancLab->setColor(Color3B(0, 153, 0));
				wancLab->setPosition(Vec2(33 + conLab->getContentSize().width, iPointY));
				bg->addChild(wancLab);
			}
			else
			{
				conLab->setColor(Color3B(102, 51, 0));
			}
		}
	}
}
void RomanceDetailsWarLayer::addSpoilsElem(Sprite* bg)
{
	if (trData->net_story_goods.count(m_iWarId))
	{
		int iElemIndex = 0;
		for (map<int, NetStoryGoods*>::iterator mIt = trData->net_story_goods[m_iWarId].begin(); mIt != trData->net_story_goods[m_iWarId].end(); mIt++)
		{

			bool bIsGet = false;

			if ((history->m_mScriptCode.count(m_iWarId) && history->m_mScriptCode[m_iWarId].count(mIt->second->iVariable))
				|| (history->m_mScriptCode.count(GLOBAL_VAR_MAPID) && history->m_mScriptCode[GLOBAL_VAR_MAPID].count(mIt->second->iVariable)))
			{
				bIsGet = true;
			}

			if (mIt->second->mHid.size() && mIt->second->mHid.begin()->second)
			{
				//TODO
				for (map<int, int>::iterator eIt = mIt->second->mHid.begin(); eIt != mIt->second->mHid.end(); eIt++)
				{
					HeroSprite* ptrHeroSp = HeroSprite::create(eIt->second);
					ptrHeroSp->setSupDrag(true);
					ptrHeroSp->setPosition(Vec2(50 + (iElemIndex % 7) * 60, 132 - (iElemIndex / 7) * 55));
					bg->addChild(ptrHeroSp);

					if (bIsGet)
					{
						Sprite* IsGetSp = ReadSpriteName("new_ui/img_global/imgs_go.png");
						IsGetSp->setAnchorPoint(Vec2(1, 0));
						IsGetSp->setPosition(Vec2(ptrHeroSp->getContentSize().width - 2, 2));
						ptrHeroSp->addChild(IsGetSp);
					}
					iElemIndex++;
					if (iElemIndex >= 14)  break;
				}
			}
			else if (iElemIndex<14&&mIt->second->mGoods.size() && mIt->second->mGoods.begin()->second)
			{
				for (map<int, int>::iterator eIt = mIt->second->mGoods.begin(); eIt != mIt->second->mGoods.end();eIt++)
				{
					GoodsSprite* ptrGoodsSp = GoodsSprite::create(eIt->first, eIt->second);
					ptrGoodsSp->setSupDrag(true);
					ptrGoodsSp->setPosition(Vec2(50 + (iElemIndex % 7) * 60, 132 - (iElemIndex / 7) * 55));
					bg->addChild(ptrGoodsSp);

					if (bIsGet)
					{
						Sprite* IsGetSp = ReadSpriteName("new_ui/img_global/imgs_go.png");
						IsGetSp->setAnchorPoint(Vec2(1, 0));
						IsGetSp->setPosition(Vec2(ptrGoodsSp->getContentSize().width - 2, 2));
						ptrGoodsSp->addChild(IsGetSp);
					}
					iElemIndex++;
					if (iElemIndex >= 14)  break;
				}
			}
		}
	}
}

void RomanceDetailsWarLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void RomanceDetailsWarLayer::clickDispatchMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->m_iBattleType = BATTLE_ROMANCE;
	history->m_iCurWarId = m_iWarId;
	history->m_iBattleStage = 0;
	ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(mDelegate));
	this->removeFromParent();
}

bool RomanceDetailsWarLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void RomanceDetailsWarLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void RomanceDetailsWarLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
}

RomanceDetailsWarLayer::RomanceDetailsWarLayer()
{

}

RomanceDetailsWarLayer::~RomanceDetailsWarLayer()
{

}