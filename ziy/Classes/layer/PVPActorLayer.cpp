//
//  PVPActorLayer.cpp
//  TRGame
//
//  Created by Jimmy on 17/03/23.
//
//

#include "PVPActorLayer.h"
#include "PVPArrayLayer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPActorLayer::PVPActorLayer()
{

}

PVPActorLayer::~PVPActorLayer()
{

}

PVPActorLayer* PVPActorLayer::create(PVPActorData* ptrActor)
{
	PVPActorLayer* pRet = new PVPActorLayer();
	if(pRet&&pRet->init(ptrActor))
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

bool PVPActorLayer::init(PVPActorData* ptrActor)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrActor = ptrActor;
	initControl();
	return true;
}

void PVPActorLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	auto spBG = ReadSpriteName("new_ui/pvp/pvp_bg3.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//挑战对象
	PVPActorSprite* spActor = PVPActorSprite::create(false);
	spActor->updateSprite(m_ptrActor);
	spActor->setPosition(point + Vec2(-118, 60));
	this->addChild(spActor);

	//胜场
	Label* labVictory = LabelSystemFont(LL("pvpvictorytimes"), 16);
	labVictory->setAnchorPoint(Vec2(0.0f, 0.5f));
	labVictory->setPosition(point + Vec2(-180, -80));
	labVictory->setColor(Color3B::YELLOW);
	labVictory->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(labVictory);

	Label* labVicVal = LabelSystemFont(StringUtils::format("%d", m_ptrActor->getWinNum()), 16);
	labVicVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labVicVal->setPosition(labVictory->getPosition() + Vec2(labVictory->getContentSize().width + 5, 0));
	labVicVal->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(labVicVal);

	//败场
	Label* labDefeat = LabelSystemFont(LL("pvpdefeattimes"), 16);
	labDefeat->setAnchorPoint(Vec2(0.0f, 0.5f));
	labDefeat->setPosition(point + Vec2(-180, -115));
	labDefeat->setColor(Color3B::YELLOW);
	labDefeat->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(labDefeat);

	Label* labDefVal = LabelSystemFont(StringUtils::format("%d", m_ptrActor->getLoseNum()), 16);
	labDefVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labDefVal->setPosition(labDefeat->getPosition() + Vec2(labDefeat->getContentSize().width + 5, 0));
	labDefVal->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(labDefVal);

	//出战武将
	map<int, HeroElem*> mHeroList = m_ptrActor->m_mHero;
	int iIndex = 0;
	for (map<int, HeroElem*>::iterator it = mHeroList.begin(); it != mHeroList.end(); it++, iIndex++)
	{
		HeroElem* ptrHero = it->second;
		Vec2 vecPos = point + Vec2(25 + iIndex % 3 * 75, 64 - iIndex / 3 * 81);
		HistoryActor* ptrActor = &history->actors[ptrHero->getHeroID() - 1];

		//背景
		auto spActorBG = ReadSpriteName("new_ui/battle/battle_bg2.png");
		spActorBG->setPosition(vecPos);
		this->addChild(spActorBG);

		//武器
		int iFashion2 = ptrHero->getFashion2();
		if (iFashion2)
		{
			int iZOrder = 0;
			string strPath2 = StringUtils::format("img/Nunits/%d_%d_mov.png", 100, iFashion2);
			auto spFashion2 = ReadSpriteNormal(strPath2);
			spFashion2->setPosition(vecPos + Vec2(0, 23));
			int iWidth = spFashion2->getContentSize().width;
			spFashion2->setTextureRect(Rect(0, 6 * iWidth, iWidth, iWidth));
			if (trData->net_hero_dress[iFashion2]->mMov[6])
			{
				iZOrder = 1;
			}
			this->addChild(spFashion2, iZOrder);
		}

		//形象
		int iFashion1 = ptrHero->getFashion1();
		int iGender = ptrHero->getGender();
		if (iGender < 1)
		{
			iGender = trData->actors[ptrActor->actor_id].iGender;
		}
		string strPath1 = "";
		if (iFashion1)
		{
			strPath1 = StringUtils::format("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
		}
		else
		{
			strPath1 = ptrActor->getActorSImgPath("mov", 0);
		}
		auto spFashion1 = ReadSpriteNormal(strPath1);
		spFashion1->setPosition(vecPos + Vec2(0, 23));
		int iWidth = spFashion1->getContentSize().width;
		spFashion1->setTextureRect(Rect(0, 6 * iWidth, iWidth, iWidth));
		this->addChild(spFashion1);

		//等级
		Label* labLevel = LabelSystemFont(StringUtils::format("%d", ptrHero->getLV()), 14);
		labLevel->setAnchorPoint(Vec2(0.0f, 0.5f));
		labLevel->setPosition(vecPos + Vec2(12, 35));
		this->addChild(labLevel);

		//名字
		string strName = "";
		if (ptrHero->getHeroID() == 1)
		{
			strName = m_ptrActor->getName();
		}
		else
		{
			strName = trData->actors[ptrHero->getHeroID() - 1].getActorName();
		}
		int iQuality = ptrHero->getQuality() > 0 ? ptrHero->getQuality() : 1;
		Label* labName = LabelSystemFont(strName, 16);
		labName->setPosition(vecPos + Vec2(0, -10));
		labName->setColor(ITools::getInstance()->getActorPzColor(iQuality));
		labName->enableOutline(Color4B::BLACK, 1);
		this->addChild(labName);
	}

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//挑战
	Sprite* spFightN = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	Sprite* spFightS = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	spFightS->setScale(fScale);
	MenuItemSprite* btnFight = MenuItemSprite::create(spFightN, spFightS, CC_CALLBACK_1(PVPActorLayer::callBackFight, this));
	spFightS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spFightS->setPosition(Vec2(spFightS->getContentSize().width / 2, spFightS->getContentSize().height / 2));
	btnFight->setPosition(point + Vec2(100, -102));
	ptrMenu->addChild(btnFight);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PVPActorLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(218, 146));
	ptrMenu->addChild(btnClose);
}

void PVPActorLayer::callBackFight(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (PVPData::getInstance()->getEntryTimes() < 1)
	{
		ITools::getInstance()->createGlobalPrompt(LL("pvptimesnotenough"), false);
		return;
	}

	if (PVPData::getInstance()->getRandID() <= m_ptrActor->getRankID())
	{
		ITools::getInstance()->createGlobalPrompt(LL("noselectyourself"), false);
	}
	else
	{
		PVPData::getInstance()->setSelectRank(m_ptrActor->getRankID());
		this->getParent()->addChild(PVPArrayLayer::create(ARRAY_ATTACK));
		this->removeFromParent();
	}
}

void PVPActorLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}