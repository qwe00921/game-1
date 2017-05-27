#include "GetStarLayer.h"
#include "Defines.h"
#include "WinConditionTool.h"
#include "BattleScreen.h"

GetStarLayer* GetStarLayer::create(int iIndex, int is_battle)
{
	GetStarLayer* pRet = new GetStarLayer();
	pRet->mIndex = iIndex;
	pRet->m_isButtle = is_battle;
	if (pRet&&pRet->init())
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

bool GetStarLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
	DSize size = ChessViewTool::getInstance()->getFullSize();
	
	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/battle/battle_sbg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	__String* tsS = __String::createWithFormat(LL("rwwcxj"), ptrNetmw->mStarLimit[0], ptrNetmw->mStarLimit[1], ptrNetmw->mStarLimit[2]);
	Label* ts = LabelSystemFont(tsS->getCString(), 16);
	ts->setPosition(Vec2(bg->getContentSize().width/2,45));
	ts->setColor(Color3B::YELLOW);
	bg->addChild(ts);

	//35,304
	Sprite* ptrChechBoxT = ReadSpriteName("new_ui/battle/box_no.png");
	int imgboxheight = ptrChechBoxT->getContentSize().height;
	int imgboxwidth = ptrChechBoxT->getContentSize().width;
	map<int, int> win_result;
	if (m_isButtle)
		win_result = WinConditionTool::getInstance()->ReachConditionResult(m_isButtle==1?false:true);
	else
		imgboxwidth = imgboxwidth / 2;
	int iCellNum = 0;
	int iCellHeight = 20;
	for (size_t i = 0; i < history->net_win_condition.size(); i++)
	{
		WinConditionPtr ptrWinC = &trData->win_condition[history->net_win_condition[i]];
		int iPointY = 304 - (imgboxheight + 2)*i - iCellNum*iCellHeight;
		if (m_isButtle)
		{
			string box_path = "new_ui/battle/box_no.png";
			if (win_result[history->net_win_condition[i]] == 1)
				box_path = "new_ui/battle/box_ok.png";
			Sprite* ptrChechBox = ReadSpriteName(box_path.c_str());
			ptrChechBox->setAnchorPoint(Vec2(0, 1));
			ptrChechBox->setPosition(Vec2(32, iPointY));
			bg->addChild(ptrChechBox);
		}

		Label* ptrWinInfo = LabelSystemFont(ptrWinC->describe.c_str(),16);
		int iLabCellWidth = ceil(ptrWinInfo->getContentSize().width / 230.0);
		ptrWinInfo->setDimensions(230, iLabCellWidth*iCellHeight);
		ptrWinInfo->setHorizontalAlignment(TextHAlignment::LEFT);
		ptrWinInfo->setAnchorPoint(Vec2(0, 1));
		ptrWinInfo->setPosition(Vec2(42 + imgboxwidth, iPointY-9));
		ptrWinInfo->setColor(Color3B(120,0,0));
		ptrWinInfo->setLineHeight(iCellHeight);
		bg->addChild(ptrWinInfo);
		iCellNum += iLabCellWidth - 1;
	}


	return true;
}

bool GetStarLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_isButtle == 2)
		history->is_battle_win = false;
	return true;
}
void GetStarLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void GetStarLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (history->game_state == GAME_STATE_BATTLE)
	{
		BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		if (scr&&scr->highlight_frame<0)
			scr->SetHighLightArea();
	}
	this->removeFromParent();
}

GetStarLayer::GetStarLayer()
{

}

void GetStarLayer::onExit()
{
	Layer::onExit();
}

GetStarLayer::~GetStarLayer()
{

}