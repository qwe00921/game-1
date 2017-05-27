//
//  ActSceneLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/17.
//
//


#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "ActSceneLayer.h"
#include "DispatchLayer.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "PopLayer.h"
#include "GetNetDateTool.h"
#include "GoodsSprite.h"
#include "NetWarRewardShow.h"

ActSceneLayer::ActSceneLayer()
{

}

ActSceneLayer::~ActSceneLayer()
{

}

void ActSceneLayer::onExit()
{
	Layer::onExit();
}

bool ActSceneLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void ActSceneLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void ActSceneLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

ActSceneLayer* ActSceneLayer::create(int iWarId)
{
	ActSceneLayer* pRet = new ActSceneLayer();
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

bool ActSceneLayer::init(int iWarId)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iWarId = iWarId;

	initControl();
	return true;
}

void ActSceneLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	NetWarInfo* ptrScene = trData->m_mNetWarInfo[m_iWarId];
	m_iCurTimes = ptrScene->iMaxTimes;

	m_iCurTimes -= GetAttackNum(m_iWarId);

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spActBG = ReadSpriteName("new_ui/battle/fb_bg1.png");
	spActBG->setPosition(point);
	this->addChild(spActBG);

	//人物
	__String* strPath = __String::createWithFormat("img/heroz/icon_fb%d.png", ptrScene->iImgId);
	Sprite* spHero = ReadSpriteNormal(strPath->getCString());
	spHero->setPosition(Vec2(spHero->getContentSize().width / 2, spActBG->getContentSize().height - spHero->getContentSize().height / 2));
	spActBG->addChild(spHero);

	//活动名称
	__String* strName = __String::createWithFormat("%s %s", ptrScene->strName.c_str(), ptrScene->strLv.c_str());
	Label* labName = LabelSystemFont(strName->getCString(), 18);
	labName->setAnchorPoint(Vec2(0.0f, 0.5f));
	labName->setPosition(Vec2(225, 238));
	labName->setColor(Color3B(102, 0, 0));
	spActBG->addChild(labName);

	//活动信息
	Label* labInfo = LabelSystemFont(ptrScene->strInfo.c_str(), 16);
	labInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
	labInfo->setPosition(Vec2(225, 224));
	labInfo->setDimensions(312, 0);
	labInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	labInfo->setColor(Color3B::YELLOW);
	spActBG->addChild(labInfo);

	//次数标签
	Label* labTimes1 = LabelSystemFont(LL("actscenetimes"), 16);
	labTimes1->setAnchorPoint(Vec2(0.0f, 0.5f));
	labTimes1->setPosition(Vec2(225, 165));
	labTimes1->setColor(Color3B(102, 0, 0));
	spActBG->addChild(labTimes1);

	//剩余次数
	__String* strTimes = __String::createWithFormat("%d/%d", m_iCurTimes, ptrScene->iMaxTimes);
	m_labTimes = LabelSystemFont(strTimes->getCString(), 16);
	m_labTimes->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labTimes->setPosition(Vec2(225 + labTimes1->getContentSize().width + 8, 165));
	m_labTimes->setColor(Color3B::YELLOW);
	spActBG->addChild(m_labTimes);

	//出征标签
	Label* labSend1 = LabelSystemFont(LL("sendtime1"), 16);
	labSend1->setAnchorPoint(Vec2(0.0f, 0.5f));
	labSend1->setPosition(Vec2(225, 142));
	labSend1->setColor(Color3B(102, 0, 0));
	spActBG->addChild(labSend1);

	//出征时间
	__String* strSend = __String::createWithFormat(LL("sendtime2"), ptrScene->strStart.c_str(), ptrScene->strEnd.c_str());
	Label* labSend2 = LabelSystemFont(strSend->getCString(), 16);
	labSend2->setAnchorPoint(Vec2(0.0f, 0.5f));
	labSend2->setPosition(Vec2(225 + labSend1->getContentSize().width + 8, 142));
	labSend2->setColor(Color3B::YELLOW);
	spActBG->addChild(labSend2);

	//掉落
	Label* labReward = LabelSystemFont(LL("actreward"), 16);
	labReward->setAnchorPoint(Vec2(0.0f, 0.5f));
	labReward->setPosition(Vec2(225, 119));
	labReward->setColor(Color3B(102, 0, 0));
	spActBG->addChild(labReward);

	//掉落物品组
	map<int, int> mRewardGoods = trData->net_war_goods[m_iWarId];
	int iIndex = 0;
	for(map<int, int>::iterator it = mRewardGoods.begin(); it != mRewardGoods.end(); it++)
	{
		int iGoodsId = it->first;
		GoodsSprite* spItem = GoodsSprite::create(iGoodsId);
		spItem->setPosition(Vec2(225 + spItem->getContentSize().width / 2 + 56 * iIndex++, 102 - spItem->getContentSize().height / 2));
		spActBG->addChild(spItem);
	}

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(ActSceneLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(285, 105));
	ptrMenu->addChild(btnClose);

	//难度选择
	Sprite* spSendN = ReadSpriteName("new_ui/battle/battle_jdsz.png");
	Sprite* spSendS = ReadSpriteName("new_ui/battle/battle_jdsz.png");
	spSendS->setScale(fScale);
	MenuItemSprite* btnSend = MenuItemSprite::create(spSendN, spSendS, CC_CALLBACK_1(ActSceneLayer::callBackSend, this));
	spSendS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSendS->setPosition(Vec2(spSendS->getContentSize().width / 2, spSendS->getContentSize().height / 2));
	btnSend->setPosition(point + Vec2(280, -80));
	ptrMenu->addChild(btnSend);
}

void ActSceneLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void ActSceneLayer::callBackSend(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	int iCurTimes = trData->m_mNetWarInfo[m_iWarId]->iMaxTimes;

	iCurTimes -= GetAttackNum(m_iWarId);

	if(iCurTimes)
	{
		history->m_iBattleType = BATTLE_ACTSCENE;
		history->m_iCurWarId = m_iWarId;
		history->m_iBattleStage = 0;
		if(!trData->m_mNetWarInfo[m_iWarId]->iStage)
			ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
		else
			ChessApplication::getInstance()->AddFunctionLayer(NetWarRewardShow::create(m_iWarId,this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("dailytimeout"), false);
		//ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_BUY_WARCOUNT, this));
	}
}

void ActSceneLayer::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if(trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
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

void ActSceneLayer::SetPopLayerElement(Sprite* ptrBg)
{
	Label* titleTtf1 = LabelSystemFont(LL("czgqjrcsxhf"), 18);
	titleTtf1->setAnchorPoint(Vec2(0, 0.5));
	titleTtf1->setPosition(Vec2(66, 135));
	ptrBg->addChild(titleTtf1);

	int iWarRefNum = history->mMapDateInfo->getHeroicMapDateElem(m_iWarId)->getRefNum();
	int iSpeedJade = 0;
	if(trData->net_heroz_price.count(iWarRefNum + 1) > 0)
	{
		iSpeedJade = trData->net_heroz_price[iWarRefNum + 1];
	}
	else if(iWarRefNum + 1 > trData->net_heroz_price[0])
	{
		iSpeedJade = trData->net_heroz_price[trData->net_heroz_price[0]];
	}
	Label* titleTtf2 = LabelSystemFont(__String::createWithFormat("%d%s", iSpeedJade, LL("gouyu"))->getCString(), 18);
	titleTtf2->setColor(Color3B(0, 255, 255));
	titleTtf2->setAnchorPoint(Vec2(0, 0.5));
	titleTtf2->setPosition(Vec2(73 + titleTtf1->getContentSize().width, 135));
	ptrBg->addChild(titleTtf2);

	Label* titleTtf3 = LabelSystemFont(__String::createWithFormat(LL("shifoujixu"), iWarRefNum)->getCString(), 18);
	titleTtf3->setPosition(Vec2(ptrBg->getContentSize().width / 2, 129 - titleTtf1->getContentSize().height));
	ptrBg->addChild(titleTtf3);
}

int ActSceneLayer::GetAttackNum(int iWarId)
{
	int iRet = 0;
	if (trData->m_mNetWarInfo.count(iWarId) && trData->m_mNetWarInfo[iWarId]->iStage)
	{
		int iEWarId = 0;
		MapDateElem* ptrMap = NULL;
		for (int i = 0; i < trData->m_mNetWarInfo[iWarId]->iStage; i++)
		{
			iEWarId = iWarId * 100 + i+1;
			ptrMap = history->mMapDateInfo->getHeroicMapDateElem(iEWarId);
			if (ptrMap)
			{
				iRet += ptrMap->getATTACKNUM();
			}
		}
	}
	else
	{
		MapDateElem* ptrMap = history->mMapDateInfo->getHeroicMapDateElem(iWarId);
		if (ptrMap)
			iRet = ptrMap->getATTACKNUM();
	}

	return iRet;
}

void ActSceneLayer::RunFunction(int iNomal)
{
	int iWarRefNum = history->mMapDateInfo->getHeroicMapDateElem(m_iWarId)->getRefNum();
	int iSpeedJade = 0;
	if(trData->net_heroz_price.count(iWarRefNum + 1) > 0)
	{
		iSpeedJade = trData->net_heroz_price[iWarRefNum + 1];
	}
	else if(iWarRefNum + 1 > trData->net_heroz_price[0])
	{
		iSpeedJade = trData->net_heroz_price[trData->net_heroz_price[0]];
	}

	if(iSpeedJade <= history->m_iJade)
	{
		GetNetDateTool::getInstance()->SendRefWarCount(m_iWarId, this);
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
	}
}

void ActSceneLayer::SendNetDateResult(bool bIsSuccess, Json::Value jsonResult, int iType)
{
	if(bIsSuccess)
	{
		if(jsonResult["jade"].isInt())
		{
			history->m_iJade = jsonResult["jade"].asInt();
		}

		int iMaxWarCount = trData->m_mNetWarInfo[m_iWarId]->iMaxTimes;
		MapDateElem* ptrMDE = history->mMapDateInfo->getHeroicMapDateElem(m_iWarId);
		ptrMDE->setATTACKNUM(0);
		ptrMDE->setRefNum(ptrMDE->getRefNum() + 1);
		history->m_pPlayerInfo->m_mMapAttackNum[m_iWarId][1]++;


		m_iCurTimes = iMaxWarCount;
		__String* strTimes = __String::createWithFormat("%d/%d", m_iCurTimes, iMaxWarCount);
		m_labTimes->setString(strTimes->getCString());

		//刷新标题栏金币勾玉数据
		if(history->m_layTitleBar)
		{
			history->m_layTitleBar->updateShow(0);
		}
	}
}
