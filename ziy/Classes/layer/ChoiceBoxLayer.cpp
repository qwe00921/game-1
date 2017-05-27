//
//  ChoiceBoxLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/18.
//
//


#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "ChoiceBoxLayer.h"
#include "BattleScreen.h"
#include "RedeScreen.h"

ChoiceBoxLayer::ChoiceBoxLayer()
{

}

ChoiceBoxLayer::~ChoiceBoxLayer()
{

}

bool ChoiceBoxLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	for(UInt i = 0; i < m_mspBG.size(); i++)
	{
		Rect rcBG = m_mspBG[i]->getBoundingBox();
		if (rcBG.containsPoint(m_spLayBG->convertToNodeSpace(touch->getLocation())) && 
			((history->m_m12ScriptCon.count(i + 1) && history->m_m12ScriptCon[i + 1]) || !history->m_m12ScriptCon.count(i + 1)))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			m_mspBG[i]->setVisible(true);
			m_mLabel[i]->setColor(Color3B::WHITE);
			m_iClickIndex = i;
		}
	}

	return true;
}

void ChoiceBoxLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if(m_iClickIndex >= 0)
	{
		Rect rcBG = m_mspBG[m_iClickIndex]->getBoundingBox();
		if(!rcBG.containsPoint(m_spLayBG->convertToNodeSpace(touch->getLocation())))
		{
			m_mspBG[m_iClickIndex]->setVisible(false);
			m_mLabel[m_iClickIndex]->setColor(Color3B(102, 51, 0));
			m_iClickIndex = -1;
		}
	}
}

void ChoiceBoxLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_iClickIndex >= 0)
	{
		for (UInt i = 0; i < m_mspBG.size(); i++)
		{
			Chess::getInstance()->choice->addElement(m_iClickIndex + 1);
		}
		history->m_bWaitCommand = true;
		BattleScreen* ptrBattle = (BattleScreen*)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		if(ptrBattle)
		{
			ptrBattle->m_pChoiceBox = NULL;
		}
		else
		{
			RedeScreen* ptrRede = (RedeScreen*)Chess::getInstance()->getScreenByID(SCREEN_REDE);
			if (ptrRede)
			{
				ptrRede->m_pChoiceBox = NULL;
			}
		}
		history->m_m12ScriptCon.clear();
		this->removeFromParent();
	}
}

ChoiceBoxLayer* ChoiceBoxLayer::create(int iActorId, string strContent)
{
	ChoiceBoxLayer* pRet = new ChoiceBoxLayer();
	if(pRet&&pRet->init(iActorId, strContent))
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

bool ChoiceBoxLayer::init(int iActorId, string strContent)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iActorId = iActorId;
	m_strContent = strContent;
	m_iClickIndex = -1;

	initCondition();
	initControl();
	return true;
}

void ChoiceBoxLayer::initCondition()
{
	m_mCondition = trData->getStringMap(m_strContent, '\n');
	for(UInt i = 0; i < m_mCondition.size(); i++)
	{
		string strCondition = m_mCondition[i];
		m_mCondition[i] = StringUtils::format("%d. %s", i + 1, strCondition.c_str());
	}
}

void ChoiceBoxLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//背景
	m_spLayBG = ReadSpriteName("new_ui/img_global/bg.png");
	m_spLayBG->setPosition(point);
	this->addChild(m_spLayBG);

	//头像
	HistoryActor* ptrActor = &history->actors[m_iActorId];
	string strPath;
    strPath = StringUtils::format("img/Face_s/%d.png", ptrActor->face_id + 1);
	if(ptrActor->actor_id == 0&&ptrActor->mFashion[0] > 0)
	{
        strPath = StringUtils::format("img/Face_s/%d_%d_%d.png", ptrActor->face_id + 1, ptrActor->mFashion[1], ptrActor->mFashion[2]);
    }
	Sprite* spFace = ReadSpriteNormal(strPath);
	spFace->setScale(0.56f);
	spFace->setPosition(Vec2(95, 82));
	m_spLayBG->addChild(spFace);

	//选择框
	Sprite* spChoiceBG = ReadSpriteName("new_ui/img_global/talk_bg.png");
	spChoiceBG->setPosition(Vec2(338, 80));
	m_spLayBG->addChild(spChoiceBG);

	//条件
	for(UInt i = 0; i < m_mCondition.size(); i++)
	{
		//背景
		m_mspBG[i] = ReadSpriteName("new_ui/img_global/choice_bg.png");
		m_mspBG[i]->setPosition(Vec2(338, 112 - i * 32));
		m_spLayBG->addChild(m_mspBG[i]);
		m_mspBG[i]->setVisible(false);

		//内容
		m_mLabel[i] = LabelSystemFont(m_mCondition[i].c_str(), 18);
		m_mLabel[i]->setAnchorPoint(Vec2(0.0f, 0.5f));
		m_mLabel[i]->setPosition(Vec2(200, 112 - i * 32));
		if (history->m_m12ScriptCon.count(i + 1) && !history->m_m12ScriptCon[i + 1])
			m_mLabel[i]->setColor(Color3B::GRAY);
		else
			m_mLabel[i]->setColor(Color3B(102, 51, 0));
		m_spLayBG->addChild(m_mLabel[i]);
	}
}