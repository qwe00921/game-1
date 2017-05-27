#include "BattleTitleLayer.h"
#include "Defines.h"
#include "BattleLadder.h"

bool BattleTitleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();
	Sprite* bg = ReadSpriteName("new_ui/bg/bg1.jpg");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	string titleS = getCurrentSenceTitle();

 	int LabFontSize = 50;
// 	if (history->m_iBattleType == BATTLE_LADDER)
// 	{
// 		map<int, map<int, LadderElemData*> >* ptrLadderElemData = BattleLadder::getInstance()->GetLadderHeroData();
// 		string OppoActorName = "";
// 		if (ptrLadderElemData->count(history->m_iCurWarId) && !BattleLadder::getInstance()->isAllStar(history->m_iCurWarId))
// 		{
// 			OppoActorName = ptrLadderElemData->at(history->m_iCurWarId)[0]->mName;
// 		}
// 		else
// 		{
// 			OppoActorName = LL("qmxzr");
// 		}
// 
// 		titleS = __String::createWithFormat("%s VS %s", history->m_pPlayerInfo->getName().c_str(), OppoActorName.c_str())->getCString();
// 		LabFontSize = 40;
// 	}

	Label* b_title = LabelSystemFont(titleS.c_str(), LabFontSize);
	b_title->setAnchorPoint(Vec2(0.5, 0));
	b_title->setPosition(Vec2(bg->getContentSize().width/2,320));
	bg->addChild(b_title);

	int randHint = rand() % trData->hint_num;
	Label* b_hint = LabelSystemFont(trData->scene_hint[randHint], 18);
	b_hint->setAnchorPoint(Vec2(0.5, 0));
	b_hint->setPosition(Vec2(bg->getContentSize().width / 2, 91));
	bg->addChild(b_hint);

	return true;
}

BattleTitleLayer::BattleTitleLayer()
{

}

BattleTitleLayer::~BattleTitleLayer()
{

}