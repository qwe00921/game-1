//
//  BattleSprite.cpp
//  TRGame
//
//  Created by Jimmy on 16/07/14.
//
//

#include "BattleSprite.h"
#include "MapElem.h"
#include "BattleLadder.h"

#define TIMER_ACTION_EFFECT 20

bool ActorInfoSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_mStrRelation[0] = "";
	m_mStrRelation[1] = LL("youjun");
	m_mStrRelation[2] = LL("dijun");
	m_mColor3B[0] = Color3B(132, 243, 132);
	m_mColor3B[1] = Color3B(255, 102, 0);
	m_mColor3B[2] = Color3B(0, 211, 247);

	initControl();
	return true;
}

void ActorInfoSprite::initControl()
{
	//背景
	m_spBG = ReadSpriteName("new_ui/battle/war_infobg1.png");
	m_spBG->setPosition(Vec2(m_spBG->getContentSize().width / 2, m_spBG->getContentSize().height / 2));
	this->addChild(m_spBG);

	//武将名字
	m_labName = LabelSystemFont("", 16);
	m_labName->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labName->setPosition(Vec2(5, 99));
	m_spBG->addChild(m_labName);

	//职业等级
	m_labJobLv = LabelSystemFont("", 14);
	m_labJobLv->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labJobLv->setPosition(Vec2(192, 99));
	m_spBG->addChild(m_labJobLv);

	//HP图标
	Sprite* spHpIcon = ReadSpriteName("new_ui/battle/use_hp.png");
	spHpIcon->setPosition(Vec2(24, 71));
	m_spBG->addChild(spHpIcon);

	//HP背景
	Sprite* spHpBG = ReadSpriteName("new_ui/battle/use_box.png");
	spHpBG->setPosition(Vec2(112, 70));
	m_spBG->addChild(spHpBG);

	//HP值
	m_spHpBar = ReadSpriteName("new_ui/battle/use_box_hp.png");
	m_spHpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_spHpBar->setPosition(Vec2(112 - m_spHpBar->getContentSize().width / 2, 70));
	m_spBG->addChild(m_spHpBar);

	//HP数值
	m_labHp = LabelSystemFont("100/100", 16);
	m_labHp->setPosition(Vec2(112, 70));
	m_spBG->addChild(m_labHp);

	//MP图标
	Sprite* spMpIcon = ReadSpriteName("new_ui/battle/use_mp.png");
	spMpIcon->setPosition(Vec2(24, 46));
	m_spBG->addChild(spMpIcon);

	//MP背景
	Sprite* spMpBG = ReadSpriteName("new_ui/battle/use_box.png");
	spMpBG->setPosition(Vec2(112, 45));
	m_spBG->addChild(spMpBG);

	//MP值
	m_spMpBar = ReadSpriteName("new_ui/battle/use_box_mp.png");
	m_spMpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_spMpBar->setPosition(Vec2(112 - m_spHpBar->getContentSize().width / 2, 45));
	m_spBG->addChild(m_spMpBar);

	//MP数值
	m_labMp = LabelSystemFont("100/100", 16);
	m_labMp->setPosition(Vec2(112, 45));
	m_spBG->addChild(m_labMp);

	//阵营
	m_labRelation = LabelSystemFont("", 16);
	m_labRelation->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labRelation->setPosition(Vec2(5, 15));
	m_spBG->addChild(m_labRelation);

	//地形
	m_labLand = LabelSystemFont("", 16);
	m_labLand->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labLand->setPosition(Vec2(150, 15));
	m_labLand->setColor(Color3B(132, 243, 132));
	m_spBG->addChild(m_labLand);

	//增益
	m_labGain = LabelSystemFont("", 14);
	m_labGain->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labGain->setPosition(Vec2(192, 15));
	m_spBG->addChild(m_labGain);
}

void ActorInfoSprite::updateSprite(HistoryActor* ptrActor, LandformAttr* ptrLand)
{
	int iRelation = ptrActor->relation;

	//背景
	string strBG = StringUtils::format("new_ui/battle/war_infobg%d.png", iRelation + 1);
	m_spBG->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strBG.c_str())));

	//武将名字
	m_labName->setString(trData->actors[ptrActor->actor_id].getActorName());
	m_labName->setColor(m_mColor3B[iRelation]);

	//职业等级
	string strJobLv = StringUtils::format("%s %d%s", trData->occupations[ptrActor->occupation_id].occupation_name, ptrActor->GetActorLv(), LL("ji"));
	m_labJobLv->setString(strJobLv);

	//HP
	int iCurHp = ptrActor->GetCurHp();
	int iTotalHp = ptrActor->GetBaseHp() + ptrActor->GetGoodsAddHp();
	m_spHpBar->setScaleX(float(iCurHp) / iTotalHp);
	string strHp = StringUtils::format("%d/%d", iCurHp, iTotalHp);
	m_labHp->setString(strHp);

	//MP
	int iCurMp = ptrActor->GetCurMp();
	int iTotalMp = ptrActor->GetBaseMp() + ptrActor->GetGoodsAddMp();
	m_spMpBar->setScaleX(float(iCurMp) / iTotalMp);
	string strMp = StringUtils::format("%d/%d", iCurMp, iTotalMp);
	m_labMp->setString(strMp);

	string RelationS = m_mStrRelation[iRelation];
	if (history->m_iBattleType == BATTLE_LADDER)
		RelationS = BattleLadder::getInstance()->GetBattleActorName(ptrActor->actor_id + 1, 2);
	//阵营
	m_labRelation->setString(RelationS.c_str());
	m_labRelation->setColor(m_mColor3B[iRelation]);

	//地形
	m_labLand->setString(ptrLand->landform_name);

	//增益
	int iLandGain = ptrActor->getLandAbilityGain(ptrLand->landform_id);
	string strGain = StringUtils::format("%d%%", iLandGain);
	m_labGain->setString(strGain);
	updateBuff(ptrActor);
	this->setVisible(true);
}

void ActorInfoSprite::updateBuff(HistoryActor* ptrActor)
{
	UInt iIndex = 0;
	for(map<int, ActorBuff*>::iterator it = ptrActor->m_mActorBuff.begin(); it != ptrActor->m_mActorBuff.end(); it++)
	{
		if (it->second->iCurRound > 0)
		{
			BuffElem* ptrBuff = trData->m_mDataBuff[it->first];
			int iImgId = ptrBuff->iImgId;
			string strPath = StringUtils::format("new_ui/buff/%d.png", iImgId);
			if (m_mspBuff[iIndex])
			{
				m_mspBuff[iIndex]->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
				m_mspBuff[iIndex]->setVisible(true);
			}
			else
			{
				m_mspBuff[iIndex] = ReadSpriteName(strPath.c_str());
				Size szBuff = m_mspBuff[iIndex]->getContentSize();
				m_mspBuff[iIndex]->setPosition(Vec2(szBuff.width / 2 + szBuff.width * iIndex, szBuff.height / 2 + m_spBG->getContentSize().height));
				this->addChild(m_mspBuff[iIndex]);
			}
			iIndex++;
		}
	}

	for(; iIndex < m_mspBuff.size(); iIndex++)
	{
		m_mspBuff[iIndex]->setVisible(false);
	}
}

bool ActionGainSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_mColor3B[0] = Color3B(132, 243, 132);
	m_mColor3B[1] = Color3B(255, 102, 0);
	m_mColor3B[2] = Color3B(0, 211, 247);
	m_iFrame = 0;

	initControl();
	return true;
}

void ActionGainSprite::initControl()
{
	//背景
	m_spBG = ReadSpriteName("new_ui/battle/war_infobg4.png");
	m_spBG->setPosition(Vec2(m_spBG->getContentSize().width / 2, m_spBG->getContentSize().height / 2));
	this->addChild(m_spBG);

	//武将名字
	m_labName = LabelSystemFont("", 16);
	m_labName->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labName->setPosition(Vec2(5, 81));
	m_spBG->addChild(m_labName);

	//等级
	m_labLv = LabelSystemFont("", 16);
	m_labLv->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labLv->setPosition(Vec2(192, 81));
	m_spBG->addChild(m_labLv);

	//HP图标
	Sprite* spHpIcon = ReadSpriteName("new_ui/battle/use_hp.png");
	spHpIcon->setPosition(Vec2(24, 53));
	m_spBG->addChild(spHpIcon);

	//HP背景
	Sprite* spHpBG = ReadSpriteName("new_ui/battle/use_box.png");
	spHpBG->setPosition(Vec2(112, 52));
	m_spBG->addChild(spHpBG);

	//HP值
	m_spHpBar = ReadSpriteName("new_ui/battle/use_box_hp.png");
	m_spHpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_spHpBar->setPosition(Vec2(112 - m_spHpBar->getContentSize().width / 2, 52));
	m_spBG->addChild(m_spHpBar);

	//HP数值
	m_labHp = LabelSystemFont("100/100", 16);
	m_labHp->setPosition(Vec2(112, 52));
	m_spBG->addChild(m_labHp);

	//MP图标
	Sprite* spMpIcon = ReadSpriteName("new_ui/battle/use_mp.png");
	spMpIcon->setPosition(Vec2(24, 28));
	m_spBG->addChild(spMpIcon);

	//MP背景
	Sprite* spMpBG = ReadSpriteName("new_ui/battle/use_box.png");
	spMpBG->setPosition(Vec2(112, 27));
	m_spBG->addChild(spMpBG);

	//MP值
	m_spMpBar = ReadSpriteName("new_ui/battle/use_box_mp.png");
	m_spMpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_spMpBar->setPosition(Vec2(112 - m_spHpBar->getContentSize().width / 2, 27));
	m_spBG->addChild(m_spMpBar);

	//MP数值
	m_labMp = LabelSystemFont("100/100", 16);
	m_labMp->setPosition(Vec2(112, 27));
	m_spBG->addChild(m_labMp);
}

void ActionGainSprite::updateSprite(HistoryActor* ptrActor, int iGainHp, int iGainMp)
{
	this->unschedule(schedule_selector(ActionGainSprite::updateSpriteBar));
	m_ptrActor = ptrActor;

	//重绘坐标
	int iPosX = 0;
	int iPosY = 0;
	if(ptrActor->xx() - history->scr_x > screen_size_x / 2)
	{
		iPosX = ptrActor->xx() - history->scr_x - m_spBG->getContentSize().width;
	}
	else
	{
		iPosX = ptrActor->xx() - history->scr_x + SCENE_TILE_WIDTH;
	}
	if(ptrActor->yy() - history->scr_y > screen_size_y / 2)
	{
		iPosY = ptrActor->yy() - history->scr_y;
	}
	else
	{
		iPosY = ptrActor->yy() - history->scr_y + SCENE_TILE_WIDTH + m_spBG->getContentSize().height;
	}
	this->setPosition(Vec2(iPosX, screen_size_y - iPosY));

	//武将名字
	m_labName->setString(trData->actors[ptrActor->actor_id].getActorName());
	m_labName->setColor(m_mColor3B[ptrActor->relation]);

	//等级
	string strLv = StringUtils::format("%d%s", ptrActor->GetActorLv(), LL("ji"));
	m_labLv->setString(strLv);

	//变化速率
	m_fHpRate = (float)iGainHp * 2 / TIMER_ACTION_EFFECT;
	m_fMpRate = (float)iGainMp * 2 / TIMER_ACTION_EFFECT;
	m_iFrame = 0;
	this->schedule(schedule_selector(ActionGainSprite::updateSpriteBar), 0);
	this->setVisible(true);
}

void ActionGainSprite::updateSpriteBar(float fInterval)
{
	if(m_iFrame > TIMER_ACTION_EFFECT)
	{
		this->unschedule(schedule_selector(ActionGainSprite::updateSpriteBar));
		this->setVisible(false);
	}
	else if(m_iFrame++ < TIMER_ACTION_EFFECT / 2)
	{
		//更新HP
		if (abs(m_fHpRate) > 0.01f || m_iFrame == 1)
		{
			int iCurHp = m_ptrActor->GetCurHp() + m_fHpRate * m_iFrame;
			int iTotalHp = m_ptrActor->GetBaseHp() + m_ptrActor->GetGoodsAddHp();
			m_spHpBar->setScaleX(float(iCurHp) / iTotalHp);
			string strHp = StringUtils::format("%d/%d", iCurHp, iTotalHp);
			m_labHp->setString(strHp);
		}

		//更新MP
		if(abs(m_fMpRate) > 0.01f || m_iFrame == 1)
		{
			int iCurMp = m_ptrActor->GetCurMp() + m_fMpRate * m_iFrame;
			int iTotalMp = m_ptrActor->GetBaseMp() + m_ptrActor->GetGoodsAddMp();
			m_spMpBar->setScaleX(float(iCurMp) / iTotalMp);
			string strMp = StringUtils::format("%d/%d", iCurMp, iTotalMp);
			m_labMp->setString(strMp);
		}
	}
}

bool LandInfoSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	initControl();
	return true;
}

void LandInfoSprite::updateSprite(int iPointX, int iPointY)
{
	this->unschedule(schedule_selector(LandInfoSprite::hideSprite));

	LandformAttr* ptrLand = &trData->lands[history->map_array[iPointY][iPointX]];
	for(int i = 0; i < history->elem_list->size(); i++) //添加战场提示如火堆，城墙等
	{
		MapElem* ptrElem = history->elem_list->elementAt(i);
		if(ptrElem->x == iPointX && ptrElem->y == iPointY)
		{
			ptrLand = &trData->lands[ptrElem->land_id];
			break;
		}
	}

	//重绘坐标
	int iPosX = 0;
	int iPosY = 0;
	if(iPointX * SCENE_TILE_WIDTH - history->scr_x > screen_size_x / 2)
	{
		iPosX = iPointX * SCENE_TILE_WIDTH - history->scr_x - m_spBG->getContentSize().width;
	}
	else
	{
		iPosX = iPointX * SCENE_TILE_WIDTH - history->scr_x + SCENE_TILE_WIDTH;
	}
	if(iPointY * SCENE_TILE_WIDTH - history->scr_y > screen_size_y / 2)
	{
		iPosY = iPointY * SCENE_TILE_WIDTH - history->scr_y + SCENE_TILE_WIDTH;
	}
	else
	{
		iPosY = iPointY * SCENE_TILE_WIDTH - history->scr_y + m_spBG->getContentSize().height;
	}
	this->setPosition(Vec2(iPosX, screen_size_y - iPosY));

	//地形
	m_labLand->setString(ptrLand->landform_name);

	//地形元素
	m_spLand->setTextureRect(Rect(iPointX * SCENE_TILE_WIDTH, iPointY * SCENE_TILE_WIDTH, SCENE_TILE_WIDTH, SCENE_TILE_WIDTH));

	//坐标
	string strPostion = StringUtils::format("[%d,%d]", iPointX, iPointY);
	m_labPostion->setString(strPostion);

	//效果
	string strEffect = "";
	if(ptrLand->landform_id >= 18 && ptrLand->landform_id <= 22)
	{
		strEffect = LL("keyihuifu");
	}
	else if(trData->occupation_landmove_info[0][ptrLand->landform_id] == 255)
	{
		strEffect = LL("bukeyidong");
	}
	m_labEffect->setString(strEffect);

	if (strEffect == "")
	{
		m_labPostion->setPosition(Vec2(102, 40));
	}
	else
	{
		m_labPostion->setPosition(Vec2(102, 50));
	}

	this->scheduleOnce(schedule_selector(LandInfoSprite::hideSprite), 3.0f);
	this->setVisible(true);
}

void LandInfoSprite::initControl()
{
	//背景
	m_spBG = ReadSpriteName("new_ui/battle/war_infobg5.png");
	m_spBG->setPosition(Vec2(m_spBG->getContentSize().width / 2, m_spBG->getContentSize().height / 2));
	this->addChild(m_spBG);

	//地形
	m_labLand = LabelSystemFont("", 16);
	m_labLand->setPosition(Vec2(75, 82));
	m_spBG->addChild(m_labLand);

	//地形元素
	string strPath;
    strPath = StringUtils::format("img/HM/%d.jpg", history->BG_index);
	m_spLand = ReadSpriteNormal(strPath);
	m_spLand->setTextureRect(Rect(0, 0, SCENE_TILE_WIDTH, SCENE_TILE_WIDTH));
	m_spLand->setPosition(Vec2(32, 39));
	m_spBG->addChild(m_spLand);

	//地形边框
	Sprite* spFrame = ReadSpriteName("new_ui/battle/battle_14.png");
	spFrame->setPosition(Vec2(32, 39));
	m_spBG->addChild(spFrame);

	//坐标
	m_labPostion = LabelSystemFont("[0,0]", 16);
	m_labPostion->setPosition(Vec2(102, 50));
	m_spBG->addChild(m_labPostion);

	//效果
	m_labEffect = LabelSystemFont("", 16);
	m_labEffect->setColor(Color3B(0, 211, 247));
	m_labEffect->setPosition(Vec2(102, 27));
	m_spBG->addChild(m_labEffect);
}

void LandInfoSprite::hideSprite(float fInterval)
{
	this->setVisible(false);
}

bool LabelSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_bIsHide = true;

	initControl();
	return true;
}

void LabelSprite::updateSprite(string strLabel, int iType, float iDelay /*= 0.0f*/)
{
	this->unschedule(schedule_selector(LabelSprite::hideSprite));

	for(int i = 0; i < LABEL_MAX; i++)
	{
		m_mspBG[i]->setVisible(i == iType);
		m_mNotice[i]->setVisible(i == iType);
	}

	//提示
	m_mNotice[iType]->setString(strLabel);

	if(iType == LABEL_SHORT)
	{
		float fWidth = m_mNotice[iType]->getContentSize().width;
		m_mspBG[iType]->setScaleX(fWidth / 2);
		m_spShortL->setPosition(Vec2(-fWidth / 2 - m_spShortL->getContentSize().width / 2, 0));
		m_spShortR->setPosition(Vec2(fWidth / 2 + m_spShortR->getContentSize().width / 2, 0));
		m_spShortL->setVisible(true);
		m_spShortR->setVisible(true);
	}
	else
	{
		m_spShortL->setVisible(false);
		m_spShortR->setVisible(false);
	}

	setClickHide(true);

	if ((BattleControler::auto_run || history->m_bAutoRun) && iDelay < 0.01f)
	{
		iDelay = 1.0f;
	}

	if (iDelay > 0.01f)
	{
		this->scheduleOnce(schedule_selector(LabelSprite::hideSprite), iDelay);
	}
	this->setVisible(true);
}

void LabelSprite::setClickHide(bool bIsHide)
{
	m_bIsHide = bIsHide;
}

bool LabelSprite::isClickHide()
{
	return m_bIsHide;
}

void LabelSprite::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	this->setPosition(point);

	//背景1
	m_mspBG[0] = ReadSpriteName("new_ui/battle/battle_tsbox2.png");
	this->addChild(m_mspBG[0]);

	//背景2
	m_mspBG[1] = ReadSpriteName("new_ui/battle/battle_tsbox1_2.png");
	this->addChild(m_mspBG[1]);

	//背景3
	m_mspBG[2] = ReadSpriteName("new_ui/battle/battle_sbg2_1.png");
	this->addChild(m_mspBG[2]);

	//Short左
	m_spShortL = ReadSpriteName("new_ui/battle/battle_tsbox1_1.png");
	this->addChild(m_spShortL);

	//Short右
	m_spShortR = ReadSpriteName("new_ui/battle/battle_tsbox1_3.png");
	this->addChild(m_spShortR);

	//提示1
	m_mNotice[0] = LabelSystemFont("", 16);
	this->addChild(m_mNotice[0]);

	//提示2
	m_mNotice[1] = LabelSystemFont("", 16);
	m_mNotice[1]->setColor(Color3B(102, 51, 0));
	this->addChild(m_mNotice[1]);

	//提示3
	m_mNotice[2] = LabelSystemFont("", 16);
	m_mNotice[2]->setColor(Color3B(102, 51, 0));
	m_mNotice[2]->setDimensions(320, 0);
	m_mNotice[2]->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(m_mNotice[2]);
}

void LabelSprite::hideSprite(float fInterval)
{
	this->setVisible(false);
}

GainNumSprite* GainNumSprite::create(int iGainNum, BattleActor* ptrActor)
{
	GainNumSprite* pRet = new GainNumSprite();
	if(pRet&&pRet->init(iGainNum, ptrActor))
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

bool GainNumSprite::init(int iGainNum, BattleActor* ptrActor)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_iGainNum = iGainNum;
	m_ptrActor = ptrActor;

	initControl();
	return true;
}

void GainNumSprite::initControl()
{
	if (m_ptrActor->m_bSkillMiss)
	{
		Sprite* spMiss = ReadSpriteName("new_ui/battle/wars_txt1.png");
		this->addChild(spMiss);
	}
	else
	{
		int iIndex1 = 0;
		if(m_ptrActor->m_bIsDeally)
		{
			iIndex1 = 4;
		}
		else
		{
			iIndex1 = m_iGainNum > 0 ? 6 : 5;
		}

		string strNum = StringUtils::format("%d", abs(m_iGainNum));
		for(UInt i = 0; i < strNum.size(); i++)
		{
			string strPath = StringUtils::format("new_ui/img_number/num%d_%c.png", iIndex1, strNum[i]);
			Sprite* spNum = ReadSpriteName(strPath.c_str());
			Size size = spNum->getContentSize();
			spNum->setPosition(Vec2(i * size.width - (strNum.size() - 1) * size.width / 2, 0));
			this->addChild(spNum);
		}
	}

	auto action = MoveBy::create(0.3f, Vec2(0, SCENE_TILE_WIDTH));
	Sequence* seque = Sequence::create(action, CallFunc::create([&]()
	{
		m_ptrActor->m_spGain = NULL;
		this->removeFromParent();
	}), NULL);
	this->runAction(seque);
}