#include "SkillAndGoodsInfoLayer.h"
#include "Defines.h"
#include "GoldExchangeLayer.h"
#include "editor-support/cocostudio/CCArmature.h"
#include "SuitInfoLayer.h"
#include "Net_HomePage.h"

using namespace cocostudio;

SkillInfoLayer* SkillInfoLayer::create(int iSkillId, int iSkillType, HistoryActorPtr ptrActor, EventDelegate* ptrDelegate)
{
	SkillInfoLayer* pRet = new SkillInfoLayer();
	if (pRet&&pRet->init(iSkillId, iSkillType,ptrActor,ptrDelegate))
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

bool SkillInfoLayer::init(int iSkillId, int iSkillType, HistoryActorPtr ptrActor, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mDelegate = ptrDelegate;
	curActor = ptrActor;
	upMenu = NULL;
	mSkillId = iSkillId;
	DSizeF size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black = LayerColor::create(Color4B(0,0,0,100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	bg = ReadSpriteName("new_ui/img_global/info_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);
	int newOffsetX = 30, newOffsetY = -10;
	int bg_height = bg->getContentSize().height;
	Sprite* equipment_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	equipment_bg->setPosition(Vec2(62 + newOffsetX, bg_height - equipment_bg->getContentSize().height / 2 - 20 + newOffsetY));
	bg->addChild(equipment_bg);

	int off_set = equipment_bg->getContentSize().width / 2;

	__String* skillIconCs = NULL;//icon路径
	string nameS = "";        //技能名
	int iHitAreaId = -1;     //施法范围
	int iHitEffectId = -1;	   //法术效果范围
	string skillTypeS = "";   //是否是主动技能
	string skillEffectS = ""; //法术效果
	__String* skillLevelCs = NULL; //技能等级
	__String* mpCs = NULL;          //mp消耗
	string targetS = "";      //目标
	string skillInfo = "";     //简介
	bool is_special = true;

	if (iSkillType == 1)
	{
		is_special = false;
		SkillAttrPtr ptrSkill = &trData->skills[iSkillId-1];
		skillIconCs = __String::createWithFormat("img/Skill/%d.png", ptrSkill->image_id+1);
		nameS = ptrSkill->skill_name;
		iHitAreaId = ptrSkill->use_region;
		iHitEffectId = ptrSkill->effect_region_type;
		skillTypeS = "new_ui/hero/skill_active.png";
		skillEffectS = ptrSkill->skill_effect;

		int skill_level = 1;
		if (!curActor->CurSkillLevelIsNull(iSkillId))
			skill_level = curActor->GetCurSkillLevel(iSkillId);
		skillLevelCs = __String::createWithFormat("%d%s", skill_level, LL("ji"));

		int iMpCon = ptrSkill->GetMpConsume(curActor,1);

		if (iMpCon < 9999)
			mpCs = __String::createWithFormat("%d%s", iMpCon, LL("dian"));
		else
			mpCs = __String::create(LL("dqsyfl"));

		if (ptrSkill->skill_object == 0)
			targetS = LL("dijun");
		else if (ptrSkill->skill_object == 1)
			targetS = LL("wojunhuoyoujun");
		else if (ptrSkill->skill_object == 2)
			targetS = LL("shiyongduixiang1");
		else if (ptrSkill->skill_object == 3)
			targetS = LL("shiyongduixiang1");
		else if (ptrSkill->skill_object == 5)
			targetS = LL("duiyou");
		else if (ptrSkill->skill_object == 6)
			targetS = LL("ziji");
		else if (ptrSkill->skill_object == 7)
			targetS = LL("kongdidijun");
		else if(ptrSkill->skill_object == 8)
			targetS = LL("fanweinei");
		skillInfo = ptrSkill->skill_info;
	}
	else 	if (iSkillType == 3)
	{
		Seid* ptrSkill = trData->m_mSeid[iSkillId];
		skillIconCs = __String::createWithFormat("img/Seid/%d.png", ptrSkill->imgid);
		nameS = ptrSkill->sename;
		skillTypeS = "new_ui/hero/skill_special.png";
		skillEffectS = LL("wu");
		mpCs = __String::create(LL("wu"));
		targetS = LL("wu");
		skillInfo = ptrSkill->Info;
		skillLevelCs = __String::createWithFormat("%d%s", 1, LL("ji"));
	}

	//图标
	Sprite* skillIcon = ITools::getInstance()->getSpriteWithPath(skillIconCs->getCString());
	skillIcon->setPosition(Vec2(off_set, off_set));
	equipment_bg->addChild(skillIcon);

	//施法类型
	Sprite* skillType = ReadSpriteName(skillTypeS.c_str());
	skillType->setPosition(Vec2(equipment_bg->getContentSize().width - skillType->getContentSize().width / 2, skillType->getContentSize().height / 2));
	equipment_bg->addChild(skillType);

	int iPointX = 68+equipment_bg->getContentSize().width/2 + newOffsetX;
	//名字
	Label* name = LabelSystemFont(nameS.c_str(), 16);
	name->setColor(Color3B::YELLOW);
	name->setAnchorPoint(Vec2(0, 1));
	name->setPosition(Vec2(iPointX, bg_height - 24));
	bg->addChild(name);

	if (!is_special)
	{
		int s_offset_y = bg_height - 27;
		//施法范围
		Label* hitAreaLab = LabelSystemFont(LL("gongjifanwei"), 16);
		hitAreaLab->setPosition(Vec2(245,350));
		hitAreaLab->setColor(Color3B(102, 0, 0));
		bg->addChild(hitAreaLab);

 		Sprite* hitAreaSp = ITools::getInstance()->getSpriteWithPath(__String::createWithFormat("img/HitArea/%d.png", iHitAreaId + 1)->getCString());
 		int iPointY = 348 - hitAreaLab->getContentSize().height / 2 - hitAreaSp->getContentSize().height / 2;
 		hitAreaSp->setPosition(Vec2(245+50 - newOffsetX + 10, iPointY));
 		bg->addChild(hitAreaSp);
 
 		//效果范围
		iPointY = 261;
 		Label* hitEffectLab = LabelSystemFont(LL("xiaoguofanwei"), 16);
		hitEffectLab->setColor(Color3B(102, 0, 0));
		hitEffectLab->setPosition(Vec2(245, iPointY));
 		bg->addChild(hitEffectLab);

 		Sprite* hitEffectSp = ITools::getInstance()->getSpriteWithPath(__String::createWithFormat("img/EffectArea/%d.png", iHitEffectId + 1)->getCString());
 		iPointY -= hitEffectLab->getContentSize().height / 2 + hitEffectSp->getContentSize().height / 2+2;
 		hitEffectSp->setPosition(Vec2(245+50 - newOffsetX + 10, iPointY));
 		bg->addChild(hitEffectSp);

		iPointX = 112 - equipment_bg->getContentSize().width / 2 - newOffsetX + 10;
		//等级
		Label* level = LabelSystemFont(__String::createWithFormat("%s:", LL("dengji"))->getCString(), 16);
		s_offset_y -= level->getContentSize().height*3 + 18;
		level->setColor(Color3B(102, 0, 0));
		level->setAnchorPoint(Vec2(0, 1));
		level->setPosition(Vec2(iPointX, s_offset_y));
		bg->addChild(level);

		level_num = LabelSystemFont(skillLevelCs->getCString(), 16);
		level_num->setAnchorPoint(Vec2(0, 1));
		level_num->setPosition(Vec2(iPointX + level->getContentSize().width, s_offset_y));
		bg->addChild(level_num);

		//效果
		Label* effect = LabelSystemFont(__String::createWithFormat("%s:", LL("xiaoguo"))->getCString(), 16);
		s_offset_y -= effect->getContentSize().height + 3;
		effect->setColor(Color3B(102, 0, 0));
		effect->setAnchorPoint(Vec2(0, 1));
		effect->setPosition(Vec2(iPointX, s_offset_y));
		bg->addChild(effect);

		Label* effect1 = LabelSystemFont(skillEffectS.c_str(), 16);
		effect1->setAnchorPoint(Vec2(0, 1));
		effect1->setPosition(Vec2(iPointX + effect->getContentSize().width, s_offset_y));
		bg->addChild(effect1);

		//法力消耗
		Label* mp = LabelSystemFont(__String::createWithFormat("%s:", LL("fali"))->getCString(), 16);
		s_offset_y -= mp->getContentSize().height + 3;
		mp->setColor(Color3B(102, 0, 0));
		mp->setAnchorPoint(Vec2(0, 1));
		mp->setPosition(Vec2(iPointX, s_offset_y));
		bg->addChild(mp);

		mptxt = LabelSystemFont(mpCs->getCString(), 16);
		mptxt->setAnchorPoint(Vec2(0, 1));
		mptxt->setPosition(Vec2(iPointX + mp->getContentSize().width, s_offset_y));
		bg->addChild(mptxt);

		//目标
		Label* target = LabelSystemFont(__String::createWithFormat("%s:", LL("mubiao"))->getCString(), 16);
		s_offset_y -= target->getContentSize().height + 3;
		target->setColor(Color3B(102, 0, 0));
		target->setAnchorPoint(Vec2(0, 1));
		target->setPosition(Vec2(iPointX, s_offset_y));
		bg->addChild(target);

		Label* targettxt = LabelSystemFont(__String::createWithFormat("%s", targetS.c_str())->getCString(), 16);
		targettxt->setAnchorPoint(Vec2(0, 1));
		targettxt->setPosition(Vec2(iPointX + target->getContentSize().width, s_offset_y));
		bg->addChild(targettxt);

		//激活
		Label* activation = LabelSystemFont(__String::createWithFormat("%s:", LL("jihuo"))->getCString(), 16);
		s_offset_y -= activation->getContentSize().height + 3;
		activation->setColor(Color3B(102, 0, 0));
		activation->setAnchorPoint(Vec2(0, 1));
		activation->setPosition(Vec2(iPointX, s_offset_y));
		bg->addChild(activation);

		int activation_level = trData->net_hero_skill[trData->actors[curActor->actor_id].skill]->skill[iSkillId];
		Label* activation1 = LabelSystemFont(__String::createWithFormat(LL("wjdjjh"), activation_level)->getCString(), 16);
		activation1->setAnchorPoint(Vec2(0, 1));
		activation1->setPosition(Vec2(iPointX + activation->getContentSize().width, s_offset_y));
		bg->addChild(activation1);

		map<int, float>HurtNum = GetHurtNum();
		//伤害
		s_offset_y -= 30;
		__String* hurtLabCs = __String::createWithFormat(trData->skills[mSkillId-1].explain.c_str(), HurtNum[0], HurtNum[1], HurtNum[2]);
		hurt_text = LabelSystemFont(hurtLabCs->getCString(), 16);
		float fSwidth = hurt_text->getContentSize().width;
		int iCellNum = ceil(fSwidth / 240);

		hurt_text->setAnchorPoint(Vec2(0, 1));
		hurt_text->setColor(Color3B::YELLOW);
		hurt_text->setPosition(Vec2(iPointX, s_offset_y));
		hurt_text->setDimensions(240, hurt_text->getContentSize().height*iCellNum);
		hurt_text->setHorizontalAlignment(TextHAlignment::LEFT);
		bg->addChild(hurt_text);

		//法术介绍
		Label* skillInfottf = LabelSystemFont(skillInfo.c_str(), 16);
		s_offset_y -= skillInfottf->getContentSize().height*iCellNum+2;
		skillInfottf->setAnchorPoint(Vec2(0, 1));
		skillInfottf->setDimensions(245, 53);
		skillInfottf->setPosition(Vec2(iPointX, s_offset_y));
		skillInfottf->setHorizontalAlignment(TextHAlignment::LEFT);
		bg->addChild(skillInfottf);

		if (history->game_state != GAME_STATE_BATTLE&&!curActor->CurSkillLevelIsNull(iSkillId))
		{
			//76
			Sprite* gold_bg = ReadSpriteName("new_ui/img_global/hero_bg4.png");
			gold_bg->setPosition(Vec2(bg->getContentSize().width / 2, 76));
			bg->addChild(gold_bg);

			Sprite* gold_img = ReadSpriteName("new_ui/img_global/icon_gb.png");
			gold_img->setAnchorPoint(Vec2(0, 0.5));
			gold_img->setPosition(Vec2(10,gold_bg->getContentSize().height/2));
			gold_bg->addChild(gold_img);

			int iGoldNum = trData->net_skill_gold[curActor->GetCurSkillLevel(iSkillId)];
			gold_num = LabelSystemFont(IntToString(iGoldNum), 16);
			gold_num->setColor(Color3B::YELLOW);
			gold_num->setPosition(Vec2(gold_bg->getContentSize().width / 2, gold_bg->getContentSize().height/2));
			gold_bg->addChild(gold_num);

			upMenu = Menu::create();
			upMenu->setPosition(Vec2(bg->getContentSize().width/2,36));
			bg->addChild(upMenu);
			//升10级
			Sprite* upM_N10 = ReadSpriteName("new_ui/img_global/global_ban1.png");
			Sprite* upM_NTxt10 = ReadSpriteName("new_ui/img_text/imgtxt5_31.png");
			upM_NTxt10->setPosition(Vec2(upM_N10->getContentSize().width / 2, upM_N10->getContentSize().height / 2));
			upM_N10->addChild(upM_NTxt10);

			Sprite* upM_S10 = ReadSpriteName("new_ui/img_global/global_ban2.png");
			Sprite* upM_STxt10 = ReadSpriteName("new_ui/img_text/imgtxt5_41.png");
			upM_STxt10->setPosition(Vec2(upM_S10->getContentSize().width / 2, upM_S10->getContentSize().height / 2));
			upM_S10->addChild(upM_STxt10);

			Sprite* upM_D10 = ReadSpriteName("new_ui/img_global/global_ban1.png");
			addSpriteGray(upM_D10);
			Sprite* upM_DTxt10 = ReadSpriteName("new_ui/img_text/imgtxt5_31.png");
			addSpriteGray(upM_DTxt10);
			upM_DTxt10->setPosition(Vec2(upM_D10->getContentSize().width / 2, upM_D10->getContentSize().height / 2));
			upM_D10->addChild(upM_DTxt10);

			auto upM_Sp10 = MenuItemSprite::create(upM_N10, upM_S10, upM_D10, CC_CALLBACK_1(SkillInfoLayer::ClickLevelUpMenu10LV, this));
			upM_Sp10->setPosition(upM_Sp10->getPosition()+Vec2(-60,0+8));
			upM_Sp10->setScale(0.9);
			upMenu->addChild(upM_Sp10);
			//
			Sprite* upM_N = ReadSpriteName("new_ui/img_global/global_ban1.png");
			Sprite* upM_NTxt = ReadSpriteName("new_ui/img_text/imgtxt5_3.png");
			upM_NTxt->setPosition(Vec2(upM_N->getContentSize().width / 2, upM_N->getContentSize().height/2));
			upM_N->addChild(upM_NTxt);

			Sprite* upM_S = ReadSpriteName("new_ui/img_global/global_ban2.png");
			Sprite* upM_STxt = ReadSpriteName("new_ui/img_text/imgtxt5_4.png");
			upM_STxt->setPosition(Vec2(upM_S->getContentSize().width / 2, upM_S->getContentSize().height / 2));
			upM_S->addChild(upM_STxt);

			Sprite* upM_D = ReadSpriteName("new_ui/img_global/global_ban1.png");
			addSpriteGray(upM_D);
			Sprite* upM_DTxt = ReadSpriteName("new_ui/img_text/imgtxt5_3.png");
			addSpriteGray(upM_DTxt);
			upM_DTxt->setPosition(Vec2(upM_D->getContentSize().width / 2, upM_D->getContentSize().height / 2));
			upM_D->addChild(upM_DTxt);

			upM_Sp = MenuItemSprite::create(upM_N, upM_S, upM_D, CC_CALLBACK_1(SkillInfoLayer::ClickLevelUpMenu, this));
			upM_Sp->setPosition(upM_Sp->getPosition() + Vec2(60, 0+8));
			upM_Sp->setScale(0.9);
			upMenu->addChild(upM_Sp);

			int iSkillLevel = curActor->GetCurSkillLevel(iSkillId);
			if (iSkillLevel > curActor->GetActorLv() - trData->net_skill_upgrade[iSkillId]->up_limit)
				upM_Sp->setEnabled(false);

			int iSkillLevel10 = curActor->GetCurSkillLevel(iSkillId);
			if (iSkillLevel10 > curActor->GetActorLv() - trData->net_skill_upgrade[iSkillId]->up_limit)
				upM_Sp10->setEnabled(false);
		}

	}
	else
	{
		int s_offset_x = 62 - equipment_bg->getContentSize().width / 2;
		int s_offset_y = bg_height - equipment_bg->getContentSize().height - 40;

		//法术介绍
		Label* skillInfottf = LabelSystemFont(skillInfo.c_str(), 16);
		skillInfottf->setAnchorPoint(Vec2(0, 1));
		skillInfottf->setDimensions(245, 0);
		skillInfottf->setPosition(Vec2(62 - equipment_bg->getContentSize().width / 2 + newOffsetX, s_offset_y));
		skillInfottf->setHorizontalAlignment(TextHAlignment::LEFT);
		bg->addChild(skillInfottf);
	}


// 	//提示
// 	CCSprite* prompt = ReadSpriteName("new_ui/prompt_2.png");
// 	prompt->setPosition(ccp(146,30));
// 	bg->addChild(prompt);

	return true;
}

void SkillInfoLayer::ClickLevelUpMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	int iGoldNum = trData->net_skill_gold[curActor->GetCurSkillLevel(mSkillId)];
	if (history->money >= iGoldNum)
	{
		upM_Sp->setEnabled(false);
		SendNetDate(1);

	}
	else
	{
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(mDelegate));
		this->removeFromParent();
	}
}

void SkillInfoLayer::ClickLevelUpMenu10LV(Ref * sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	int iGoldNum = trData->net_skill_gold[curActor->GetCurSkillLevel(mSkillId)];
	if (history->money >= iGoldNum*10)
	{
		upM_Sp->setEnabled(false);
		SendNetDate(10);

	}
	else
	{
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(mDelegate));
		this->removeFromParent();
	}
}

bool SkillInfoLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	bRemoveThis = true;
	if (history->game_state != GAME_STATE_BATTLE && !curActor->CurSkillLevelIsNull(mSkillId))
	{
		Rect bgBox = bg->getBoundingBox();
		if (Rect(0, 0, bgBox.size.width, bgBox.size.height).containsPoint(bg->convertToNodeSpace(touch->getLocation())))
		{
			bRemoveThis = false;
		}
	}
	return true;
}

void SkillInfoLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	
}

void SkillInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (bRemoveThis)
	{
		HongDian::getInstence()->autoUpdate();
		HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, JINENG_HONGDIAN);
		HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, ANNIU_HONGDIAN);
		this->removeFromParentAndCleanup(true);
		
		
	}

}

void SkillInfoLayer::onEnter()
{
	Layer::onEnter();
}

void SkillInfoLayer::onExit()
{
	Layer::onExit();
}

SkillInfoLayer::SkillInfoLayer()
{

}

void SkillInfoLayer::SendNetDate(int itime)
{
	CocosMD5 md5;
	int times= itime;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	int iSkillLv = trData->net_hero_skill[trData->actors[curActor->actor_id].skill]->skill[mSkillId];
	__String* validateCs = __String::createWithFormat("id=%d&slv=%d&times=%d&hid=%d&gameToken=%s", mSkillId, iSkillLv, times, curActor->actor_id+1,GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"id\":%d,\"slv\":%d,\"times\":%d,\"hid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", mSkillId, iSkillLv, times, curActor->actor_id + 1, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("upSkill",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(SkillInfoLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		NULL,
		true);

	ITools::getInstance()->addWaitLayer();
}

void SkillInfoLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	int iResCode = response->getResponseCode();
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if(iResCode == 200 || iResCode == 201)
		{
			if (value["result"].isObject())
			{
				ChessPlayNetSound(201);
				int times = value["result"]["times"].asInt();
				ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("sdjsjcg"),trData->skills[mSkillId-1].skill_name)->getCString(), true);
				if (value["result"]["gold"].isInt())
					history->money = value["result"]["gold"].asInt();

				if (mDelegate)
					mDelegate->updateMoneyShow();
				curActor->SetCurSkillLevel(mSkillId, -1, times);

				if (!curActor->SelfSkillLevelIsNull(mSkillId))
					curActor->SetSelfSkillLevel(mSkillId,-1, times);
				else
					curActor->SetSelfSkillLevel(mSkillId, 2);

				int skill_level = curActor->GetCurSkillLevel(mSkillId);
				level_num->setString(__String::createWithFormat("%d%s", skill_level, LL("ji"))->getCString());

				int consumeLess = trData->skills[mSkillId - 1].GetMpConsume(curActor);

				__String* mpCs = NULL;
				if (consumeLess < 9999)
					mpCs = __String::createWithFormat("%d%s", consumeLess, LL("dian"));
				else
					mpCs = __String::create(LL("dqsyfl"));

				mptxt->setString(mpCs->getCString());

				int iGoldNum = trData->net_skill_gold[skill_level];
				gold_num->setString(IntToString(iGoldNum));

				map<int, float>HurtNum = GetHurtNum();
				__String* hurtLabCs = __String::createWithFormat(trData->skills[mSkillId - 1].explain.c_str(), HurtNum[0], HurtNum[1], HurtNum[2]);
				hurt_text->setString(hurtLabCs->getCString());

				if (skill_level > curActor->GetActorLv() - trData->net_skill_upgrade[mSkillId]->up_limit)
					upM_Sp->setEnabled(false);
				else
					upM_Sp->setEnabled(true);
			}
			else
				upM_Sp->setEnabled(true);
			return;
		}
		else
		{
			history->parseErrorMsg(value, strTag);
		}
		upM_Sp->setEnabled(true);
	}
}

map<int, float> SkillInfoLayer::GetHurtNum()
{
	SkillAttrPtr ptrSkill = &trData->skills[mSkillId - 1];
	map<int, float> HurtNum;
	if (trData->net_skill_upgrade[mSkillId]->up_hurt_num)
		HurtNum[0] = ptrSkill->hurt_num + ptrSkill->GetExtraGainHp(curActor, 1);
	if (trData->net_skill_upgrade[mSkillId]->up_prop_num)
		HurtNum[1] = ptrSkill->prop_num + ptrSkill->GetExtraGainHp(curActor, 2);
	if (trData->net_skill_upgrade[mSkillId]->up_prop_per)
		HurtNum[2] = ptrSkill->prop_per + ptrSkill->GetExtraGainHp(curActor, 3);

	if (HurtNum[0])
	{
		HurtNum[3] = HurtNum[0];
		if (HurtNum[1])
		{
			HurtNum[4] = HurtNum[1];
			if (HurtNum[2])
				HurtNum[5] = HurtNum[2];
		}
		else if (HurtNum[2])
		{
			HurtNum[4] = HurtNum[2];
		}
	}
	else if (HurtNum[1])
	{
		HurtNum[3] = HurtNum[1];
		if (HurtNum[2])
			HurtNum[4] = HurtNum[2];
	}
	else if (HurtNum[2])
	{
		HurtNum[3] = HurtNum[2];
	}
	map<int, float> mRet;
	mRet[0] = HurtNum[3];
	mRet[1] = HurtNum[4];
	mRet[2] = HurtNum[5];
	return mRet;
}

////////////////

GoodsInfoLayer* GoodsInfoLayer::create(HistoryGoodsPtr ptrGoods, HistoryActorPtr ptrActor, int iFNType, EventDelegate* ptrED, int iGoldIndex)
{
	GoodsInfoLayer* pRet = new GoodsInfoLayer();
	pRet->mFNType = iFNType;
	pRet->m_EventDelegate = ptrED;
	pRet->mGoldIndex = iGoldIndex;
	if (pRet&&pRet->init(ptrGoods,ptrActor))
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

bool GoodsInfoLayer::init(HistoryGoodsPtr ptrGoods, HistoryActorPtr ptrActor)
{
	if (!Layer::init())
	{
		return false;
	}

	if (!ptrGoods->isNormalGood())
	{
		return false;
	}
	this->mEnableRemove = true;
	this->mGoods = ptrGoods;
	m_ptrActor = ptrActor;

	DSizeF size = ChessViewTool::getInstance()->getFullSize();

	black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	bg = ReadSpriteName("new_ui/img_global/box_bg1.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	int newOffsetX = 30, newOffsetY = -10;
	int bg_offset_y = bg->getContentSize().height - 20 + newOffsetY;
	Sprite* equipment_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	equipment_bg->setPosition(Vec2(62 + newOffsetX, bg_offset_y - equipment_bg->getContentSize().height/2));
	bg->addChild(equipment_bg);

	int off_set = equipment_bg->getContentSize().width / 2;

	__String* goodsIconCs = NULL;               //icon路径
	__String* goodsEffectS = NULL;							//效果
	string category = "";                                 //类别 
	__String* nameS = NULL;							//装备名
	int nameColor = 0;
	string goodsInfo = "";								//简介

	string atkOrdef = "";									//攻击或防御
	string ownNumS = "";								//自带攻击数值

	__String* goodsQualityCs = __String::create("q0");			//品质
	__String* goodsStarCs = NULL;				//星级
	string strNumS = "";									//强化攻击数值
	string str1S = LL("qhdsanxjhtssx");										//锻造附加特性1
	int str1Color = 0;
	string str2S = LL("qhdliuxjhtssx");									//锻造附加特性2
	int str2Color = 0;
	string str3S = LL("qhdshixjhtssx");									//锻造附加特性3
	int str3Color = 0;
	string str4S = "";										//锻造附加特性4
	int str4Color = 0;
	bool is_assist = true;

	if (!ptrGoods->isForgeEnable())
	{
		str1S = "---";
		str2S = "---";
		str3S = "---";
	}

	goodsIconCs = __String::createWithFormat("%s", history->GetGoodsImagePath(ptrGoods->good_id).c_str());
	goodsEffectS = __String::createWithFormat("%s (%s)", ptrGoods->getSpecialEffectName(), ptrGoods->getSpecialEffectInfo());
	category = ptrGoods->GetAttributeInfo();
	nameS = __String::create(ptrGoods->GetGoodsName());
	goodsInfo = ptrGoods->getInfo();

	if (ptrGoods->GetGoodsType() == GOOD_TYPE_WEAPON || ptrGoods->GetGoodsType() == GOOD_TYPE_COAT)
	{
		is_assist = false;
		int value = ptrGoods->GetGoodsPower();
		StrengthenConsumePtr strCon = NULL;

		atkOrdef = ptrGoods->getAttributeText();
		ownNumS = IntToString(value);

		if (ptrGoods->iQuality > 0)
		{
			strCon = &trData->strengthen_consume[ptrGoods->iQuality - 1];
			nameColor = strCon->quality;
			goodsQualityCs = __String::createWithFormat("q%d", strCon->quality);
			goodsStarCs = __String::createWithFormat("new_ui/img_number/star_b_%d.png", ptrGoods->iQuality);
			int strValue = ptrGoods->getStarAddVal();
			if (strValue)
				strNumS = IntToString_Sign(strValue);

			for (size_t i = 0; i < ptrGoods->GetStrengEffect().size(); i++)
			{
				int type = ptrGoods->GetStrengEffect()[i];
				for (int j = 0; j < trData->strengthen_property_num; j++)
				{
					if (type == trData->strengthen_property[j].id)
					{
						//找到了字典
						if (i == 0)
						{
							str1S = trData->strengthen_property[j].name;
							str1Color = trData->strengthen_property[j].quality;
							break;
						}
						else if (i == 1)
						{
							str2S = trData->strengthen_property[j].name;
							str2Color = trData->strengthen_property[j].quality;
							break;
						}
						else if (i == 2)
						{
							str3S = trData->strengthen_property[j].name;
							str3Color = trData->strengthen_property[j].quality;
							break;
						}
						else if (i == 3)
						{
							str4S = trData->strengthen_property[j].name;
							str4Color = trData->strengthen_property[j].quality;
							break;
						}
					}
				}
			}
		}
		
	}
	else
	{
		atkOrdef = "";
		ownNumS = "---";
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/special_eff.ExportJson", 0, 1));
	//图标
	Sprite* we_icon = ITools::getInstance()->getSpriteWithPath(goodsIconCs->getCString());
	we_icon->setPosition(Vec2(off_set, off_set));
	equipment_bg->addChild(we_icon);

    Armature* we_arm = Armature::create("special_eff");
	we_arm->setPosition(Vec2(off_set, off_set));
	we_arm->getAnimation()->play(goodsQualityCs->getCString());
	equipment_bg->addChild(we_arm);
	

	if (goodsStarCs)
	{
		Sprite* we_quality = ReadSpriteName(goodsStarCs->getCString());
		we_quality->setPosition(Vec2(off_set, 6));
		equipment_bg->addChild(we_quality);
	}

	//名字
	Label* ptrName = LabelSystemFont(nameS->getCString(), 18);
	ptrName->setAnchorPoint(Vec2(0.0f, 0.5f));
	ptrName->setPosition(Vec2(102 + newOffsetX, bg_offset_y - 11));
	ptrName->setColor(ITools::getInstance()->getPZColor(nameColor));
	bg->addChild(ptrName);

	//类型
	Label* Category_ttf = LabelSystemFont(StringUtils::format("%s:", LL("labtype")),16);
	Category_ttf->setAnchorPoint(Vec2(0.0f, 0.5f));
	Category_ttf->setPosition(Vec2(102 + newOffsetX, bg_offset_y - 34));
	Category_ttf->setColor(Color3B(102, 0, 0));
	bg->addChild(Category_ttf);

	Label* ptrCategory = LabelSystemFont(category.c_str(), 16);
	ptrCategory->setAnchorPoint(Vec2(0.0f, 0.5f));
	ptrCategory->setPosition(Vec2(102 + newOffsetX + Category_ttf->getContentSize().width, bg_offset_y - 34));
	bg->addChild(ptrCategory);

	if(ptrGoods->getStoneID())
	{
		//附魔标签
		Label* labEnchant = LabelSystemFont(StringUtils::format("%s:", LL("labenchant")), 16);
		labEnchant->setAnchorPoint(Vec2(0.0f, 0.5f));
		labEnchant->setPosition(Vec2(102 + newOffsetX, bg_offset_y - 56));
		labEnchant->setColor(Color3B(0x66, 0, 0));
		bg->addChild(labEnchant);

		//附魔文字
		HistoryGoods hisGoods(ptrGoods->getStoneID());
		Label* labEnchVal = LabelSystemFont(hisGoods.GetGoodsName(), 16);
		labEnchVal->setAnchorPoint(Vec2(0.0f, 0.5f));
		labEnchVal->setPosition(Vec2(102 + newOffsetX + labEnchant->getContentSize().width, bg_offset_y - 56));
		labEnchVal->setColor(Color3B::RED);
		bg->addChild(labEnchVal);
	}

	if (ptrGoods->GetGoodsType() != GOOD_TYPE_CONSUME)
	{
		bg_offset_y -= equipment_bg->getContentSize().height + 5;
		int tx_offset_y = bg_offset_y;
		//特性
		if (ptrGoods->isSpecial())
		{
			Label* ptrGoodsEffectS = LabelSystemFont(__String::createWithFormat("%s:", LL("texing"))->getCString(), 16);
			ptrGoodsEffectS->setAnchorPoint(Vec2(0, 1));
			ptrGoodsEffectS->setColor(Color3B(102, 0, 0));
			ptrGoodsEffectS->setPosition(Vec2(28 + newOffsetX, tx_offset_y));
			bg->addChild(ptrGoodsEffectS);

			Label* ptrGoodsEffectS1 = LabelSystemFont(goodsEffectS->getCString(), 16);
			float fSwidth = ptrGoodsEffectS1->getContentSize().width;
			int iCellNum = ceil(fSwidth / 224);
			if (iCellNum > 2) iCellNum = 2;
			ptrGoodsEffectS1->setAnchorPoint(Vec2(0, 1));
			ptrGoodsEffectS1->setColor(Color3B::YELLOW);
			ptrGoodsEffectS1->setPosition(Vec2(28 + newOffsetX + ptrGoodsEffectS->getContentSize().width, tx_offset_y));
			ptrGoodsEffectS1->setDimensions(224, ptrGoodsEffectS1->getContentSize().height*iCellNum);
			ptrGoodsEffectS1->setHorizontalAlignment(TextHAlignment::LEFT);
			bg->addChild(ptrGoodsEffectS1);
			tx_offset_y -= ptrGoodsEffectS->getContentSize().height*iCellNum +2;
		}

		//攻击数值
		if (!atkOrdef.empty())
		{
			Label* ptrAtkOrdef = LabelSystemFont(__String::createWithFormat("%s:", atkOrdef.c_str())->getCString(), 16);
			ptrAtkOrdef->setColor(Color3B(102, 0, 0));
			ptrAtkOrdef->setAnchorPoint(Vec2(0, 1));
			ptrAtkOrdef->setPosition(Vec2(28 + newOffsetX, tx_offset_y));
			bg->addChild(ptrAtkOrdef);

			Label* ptrAtkOrdef1 = LabelSystemFont(ownNumS.c_str(), 16);
			ptrAtkOrdef1->setAnchorPoint(Vec2(0, 1));
			ptrAtkOrdef1->setColor(Color3B::YELLOW);
			ptrAtkOrdef1->setPosition(Vec2(28 + newOffsetX + ptrAtkOrdef->getContentSize().width, tx_offset_y));
			bg->addChild(ptrAtkOrdef1);

			if (!strNumS.empty())
			{
				Label* ptrAtkOrdef2 = LabelSystemFont(strNumS.c_str(), 16);
				ptrAtkOrdef2->setAnchorPoint(Vec2(0, 1));
				ptrAtkOrdef2->setColor(Color3B::GREEN);
				ptrAtkOrdef2->setPosition(Vec2(31 + newOffsetX + ptrAtkOrdef->getContentSize().width + ptrAtkOrdef1->getContentSize().width, tx_offset_y));
				bg->addChild(ptrAtkOrdef2);
			}
			tx_offset_y -= ptrAtkOrdef->getContentSize().height + 2;
		}
		
		if (ptrGoods->isForgeEnable())
		{
			GoodsItem* ptrItem = trData->m_mGoodsItem[ptrGoods->good_id];
			//强化
			Label* ptrStr = LabelSystemFont(__String::createWithFormat("%s:", LL("qianghua"))->getCString(), 16);
			ptrStr->setAnchorPoint(Vec2(0, 1));
			ptrStr->setColor(Color3B(102, 0, 0));
			ptrStr->setPosition(Vec2(28 + newOffsetX, tx_offset_y));
			bg->addChild(ptrStr);

			Label* ptrStr1 = LabelSystemFont(str1S.c_str(), 16);
			ptrStr1->setAnchorPoint(Vec2(0, 1));
			ptrStr1->setColor(ITools::getInstance()->getPZColor(str1Color));
			ptrStr1->setPosition(Vec2(28 + newOffsetX + ptrStr->getContentSize().width, tx_offset_y));
			bg->addChild(ptrStr1);

			if (ptrItem->iTypeId >= 6)
			{
				tx_offset_y -= ptrStr1->getContentSize().height + 2;
				Label* ptrStr2 = LabelSystemFont(str2S.c_str(), 16);
				ptrStr2->setAnchorPoint(Vec2(0, 1));
				ptrStr2->setColor(ITools::getInstance()->getPZColor(str2Color));
				ptrStr2->setPosition(Vec2(28 + newOffsetX + ptrStr->getContentSize().width, tx_offset_y));
				bg->addChild(ptrStr2);
			}

			if (ptrItem->iTypeId >= 10)
			{
				tx_offset_y -= ptrStr1->getContentSize().height + 2;
				Label* ptrStr3 = LabelSystemFont(str3S.c_str(), 16);
				ptrStr3->setAnchorPoint(Vec2(0, 1));
				ptrStr3->setColor(ITools::getInstance()->getPZColor(str3Color));
				ptrStr3->setPosition(Vec2(28 + newOffsetX + ptrStr->getContentSize().width, tx_offset_y));
				bg->addChild(ptrStr3);
			}

			tx_offset_y -= ptrStr->getContentSize().height * 2;
		}
		else
		{
			tx_offset_y -= 16;
		}

		//套装
		if (ptrGoods->isSuit())
		{
			//按钮
			Menu* SuitM = Menu::create();
			SuitM->setPosition(Vec2(236,330));
			Sprite* Suit_ItemN = ReadSpriteName("new_ui/img_global/suit_bnt1.png");
			Sprite* Suit_ItemS = ReadSpriteName("new_ui/img_global/suit_bnt1.png");
			Suit_ItemS->setScale(0.9f);
			MenuItemSprite* Suit_Item = MenuItemSprite::create(Suit_ItemN, Suit_ItemS, CC_CALLBACK_1(GoodsInfoLayer::clickSuitMenu, this));
			Suit_ItemS->setAnchorPoint(Vec2(0.5,0.5));
			Suit_ItemS->setPosition(Vec2(Suit_ItemS->getContentSize().width / 2, Suit_ItemS->getContentSize().height/2));
			SuitM->addChild(Suit_Item);
			bg->addChild(SuitM);

			//效果
			tx_offset_y += 13;

			Label* ptrGoodsSuit = LabelSystemFont(__String::createWithFormat("%s:", LL("taozhuang"))->getCString(), 16);
			ptrGoodsSuit->setAnchorPoint(Vec2(0, 1));
			ptrGoodsSuit->setColor(Color3B(102, 0, 0));
			ptrGoodsSuit->setPosition(Vec2(28 + newOffsetX, tx_offset_y));
			bg->addChild(ptrGoodsSuit);

			map<int, int> mSuitId = ptrActor->GetNetSuitData();
			int iCellNum = 0;

			if (mSuitId.size())
			{
				int iOpenSuitId = 0;
				GoodsItem* ptrItem = trData->m_mGoodsItem[ptrGoods->good_id];
				for (map<int, int>::iterator mIt = ptrItem->mSuitList.begin(); mIt != ptrItem->mSuitList.end(); mIt++)
				{
					if (mSuitId.count(mIt->first))
					{
						iOpenSuitId = mIt->first;
						break;
					}
				}

				if (iOpenSuitId)
				{
					NetSuitDataPtr ptrNSD = trData->net_suit_data[iOpenSuitId];
					for (map<int, int>::iterator nIt = ptrNSD->seid1.begin(); nIt != ptrNSD->seid1.end(); nIt++)
					{
						if(trData->m_mSeid.count(nIt->first))
						{
							SeidPtr ptrSeid = trData->m_mSeid[nIt->first];
							Label* ptrGoodsSuitLab = LabelSystemFont(ptrSeid->Info, 16);
							float fSwidth = ptrGoodsSuitLab->getContentSize().width;
							iCellNum = ceil(fSwidth / 224);
							if (iCellNum > 2) iCellNum = 2;
							ptrGoodsSuitLab->setAnchorPoint(Vec2(0, 1));
							ptrGoodsSuitLab->setPosition(Vec2(28 + newOffsetX + ptrGoodsSuit->getContentSize().width, tx_offset_y));
							ptrGoodsSuitLab->setDimensions(224, ptrGoodsSuitLab->getContentSize().height*iCellNum);
							ptrGoodsSuitLab->setHorizontalAlignment(TextHAlignment::LEFT);
							bg->addChild(ptrGoodsSuitLab);
							tx_offset_y -= ptrGoodsSuit->getContentSize().height*iCellNum+2;
						}
					}

					bool bIsGray = true;
					if (ptrNSD->hid.count(ptrActor->actor_id+1))
					{
						bIsGray = false;
					}

					for (map<int, int>::iterator nIt = ptrNSD->seid2.begin(); nIt != ptrNSD->seid2.end(); nIt++)
					{
						if(trData->m_mSeid.count(nIt->first))
						{
							SeidPtr ptrSeid = trData->m_mSeid[nIt->first];
							Label* ptrGoodsSuitLab = LabelSystemFont(ptrSeid->Info, 16);
							float fSwidth = ptrGoodsSuitLab->getContentSize().width;
							iCellNum = ceil(fSwidth / 224);
							if (iCellNum > 2) iCellNum = 2;
							ptrGoodsSuitLab->setAnchorPoint(Vec2(0, 1));
							ptrGoodsSuitLab->setPosition(Vec2(28 + newOffsetX + ptrGoodsSuit->getContentSize().width, tx_offset_y));
							ptrGoodsSuitLab->setDimensions(224, ptrGoodsSuitLab->getContentSize().height*iCellNum);
							ptrGoodsSuitLab->setHorizontalAlignment(TextHAlignment::LEFT);
							if (bIsGray)
								ptrGoodsSuitLab->setColor(Color3B(153,153,153));
							bg->addChild(ptrGoodsSuitLab);
							tx_offset_y -= ptrGoodsSuit->getContentSize().height*iCellNum+2;
						}
					}
				}
			}
			else
			{
				Label* ptrWJHLab = LabelSystemFont(LL("weijihuo"), 16);
				ptrWJHLab->setAnchorPoint(Vec2(0, 1));
				ptrWJHLab->setPosition(Vec2(28 + newOffsetX + ptrGoodsSuit->getContentSize().width, tx_offset_y));
				bg->addChild(ptrWJHLab);
				tx_offset_y -= ptrGoodsSuit->getContentSize().height + 2;
			}

			tx_offset_y -= 16;
		}

		//简介
		Label* ptrInfo = LabelSystemFont(goodsInfo.c_str(), 16);
		ptrInfo->setAnchorPoint(Vec2(0, 1));
		ptrInfo->setPosition(Vec2(26 + newOffsetX, tx_offset_y));
		ptrInfo->setDimensions(246, 100);
		ptrInfo->setHorizontalAlignment(TextHAlignment::LEFT);
		bg->addChild(ptrInfo);
	}
	else
	{
		__String* yyy_num = __String::createWithFormat(LL("yiyongyoudjian"), ptrGoods->getStoreNum());
		Label* yyydj = LabelSystemFont(yyy_num->getCString(), 16);
		yyydj->setColor(Color3B(102, 0, 0));
		yyydj->setAnchorPoint(Vec2(0, 1));
		yyydj->setPosition(Vec2(99 + newOffsetX,bg_offset_y - 10 - ptrName->getContentSize().height));
		bg->addChild(yyydj);

		Label* ptrInfo = LabelSystemFont(goodsInfo.c_str(), 16);
		ptrInfo->setAnchorPoint(Vec2(0, 1));
		ptrInfo->setPosition(Vec2(26 + newOffsetX, bg_offset_y - equipment_bg->getContentSize().height -20));
		ptrInfo->setDimensions(246, 100);
		ptrInfo->setHorizontalAlignment(TextHAlignment::LEFT);
		bg->addChild(ptrInfo);
	}

	if (mFNType == EQ_TYPE_UPLOAD || mFNType == EQ_TYPE_EQUIP)
	{
		string FTM_ItemTNS = "new_ui/img_text/imgtxt3_9.png";
		string  FTM_ItemTSS = "new_ui/img_text/imgtxt3_10.png";
		if (mFNType == EQ_TYPE_UPLOAD)
		{
			FTM_ItemTNS = "new_ui/img_text/imgtxt3_7.png";
			FTM_ItemTSS = "new_ui/img_text/imgtxt3_8.png";
		}

		int iEnableEquipmentUp = 0;
		if (ptrGoods->isForgeEnable() && ptrGoods->iQuality != ptrGoods->getMaxStar() && !ptrGoods->isAssist())
			iEnableEquipmentUp = 1;

		FTM = Menu::create();
		FTM->setPosition(Vec2(bg->getContentSize().width / 2 - 65 * iEnableEquipmentUp, 40));
		Sprite* FTM_ItemTN = ReadSpriteName(FTM_ItemTNS.c_str());
		Sprite* FTM_ItemN = ReadSpriteName("new_ui/img_global/global_ban1.png");
		FTM_ItemTN->setPosition(Vec2(FTM_ItemN->getContentSize().width / 2, FTM_ItemN->getContentSize().height / 2));
		FTM_ItemN->addChild(FTM_ItemTN);

		Sprite* FTM_ItemTS = ReadSpriteName(FTM_ItemTSS.c_str());
		Sprite* FTM_ItemS = ReadSpriteName("new_ui/img_global/global_ban2.png");
		FTM_ItemTS->setPosition(Vec2(FTM_ItemS->getContentSize().width / 2, FTM_ItemS->getContentSize().height / 2));
		FTM_ItemS->addChild(FTM_ItemTS);
		MenuItemSprite* FTM_Item = MenuItemSprite::create(FTM_ItemN, FTM_ItemS, CC_CALLBACK_1(GoodsInfoLayer::clickButMenu, this));
		FTM->addChild(FTM_Item);
		bg->addChild(FTM);

		if (iEnableEquipmentUp)
		{
			Menu* StrengM = Menu::create();
			StrengM->setPosition(Vec2(bg->getContentSize().width / 2 + 65 + newOffsetX-20, 40));
			Sprite* Streng_ItemTN = ReadSpriteName("new_ui/img_text/imgtxt2_1.png");
			Sprite* Streng_ItemN = ReadSpriteName("new_ui/img_global/global_ban1.png");
			Streng_ItemTN->setPosition(Vec2(Streng_ItemN->getContentSize().width / 2, Streng_ItemN->getContentSize().height / 2));
			Streng_ItemN->addChild(Streng_ItemTN);

			Sprite* Streng_ItemTS = ReadSpriteName("new_ui/img_text/imgtxt2_2.png");
			Sprite* Streng_ItemS = ReadSpriteName("new_ui/img_global/global_ban2.png");
			Streng_ItemTS->setPosition(Vec2(Streng_ItemS->getContentSize().width / 2, Streng_ItemS->getContentSize().height / 2));
			Streng_ItemS->addChild(Streng_ItemTS);
			MenuItemSprite* Streng_Item = MenuItemSprite::create(Streng_ItemN, Streng_ItemS, CC_CALLBACK_1(GoodsInfoLayer::clickStrengMenu, this));
			StrengM->addChild(Streng_Item);
			bg->addChild(StrengM);
		}
	}
	else if (mFNType == EQ_TYPE_BUY)
	{
		string FTM_ItemTNS = "new_ui/img_global/global_ban_txt27.png";
		string  FTM_ItemTSS = "new_ui/img_global/global_ban_txt28.png";
		Sprite* gold_bg = ReadSpriteName("new_ui/shop/shop_bg3.png");
		gold_bg->setPosition(Vec2(bg->getContentSize().width / 2 + newOffsetX, 72));
		bg->addChild(gold_bg);

		Sprite* gold_icon = ReadSpriteName("new_ui/img_global/icon_yb.png");
		gold_icon->setPosition(Vec2(gold_icon->getContentSize().width / 2 + newOffsetX, gold_bg->getContentSize().height / 2));
		gold_bg->addChild(gold_icon);

		FTM = Menu::create();
		FTM->setPosition(Vec2(bg->getContentSize().width / 2 + newOffsetX, 40));
		Sprite* FTM_ItemTN = ReadSpriteName(FTM_ItemTNS.c_str());
		Sprite* FTM_ItemN = ReadSpriteName("new_ui/img_global/global_ban1.png");
		FTM_ItemTN->setPosition(Vec2(FTM_ItemN->getContentSize().width / 2, FTM_ItemN->getContentSize().height / 2));
		FTM_ItemN->addChild(FTM_ItemTN);

		Sprite* FTM_ItemTS = ReadSpriteName(FTM_ItemTSS.c_str());
		Sprite* FTM_ItemS = ReadSpriteName("new_ui/img_global/global_ban2.png");
		FTM_ItemTS->setPosition(Vec2(FTM_ItemS->getContentSize().width / 2, FTM_ItemS->getContentSize().height / 2));
		FTM_ItemS->addChild(FTM_ItemTS);

		MenuItemSprite* FTM_Item = MenuItemSprite::create(FTM_ItemN, FTM_ItemS, CC_CALLBACK_1(GoodsInfoLayer::clickButMenu, this));
		FTM->addChild(FTM_Item);
		bg->addChild(FTM);
	}
	else
		FTM = NULL;

// 	//提示
// 	CCSprite* prompt = ReadSpriteName("new_ui/prompt_2.png");
// 	prompt->setPosition(ccp(146, 30));
// 	bg->addChild(prompt);



	return true;
}


bool GoodsInfoLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	click_remove_layer = 0;

	if (mFNType > EQ_TYPE_NOMAL)
	{
		Rect bg_box = bg->getBoundingBox();
		if (!Rect(0, 0, bg_box.size.width, bg_box.size.height).containsPoint(bg->convertToNodeSpace(touch->getLocation())))
		{
			click_remove_layer = 1;
		}
	}
	else
		click_remove_layer = 1;
	return true;
}

void GoodsInfoLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void GoodsInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (click_remove_layer&&mEnableRemove)
	{
		if (mFNType == EQ_TYPE_BUY&&m_EventDelegate != NULL)
			m_EventDelegate->ChildCloseLayer(EQ_TYPE_BUY);
		this->removeFromParentAndCleanup(true);
	}
}

void GoodsInfoLayer::clickButMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mFNType == EQ_TYPE_EQUIP) //判断是否可以装备
	{
		if(!mGoods->isEquipEnable(m_ptrActor->actor_id))
		{
			ITools::getInstance()->createGlobalPrompt(LL("equipforbidden"), false);
			return;
		}
	}

	if (m_EventDelegate != NULL)
	{
		if (FTM != NULL)
			FTM->setEnabled(false);
		if (mFNType == EQ_TYPE_EQUIP || mFNType == EQ_TYPE_UPLOAD)
			m_EventDelegate->UpdateEquipment(mFNType);
		else if (mFNType == EQ_TYPE_BUY)
		{
			m_EventDelegate->RunFunction(mFNType);
			m_EventDelegate->ChildCloseLayer(mFNType);
		}
	}
	if (mFNType != EQ_TYPE_EQUIP&&mFNType != EQ_TYPE_UPLOAD)
		this->removeFromParent();
}

void GoodsInfoLayer::clickStrengMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_EventDelegate != NULL)
	{
		m_EventDelegate->RunFunction(1);
	}
}

void GoodsInfoLayer::clickSuitMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(SuitInfoLayer::create(mGoods->good_id));
}

void GoodsInfoLayer::setBlackLayerVisible(bool bVisible)
{
	black->setVisible(false);
}
void GoodsInfoLayer::setBgPoint(int iX, int iY, float iAx, float iAy)
{
	bg->setAnchorPoint(Vec2(iAx, iAy));
	bg->setPosition(Vec2(iX,iY));
}

void GoodsInfoLayer::setEnableRemove(bool bEnable)
{
	mEnableRemove = bEnable;
}

void GoodsInfoLayer::onExit()
{
	Layer::onExit();
}

void GoodsInfoLayer::onEnter()
{
	Layer::onEnter();
}

GoodsInfoLayer::GoodsInfoLayer()
{

}