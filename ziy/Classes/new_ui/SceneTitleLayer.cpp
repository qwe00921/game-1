#include "SceneTitleLayer.h"
#include "Defines.h"
#include "BattleLadder.h"
#include "PVPData.h"


bool SceneTitleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	if (history->m_iBattleType == BATTLE_PVP)
	{
		cur_num = NULL;
		battle_scene = NULL;
		enable_click = false;
		title_offset_x = 0;
		title_show_state = 1;
		set_pointx_num = 0;

		DSize size = ChessViewTool::getInstance()->getFullSize();

		title_bg = ReadSpriteName("new_ui/daily/stalker_namebg1.png");
		title_bg->setAnchorPoint(Vec2(0.5, 1));
		title_bg->setPosition(Vec2(size.width / 2, size.height));
		this->addChild(title_bg);


		Label* curNameLab = LabelSystemFont(history->m_pPlayerInfo->getName().c_str(), 16);
		curNameLab->setColor(Color3B::GREEN);
		curNameLab->setAnchorPoint(Vec2(1, 0.5));
		curNameLab->setPosition(Vec2(100, title_bg->getContentSize().height / 2 + 4));
		curNameLab->enableOutline(Color4B(0, 102, 0, 255), 1);
		title_bg->addChild(curNameLab);

		int cur_round_num = history->GetCurRound();
		if (cur_round_num <= 0) cur_round_num = 1;
		__String* num_txt = __String::createWithFormat("%d/%d", cur_round_num, history->GetMaxRound());
		cur_num = LabelSystemFont(num_txt->getCString(), 16);
		cur_num->setColor(Color3B(255, 255, 0));
		cur_num->setPosition(Vec2(title_bg->getContentSize().width / 2, title_bg->getContentSize().height / 2 + 4));
		cur_num->enableOutline(Color4B(173, 122, 29, 255), 1);
		title_bg->addChild(cur_num);

		//map<int, map<int, LadderElemData*> >* ptrLadderElemData = BattleLadder::getInstance()->GetLadderHeroData();

		//string OppoActorName = "";
		//if (ptrLadderElemData->count(history->m_iCurWarId) && !BattleLadder::getInstance()->isAllStar(history->m_iCurWarId))
		//{
		//	OppoActorName = ptrLadderElemData->at(history->m_iCurWarId)[0]->mName;
		//}
		//else
		//{
		//	OppoActorName = LL("qmxzr");
		//}

		int iSelectId = PVPData::getInstance()->getSelectRank();
		string strOppoName = PVPData::getInstance()->m_mPVPActor[iSelectId]->getName();
		Label* oppoNameLab = LabelSystemFont(strOppoName, 16);
		oppoNameLab->setColor(Color3B(0, 255, 255));
		oppoNameLab->setAnchorPoint(Vec2(0, 0.5));
		oppoNameLab->setPosition(Vec2(title_bg->getContentSize().width - 100, title_bg->getContentSize().height / 2 + 4));
		oppoNameLab->enableOutline(Color4B(0, 102, 102, 255), 1);
		title_bg->addChild(oppoNameLab);
	}
	else
	{
		cur_num = NULL;
		battle_scene = NULL;
		enable_click = true;
		title_offset_x = 0;
		title_show_state = 1;
		set_pointx_num = 0;

		title_bg = ReadSpriteName("new_ui/battle/ls_box.png");
		title_bg->setAnchorPoint(Vec2(0, 1));
		this->addChild(title_bg);
		int title_bg_width = title_bg->getContentSize().width;

		int iPointY = title_bg->getContentSize().height / 2;

		battle_scene = LabelSystemFont(history->battle_name, 16);
		battle_scene->setAnchorPoint(Vec2(0, 0.5));
		title_bg->addChild(battle_scene);
		int battle_scene_width = battle_scene->getContentSize().width;

		Label* huihe_txt = LabelSystemFont(LL("huihe"), 16);
		huihe_txt->setAnchorPoint(Vec2(0, 0.5));
		title_bg->addChild(huihe_txt);
		int huihe_txt_width = huihe_txt->getContentSize().width;


		int cur_round_num = history->GetCurRound();
		if (cur_round_num <= 0) cur_round_num = 1;
		__String* num_txt = __String::createWithFormat("%d/%d", cur_round_num, history->GetMaxRound());
		cur_num = LabelSystemFont(num_txt->getCString(), 16);
		cur_num->setAnchorPoint(Vec2(0, 0.5));
		title_bg->addChild(cur_num);
		int cur_num_width = cur_num->getContentSize().width;

		title_bg->setPosition(Vec2(-(title_bg_width - battle_scene_width - cur_num_width - huihe_txt_width - 50), screen_size_y - 4));
		battle_scene->setPosition(Vec2(title_bg_width - battle_scene_width - cur_num_width - huihe_txt_width - 40, iPointY));
		huihe_txt->setPosition(Vec2(title_bg_width - cur_num_width - huihe_txt_width - 28, iPointY));
		cur_num->setPosition(Vec2(title_bg_width - cur_num_width - 30, iPointY));

		fOffsetX = (battle_scene_width + huihe_txt_width + 17) / 40.0;

		this->schedule(schedule_selector(SceneTitleLayer::AutoUpdateTitleX), 0, 40, 8);
	}
	return true;
}

bool SceneTitleLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Rect BgBox = title_bg->getBoundingBox();
	if (enable_click&&Rect(0, 0, BgBox.size.width, BgBox.size.height).containsPoint(title_bg->convertToNodeSpace(pTouch->getLocation())))
	{
		enable_click = false;
		this->unschedule(schedule_selector(SceneTitleLayer::AutoUpdateTitleX));
		this->schedule(schedule_selector(SceneTitleLayer::updateTitleX),0,40,0);
		return true;
	}
	return false;
}

void SceneTitleLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void SceneTitleLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void SceneTitleLayer::UpdateRound()
{
	cur_num->setString(__String::createWithFormat("%d/%d",history->GetCurRound(), history->GetMaxRound())->getCString());
}

void SceneTitleLayer::updateTitleX(float f)
{
	set_pointx_num++;
	if (!title_show_state)
		title_bg->setPositionX(title_bg->getPositionX() + fOffsetX);
	else
		title_bg->setPositionX(title_bg->getPositionX() - fOffsetX);
	if (set_pointx_num == 41)
	{
		set_pointx_num = 0;
		enable_click = true;
		if (title_show_state)
			title_show_state = 0;
		else
		{
			this->schedule(schedule_selector(SceneTitleLayer::AutoUpdateTitleX), 0, 40, 5);
			title_show_state = 1;
		}
	}
}

void SceneTitleLayer::AutoUpdateTitleX(float f)
{
	enable_click = false;
	updateTitleX(f);
}

void SceneTitleLayer::onExit()
{
	Layer::onExit();
}

SceneTitleLayer::SceneTitleLayer()
{

}

SceneTitleLayer::~SceneTitleLayer()
{

}