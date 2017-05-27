#include "ActorInfoLayer.h"
#include "Defines.h"
#include "SkillAndGoodsInfoLayer.h"
#include "ActorSourceLayer.h"
#include "PopLayer.h"
#include "ActorJoinOwnLayer.h"
#include "NetEquipLayer.h"
#include "EquipFashionLayer.h"
#include "Net_HomePage.h"
#include "NetPromptTool.h"
#include "ActorPracticeLayer.h"
#include "GuideLayer.h"
#include "Net_HomePage.h"

ActorInfoLayer* ActorInfoLayer::create(HistoryActorPtr Actor,int iInfoState, EventDelegate* ptrDelegate)
{
	ActorInfoLayer* pRet = new ActorInfoLayer();
	pRet->mDelegate = ptrDelegate;
	pRet->mInfoState = iInfoState;
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

bool ActorInfoLayer::init(HistoryActorPtr Actor)
{
	if (!Layer::init())
	{
		return false;
	}
	m_iOwnActorIndex = 0;
	if (mInfoState == BATTLE_STATE_INFO)
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
	if (history->game_state == GAME_STATE_REDE&&!Actor->relation)
		GetOwnActorList(Actor);


	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	curActor = Actor;
	bIsSkillTbv = false;
	mMenuSZ = NULL;
	mMenuXL = NULL;
	mMenuJJ = NULL;
	mMenuPower = NULL;
	listLeftMenu = NULL;
	listRightMenu = NULL;
	mCloseMenu = NULL;


	//背景
	Sprite* bg = ReadSpriteName("new_ui/img_global/global_bg.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	 
	//背景文字
	Sprite* bg_txt = ReadSpriteName("new_ui/img_text/bt1_2.png");
	bg_txt->setPosition(Vec2(bg->getContentSize().width/2-45, 360));
	bg->addChild(bg_txt);
	 
	//武将信息背景
	Sprite* right_bg = ReadSpriteName("new_ui/img_global/hero_bg3.png");
	right_bg->setAnchorPoint(Vec2(0, 1));
	right_bg->setPosition(Vec2(292 + 9, 306 + 27));
	bg->addChild(right_bg);

	//姓名背景
	name_bg = ReadSpriteName(StringUtils::format("new_ui/hero/heroname_q%d.png", Actor->actor_quality).c_str());
	name_bg->setPosition(Vec2(right_bg->getContentSize().width/2+18+15,right_bg->getContentSize().height-25));
	right_bg->addChild(name_bg);

	//姓名//等级
	name = LabelSystemFont("", 18);
	name->setPosition(Vec2(name_bg->getContentSize().width/4,31));
	name_bg->addChild(name);
	name->enableOutline(Color4B::BLACK, 1);
	 
	int teoffset = 12;
	//武将头像
	__String* face_path = __String::create("");
    face_path = __String::createWithFormat("%s%d%s", "img/Face_s/", Actor->face_id + 1, ".png");
	if (!Actor->face_id && history->m_iBattleType == BATTLE_PVP)
	{
		face_path = __String::createWithFormat("img/Face_s/%d_%d_%d.png", Actor->face_id + 1, Actor->mFashion[1], Actor->mFashion[2]);
	}
	else if ((!Actor->actor_id || ((history->m_iBattleType == BATTLE_LADDER|| history->m_iBattleType == BATTLE_IMAGE) && history->mLadderEActor[Actor->actor_id] == 1)) && Actor->mFashion[0] > 0)
	{
       face_path = __String::createWithFormat("img/Face_s/%d_%d_%d.png", Actor->face_id + 1, Actor->mFashion[1], Actor->mFashion[2]);
    }
	actor_face = ITools::getInstance()->getSpriteWithPath(face_path->getCString());
	actor_face->setFlippedX(true);
	actor_face->setScale(0.55f);
	actor_face->setAnchorPoint(Vec2(0, 0));
	actor_face->setPosition(Vec2(174, 193));
	right_bg->addChild(actor_face);
	 
	//hp
	Label* hp = LabelSystemFont(LL("shengming"), 16);
	hp->setAnchorPoint(Vec2(0, 0));
	hp->setPosition(Vec2(35, 228 + teoffset));
	right_bg->addChild(hp);
	 
	//mp
	Label* mp = LabelSystemFont(LL("fali"), 16);
	mp->setAnchorPoint(Vec2(0, 0));
	mp->setPosition(Vec2(35, 208 + teoffset));
	right_bg->addChild(mp);
	 
	//exp
	Label* exp = LabelSystemFont(LL("jingyan"), 16);
	exp->setAnchorPoint(Vec2(0, 0));
	exp->setPosition(Vec2(35, 187 + teoffset));
	right_bg->addChild(exp);
	 
	//hp色条
	Sprite* bar_bg_hp = ReadSpriteName("new_ui/hero/strip_bg.png");
	bar_bg_hp->setAnchorPoint(Vec2(0, 0));
	bar_bg_hp->setPosition(Vec2(66, 228 + teoffset));
	right_bg->addChild(bar_bg_hp);
	 
	bar_hp = ReadSpriteName("new_ui/hero/strip_hp.png");
	bar_hp->setAnchorPoint(Vec2(0, 0));
	bar_hp->setPosition(Vec2(4, 4));
	bar_bg_hp->addChild(bar_hp);
	 
	bar_hp_ttf = LabelSystemFont("", 14);
	bar_hp_ttf->setPosition(Vec2(49, 9));
	bar_bg_hp->addChild(bar_hp_ttf);
	 
	//mp色条
	Sprite* bar_bg_mp = ReadSpriteName("new_ui/hero/strip_bg.png");
	bar_bg_mp->setAnchorPoint(Vec2(0, 0));
	bar_bg_mp->setPosition(Vec2(66, 208 + teoffset));
	right_bg->addChild(bar_bg_mp);
	 
	bar_mp = ReadSpriteName("new_ui/hero/strip_mp.png");
	bar_mp->setAnchorPoint(Vec2(0, 0));
	bar_mp->setPosition(Vec2(4, 4));
	bar_bg_mp->addChild(bar_mp);
	 
	bar_mp_ttf = LabelSystemFont("", 14);
	bar_mp_ttf->setPosition(Vec2(49, 9));
	bar_bg_mp->addChild(bar_mp_ttf);
	 
	//exp色条
	Sprite* bar_bg_exp = ReadSpriteName("new_ui/hero/strip_bg.png");
	bar_bg_exp->setAnchorPoint(Vec2(0, 0));
	bar_bg_exp->setPosition(Vec2(66, 187 + teoffset));
	right_bg->addChild(bar_bg_exp);
	 
	bar_exp = ReadSpriteName("new_ui/hero/strip_exp.png");
	bar_exp->setAnchorPoint(Vec2(0, 0));
	bar_exp->setPosition(Vec2(4, 4));
	bar_bg_exp->addChild(bar_exp);
	 
	bar_exp_ttf = LabelSystemFont("", 14);
	bar_exp_ttf->setPosition(Vec2(49, 9));
	bar_bg_exp->addChild(bar_exp_ttf);
	 

	//品质288，215
	actor_job = ReadSpriteName("new_ui/hero/job1.png");
	actor_job->setPosition(Vec2(284, 204 + teoffset));
	right_bg->addChild(actor_job);
	     
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/special_eff.ExportJson", 0, 1));
	 	
	//武器
	we_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	we_bg->setPosition(Vec2(56, 143));
	right_bg->addChild(we_bg);
	
	int off_setX = we_bg->getContentSize().width / 2;
	int off_setY = we_bg->getContentSize().height / 2;

	we_icon = ITools::getInstance()->getSpriteWithPath("img/Items/a1.png");
	we_icon->setPosition(Vec2(off_setX, off_setY));
	we_bg->addChild(we_icon);

	spWeStone = ITools::getInstance()->getSpriteWithPath("img/Items/a1.png");
	spWeStone->setScale(0.3f);
	spWeStone->setAnchorPoint(Vec2(0.0f, 0.5f));
	spWeStone->setPosition(Vec2(3, off_setY));
	we_bg->addChild(spWeStone);
	 
	we_arm = Armature::create("special_eff");
	we_arm->setPosition(Vec2(off_setX, off_setY));
	we_arm->getAnimation()->play("q0");
	we_bg->addChild(we_arm);
	 
	we_quality = ReadSpriteName("new_ui/img_number/star_b_1.png");
	we_quality->setPosition(Vec2(off_setX, 6));
	we_bg->addChild(we_quality);

	we_add = ReadSpriteName("new_ui/hero/hero_add.png");
	we_add->setPosition(Vec2(off_setX, off_setY));
	we_bg->addChild(we_add);
	 
	we_effect = LabelSystemFont("", 16);
	we_effect->setColor(Color3B(102,0,0));
	we_effect->setPosition(Vec2(off_setX, -17));
	we_bg->addChild(we_effect);

	HongDian::getInstence()->addHongDianNode(we_bg,2001);
	 
	//防具
	co_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	co_bg->setPosition(Vec2(147, 143));
	right_bg->addChild(co_bg);
	 
	co_icon = ITools::getInstance()->getSpriteWithPath("img/Items/a2.png");
	co_icon->setPosition(Vec2(off_setX, off_setY));
	co_bg->addChild(co_icon);

	spCoStone = ITools::getInstance()->getSpriteWithPath("img/Items/a1.png");
	spCoStone->setScale(0.3f);
	spCoStone->setAnchorPoint(Vec2(0.0f, 0.5f));
	spCoStone->setPosition(Vec2(3, off_setY));
	co_bg->addChild(spCoStone);
	 
	co_arm = Armature::create("special_eff");
	co_arm->setPosition(Vec2(off_setX, off_setY));
	co_arm->getAnimation()->play("q0");
	co_bg->addChild(co_arm);
	 
	co_quality = ReadSpriteName("new_ui/img_number/star_b_1.png");
	co_quality->setPosition(Vec2(off_setX, 6));
	co_bg->addChild(co_quality);

	co_add = ReadSpriteName("new_ui/hero/hero_add.png");
	co_add->setPosition(Vec2(off_setX, off_setY));
	co_bg->addChild(co_add);
	 
	co_effect = LabelSystemFont("", 16);
	co_effect->setColor(Color3B(102, 0, 0));
	co_effect->setPosition(Vec2(off_setX, -17));
	co_bg->addChild(co_effect);

	HongDian::getInstence()->addHongDianNode(co_bg, 2002);
	 
	//辅助
	as_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	as_bg->setPosition(Vec2(238, 143));
	right_bg->addChild(as_bg);
	 
	as_icon = ITools::getInstance()->getSpriteWithPath("img/Items/a3.png");
	as_icon->setPosition(Vec2(off_setX, off_setY));
	as_bg->addChild(as_icon);

	spAsStone = ITools::getInstance()->getSpriteWithPath("img/Items/a1.png");
	spAsStone->setScale(0.3f);
	spAsStone->setAnchorPoint(Vec2(0.0f, 0.5f));
	spAsStone->setPosition(Vec2(3, off_setY));
	as_bg->addChild(spAsStone);
	
	as_add = ReadSpriteName("new_ui/hero/hero_add.png");
	as_add->setPosition(Vec2(off_setX, off_setY));
	as_bg->addChild(as_add);

	as_effect = LabelSystemFont("", 16);
	as_effect->setColor(Color3B(102, 0, 0));
	as_effect->setPosition(Vec2(off_setX, -17));
	as_bg->addChild(as_effect);

	HongDian::getInstence()->addHongDianNode(as_bg, 2003);
	 
	//异常状态说明
	debuff = LabelSystemFont("", 16);
	debuff->setDimensions(260, 60);
	debuff->setAnchorPoint(Vec2(0, 1));
	debuff->setPosition(Vec2(18, 70));
	debuff->setHorizontalAlignment(TextHAlignment::LEFT);
	right_bg->addChild(debuff);

	if (mInfoState == READ_STATE_INFO)
	{
		debuff->setVisible(false);
		Sprite* p_mn = NULL;
		Sprite* p_ms = NULL;
		Sprite* p_md = NULL;

		//修炼
		Sprite* X_bg = ReadSpriteName("new_ui/hero/hero_icon_bg.png");
		X_bg->setPosition(Vec2(120, 40));
		right_bg->addChild(X_bg);
		mMenuXL = CCMenu::create();
		mMenuXL->setPosition(Vec2(120, 40));
		right_bg->addChild(mMenuXL);
		p_mn = ReadSpriteName("new_ui/hero/hero_icon2.png");
		p_ms = ReadSpriteName("new_ui/hero/hero_icon2.png");
		p_ms->setScale(0.9f);
		MenuItemSprite* p_menux = MenuItemSprite::create(p_mn, p_ms, CC_CALLBACK_1(ActorInfoLayer::clickCultivationMenu, this));
		p_ms->setAnchorPoint(Vec2(0.5, 0.5));
		p_ms->setPosition(Vec2(p_ms->getContentSize().width / 2, p_ms->getContentSize().height / 2));
		mMenuXL->addChild(p_menux);

		HongDian::getInstence()->addHongDianNode(X_bg, 2004);

 		//神装
 		Sprite* S_bg = ReadSpriteName("new_ui/hero/hero_icon_bg.png");
 		S_bg->setPosition(Vec2(185, 40));
 		right_bg->addChild(S_bg);
 		mMenuSZ = Menu::create();
 		mMenuSZ->setPosition(Vec2(185, 40));
 		right_bg->addChild(mMenuSZ);
 		p_mn = ReadSpriteName("new_ui/hero/hero_icon3.png");
 		p_ms = ReadSpriteName("new_ui/hero/hero_icon3.png");
 		p_md = ReadSpriteName("new_ui/hero/hero_icon3.png");
		p_ms->setScale(0.9f);
 		MenuItemSprite* p_menuS = MenuItemSprite::create(p_mn, p_ms, CC_CALLBACK_1(ActorInfoLayer::clickEquipmentMenu,this));
  		p_ms->setAnchorPoint(Vec2(0.5, 0.5));
  		p_ms->setPosition(Vec2(p_ms->getContentSize().width / 2, p_ms->getContentSize().height / 2));
 		mMenuSZ->addChild(p_menuS);

		HongDian::getInstence()->addHongDianNode(S_bg, 2005);

		//进阶
		Sprite* J_bg = ReadSpriteName("new_ui/hero/hero_icon_bg.png");
		J_bg->setPosition(Vec2(250, 40));
		right_bg->addChild(J_bg);
		mMenuJJ = Menu::create();
		mMenuJJ->setPosition(Vec2(250, 40));
		right_bg->addChild(mMenuJJ);
		p_mn = ReadSpriteName("new_ui/hero/hero_icon1.png");
		p_ms = ReadSpriteName("new_ui/hero/hero_icon1.png");
		p_md = ReadSpriteName("new_ui/hero/hero_icon1.png");
		p_ms->setScale(0.9f);
		MenuItemSprite* p_menuj = MenuItemSprite::create(p_mn, p_ms, p_md, CC_CALLBACK_1(ActorInfoLayer::clickPromoteMenu, this));
		p_ms->setAnchorPoint(Vec2(0.5, 0.5));
		p_ms->setPosition(Vec2(p_ms->getContentSize().width / 2, p_ms->getContentSize().height / 2));
		mMenuJJ->addChild(p_menuj);

		HongDian::getInstence()->addHongDianNode(J_bg, 2006);

	}
	//基本属性
	info1 = ReadSpriteName("new_ui/img_text/subtitle_4.png");
	info1->setPosition(Vec2(150, 315));
	bg->addChild(info1);

	left_bg = ReadSpriteName("new_ui/img_global/hero_bg2.png");
	left_bg->setPosition(Vec2(20, 306-8));
	left_bg->setAnchorPoint(Vec2(0, 1));
	bg->addChild(left_bg);

	//阵营
	relation = ReadSpriteName("new_ui/hero/icon_we.png");
	relation->setPosition(Vec2(45, 327 - 12));
	bg->addChild(relation);
	if (mInfoState == READ_STATE_INFO)
		relation->setVisible(false);

	//能力table
	power_tbv = ActorTableView::create(Actor, 1);
	power_tbv->setPosition(power_tbv->getPosition()+Vec2(0,-5));
	left_bg->addChild(power_tbv);

	skill_tbv = SkillTableView::create(Actor,this);
	skill_tbv->setPosition(skill_tbv->getPosition() + Vec2(0, -10));
	skill_tbv->setVisible(false);
	left_bg->addChild(skill_tbv);

	occu_info = ActorOccuInfoLayer::create(Actor);
	occu_info->setPosition(occu_info->getPosition() + Vec2(0, -10));
	occu_info->setVisible(false);
	left_bg->addChild(occu_info);

	Armature* switch1 = Armature::create("special_eff");
	switch1->setPosition(Vec2(15,166));
	switch1->getAnimation()->play("hd1");
	bg->addChild(switch1);

	Armature* switch2 = Armature::create("special_eff");
	switch2->setPosition(Vec2(599, 166));
	switch2->setScaleX(-1);
	switch2->getAnimation()->play("hd1");
	bg->addChild(switch2);

	//按钮
	mMenuPower = Menu::create();
	//PtrMenu->setAnchorPoint(ccp(0, 1));
	mMenuPower->setPosition(Vec2(26, 52));
	bg->addChild(mMenuPower);

	Sprite* p_m = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* p_ms = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* p_m_icon = ReadSpriteName("new_ui/img_text/imgtxt3_1.png");
	Sprite* p_ms_icon = ReadSpriteName("new_ui/img_text/imgtxt3_2.png");
	p_m_icon->setPosition(Vec2(40, 18));
	p_ms_icon->setPosition(Vec2(40, 18));
	p_m->addChild(p_m_icon);
	p_ms->addChild(p_ms_icon);

	MenuItemSprite* p_menu = MenuItemSprite::create(p_m, p_ms, CC_CALLBACK_1(ActorInfoLayer::clickPowerMenu, this));
	select_menu = p_menu;
	p_menu->selected();
	p_menu->setAnchorPoint(Vec2(0, 1));
	p_menu->setPosition(Vec2(0,0));
	mMenuPower->addChild(p_menu);

	Sprite* s_m = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* s_ms = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* s_m_icon = ReadSpriteName("new_ui/img_text/imgtxt3_3.png");
	Sprite* s_ms_icon = ReadSpriteName("new_ui/img_text/imgtxt3_4.png");
	s_m_icon->setPosition(Vec2(40, 18));
	s_ms_icon->setPosition(Vec2(40, 18));
	s_m->addChild(s_m_icon);
	s_ms->addChild(s_ms_icon);

	MenuItemSprite* s_menu = MenuItemSprite::create(s_m, s_ms, CC_CALLBACK_1(ActorInfoLayer::clickSkillMenu, this));
	s_menu->setAnchorPoint(Vec2(0, 1));
	s_menu->setPosition(Vec2(83, 0));
	mMenuPower->addChild(s_menu);

	HongDian::getInstence()->addHongDianNode(s_menu, 2007);

	Sprite* o_m = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* o_ms = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* o_m_icon = ReadSpriteName("new_ui/img_text/imgtxt3_5.png");
	Sprite* o_ms_icon = ReadSpriteName("new_ui/img_text/imgtxt3_6.png");
	o_m_icon->setPosition(Vec2(40, 18));
	o_ms_icon->setPosition(Vec2(40, 18));
	o_m->addChild(o_m_icon);
	o_ms->addChild(o_ms_icon);

	MenuItemSprite* o_menu = MenuItemSprite::create(o_m, o_ms, CC_CALLBACK_1(ActorInfoLayer::clickOccuMenu, this));
	o_menu->setAnchorPoint(Vec2(0, 1));
	o_menu->setPosition(Vec2(166, 0));
	mMenuPower->addChild(o_menu);

	listLeftMenu = Menu::create();
	listLeftMenu->setPosition(Vec2(15, 166));
	Sprite* listLeftNormalImage = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* listLeftSelectImage = ReadSpriteName("new_ui/img_global/switch1.png");
	listLeftNormalImage->setOpacity(0);
	listLeftSelectImage->setOpacity(0);
	MenuItemSprite* listLeft = MenuItemSprite::create(listLeftNormalImage, listLeftSelectImage, CC_CALLBACK_1(ActorInfoLayer::clickListLeft, this));
	listLeftMenu->addChild(listLeft);
	bg->addChild(listLeftMenu);

	listRightMenu = Menu::create();
	listRightMenu->setPosition(Vec2(599, 166));
	Sprite* listRightNormalImage = ReadSpriteName("new_ui/img_global/switch1.png");
	listRightNormalImage->setFlippedX(true);
	listRightNormalImage->setOpacity(0);
	Sprite* listRightSelectImage = ReadSpriteName("new_ui/img_global/switch1.png");
	listRightSelectImage->setFlippedX(true);
	listRightSelectImage->setOpacity(0);
	MenuItemSprite* listRight = MenuItemSprite::create(listRightNormalImage, listRightSelectImage, CC_CALLBACK_1(ActorInfoLayer::clickListRight, this));
	listRightMenu->addChild(listRight);
	bg->addChild(listRightMenu);

	mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(600-3, 358));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press1.png");
	close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ActorInfoLayer::clickCloseLayer, this));
	mCloseMenu->addChild(close_menu);

	
	this->UpdateActorDate(Actor);
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, ANNIU_HONGDIAN);
	//history->updateGuide(EMIL_GUIDE);
	for (auto offset : bg->getChildren())
	{
		offset->setPosition(Vec2(offset->getPositionX() + 45, offset->getPositionY() + 20));
	}
	return true;
}

void ActorInfoLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (mDelegate != NULL&&mInfoState == READ_STATE_INFO)
		mDelegate->ChildCloseLayer(0);
	else if (mDelegate != NULL&&mInfoState == BATTLE_STATE_INFO)
		mDelegate->ResetBattleScreenState();
	//history->updateGuide(EMIL_GUIDE);
	this->removeFromParent();
	HongDian::getInstence()->autoUpdateWuJiang(WUJIANG_JIEMIAN);
}

void ActorInfoLayer::clickPowerMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	info1->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/img_text/subtitle_4.png")));
	MenuItemSprite* m_menu = (MenuItemSprite*)sender;
	select_menu->unselected();
	m_menu->selected();
	select_menu = m_menu;
	power_tbv->setVisible(true);
	skill_tbv->setVisible(false);
	occu_info->setVisible(false);
	bIsSkillTbv = false;
}

void ActorInfoLayer::clickSkillMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	info1->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/img_text/subtitle_5.png")));
	MenuItemSprite* m_menu = (MenuItemSprite*)sender;
	select_menu->unselected();
	m_menu->selected();
	select_menu = m_menu;
	power_tbv->setVisible(false);
	skill_tbv->setVisible(true);
	occu_info->setVisible(false);
	bIsSkillTbv = true;
}

void ActorInfoLayer::clickOccuMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	info1->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/img_text/subtitle_6.png")));
	MenuItemSprite* m_menu = (MenuItemSprite*)sender;
	select_menu->unselected();
	m_menu->selected();
	select_menu = m_menu;
	power_tbv->setVisible(false);
	skill_tbv->setVisible(false);
	occu_info->setVisible(true);
	bIsSkillTbv = false;
}
void ActorInfoLayer::clickListLeft(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	SetNextActor(false);
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, ANNIU_HONGDIAN);
}
void ActorInfoLayer::clickListRight(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	SetNextActor(true);
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, ANNIU_HONGDIAN);
}

void ActorInfoLayer::clickEquipmentMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	EquipFashionLayer* ptrEquipF = EquipFashionLayer::create(curActor, this);
	if (ptrEquipF)
		ChessApplication::getInstance()->AddFunctionLayer(ptrEquipF);
	else
		ITools::getInstance()->createGlobalPrompt(LL("wjwkfsz"),false);
}
void ActorInfoLayer::clickCultivationMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (curActor->actor_quality >= trData->net_hero_quality.rbegin()->first)
	{
		ChessApplication::getInstance()->AddFunctionLayer(ActorPracticeLayer::create(curActor, this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("practiceopennotice"), false);
	}
}
void ActorInfoLayer::clickPromoteMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	CCLOG("jinjie");
	int iActorId = curActor->actor_id + 1;
	if (trData->net_hero_quality.find(curActor->actor_quality + 1) != trData->net_hero_quality.end())
	{
		int iCurSoulNum = history->mChipSoulDate->GetActorChipNumByActorId(iActorId);
		int iRankSoulNum = trData->net_hero_quality[curActor->actor_quality]->rank_soul;
		if (iCurSoulNum >= iRankSoulNum)
			ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_WJ_JJ, this));
		else
			ChessApplication::getInstance()->AddFunctionLayer(ActorSourceLayer::create(iActorId, iCurSoulNum, iRankSoulNum, this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("wjyddzgpz"), false);
	}
}

void ActorInfoLayer::clickNullMenu(Ref* sender)
{
	CCLOG("clickNullMenu");
}

void ActorInfoLayer::SetPopLayerElement(Sprite* ptrBg)
{
	Label* title = LabelSystemFont(LL("wjjjktsnl"), 18);
	title->setPosition(Vec2(ptrBg->getContentSize().width / 2, 143));
	ptrBg->addChild(title);
	Sprite* sual = ReadSpriteName("new_ui/hero/icon_soul.png");
	sual->setPosition(Vec2(156,109));
	ptrBg->addChild(sual);
	int iCurSoulNum = history->mChipSoulDate->GetActorChipNumByActorId(curActor->actor_id + 1);
	int iRankSoul = trData->net_hero_quality[curActor->actor_quality]->rank_soul;
	__String* sual_num = __String::createWithFormat("%d/%d", iCurSoulNum, iRankSoul);
	Label* sual_ttf = LabelSystemFont(sual_num->getCString(), 16); 
	sual_ttf->setAnchorPoint(Vec2(0, 0.5));
	sual_ttf->setColor(Color3B(0, 255, 255));
	sual_ttf->setPosition(Vec2(156+sual->getContentSize().width,109));
	ptrBg->addChild(sual_ttf);
}

void ActorInfoLayer::RunFunction(int iNomal)
{
	SendNetDate();
}

void ActorInfoLayer::updateShow(int index)
{
	UpdateCurrentDate();
}
void ActorInfoLayer::updateMoneyShow()
{
	if (mDelegate)
		mDelegate->updateMoneyShow();
}

void ActorInfoLayer::RemoveAllParentLayer()
{
	if (mDelegate)
		mDelegate->RemoveAllParentLayer();
	this->removeFromParent();
}

void ActorInfoLayer::UpdateActorDate(HistoryActorPtr ptrActor)
{
	if (!ptrActor)
		return;
	__String* face_path = __String::create("");
    face_path = __String::createWithFormat("%s%d%s", "img/Face_s/", ptrActor->face_id+1, ".png");
	if (!ptrActor->face_id && history->m_iBattleType == BATTLE_PVP)
	{
		face_path = __String::createWithFormat("img/Face_s/%d_%d_%d.png", ptrActor->face_id + 1, ptrActor->mFashion[1], ptrActor->mFashion[2]);
	}
	else if ((!ptrActor->actor_id || ((history->m_iBattleType == BATTLE_LADDER || history->m_iBattleType == BATTLE_IMAGE) && history->mLadderEActor[ptrActor->actor_id] == 1)) && ptrActor->mFashion[0]>0)
	{
        face_path = __String::createWithFormat("img/Face_s/%d_%d_%d.png", ptrActor->face_id + 1, ptrActor->mFashion[1], ptrActor->mFashion[2]);
    }
	actor_face->setTexture(ITools::getInstance()->getTextureWithPath(face_path->getCString()));

	name_bg->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/heroname_q%d.png", ptrActor->actor_quality).c_str())));

	name->setString(__String::createWithFormat("%s Lv%d", trData->actors[ptrActor->actor_id].getActorName(),ptrActor->GetActorLv())->getCString());

	actor_job->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId()).c_str())));

	float r_hp = ptrActor->GetCurHp();
	float m_hp = ptrActor->GetBaseHp() + ptrActor->GetGoodsAddHp();
	float f_bar_hp = r_hp / m_hp;
	bar_hp->setScaleX(f_bar_hp);

	__String* bar_hp_ttfCs = __String::createWithFormat("%d/%d", ptrActor->GetCurHp(), ptrActor->GetBaseHp() + ptrActor->GetGoodsAddHp());
	bar_hp_ttf->setString(bar_hp_ttfCs->getCString());

	float r_mp = ptrActor->GetCurMp();
	float m_mp = ptrActor->GetBaseMp() + ptrActor->GetGoodsAddMp();
	float f_bar_mp = r_mp / m_mp;
	bar_mp->setScaleX(f_bar_mp);

	__String* bar_mp_ttfCs = __String::createWithFormat("%d/%d", ptrActor->GetCurMp(), ptrActor->GetBaseMp() + ptrActor->GetGoodsAddMp());
	bar_mp_ttf->setString(bar_mp_ttfCs->getCString());

	int iActorLv = ptrActor->GetActorLv();
	if (history->game_state == GAME_STATE_BATTLE&&ptrActor->GetActorOldLv() > 0)
		iActorLv = ptrActor->GetActorOldLv();
	float r_exp = ptrActor->exp;
	float m_exp = trData->net_hero_exp[iActorLv]->mExp;

	float f_bar_exp = r_exp / m_exp;
	bar_exp->setScaleX(f_bar_exp);


	__String* bar_exp_ttfCs = __String::createWithFormat("%d/%d", ptrActor->exp, (int)m_exp);
	bar_exp_ttf->setString(bar_exp_ttfCs->getCString());

	if (mInfoState == BATTLE_STATE_INFO)
	{
		string relationS = "new_ui/hero/icon_friend.png";
		if (ptrActor->relation == 0)
			relationS = "new_ui/hero/icon_we.png";
		else if (ptrActor->relation == 2)
			relationS = "new_ui/hero/icon_enemy.png";
		relation->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(relationS.c_str())));
	}
	__String* quality_path = __String::createWithFormat("new_ui/hero/hero_q%d.png", ptrActor->actor_quality);

	//actor_quality->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(quality_path->getCString())));

	__String* we_iconCs = NULL;
	__String* we_armCs = NULL;
	__String* we_qualityCs = NULL;
	string we_effectS = "";
	if (ptrActor->weapon.isNormalGood())
	{
		int iGoodsId = ptrActor->weapon.good_id;
		int iGoodsLevel = ptrActor->weapon.GetRealLevel();
		we_iconCs = __String::createWithFormat("img/Items/%d.png", trData->m_mGoodsItem[iGoodsId]->iImgId);

		int quality = 0;
		if (ptrActor->weapon.iQuality - 1 > 0){
			quality = trData->strengthen_consume[ptrActor->weapon.iQuality - 1].quality;
		}
		we_armCs = __String::createWithFormat("q%d", quality);

		if (ptrActor->weapon.iQuality > 0)
			we_qualityCs = __String::createWithFormat("new_ui/img_number/star_b_%d.png", ptrActor->weapon.iQuality);
		we_effectS = trData->m_mGoodsItem[iGoodsId]->GetSpecialEffectName(iGoodsLevel);

		we_add->setVisible(false);
	}
	else
	{
		if (history->game_state == GAME_STATE_REDE)
			we_add->setVisible(true);
		else
			we_add->setVisible(false);
	}

	if (we_iconCs)
	{
		we_icon->setTexture(ITools::getInstance()->getTextureWithPath(we_iconCs->getCString()));
	}
	else
	{
		we_icon->setTexture(ITools::getInstance()->getTextureWithPath("img/Items/a1.png"));
	}
	if (we_armCs)
	{
		we_arm->getAnimation()->play(we_armCs->getCString());
	}
	else
		we_arm->getAnimation()->play("q0");

	if (we_qualityCs)
	{
		we_quality->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(we_qualityCs->getCString())));
		we_quality->setVisible(true);
	}
	else
		we_quality->setVisible(false);
	we_effect->setString(we_effectS.c_str());

	__String* co_iconCs = NULL;
	__String* co_armCs = NULL;
	__String* co_qualityCs = NULL;
	string co_effectS = "";
	if (ptrActor->coat.isNormalGood())
	{
		int iGoodsId = ptrActor->coat.good_id;
		int iGoodsLevel = ptrActor->coat.GetRealLevel();
		co_iconCs = __String::createWithFormat("img/Items/%d.png", trData->m_mGoodsItem[iGoodsId]->iImgId);

		int quality = 0;
		if (ptrActor->coat.iQuality - 1 > 0){
			quality = trData->strengthen_consume[ptrActor->coat.iQuality - 1].quality;
		}
		co_armCs = __String::createWithFormat("q%d", quality);

		if (ptrActor->coat.iQuality > 0)
			co_qualityCs = __String::createWithFormat("new_ui/img_number/star_b_%d.png", ptrActor->coat.iQuality);
		co_effectS = trData->m_mGoodsItem[iGoodsId]->GetSpecialEffectName(iGoodsLevel);

		co_add->setVisible(false);
	}
	else
	{
		if (history->game_state == GAME_STATE_REDE)
			co_add->setVisible(true);
		else
			co_add->setVisible(false);
	}
	if (co_iconCs)
	{
		co_icon->setTexture(ITools::getInstance()->getTextureWithPath(co_iconCs->getCString()));
	}
	else
	{
		co_icon->setTexture(ITools::getInstance()->getTextureWithPath("img/Items/a2.png"));
	}
	if (co_armCs)
	{
		co_arm->getAnimation()->play(co_armCs->getCString());
	}
	else
		co_arm->getAnimation()->play("q0");

	if (co_qualityCs)
	{
		co_quality->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(co_qualityCs->getCString())));
		co_quality->setVisible(true);
	}
	else
		co_quality->setVisible(false);
	co_effect->setString(co_effectS.c_str());

	__String* as_iconCs = NULL;
	string as_effectS = "";
	if (ptrActor->assist_good.isNormalGood())
	{
		int iGoodsId = ptrActor->assist_good.good_id;
		as_iconCs = __String::createWithFormat("img/Items/%d.png", trData->m_mGoodsItem[iGoodsId]->iImgId);
		as_effectS = trData->m_mGoodsItem[iGoodsId]->GetSpecialEffectName(0);
		as_add->setVisible(false);
	}
	else
	{
		if (history->game_state == GAME_STATE_REDE)
			as_add->setVisible(true);
		else
			as_add->setVisible(false);
	}
	if (as_iconCs)
	{
		as_icon->setTexture(ITools::getInstance()->getTextureWithPath(as_iconCs->getCString()));
	}
	else
	{
		as_icon->setTexture(ITools::getInstance()->getTextureWithPath("img/Items/a3.png"));
	}
	as_effect->setString(as_effectS.c_str());

	string strState = "";
	for(map<int, ActorBuff*>::iterator it = ptrActor->m_mActorBuff.begin(); it != ptrActor->m_mActorBuff.end(); it++)
	{
		if (it->second->iCurRound > 0)
		{
			BuffElem* ptrBuff = trData->m_mDataBuff[it->first];
			strState += (ptrBuff->strName + " ");
		}
	}

	if(strState == "")
	{
		strState = LL("zhengchang");
	}
	debuff->setString(strState);

	//附魔灵石
	if (ptrActor->weapon.isNormalGood() && ptrActor->weapon.getStoneID())
	{
		spWeStone->setTexture(history->getGoodsTexture(ptrActor->weapon.getStoneID()));
		spWeStone->setVisible(true);
	}
	else
	{
		spWeStone->setVisible(false);
	}

	if(ptrActor->coat.isNormalGood() && ptrActor->coat.getStoneID())
	{
		spCoStone->setTexture(history->getGoodsTexture(ptrActor->coat.getStoneID()));
		spCoStone->setVisible(true);
	}
	else
	{
		spCoStone->setVisible(false);
	}

	if(ptrActor->assist_good.isNormalGood() && ptrActor->assist_good.getStoneID())
	{
		spAsStone->setTexture(history->getGoodsTexture(ptrActor->assist_good.getStoneID()));
		spAsStone->setVisible(true);
	}
	else
	{
		spAsStone->setVisible(false);
	}
}

void ActorInfoLayer::UpdateCurrentDate()
{
	bool ptbv = power_tbv->isVisible();
	bool stbv = skill_tbv->isVisible();
	bool otbv = occu_info->isVisible();
	UpdateActorDate(curActor);
	power_tbv->removeFromParentAndCleanup(true);
	skill_tbv->removeFromParentAndCleanup(true);
	occu_info->removeFromParentAndCleanup(true);

	power_tbv = ActorTableView::create(curActor, 1);
	power_tbv->setVisible(ptbv);
	left_bg->addChild(power_tbv);

	skill_tbv = SkillTableView::create(curActor, this);
	skill_tbv->setVisible(stbv);
	left_bg->addChild(skill_tbv);

	occu_info = ActorOccuInfoLayer::create(curActor);
	occu_info->setVisible(otbv);
	left_bg->addChild(occu_info);
}

void ActorInfoLayer::SetNextActor(bool is_next)
{
// 	HistoryActorPtr ptrNextActor = NULL;
// 	UpdateActorDate(ptrNextActor);
// 	power_tbv->UpdateTableView(ptrNextActor);

	HistoryActorPtr actor = NULL;
	if (is_next)
	{
		if (curActor->relation == 0)
		{
			if (history->game_state == GAME_STATE_REDE)
			{
				actor = GetSortActor(is_next);
			}
			else
				actor = history->getNextOwn(curActor);
		}
		else if (curActor->relation == 1)
			actor = history->getNextFriend(curActor);
		else if (curActor->relation == 2)
			actor = history->getNextEnemy(curActor);
		if (actor != NULL)
		{
			curActor = actor;
			UpdateCurrentDate();
		}
	}
	else
	{
		if (curActor->relation == 0)
		{
			if (history->game_state == GAME_STATE_REDE)
			{
				actor = GetSortActor(is_next);
			}
			else
				actor = history->getPreOwn(curActor);
		}
		else if (curActor->relation == 1)
			actor = history->getPreFriend(curActor);
		else if (curActor->relation == 2)
			actor = history->getPreEnemy(curActor);
		if (actor != NULL)
		{
			curActor = actor;
			UpdateCurrentDate();
		}
	}
}

void ActorInfoLayer::GetOwnActorList(HistoryActorPtr ptrCurActor)
{
	map<int, int> mActorTList;
	int iAListIndex = 0;
	for (int i = 0; i < history->ownArmyNum; i++)
	{
		mActorTList[iAListIndex++] = history->ownArmy[i] + 1;
	}

	int iActorId1 = 0, iActorId2 = 0,temp = 0;
	for (size_t i = 1; i < mActorTList.size(); i++)
	{
		for (size_t j = mActorTList.size() - 1; j > i; j--)
		{
			iActorId1 = mActorTList[j] - 1;
			iActorId2 = mActorTList[j - 1] - 1;
			if (history->actors[iActorId1].actor_quality > history->actors[iActorId2].actor_quality ||
				(history->actors[iActorId1].actor_quality == history->actors[iActorId2].actor_quality&&history->actors[iActorId1].GetActorLv() > history->actors[iActorId2].GetActorLv()))
			{
				temp = mActorTList[j];
				mActorTList[j] = mActorTList[j - 1];
				mActorTList[j - 1] = temp;
			}
		}
	}

	for (size_t i = 0; i < mActorTList.size(); i++)
	{
		if (mActorTList[i] - 1 == ptrCurActor->actor_id)
		{
			m_iOwnActorIndex = i;
			break;
		}
	}

	mOwnActorList = mActorTList;
}

HistoryActorPtr ActorInfoLayer::GetSortActor(bool bIsNext)
{
	if (bIsNext)
	{
		if (m_iOwnActorIndex == mOwnActorList.size()-1)
		{
			m_iOwnActorIndex = 0;
		}
		else
		{
			m_iOwnActorIndex++;
		}
	}
	else
	{
		if (m_iOwnActorIndex == 0)
		{
			m_iOwnActorIndex = mOwnActorList.size() - 1;
		}
		else
		{
			m_iOwnActorIndex--;
		}
	}

	return &history->actors[mOwnActorList[m_iOwnActorIndex] - 1];
}

void ActorInfoLayer::AddSkillInfoLayer(int iSkillId, int iSkillType)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	SkillInfoLayer* infoLayer = SkillInfoLayer::create(iSkillId, iSkillType, curActor,this);
	this->addChild(infoLayer);
}

void ActorInfoLayer::AddGoodsInfoLayer()
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (LongPress == 1)
	{
		if (history->game_state == GAME_STATE_REDE)
		{
			NetEquipLayer* ptrNel = NetEquipLayer::create(curActor, this, GOOD_TYPE_WEAPON);
			if (ptrNel)
				ChessApplication::getInstance()->AddFunctionLayer(ptrNel);
			else
				ITools::getInstance()->createGlobalPrompt(GetActorCanUseGoods(GOOD_TYPE_WEAPON)->getCString(), false);
		}
		else
			this->addChild(GoodsInfoLayer::create(&curActor->weapon, curActor));
	}
	else if (LongPress == 2)
	{
		if (history->game_state == GAME_STATE_REDE)
		{
			NetEquipLayer* ptrNel = NetEquipLayer::create(curActor, this, GOOD_TYPE_COAT);
			if (ptrNel)
				ChessApplication::getInstance()->AddFunctionLayer(ptrNel);
			else
				ITools::getInstance()->createGlobalPrompt(GetActorCanUseGoods(GOOD_TYPE_COAT)->getCString(), false);
		}
		else
			this->addChild(GoodsInfoLayer::create(&curActor->coat, curActor));
	}
	else if (LongPress == 3)
	{
		if (history->game_state == GAME_STATE_REDE)
		{
			NetEquipLayer* ptrNel = NetEquipLayer::create(curActor, this, GOOD_TYPE_ASSIST);
			if (ptrNel)
				ChessApplication::getInstance()->AddFunctionLayer(ptrNel);
			else
				ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("myshwjdzb"),LL("fuzhu"))->getCString(), false);
		}
		else
			this->addChild(GoodsInfoLayer::create(&curActor->assist_good, curActor));
	}
}

bool ActorInfoLayer::isSkilltbv()
{
	return bIsSkillTbv;
}
bool ActorInfoLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	LongPress = 0;
	Rect we_bgBox = we_bg->getBoundingBox();
	if (Rect(0, 0, we_bgBox.size.width, we_bgBox.size.height).containsPoint(we_bg->convertToNodeSpace(touch->getLocation()))&&(curActor->weapon.isNormalGood()||history->game_state==GAME_STATE_REDE))
	{
		LongPress = 1;
		AddGoodsInfoLayer();
		return true;
	}
	Rect co_bgBox = co_bg->getBoundingBox();
	if (Rect(0, 0, co_bgBox.size.width, co_bgBox.size.height).containsPoint(co_bg->convertToNodeSpace(touch->getLocation())) && (curActor->coat.isNormalGood() || history->game_state == GAME_STATE_REDE))
	{
		LongPress = 2;
		AddGoodsInfoLayer();
		return true;
	}
	Rect as_bgBox = as_bg->getBoundingBox();
	if (Rect(0, 0, as_bgBox.size.width, as_bgBox.size.height).containsPoint(as_bg->convertToNodeSpace(touch->getLocation())) && (curActor->assist_good.isNormalGood() || history->game_state == GAME_STATE_REDE))
	{
		LongPress = 3;
		AddGoodsInfoLayer();
		return true;
	}
	return true;
}

void ActorInfoLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}

void ActorInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}

void ActorInfoLayer::onExit()
{
	Layer::onExit();
}
void ActorInfoLayer::onEnter()
{
	Layer::onEnter();
}

ActorInfoLayer::ActorInfoLayer()
{
	
}

void ActorInfoLayer::SendNetDate()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	int iNetDateIndex = history->mChipSoulDate->GetSoulElemByActorId(curActor->actor_id+1)->id;
	__String* validateCs = __String::createWithFormat("id=%d&gameToken=%s", iNetDateIndex, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iNetDateIndex, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("addOrUpHero",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ActorInfoLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void ActorInfoLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if(value["result"].isObject())
		{
			Json::Value NetDate = value["result"];
			if(NetDate["gold"].isInt())
				history->money = NetDate["gold"].asInt();
			int iMinus = 0;
			if(NetDate["minus"].isInt())
				iMinus = NetDate["minus"].asInt();
			history->mChipSoulDate->ReduceActorSoulNumByActorId(curActor->actor_id + 1, iMinus);
			curActor->actor_quality++;
			ChessApplication::getInstance()->AddFunctionLayer(ActorJoinOwnLayer::create(curActor->actor_id + 1, FUNCTION_ASCENDSTAIRS));
			curActor->InitFiveAllAbility();
			UpdateCurrentDate();
			if(mDelegate)
				mDelegate->updateParentShow();
			updateMoneyShow();

			NetPromptTool::getInstance()->AddNetPrompt(PROMPT_WJJJ, curActor->actor_id + 1, curActor->actor_quality);
		}
	}
}

__String* ActorInfoLayer::GetActorCanUseGoods(int iGoodsType)
{
	string sCanUseWea = "";
	bool addDH = false;
	for (UInt j = 0; j < trData->m_mGoodsItemItid.size(); j++)
	{
		int iItid = trData->m_mGoodsItemItid[j];
		if (trData->occupation_good_info[curActor->occupation_id][iItid]&&trData->m_mItemType[iItid]->itpos == iGoodsType)//只有武器和防具
		{
			if (addDH)
				sCanUseWea += LL("dunhao");
			sCanUseWea += trData->m_mItemType[iItid]->itname;
			addDH = true;
		}
	}
	return __String::createWithFormat(LL("myshwjdzb"), sCanUseWea.c_str());
}

void ActorInfoLayer::updateParentShow()
{
	UpdateCurrentDate();
	if (curActor->actor_id == 0)
	{
		if(history->m_layTitleBar)
		{
			((TitleBarLayer*)history->m_layTitleBar)->UpdataFashionShow();
		}
	}
	if (mDelegate)
		mDelegate->updateParentShow();
}