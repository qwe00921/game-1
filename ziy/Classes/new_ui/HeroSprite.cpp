#include "HeroSprite.h"

HeroSprite* HeroSprite::create(int iHeroId, int iQuality /*= 0*/, float fScale /*= 0.8f*/)
{
	HeroSprite *pRet = new HeroSprite();
	if(pRet && pRet->init(iHeroId, iQuality, fScale))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool HeroSprite::init(int iHeroId, int iQuality, float fScale)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_iHeroId = iHeroId;
	m_iQuality = iQuality;
	m_fScale = fScale;
	m_bIsClick = false;
	m_bIsDrag = false;

	initControl();
	this->setContentSize(m_rcBG.size);

	return true;
}

bool HeroSprite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Vec2 point = convertToNodeSpace(touch->getLocation());
	if(m_rcBG.containsPoint(point) && isVisible() && getParent()->isVisible())
	{
		m_posTouch = touch->getLocation();
		m_bIsClick = true;
	}
	return true;
}

void HeroSprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsClick)
	{
		float fOffsetY = m_posTouch.y - touch->getLocation().y;
		if(!m_rcBG.containsPoint(convertToNodeSpace(touch->getLocation())) || m_bIsDrag && fabs(fOffsetY) > 5)
		{
			m_bIsClick = false;
		}
	}
}

void HeroSprite::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsClick)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ChessApplication::getInstance()->AddFunctionLayer(HeroInfoLayer::create(m_iHeroId));
		m_bIsClick = false;
	}
}

void HeroSprite::initControl()
{
	string strBG = m_fScale == 1.0f ? "new_ui/img_global/item_box_bg2.png" : "new_ui/img_global/item_box_bg.png";
	Sprite* spHeroBG = ReadSpriteName(strBG.c_str());
	Vec2 vecPos = Vec2(spHeroBG->getContentSize().width / 2, spHeroBG->getContentSize().height / 2);

	spHeroBG->setPosition(vecPos);
	m_rcBG = spHeroBG->getBoundingBox();
	this->addChild(spHeroBG);

	if (trData->net_hero_list.count(m_iHeroId))
	{
		//ICON
		Sprite* spIcon = history->getGoodsSprite(trData->net_hero_list[m_iHeroId]->good_id);
		spIcon->setScale(m_fScale);
		spIcon->setPosition(vecPos);
		spHeroBG->addChild(spIcon);

		//品质
		if (!m_iQuality && trData->actors[m_iHeroId - 1].actor_quality > 1)
		{
			m_iQuality = trData->actors[m_iHeroId - 1].actor_quality - 1;
		}

		if (m_iQuality)
		{
			string strQuality = m_fScale == 1.0f ? "new_ui/img_global/q%d_1.png" : "new_ui/img_global/q%d.png";
			Sprite* spQuality = ReadSpriteName(StringUtils::format(strQuality.c_str(), m_iQuality).c_str());
			spQuality->setPosition(vecPos);
			spHeroBG->addChild(spQuality);
		}
	}
}

void HeroSprite::setSupDrag(bool bIsDrag)
{
	m_bIsDrag = bIsDrag;
}

//////HeroInfoLayer

HeroInfoLayer* HeroInfoLayer::create(int iHeroId,int iLv)
{
	HeroInfoLayer* pRet = new HeroInfoLayer();
	if (pRet&&pRet->init(iHeroId,iLv))
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

bool HeroInfoLayer::init(int iHeroId,int iLv)
{
	if (!Layer::init())
	{
		return false;
	}
	this->m_iHeroid = iHeroId;
	this->m_iLv = iLv;

	DSize  size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2());
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/img_global/info_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	HistoryActorPtr ptrActor = &history->actors[iHeroId - 1];

	//武将背景
	Sprite* img_bg = ReadSpriteName("new_ui/hero/arms_bg.png");
	img_bg->setPosition(Vec2(73, 306));
	bg->addChild(img_bg);

	__String* f_path = NULL;
	__String* img_path = NULL;

	NetHeroNoDress* ptrNHND = NULL;
	Sprite* arms_fashion = NULL;
	if (trData->net_hero_no_dress.count(iHeroId))
	{
		ptrNHND = trData->net_hero_no_dress[iHeroId];
		f_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", 100, ptrNHND->fashion2);
		img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrNHND->gender, ptrNHND->fashion1);
		arms_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
		int fashion_width = arms_fashion->getContentSize().width;
		arms_fashion->setTextureRect(Rect(0, fashion_width * 4, fashion_width, fashion_width));
		int iZorder = trData->net_hero_dress[ptrNHND->fashion2]->mSpc[4];
		if (iZorder) iZorder++;
		arms_fashion->setPosition(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2 + 25);
		img_bg->addChild(arms_fashion, iZorder);
	}
	else
	{
		img_path = __String::create(ptrActor->getActorSImgPath("spc", 0));
	}

	//武将
	Sprite* actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
	int img_width = actor_img->getContentSize().width;
	//actor_img->setAnchorPoint(Vec2(0.5, 0));
	actor_img->setTextureRect(Rect(0, img_width * 4, img_width, img_width));
	actor_img->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2 + 25));
	img_bg->addChild(actor_img);

	//品质
	__String* a_qualityCs = __String::createWithFormat("new_ui/hero/job%d.png", trData->actors[iHeroId - 1].job_img);
	Sprite* a_quality = ReadSpriteName(a_qualityCs->getCString());
	a_quality->setPosition(Vec2(33, 293));
	a_quality->setScale(0.8f);
	bg->addChild(a_quality);

	//名字
	Label* a_name = LabelSystemFont(trData->actors[iHeroId - 1].getActorName(), 16);
	a_name->setAnchorPoint(Vec2(0, 1));
	a_name->setColor(Color3B(255, 255, 153));
	a_name->setPosition(Vec2(122, 350));
	bg->addChild(a_name);

	OccupationAttrPtr occu = &trData->occupations[trData->actors[iHeroId - 1].occupation_id];
	//兵种
	Label* a_occu = LabelSystemFont(occu->occupation_name, 16);
	a_occu->setColor(Color3B::YELLOW);
	a_occu->setAnchorPoint(Vec2(0, 1));
	a_occu->setPosition(Vec2(122, 330));
	bg->addChild(a_occu);

	int iMaxLv = trData->actors[iHeroId - 1].actor_quality*LEVELGROWTH;

	for (int i = 0; i < trData->actors[iHeroId - 1].seid_num; i++)
	{
		if (trData->actors[iHeroId - 1].seid[i] == 9999)
		{
			iMaxLv = 100;
			break;
		}
	}

	//等级
	Label* Lvlab = LabelSystemFont(__String::createWithFormat("%s %d/%d", LL("dengji"), iLv, iMaxLv)->getCString(), 16);
	Lvlab->setAnchorPoint(Vec2(0,1));
	Lvlab->setPosition(Vec2(122,310));
	bg->addChild(Lvlab);

	//攻击范围
	Sprite* hitArea = ITools::getInstance()->getSpriteWithPath(__String::createWithFormat("img/HitArea/%d.png", occu->attack_type_id + 1)->getCString());
	hitArea->setPosition(Vec2(238, 322));
	bg->addChild(hitArea);

	//基本信息
	Sprite* a_obtain = ReadSpriteName("new_ui/img_global/global_txt_img.png");
	a_obtain->setPosition(Vec2(bg->getContentSize().width / 2, 265));
	bg->addChild(a_obtain);

	int iPage = addSkillElem(bg)+1;

	Label* infoLab = LabelSystemFont(trData->actors[m_iHeroid -1].actor_info,16);
	infoLab->setDimensions(250, 0);
	infoLab->setAnchorPoint(Vec2(0,1));
	infoLab->setPosition(Vec2(25,240 - 50*iPage));
	bg->addChild(infoLab);

	return true;
}

int HeroInfoLayer::addSkillElem(Sprite* BgSp)
{
	int skill_elem_index = 0;
	int iSkillIndex = trData->actors[m_iHeroid - 1].skill;
	__String* iconPathStr = NULL;
	string SkillTypeStr = "new_ui/hero/skill_active.png";
	string SeidTypeStr = "new_ui/hero/skill_special.png";

	if (trData->net_hero_skill.count(iSkillIndex))
	{
		NetHeroSkillPtr ptrNHS = trData->net_hero_skill[iSkillIndex];
		for (map<int, int>::iterator mIt = ptrNHS->skill.begin(); mIt != ptrNHS->skill.end(); mIt++)
		{
			int iSkillId = mIt->first;
			if (trData->skill_num >= iSkillId)
			{
				int iPointX = 50 + (skill_elem_index % 5) * 50;
				int iPointY = 225 - (skill_elem_index / 5) * 50;
				iconPathStr = __String::createWithFormat("img/Skill/%d.png",trData->skills[iSkillId - 1].image_id + 1);
				Sprite* iconSp = ITools::getInstance()->getSpriteWithPath(iconPathStr->getCString());
				iconSp->setPosition(Vec2(iPointX, iPointY));
				iconSp->setScale(0.81f);
				BgSp->addChild(iconSp);

				Sprite* TypeSp = ReadSpriteName(SkillTypeStr.c_str());
				TypeSp->setPosition(Vec2(iPointX + 12, iPointY - 12));
				BgSp->addChild(TypeSp);

				skill_elem_index++;
				if (skill_elem_index>=10)
					break;
			}
		}
	}
	if (m_iHeroid - 1< trData->actor_num&&skill_elem_index<10)
	{
		Seid* seid = NULL;
		for (int i = 0; i < trData->actors[m_iHeroid - 1].seid_num; i++)
		{
			seid = trData->m_mSeid[trData->actors[m_iHeroid - 1].seid[i]];
			if (!seid->seid || seid->type == 385) continue;

			int iPointX = 50 + (skill_elem_index % 5) * 50;
			int iPointY = 225 - (skill_elem_index / 5) * 50;
			iconPathStr = __String::createWithFormat("img/Seid/%d.png", seid->imgid);
			Sprite* iconSp = ITools::getInstance()->getSpriteWithPath(iconPathStr->getCString());
			iconSp->setPosition(Vec2(iPointX, iPointY));
			iconSp->setScale(0.81f);
			BgSp->addChild(iconSp);

			Sprite* TypeSp = ReadSpriteName(SeidTypeStr.c_str());
			TypeSp->setPosition(Vec2(iPointX + 12, iPointY - 12));
			BgSp->addChild(TypeSp);

			skill_elem_index++;
			if (skill_elem_index >= 10)
				break;
		}
	}
	return skill_elem_index / 5;
}

bool HeroInfoLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}
void HeroInfoLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}
void HeroInfoLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	this->removeFromParent();
}
