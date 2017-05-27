//
//  NoticeActOpenLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/22.
//
//

#include "Common.h"
#include "main.h"
#include "NoticeActOpenLayer.h"
#include "Net_HomePage.h"
#include "HeroicAmbitionLayer.h"
#include "DestroySixCountriesLayer.h"
#include "PubDrinkLayer.h"

NoticeActOpenLayer::NoticeActOpenLayer()
{

}

NoticeActOpenLayer::~NoticeActOpenLayer()
{

}

void NoticeActOpenLayer::onExit()
{
	Layer::onExit();
}

bool NoticeActOpenLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void NoticeActOpenLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void NoticeActOpenLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_iCurIndex < (int)m_mWarId.size())
	{
		updateNoticeLayer(m_mWarId[m_iCurIndex]);
	}
	else
	{
		if (m_mWarId[0] == 1) //酒馆
		{
			Chess::getInstance()->removeAllScreen();
			ChessApplication::getInstance()->removeAllFunctionLayer();
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer")==NULL)
			{
				ChessApplication::getInstance()->AddFunctionLayer(PubDrinkLayer::create());
			}
			
		}
		else
		{
			NetWarInfo* ptrNetWar = trData->m_mNetWarInfo[m_mWarId[0]];
			if(ptrNetWar->iType == BATTLE_SIXWAR)
			{
				Chess::getInstance()->removeAllScreen();
				ChessApplication::getInstance()->removeAllFunctionLayer();
				ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create());
			}
			else if (ptrNetWar->iType == BATTLE_HEROIC)
			{
				Chess::getInstance()->removeAllScreen();
				ChessApplication::getInstance()->removeAllFunctionLayer();
				ChessApplication::getInstance()->AddFunctionLayer(HeroicAmbitionLayer::create(ptrNetWar->iWarId));
			}
			else
			{
				this->removeFromParent();
			}
		}
	}
}

NoticeActOpenLayer* NoticeActOpenLayer::create(map<int, int> mWarId)
{
	NoticeActOpenLayer* pRet = new NoticeActOpenLayer();
	if(pRet&&pRet->init(mWarId))
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

NoticeActOpenLayer* NoticeActOpenLayer::create(int iWarId)
{
	NoticeActOpenLayer* pRet = new NoticeActOpenLayer();
	if(pRet&&pRet->init(iWarId))
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

bool NoticeActOpenLayer::init(map<int, int> mWarId)
{
	if(!Layer::init())
	{
		return false;
	}

	m_mWarId = mWarId;
	m_iCurIndex = 0;

	initControl();
	updateNoticeLayer(m_mWarId[m_iCurIndex]);

	return true;
}

bool NoticeActOpenLayer::init(int iWarId)
{
	if(!Layer::init())
	{
		return false;
	}

	m_mWarId[0] = iWarId;
	m_iCurIndex = 0;

	initControl();
	updateNoticeLayer(m_mWarId[m_iCurIndex]);

	return true;
}

void NoticeActOpenLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	m_spLayBG = ReadSpriteName("new_ui/img_global/global_hot_bg1.png");
	m_spLayBG->setPosition(point);
	this->addChild(m_spLayBG);

	//ICON动画
	Armature* armIcon = Armature::create("icon_eff");
	armIcon->getAnimation()->play("war_tx1");
	armIcon->setPosition(Vec2(55, 46));
	m_spLayBG->addChild(armIcon);

	//ICON
	m_spIcon = ReadSpriteName("new_ui/battle/icon_hot100.png");
	m_spIcon->setPosition(Vec2(55, 46));
	m_spLayBG->addChild(m_spIcon);

	//Notice1
	m_labNotice1 = LabelSystemFont("", 18);
	m_labNotice1->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labNotice1->setPosition(Vec2(110, 54));
	m_labNotice1->setColor(Color3B(102, 0, 0));
	m_spLayBG->addChild(m_labNotice1);

	//Notice2
	m_labNotice2 = LabelSystemFont("", 16);
	m_labNotice2->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labNotice2->setPosition(Vec2(110, 27));
	m_labNotice2->setColor(Color3B(255, 102, 0));
	m_spLayBG->addChild(m_labNotice2);
}

void NoticeActOpenLayer::updateNoticeLayer(int iWarId)
{
	NoticeActOpen* ptrNotice = trData->m_mNoticeOpen[iWarId];
	__String* strPath = __String::createWithFormat("new_ui/battle/icon_hot%d.png", ptrNotice->iImgId);
	m_spIcon->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath->getCString())));

	m_labNotice1->setString(ptrNotice->strText1.c_str());
	m_labNotice2->setString(ptrNotice->strText2.c_str());

	m_spLayBG->setScale(0.1f);
	ScaleTo* actScale = ScaleTo::create(0.2f, 1.0f);
	m_spLayBG->runAction(actScale);
	m_iCurIndex++;
}
