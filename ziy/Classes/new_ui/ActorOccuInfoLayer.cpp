#include "ActorOccuInfoLayer.h"
#include "Defines.h"
#include "main.h"

ActorOccuInfoLayer* ActorOccuInfoLayer::create(HistoryActorPtr Actor)
{
	ActorOccuInfoLayer* pRet = new ActorOccuInfoLayer();
	if (pRet&&pRet->init(Actor))
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

bool ActorOccuInfoLayer::init(HistoryActorPtr Actor)
{
	if (!Layer::init())
	{
		return false;
	}
	curActor = Actor;
	OccupationAttrPtr occu = &trData->occupations[curActor->occupation_id];
	this->setAnchorPoint(Vec2(0, 0));
	this->setPosition(Vec2(0,0));
	auto offsetY = -8;
	Label* occu_name = LabelSystemFont(occu->occupation_name, 16);
	occu_name->setPosition(Vec2(74, 232+ offsetY));
	this->addChild(occu_name);

	//S形象
	Sprite* arms_bg = ReadSpriteName("new_ui/hero/arms_bg.png");
	arms_bg->setPosition(Vec2(24, 172 + offsetY));
	arms_bg->setAnchorPoint(Vec2(0, 0.5));
	this->addChild(arms_bg);

	fashion_width = 0;
	arms_width = 0;
	__String* f_path = NULL;
	__String* img_path = NULL;
	if (Actor->mFashion[0])
	{
		f_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", Actor->mFashion[3], Actor->mFashion[4]);
		img_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", Actor->mFashion[1], Actor->mFashion[2]);
		arms_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
		fashion_width = arms_fashion->getContentSize().width;
		arms_fashion->setTextureRect(Rect(0, 0, fashion_width, fashion_width));
		arms_fashion->setPosition(Vec2(73, 197));
		int iZorder = trData->net_hero_dress[Actor->mFashion[4]]->mMov[0];
		if (iZorder) iZorder++;
		arms_fashion->setLocalZOrder(iZorder);
		this->addChild(arms_fashion);
	}
	else
	{
		img_path = __String::create(Actor->getActorSImgPath("mov"));
		arms_fashion = NULL;
	}
	arms = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
	arms_width = arms->getContentSize().width;
	arms->setTextureRect(Rect(0, 0, arms_width, arms_width));
	arms->setPosition(Vec2(73, 197));
	this->addChild(arms,1);
	arms_rect = 0;
	click_arms = 0;

	//攻击范围
	Label* g_f = LabelSystemFont(LL("gongjifanwei"),16);
	g_f->setPosition(Vec2(188,232 + offsetY));
	this->addChild(g_f);

	Sprite* hitArea = ITools::getInstance()->getSpriteWithPath(__String::createWithFormat("img/HitArea/%d.png", occu->attack_type_id+1)->getCString());
	hitArea->setPosition(Vec2(188,180 + offsetY));
	this->addChild(hitArea);

	Label* occuInfo = LabelSystemFont(occu->occupation_info, 16);
	occuInfo->setDimensions(232, 90);
	occuInfo->setAnchorPoint(Vec2(0, 1));
	occuInfo->setPosition(Vec2(17,100));
	occuInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(occuInfo);

	this->schedule(schedule_selector(ActorOccuInfoLayer::updateArmsImg),0.2f);

	addSeidElem();

	return true;
}

void ActorOccuInfoLayer::addSeidElem()
{
	for (int i = 0; i < trData->occupations[curActor->occupation_id].seid_num; i++)
	{
		if (i>=2)  break;
		Seid* seid = trData->m_mSeid[trData->occupations[curActor->occupation_id].seid[i]];
		if (seid->seid == 0)
			continue;
		Sprite* seidBg = ReadSpriteName("new_ui/stunt_bg1.png");
		seidBg->setPosition(Vec2(72+(i%2)*117,123));
		this->addChild(seidBg);

		Label* seidName = LabelSystemFont(seid->sename, 16);
		seidName->setPosition(Vec2(53,17));
		seidBg->addChild(seidName);

		Sprite* seidInfo = ReadSpriteName("new_ui/info_bg1.png");
		seidInfo->setAnchorPoint(Vec2(0, 0.5));
		seidInfo->setPosition(Vec2(125 + (i % 2) * 117, 129));
		seidInfo->setVisible(false);

		Label* info = LabelSystemFont(seid->Info,16);
		info->setColor(Color3B(0,255,255));
		info->setAnchorPoint(Vec2(0, 1));
		info->setPosition(Vec2(10, 62));
		info->setDimensions(168,54);
		info->setHorizontalAlignment(TextHAlignment::LEFT);
		seidInfo->addChild(info);


		OccuSeid[i] = new OccuSeidElem();
		OccuSeid[i]->setBG(seidBg);
		OccuSeid[i]->setSNAME(seidName);
		OccuSeid[i]->setINFO(seidInfo);
	}
	for (size_t i = 0; i < OccuSeid.size(); i++)
	{
		this->addChild(OccuSeid[i]->getINFO());
	}
}

void ActorOccuInfoLayer::updateArmsImg(float fNum)
{
	if (curActor->mFashion[0])
		arms_fashion->setTextureRect(Rect(0, fashion_width * arms_rect, fashion_width, fashion_width));
	arms->setTextureRect(Rect(0, arms_width * arms_rect, arms_width, arms_width));
	if (arms_rect == 0)
		arms_rect = 1;
	else
		arms_rect = 0;
}

void ActorOccuInfoLayer::ArmsAttackAnimation(float fNum)
{
	if (click_arms == 0)
	{
		this->unschedule(schedule_selector(ActorOccuInfoLayer::updateArmsImg));

		__String* f_path = NULL;
		__String* img_path = NULL;
		if (curActor->mFashion[0])
		{
			f_path = __String::createWithFormat("img/Nunits/%d_%d_atk.png", curActor->mFashion[3], curActor->mFashion[4]);
			img_path = __String::createWithFormat("img/Nunits/%d_%d_atk.png", curActor->mFashion[1], curActor->mFashion[2]);
			Texture2D* text = ITools::getInstance()->getTextureWithPath(f_path->getCString());
			fashion_width = text->getContentSize().width;
			arms_fashion->setTexture(text);
			int iZorder = trData->net_hero_dress[curActor->mFashion[4]]->mAtk[0];
			if (iZorder) iZorder++;
			arms_fashion->setLocalZOrder(iZorder);
			arms_fashion->setTextureRect(Rect(0, 0, fashion_width, fashion_width));
		}
		else
		{
			img_path = __String::create(curActor->getActorSImgPath("atk"));
		}

		Texture2D* text1 = ITools::getInstance()->getTextureWithPath(img_path->getCString());
		arms_width = text1->getContentSize().width;
		arms->setTexture(text1);
		arms->setTextureRect(Rect(0, 0, arms_width, arms_width));
		this->schedule(schedule_selector(ActorOccuInfoLayer::ArmsAttackAnimation), 0.05f);
	}
	else if (click_arms > 0 && click_arms < 4)
	{
		if (curActor->mFashion[0])
		{
			int iZorder = trData->net_hero_dress[curActor->mFashion[4]]->mAtk[click_arms];
			if (iZorder) iZorder++;
			arms_fashion->setTextureRect(Rect(0, click_arms * fashion_width, fashion_width, fashion_width));
			arms_fashion->setLocalZOrder(iZorder);
		}

		arms->setTextureRect(Rect(0, click_arms*arms_width, arms_width, arms_width));
	}
	else if (click_arms == 4)
	{
		this->unschedule(schedule_selector(ActorOccuInfoLayer::ArmsAttackAnimation));
		__String* f_path = NULL;
		__String* img_path = NULL;
		if (curActor->mFashion[0])
		{
			f_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", curActor->mFashion[3], curActor->mFashion[4]);
			img_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", curActor->mFashion[1], curActor->mFashion[2]);
			Texture2D* text = ITools::getInstance()->getTextureWithPath(f_path->getCString());
			fashion_width = text->getContentSize().width;
			arms_fashion->setTexture(text);
			arms_fashion->setTextureRect(Rect(0, 0, fashion_width, fashion_width));
			int iZorder = trData->net_hero_dress[curActor->mFashion[4]]->mMov[0];
			if (iZorder) iZorder++;
			arms_fashion->setLocalZOrder(iZorder);
		}
		else
		{
			img_path = __String::create(curActor->getActorSImgPath("mov"));
		}
		Texture2D* text1 = ITools::getInstance()->getTextureWithPath(img_path->getCString());
		arms_width = text1->getContentSize().width;
		arms->setTexture(text1);
		arms->setTextureRect(Rect(0, 0, arms_width, arms_width));
		click_arms = -1;
		arms_rect = 1;
	}
	if (click_arms==3)
		this->schedule(schedule_selector(ActorOccuInfoLayer::updateArmsImg), 0.2f);
	click_arms++;
}

bool ActorOccuInfoLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (!this->isVisible())
		return false;
	selectOccuSeidNum = -1;
	for (size_t i = 0; i < OccuSeid.size(); i++)
	{
		Rect TextS = OccuSeid[i]->getBG()->getBoundingBox();
		if (Rect(0, 0, TextS.size.width, TextS.size.height).containsPoint(OccuSeid[i]->getBG()->convertToNodeSpace(touch->getLocation())))
		{
			OccuSeid[i]->getBG()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/stunt_bg2.png")));
			OccuSeid[i]->getSNAME()->setColor(Color3B::YELLOW);
			OccuSeid[i]->getINFO()->setVisible(true);
			selectOccuSeidNum = i;
			return true;
		}
	}
	Rect armsBox = arms->getBoundingBox();
	if (!click_arms&&Rect(0, 0, armsBox.size.width, armsBox.size.height).containsPoint(arms->convertToNodeSpace(touch->getLocation())))
	{
		playSound(trData->occu_types[curActor->GetActorOccuType()].atk_music);
		ArmsAttackAnimation(0);
	}
	return false;
}

void ActorOccuInfoLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void ActorOccuInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (selectOccuSeidNum >= 0)
	{
		OccuSeid[selectOccuSeidNum]->getBG()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/stunt_bg1.png")));
		OccuSeid[selectOccuSeidNum]->getSNAME()->setColor(Color3B(255, 255, 255));
		OccuSeid[selectOccuSeidNum]->getINFO()->setVisible(false);
		selectOccuSeidNum = 0;
	}
}

void ActorOccuInfoLayer::onExit()
{
	Layer::onExit();
}

ActorOccuInfoLayer::ActorOccuInfoLayer()
{

}