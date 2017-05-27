#include "SuitInfoLayer.h"
#include "Defines.h"

SuitInfoLayer* SuitInfoLayer::create(int iGoodsId)
{
	SuitInfoLayer* pRet = new SuitInfoLayer();
	if (pRet&&pRet->init(iGoodsId))
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

bool SuitInfoLayer::init(int iGoodsId)
{
	if (!Layer::init())
	{
		return false;
	}
	this->mGoodsId = iGoodsId;
	this->mCurElemIndex = 0;
	GetDataElem();

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);

	bg = ReadSpriteName("new_ui/img_global/suit_bg1.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	//102,51,0
	Label* nameLab = LabelSystemFont("",20);
	nameLab->setPosition(Vec2(iBgWidth/2,iBgHeight- 22));
	nameLab->enableOutline(Color4B(102,51,0,255),1);
	bg->addChild(nameLab);


	//武器
	Sprite* m_spWeaponBG = ReadSpriteName("new_ui/img_global/item_box_bg.png");
	m_spWeaponBG->setPosition(Vec2(iBgWidth / 2 - 78, 226));
	bg->addChild(m_spWeaponBG);

	int iGoogsBgWidth = m_spWeaponBG->getContentSize().width;

	Sprite* m_spWeapon = ReadSpriteNormal("img/Items/a1.png");
	m_spWeapon->setScale(0.8f);
	m_spWeapon->setPosition(Vec2(iGoogsBgWidth / 2, iGoogsBgWidth/2));
	m_spWeaponBG->addChild(m_spWeapon);

	Label* m_spWeaponName = LabelSystemFont("", 14);
	m_spWeaponName->setAnchorPoint(Vec2(0.5,1));
	m_spWeaponName->setPosition(Vec2(iGoogsBgWidth / 2, -4));
	m_spWeaponName->enableOutline(Color4B(102, 102, 102, 255), 1);
	m_spWeaponBG->addChild(m_spWeaponName);

	//防具
	Sprite* m_spCoatBG = ReadSpriteName("new_ui/img_global/item_box_bg.png");
	m_spCoatBG->setPosition(Vec2(iBgWidth/2, 226));
	bg->addChild(m_spCoatBG);

	Sprite* m_spCoat = ReadSpriteNormal("img/Items/a2.png");
	m_spCoat->setScale(0.8f);
	m_spCoat->setPosition(Vec2(iGoogsBgWidth / 2, iGoogsBgWidth / 2));
	m_spCoatBG->addChild(m_spCoat);

	Label* m_spCoatName = LabelSystemFont("", 14);
	m_spCoatName->setAnchorPoint(Vec2(0.5, 1));
	m_spCoatName->setPosition(Vec2(iGoogsBgWidth / 2, -4));
	m_spCoatName->enableOutline(Color4B(102, 102, 102, 255), 1);
	m_spCoatBG->addChild(m_spCoatName);

	//辅助
	Sprite* m_spAssistBG = ReadSpriteName("new_ui/img_global/item_box_bg.png");
	m_spAssistBG->setPosition(Vec2(iBgWidth / 2 + 78, 226));
	bg->addChild(m_spAssistBG);

	Sprite* m_spAssist = ReadSpriteNormal("img/Items/a3.png");
	m_spAssist->setScale(0.8f);
	m_spAssist->setPosition(Vec2(iGoogsBgWidth / 2, iGoogsBgWidth / 2));
	m_spAssistBG->addChild(m_spAssist);

	Label* m_spAssistName = LabelSystemFont("", 14);
	m_spAssistName->setAnchorPoint(Vec2(0.5, 1));
	m_spAssistName->setPosition(Vec2(iGoogsBgWidth / 2, -4));
	m_spAssistName->enableOutline(Color4B(102, 102, 102, 255), 1);
	m_spAssistBG->addChild(m_spAssistName);


	Label* seidInfo1Lab = LabelSystemFont("",16);
	seidInfo1Lab->setPosition(Vec2(iBgWidth/2,142));
	seidInfo1Lab->enableOutline(Color4B(102, 102, 102, 255), 1);
	bg->addChild(seidInfo1Lab);

	Label* seidInfo2Lab = LabelSystemFont("", 16);
	seidInfo2Lab->setPosition(Vec2(iBgWidth / 2, 122));
	seidInfo2Lab->enableOutline(Color4B(102, 102, 102, 255), 1);
	bg->addChild(seidInfo2Lab);

	Label* actorInfoLab = LabelSystemFont("", 16);
	actorInfoLab->setPosition(Vec2(iBgWidth / 2, 66));
	actorInfoLab->enableOutline(Color4B(102, 102, 102, 255), 1);
	bg->addChild(actorInfoLab);

	Label* actorSuitLab = LabelSystemFont("", 16);
	actorSuitLab->setPosition(Vec2(iBgWidth / 2, 46));
	actorSuitLab->enableOutline(Color4B(102, 102, 102, 255), 1);
	bg->addChild(actorSuitLab);

	mShowElem.seteNameLab(nameLab);
	mShowElem.seteWeaponBg(m_spWeaponBG);
	mShowElem.seteWeaponImg(m_spWeapon);
	mShowElem.seteWeaponName(m_spWeaponName);
	mShowElem.seteCoatBg(m_spCoatBG);
	mShowElem.seteCoatImg(m_spCoat);
	mShowElem.seteCoatName(m_spCoatName);
	mShowElem.seteAssistBg(m_spAssistBG);
	mShowElem.seteAssistImg(m_spAssist);
	mShowElem.seteAssistName(m_spAssistName);
	mShowElem.seteSeidInfo1Lab(seidInfo1Lab);
	mShowElem.seteSeidInfo2Lab(seidInfo2Lab);
	mShowElem.seteActorInfoLab(actorInfoLab);
	mShowElem.seteActorSuitLab(actorSuitLab);


	//左右箭头
	Sprite* ProNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* ProSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	ProSSp->setScale(0.9f);
	int iSwitchWidth = ProNSp->getContentSize().width;

	Menu* proMenu = Menu::create();
	proMenu->setPosition(Vec2(iSwitchWidth / 2, iBgHeight / 2));
	bg->addChild(proMenu);

	MenuItemSprite* pro_mItem = MenuItemSprite::create(ProNSp, ProSSp, CC_CALLBACK_1(SuitInfoLayer::clickProMenu, this));
	proMenu->addChild(pro_mItem);
	ProSSp->setAnchorPoint(Vec2(0.5f,0.5f));
	ProSSp->setPosition(Vec2(ProSSp->getContentSize().width / 2, ProSSp->getContentSize().height / 2));

	Sprite* NextNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* NextSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	NextNSp->setFlippedX(true);
	NextSSp->setFlippedX(true);
	NextSSp->setScale(0.9f);

	Menu* NextMenu = Menu::create();
	NextMenu->setPosition(Vec2(iBgWidth-iSwitchWidth / 2, iBgHeight / 2));
	bg->addChild(NextMenu);

	MenuItemSprite* Next_mItem = MenuItemSprite::create(NextNSp, NextSSp, CC_CALLBACK_1(SuitInfoLayer::clickNextMenu, this));
	NextMenu->addChild(Next_mItem);
	NextSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	NextSSp->setPosition(Vec2(NextSSp->getContentSize().width / 2, NextSSp->getContentSize().height / 2));
	
	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(iBgWidth-5, iBgHeight -5));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(SuitInfoLayer::clickCloseMenu, this));
	mCloseMenu->addChild(close_menu);

	if (mDataElem.size() <= 1)
	{
		proMenu->setVisible(false);
		NextMenu->setVisible(false);
	}

	int mElemNum = mDataElem.size();
	int iOffsetX = 18;

	int iStarPointX = iBgWidth / 2 - (mElemNum - 1) * iOffsetX / 2;

	for (int i = 0; i < mElemNum; i++)
	{
		mElemDot[i] = ReadSpriteName("new_ui/img_global/suit_bnt3.png");
		mElemDot[i]->setPosition(Vec2(iStarPointX + i*iOffsetX, 20));
		bg->addChild(mElemDot[i]);
	}
	mCurDotSp = mElemDot[mCurElemIndex];

	UpdateElemShow();

	return true;
}

void SuitInfoLayer::UpdateElemShow()
{
	SuitInfoElem* ptrSuitinfoE = mDataElem[mCurElemIndex];
	map<int, Sprite*> mShowGoodsBg;
	int iShowGoodsBgIndex = 0;

	mCurDotSp->setTexture(ReadSpriteTexture("new_ui/img_global/suit_bnt3.png"));
	mCurDotSp = mElemDot[mCurElemIndex];
	mCurDotSp->setTexture(ReadSpriteTexture("new_ui/img_global/suit_bnt2.png"));

	mShowElem.geteNameLab()->setString(ptrSuitinfoE->geteSuitNameS());

	if (!ptrSuitinfoE->geteWeaponS().empty())
	{
		mShowElem.geteWeaponBg()->setVisible(true);
		mShowElem.geteWeaponImg()->setTexture(GetSpriteTexture(ptrSuitinfoE->geteWeaponS().c_str()));
		mShowElem.geteWeaponName()->setString(ptrSuitinfoE->geteWeaponName().c_str());
		mShowGoodsBg[iShowGoodsBgIndex++] = mShowElem.geteWeaponBg();
	}
	else
	{
		mShowElem.geteWeaponBg()->setVisible(false);
	}

	if (!ptrSuitinfoE->geteCoatS().empty())
	{
		mShowElem.geteCoatBg()->setVisible(true);
		mShowElem.geteCoatImg()->setTexture(GetSpriteTexture(ptrSuitinfoE->geteCoatS().c_str()));
		mShowElem.geteCoatName()->setString(ptrSuitinfoE->geteCoatName().c_str());
		mShowGoodsBg[iShowGoodsBgIndex++] = mShowElem.geteCoatBg();
	}
	else
	{
		mShowElem.geteCoatBg()->setVisible(false);
	}

	if (!ptrSuitinfoE->geteAssistS().empty())
	{
		mShowElem.geteAssistBg()->setVisible(true);
		mShowElem.geteAssistImg()->setTexture(GetSpriteTexture(ptrSuitinfoE->geteAssistS().c_str()));
		mShowElem.geteAssistName()->setString(ptrSuitinfoE->geteAssistName().c_str());
		mShowGoodsBg[iShowGoodsBgIndex++] = mShowElem.geteAssistBg();
	}
	else
	{
		mShowElem.geteAssistBg()->setVisible(false);
	}

	int iBgWidth = bg->getContentSize().width;
	int iStarX = (iBgWidth-78 * 2)/2;
	if (mShowGoodsBg.size() <= 2)
		iStarX = (iBgWidth - 78) / 2;

	for (int i = 0; i < (int)mShowGoodsBg.size(); i++)
	{
		mShowGoodsBg[i]->setPositionX(iStarX + i*78);
	}

	map<int, Label*> mSeidInfo;
	mSeidInfo[0] = mShowElem.geteSeidInfo1Lab();
	mSeidInfo[1] = mShowElem.geteSeidInfo2Lab();
	int iSeidInfoIndex = 0;
	for (map<int, int>::iterator eIt = ptrSuitinfoE->mSeid.begin(); eIt != ptrSuitinfoE->mSeid.end(); eIt++)
	{
		mSeidInfo[iSeidInfoIndex++]->setString(trData->m_mSeid[eIt->first]->Info);

		if (iSeidInfoIndex > 1)  break;
	}
	for (int i = iSeidInfoIndex; i < 2; i++)
		mSeidInfo[iSeidInfoIndex++]->setString("");

	if (ptrSuitinfoE->geteIsActorSuit())
	{
		mShowElem.geteActorInfoLab()->setVisible(true);
		mShowElem.geteActorInfoLab()->setString(ptrSuitinfoE->geteSuitInfoS().c_str());

		mShowElem.geteActorSuitLab()->setVisible(true);
		map<int, int>::iterator eIt = ptrSuitinfoE->mActorSeid.begin();
		mShowElem.geteActorSuitLab()->setString(trData->m_mSeid[eIt->first]->Info);
	}
	else
	{
		mShowElem.geteActorInfoLab()->setVisible(false);
		mShowElem.geteActorSuitLab()->setVisible(false);
	}

}

void SuitInfoLayer::GetDataElem()
{
	GoodsItem* ptrGoods = trData->m_mGoodsItem[mGoodsId];
	int iElemIndex = 0;

	for (map<int, int>::iterator mIt = ptrGoods->mSuitList.begin(); mIt != ptrGoods->mSuitList.end(); mIt++)
	{
		if (mIt->first)
		{
			NetSuitDataPtr ptrSuitData = trData->net_suit_data[mIt->first];
			GoodsItem* ptrOtherGoogs = NULL;
			SuitInfoElem* ptrSuitInfoElem = new SuitInfoElem();

			for (map<int, int>::iterator iIt = ptrSuitData->item_id.begin(); iIt != ptrSuitData->item_id.end(); iIt++)
			{
				if (!iIt->first) continue;

				ptrOtherGoogs = trData->m_mGoodsItem[iIt->first];

				if (ptrOtherGoogs->m_iGoodType == GOOD_TYPE_WEAPON|| ptrOtherGoogs->isWapFashion())
				{
					ptrSuitInfoElem->seteWeaponS(history->GetGoodsImagePath(ptrOtherGoogs->iGoodsId).c_str());
					ptrSuitInfoElem->seteWeaponName(ptrOtherGoogs->strName);
				}
				else if (ptrOtherGoogs->m_iGoodType == GOOD_TYPE_COAT|| ptrOtherGoogs->isFashion())
				{
					ptrSuitInfoElem->seteCoatS(history->GetGoodsImagePath(ptrOtherGoogs->iGoodsId).c_str());
					ptrSuitInfoElem->seteCoatName(ptrOtherGoogs->strName);
				}
				else if (ptrOtherGoogs->m_iGoodType == GOOD_TYPE_ASSIST)
				{
					ptrSuitInfoElem->seteAssistS(history->GetGoodsImagePath(ptrOtherGoogs->iGoodsId).c_str());
					ptrSuitInfoElem->seteAssistName(ptrOtherGoogs->strName);
				}
			}

			ptrSuitInfoElem->seteIndex(mIt->first);

			ptrSuitInfoElem->mSeid = ptrSuitData->seid1;

			ptrSuitInfoElem->mActorSeid = ptrSuitData->seid2;

			ptrSuitInfoElem->seteSuitNameS(ptrSuitData->name);

			ptrSuitInfoElem->seteSuitInfoS(ptrSuitData->info);

			if (ptrSuitData->hid.size() == 1 && ptrSuitData->hid.count(0))
				ptrSuitInfoElem->seteIsActorSuit(0);
			else
				ptrSuitInfoElem->seteIsActorSuit(1);

			mDataElem[iElemIndex++] = ptrSuitInfoElem;
		}
	}
}

void SuitInfoLayer::clickProMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mDataElem.size() == 1)
		return;

	if (mCurElemIndex > 0)
	{
		mCurElemIndex--;
	}
	else
	{
		mCurElemIndex = mDataElem.size() - 1;
	}

	UpdateElemShow();
}

void SuitInfoLayer::clickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mDataElem.size() == 1)
		return;

	if (mCurElemIndex < (int)(mDataElem.size() - 1))
	{
		mCurElemIndex++;
	}
	else
	{
		mCurElemIndex = 0;
	}

	UpdateElemShow();
}

void SuitInfoLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

SuitInfoLayer::SuitInfoLayer()
{

}

SuitInfoLayer::~SuitInfoLayer()
{
	for (int i = 0; i < (int)mDataElem.size(); i++)
		delete mDataElem[i];
	mDataElem.clear();
}