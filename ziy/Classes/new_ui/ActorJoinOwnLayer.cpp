#include "ActorJoinOwnLayer.h"
#include "Defines.h"


ActorJoinOwnLayer* ActorJoinOwnLayer::create(int iActorId, int iFunctionType, EventDelegate* ptrDelegate)
{
	ActorJoinOwnLayer* pRet = new ActorJoinOwnLayer();
	if (pRet&&pRet->init(iActorId,iFunctionType,ptrDelegate))
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

bool ActorJoinOwnLayer::init(int iActorId, int iFunctionType, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mActorId = iActorId;
	mDelegate = ptrDelegate;
	bEnableRemove = false;

	ActorAttrPtr dActor = &trData->actors[mActorId - 1];
	HistoryActorPtr ptrActor = &history->actors[mActorId - 1];
	int actor_quality = dActor->actor_quality;
	if (iFunctionType == FUNCTION_ASCENDSTAIRS)
		actor_quality = ptrActor->actor_quality;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_bg = LayerColor::create(Color4B(0, 0, 0, 127));
	black_bg->setPosition(Vec2(0, 0));
	this->addChild(black_bg);

	Sprite* bg = ReadSpriteName("new_ui/hero/hero_bg1.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	//头像
	if (!ptrActor->actor_id&&ptrActor->mFashion[0])
	{
		string armaS = "r_female";
		if (ptrActor->mFashion[1] == 1)
			armaS = "r_male";
		__String* play_index = __String::createWithFormat("R%d_%d_1", ptrActor->mFashion[1], ptrActor->mFashion[2]);
		Armature* hero = Armature::create(armaS.c_str());
		hero->setAnchorPoint(Vec2(0.5, 0));
		hero->setScale(0.82f);
		hero->setPosition(Vec2(220, 5));
		hero->getAnimation()->play(play_index->getCString());
		bg->addChild(hero);
	}
	else
	{
		__String* face_path = __String::create("");
       face_path = __String::createWithFormat("img/Face/%d.png", dActor->face_id + 1);
        Sprite* face = ITools::getInstance()->getSpriteWithPath(face_path->getCString());
		face->setAnchorPoint(Vec2(0.5, 0));
		face->setScale(0.82f);
		face->setPosition(Vec2(220, 5));
		bg->addChild(face);
	}

	//姓名
	Sprite* name_bg = ReadSpriteName(StringUtils::format("new_ui/hero/heroname_q%d.png", actor_quality).c_str());
	name_bg->setPosition(Vec2(220, 5));
	bg->addChild(name_bg);

	Label* name = LabelSystemFont(dActor->getActorName(), 18);
	name->setPosition(Vec2(name_bg->getContentSize().width/2,33));
	name_bg->addChild(name);

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));

	__String* play_index = __String::createWithFormat("hero_q%d", actor_quality);
	Armature* img_arma = Armature::create("icon_eff");
	img_arma->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height-28));
	img_arma->getAnimation()->play(play_index->getCString());
	bg->addChild(img_arma);

	//武将背景
	Sprite* img_bg = ReadSpriteName("new_ui/hero/arms_bg.png");
	img_bg->setAnchorPoint(Vec2(0.5, 1));
	img_bg->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height));
	bg->addChild(img_bg);

	//时装
	__String* f_path = NULL;
	__String* img_path = NULL;
	if (ptrActor->mFashion[0])
	{
		f_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);
		img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
		Sprite* actor_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
		int fashion_width = actor_fashion->getContentSize().width;
		actor_fashion->setTextureRect(Rect(0, fashion_width * 4, fashion_width, fashion_width));
		actor_fashion->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2 + 24));
		img_bg->addChild(actor_fashion,trData->net_hero_dress[ptrActor->mFashion[4]]->mSpc[4]);
	}
	else
	{
		img_path = __String::create(ptrActor->getActorSImgPath("spc", 0));
	}
	//武将
	Sprite* actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
	int img_width = actor_img->getContentSize().width;
	actor_img->setTextureRect(Rect(0, img_width * 4, img_width, img_width));
	actor_img->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2+24));
	img_bg->addChild(actor_img);

	//最高等级
	int iMaxQ = actor_quality;

	if (iFunctionType == FUNCTION_JOIN&&ptrActor->GetSoulBox())
	{
		iMaxQ = 5;
	}

	Sprite* max_level = ReadSpriteName(__String::createWithFormat("new_ui/hero/maxlv_q%d.png", iMaxQ)->getCString());
	max_level->setAnchorPoint(Vec2(0,0));
	max_level->setPosition(Vec2(42,bg->getContentSize().height - 30));
	bg->addChild(max_level);

	//品质
	__String* jobimg_path = __String::createWithFormat("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId());
    CCLOG("%s",jobimg_path->getCString());
	Sprite* jobImgSp = ReadSpriteName(jobimg_path->getCString());
	jobImgSp->setPosition(Vec2(338, 50));
	bg->addChild(jobImgSp);

	if (iFunctionType == FUNCTION_ASCENDSTAIRS)
	{
		//573,283
		Sprite* pjts = ReadSpriteName("new_ui/img_text/ybt1_2.png");
		pjts->setPosition(Vec2(550, 282));
		bg->addChild(pjts);

		addGrownElem(bg);
	}
	else
	{
		//573,283
		Sprite* hdwj = ReadSpriteName("new_ui/img_text/ybt1_1.png");
		hdwj->setPosition(Vec2(550, 282));
		bg->addChild(hdwj);

		Sprite* actor_skill = ReadSpriteName("new_ui/hero/hero_txt3.png");
		actor_skill->setPosition(Vec2(550, 242));
		bg->addChild(actor_skill);

		addSkillElem(bg);
	}

	this->scheduleOnce(schedule_selector(ActorJoinOwnLayer::updateEnableRemove),1);

	return true;
}

void ActorJoinOwnLayer::addSkillElem(Sprite* ptrBg)
{
	//435,195
	ActorAttrPtr dActor = &trData->actors[mActorId - 1];
	if (dActor->skill <= 0) return;
	NetHeroSkillPtr ptrNHS = trData->net_hero_skill[dActor->skill];
	int skill_num = 0;
	for (map<int, int>::iterator it = ptrNHS->skill.begin(); it != ptrNHS->skill.end();it++)
	{
		int skill_id = it->first;
		SkillAttrPtr ptrSkill = &trData->skills[skill_id - 1];
		__String* img_path = __String::createWithFormat("img/Skill/%d.png", ptrSkill->image_id+1);
		Sprite* skill_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
		skill_img->setPosition(Vec2(450, 195 - skill_num*72));
		ptrBg->addChild(skill_img);

		int text_x = 452 + skill_img->getContentSize().width / 2;
		int text_y = 195 - skill_num * 72 + skill_img->getContentSize().height / 2;
		Label* skill_name = LabelSystemFont(ptrSkill->skill_name,18);
		skill_name->setColor(Color3B::YELLOW);
		skill_name->setAnchorPoint(Vec2(0, 1));
		skill_name->setPosition(Vec2(text_x, text_y));
		ptrBg->addChild(skill_name);

		Label* skill_info = LabelSystemFont(ptrSkill->skill_effect, 16);
		skill_info->setAnchorPoint(Vec2(0, 1));
		skill_info->setPosition(Vec2(text_x, text_y-22));
		ptrBg->addChild(skill_info);

		skill_num++;
		if (skill_num>=3)  break;
	}
}

void ActorJoinOwnLayer::addAbilityGrownElem(Sprite* ptrBg, char* ptrBbilityName, float iNumOld, float iNumNew,int iOffset)
{
	int ttf_point_y = 224 - 41 * iOffset;
	int ttf_point_x = 440;
	Label* atk_ttf = LabelSystemFont(ptrBbilityName, 16);
	atk_ttf->setColor(Color3B::YELLOW);
	atk_ttf->setAnchorPoint(Vec2(0, 1));
	atk_ttf->setPosition(Vec2(ttf_point_x, ttf_point_y));
	ptrBg->addChild(atk_ttf);

	int num_point_x = ttf_point_x + atk_ttf->getContentSize().width + 20;
	__String* atk_num_old = __String::createWithFormat("%0.1f", iNumOld);
	Label* atk_num = LabelSystemFont(atk_num_old->getCString(), 16);
	atk_num->setAnchorPoint(Vec2(0, 1));
	atk_num->setPosition(Vec2(num_point_x, ttf_point_y));
	ptrBg->addChild(atk_num);

	num_point_x += atk_num->getContentSize().width + 37;
	Sprite* arrow = ReadSpriteName("new_ui/img_global/global_sj.png");
	arrow->setPosition(Vec2(num_point_x, ttf_point_y - atk_num->getContentSize().height / 2));
	ptrBg->addChild(arrow);

	num_point_x += 37;
	__String* atk_num_new = __String::createWithFormat("%0.1f", iNumNew);
	Label* atk_numN = LabelSystemFont(atk_num_new->getCString(), 16);
	atk_numN->setAnchorPoint(Vec2(0, 1));
	atk_numN->setPosition(Vec2(num_point_x, ttf_point_y));
	ptrBg->addChild(atk_numN);
}

void ActorJoinOwnLayer::addGrownElem(Sprite* ptrBg)
{
	ActorAttrPtr dActor = &trData->actors[mActorId - 1];
	HistoryActorPtr ptrActor = &history->actors[mActorId - 1];

	//436,248
	int ttf_point_y = 245;
	int ttf_point_x = 440;
	GrownAbilityInfoPtr ptrGAIOld = trData->net_hero_grown[dActor->qid][ptrActor->actor_quality - 1];
	GrownAbilityInfoPtr ptrGAINew = trData->net_hero_grown[dActor->qid][ptrActor->actor_quality];

	addAbilityGrownElem(ptrBg, LL("gongjichengzhang"), ptrGAIOld->atk, ptrGAINew->atk, 0);

	addAbilityGrownElem(ptrBg, LL("fangyuchengzhang"), ptrGAIOld->def, ptrGAINew->def, 1);

	addAbilityGrownElem(ptrBg, LL("jingshenchengzhang"), ptrGAIOld->ints, ptrGAINew->ints, 2);

	addAbilityGrownElem(ptrBg, LL("baofachengzhang"), ptrGAIOld->burst, ptrGAINew->burst, 3);

	addAbilityGrownElem(ptrBg, LL("shiqichengzhang"), ptrGAIOld->fortune, ptrGAINew->fortune, 4);
}

bool ActorJoinOwnLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	return true;
}

void ActorJoinOwnLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (bEnableRemove)
	{
		if (mDelegate)
			mDelegate->ChildCloseLayer(0);
		this->removeFromParent();
	}
}

void ActorJoinOwnLayer::updateEnableRemove(float f)
{
	bEnableRemove = true;
}

ActorJoinOwnLayer::ActorJoinOwnLayer()
{

}

ActorJoinOwnLayer::~ActorJoinOwnLayer()
{

}
void ActorJoinOwnLayer::onExit()
{
	Layer::onExit();
}

void ActorJoinOwnLayer::onEnter()
{
	Layer::onEnter();
	ChessPlayNetSound(202);
}