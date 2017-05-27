#include "Common.h"
#include "main.h"
#include "BattleActor.h"
#include "BattleAi.h"
#include "BattleScreen.h"
#include "ActionLayer.h"

#define TIMER_ACTOR_CHANGE_FASHION 60

int BattleActor::normal_frame = 0;
int BattleActor::move_end_direction = -1;
int BattleActor::move_x = 0;
int BattleActor::move_y = 0;
int BattleActor::move_step = 0;
int BattleActor::move_index = 0;
int BattleActor::PIXEL_PER_FRAME = SCENE_TILE_WIDTH/TIMER_ACTOR_MOVE;
DList<MovePath>* BattleActor::moveList = NULL;
int	BattleActor::is_default_move = 0;
TRImagePtr BattleActor::goodImg = NULL;

BattleActor::BattleActor(HistoryActorPtr actor)
{
	hisActor = actor;
	frame = 0;
	m_need_frame = 0;
	hurt_hp = 0;
	hurt_mp =0;
	hurt_money = 0;
	unnormal_state = 0;
	hp_gain = 0;
	mp_gain = 0;
	cur_probability = -1;
	m_bIsDeally = false;
	m_bSkillMiss = false;
	this->imgBlue = CreateGlobalImageFile("new_ui/battle/hp_blue.png");
	this->imgGreen = CreateGlobalImageFile("new_ui/battle/hp_green.png");
	this->imgOrange = CreateGlobalImageFile("new_ui/battle/hp_orange.png");
	this->imgRed = CreateGlobalImageFile("new_ui/battle/hp_red_lost.png");
	this->imgYellow = CreateGlobalImageFile("new_ui/battle/hp_yellow_treat.png");
	this->imgAttack = CreateGlobalImageFile("new_ui/battle/attack.png");

	BattleScreen* ptrScreen = (BattleScreen*)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
	m_labPercent = LabelSystemFont("", 12);
	m_labPercent->setAnchorPoint(Vec2(1.0f, 1.0f));
	m_labPercent->enableOutline(Color4B::BLACK, 1);
	m_labPercent->setVisible(false);
	ptrScreen->m_pBattleLayer->addChild(m_labPercent);
	m_spGain = NULL;
}
void BattleActor::paintUnnormalState(DGraphics* g, int battle_state, SkillAttrPtr curSkill)
{
	int iCurPercent = -1; //命中百分比
	Vec2 vecPos = Vec2(0, 0);

	if (battle_state == BATTLE_STATE_SHOW_ATTACK_REGION && curSkill)
	{
		if (cur_probability != -1)
		{
			iCurPercent = cur_probability;
			vecPos = Vec2(hisActor->xx() - history->scr_x + SCENE_TILE_WIDTH - 4, hisActor->yy() - history->scr_y + this->imgRed->height);
		}
	}
	
	if (battle_state == BATTLE_STATE_SHOW_ATTACK_REGION || battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION)
	{
		if (hp_gain > 0)
		{
			int iCurHp = hisActor->GetCurHp();
			int iBaseHp = hisActor->GetBaseHp();
			int iGoodsAddHp = hisActor->GetGoodsAddHp();
			if (iCurHp + hp_gain > (iBaseHp + iGoodsAddHp))
			{
				hp_gain = iBaseHp + iGoodsAddHp - iCurHp;
			}
			int Hp_len = iCurHp * 40 / (iBaseHp + iGoodsAddHp);
			int Hp_Num1 = Hp_len / 2;
			int Hp_Num2 = Hp_len % 2;
			TRImagePtr img;
			if (hisActor->relation == 0)//我军
			{
				img = this->imgGreen;
			}
			else if (hisActor->relation == 1)//友军
			{
				img = this->imgOrange;
			}
			else //敌军
			{
				img = this->imgBlue;
			}
			for (int i = 0; i < Hp_Num1; i++)
			{
				g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * i, (hisActor->yy() - history->scr_y), 0);
			}
			if (Hp_Num2)
			{
				g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * Hp_Num1, (hisActor->yy() - history->scr_y), 0);
			}
			Hp_len = (hp_gain)* 40 / (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp());
			int Hp_Num3 = Hp_len / 2;
			int Hp_Num4 = Hp_len % 2;
			for (int i = 0; i < Hp_Num3; i++)
			{
				g->drawImage(imgYellow, ((hisActor->xx() - history->scr_x + 4) + 2 * (Hp_Num1 + Hp_Num2)) + 2 * i, (hisActor->yy() - history->scr_y), 0);
			}
			if (Hp_Num4)
			{
				g->drawImage(imgYellow, ((hisActor->xx() - history->scr_x + 4) + 2 * (Hp_Num1 + Hp_Num2)) + 2 * Hp_Num3, (hisActor->yy() - history->scr_y), 0);
			}

		}
		else if (hp_gain < 0)
		{
			int iCurHp = hisActor->GetCurHp();
			if (iCurHp + hp_gain < 0)
			{
				hp_gain = -iCurHp;
			}

			int Hp_len = (iCurHp + hp_gain) * 40 / (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp());
			int Hp_Num1 = Hp_len / 2;
			int Hp_Num2 = Hp_len % 2;
			TRImagePtr img;
			if (hisActor->relation == 0)//我军
			{
				img = this->imgGreen;
			}
			else if (hisActor->relation == 1)//友军
			{
				img = this->imgOrange;
			}
			else //敌军
			{
				img = this->imgBlue;
			}
			for (int i = 0; i < Hp_Num1; i++)
			{
				g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * i, (hisActor->yy() - history->scr_y), 0);
			}
			if (Hp_Num2)
			{
				g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * Hp_Num1, (hisActor->yy() - history->scr_y), 0);
			}
			Hp_len = (-hp_gain) * 40 / (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp());
			int Hp_Num3 = Hp_len / 2;
			int Hp_Num4 = Hp_len % 2;
			for (int i = 0; i < Hp_Num3; i++)
			{
				g->drawImage(imgRed, ((hisActor->xx() - history->scr_x + 4) + 2 * (Hp_Num1 + Hp_Num2)) + 2 * i, (hisActor->yy() - history->scr_y), 0);
			}
			if (Hp_Num4)
			{
				g->drawImage(imgRed, ((hisActor->xx() - history->scr_x + 4) + 2 * (Hp_Num1 + Hp_Num2)) + 2 * Hp_Num3, (hisActor->yy() - history->scr_y), 0);
			}

			if (hisActor->relation == 2&&BattleControler::auto_run == 0)
			{
				int iImgAttack = normal_frame % 120;
				if (iImgAttack < 60)
					iImgAttack = iImgAttack / 12;
				else
					iImgAttack = 9 - iImgAttack / 12;
				g->drawImage(imgAttack, (hisActor->xx() - history->scr_x + 8), (hisActor->yy() - history->scr_y + 8 + iImgAttack), 0);
			}

			if(cur_probability != -1)
			{
				iCurPercent = cur_probability;
				vecPos = Vec2(hisActor->xx() - history->scr_x + SCENE_TILE_WIDTH - 4, hisActor->yy() - history->scr_y + this->imgRed->height);
			}
		}
		else
		{
			if (history->setting_flag[2])//是否全屏显示血条
			{
				int Hp_len = this->hisActor->GetCurHp() * 40 / (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp());
				int Hp_Num1 = Hp_len / 2;
				int Hp_Num2 = Hp_len % 2;
				TRImagePtr img;
				if (hisActor->relation == 0)//我军
				{
					img = this->imgGreen;
				}
				else if (hisActor->relation == 1)//友军
				{
					img = this->imgOrange;
				}
				else //敌军
				{
					img = this->imgBlue;
				}
				for (int i = 0; i < Hp_Num1; i++)
				{
					g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * i, (hisActor->yy() - history->scr_y), 0);
				}
				if (Hp_Num2)
				{
					g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * Hp_Num1, (hisActor->yy() - history->scr_y), 0);
				}
			}
		}
	}else if (history->setting_flag[2] != 0)//是否全屏显示血条
	{
		int Hp_len = this->hisActor->GetCurHp() * 40 / (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp());
		int Hp_Num1 = Hp_len / 2;
		int Hp_Num2 = Hp_len % 2;
		TRImagePtr img;
		if (hisActor->relation == 0)//我军
		{
			img = this->imgGreen;
		}
		else if (hisActor->relation == 1)//友军
		{
			img = this->imgOrange;
		}
		else //敌军
		{
			img = this->imgBlue;
		}
		for (int i = 0; i < Hp_Num1; i++)
		{
			g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * i, (hisActor->yy() - history->scr_y), 0);
		}
		if (Hp_Num2)
		{
			g->drawImage(img, (hisActor->xx() - history->scr_x + 4) + 2 * Hp_Num1, (hisActor->yy() - history->scr_y), 0);
		}
	}
	
	if (iCurPercent != -1)
	{
		m_labPercent->setString(StringUtils::format("%d%%", iCurPercent));
		m_labPercent->setPosition(Vec2(vecPos.x, screen_size_y - vecPos.y));
		m_labPercent->setVisible(true);
	}
	else
	{
		m_labPercent->setVisible(false);
	}
}

//"平原","草原","树林","荒地","山地","岩山","山崖","雪原","桥梁","浅滩",
//"沼泽","池塘","小河","大河","栅栏","城墙","城内","城门","城池","关隘",
//"鹿砦","村庄","兵营","民居","宝库","火池","火堆","船只","障碍","地下",
void BattleActor::paintLand(DGraphics* g)
{
	int y = hisActor->y;
	int x = hisActor->x;

	if (y >= history->map_height)
		y = history->map_height - 1;
	if (x >= history->map_width)
		x = history->map_width - 1;
	if (y < 0)
		y = 0;
	if (x < 0)
		x = 0;

	LandformAttrPtr curLandForm = &trData->lands[history->map_array[y][x]];
	switch(curLandForm->landform_id)
	{ 
	case 1://草原
		g->bg_img = history->imgLandGrass;
		break;
	case 2://树林
		g->bg_img = history->imgLandForest;
		break;
	case 7://雪原
		g->bg_img = history->imgLandSnow;
		break;
	case 9://浅滩
	case 10:
		g->bg_img = history->imgLandSwamp;
		break;
	case 13://大河
		{
			if (hisActor->style != ACTOR_STATE_MOVE && !hisActor->good_effects[33]&&hisActor->occupation_id != 19)
			{
				switch (hisActor->direction)
				{
				case 0:
					g->bg_img = history->imgLandRiver_ahead[(frame/10) % 2];
					g->drawImage(history->imgRiver_ahead[(frame / 10) % 2], (hisActor->xx() - history->scr_x),
						(hisActor->yy() - history->scr_y), 2);
					break;
				case 1:
					g->bg_img = history->imgLandRiver_left[(frame / 10) % 2];
					g->drawImage(history->imgRiver_left[(frame / 10) % 2], (hisActor->xx() - history->scr_x),
						(hisActor->yy() - history->scr_y), 2);
					break;
				case 2:
					g->bg_img = history->imgLandRiver_back[(frame / 10) % 2];
					g->drawImage(history->imgRiver_back[(frame / 10) % 2], (hisActor->xx() - history->scr_x),
						(hisActor->yy() - history->scr_y), 2);
					break;
				case 3:
					g->bg_img = history->imgLandRiver_left[(frame / 10) % 2];
					g->drawImage(history->imgRiver_left[(frame / 10) % 2], (hisActor->xx() - history->scr_x),
						(hisActor->yy() - history->scr_y), 2);
					break;
				}
			}
			
		}
		break;
	default:
		g->bg_img = NULL;
		break;
	}
}
void BattleActor::paintSpecialEffect(DGraphics* g)
{
	if (hisActor->paint_special_effect[0] == 1)
	{
		int iCenterX = hisActor->xx() - history->scr_x+24;
		int iCenterY = hisActor->yy() - history->scr_y+24;
		if (hisActor->style == ACTOR_STATE_DEADLY_ATTACK_PREPARE)
		{
			if (frame > 15)
			{
				g->drawImage(history->imgDoubleSeid[frame / 3 - 5], iCenterX - history->imgDoubleSeid[frame / 3 - 5]->width / 2, iCenterY - history->imgDoubleSeid[frame / 3 - 5]->height / 2);
				g->drawImage(history->imgDoubleSeid[frame / 3 + 3], iCenterX - history->imgDoubleSeid[frame / 3 +3]->width / 2, iCenterY - history->imgDoubleSeid[frame / 3 +3]->height / 2);
			}
		}
		else if (hisActor->style == ACTOR_STATE_ATTACK_PREPARE_SOUND)
		{
			g->drawImage(history->imgDoubleSeid[frame / 3], iCenterX - history->imgDoubleSeid[frame / 3]->width / 2, iCenterY - history->imgDoubleSeid[frame / 3]->height / 2);
			g->drawImage(history->imgDoubleSeid[frame / 3 + 8], iCenterX - history->imgDoubleSeid[frame / 3 +8]->width / 2, iCenterY - history->imgDoubleSeid[frame / 3 +8]->height / 2);
		}
		else if (hisActor->style == ACTOR_STATE_NORMAL_ATTACK)
		{
			if (frame<9)
				g->drawImage(history->imgDoubleSeid[frame / 3 + 5], iCenterX - history->imgDoubleSeid[frame / 3+5]->width / 2, iCenterY - history->imgDoubleSeid[frame / 3+5]->height / 2);
			if (frame<3)
				g->drawImage(history->imgDoubleSeid[frame / 3 + 13], iCenterX - history->imgDoubleSeid[frame / 3+13]->width / 2, iCenterY - history->imgDoubleSeid[frame / 3+13]->height / 2);

			if (frame == 10)
				hisActor->paint_special_effect[0] = -1;
		}
	}
}
void BattleActor::paint(DGraphics* g, int battle_state, SkillAttrPtr curSkill)
{
	if(hisActor->hide==1)
		return;
	int image_inner_index = 0;
	int image_index = history->getBattleActorImageId(hisActor->actor_id);
	int image_transform = TRANS_WITH_BG_IMG;
	int image_width = SCENE_TILE_WIDTH;
	int image_height = SCENE_TILE_WIDTH;
	TRImagePtr image = NULL;
	frame++;
	
	if(hisActor->history_image_fight_id <= 100)
	{
		if(hisActor->image_change_occu_id<0)
		{
			if (hisActor->controlled_by_enemy)
				image_index += 2;
			else 
				image_index += hisActor->relation;
		}
		else if(hisActor->image_change_occu_id>0 && hisActor->image_change_occu_id <= 7)
		{
			image_index += hisActor->image_change_occu_id - 1;
		}
	}

	paintLand(g);
	paintSpecialEffect(g);
// 	if (hisActor->actor_id == 0&&!hisActor->mFashion[0] )
// 	{
// 		hisActor->mFashion[0] = 1;
// 		hisActor->mFashion[1] = 1;
// 		hisActor->mFashion[2] = 2;
// 		hisActor->mFashion[3] = 100;
// 		hisActor->mFashion[4] = 2;
// 	}
	if (hisActor->mFashion[0])
	{
		paintFashion(g, battle_state, curSkill);
	}
	else
	{
	switch(hisActor->style)
	{
	case ACTOR_STATE_NORMAL:
		{
			image = history->imgMov[image_index];
			if(hisActor->action_finished)
			{
				if (hisActor->GetCurHp() == 0 || (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp()) / hisActor->GetCurHp()>5)
				{
					image_inner_index = 9;
				}
				else
				{
					switch(hisActor->direction)
					{
					case 0:
						image_inner_index = 7;
						break;
					case 1:
						image_inner_index = 8;
						image_transform |= TRANS_MIRROR;
						break;
					case 2:
						image_inner_index = 6;
						break;
					case 3:
						image_inner_index = 8;
						break;
					}
				}
			}
			else if((normal_frame%(TIMER_ACTOR_CHANGE_STYLE_FRAME*2) < TIMER_ACTOR_CHANGE_STYLE_FRAME))
				//&&(BattleScreen.battle_state==BATTLE_STATE_WAIT_COMMAND))
			{
				if (hisActor->relation == 1)
				{
					hisActor->relation = 1;
				}
				
				if (hisActor->GetCurHp() == 0 || (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp()) / hisActor->GetCurHp()>5)
				{
					image_inner_index = 9;
				}
				else
				{
					switch(hisActor->direction)
					{
					case 0:
						image_inner_index = 2;
						break;
					case 1:
						image_inner_index = 4;
						image_transform |= TRANS_MIRROR;
						break;
					case 2:
						image_inner_index = 0;
						break;
					case 3:
						image_inner_index = 4;
						break;
					}
				}
			}
			else
			{
				if (hisActor->relation == 1)
				{
					hisActor->relation = 1;
				}
				if (hisActor->GetCurHp() == 0 || (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp()) / hisActor->GetCurHp()>5)
				{
					image_inner_index = 10;
				}
				else
				{
					switch(hisActor->direction)
					{
					case 0:
						image_inner_index = 3;
						break;
					case 1:
						image_inner_index = 5;
						image_transform |= TRANS_MIRROR;
						break;
					case 2:
						image_inner_index = 1;
						break;
					case 3:
						image_inner_index = 5;
						break;
					}
				}
			}
			
			if(hisActor->action_finished)
			{
				image_transform |= TRANS_FINISH;
			}
			else if(unnormal_state&&hisActor->state_poison)
			{
				image_transform |= TRANS_POISON;
			}
			g->drawRegion(image,	0,image_height*image_inner_index,
				image_width,image_height,image_transform,
				(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(normal_frame%(2*TIMER_ACTOR_CHANGE_STYLE_FRAME) >= TIMER_ACTOR_CHANGE_STYLE_FRAME/2&&
				normal_frame%(2*TIMER_ACTOR_CHANGE_STYLE_FRAME) <= TIMER_ACTOR_CHANGE_STYLE_FRAME*3/2)
			{
				unnormal_state = true;
			}
			else
			{
				unnormal_state = false;
			}
			paintUnnormalState(g, battle_state, curSkill);
		}
		break;
	case ACTOR_STATE_CELEBRATE:
		image = history->imgSpc[image_index];
		image_inner_index = 4;
		g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
			image_transform,(hisActor->xx() - history->scr_x),
			(hisActor->yy() - history->scr_y));
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		if (frame == 100)
		{
			setState(ACTOR_STATE_NORMAL);
		}
		break;
	case ACTOR_STATE_CELEBRATE_NO_SOUND:
		{
			image = history->imgSpc[image_index];
			image_inner_index = 4;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame == 20)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}	
		break;
	case ACTOR_ACTION_DEF:
		{
			image = history->imgSpc[image_index];
			switch(hisActor->direction)
			{
			case 0:
				image_inner_index = 1;
				break;
			case 1:
				image_inner_index = 2;
				image_transform |= TRANS_MIRROR;
				break;
			case 2:
				image_inner_index = 0;
				break;
			case 3:
				image_inner_index = 2;
				break;
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
		}
		break;
	case ACTOR_STATE_DEF:
		{
			image = history->imgSpc[image_index];
			switch(hisActor->direction)
			{
			case 0:
				image_inner_index = 1;
				break;
			case 1:
				image_inner_index = 2;
				image_transform |= TRANS_MIRROR;
				break;
			case 2:
				image_inner_index = 0;
				break;
			case 3:
				image_inner_index = 2;
				break;
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame == 1)
			{
				playSound(SOUND_FAINT);
			}
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_HITTED:
		{
			image = history->imgSpc[image_index];	
			image_inner_index = 3;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame == 1)
			{
				playSound(SOUND_FAINT);
			}
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_WEAK:
		{
			image = history->imgMov[image_index];
			image_inner_index = 9;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame == 100)
				setState(ACTOR_STATE_NORMAL);
			if(frame == 1)
			{
				playSound(SOUND_FAINT);
			}
		}	
		break;
	case ACTOR_STATE_SKILL_PREPARE:
		image_width = image_height = 64;
		image = history->imgAtc[image_index];	
		switch(hisActor->direction)
		{
		case 0:
			image_inner_index = 4;
			break;
		case 1:
			image_inner_index = 8;
			image_transform |= TRANS_MIRROR;
			break;
		case 2:
			image_inner_index = 0;
			break;
		case 3:
			image_inner_index = 8;
			break;
		}
		g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
			image_transform,(hisActor->xx() - history->scr_x) - 8,
			(hisActor->yy() - history->scr_y) - 8);
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		break;
	case ACTOR_STATE_ATTACK_PREPARE:
		image_width = image_height = 64;
		image = history->imgAtc[image_index];	
		switch(hisActor->direction)
		{
		case 0:
			image_inner_index = 4;
			break;
		case 1:
			image_inner_index = 8;
			image_transform |= TRANS_MIRROR;
			break;
		case 2:
			image_inner_index = 0;
			break;
		case 3:
			image_inner_index = 8;
			break;
		}
		g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
			image_transform,(hisActor->xx() - history->scr_x) - 8,
			(hisActor->yy() - history->scr_y) - 8);
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		if(frame == TIMER_ACTOR_PREPARE_ATTACK_DELAY)
		{
			setState(ACTOR_STATE_NORMAL);
		}
		break;
	case ACTOR_STATE_ATTACK_PREPARE_SOUND:
		{
			image_width = image_height = 64;
			image = history->imgAtc[image_index];	
			switch(hisActor->direction)
			{
			case 0:
				image_inner_index = 4;
				break;
			case 1:
				image_inner_index = 8;
				image_transform |= TRANS_MIRROR;
				break;
			case 2:
				image_inner_index = 0;
				break;
			case 3:
				image_inner_index = 8;
				break;
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x) - 8,
				(hisActor->yy() - history->scr_y) - 8);
			if(frame == 1)
			{
				if (trData->occu_types[hisActor->GetActorOccuType()].atk_music >= 0)
					playSound(trData->occu_types[hisActor->GetActorOccuType()].atk_music);
			}
		}
		break;
	case ACTOR_STATE_ATTACK:
		{
			image_width = image_height = 64;
			image = history->imgAtc[image_index];	
			if(frame<TIMER_ACTOR_ATTACK_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 4;
					break;
				case 1:
					image_inner_index = 8;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 0;
					break;
				case 3:
					image_inner_index = 8;
					break;
				}
			}	
			else if(frame<2*TIMER_ACTOR_ATTACK_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 5;
					break;
				case 1:
					image_inner_index = 9;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 1;
					break;
				case 3:
					image_inner_index = 9;
					break;
				}
			}	
			else if(frame<3*TIMER_ACTOR_ATTACK_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 6;
					break;
				case 1:
					image_inner_index = 10;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 2;
					break;
				case 3:
					image_inner_index = 10;
					break;
				}
			}
			else
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 7;
					break;
				case 1:
					image_inner_index = 11;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 3;
					break;
				case 3:
					image_inner_index = 11;
					break;
				}
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x)-8,
				(hisActor->yy() - history->scr_y)-8);
			if(frame == 1)
			{
				playSound(SOUND_ATTACK);
			}
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_ATTACK_TWICE:
		{
			image_width = image_height = 64;
			image = history->imgAtc[image_index];	
			if(frame<TIMER_ACTOR_ATTACK_FRAME||(frame>=5*TIMER_ACTOR_ATTACK_FRAME&&frame<6*TIMER_ACTOR_ATTACK_FRAME))
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 4;
					break;
				case 1:
					image_inner_index = 8;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 0;
					break;
				case 3:
					image_inner_index = 8;
					break;
				}
			}	
			else if(frame<2*TIMER_ACTOR_ATTACK_FRAME||(frame>=6*TIMER_ACTOR_ATTACK_FRAME&&frame<7*TIMER_ACTOR_ATTACK_FRAME))
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 5;
					break;
				case 1:
					image_inner_index = 9;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 1;
					break;
				case 3:
					image_inner_index = 9;
					break;
				}
			}	
			else if(frame<3*TIMER_ACTOR_ATTACK_FRAME||(frame>=7*TIMER_ACTOR_ATTACK_FRAME&&frame<8*TIMER_ACTOR_ATTACK_FRAME))
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 6;
					break;
				case 1:
					image_inner_index = 10;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 2;
					break;
				case 3:
					image_inner_index = 10;
					break;
				}
			}
			else if(frame<5*TIMER_ACTOR_ATTACK_FRAME||(frame>=8*TIMER_ACTOR_ATTACK_FRAME))
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 7;
					break;
				case 1:
					image_inner_index = 11;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 3;
					break;
				case 3:
					image_inner_index = 11;
					break;
				}
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x)-8,
				(hisActor->yy() - history->scr_y)-8);
			if(frame == 1||frame == 5*TIMER_ACTOR_ATTACK_FRAME)
			{
				playSound(SOUND_ATTACK);
			}
			if(frame==10*TIMER_ACTOR_ATTACK_FRAME)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_ROUND_SLOW:
		{
			if((frame-1)%(3*TIMER_ACTOR_ROUND_SLOW_FRAME) == 0)
			{
				if(frame == 1||frame == 6*TIMER_ACTOR_ROUND_SLOW_FRAME+1)
					playSound(SOUND_ACTOR_TURN);
				if(hisActor->direction==3)
					hisActor->direction = 0;
				else
					hisActor->direction++;
				
			}
			if(frame == 12*TIMER_ACTOR_ROUND_SLOW_FRAME)
			{
				setState(ACTOR_STATE_NORMAL);
			}
			image = history->imgMov[image_index];
			if((frame-1)%(3*TIMER_ACTOR_ROUND_SLOW_FRAME) >= TIMER_ACTOR_ROUND_SLOW_FRAME&&
				(frame-1)%(3*TIMER_ACTOR_ROUND_SLOW_FRAME) < 2*TIMER_ACTOR_ROUND_SLOW_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 2;
					break;
				case 1:
					image_inner_index = 4;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 0;
					break;
				case 3:
					image_inner_index = 4;
					break;
				}
			}
			else
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 3;
					break;
				case 1:
					image_inner_index = 5;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 1;
					break;
				case 3:
					image_inner_index = 5;
					break;
				}
			}

			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
		}
		break;
	case ACTOR_STATE_BREATH:
		{
			if(frame%(TIMER_ACTOR_CHANGE_STYLE_FRAME*2)<TIMER_ACTOR_CHANGE_STYLE_FRAME)
			{
				image = history->imgMov[image_index];
				image_inner_index = 9;
			}	
			else
			{
				image = history->imgMov[image_index];
				image_inner_index = 10;
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame == 1)
			{
				playSound(SOUND_FAINT);
			}
			if(frame == TIMER_ACTOR_CHANGE_STYLE_FRAME*4)
				setState(ACTOR_STATE_NORMAL);
		}
		break;
	case ACTOR_STATE_FAINT:
		{
			//未完成
			image = history->imgSpc[image_index];
			image_inner_index = 3;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame == TIMER_ACTOR_FAINT_FRAME)
				setState(ACTOR_STATE_NORMAL);
			if(frame == 1)
			{
				playSound(SOUND_FAINT);
			}
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_ROUND_QUICK:
	case ACTOR_STATE_ROUND_NORMAL:
		{
			//未完成
			if((frame-1)%(TIMER_ACTOR_ROUND_NORMAL_FRAME) == 0)
			{
				
				if(hisActor->direction==3)
					hisActor->direction = 0;
				else
					hisActor->direction++;
				
			}
			image = history->imgSpc[image_index];	
			switch(hisActor->direction)
			{
			case 0:
				image_inner_index = 1;
				break;
			case 1:
				image_inner_index = 2;
				image_transform |= TRANS_MIRROR;
				break;
			case 2:
				image_inner_index = 0;
				break;
			case 3:
				image_inner_index = 2;
				break;
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame >= 4*TIMER_ACTOR_ROUND_NORMAL_FRAME)
				setState(ACTOR_STATE_NORMAL);
		}
		break;
	case ACTOR_STATE_MOVE:
		{
			image = history->imgMov[image_index];	
			//history->setting_flag[5]+1
			if((frame-1)%(48/PIXEL_PER_FRAME)==0)
			{
				MovePathPtr move  = moveList->elementAt(move_index);
				if (!move)
				{
					break;
				}
				move_x = 0;
				move_y = 0;
				if(move_step!=0)
				{
					DPointPtr thisPoint= move->path->elementAt(move_step - 1);
					hisActor->x =  thisPoint->x;
					hisActor->y =  thisPoint->y;
					history->CheckPosActor(hisActor);
				}
				if (history->var2[5018] == 1)
				{
					if (move_step == move->path->size())
					{
					}
					else
					{
						DPointPtr nextPoint = move->path->elementAt(move_step);
						HistoryActorPtr actor = history->FindActorPosition(nextPoint->x, nextPoint->y);
						if (actor && actor->relation == ACTOR_RELATION_ENEMY)
						{
							move_step = move->path->size();
							if (!this->hisActor->good_effects[36])
							{
								this->hisActor->action_finished = 1;
							}
						}
					}
				}
				if(move_step == move->path->size())
				{
					setState(ACTOR_STATE_NORMAL);
					if(move_end_direction>=0)
					{
						hisActor->direction = (short)move_end_direction;
						move_end_direction = -1;
					}
					//move结束，改变方向。
					if(is_default_move)
					{
						clearMoveList();
					}

					if (hisActor->m_pArmSkill)
					{
						((ActionLayer*)(ChessApplication::getInstance()->GetActionLayer()))->stopSkillAction(hisActor);
					}
				}
				else
				{
					DPointPtr nextPoint= move->path->elementAt(move_step);
					if(nextPoint->x<hisActor->x)
					{
						hisActor->direction = 3;
					}
					else if(nextPoint->x>hisActor->x)
					{
						hisActor->direction = 1;
					}
					else if(nextPoint->y<hisActor->y)
					{
						hisActor->direction = 0;
					}
					else if(nextPoint->y>hisActor->y)
					{
						hisActor->direction = 2;
					}
					move_step++;	
				}
			}
			if(hisActor->style !=ACTOR_STATE_NORMAL)
			{
				switch(hisActor->direction)
				{
				case 0:
					move_y-=PIXEL_PER_FRAME;
					break;
				case 1:
					move_x+=PIXEL_PER_FRAME;
					break;
				case 2:
					move_y+=PIXEL_PER_FRAME;
					break;
				case 3:
					move_x-=PIXEL_PER_FRAME;
					break;
				}
			}
			if ((frame - 1) % (48 / PIXEL_PER_FRAME)<24 / PIXEL_PER_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 2;
					break;
				case 1:
					image_inner_index = 4;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 0;
					break;
				case 3:
					image_inner_index = 4;
					break;
				}
			}
			else
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 3;
					break;
				case 1:
					image_inner_index = 5;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 1;
					break;
				case 3:
					image_inner_index = 5;
					break;
				}
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x)+move_x,
				(hisActor->yy() - history->scr_y)+move_y);//画移动图片
			int y = hisActor->y;
			int x = hisActor->x;
			if (y >= history->map_height)
				y = history->map_height - 1;
			if (x >= history->map_width)
				x = history->map_width - 1;
			if (y < 0)
				y = 0;
			if (x < 0)
				x = 0;
			LandformAttrPtr curLandForm = &trData->lands[history->map_array[y][x]];
			switch (curLandForm->landform_id)
			{
				case 13://大河移动
				{
					if (!hisActor->good_effects[33] && hisActor->occupation_id != 19)
					{
						switch (hisActor->direction)
						{
						case 0:
							g->bg_img = history->imgLandRiver_ahead[frame % 2];
							g->drawImage(history->imgRiver_ahead[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
								(hisActor->yy() - history->scr_y) + move_y, 2);
							break;
						case 1:
							g->bg_img = history->imgLandRiver_left[frame % 2];
							g->drawImage(history->imgRiver_left[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
								(hisActor->yy() - history->scr_y) + move_y, 2);
							break;
						case 2:
							g->bg_img = history->imgLandRiver_back[frame % 2];
							g->drawImage(history->imgRiver_back[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
								(hisActor->yy() - history->scr_y) + move_y, 2);
							break;
						case 3:
							g->bg_img = history->imgLandRiver_left[frame % 2];
							g->drawImage(history->imgRiver_left[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
								(hisActor->yy() - history->scr_y) + move_y, 2);
							break;
						}
					}
				}
				break;
			default:
				g->bg_img = NULL;
				break;
			}


			if(moveList!=NULL)
			{
				MovePathPtr move  = moveList->elementAt(move_index);
				//&& (move_step < move->path->size()-1))||frame ==1
				if ((frame % (96 / PIXEL_PER_FRAME) == 1) && move != NULL)
				{
					int y = hisActor->y;
					int x = hisActor->x;

					if (y >= history->map_height)
						y = history->map_height - 1;
					if (x >= history->map_width)
						x = history->map_width - 1;
					if (y < 0)
						y = 0;
					if (x < 0)
						x = 0;
					LandformAttrPtr curLandForm = &trData->lands[history->map_array[y][x]];
					
						switch (curLandForm->landform_id)//移动声效
						{
							case 1://草原
								playSound(SOUND_MOVE_ON_GRASS);
								break;
							case 2://树林
								playSound(SOUND_MOVE_ON_GRASS);
								break;
							case 7://雪原
								playSound(SOUND_MOVE_ON_SNOW);
								break;
							case 9://浅滩
							case 10:
								playSound(SOUND_MOVE_ON_FORD);
								break;
							case 13://大河
								playSound(SOUND_MOVE_ON_RIVER);
								break;
							default:
							{
								switch (trData->occupations[hisActor->occupation_id].musicId)
								{
									case 0:
										break;
									case 1:
										playSound(SOUND_INFANTRY_MOVE);
										break;
									case 2:
										playSound(SOUND_CAVALRY_MOVE);
										break;
									case 3:
										playSound(SOUND_CAMION_MOVE);
										break;
									case 4:
										playSound(SOUND_WIND_MOVE);
									default:
										playSound(SOUND_INFANTRY_MOVE);
										break;
								}

							}
								break;
						}
				}
			}
		}
		break;
	case ACTOR_STATE_LEAVE_NORMAL:
		{
			if(frame == 1)
				playSound(SOUND_ACTOR_LEAVE_NORMAL);
			//	playSound(SOUND_ACTOR_DEAD_OWN);
			if(frame>=5*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				hisActor->hide = 1;
				hisActor->is_leave = 1;
				if (hisActor->controlled_by_enemy)
				{
					hisActor->recoverState();
					hisActor->delete_controlled_by_enemy();
				}
				break;
			}
			else if(frame%(2*TIMER_ACTOR_LEAVE_NORMAL_FRAME)<TIMER_ACTOR_LEAVE_NORMAL_FRAME||frame==5*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				image = history->imgMov[image_index];
				image_inner_index = 9;
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
		}
		break;
	case ACTOR_STATE_LEAVE_HIDE:
		if(frame>=5*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			hisActor->hide = 1;
			hisActor->is_leave = 1;
			if (hisActor->controlled_by_enemy)
			{
				hisActor->recoverState();
				hisActor->delete_controlled_by_enemy();
			}
			//hisActor->style = ACTOR_STATE_NORMAL;
			break;
		}
		else if(3*frame<image_height)
		{
			image = history->imgSpc[image_index];	
			image_inner_index = 0;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height - 3*frame,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
		}
		if (frame == 1)
		{
			playSound(SOUND_ACTOR_LEAVE_HIDE);
		}
		break;
	case ACTOR_STATE_LEAVE_DEAD:
		image = history->imgMov[image_index];
		image_inner_index = 9;
		if(frame>=20*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			hisActor->hide = 1;
			hisActor->is_leave = 1;
			if (hisActor->controlled_by_enemy)
			{
				hisActor->recoverState();
				hisActor->delete_controlled_by_enemy();
			}
			break;
		}
		else if(frame<=5*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			if(frame%(2*TIMER_ACTOR_LEAVE_NORMAL_FRAME)<TIMER_ACTOR_LEAVE_NORMAL_FRAME||frame==5*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
		}
		else if(frame<=10*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			if(frame == 10*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
				playSound(SOUND_ACTOR_LEAVE_DEAD);
			if(frame%(TIMER_ACTOR_LEAVE_NORMAL_FRAME)<(TIMER_ACTOR_LEAVE_NORMAL_FRAME/2)||frame==10*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
		}
		else if(frame<=15*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			//image_transform |= 1<<((frame/TIMER_ACTOR_LEAVE_NORMAL_FRAME)-2);
			//image_transform |= 1<<13;
			int fff = -2+(frame/TIMER_ACTOR_LEAVE_NORMAL_FRAME);
			image_transform |= 1<<(fff);
			if(frame%(TIMER_ACTOR_LEAVE_NORMAL_FRAME)<TIMER_ACTOR_LEAVE_NORMAL_FRAME||frame==10*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
		}
		else if(frame<=16*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			image_transform |= 1<<13;
			//image_transform |= 1<<(-5+(frame/TIMER_ACTOR_LEAVE_NORMAL_FRAME));
			if(frame%(TIMER_ACTOR_LEAVE_NORMAL_FRAME)<TIMER_ACTOR_LEAVE_NORMAL_FRAME||frame==10*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
		}
		else
		{
			image_transform |= 1<<13;
			image_transform |= 1<<((frame/TIMER_ACTOR_LEAVE_NORMAL_FRAME));
			if(frame%(TIMER_ACTOR_LEAVE_NORMAL_FRAME)<TIMER_ACTOR_LEAVE_NORMAL_FRAME||frame==10*TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
		}
		break;
	case ACTOR_STATE_NORMAL_ATTACK:
		{
			image_width = image_height = 64;
			image = history->imgAtc[image_index];	
			if(frame<TIMER_ACTOR_ATTACK_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 4;
					break;
				case 1:
					image_inner_index = 8;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 0;
					break;
				case 3:
					image_inner_index = 8;
					break;
				}
			}	
			else if(frame<2*TIMER_ACTOR_ATTACK_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 5;
					break;
				case 1:
					image_inner_index = 9;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 1;
					break;
				case 3:
					image_inner_index = 9;
					break;
				}
			}	
			else if(frame<3*TIMER_ACTOR_ATTACK_FRAME)
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 6;
					break;
				case 1:
					image_inner_index = 10;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 2;
					break;
				case 3:
					image_inner_index = 10;
					break;
				}
			}
			else
			{
				switch(hisActor->direction)
				{
				case 0:
					image_inner_index = 7;
					break;
				case 1:
					image_inner_index = 11;
					image_transform |= TRANS_MIRROR;
					break;
				case 2:
					image_inner_index = 3;
					break;
				case 3:
					image_inner_index = 11;
					break;
				}
			}
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x)-8,
				(hisActor->yy() - history->scr_y)-8);
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_ATTACKED:
		{
			image = history->imgSpc[image_index];	
			image_inner_index = 3;
			if(frame<2*TIMER_ACTOR_ATTACK_FRAME)
			{
				image_transform |= TRANS_WHITE_9;
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
			else
			{
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x),
					(hisActor->yy() - history->scr_y));
			}
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_DEADLY_ATTACK_PREPARE:
		{
			image_width = image_height = 64;
			image = history->imgAtc[image_index];	
			switch(hisActor->direction)
			{
			case 0:
				image_inner_index = 4;
				break;
			case 1:
				image_inner_index = 8;
				image_transform |= TRANS_MIRROR;
				break;
			case 2:
				image_inner_index = 0;
				break;
			case 3:
				image_inner_index = 8;
				break;
			}
			if(frame<9)
			{
				image_transform |= 1<<(frame+4);
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x) - 8,
					(hisActor->yy() - history->scr_y) - 8);
			}
			else
			{
				image_transform |= 1<<(13);
				g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
					image_transform,(hisActor->xx() - history->scr_x) - 8,
					(hisActor->yy() - history->scr_y) - 8);
			}
			if(frame == 1)
			{
				if (trData->occu_types[hisActor->GetActorOccuType()].atk_music >= 0)
					playSound(trData->occu_types[hisActor->GetActorOccuType()].atk_music);
			}
			else if(frame == TIMER_ACTOR_DEADLY_ATTACK_SOUND_DELAY)
			{
				playSound(SOUND_DEADLY_PREPARE_ATTACK);
			}
			if (frame == 100)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_GOOD_GIVE:
		{
			if(frame == 1)
				playSound(SOUND_GET_GOOD);
			image = history->imgSpc[image_index];
			image_inner_index = 4;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			if(frame <20)
			{
				if (goodImg != NULL)
				{
					g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10,
						(hisActor->yy() - history->scr_y) + 10 - frame, 1);
				}
				
			}
			else
			{
				if (goodImg != NULL)
				{
					g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10,
						(hisActor->yy() - history->scr_y) - 10, 1);
				}
			}
			if(frame == TIMER_GOOD_GIVE_FRAME)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_CHANGE_FASHION:
	{
		if(frame == 1)
			playSound(SOUND_GET_GOOD);
		image = history->imgSpc[image_index];
		image_inner_index = 4;

		if (frame > 25 && frame < 35)
		{
			image_transform |= 1 << (frame + 4);
		}
		else if (frame >= 35)
		{
			image_transform |= 1 << (13);
		}
		g->drawRegion(image, 0, image_height*image_inner_index, image_width, image_height, image_transform, (hisActor->xx() - history->scr_x), (hisActor->yy() - history->scr_y));

		if(frame < 20)
		{
			if(goodImg != NULL)
			{
				g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10, (hisActor->yy() - history->scr_y) + 10 - frame, 1);
			}
		}

		if(frame == TIMER_ACTOR_CHANGE_FASHION)
		{
			setState(ACTOR_STATE_NORMAL);
		}
	}
	break;
	case ACTOR_STATE_GOOD_ACCEPT:
		{
			image = history->imgSpc[image_index];
			image_inner_index = 4;
			g->drawRegion(image,0,image_height*image_inner_index,image_width,image_height,
				image_transform,(hisActor->xx() - history->scr_x),
				(hisActor->yy() - history->scr_y));
			image_transform |= 1<<(14+frame/4);
			if (goodImg != NULL)
			{
				g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10,
					(hisActor->yy() - history->scr_y) - 10 + frame, image_transform);
			}
		
			if(frame == TIMER_GOOD_ACCEPT_FRAME)
			{
				setState(ACTOR_STATE_NORMAL);
			}
		}
		break;
	case ACTOR_STATE_RUN_ACTION:
		if (frame == m_need_frame)
		{
			setState(ACTOR_STATE_NORMAL);
		}
		break;
	}
	}
	if (hisActor->blood_shield && !hisActor->hide && (hisActor->relation != 2 && history->curActionSide == 2 || hisActor->relation == 2 && history->curActionSide != 2))
	{
		g->drawImage(history->imgShield[frame % 40 / 5],
			(hisActor->xx() - history->scr_x - 7),
			(hisActor->yy() - history->scr_y-7), 1);
	}
	g->bg_img = NULL;
	hisActor->updateBuffPosition(hisActor->style != ACTOR_STATE_MOVE);
	DrawGainText();
}
void BattleActor::GenerateDefaultMove(int des_x,int des_y)
{
	clearMoveList();
	hisActor->GenerateDefaultMove(hisActor->x,hisActor->y,des_x,des_y);
	/*is_default_move = 1;
	moveList = new DVector(false);
	DPointPtr tile;
	MovePathPtr movePoint = (MovePathPtr)ChessAlloc(sizeof(MovePath));
	int x = hisActor->x,y = hisActor->y;
	int dx,dy;
	if(x<des_x)
		dx = 1;
	else 
		dx = -1;
	if(y<des_y)
		dy = 1;
	else 
		dy = -1;
	
	movePoint->point.x = des_x;
	movePoint->point.y = des_y;
	movePoint->path = new DVector(true);
	while(x!=des_x)
	{
		tile = (DPointPtr)ChessAlloc(sizeof(DPoint));
		x += dx;
		tile->x = x;
		tile->y = y;
		movePoint->path->addElement(tile);
	}
	while(y!=des_y)
	{
		tile = (DPointPtr)ChessAlloc(sizeof(DPoint));
		y += dy;
		tile->x = x;
		tile->y = y;
		movePoint->path->addElement(tile);
	}
	moveList->addElement(movePoint);
	move_index = 0;*/
}
void BattleActor::setMoveIndex(int i)
{
	move_index = i;
}
void BattleActor::setMoveList(DList<MovePath>* mlist)
{
	moveList = mlist;
}
void BattleActor::clearMoveList()
{
	if (moveList == NULL)
	{
		return;
	}
	SAFE_DELETE(moveList);
	move_index = 0;
}
int BattleActor::setState(int state,int iSetFrame)
{
	if ((hisActor->style == ACTOR_STATE_LEAVE_NORMAL || hisActor->style == ACTOR_STATE_LEAVE_DEAD || hisActor->style == ACTOR_STATE_LEAVE_HIDE) || (state == ACTOR_STATE_LEAVE_NORMAL || state == ACTOR_STATE_LEAVE_DEAD || state == ACTOR_STATE_LEAVE_HIDE) && hisActor->is_leave == 1)
		return 0;

	int need_frame = 0;
	frame = 0;
	m_need_frame = 0;
	hisActor->style = state;

	if (255==hisActor->style){//剧本指令动作无

		hisActor->style = ACTOR_STATE_NORMAL;
	}
	
	hisActor->hide = 0;
	hurt_hp = 0;
	hurt_mp = 0;
	hurt_money = 0;
	m_bIsDeally = false;
	m_bSkillMiss = false;
	history->CheckPosActor(hisActor);
	switch(hisActor->style){
	case ACTOR_STATE_NORMAL:
	case ACTOR_STATE_CELEBRATE:
	case ACTOR_STATE_CELEBRATE_NO_SOUND:
	case ACTOR_STATE_DEF:
	case ACTOR_STATE_HITTED:
	case ACTOR_STATE_WEAK:
		break;
	case ACTOR_STATE_BREATH:
		need_frame = TIMER_ACTOR_CHANGE_STYLE_FRAME*4;
		break;
	case ACTOR_STATE_FAINT:
		need_frame = TIMER_ACTOR_FAINT_FRAME;
		break;
	case ACTOR_STATE_ATTACK:
		need_frame = TIMER_ACTOR_ATTACK_FRAME*2;
		break;
	case ACTOR_STATE_ATTACK_TWICE:
		need_frame = TIMER_ACTOR_ATTACK_FRAME*10;
		break;
	case ACTOR_STATE_ROUND_SLOW:
		need_frame = TIMER_ACTOR_ROUND_SLOW_FRAME*12;
		break;
	case ACTOR_STATE_ROUND_QUICK:
	case ACTOR_STATE_ROUND_NORMAL:
		need_frame = TIMER_ACTOR_ROUND_SLOW_FRAME*5;
		break;
	case ACTOR_STATE_MOVE://移动速度
		{
			move_x = 0;
			move_y = 0;
			move_step = 0;
			
			PIXEL_PER_FRAME = 24 / (history->setting_flag[6] + 1);//每秒偏移
			
			if (history->setting_flag[6] != 0)//当选择为最快速度时，步兵，骑兵一样快
			{
				switch (trData->occupations[hisActor->occupation_id].moveId)
				{
				case 1:
					PIXEL_PER_FRAME = PIXEL_PER_FRAME / 2;
					break;
				case 2:
					break;
				default:
					PIXEL_PER_FRAME = PIXEL_PER_FRAME / 2;
					break;
				}
			}
			MovePathPtr path = moveList->elementAt(move_index);
			if (path)
			{
				need_frame = path->path->size()*(48 / PIXEL_PER_FRAME) + 1;
			}
			break;
		}
	case ACTOR_STATE_LEAVE_HIDE:
		need_frame = 5*TIMER_ACTOR_LEAVE_NORMAL_FRAME;
		break;
	case ACTOR_STATE_LEAVE_NORMAL:
		need_frame = 5*TIMER_ACTOR_LEAVE_NORMAL_FRAME;
		break;
	case ACTOR_STATE_LEAVE_DEAD:
		need_frame = 20*TIMER_ACTOR_LEAVE_NORMAL_FRAME;
		break;
	case ACTOR_STATE_NORMAL_ATTACK:
		break;
	case ACTOR_STATE_ATTACKED:
		break;
	case ACTOR_STATE_ATTACK_PREPARE_SOUND:
	case ACTOR_STATE_SKILL_PREPARE:
	case ACTOR_STATE_ATTACK_PREPARE:
		need_frame = TIMER_ACTOR_PREPARE_ATTACK_DELAY;
		break;
	case ACTOR_STATE_DEADLY_ATTACK_PREPARE:
		need_frame = 2*TIMER_ACTOR_PREPARE_ATTACK_DELAY;
		break;
	case ACTOR_STATE_GOOD_GIVE:
		need_frame = TIMER_GOOD_GIVE_FRAME;
		break;
	case ACTOR_STATE_CHANGE_FASHION:
		need_frame = TIMER_ACTOR_CHANGE_FASHION;
		break;
	case ACTOR_STATE_GOOD_ACCEPT:	
		need_frame = TIMER_GOOD_ACCEPT_FRAME;
		break;
	case ACTOR_ACTION_DEF:
		break;
	case ACTOR_STATE_RUN_ACTION:
		m_need_frame = iSetFrame-1;
		need_frame = iSetFrame;
		break;
	default:
		break;
	}
	return need_frame;
}
void BattleActor::SetGoodImage(int good_id)
{
	goodImg = history->GetGoodImage(good_id);
}
BattleActor::~BattleActor()
{
	SAFE_DELETE_IMAGE(this->imgBlue);
	SAFE_DELETE_IMAGE(this->imgGreen);
	SAFE_DELETE_IMAGE(this->imgOrange);
	SAFE_DELETE_IMAGE(this->imgRed);
	SAFE_DELETE_IMAGE(this->imgYellow);
	SAFE_DELETE_IMAGE(this->imgAttack);
}

void BattleActor::DrawGainText()
{

	int iGainNum = 0;

	if(hurt_money)
	{
		iGainNum = hurt_money;
	}
	else if(hurt_hp)
	{
		iGainNum = hurt_hp;
	}
	else if(hurt_mp)
	{
		iGainNum = hurt_mp;
	}

	if(!hisActor->hide && (iGainNum || m_bSkillMiss) && !m_spGain)
	{
		m_spGain = GainNumSprite::create(iGainNum, this);
		m_spGain->setPosition(Vec2(hisActor->xx() - history->scr_x + SCENE_TILE_WIDTH / 2, screen_size_y - (hisActor->yy() - history->scr_y + SCENE_TILE_WIDTH / 2)));
		BattleScreen* ptrScreen = (BattleScreen*)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		ptrScreen->m_pBattleLayer->addChild(m_spGain);
	}

	hurt_hp = 0;
	hurt_mp = 0;
	hurt_money = 0;
	m_bSkillMiss = false;
}
void BattleActor::paintFashion(DGraphics* g, int battle_state, SkillAttrPtr curSkill)
{
	int image_inner_index = 0;
	//int image_index = history->getBattleActorImageId(hisActor->actor_id);
	int fashion_index = history->GetFashionImgIndex(hisActor);
	int iFSImgIndex = hisActor->mFashion[2];
	int iWFSImgIndex = hisActor->mFashion[4];

	int image_transform = TRANS_WITH_BG_IMG;
	int image_width = SCENE_TILE_WIDTH;
	int image_height = SCENE_TILE_WIDTH;

	
	TRImagePtr image = NULL;
	TRImagePtr image_arm = NULL;

	int arm_width = SCENE_TILE_WIDTH;
	int arm_height = SCENE_TILE_WIDTH;
	int fashion_offset = 0;
	int fashion_width = SCENE_TILE_WIDTH;
	int is_top = 0;

	switch (hisActor->style)
	{
	case ACTOR_STATE_NORMAL://正常状态（待命、非待命）
	{
								image = history->imgFashion[iFSImgIndex][fashion_index];
								image_arm = history->imgFashionArms[iWFSImgIndex][0];

								arm_width = arm_height = image_arm->width;
								fashion_offset = (image_width - image->width) / 2;
								fashion_width = image->width;

								if (hisActor->action_finished)
								{
									if (hisActor->GetCurHp() == 0 || (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp()) / hisActor->GetCurHp() > 5)
									{
										image_inner_index = 9;
									}
									else
									{
										switch (hisActor->direction)
										{
										case 0:
											image_inner_index = 7;
											break;
										case 1:
											image_inner_index = 8;
											image_transform |= TRANS_MIRROR;
											break;
										case 2:
											image_inner_index = 6;
											break;
										case 3:
											image_inner_index = 8;
											break;
										}
									}
								}
								else if ((normal_frame % (TIMER_ACTOR_CHANGE_STYLE_FRAME * 2) < TIMER_ACTOR_CHANGE_STYLE_FRAME))
									//&&(BattleScreen.battle_state==BATTLE_STATE_WAIT_COMMAND))
								{
									if (hisActor->GetCurHp() == 0 || (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp()) / hisActor->GetCurHp()>5)
									{
										image_inner_index = 9;
									}
									else
									{
										switch (hisActor->direction)
										{
										case 0:
											image_inner_index = 2;
											break;
										case 1:
											image_inner_index = 4;
											image_transform |= TRANS_MIRROR;
											break;
										case 2:
											image_inner_index = 0;
											break;
										case 3:
											image_inner_index = 4;
											break;
										}
									}
								}
								else
								{
									if (hisActor->GetCurHp() == 0 || (hisActor->GetBaseHp() + hisActor->GetGoodsAddHp()) / hisActor->GetCurHp() > 5)
									{
										image_inner_index = 10;
									}
									else
									{
										switch (hisActor->direction)
										{
										case 0:
											image_inner_index = 3;
											break;
										case 1:
											image_inner_index = 5;
											image_transform |= TRANS_MIRROR;
											break;
										case 2:
											image_inner_index = 1;
											break;
										case 3:
											image_inner_index = 5;
											break;
										}
									}
								}

								if (hisActor->action_finished)
								{
									image_transform |= TRANS_FINISH;
								}
								else if (unnormal_state&&hisActor->state_poison)
								{
									image_transform |= TRANS_POISON;
								}

								is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];

								if (!is_top)
								{
									g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										arm_width, arm_width, image_transform,
										(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								}
								g->drawRegion(image, 0, fashion_width*image_inner_index,
									fashion_width, fashion_width, image_transform,
									(hisActor->xx() - history->scr_x + fashion_offset),
									(hisActor->yy() - history->scr_y + fashion_offset));
								if (is_top)
								{
									g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										arm_width, arm_width, image_transform,
										(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								}
								
								if (normal_frame % (2 * TIMER_ACTOR_CHANGE_STYLE_FRAME) >= TIMER_ACTOR_CHANGE_STYLE_FRAME / 2 &&
									normal_frame % (2 * TIMER_ACTOR_CHANGE_STYLE_FRAME) <= TIMER_ACTOR_CHANGE_STYLE_FRAME * 3 / 2)
								{
									unnormal_state = true;
								}
								else
								{
									unnormal_state = false;
								}
								paintUnnormalState(g, battle_state, curSkill);
	}
		break;
	case ACTOR_STATE_CELEBRATE://举起武器
		image = history->imgFashion[iFSImgIndex][fashion_index+2];
		image_arm = history->imgFashionArms[iWFSImgIndex][2];
		arm_width = arm_height = image_arm->width;
		fashion_offset = (image_width - image->width) / 2;
		fashion_width = image->width;
		image_inner_index = 4;
		is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

		if (!is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}

		g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
			image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
			(hisActor->yy() - history->scr_y + fashion_offset));

		if (is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}

		if (frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		if (frame == 100)
		{
			setState(ACTOR_STATE_NORMAL);
		}
		break;
	case ACTOR_STATE_CELEBRATE_NO_SOUND:
	{
											//image = history->imgSpc[image_index];
											image = history->imgFashion[iFSImgIndex][fashion_index + 2];
											image_arm = history->imgFashionArms[iWFSImgIndex][2];
											arm_width = arm_height = image_arm->width;
											fashion_offset = (image_width - image->width) / 2;
											fashion_width = image->width;
											image_inner_index = 4;
											is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

											if (!is_top)
											{
												g->drawRegion(image_arm, 0, arm_width*image_inner_index,
													arm_width, arm_width, image_transform,
													(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
													(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
											}

											g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
												image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
												(hisActor->yy() - history->scr_y + fashion_offset));

											if (is_top)
											{
												g->drawRegion(image_arm, 0, arm_width*image_inner_index,
													arm_width, arm_width, image_transform,
													(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
													(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
											}
											
											if (frame == 20)
											{
												setState(ACTOR_STATE_NORMAL);
											}
	}
		break;
	case ACTOR_ACTION_DEF://防御时的动作
	{
							  //image = history->imgSpc[image_index];
							  image = history->imgFashion[iFSImgIndex][fashion_index+2];
							  image_arm = history->imgFashionArms[iWFSImgIndex][2];
							  arm_width = arm_height = image_arm->width;
							  fashion_offset = (image_width - image->width) / 2;
							  fashion_width = image->width;

							  switch (hisActor->direction)
							  {
							  case 0:
								  image_inner_index = 1;
								  break;
							  case 1:
								  image_inner_index = 2;
								  image_transform |= TRANS_MIRROR;
								  break;
							  case 2:
								  image_inner_index = 0;
								  break;
							  case 3:
								  image_inner_index = 2;
								  break;
							  }
							  is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];
							  if (!is_top)
							  {
								  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									  arm_width, arm_width, image_transform,
									  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							  }
							  g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								  image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
								  (hisActor->yy() - history->scr_y + fashion_offset));
							  if (is_top)
							  {
								  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									  arm_width, arm_width, image_transform,
									  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							  }
	}
		break;
	case ACTOR_STATE_DEF:
	{
							//image = history->imgSpc[image_index];
							image = history->imgFashion[iFSImgIndex][fashion_index+2];
							image_arm = history->imgFashionArms[iWFSImgIndex][2];
							arm_width = arm_height = image_arm->width;
							fashion_offset = (image_width - image->width) / 2;
							fashion_width = image->width;
							switch (hisActor->direction)
							{
							case 0:
								image_inner_index = 1;
								break;
							case 1:
								image_inner_index = 2;
								image_transform |= TRANS_MIRROR;
								break;
							case 2:
								image_inner_index = 0;
								break;
							case 3:
								image_inner_index = 2;
								break;
							}

							is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

							if (!is_top)
							{
								g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									arm_width, arm_width, image_transform,
									(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							}
							g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
								(hisActor->yy() - history->scr_y + fashion_offset));
							if (is_top)
							{
								g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									arm_width, arm_width, image_transform,
									(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							}
							if (frame == 1)
							{
								playSound(SOUND_FAINT);
							}
							if (frame == 100)
							{
								setState(ACTOR_STATE_NORMAL);
							}
	}
		break;
	case ACTOR_STATE_HITTED:
	{
							   //image = history->imgSpc[image_index];
							   image = history->imgFashion[iFSImgIndex][fashion_index + 2];
							   image_arm = history->imgFashionArms[iWFSImgIndex][2];
							   arm_width = arm_height = image_arm->width;
							   fashion_offset = (image_width - image->width) / 2;
							   fashion_width = image->width;
							   image_inner_index = 3;

							   is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

							   if (!is_top)
							   {
								   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									   arm_width, arm_width, image_transform,
									   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							   }

							   g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								   image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
								   (hisActor->yy() - history->scr_y + fashion_offset));

							   if (is_top)
							   {
								   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									   arm_width, arm_width, image_transform,
									   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							   }

							   if (frame == 1)
							   {
								   playSound(SOUND_FAINT);
							   }
							   if (frame == 100)
							   {
								   setState(ACTOR_STATE_NORMAL);
							   }
	}
		break;
	case ACTOR_STATE_WEAK:
	{
							 //image = history->imgMov[image_index];
							 image = history->imgFashion[iFSImgIndex][fashion_index];
							 image_arm = history->imgFashionArms[iWFSImgIndex][0];
							 arm_width = arm_height = image_arm->width;
							 fashion_offset = (image_width - image->width) / 2;
							 fashion_width = image->width;
							 image_inner_index = 9;

							 is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];

							 if (!is_top)
							 {
								 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									 arm_width, arm_width, image_transform,
									 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							 }

							 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								 image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
								 (hisActor->yy() - history->scr_y + fashion_offset));

							 if (is_top)
							 {
								 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									 arm_width, arm_width, image_transform,
									 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							 }

							 if (frame == 100)
								 setState(ACTOR_STATE_NORMAL);
							 if (frame == 1)
							 {
								 playSound(SOUND_FAINT);
							 }
	}
		break;
	case ACTOR_STATE_SKILL_PREPARE:
		//image_width = image_height = 64;
		//image = history->imgAtc[image_index];
		image = history->imgFashion[iFSImgIndex][fashion_index+1];
		image_arm = history->imgFashionArms[iWFSImgIndex][1];
		arm_width = arm_height = image_arm->width;
		fashion_offset = (image_width - image->width) / 2;
		fashion_width = image->width;

		switch (hisActor->direction)
		{
		case 0:
			image_inner_index = 4;
			break;
		case 1:
			image_inner_index = 8;
			image_transform |= TRANS_MIRROR;
			break;
		case 2:
			image_inner_index = 0;
			break;
		case 3:
			image_inner_index = 8;
			break;
		}

		is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];

		if (!is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}
		g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
			image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
			(hisActor->yy() - history->scr_y + fashion_offset));
		if (is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}
		if (frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		break;
	case ACTOR_STATE_ATTACK_PREPARE:
		//image_width = image_height = 64;
		//image = history->imgAtc[image_index];
		image = history->imgFashion[iFSImgIndex][fashion_index + 1];
		image_arm = history->imgFashionArms[iWFSImgIndex][1];
		arm_width = arm_height = image_arm->width;
		fashion_offset = (image_width - image->width) / 2;
		fashion_width = image->width;

		switch (hisActor->direction)
		{
		case 0:
			image_inner_index = 4;
			break;
		case 1:
			image_inner_index = 8;
			image_transform |= TRANS_MIRROR;
			break;
		case 2:
			image_inner_index = 0;
			break;
		case 3:
			image_inner_index = 8;
			break;
		}

		is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];

		if (!is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}
		g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
			image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
			(hisActor->yy() - history->scr_y+fashion_offset));
		if (is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}
		if (frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		if (frame == TIMER_ACTOR_PREPARE_ATTACK_DELAY)
		{
			setState(ACTOR_STATE_NORMAL);
		}
		break;
	case ACTOR_STATE_ATTACK_PREPARE_SOUND:
	{
											 //image_width = image_height = 48;
											 //image = history->imgAtc[image_index];
											 image = history->imgFashion[iFSImgIndex][fashion_index+1];
											 image_arm = history->imgFashionArms[iWFSImgIndex][1];
											 arm_width = arm_height = image_arm->width;
											 fashion_offset = (image_width - image->width) / 2;
											 fashion_width = image->width;
											 switch (hisActor->direction)
											 {
											 case 0:
												 image_inner_index = 4;
												 break;
											 case 1:
												 image_inner_index = 8;
												 image_transform |= TRANS_MIRROR;
												 break;
											 case 2:
												 image_inner_index = 0;
												 break;
											 case 3:
												 image_inner_index = 8;
												 break;
											 }

											 is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];

											 if (!is_top)
											 {
												 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
													 arm_width, arm_width, image_transform,
													 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
													 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
											 }
											 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
												 image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
												 (hisActor->yy() - history->scr_y+fashion_offset));
											 if (is_top)
											 {
												 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
													 arm_width, arm_width, image_transform,
													 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
													 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
											 }
											 if (frame == 1)
											 {
												 if (trData->occu_types[hisActor->GetActorOccuType()].atk_music >= 0)
													 playSound(trData->occu_types[hisActor->GetActorOccuType()].atk_music);
											 }
	}
		break;
	case ACTOR_STATE_ATTACK:
	{
							   //image_width = image_height = 64;
							   //image = history->imgAtc[image_index];
							   image = history->imgFashion[iFSImgIndex][fashion_index + 1];
							   image_arm = history->imgFashionArms[iWFSImgIndex][1];
							   arm_width = arm_height = image_arm->width;
							   fashion_offset = (image_width - image->width) / 2;
							   fashion_width = image->width;

							   if (frame < TIMER_ACTOR_ATTACK_FRAME)
							   {
								   switch (hisActor->direction)
								   {
								   case 0:
									   image_inner_index = 4;
									   break;
								   case 1:
									   image_inner_index = 8;
									   image_transform |= TRANS_MIRROR;
									   break;
								   case 2:
									   image_inner_index = 0;
									   break;
								   case 3:
									   image_inner_index = 8;
									   break;
								   }
							   }
							   else if (frame < 2 * TIMER_ACTOR_ATTACK_FRAME)
							   {
								   switch (hisActor->direction)
								   {
								   case 0:
									   image_inner_index = 5;
									   break;
								   case 1:
									   image_inner_index = 9;
									   image_transform |= TRANS_MIRROR;
									   break;
								   case 2:
									   image_inner_index = 1;
									   break;
								   case 3:
									   image_inner_index = 9;
									   break;
								   }
							   }
							   else if (frame < 3 * TIMER_ACTOR_ATTACK_FRAME)
							   {
								   switch (hisActor->direction)
								   {
								   case 0:
									   image_inner_index = 6;
									   break;
								   case 1:
									   image_inner_index = 10;
									   image_transform |= TRANS_MIRROR;
									   break;
								   case 2:
									   image_inner_index = 2;
									   break;
								   case 3:
									   image_inner_index = 10;
									   break;
								   }
							   }
							   else
							   {
								   switch (hisActor->direction)
								   {
								   case 0:
									   image_inner_index = 7;
									   break;
								   case 1:
									   image_inner_index = 11;
									   image_transform |= TRANS_MIRROR;
									   break;
								   case 2:
									   image_inner_index = 3;
									   break;
								   case 3:
									   image_inner_index = 11;
									   break;
								   }
							   }

							   is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];

							   if (!is_top)
							   {
								   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									   arm_width, arm_width, image_transform,
									   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							   }
							   g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								   image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
								   (hisActor->yy() - history->scr_y+fashion_offset));
							   if (is_top)
							   {
								   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									   arm_width, arm_width, image_transform,
									   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							   }
							   if (frame == 1)
							   {
								   playSound(SOUND_ATTACK);
							   }
							   if (frame == 100)
							   {
								   setState(ACTOR_STATE_NORMAL);
							   }
	}
		break;
	case ACTOR_STATE_ATTACK_TWICE:
	{
									 //image_width = image_height = 64;
									 //image = history->imgAtc[image_index];
									 image = history->imgFashion[iFSImgIndex][fashion_index + 1];
									 image_arm = history->imgFashionArms[iWFSImgIndex][1];
									 arm_width = arm_height = image_arm->width;
									 fashion_offset = (image_width - image->width) / 2;
									 fashion_width = image->width;
									 
									 if (frame < TIMER_ACTOR_ATTACK_FRAME || (frame >= 5 * TIMER_ACTOR_ATTACK_FRAME&&frame < 6 * TIMER_ACTOR_ATTACK_FRAME))
									 {
										 switch (hisActor->direction)
										 {
										 case 0:
											 image_inner_index = 4;
											 break;
										 case 1:
											 image_inner_index = 8;
											 image_transform |= TRANS_MIRROR;
											 break;
										 case 2:
											 image_inner_index = 0;
											 break;
										 case 3:
											 image_inner_index = 8;
											 break;
										 }
									 }
									 else if (frame < 2 * TIMER_ACTOR_ATTACK_FRAME || (frame >= 6 * TIMER_ACTOR_ATTACK_FRAME&&frame < 7 * TIMER_ACTOR_ATTACK_FRAME))
									 {
										 switch (hisActor->direction)
										 {
										 case 0:
											 image_inner_index = 5;
											 break;
										 case 1:
											 image_inner_index = 9;
											 image_transform |= TRANS_MIRROR;
											 break;
										 case 2:
											 image_inner_index = 1;
											 break;
										 case 3:
											 image_inner_index = 9;
											 break;
										 }
									 }
									 else if (frame < 3 * TIMER_ACTOR_ATTACK_FRAME || (frame >= 7 * TIMER_ACTOR_ATTACK_FRAME&&frame < 8 * TIMER_ACTOR_ATTACK_FRAME))
									 {
										 switch (hisActor->direction)
										 {
										 case 0:
											 image_inner_index = 6;
											 break;
										 case 1:
											 image_inner_index = 10;
											 image_transform |= TRANS_MIRROR;
											 break;
										 case 2:
											 image_inner_index = 2;
											 break;
										 case 3:
											 image_inner_index = 10;
											 break;
										 }
									 }
									 else if (frame < 5 * TIMER_ACTOR_ATTACK_FRAME || (frame >= 8 * TIMER_ACTOR_ATTACK_FRAME))
									 {
										 switch (hisActor->direction)
										 {
										 case 0:
											 image_inner_index = 7;
											 break;
										 case 1:
											 image_inner_index = 11;
											 image_transform |= TRANS_MIRROR;
											 break;
										 case 2:
											 image_inner_index = 3;
											 break;
										 case 3:
											 image_inner_index = 11;
											 break;
										 }
									 }

									 is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];

									 if (!is_top)
									 {
										 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											 arm_width, arm_width, image_transform,
											 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									 }
									 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
										 image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
										 (hisActor->yy() - history->scr_y+fashion_offset));
									 if (is_top)
									 {
										 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											 arm_width, arm_width, image_transform,
											 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									 }
									 if (frame == 1 || frame == 5 * TIMER_ACTOR_ATTACK_FRAME)
									 {
										 playSound(SOUND_ATTACK);
									 }
									 if (frame == 10 * TIMER_ACTOR_ATTACK_FRAME)
									 {
										 setState(ACTOR_STATE_NORMAL);
									 }
	}
		break;
	case ACTOR_STATE_ROUND_SLOW:
	{
								   // image = history->imgMov[image_index];
								   image = history->imgFashion[iFSImgIndex][fashion_index];
								   image_arm = history->imgFashionArms[iWFSImgIndex][0];
								   arm_width = arm_height = image_arm->width;
								   fashion_offset = (image_width - image->width) / 2;
								   fashion_width = image->width;

								   if ((frame - 1) % (3 * TIMER_ACTOR_ROUND_SLOW_FRAME) == 0)
								   {
									   if (frame == 1 || frame == 6 * TIMER_ACTOR_ROUND_SLOW_FRAME + 1)
										   playSound(SOUND_ACTOR_TURN);
									   if (hisActor->direction == 3)
										   hisActor->direction = 0;
									   else
										   hisActor->direction++;

								   }
								   if (frame == 12 * TIMER_ACTOR_ROUND_SLOW_FRAME)
								   {
									   setState(ACTOR_STATE_NORMAL);
								   }
								  
								   if ((frame - 1) % (3 * TIMER_ACTOR_ROUND_SLOW_FRAME) >= TIMER_ACTOR_ROUND_SLOW_FRAME &&
									   (frame - 1) % (3 * TIMER_ACTOR_ROUND_SLOW_FRAME) < 2 * TIMER_ACTOR_ROUND_SLOW_FRAME)
								   {
									   switch (hisActor->direction)
									   {
									   case 0:
										   image_inner_index = 2;
										   break;
									   case 1:
										   image_inner_index = 4;
										   image_transform |= TRANS_MIRROR;
										   break;
									   case 2:
										   image_inner_index = 0;
										   break;
									   case 3:
										   image_inner_index = 4;
										   break;
									   }
								   }
								   else
								   {
									   switch (hisActor->direction)
									   {
									   case 0:
										   image_inner_index = 3;
										   break;
									   case 1:
										   image_inner_index = 5;
										   image_transform |= TRANS_MIRROR;
										   break;
									   case 2:
										   image_inner_index = 1;
										   break;
									   case 3:
										   image_inner_index = 5;
										   break;
									   }
								   }

								   is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];

								   if (!is_top)
								   {
									   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										   arm_width, arm_width, image_transform,
										   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								   }
								   g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
									   image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
									   (hisActor->yy() - history->scr_y+fashion_offset));
								   if (is_top)
								   {
									   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										   arm_width, arm_width, image_transform,
										   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								   }
	}
		break;
	case ACTOR_STATE_BREATH:
	{
							   // image = history->imgMov[image_index];
							   image = history->imgFashion[iFSImgIndex][fashion_index];
							   image_arm = history->imgFashionArms[iWFSImgIndex][0];
							   arm_width = arm_height = image_arm->width;
							   fashion_offset = (image_width - image->width) / 2;
							   fashion_width = image->width;
							   
							   if (frame % (TIMER_ACTOR_CHANGE_STYLE_FRAME * 2) < TIMER_ACTOR_CHANGE_STYLE_FRAME)
							   {
								   image_inner_index = 9;
							   }
							   else
							   {
								   image_inner_index = 10;
							   }

							   is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];

							   if (!is_top)
							   {
								   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									   arm_width, arm_width, image_transform,
									   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							   }

							   g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								   image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
								   (hisActor->yy() - history->scr_y+fashion_offset));

							   if (is_top)
							   {
								   g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									   arm_width, arm_width, image_transform,
									   (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									   (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							   }

							   if (frame == 1)
							   {
								   playSound(SOUND_FAINT);
							   }
							   if (frame == TIMER_ACTOR_CHANGE_STYLE_FRAME * 4)
								   setState(ACTOR_STATE_NORMAL);
	}
		break;
	case ACTOR_STATE_FAINT:
	{
							  //未完成
							  //image = history->imgSpc[image_index];
							  image = history->imgFashion[iFSImgIndex][fashion_index+2];
							  image_arm = history->imgFashionArms[iWFSImgIndex][2];
							  arm_width = arm_height = image_arm->width;
							  fashion_offset = (image_width - image->width) / 2;
							  fashion_width = image->width;
							  image_inner_index = 3;

							  is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

							  if (!is_top)
							  {
								  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									  arm_width, arm_width, image_transform,
									  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							  }

							  g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								  image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
								  (hisActor->yy() - history->scr_y+fashion_offset));
							 
							  if (is_top)
							  {
								  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									  arm_width, arm_width, image_transform,
									  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
									  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
							  }

							  if (frame == TIMER_ACTOR_FAINT_FRAME)
								  setState(ACTOR_STATE_NORMAL);
							  if (frame == 1)
							  {
								  playSound(SOUND_FAINT);
							  }
							  if (frame == 100)
							  {
								  setState(ACTOR_STATE_NORMAL);
							  }
	}
		break;
	case ACTOR_STATE_ROUND_QUICK:
	case ACTOR_STATE_ROUND_NORMAL:
	{
									 //未完成
									 //image = history->imgSpc[image_index];
									 image = history->imgFashion[iFSImgIndex][fashion_index + 2];
									 image_arm = history->imgFashionArms[iWFSImgIndex][2];
									 arm_width = arm_height = image_arm->width;
									 fashion_offset = (image_width - image->width) / 2;
									 fashion_width = image->width;

									 if ((frame - 1) % (TIMER_ACTOR_ROUND_NORMAL_FRAME) == 0)
									 {

										 if (hisActor->direction == 3)
											 hisActor->direction = 0;
										 else
											 hisActor->direction++;

									 }
									 switch (hisActor->direction)
									 {
									 case 0:
										 image_inner_index = 1;
										 break;
									 case 1:
										 image_inner_index = 2;
										 image_transform |= TRANS_MIRROR;
										 break;
									 case 2:
										 image_inner_index = 0;
										 break;
									 case 3:
										 image_inner_index = 2;
										 break;
									 }

									 is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

									 if (!is_top)
									 {
										 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											 arm_width, arm_width, image_transform,
											 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									 }
									 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
										 image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
										 (hisActor->yy() - history->scr_y+fashion_offset));
									 if (is_top)
									 {
										 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											 arm_width, arm_width, image_transform,
											 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									 }
									 if (frame >= 4 * TIMER_ACTOR_ROUND_NORMAL_FRAME)
										 setState(ACTOR_STATE_NORMAL);
	}
		break;
	case ACTOR_STATE_MOVE:
	{
							 //image = history->imgMov[image_index];
							 image = history->imgFashion[iFSImgIndex][fashion_index];
							 image_arm = history->imgFashionArms[iWFSImgIndex][0];
							 arm_width = arm_height = image_arm->width;
							 fashion_offset = (image_width - image->width) / 2;
							 fashion_width = image->width;
							 
							 //history->setting_flag[5]+1
							 if ((frame - 1) % (48 / PIXEL_PER_FRAME) == 0)
							 {
								 MovePathPtr move = moveList->elementAt(move_index);
								 if (!move)
								 {
									 return;
								 }
								 move_x = 0;
								 move_y = 0;
								 if (move_step != 0)
								 {
									 DPointPtr thisPoint = move->path->elementAt(move_step - 1);
									 hisActor->x = thisPoint->x;
									 hisActor->y = thisPoint->y;
									 history->CheckPosActor(hisActor);
								 }
								 if (history->var2[5018] == 1)
								 {
									 if (move_step == move->path->size())
									 {
									 }
									 else
									 {
										 DPointPtr nextPoint = move->path->elementAt(move_step);
										 HistoryActorPtr actor = history->FindActorPosition(nextPoint->x, nextPoint->y);
										 if (actor && actor->relation == ACTOR_RELATION_ENEMY)
										 {
											 move_step = move->path->size();
											 if (!this->hisActor->good_effects[36])
											 {
												 this->hisActor->action_finished = 1;
											 }
										 }
									 }
								 }
								 if (move_step == move->path->size())
								 {
									 setState(ACTOR_STATE_NORMAL);
									 if (move_end_direction >= 0)
									 {
										 hisActor->direction = (short)move_end_direction;
										 move_end_direction = -1;
									 }
									 //move结束，改变方向。
									 if (is_default_move)
									 {
										 clearMoveList();
									 }

									 if(hisActor->m_pArmSkill)
									 {
										 ((ActionLayer*)(ChessApplication::getInstance()->GetActionLayer()))->stopSkillAction(hisActor);
									 }
								 }
								 else
								 {
									 DPointPtr nextPoint = move->path->elementAt(move_step);
									 if (nextPoint->x<hisActor->x)
									 {
										 hisActor->direction = 3;
									 }
									 else if (nextPoint->x>hisActor->x)
									 {
										 hisActor->direction = 1;
									 }
									 else if (nextPoint->y<hisActor->y)
									 {
										 hisActor->direction = 0;
									 }
									 else if (nextPoint->y>hisActor->y)
									 {
										 hisActor->direction = 2;
									 }
									 move_step++;
								 }
							 }
							 if (hisActor->style != ACTOR_STATE_NORMAL)
							 {
								 switch (hisActor->direction)
								 {
								 case 0:
									 move_y -= PIXEL_PER_FRAME;
									 break;
								 case 1:
									 move_x += PIXEL_PER_FRAME;
									 break;
								 case 2:
									 move_y += PIXEL_PER_FRAME;
									 break;
								 case 3:
									 move_x -= PIXEL_PER_FRAME;
									 break;
								 }
							 }
							 if ((frame - 1) % (48 / PIXEL_PER_FRAME) < 24 / PIXEL_PER_FRAME)
							 {
								 switch (hisActor->direction)
								 {
								 case 0:
									 image_inner_index = 2;
									 break;
								 case 1:
									 image_inner_index = 4;
									 image_transform |= TRANS_MIRROR;
									 break;
								 case 2:
									 image_inner_index = 0;
									 break;
								 case 3:
									 image_inner_index = 4;
									 break;
								 }
							 }
							 else
							 {
								 switch (hisActor->direction)
								 {
								 case 0:
									 image_inner_index = 3;
									 break;
								 case 1:
									 image_inner_index = 5;
									 image_transform |= TRANS_MIRROR;
									 break;
								 case 2:
									 image_inner_index = 1;
									 break;
								 case 3:
									 image_inner_index = 5;
									 break;
								 }
							 }
							 is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];
							 if (!is_top)
							 {
								 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									 arm_width, arm_width, image_transform,
									 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2) + move_x,
									 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2) + move_y);
							 }
							 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
								 image_transform, (hisActor->xx() - history->scr_x+fashion_offset) + move_x,
								 (hisActor->yy() - history->scr_y+fashion_offset) + move_y);//画移动图片
							 if (is_top)
							 {
								 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
									 arm_width, arm_width, image_transform,
									 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2) + move_x,
									 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2) + move_y);
							 }
							 int y = hisActor->y;
							 int x = hisActor->x;
							 if (y >= history->map_height)
								 y = history->map_height - 1;
							 if (x >= history->map_width)
								 x = history->map_width - 1;
							 if (y < 0)
								 y = 0;
							 if (x < 0)
								 x = 0;
							 LandformAttrPtr curLandForm = &trData->lands[history->map_array[y][x]];
							 switch (curLandForm->landform_id)
							 {
							 case 13://大河移动
							 {
										 if (!hisActor->good_effects[33] && hisActor->occupation_id != 19)
										 {
											 switch (hisActor->direction)
											 {
											 case 0:
												 g->bg_img = history->imgLandRiver_ahead[frame % 2];
												 g->drawImage(history->imgRiver_ahead[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
													 (hisActor->yy() - history->scr_y) + move_y, 2);
												 break;
											 case 1:
												 g->bg_img = history->imgLandRiver_left[frame % 2];
												 g->drawImage(history->imgRiver_left[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
													 (hisActor->yy() - history->scr_y) + move_y, 2);
												 break;
											 case 2:
												 g->bg_img = history->imgLandRiver_back[frame % 2];
												 g->drawImage(history->imgRiver_back[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
													 (hisActor->yy() - history->scr_y) + move_y, 2);
												 break;
											 case 3:
												 g->bg_img = history->imgLandRiver_left[frame % 2];
												 g->drawImage(history->imgRiver_left[frame % 2], (hisActor->xx() - history->scr_x) + move_x,
													 (hisActor->yy() - history->scr_y) + move_y, 2);
												 break;
											 }
										 }
							 }
								 break;
							 default:
								 g->bg_img = NULL;
								 break;
							 }


							 if (moveList != NULL)
							 {
								 MovePathPtr move = moveList->elementAt(move_index);
								 //&& (move_step < move->path->size()-1))||frame ==1
								 if ((frame % (96 / PIXEL_PER_FRAME) == 1) && move != NULL)
								 {
									 int y = hisActor->y;
									 int x = hisActor->x;

									 if (y >= history->map_height)
										 y = history->map_height - 1;
									 if (x >= history->map_width)
										 x = history->map_width - 1;
									 if (y < 0)
										 y = 0;
									 if (x < 0)
										 x = 0;
									 LandformAttrPtr curLandForm = &trData->lands[history->map_array[y][x]];

									 switch (curLandForm->landform_id)//移动声效
									 {
									 case 1://草原
										 playSound(SOUND_MOVE_ON_GRASS);
										 break;
									 case 2://树林
										 playSound(SOUND_MOVE_ON_GRASS);
										 break;
									 case 7://雪原
										 playSound(SOUND_MOVE_ON_SNOW);
										 break;
									 case 9://浅滩
									 case 10:
										 playSound(SOUND_MOVE_ON_FORD);
										 break;
									 case 13://大河
										 playSound(SOUND_MOVE_ON_RIVER);
										 break;
									 default:
									 {
												switch (trData->occupations[hisActor->occupation_id].musicId)
												{
												case 0:
													break;
												case 1:
													playSound(SOUND_INFANTRY_MOVE);
													break;
												case 2:
													playSound(SOUND_CAVALRY_MOVE);
													break;
												case 3:
													playSound(SOUND_CAMION_MOVE);
													break;
												case 4:
													playSound(SOUND_WIND_MOVE);
												default:
													playSound(SOUND_INFANTRY_MOVE);
													break;
												}

									 }
										 break;
									 }
								 }
							 }
	}
		break;
	case ACTOR_STATE_LEAVE_NORMAL:
	{
									 if (frame == 1)
										 playSound(SOUND_ACTOR_LEAVE_NORMAL);
									 //	playSound(SOUND_ACTOR_DEAD_OWN);
									 if (frame >= 5 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
									 {
										 hisActor->hide = 1;
										 hisActor->is_leave = 1;
										 if (hisActor->controlled_by_enemy)
										 {
											 hisActor->recoverState();
											 hisActor->delete_controlled_by_enemy();
										 }
										 return;
									 }
									 else if (frame % (2 * TIMER_ACTOR_LEAVE_NORMAL_FRAME) < TIMER_ACTOR_LEAVE_NORMAL_FRAME || frame == 5 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
									 {
										 //image = history->imgMov[image_index];
										 image = history->imgFashion[iFSImgIndex][fashion_index];
										 image_arm = history->imgFashionArms[iWFSImgIndex][0];
										 arm_width = arm_height = image_arm->width;
										 fashion_offset = (image_width - image->width) / 2;
										 fashion_width = image->width;
										 image_inner_index = 9;

										 is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];

										 if (!is_top)
										 {
											 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
												 arm_width, arm_width, image_transform,
												 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
												 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
										 }

										 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
											 image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
											 (hisActor->yy() - history->scr_y+fashion_offset));

										 if (is_top)
										 {
											 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
												 arm_width, arm_width, image_transform,
												 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
												 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
										 }
									 }
	}
		break;
	case ACTOR_STATE_LEAVE_HIDE:
		if (frame >= 5 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			hisActor->hide = 1;
			hisActor->is_leave = 1;
			if (hisActor->controlled_by_enemy)
			{
				hisActor->recoverState();
				hisActor->delete_controlled_by_enemy();
			}
			//hisActor->style = ACTOR_STATE_NORMAL;
			return;
		}
		else if (3 * frame < image_height)
		{
			//image = history->imgSpc[image_index];
			image = history->imgFashion[iFSImgIndex][fashion_index+2];
			image_arm = history->imgFashionArms[iWFSImgIndex][2];
			arm_width = arm_height = image_arm->width;
			fashion_offset = (image_width - image->width) / 2;
			fashion_width = image->width;
			image_inner_index = 0;
			is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

			if (!is_top)
			{
				g->drawRegion(image_arm, 0, arm_width*image_inner_index,
					arm_width, arm_width - 3 * frame, image_transform,
					(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
					(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
			}
			g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width - 3 * frame,
				image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
				(hisActor->yy() - history->scr_y+fashion_offset));
			if (is_top)
			{
				g->drawRegion(image_arm, 0, arm_width*image_inner_index,
					arm_width, arm_width - 3 * frame, image_transform,
					(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
					(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
			}
		}
		if (frame == 1)
		{
			playSound(SOUND_ACTOR_LEAVE_HIDE);
		}
		break;
	case ACTOR_STATE_LEAVE_DEAD:
		//image = history->imgMov[image_index];
		image = history->imgFashion[iFSImgIndex][fashion_index];
		image_arm = history->imgFashionArms[iWFSImgIndex][0];
		arm_width = arm_height = image_arm->width;
		fashion_offset = (image_width - image->width) / 2;
		fashion_width = image->width;
		image_inner_index = 9;
		is_top = trData->net_hero_dress[iWFSImgIndex]->mMov[image_inner_index];
		if (frame >= 20 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			hisActor->hide = 1;
			hisActor->is_leave = 1;
			if (hisActor->controlled_by_enemy)
			{
				hisActor->recoverState();
				hisActor->delete_controlled_by_enemy();
			}
			return;
		}
		else if (frame <= 5 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			if (frame % (2 * TIMER_ACTOR_LEAVE_NORMAL_FRAME) < TIMER_ACTOR_LEAVE_NORMAL_FRAME || frame == 5 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				if (!is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
				g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
					image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
					(hisActor->yy() - history->scr_y+fashion_offset));
				if (is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
			}
		}
		else if (frame <= 10 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			if (frame == 10 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
				playSound(SOUND_ACTOR_LEAVE_DEAD);
			if (frame % (TIMER_ACTOR_LEAVE_NORMAL_FRAME) < (TIMER_ACTOR_LEAVE_NORMAL_FRAME / 2) || frame == 10 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				if (!is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
				g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
					image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
					(hisActor->yy() - history->scr_y+fashion_offset));
				if (is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
			}
		}
		else if (frame <= 15 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			//image_transform |= 1<<((frame/TIMER_ACTOR_LEAVE_NORMAL_FRAME)-2);
			//image_transform |= 1<<13;
			int fff = -2 + (frame / TIMER_ACTOR_LEAVE_NORMAL_FRAME);
			image_transform |= 1 << (fff);
			if (frame % (TIMER_ACTOR_LEAVE_NORMAL_FRAME) < TIMER_ACTOR_LEAVE_NORMAL_FRAME || frame == 10 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				if (!is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
				g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
					image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
					(hisActor->yy() - history->scr_y+fashion_offset));
				if (is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
			}
		}
		else if (frame <= 16 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
		{
			image_transform |= 1 << 13;
			//image_transform |= 1<<(-5+(frame/TIMER_ACTOR_LEAVE_NORMAL_FRAME));
			if (frame % (TIMER_ACTOR_LEAVE_NORMAL_FRAME) < TIMER_ACTOR_LEAVE_NORMAL_FRAME || frame == 10 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				if (!is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
				g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
					image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
					(hisActor->yy() - history->scr_y+fashion_offset));
				if (is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
			}
		}
		else
		{
			image_transform |= 1 << 13;
			image_transform |= 1 << ((frame / TIMER_ACTOR_LEAVE_NORMAL_FRAME));
			if (frame % (TIMER_ACTOR_LEAVE_NORMAL_FRAME) < TIMER_ACTOR_LEAVE_NORMAL_FRAME || frame == 10 * TIMER_ACTOR_LEAVE_NORMAL_FRAME)
			{
				if (!is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
				g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
					image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
					(hisActor->yy() - history->scr_y+fashion_offset));
				if (is_top)
				{
					g->drawRegion(image_arm, 0, arm_width*image_inner_index,
						arm_width, arm_width, image_transform,
						(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
						(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
				}
			}
		}
		break;
	case ACTOR_STATE_NORMAL_ATTACK:
	{
									  //image_width = image_height = 48;
									  //image = history->imgAtc[image_index];
									  image = history->imgFashion[iFSImgIndex][fashion_index + 1];
									  image_arm = history->imgFashionArms[iWFSImgIndex][1];
									  arm_width = arm_height = image_arm->width;
									  fashion_offset = (image_width - image->width) / 2;
									  fashion_width = image->width;
									  if (frame < TIMER_ACTOR_ATTACK_FRAME)
									  {
										  switch (hisActor->direction)
										  {
										  case 0:
											  image_inner_index = 4;
											  break;
										  case 1:
											  image_inner_index = 8;
											  image_transform |= TRANS_MIRROR;
											  break;
										  case 2:
											  image_inner_index = 0;
											  break;
										  case 3:
											  image_inner_index = 8;
											  break;
										  }
									  }
									  else if (frame < 2 * TIMER_ACTOR_ATTACK_FRAME)
									  {
										  switch (hisActor->direction)
										  {
										  case 0:
											  image_inner_index = 5;
											  break;
										  case 1:
											  image_inner_index = 9;
											  image_transform |= TRANS_MIRROR;
											  break;
										  case 2:
											  image_inner_index = 1;
											  break;
										  case 3:
											  image_inner_index = 9;
											  break;
										  }
									  }
									  else if (frame < 3 * TIMER_ACTOR_ATTACK_FRAME)
									  {
										  switch (hisActor->direction)
										  {
										  case 0:
											  image_inner_index = 6;
											  break;
										  case 1:
											  image_inner_index = 10;
											  image_transform |= TRANS_MIRROR;
											  break;
										  case 2:
											  image_inner_index = 2;
											  break;
										  case 3:
											  image_inner_index = 10;
											  break;
										  }
									  }
									  else
									  {
										  switch (hisActor->direction)
										  {
										  case 0:
											  image_inner_index = 7;
											  break;
										  case 1:
											  image_inner_index = 11;
											  image_transform |= TRANS_MIRROR;
											  break;
										  case 2:
											  image_inner_index = 3;
											  break;
										  case 3:
											  image_inner_index = 11;
											  break;
										  }
									  }
									  is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];
									  if (!is_top)
									  {
										  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											  arm_width, arm_width, image_transform,
											  (hisActor->xx() - history->scr_x +(image_width - arm_width) / 2),
											  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									  }
									  g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
										  image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
										  (hisActor->yy() - history->scr_y+fashion_offset));
									  if (is_top)
									  {
										  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											  arm_width, arm_width, image_transform,
											  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									  }
									  if (frame == 100)
									  {
										  setState(ACTOR_STATE_NORMAL);
									  }
	}
		break;
	case ACTOR_STATE_ATTACKED:
	{
								 //image = history->imgSpc[image_index];
								 image = history->imgFashion[iFSImgIndex][fashion_index + 2];
								 image_arm = history->imgFashionArms[iWFSImgIndex][2];
								 arm_width = arm_height = image_arm->width;
								 fashion_offset = (image_width - image->width) / 2;
								 fashion_width = image->width;
								 image_inner_index = 3;
								 is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

								 if (frame < 2 * TIMER_ACTOR_ATTACK_FRAME)
								 {
									 image_transform |= TRANS_WHITE_9;
								 }

								 if (!is_top)
								 {
									 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										 arm_width, arm_width, image_transform,
										 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								 }

								 g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
									 image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
									 (hisActor->yy() - history->scr_y + fashion_offset));

								 if (is_top)
								 {
									 g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										 arm_width, arm_width, image_transform,
										 (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										 (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								 }

								 if (frame == 100)
								 {
									 setState(ACTOR_STATE_NORMAL);
								 }
	}
		break;
	case ACTOR_STATE_DEADLY_ATTACK_PREPARE:
	{
											  //image_width = image_height = 48;
											  //image = history->imgAtc[image_index];
											  image = history->imgFashion[iFSImgIndex][fashion_index + 1];
											  image_arm = history->imgFashionArms[iWFSImgIndex][1];
											  arm_width = arm_height = image_arm->width;
											  fashion_offset = (image_width - image->width) / 2;
											  fashion_width = image->width;
											  switch (hisActor->direction)
											  {
											  case 0:
												  image_inner_index = 4;
												  break;
											  case 1:
												  image_inner_index = 8;
												  image_transform |= TRANS_MIRROR;
												  break;
											  case 2:
												  image_inner_index = 0;
												  break;
											  case 3:
												  image_inner_index = 8;
												  break;
											  }
											  is_top = trData->net_hero_dress[iWFSImgIndex]->mAtk[image_inner_index];
											  if (frame < 9)
											  {
												  image_transform |= 1 << (frame + 4);
												  if (!is_top)
												  {
													  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
														  arm_width, arm_width, image_transform,
														  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
														  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
												  }
												  g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
													  image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
													  (hisActor->yy() - history->scr_y+fashion_offset));
												  if (is_top)
												  {
													  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
														  arm_width, arm_width, image_transform,
														  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
														  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
												  }
											  }
											  else
											  {
												  image_transform |= 1 << (13);
												  if (!is_top)
												  {
													  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
														  arm_width, arm_width, image_transform,
														  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
														  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
												  }
												  g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
													  image_transform, (hisActor->xx() - history->scr_x+fashion_offset) ,
													  (hisActor->yy() - history->scr_y+fashion_offset));
												  if (is_top)
												  {
													  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
														  arm_width, arm_width, image_transform,
														  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
														  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
												  }
											  }
											  if (frame == 1)
											  {
												  if (trData->occu_types[hisActor->GetActorOccuType()].atk_music >= 0)
													  playSound(trData->occu_types[hisActor->GetActorOccuType()].atk_music);
											  }
											  else if (frame == TIMER_ACTOR_DEADLY_ATTACK_SOUND_DELAY)
											  {
												  playSound(SOUND_DEADLY_PREPARE_ATTACK);
											  }
											  if (frame == 100)
											  {
												  setState(ACTOR_STATE_NORMAL);
											  }
	}
		break;
	case ACTOR_STATE_GOOD_GIVE:
	{
								  if (frame == 1)
									  playSound(SOUND_GET_GOOD);
								  //image = history->imgSpc[image_index];
								  image = history->imgFashion[iFSImgIndex][fashion_index + 2];
								  image_arm = history->imgFashionArms[iWFSImgIndex][2];
								  arm_width = arm_height = image_arm->width;
								  fashion_offset = (image_width - image->width) / 2;
								  fashion_width = image->width;
								  image_inner_index = 4;
								  is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

								  if (!is_top)
								  {
									  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										  arm_width, arm_width, image_transform,
										  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								  }

								  g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
									  image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
									  (hisActor->yy() - history->scr_y+fashion_offset));
								  
								  if (is_top)
								  {
									  g->drawRegion(image_arm, 0, arm_width*image_inner_index,
										  arm_width, arm_width, image_transform,
										  (hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
										  (hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
								  }

								  if (frame < 20)
								  {
									  if (goodImg != NULL)
									  {
										  g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10,
											  (hisActor->yy() - history->scr_y) + 10 - frame, 1);
									  }

								  }
								  else
								  {
									  if (goodImg != NULL)
									  {
										  g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10,
											  (hisActor->yy() - history->scr_y) - 10, 1);
									  }
								  }
								  if (frame == TIMER_GOOD_GIVE_FRAME)
								  {
									  setState(ACTOR_STATE_NORMAL);
								  }
	}
		break;
	case ACTOR_STATE_CHANGE_FASHION:
	{
		if(frame == 1)
			playSound(SOUND_GET_GOOD);
		//image = history->imgSpc[image_index];
		image = history->imgFashion[iFSImgIndex][fashion_index + 2];
		image_arm = history->imgFashionArms[iWFSImgIndex][2];
		arm_width = arm_height = image_arm->width;
		fashion_offset = (image_width - image->width) / 2;
		fashion_width = image->width;
		image_inner_index = 4;
		is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

		if(frame > 25 && frame < 35)
		{
			image_transform |= 1 << (frame + 4);
		}
		else if(frame >= 35)
		{
			image_transform |= 1 << (13);
		}

		if(!is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}

		g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
			image_transform, (hisActor->xx() - history->scr_x + fashion_offset),
			(hisActor->yy() - history->scr_y + fashion_offset));

		if(is_top)
		{
			g->drawRegion(image_arm, 0, arm_width*image_inner_index,
				arm_width, arm_width, image_transform,
				(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
				(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
		}

		if(frame < 20)
		{
			if(goodImg != NULL)
			{
				g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10, (hisActor->yy() - history->scr_y) + 10 - frame, 1);
			}
		}

		if(frame == TIMER_ACTOR_CHANGE_FASHION)
		{
			setState(ACTOR_STATE_NORMAL);
		}
	}
	break;
	case ACTOR_STATE_GOOD_ACCEPT:
	{
									//image = history->imgSpc[image_index];
									image = history->imgFashion[iFSImgIndex][fashion_index + 2];
									image_arm = history->imgFashionArms[iWFSImgIndex][2];
									arm_width = arm_height = image_arm->width;
									fashion_offset = (image_width - image->width) / 2;
									fashion_width = image->width;
									image_inner_index = 4;
									is_top = trData->net_hero_dress[iWFSImgIndex]->mSpc[image_inner_index];

									if (!is_top)
									{
										g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											arm_width, arm_width, image_transform,
											(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									}

									g->drawRegion(image, 0, fashion_width*image_inner_index, fashion_width, fashion_width,
										image_transform, (hisActor->xx() - history->scr_x+fashion_offset),
										(hisActor->yy() - history->scr_y+fashion_offset));
									
									if (is_top)
									{
										g->drawRegion(image_arm, 0, arm_width*image_inner_index,
											arm_width, arm_width, image_transform,
											(hisActor->xx() - history->scr_x + (image_width - arm_width) / 2),
											(hisActor->yy() - history->scr_y + (image_width - arm_width) / 2));
									}

									image_transform |= 1 << (14 + frame / 4);
									if (goodImg != NULL)
									{
										g->drawImage(goodImg, (hisActor->xx() - history->scr_x) + 10,
											(hisActor->yy() - history->scr_y) - 10 + frame, image_transform);
									}

									if (frame == TIMER_GOOD_ACCEPT_FRAME)
									{
										setState(ACTOR_STATE_NORMAL);
									}
	}
		break;
	case ACTOR_STATE_RUN_ACTION:
		if (frame == m_need_frame)
		{
			setState(ACTOR_STATE_NORMAL);
		}
		break;
	}
}