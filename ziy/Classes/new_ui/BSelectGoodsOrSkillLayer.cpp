#include "BSelectGoodsOrSkillLayer.h"
#include "Defines.h"
#include "BSelectGoodsOrSkillTableView.h"
#include "BattleScreen.h"


BSelectGoodsOrSkillLayer * BSelectGoodsOrSkillLayer::create(BattleControlerPtr BCHandle, int iShowType)
{
	BSelectGoodsOrSkillLayer* pRet = new BSelectGoodsOrSkillLayer();
	if (pRet&&pRet->init(BCHandle, iShowType))
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

bool BSelectGoodsOrSkillLayer::init(BattleControlerPtr BCHandle, int iShowType)
{
	if (!Layer::init())
	{
		return false;
	}
	mShowType = iShowType;
	mHandle = BCHandle;
	curActor = history->cur_actor;

	__String* icon_path = NULL;


	if (iShowType == BSELECT_SHOW_GOODS)
	{
		icon_path = __String::create("new_ui/battle/use_item.png");
		GetGoodsDateList();
	}
	else
	{
		icon_path = __String::create("new_ui/battle/use_skill.png");
		GetSkillDateList();
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/battle/use_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);
	mBg = bg;

	int iPointX = 0;
	int iPointY = 355;
	//0,355
	Sprite* type_icon = ReadSpriteName(icon_path->getCString());
	type_icon->setAnchorPoint(Vec2(0,0.5f));
	type_icon->setPosition(Vec2(iPointX, iPointY));
	bg->addChild(type_icon);

	iPointX += type_icon->getContentSize().width + 10;
	Label* actor_name = LabelSystemFont(trData->actors[curActor->actor_id].getActorName(),18);
	actor_name->setAnchorPoint(Vec2(0, 0.5f));
	actor_name->setColor(Color3B(153,51,0));
	actor_name->setPosition(Vec2(iPointX, iPointY));
	bg->addChild(actor_name);

	//220,355
	iPointX = 220;
	Sprite* hpSp = ReadSpriteName("new_ui/battle/use_hp.png");
	hpSp->setPosition(Vec2(iPointX, iPointY));
	bg->addChild(hpSp);

	iPointX += 3 + hpSp->getContentSize().width / 2;
	Sprite* hpBarBgSp = ReadSpriteName("new_ui/battle/use_box.png");
	hpBarBgSp->setAnchorPoint(Vec2(0,0.5f));
	hpBarBgSp->setPosition(Vec2(iPointX, iPointY));
	bg->addChild(hpBarBgSp);

	float fCurNum = curActor->GetCurHp();
	float fMaxNum = curActor->GetBaseHp() + curActor->GetGoodsAddHp();
	float fProportion = fCurNum / fMaxNum;
	
	Sprite* hpBarSp = ReadSpriteName("new_ui/battle/use_box_hp.png");
	hpBarSp->setAnchorPoint(Vec2(0,0.5f));
	hpBarSp->setScaleX(fProportion);
	hpBarSp->setPosition(Vec2(4, hpBarBgSp->getContentSize().height/2));
	hpBarBgSp->addChild(hpBarSp);

	Label* hpNumLab = LabelSystemFont(StringUtils::format("%d/%d", (int)fCurNum, (int)fMaxNum).c_str(), 16);
	hpNumLab->setPosition(Vec2(hpBarBgSp->getContentSize().width / 2, hpBarBgSp->getContentSize().height / 2));
	hpBarBgSp->addChild(hpNumLab);

	//412
	iPointX = 412;
	Sprite* mpSp = ReadSpriteName("new_ui/battle/use_mp.png");
	mpSp->setPosition(Vec2(iPointX, iPointY));
	bg->addChild(mpSp);

	iPointX += 3+ mpSp->getContentSize().width/2;
	Sprite* mpBarBgSp = ReadSpriteName("new_ui/battle/use_box.png");
	mpBarBgSp->setAnchorPoint(Vec2(0, 0.5f));
	mpBarBgSp->setPosition(Vec2(iPointX, iPointY));
	bg->addChild(mpBarBgSp);

	fCurNum = curActor->GetCurMp();
	fMaxNum = curActor->GetBaseMp() + curActor->GetGoodsAddMp();
	fProportion = fCurNum / fMaxNum;

	Sprite* mpBarSp = ReadSpriteName("new_ui/battle/use_box_mp.png");
	mpBarSp->setAnchorPoint(Vec2(0, 0.5f));
	mpBarSp->setScaleX(fProportion);
	mpBarSp->setPosition(Vec2(4, mpBarBgSp->getContentSize().height / 2));
	mpBarBgSp->addChild(mpBarSp);

	Label* mpNumLab = LabelSystemFont(StringUtils::format("%d/%d",(int)fCurNum,(int)fMaxNum).c_str(),16);
	mpNumLab->setPosition(Vec2(mpBarBgSp->getContentSize().width/2, mpBarBgSp->getContentSize().height / 2));
	mpBarBgSp->addChild(mpNumLab);

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(bg->getContentSize().width - 20, bg->getContentSize().height - 20));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(BSelectGoodsOrSkillLayer::clickCloseLayer, this));
	mCloseMenu->addChild(close_menu);


	BSelectGoodsOrSkillTableView* ptrListView = BSelectGoodsOrSkillTableView::create(&mElemDate,this);
	bg->addChild(ptrListView);

	return true;
}



bool BSelectGoodsOrSkillLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	mClickRemoveThis = false;
	Rect bgBox = mBg->getBoundingBox();
	if (!Rect(0, 0, bgBox.size.width, bgBox.size.height).containsPoint(mBg->convertToNodeSpace(touch->getLocation())))
	{
		mClickRemoveThis = true;
	}

	return true;
}
void BSelectGoodsOrSkillLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}
void BSelectGoodsOrSkillLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (mClickRemoveThis)
	{
		RemoveThisLayer();
	}
}

void BSelectGoodsOrSkillLayer::GetGoodsDateList()
{
	int iGoodsIndex = 0;
	for (map<int, int>::iterator it = history->m_mConsuNum.begin(); it != history->m_mConsuNum.end(); it++)
	{
		int iGoodsId = it->first;
		int iCount = it->second;
		if (iCount > 0 && trData->isConsume(iGoodsId))
		{
			GoodsOrSkillDatePtr ptrGOSElem = new GoodsOrSkillDate();
			ptrGOSElem->setEINDEX(iGoodsIndex);
			ptrGOSElem->setESHOWTYPE(mShowType);
			ptrGOSElem->setEID(iGoodsId);
			ptrGOSElem->setENUM(iCount);
			mElemDate[iGoodsIndex++] = ptrGOSElem;
		}
	}
}

void BSelectGoodsOrSkillLayer::GetSkillDateList()
{
	curActor->setIsCopySkill(false);
	int iGoodsIndex = 0;
	int iHeroSkillId = trData->actors[curActor->actor_id].skill;
	if (trData->net_hero_skill.count(iHeroSkillId))
	{
		NetHeroSkillPtr ptrNSkill = trData->net_hero_skill[iHeroSkillId];

		for (map<int, int>::iterator EIt = ptrNSkill->skill.begin(); EIt != ptrNSkill->skill.end(); EIt++)
		{
			int iSkillId = EIt->first;
			if (!curActor->CurSkillLevelIsNull(iSkillId))
			{
				GoodsOrSkillDatePtr ptrGOSElem = new GoodsOrSkillDate();
				ptrGOSElem->setEINDEX(iGoodsIndex);
				ptrGOSElem->setESHOWTYPE(mShowType);
				ptrGOSElem->setEID(iSkillId);
				if (trData->skills[iSkillId - 1].iTimes < 1)
					ptrGOSElem->setENUM(trData->skills[iSkillId - 1].GetMpConsume(curActor));
				else
					ptrGOSElem->setENUM(trData->skills[iSkillId - 1].iTimes - curActor->mSkillUseNumLimt[iSkillId - 1]);
				if (ptrGOSElem->getENUM() < 0)
					ptrGOSElem->setENUM(0);
				ptrGOSElem->setELEVEL(curActor->GetCurSkillLevel(iSkillId));
				ptrGOSElem->setIsCopy(false);

				if (!trData->skills[iSkillId - 1].BeingAbleToCastSpells(curActor))
				{
					ptrGOSElem->setEENABLECLICK(false);
				}

				mElemDate[iGoodsIndex++] = ptrGOSElem;
			}
		}
	}

	for (map<int, CopySkill*>::iterator it = curActor->m_mCopySkill.begin(); it != curActor->m_mCopySkill.end(); it++)
	{
		CopySkill* ptrSkill = it->second;

		GoodsOrSkillDatePtr ptrGOSElem = new GoodsOrSkillDate();
		ptrGOSElem->setEINDEX(iGoodsIndex);
		ptrGOSElem->setESHOWTYPE(mShowType);
		ptrGOSElem->setEID(ptrSkill->iSkillId);
		if(trData->skills[ptrSkill->iSkillId - 1].iTimes < 1)
			ptrGOSElem->setENUM(trData->skills[ptrSkill->iSkillId - 1].GetMpConsume(curActor));
		else
			ptrGOSElem->setENUM(trData->skills[ptrSkill->iSkillId - 1].iTimes - curActor->mSkillUseNumLimt[ptrSkill->iSkillId - 1]);

		if (ptrGOSElem->getENUM() < 0)
			ptrGOSElem->setENUM(0);
		ptrGOSElem->setELEVEL(curActor->GetCurSkillLevel(ptrSkill->iSkillId));
		ptrGOSElem->setIsCopy(true);

		if(!trData->skills[ptrSkill->iSkillId - 1].BeingAbleToCastSpells(curActor) || ptrSkill->iUsed)
		{
			ptrGOSElem->setEENABLECLICK(false);
		}
		mElemDate[iGoodsIndex++] = ptrGOSElem;
	}

	for (map<int, int>::iterator EIt = curActor->m_mSeidAddSkill.begin(); EIt != curActor->m_mSeidAddSkill.end(); EIt++)
	{
		int iSkillId = EIt->first;

		GoodsOrSkillDatePtr ptrGOSElem = new GoodsOrSkillDate();
		ptrGOSElem->setEINDEX(iGoodsIndex);
		ptrGOSElem->setESHOWTYPE(mShowType);
		ptrGOSElem->setEID(iSkillId);
		//ptrGOSElem->setENUM(trData->skills[iSkillId - 1].GetMpConsume(curActor));
		if (trData->skills[iSkillId - 1].iTimes < 1)
			ptrGOSElem->setENUM(trData->skills[iSkillId - 1].GetMpConsume(curActor));
		else
			ptrGOSElem->setENUM(trData->skills[iSkillId - 1].iTimes - curActor->mSkillUseNumLimt[iSkillId - 1]);

		if (ptrGOSElem->getENUM() < 0)
			ptrGOSElem->setENUM(0);
		ptrGOSElem->setELEVEL(curActor->GetCurSkillLevel(iSkillId));
		ptrGOSElem->setIsCopy(false);

		if (!trData->skills[iSkillId - 1].BeingAbleToCastSpells(curActor))
		{
			ptrGOSElem->setEENABLECLICK(false);
		}

		mElemDate[iGoodsIndex++] = ptrGOSElem;
	}
}

void BSelectGoodsOrSkillLayer::clickCloseLayer(Ref * sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	RemoveThisLayer();
}

void BSelectGoodsOrSkillLayer::RunFunction(int iNomal)
{
	if (iNomal > -1)
	{
		int iId = mElemDate[iNomal]->getEID();
		if (mShowType == BSELECT_SHOW_SKILL)
		{
			if (trData->skill_num >= iId)
			{
				curActor->setIsCopySkill(mElemDate[iNomal]->getIsCopy());
				mHandle->SelectSkill(&trData->skills[iId - 1]);
			}
		}
		else
		{
			mHandle->SelectGood(trData->m_mGoodsItem[iId]);
		}
	}
	RemoveThisLayer(false);
}

void BSelectGoodsOrSkillLayer::RemoveThisLayer(bool bOnlyRemove)
{

	if (bOnlyRemove)
	{
		if (mShowType == BSELECT_SHOW_SKILL)
			mHandle->SelectSkill(NULL);
		else
			mHandle->SelectGood(NULL);
	}

	mHandle->scr->mFunctionLayer = NULL;
	this->removeFromParent();
}

BSelectGoodsOrSkillLayer::BSelectGoodsOrSkillLayer()
{

}

BSelectGoodsOrSkillLayer::~BSelectGoodsOrSkillLayer()
{
	for (size_t i = 0; i < mElemDate.size(); i++)
	{
		delete mElemDate[i];
	}
}