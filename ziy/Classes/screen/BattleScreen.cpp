#include "Common.h"
#include "main.h"
#include "BattleControler.h"
#include "BattleScreen.h"
#include "ScriptHandler.h"
#include "BattleAI.h"
#include "BattleControler.h"
#include "MapElem.h"
#include "button.h"
#include "MiniMap.h"
#include "StatConfig.h"
#include "ActorInfoLayer.h"
#include "EquipmentUp.h"
#include "WinConditionTool.h"
#include "PopLayer.h"
#include "ShortcutFunctionLayer.h"
#include "BattleRoundStart.h"
void BattleScreen::paintBG(DGraphics* g)
{
	TRImagePtr bgImg;
	bgImg = history->imgBG;
	if(bgImg != NULL)
	{
		int bgScrX = history->scr_x,bgScrY = history->scr_y;
		int bgWidth = screen_size_x;
		int bgHeight = screen_size_y;

		if(history->scr_x<0)
		{
			bgScrX = 0;
		}
		if (history->scr_y < 0)
		{
			bgScrY = 0;
		}

		if(history->scr_x + screen_size_x > bgImg->width)
		{
			bgWidth = bgImg->width;
		}
		if (history->scr_y + screen_size_y > bgImg->height)
		{
			bgHeight = bgImg->height;
		}

		g->drawRegion(bgImg, bgScrX, bgScrY, bgWidth, bgHeight, 0, 0, 0);
	}
	else
	{
		g->setColor_RGB(0);
		g->fillRect(0, 0, screen_size_x, screen_size_y);
	}
}
void BattleScreen::paintMoveRegion(DGraphics* g, bool bOnlyMove)
{
	int attack_id = history->cur_actor->getAttackAreaId();
	int x, y;
	DList<MovePath>* moveList = BattleActor::moveList; 
	//if (moveList == NULL)return;
	if(history->cur_actor->relation == 0)
	{
		for(int i = 0; i < moveList->size(); i++)
		{
			MovePathPtr move = (MovePathPtr)moveList->elementAt(i);
			//g->fillTransRect_Blue((move->point.x*SCENE_TILE_WIDTH-history->scr_x),(move->point.y*SCENE_TILE_WIDTH - history->scr_y),SCENE_TILE_WIDTH,SCENE_TILE_WIDTH);
			g->drawImage(imgMoveRegion, (move->point.x*SCENE_TILE_WIDTH - history->scr_x), (move->point.y*SCENE_TILE_WIDTH - history->scr_y), 1);
		}
	}
	else
	{
		for(int i = 0; i < moveList->size(); i++)
		{
			MovePathPtr move = (MovePathPtr)moveList->elementAt(i);
			//g->fillTransRect_Green((move->point.x*SCENE_TILE_WIDTH - history->scr_x), (move->point.y*SCENE_TILE_WIDTH - history->scr_y), SCENE_TILE_WIDTH, SCENE_TILE_WIDTH);
			g->drawImage(imgMoveRegionFriend, (move->point.x*SCENE_TILE_WIDTH - history->scr_x), (move->point.y*SCENE_TILE_WIDTH - history->scr_y), 1);
		}
	}

	if (!bOnlyMove)
	{
		if (trData->hitArea[attack_id].length >= 0)
		{
			for (int i = 0; i < trData->hitArea[attack_id].length; i++)
			{
				x = (history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x)*SCENE_TILE_WIDTH - history->scr_x;
				y = (history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y)*SCENE_TILE_WIDTH - history->scr_y;
				g->drawImage(this->imgAttackArea, (x), (y), 1);
			}
		}
		else if (trData->hitArea[attack_id].length == -1)
		{
			for (int i = 0; i < pointNum; i++)
			{
				if (pointList[i].x != history->cur_actor->x || pointList[i].y != history->cur_actor->y)
				{
					x = pointList[i].x * SCENE_TILE_WIDTH - history->scr_x;
					y = pointList[i].y * SCENE_TILE_WIDTH - history->scr_y;
					g->drawImage(this->imgAttackArea, x, y, 1);
				}
			}
		}
	}

}
void BattleScreen::paintAttackRegion(DGraphics* g)
{
	int attack_id = history->cur_actor->getAttackAreaId();
	int i, x, y;
	SkillAttrPtr skill = handler.curSkill;
	if(handler.curGood != NULL)
	{
		for(i = 0; i < trData->hitArea[1].length; i++)
		{
			x = (history->cur_actor->x + trData->hitArea[1].pointlist[i].x)*SCENE_TILE_WIDTH - history->scr_x;
			y = (history->cur_actor->y + trData->hitArea[1].pointlist[i].y)*SCENE_TILE_WIDTH - history->scr_y;
			//g->fillTransRect_Red((x), (y), SCENE_TILE_WIDTH, SCENE_TILE_WIDTH);
			g->drawImage(imgAttackRegion, x, y, 1);
		}
		x = history->cur_actor->x*SCENE_TILE_WIDTH - history->scr_x;
		y = history->cur_actor->y*SCENE_TILE_WIDTH - history->scr_y;
		//g->fillTransRect_Red((x), (y), SCENE_TILE_WIDTH, SCENE_TILE_WIDTH);
		g->drawImage(imgAttackRegion, x, y, 1);
	}
	else if(skill != NULL)
	{
		if(trData->hitArea[skill->use_region].length >= 0)
		{
			for(i = 0; i < trData->hitArea[skill->use_region].length; i++)
			{
				x = (history->cur_actor->x + trData->hitArea[skill->use_region].pointlist[i].x)*SCENE_TILE_WIDTH - history->scr_x;
				y = (history->cur_actor->y + trData->hitArea[skill->use_region].pointlist[i].y)*SCENE_TILE_WIDTH - history->scr_y;
				//g->fillTransRect_Red((x), (y), SCENE_TILE_WIDTH, SCENE_TILE_WIDTH);
				g->drawImage(imgAttackRegion, x, y, 1);
			}
			if(skill->skill_object != 0)
			{
				x = history->cur_actor->x*SCENE_TILE_WIDTH - history->scr_x;
				y = history->cur_actor->y*SCENE_TILE_WIDTH - history->scr_y;
				//g->fillTransRect_Red((x), (y), SCENE_TILE_WIDTH, SCENE_TILE_WIDTH);
				g->drawImage(imgAttackRegion, x, y, 1);
			}
		}
		else if(trData->hitArea[skill->use_region].length == -1)
		{
			//g->fillTransRect_Red(0, 0, screen_size_x, screen_size_y);
			for(int i = 0; i < pointNum; i++)
			{
				if(pointList[i].x != history->cur_actor->x || pointList[i].y != history->cur_actor->y)
				{
					x = pointList[i].x * SCENE_TILE_WIDTH - history->scr_x;
					y = pointList[i].y * SCENE_TILE_WIDTH - history->scr_y;
					g->drawImage(this->imgAttackRegion, x, y, 1);
				}
			}
		}
	}
	else
	{
		if(trData->hitArea[attack_id].length >= 0)
		{
			for(i = 0; i < trData->hitArea[attack_id].length; i++)
			{
				x = (history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x)*SCENE_TILE_WIDTH - history->scr_x;
				y = (history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y)*SCENE_TILE_WIDTH - history->scr_y;
				//g->fillTransRect_Red((x), (y), SCENE_TILE_WIDTH, SCENE_TILE_WIDTH);
				g->drawImage(imgAttackRegion, x, y, 1);
			}
		}
		else  if(trData->hitArea[attack_id].length == -1)
		{
			//g->fillTransRect_Red(0, 0, screen_size_x, screen_size_y);

			for(int i = 0; i < pointNum; i++)
			{
				if(pointList[i].x != history->cur_actor->x || pointList[i].y != history->cur_actor->y)
				{
					x = pointList[i].x * SCENE_TILE_WIDTH - history->scr_x;
					y = pointList[i].y * SCENE_TILE_WIDTH - history->scr_y;
					g->drawImage(this->imgAttackRegion, x, y, 1);
				}
			}

		}
	}
}
void BattleScreen::paintActionGain(DGraphics* g)
{
	int i, x, y;
	SkillAttrPtr skill = handler.curSkill;
	if(skill)
	{
		if(trData->effectArea[skill->effect_region_type].length > 0)
		{
			for(i = 0; i < trData->effectArea[skill->effect_region_type].length; i++)
			{
				if(i == 0)
					continue;
				x = (this->point_x + trData->effectArea[skill->effect_region_type].pointlist[i].x)*SCENE_TILE_WIDTH - history->scr_x;
				y = (this->point_y + trData->effectArea[skill->effect_region_type].pointlist[i].y)*SCENE_TILE_WIDTH - history->scr_y;
				g->drawImage(this->imgEffectArea, (x), (y), 1);
			}
		}
		else if(trData->effectArea[skill->effect_region_type].length == -2)
		{
			for(int i = 0; i < pointNum; i++)
			{
				if(pointList[i].x != history->cur_actor->x || pointList[i].y != history->cur_actor->y)
				{
					x = pointList[i].x * SCENE_TILE_WIDTH - history->scr_x;
					y = pointList[i].y * SCENE_TILE_WIDTH - history->scr_y;
					g->drawImage(this->imgEffectArea, x, y, 1);
				}
			}
		}
		else if(trData->effectArea[skill->effect_region_type].length == -13)
		{
			ActionSingleGainPtr gain = handler.getSingleGain(point_x, point_y, 0);
			if (gain)
			{
				map<int, DPoint> mPoint = gain->m_mEffectArea;
				for(UInt i = 0; i < mPoint.size(); i++)
				{
					x = mPoint[i].x * SCENE_TILE_WIDTH - history->scr_x;
					y = mPoint[i].y * SCENE_TILE_WIDTH - history->scr_y;
					g->drawImage(this->imgEffectArea, x, y, 1);
				}
			}
		}
	}
	else if(history->cur_actor->good_hit_area&&trData->effectArea[history->cur_actor->good_hit_area].length > 0)
	{
		for(i = 0; i < trData->effectArea[history->cur_actor->good_hit_area].length; i++)
		{
 			if(i == 0)
 				continue;
			x = (this->point_x + trData->effectArea[history->cur_actor->good_hit_area].pointlist[i].x)*SCENE_TILE_WIDTH - history->scr_x;
			y = (this->point_y + trData->effectArea[history->cur_actor->good_hit_area].pointlist[i].y)*SCENE_TILE_WIDTH - history->scr_y;
			g->drawImage(this->imgEffectArea, x, y, 1);
		}
	}
}
void BattleScreen::paintActor(DGraphics* g)
{
	int i;
	BattleActorPtr actor;
	for(i = 0; i < handler.actor_list->size(); i++)
	{
		actor = (BattleActorPtr)handler.actor_list->elementAt(i);
		actor->paint(g, handler.battle_state, handler.curSkill);
	}
}

void BattleScreen::paintBossIcon(DGraphics* g)
{
	if (history->m_mBossActor.size())
	{
		HistoryActorPtr ptrEActor = NULL;
		for (map<int, int>::iterator mIt = history->m_mBossActor.begin(); mIt != history->m_mBossActor.end(); mIt++)
		{
			ptrEActor = &history->actors[mIt->first - 1];
			if (!ptrEActor->hide)
			{
				g->drawImage(this->imgBossIcon, ptrEActor->xx() - history->scr_x + 24 - imgBossIcon->width/2, ptrEActor->yy() - history->scr_y - imgBossIcon->height);
			}
		}
	}
}

void BattleScreen::showActorInfo()
{
	ChessApplication::getInstance()->AddFunctionLayer(ActorInfoLayer::create(history->cur_actor, BATTLE_STATE_INFO, this));
}

void BattleScreen::paintMapElem(DGraphics* g)
{
	int i;
	for(i = 0; i < history->elem_list->size(); i++)
	{
		MapElemPtr elem = (MapElemPtr)history->elem_list->elementAt(i);
		elem->paint(g);
	}
}
void BattleScreen::onPaint(DGraphics* g)
{
	if(history->is_paint_bust&&!isPaintBust)
		PainBustsetbuttonvisible(0);
	else if(!history->is_paint_bust&&isPaintBust)
		PainBustsetbuttonvisible(1);
	if(handler.battle_state == BATTLE_STATE_ENDING_LOSE)
	{
		return;
	}
	else if(!history->initilized || handler.battle_state == BATTLE_STATE_ENDING_WIN || handler.battle_state == BATTLE_STATE_BATTLE_END)
	{
		Screen::onPaint(g);
		return;
	}

	paintBG(g);
	paintMapElem(g);
	if(history->need_show_land)
	{
		if(need_show_land && enable_show_land && point_y < history->map_height && point_x < history->map_width)
		{
			m_spLandInfo->updateSprite(point_x, point_y);
			need_show_land = 0;
		}
	}
	else
	{
		need_show_land = 0;
	}
	if(handler.battle_state == BATTLE_STATE_SHOW_MOVE_REGION||handler.battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION)
		paintMoveRegion(g);
	if(handler.battle_state == BATTLE_STATE_SHOW_ATTACK_REGION)
		paintAttackRegion(g);
	if(handler.battle_state == BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION)
		paintMoveRegion(g,true);
	if(handler.battle_state == BATTLE_STATE_WAIT_COMMAND ||
		handler.battle_state == BATTLE_STATE_SHOW_MOVE_REGION ||
		handler.battle_state == BATTLE_STATE_SHOW_ATTACK_REGION ||
		handler.battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION || 
		handler.battle_state == BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION)
	{
		g->drawImage(imgPoint, SCENE_TILE_WIDTH*this->point_x - history->scr_x, SCENE_TILE_WIDTH*this->point_y - history->scr_y, 1);
	}
	if(handler.battle_state == BATTLE_STATE_SHOW_ACTION_GAIN)
	{
		g->drawImage(imgAttackRegion, (handler.ai.mostGain->hitPoint.x*SCENE_TILE_WIDTH - history->scr_x), (handler.ai.mostGain->hitPoint.y*SCENE_TILE_WIDTH - history->scr_y), 1);
	}
	if(highlight_frame >= 0)
	{
		if(highlight_frame == 0 && curPointActor)
		{
			curLandForm = &trData->lands[history->map_array[(rect_highlight.y) / SCENE_TILE_WIDTH][(rect_highlight.x) / SCENE_TILE_WIDTH]];
			m_spActorInfo->updateSprite(curPointActor, curLandForm);
		}
		highlight_frame++;
		if(highlight_frame % (2 * TIMER_HIGH_LIGHT_FRAME) < TIMER_HIGH_LIGHT_FRAME)
			g->drawTileImage(imgAttackRegion, 0, 0, imgAttackRegion->width, imgAttackRegion->height, rect_highlight.x - history->scr_x, rect_highlight.y - history->scr_y, rect_highlight.width, rect_highlight.height);
		if(highlight_frame == 6 * TIMER_HIGH_LIGHT_FRAME - 1)
		{
			m_spActorInfo->setVisible(false);
			highlight_frame = -1;
		}

	}
	paintActor(g);
	paintBossIcon(g);
	if(handler.battle_state == BATTLE_STATE_SHOW_ACTION_GAIN || handler.battle_state == BATTLE_STATE_SHOW_ATTACK_REGION)
		paintActionGain(g);

	if(buttonMiniMap && mini_map)
	{
		buttonMiniMap->rect.x = mini_map->rect.x - img_mini_map->width + 1;
	}
	paintTranslucentImg(g);
	Screen::onPaint(g);
}

void BattleScreen::handleTimer()
{
	Screen::handleTimer();
	if((highlight_frame >= 0) || bonus != NULL || mFunctionLayer != NULL ||  m_spActGain->isVisible() || m_spLabel->isVisible() || m_pTalkLayer->isVisible() || m_pChoiceBox || m_armBuff)
	{
		return;
	}

	bool bEnabled = false;
	if(history->curActionSide == ACTOR_RELATION_OWN && history->m_bWaitCommand)
	{
		bEnabled = true;
	}
	m_pBatBarLayer->SetMenuEnabled(m_pBatBarLayer->m_pFunc, bEnabled);

	handler.Ontimer();
}
void BattleScreen::nextRound()
{
	m_iFinishCurRoundReason = 2;
	enable_show_land = 0;
	mFunctionLayer = ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_JSZD, 0, 0);
	Director::getInstance()->getRunningScene()->addChild(mFunctionLayer, 98);
}
void BattleScreen::RunFunction(int iNomal)
{
	mFunctionLayer = NULL;
	enable_show_land = 1;
	need_show_land = 0;
	m_spLandInfo->setVisible(false);
	if(iNomal)
	{
		if(m_iFinishCurRoundReason == 2)
		{
			handler.RoundStartFriendNew();
			m_iFinishCurRoundReason = -1;
		}
		else
		{
			handler.RoundStartFriend();
			WinConditionTool::getInstance()->BattleRoundEndSetCon();
		}
	}
}

void BattleScreen::updateRTDate(int iNomal)
{
	if(iNomal == 1)
	{
		handler.battle_state = BATTLE_STATE_EXIT;
		resetWindow();
	}
}

void BattleScreen::ChildCloseLayer(int iNomal)
{
	this->mFunctionLayer = NULL;
}

void BattleScreen::ResetBattleScreenState()
{
	if (mShieldScreen)
	{
		removeScreen(mShieldScreen, 1);
		mShieldScreen = NULL;
	}
}

void BattleScreen::AddBattleScreenShield()
{
	if (mShieldScreen)
	{
		removeScreen(mShieldScreen, 1);
	}
	mShieldScreen = new Screen(SCREEN_GUIDE);
	appendScreen(mShieldScreen);
}

void BattleScreen::updateWeather()
{
	if (history->m_iCurAir)
	{
		string strInfo = StringUtils::format("weather%d", history->m_iCurAir);
		m_armWeather->getAnimation()->play(strInfo);
	}
	m_armWeather->setVisible(history->m_iCurAir != 0);
}

void BattleScreen::updateWeatherScale(float fScale)
{
	m_armWeather->getScheduler()->setTimeScale(fScale);
}

int __lock;
void BattleScreen::autopointClicked(int x, int y)
{
	__lock = 1;
	pointerClicked(x, y);
	__lock = 0;
}
int BattleScreen::pointerClicked(int x, int y)
{
	if(bNotTriggerClick)
	{
		bNotTriggerClick = false;
		return 1;
	}
	if((this->handler.auto_run == 1 || history->curActionSide != 0) && __lock == 0)
		return 0;

	point_x = (x + history->scr_x) / SCENE_TILE_WIDTH;
	if(point_x < 0)
	{
		point_x = 0;
	}
	else if(point_x >= history->map_width)
	{
		point_x = history->map_width - 1;
	}
	point_y = (y + history->scr_y) / SCENE_TILE_WIDTH;
	if(point_y < 0)
	{
		point_y = 0;
	}
	else if(point_y >= history->map_height)
	{
		point_y = history->map_height - 1;
	}

	curPointActor = history->FindActorPosition(point_x, point_y);




	switch(handler.battle_state)
	{
	case BATTLE_STATE_WAIT_COMMAND:
	{
		if(curPointActor)
		{
			history->cur_actor = curPointActor;
			handler.ShowMoveRegion();
			handler.setActorLastXAndY(history->cur_actor->x, history->cur_actor->y);
			ptrSelectAction->setActionButton();
			if(handler.auto_run == 0)
				playSound(SOUND_LEFT_MOUSE_CLICK);
		}
		else if(enable_show_land)
		{
			need_show_land = 1;
			history->CheckPos(point_x, point_y);
			playSound(SOUND_LEFT_MOUSE_CLICK);
			//显示地面情况
		}
	}
		break;
	case BATTLE_STATE_SHOW_MOVE_REGION:
	{
		if(curPointActor&&curPointActor != history->cur_actor)
		{
			handler.CancelMoveRegion();
			history->cur_actor = curPointActor;
			handler.ShowMoveRegion();
			if(handler.auto_run == 0)
				playSound(SOUND_LEFT_MOUSE_CLICK);
		}
		else if(history->cur_actor->relation == 1)
		{
			if(history->curActionSide != 1)
			{

				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("zheshiyoujunbudui"));
				}
			}
			else
			{
				handler.MoveActor(handler.ai.mostGain->moveIndex);
			}
		}
		else if(history->cur_actor->relation == 2/*&&handler.auto_run==1*/)//敌军移动
		{
			if(history->curActionSide != 2)
			{

				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("zheshidijunbudui"));
				}

			}
			else
			{
				handler.MoveActor(handler.ai.mostGain->moveIndex);
			}
		}
		else if(history->cur_actor->relation == 0)
		{
			if(history->cur_actor->action_finished == 1)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("xingdongyijingjiesu"));
				}


			}
			else if(history->cur_actor->state_puzzle != 0)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("hunluan"));
				}
			}
			else if(history->cur_actor->state_obfuscation != 0)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("hunmi"));
				}
			}
			else
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						handler.MoveActor(i);
						canmove = 1;
						playSound(SOUND_LEFT_MOUSE_CLICK);
						break;
					}
				}
				if(canmove)
				{
				}
				else
				{
					RpointerPressed(0, screen_size_y - 1);
				}
			}
		}
	}
		break;
	case BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION:
	{
		if (curPointActor&&curPointActor != history->cur_actor)
		{
			handler.CancelMoveRegion();
			handler.battle_state = BATTLE_STATE_RUN_SCRIPT;
			handler.handler.SetScriptList(1);
// 			history->cur_actor = curPointActor;
// 			handler.ShowMoveRegion();
// 			if (handler.auto_run == 0)
// 				playSound(SOUND_LEFT_MOUSE_CLICK);
		}
		else
		{
			int canmove = 0;
			MovePathPtr move;
			for (int i = 0; i < BattleActor::moveList->size(); i++)
			{
				move = BattleActor::moveList->elementAt(i);
				if (move->point.x == point_x&&move->point.y == point_y)
				{
					handler.DispatchActorMove(i);
					canmove = 1;
					playSound(SOUND_LEFT_MOUSE_CLICK);
					break;
				}
			}
			if (canmove)
			{
			}
			else
			{
				RpointerPressed(0, screen_size_y - 1);
			}
		}
	}
	break;
	case BATTLE_STATE_SHOW_ATTACK_MOVE_REGION:
	{
		if(curPointActor&&curPointActor != history->cur_actor)
		{
			ptrSelectAction->setVisible(false);
			ActionSingleGainPtr gain = handler.getSingleGain(point_x, point_y, 0);
			if(curPointActor->relation == 2 && gain != NULL&&history->cur_actor->relation == 0 && !history->cur_actor->action_finished&&
				!history->cur_actor->state_puzzle&&!history->cur_actor->state_obfuscation&&history->cur_actor->actor_only_move!=2)
			{
				handler.getSingleGain(point_x, point_y, 1);
				handler.ActorDoAction();
				playSound(SOUND_LEFT_MOUSE_CLICK);
			}
			else
			{
				handler.ai.ClearGainList();
				history->cur_actor = curPointActor;
				handler.ShowMoveRegion();
				handler.setActorLastXAndY(history->cur_actor->x, history->cur_actor->y);
				ptrSelectAction->setActionButton();
				if(handler.auto_run == 0)
					playSound(SOUND_LEFT_MOUSE_CLICK);
			}
		}
		else if(history->cur_actor->relation == 1)
		{
			if(history->curActionSide != 1)
			{

				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("zheshiyoujunbudui"));
				}
			}
			else
			{
				handler.MoveActor(handler.ai.mostGain->moveIndex);
			}
		}
		else if(history->cur_actor->relation == 2/*&&handler.auto_run==1*/)//敌军移动
		{
			if(history->curActionSide != 2)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("zheshidijunbudui"));
				}
			}
			else
			{
				handler.MoveActor(handler.ai.mostGain->moveIndex);
			}
		}
		else if(history->cur_actor->relation == 0)
		{
			ptrSelectAction->setVisible(false);
			if(history->cur_actor->action_finished == 1)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("xingdongyijingjiesu"));
				}
			}
			else if(history->cur_actor->state_puzzle != 0)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("hunluan"));
				}
			}
			else if(history->cur_actor->state_obfuscation != 0)
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						canmove = 1;
						break;
					}
				}
				RpointerPressed(0, screen_size_y - 1);
				if(canmove)
				{
					updateNotice(LL("hunmi"));
				}
			}
			else
			{
				int canmove = 0;
				MovePathPtr move;
				for(int i = 0; i < BattleActor::moveList->size(); i++)
				{
					move = BattleActor::moveList->elementAt(i);
					if(move->point.x == point_x&&move->point.y == point_y)
					{
						handler.MoveActor(i);
						canmove = 1;
						playSound(SOUND_LEFT_MOUSE_CLICK);
						break;
					}
				}
				if(canmove)
				{
				}
				else
				{
					RpointerPressed(0, screen_size_y - 1);
				}
			}
		}
	}
		break;
	case BATTLE_STATE_SHOW_ATTACK_REGION:
	{
		ActionSingleGainPtr gain = handler.getSingleGain(point_x, point_y, 0);
		string strNotice = "";

		if(gain != NULL)
		{
			if(gain->actor_relive)//点击空地释放
			{
				if(!history->FindActor(gain->actor_relive_id)->MapisMoveable(point_x, point_y))
				{
					strNotice = LL("cidiwufashiyong");
				}
			}
// 			else if (gain->actor_tower_def)//点击空地释放
// 			{
// 				if (!history->actors[gain->actor_tower_def+899].MapisMoveable(point_x, point_y))
// 				{
// 					strNotice = LL("cidiwufashiyong");
// 				}
// 			}
			else if(gain->land_strict)
			{
				strNotice = LL("cidiwufashiyong");
			}
			else if(gain->enemy_skipper_strict)
			{
				strNotice = LL("wufaduizhujiangshiyong");
			}
			else
			{
				ptrSelectAction->setVisible(false);
				handler.getSingleGain(point_x, point_y, 1);
				handler.ActorDoAction();
				playSound(SOUND_LEFT_MOUSE_CLICK);
			}
		}
		else
		{
			int in_area = 0;
			SkillAttrPtr skill = handler.curSkill;
			int attack_id = history->cur_actor->getAttackAreaId();
			if(handler.curGood != NULL)
			{
				for(int i = 0; i < trData->hitArea[1].length; i++)
				{
					if(point_x == history->cur_actor->x + trData->hitArea[1].pointlist[i].x &&point_y == history->cur_actor->y + trData->hitArea[1].pointlist[i].y)
					{
						if(history->FindOppoActorPosition(history->cur_actor, point_x, point_y) != NULL)
						{
							strNotice = LL("ciwupinbunengyongyudifangbudui");
							in_area = 1;
						}
					}
				}
				if(in_area == 0)
				{
					RpointerPressed(0, screen_size_y - 1);
					if(history->FindActorPosition(point_x, point_y) != NULL)
					{
						strNotice = LL("buzaishiyongfanweinei");
					}
				}
			}
			else if(skill != NULL)
			{
				if(trData->hitArea[skill->use_region].length >= 0)
				{
					for(int i = 0; i < trData->hitArea[skill->use_region].length; i++)
					{
						if(point_x == history->cur_actor->x + trData->hitArea[skill->use_region].pointlist[i].x
							&&point_y == history->cur_actor->y + trData->hitArea[skill->use_region].pointlist[i].y)
						{
							if(skill->use_region != 0 && history->FindOppoActorPosition(history->cur_actor, point_x, point_y) != NULL)
							{
								strNotice = LL("cijinengbunengyongyudifangbudui");
								in_area = 1;
							}
							else if (skill->skill_object == 5 && history->FindOwnActorPosition(history->cur_actor, point_x, point_y) != NULL)
							{
								strNotice = LL("cijinengbunengyongyuziji");
								in_area = 1;
							}
							else if (history->FindOwnActorPosition(history->cur_actor, point_x, point_y) != NULL)
							{
								strNotice = LL("cijinengbunengyongyujifangbudui");
								in_area = 1;
							}
						}
					}
				}
				if (in_area == 0 && skill->skill_object == 5)
				{
					HistoryActorPtr ptrSkillActor = history->FindActorPosition(point_x, point_y);
					if (ptrSkillActor&&ptrSkillActor->actor_id == history->cur_actor->actor_id)
						strNotice = LL("cijinengbunengyongyuziji");
					else
					{
						RpointerPressed(0, screen_size_y - 1);
						if (ptrSkillActor != NULL)
						{
							strNotice = LL("buzaishiyongfanweinei");
						}
					}
				}
				else if(in_area == 0)
				{
					RpointerPressed(0, screen_size_y - 1);
					if(history->FindActorPosition(point_x, point_y) != NULL)
					{
						strNotice = LL("buzaishiyongfanweinei");
					}
				}
			}
			else if(trData->hitArea[attack_id].length >= 0)
			{
				for(int i = 0; i < trData->hitArea[attack_id].length; i++)
				{
					if(history->FindOwnActorPosition(history->cur_actor, point_x, point_y) != NULL)
					{
						strNotice = LL("bunenggongjijifangbudui");
						in_area = 1;
					}
				}
				if(in_area == 0)
				{
					RpointerPressed(0, screen_size_y - 1);
					if(history->FindActorPosition(point_x, point_y) != NULL)
					{
						strNotice = LL("buzaigongjifanweinei");
					}
				}
			}
		}
		if(strNotice != "")
		{
			updateNotice(strNotice);
		}
	}
		break;
	case BATTLE_STATE_SHOW_ACTION_GAIN:
	{
		handler.ActorDoAction();
		break;
	}
	}
	return 1;
}
int BattleScreen::pointerPressed(int x, int y)
{
	if(bNotTriggerClick)
	{
		return 1;
	}

	m_spLandInfo->setVisible(false);
	if (m_spLabel->isClickHide() && m_spLabel->isVisible())
	{
		m_spLabel->setVisible(false);
	}

	pointerDragged(x, y);

	if(handler.auto_run == 0)
	{
		if(Screen::pointerPressed(x, y))
			return 1;
	}
	last_x = x;
	last_y = y;
	return 1;
}

int BattleScreen::RpointerPressed(int x, int y)
{
	CCLOG("===========RpointerPressed============");
	handler.ClearGain();
	
	if(Screen::RpointerPressed(x, y) || history->curActionSide != ACTOR_RELATION_OWN)
		return 1;

	auto Guide = Director::getInstance()->getRunningScene()->getChildByName("GuideLayer");
	if (Guide!=NULL)
	{
		return 1;
	}

	curPointActor = history->FindActorPosition(point_x, point_y);
	switch(handler.battle_state)
	{
	case BATTLE_STATE_WAIT_COMMAND:
	{
		history->cur_actor = curPointActor;
		if(history->cur_actor)
		{
			AddBattleScreenShield();
			showActorInfo();			
			playSound(SOUND_LEFT_MOUSE_CLICK);
		}
		else
		{
			nextRound();
			playSound(SOUND_LEFT_MOUSE_CLICK);
		}
	}
		break;
	case BATTLE_STATE_SHOW_MOVE_REGION:
	{
		handler.CancelMoveRegion();
		//playSound(SOUND_LEFT_MOUSE_CLICK);
		playSound(SOUND_RIGHT_MOUSE_CLICK);
		history->cur_actor = curPointActor;
		if(curPointActor)
		{
			ptrSelectAction->setVisible(false);
			AddBattleScreenShield();
			showActorInfo();
			
		}
	}
		break;
	case BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION:
	{
		handler.CancelMoveRegion();
		playSound(SOUND_RIGHT_MOUSE_CLICK);
		handler.battle_state = BATTLE_STATE_RUN_SCRIPT;
		handler.handler.SetScriptList(1);
		history->cur_actor = curPointActor;
		if (curPointActor)
		{
			ptrSelectAction->setVisible(false);
			AddBattleScreenShield();
			showActorInfo();
		}
	}
	break;
	case BATTLE_STATE_SHOW_ATTACK_MOVE_REGION:
	{
		handler.CancelMoveRegion();
		handler.ai.ClearGainList();
		handler.curSkill = NULL;
		handler.curGood = NULL;
		playSound(SOUND_RIGHT_MOUSE_CLICK);
		history->cur_actor = curPointActor;
		ptrSelectAction->setVisible(false);
		if(curPointActor)
		{
			AddBattleScreenShield();
			showActorInfo();
		}
	}
		break;
	case BATTLE_STATE_SHOW_ATTACK_REGION:
	{
		int enable_back = 0;
		if(handler.curSkill != NULL)
			enable_back = 1;
		else if(handler.curGood != NULL)
			enable_back = 2;
		handler.ai.ClearGainList();
		handler.curSkill = NULL;
		handler.curGood = NULL;
		if(handler.ai.skill_double_hit != 1)
		{
			if(!enable_back)
				handler.SelectAction(4);
			else
			{
				handler.ClearGain();
				if(history->cur_actor->actor_only_move != 2)
				{
					handler.ai.GenerateAttackGainList(BattleActor::move_index);
					handler.SelectAction(enable_back);
				}
			}
			if(!enable_back)
				ptrSelectAction->setVisible(true);
			playSound(SOUND_RIGHT_MOUSE_CLICK);
			//handler.battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
			if(handler.ai.skill_double_hit >= 2)
				handler.ai.skill_double_hit = 0;
		}
		else
		{
			handler.battle_state = BATTLE_STATE_DO_ACTION;
		}
	}
		break;
	}
	return 1;
}
int BattleScreen::pointerPressDragged(int x, int y)
{
	auto Guide = Director::getInstance()->getRunningScene()->getChildByName("GuideLayer");
	if (Guide != NULL)
	{
		return 0;
	}

	if(handler.battle_state == BATTLE_STATE_WAIT_COMMAND
		|| handler.battle_state == BATTLE_STATE_SHOW_MOVE_REGION
		|| handler.battle_state == BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION
		|| handler.battle_state == BATTLE_STATE_SHOW_ATTACK_REGION
		|| handler.battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION)
	{
		history->scr_x += last_x - x;
		history->scr_y += last_y - y;
		if(history->scr_x < 0)
			history->scr_x = 0;
		if(history->scr_y < 0)
			history->scr_y = 0;

		if(history->scr_x >= history->map_width*SCENE_TILE_WIDTH - screen_size_x)
			history->scr_x = history->map_width*SCENE_TILE_WIDTH - screen_size_x - 1;

		if(history->scr_y >= history->map_height*SCENE_TILE_WIDTH - screen_size_y)
			history->scr_y = history->map_height*SCENE_TILE_WIDTH - screen_size_y - 1;
		last_x = x;
		last_y = y;
		return 1;
	}
	return 0;
}
int BattleScreen::pointerDragged(int x, int y)
{
	if(Screen::pointerDragged(x, y) || m_spLandInfo->isVisible() || m_spLabel->isVisible())
		return 1;
	x = (x + history->scr_x) / SCENE_TILE_WIDTH;
	y = (y + history->scr_y) / SCENE_TILE_WIDTH;
	if(x == point_x&&y == point_y)
		return 1;
	if(handler.battle_state == BATTLE_STATE_WAIT_COMMAND
		|| handler.battle_state == BATTLE_STATE_SHOW_MOVE_REGION
		|| handler.battle_state == BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION
		|| handler.battle_state == BATTLE_STATE_SHOW_ATTACK_REGION
		|| handler.battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION)
	{
		point_x = x;
		point_y = y;
		if (m_spActorInfo->isVisible())
		{
			m_spActorInfo->setVisible(false);
		}
		curPointActor = history->FindActorPosition(point_x, point_y);
		if(curPointActor)
		{
			curLandForm = &trData->lands[history->map_array[point_y][point_x]];
			if(handler.battle_state == BATTLE_STATE_WAIT_COMMAND || handler.battle_state == BATTLE_STATE_SHOW_MOVE_REGION || handler.battle_state == BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION)
			{
				m_spActorInfo->updateSprite(curPointActor, curLandForm);
			}
			else if(handler.battle_state == BATTLE_STATE_SHOW_ATTACK_REGION || handler.battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION)
			{
				if(!handler.auto_run)
				{
					ActionSingleGainPtr ptrGain = handler.getSingleGain(point_x, point_y, 0);
					if(!ptrGain)
					{
						m_spActorInfo->updateSprite(curPointActor, curLandForm);
					}
				}
			}
		}
	}
	return 1;
}

void BattleScreen::onMessage(MessagePtr message)
{
	switch(message->msgId)
	{
	case CTRL_BONUS + MSG_BONUS_RELEASED:
	{
		removeControl(bonus);
		bonus = NULL;
	}
		break;
	case CTRL_MINI_MAP_BUTTON + MSG_BUTTON_RELEASED:
		mini_map->changeState();
		break;
	case CTRL_MINI_MAP + MSG_MINI_MAP_PRESSED:
		break;
	case SCR_TEST_ASK + MSG_COMFORM_YES:
		chess->testAsk = 1;
		break;
	}
}
void BattleScreen::resetWindow()
{
	ChessViewTool::getInstance()->setScreenDefaultSize();
	history->initilized = 0;
}

BattleScreen::BattleScreen() : Screen(SCREEN_BATTLE)
{
	this->imgBG = NULL;
	this->point_x = 0;//3;
	this->point_y = 0;//10;
    this->m_iFinishCurRoundReason = -1;
	this->imgPoint = CreateGlobalImage("Mark/14");
	this->imgAttackArea = CreateGlobalImageFile("ui/hongbiankuang.png");
	this->imgEffectArea = CreateGlobalImage("Mark/13");
	this->img_mini_map = CreateGlobalImage("ui/MiniMap");
	this->imgAttackRegion = CreateGlobalImageFile("ui/honggezi.png");
	this->imgMoveRegion = CreateGlobalImageFile("ui/langezi.png");
	this->imgMoveRegionFriend = CreateGlobalImageFile("ui/lvgezi.png");
	this->imgBossIcon = CreateGlobalImageFile("new_ui/battle/war_boss.png");

	if (trData->net_map_clear.count(history->m_iCurWarId))
	{
		for (int i = 0; i < (int)trData->net_map_clear[history->m_iCurWarId].size(); i++)
		{
			string sImgName = trData->net_map_clear[history->m_iCurWarId][i]->clear_img;
			string sImgPath = StringUtils::format("img/clear/%s.png",sImgName.c_str());
			m_mTranslucentImg[sImgName] = CreateImageFile(sImgPath.c_str());
		}
	}

	this->mini_map = NULL;
	this->curPointActor = NULL;
	this->bonus = NULL;
	this->mFunctionLayer = NULL;
	this->handler.scr = this;
	this->highlight_frame = -1;
	this->randHint = -1;
	history->need_show_land = 1;
	need_show_land = 0;
	this->enable_show_land = 1;
	this->buttonMiniMap = NULL;
	battle_sec = 1;
	pointList = NULL;
	pointNum = 0;
	isPaintBust = 0;
	bNotTriggerClick = false;
	history->m_bWaitCommand = false;
	history->m_bAutoRun = false;
	history->m_bTakeTurns = false;
	m_pChoiceBox = NULL;
	m_armBuff = NULL;
	mShieldScreen = NULL;
	m_armWeather = nullptr;

	//Battle层
	m_pBattleLayer = BattleLayer::create();
	ChessApplication::getInstance()->AddFunctionLayer(m_pBattleLayer);

	//武将行动选择
	this->ptrSelectAction = SelectActionLayer::create(&handler);
	m_pBattleLayer->addChild(ptrSelectAction, 1);

	//菜单栏
	m_pBatBarLayer = MenuBarLayer::create(this);
	m_pBattleLayer->addChild(m_pBatBarLayer, 2);
	m_pBatBarLayer->m_pMain->setVisible(false);
	m_pBatBarLayer->m_pFunc->setVisible(false);

	//武将信息
	m_spActorInfo = ActorInfoSprite::create();
	m_pBattleLayer->addChild(m_spActorInfo, 1);
	m_spActorInfo->setVisible(false);

	//行动增益
	m_spActGain = ActionGainSprite::create();
	m_pBattleLayer->addChild(m_spActGain, 1);
	m_spActGain->setVisible(false);

	//地形信息
	m_spLandInfo = LandInfoSprite::create();
	m_pBattleLayer->addChild(m_spLandInfo, 1);
	m_spLandInfo->setVisible(false);

	//提示标签
	m_spLabel = LabelSprite::create();
	m_pBattleLayer->addChild(m_spLabel, 2);
	m_spLabel->setVisible(false);

	//对话标签
	m_pTalkLayer = TalkLayer::create();
	m_pBattleLayer->addChild(m_pTalkLayer, 3);
	m_pTalkLayer->setVisible(false);

	//天气动画
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/weather.ExportJson", 0, 1));
	m_armWeather = Armature::create("weather");
	m_armWeather->getAnimation()->play("weather1");
	m_armWeather->setPosition(point);
	m_pBattleLayer->addChild(m_armWeather);
	m_armWeather->setVisible(false);
}
void BattleScreen::initControl()
{
	Json::Value callStr;
	callStr["name"] = "keepScreenOn";
	callStr["keep"] = true;
	cppBridgeCall(callStr.toStyledString().c_str());

	DRect rect;
	mini_map = new MiniMap(CTRL_MINI_MAP);
	appendControl(mini_map);

	ptrSceneTitle = SceneTitleLayer::create();
	ChessApplication::getInstance()->GetActionLayer()->addChild(ptrSceneTitle);

	rect.x = mini_map->rect.x - img_mini_map->width + 1;
	rect.y = 0;
	rect.width = img_mini_map->width;
	rect.height = img_mini_map->height;
	buttonMiniMap = new Button(&rect, NULL, CTRL_MINI_MAP_BUTTON, img_mini_map, NULL);
	buttonMiniMap->setDrawButtonRect(false);
	appendControl(buttonMiniMap);

	m_pBatBarLayer->m_pMain->setVisible(true);

	InitFullScreenPoint();
}

BattleScreen::~BattleScreen()
{
	SAFE_DELETE_IMAGE(this->imgPoint);
	SAFE_DELETE_IMAGE(this->imgAttackArea);
	SAFE_DELETE_IMAGE(this->imgEffectArea);
	SAFE_DELETE_IMAGE(this->img_mini_map);
	SAFE_DELETE_IMAGE(this->imgAttackRegion);
	SAFE_DELETE_IMAGE(this->imgMoveRegion);
	SAFE_DELETE_IMAGE(this->imgMoveRegionFriend);
	SAFE_DELETE_IMAGE(this->imgBossIcon);
	SAFE_DELETE_ARRAY(pointList);

	for (map<string, TRImagePtr>::iterator NMIt = m_mTranslucentImg.begin(); NMIt != m_mTranslucentImg.end(); NMIt++)
	{
		SAFE_DELETE_IMAGE(NMIt->second);
	}

	ptrSceneTitle->removeFromParent();
	history->m_mBossActor.clear();
}

void BattleScreen::onAppend()
{
	handler.onInit();
}

void BattleScreen::onRemove()
{
	resetWindow();
	history->clearSceneData();
	handler.auto_run = 0;
	handler.onDestroy();
	ptrSelectAction->removeFromParent();
	m_pBattleLayer->removeFromParent();
}

void BattleScreen::InitFullScreenPoint()
{
	pointNum = history->map_width * history->map_height;
	pointList = new DPoint[pointNum];

	int index = 0;
	for(int i = 0; i < history->map_width; i++)
	{
		for(int j = 0; j < history->map_height; j++)
		{
			pointList[index].x = i;
			pointList[index].y = j;
			index++;
		}
	}
}
void BattleScreen::PainBustsetbuttonvisible(int Visible)
{
	mini_map->visible = Visible;
	buttonMiniMap->visible = Visible;
	if(isPaintBust == 0)
		isPaintBust = 1;
	else if(isPaintBust == 1)
		isPaintBust = 0;
}

void BattleScreen::updateNotice(string strNotice)
{
	m_spLabel->updateSprite(strNotice, LABEL_SHORT, 0.8f);
	playSound(SOUND_MOUSE_CLICK_ERROR);
}

void BattleScreen::setMiniMapVisible(bool bVisible)
{
	mini_map->visible = bVisible;
	buttonMiniMap->visible = bVisible;
}


void BattleScreen::paintTranslucentImg(DGraphics* g)
{
	if (trData->net_map_clear.count(history->m_iCurWarId))
	{
		NetMapClear* ptrNMC = NULL;
		for (int i = 0; i < (int)trData->net_map_clear[history->m_iCurWarId].size(); i++)
		{
			ptrNMC = trData->net_map_clear[history->m_iCurWarId][i];
			g->drawImage(m_mTranslucentImg[ptrNMC->clear_img], ptrNMC->position[0] * SCENE_TILE_WIDTH - history->scr_x, ptrNMC->position[1] * SCENE_TILE_WIDTH - history->scr_y);
		}
	}
}

void BattleScreen::SetHighLightArea()
{
	setLayerVisible(true);
	if (history->is_victory_area)
	{
		curPointActor = NULL;
		highlight_frame = 0;
		int x1 = history->victory_x1;
		int y1 = history->victory_y1;
		int x2 = history->victory_x2;
		int y2 = history->victory_y2;
		rect_highlight.x = x1*SCENE_TILE_WIDTH;
		rect_highlight.y = y1*SCENE_TILE_WIDTH;
		rect_highlight.width = (x2 - x1 + 1)*SCENE_TILE_WIDTH;
		rect_highlight.height = (y2 - y1 + 1)*SCENE_TILE_WIDTH;
		history->CheckPos(x1, y1);
		history->CheckPos(x2, y2);
	}
}

void BattleScreen::addArmBuff(HistoryActor* ptrActor, int iBuffid)
{
	BuffElem* ptrBuff = trData->m_mDataBuff[iBuffid];
	ActorBuff* pActBuff = ptrActor->m_mActorBuff[iBuffid];
	string strAction = "";
	int iSound = 0;
	if (pActBuff->iArmIndex == 1)
	{
		strAction = ptrBuff->strAnima1;
		iSound = ptrBuff->iSound + 100;
	}
	else if(pActBuff->iArmIndex == 2)
	{
		strAction = ptrBuff->strAnima2;
		iSound = SOUND_UNNORMAL_CLEAR;
	}
	else
	{
		return;
	}

	playSound(iSound);
	string strPath = StringUtils::format("buff_effect/%s.ExportJson", strAction.c_str());
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath(strPath.c_str(), 0, 1));

	history->CheckPosActor(ptrActor);
	int iPosX = ptrActor->xx() - history->scr_x + SCENE_TILE_WIDTH / 2;
	int iPosY = ptrActor->yy() - history->scr_y + SCENE_TILE_WIDTH / 2;
	m_armBuff = Armature::create(strAction);
	m_armBuff->setPosition(Vec2(iPosX, screen_size_y - iPosY));
	m_armBuff->getAnimation()->play(strAction);
	m_pBattleLayer->addChild(m_armBuff);
	m_armBuff->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if(type == COMPLETE || type == LOOP_COMPLETE)
		{
			armature->removeFromParent();
			pActBuff->iArmIndex = 0;
			m_armBuff = nullptr;
			ptrActor->updateBuffDisplay(m_pBattleLayer);
			if(m_spActorInfo->isVisible())
			{
				m_spActorInfo->updateBuff(ptrActor);
			}
		}
	});
}
