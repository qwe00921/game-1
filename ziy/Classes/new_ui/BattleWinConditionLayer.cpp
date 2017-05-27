#include "BattleWinConditionLayer.h"
#include "Defines.h"

BattleWinConditionLayer* BattleWinConditionLayer::create(BattleScreenPtr ptrBs,bool bOpenSchedule)
{
	BattleWinConditionLayer* pRet = new BattleWinConditionLayer();
	if (pRet&&pRet->init(ptrBs, bOpenSchedule))
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

bool BattleWinConditionLayer::init(BattleScreenPtr ptrBs, bool bOpenSchedule)
{
	if (!Layer::init())
	{
		return false;
	}
	m_pBattleScreen = ptrBs;
	m_bOpenSchedule = bOpenSchedule;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_bg = LayerColor::create(Color4B(0,0,0,100));
	this->addChild(black_bg);

	Sprite* bg = ReadSpriteName("new_ui/battle/warround_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	for (size_t i = 0; i<m_pBattleScreen->WinStrList.size(); i++)
	{
		Label* win_str = LabelSystemFont(m_pBattleScreen->WinStrList[i].c_str(), 13);
		win_str->setAnchorPoint(Vec2(0,1));
		win_str->setPosition(Vec2(59,214-i*15+15));
		win_str->setColor(Color3B(102,0,0));
		bg->addChild(win_str);
	}

	for (size_t i = 0; i< m_pBattleScreen->LostStrList.size(); i++)
	{
		Label* lost_str = LabelSystemFont(m_pBattleScreen->LostStrList[i].c_str(), 13);
		lost_str->setAnchorPoint(Vec2(0, 1));
		lost_str->setPosition(Vec2(59, 134 - i * 15+15));
		lost_str->setColor(Color3B(102, 0, 0));
		bg->addChild(lost_str);
	}

	int iMaxRound = history->GetMaxRound();
	if (history->m_iBattleType == BATTLE_SIXWAR)
	{
        iMaxRound = trData->m_mNetWarInfo[history->m_iCurWarId]->iRound;		
	}

	Label* max_round = LabelSystemFont(IntToString(iMaxRound), 13);
	max_round->setAnchorPoint(Vec2(0,1));
	max_round->setPosition(Vec2(59,68));
	max_round->setColor(Color3B(102, 0, 0));
	bg->addChild(max_round);

	if (bOpenSchedule)
		this->scheduleOnce(schedule_selector(BattleWinConditionLayer::RemoveThis),4);
	return true;
}

void BattleWinConditionLayer::RemoveThis(float f)
{
	m_pBattleScreen->mFunctionLayer = NULL;
	this->removeFromParent();
}

bool BattleWinConditionLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void BattleWinConditionLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bOpenSchedule&&history->game_state == GAME_STATE_BATTLE)
	{
		BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		if (scr&&scr->highlight_frame < 0)
			scr->SetHighLightArea();
	}
	RemoveThis(0);
}

void BattleWinConditionLayer::onExit()
{
	Layer::onExit();
}

BattleWinConditionLayer::BattleWinConditionLayer()
{

}

BattleWinConditionLayer::~BattleWinConditionLayer()
{

}