#include "Common.h"
#include "main.h"
#include "PKScreen.h"
#include "button.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "Defines.h"

enum
{
	PK_ACTOR_STATE_NORMAL,
	PK_ACTOR_STATE_TURN,
	PK_ACTOR_STATE_FORWARD,
	PK_ACTOR_STATE_MINI_FORWARD,
	PK_ACTOR_STATE_MINI_BACKWARD,
	PK_ACTOR_STATE_CELEBRATE,
	PK_ACTOR_STATE_DEFENCE,
	PK_ACTOR_STATE_ATTACKED,
	PK_ACTOR_STATE_ATTACK_PREPARE,
	PK_ACTOR_STATE_ATTACK,
	PK_ACTOR_STATE_ATTACK_TWICE,
	PK_ACTOR_STATE_FAINT,
	PK_ACTOR_STATE_BREATHE,
	PK_ACTOR_STATE_RUN_AWAY,
	PK_ACTOR_STATE_DANCE1,
	PK_ACTOR_STATE_DANCE2,
	PK_ACTOR_STATE_DEADLY_ATTACK,
	PK_ACTOR_STATE_MOVE_ATTACK,
	PK_ACTOR_STATE_ATTACK_DEF,
	PK_ACTOR_STATE_DEADLY_ATTACK_DEF,
	PK_ACTOR_STATE_DEADLY_ATTACK_HITTED,
	PK_ACTOR_STATE_ATTACK_HITTED,
	PK_ACTOR_STATE_ASSAULT,
	PK_ACTOR_STATE_APPEAR,
	PK_ACTOR_STATE_DEF_BAC, //格挡后退
	PK_ACTOR_STATE_BAC, //后退
	PK_ACTOR_STATE_BAC_TURN,//躲闪绕前
	PK_ACTOR_STATE_HIDE,
	PK_ACTOR_STATE_MAX
};
enum
{
	PK_NORMAL,
	PK_START,
	PK_ACTOR_APPEAR,
	PK_SHOW_K_O,
	PK_ACTOR_DIE,
	PK_ACTOR_TALK,
	PK_ACTION
};
enum
{
	CTRL_TALK_LABEL
};

bool PKLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	Size size = Director::getInstance()->getWinSize();
	Vec2 center = Vec2(size.width / 2, size.height - 75);
	m_pSpriteVS = ReadSpriteName("new_ui/img_global/pk_vs.png");
	if(m_pSpriteVS != NULL)
	{
		m_pSpriteVS->setPosition(center);
		this->addChild(m_pSpriteVS);
	}
	return true;
}

void PKLayer::RunSpriteAction()
{
	m_pSpriteVS->setScale(3.0f);
	ScaleTo* scale1 = ScaleTo::create(0.2f, 0.8f);
	ScaleTo* scale2 = ScaleTo::create(0.05f, 1.0f);
	m_pSpriteVS->runAction(Sequence::create(scale1, scale2, NULL));
	MoveBy* move1 = MoveBy::create(0.016f, Vec2(2, 0));
	MoveBy* move2 = MoveBy::create(0.032f, Vec2(-4, 0));
	MoveBy* move3 = MoveBy::create(0.016f, Vec2(2, 0));
	m_pSpriteVS->runAction(Repeat::create(Sequence::create(move1, move2, move3, NULL), 12));
}

cocos2d::Vec2 PKLayer::GetVSPosition()
{
	return m_pSpriteVS->getPosition();
}

PKActor::PKActor(HistoryActorPtr actor, int relation)
{
	this->frame = 0;
	this->direction = relation;
	this->state = PK_ACTOR_STATE_HIDE;
	this->is_dead = 0;
	this->hisActor = actor;
	this->relation = relation;
	this->delay = 0;
	this->iScale = 2.5f;
	imgMov = history->GetActorScaleImage(hisActor->actor_id, "mov", iScale);
	imgAtk = history->GetActorScaleImage(hisActor->actor_id, "atk", iScale);
	imgSpc = history->GetActorScaleImage(hisActor->actor_id, "spc", iScale);
}
void PKActor::onPaint(DGraphics* g)
{
	int image_index = history->getBattleActorImageId(hisActor->actor_id);
	int image_inner_index;
	int image_transform = TRANS_NORMAL;
	int image_width = SCENE_TILE_WIDTH;
	int image_height = SCENE_TILE_WIDTH;
	TRImagePtr imgCur = NULL;

	if(hisActor->history_image_fight_id <= 100 && hisActor->image_change_occu_id<0)
		image_index += hisActor->relation;
	else if(hisActor->history_image_fight_id <= 100 && hisActor->image_change_occu_id>0 && hisActor->image_change_occu_id <= 7)
		image_index += hisActor->image_change_occu_id - 1;
	if(direction == 0)
		image_transform = TRANS_MIRROR;
	if(is_dead)
	{
		image_transform |= TRANS_DIE;
	}
	switch(state)
	{
	case PK_ACTOR_STATE_TURN:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		imgCur = imgMov;
		image_inner_index = 8;
		break;
	case PK_ACTOR_STATE_FORWARD:
		imgCur = imgMov;
		if(frame < 6)
		{
			if(frame == 0)
			{
				switch(trData->occupations[hisActor->occupation_id].musicId)
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
			if(frame % 6 < 3)
				image_inner_index = 4;
			else
				image_inner_index = 5;
			if(oppPKActor->x > this->x)
			{
				this->x += 16;
			}
			else
			{
				this->x -= 16;
			}

		}
		else
		{
			image_inner_index = 5;
		}
		break;
	case PK_ACTOR_STATE_MINI_FORWARD:
		imgCur = imgMov;
		if(frame < 8)
		{
			if(frame == 0)
			{
				switch(trData->occupations[hisActor->occupation_id].musicId)
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
			if(frame<4)
				image_inner_index = 4;
			else
				image_inner_index = 5;
			if(oppPKActor->x > this->x)
			{
				this->x += 4;
			}
			else
			{
				this->x -= 4;
			}
		}
		else
		{
			image_inner_index = 5;
		}

		break;
	case PK_ACTOR_STATE_MINI_BACKWARD:
		imgCur = imgMov;
		if(frame < 8)
		{
			if(frame == 0)
			{
				switch(trData->occupations[hisActor->occupation_id].musicId)
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
			if(frame<4)
				image_inner_index = 4;
			else
				image_inner_index = 5;
			if(oppPKActor->x > this->x)
			{
				this->x -= 4;
			}
			else
			{
				this->x += 4;
			}
		}
		else
		{
			image_inner_index = 5;
		}
		break;
	case PK_ACTOR_STATE_CELEBRATE:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		imgCur = imgSpc;
		image_inner_index = 4;
		break;
	case PK_ACTOR_STATE_DEFENCE:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		imgCur = imgSpc;
		image_inner_index = 2;
		break;
	case PK_ACTOR_STATE_ATTACKED:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		imgCur = imgSpc;
		image_inner_index = 3;
		break;
	case PK_ACTOR_STATE_ATTACK_PREPARE:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		image_width = image_height = 64;
		imgCur = imgAtk;
		image_inner_index = 8;
		break;
	case PK_ACTOR_STATE_ATTACK:
		image_width = image_height = 64;
		imgCur = imgAtk;
		if(frame < 8)
			image_inner_index = frame / 2 + 8;
		else
			image_inner_index = 11;
		break;
	case PK_ACTOR_STATE_ATTACK_TWICE:
		image_width = image_height = 64;
		imgCur = imgAtk;
		if(frame < 16)
			image_inner_index = (frame % 8) / 2 + 8;
		else
			image_inner_index = 11;
		break;
	case PK_ACTOR_STATE_FAINT:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		imgCur = imgMov;
		image_inner_index = 9;
		break;
	case PK_ACTOR_STATE_BREATHE:
		imgCur = imgMov;
		if(frame % (TIMER_ACTOR_CHANGE_STYLE_FRAME * 2) < TIMER_ACTOR_CHANGE_STYLE_FRAME)
		{
			image_inner_index = 9;
		}
		else
		{
			image_inner_index = 10;
		}
		break;
	case PK_ACTOR_STATE_RUN_AWAY:
		break;
	case PK_ACTOR_STATE_DANCE1:
		break;
	case PK_ACTOR_STATE_DANCE2:
		break;
	case PK_ACTOR_STATE_DEADLY_ATTACK:
		if(frame == 1)
		{
			playSound(SOUND_DEADLY_PREPARE_ATTACK);
		}
		image_width = image_height = 64;
		imgCur = imgAtk;
		if(frame < 9)
		{
			image_inner_index = 8;
			image_transform |= 1 << (frame + 4);
		}
		else
		{
			if(frame < 17)
				image_inner_index = (frame - 9) / 2 + 8;
			else
				image_inner_index = 11;
		}
		break;
	case PK_ACTOR_STATE_MOVE_ATTACK:
		if(frame<6)
		{
			image_width = image_height = 64;
			imgCur = imgAtk;
			image_inner_index = (frame) / 6 + 8;
			if(direction == 0)
			{
				if(oppPKActor->x > this->x)
				{
					this->x += 32;
				}
				else
				{
					this->x -= 32;
				}
				for(int i = 3; i > 0; i--)
				{
					g->drawRegion(imgCur, 0, image_height*image_inner_index * iScale, image_width * iScale, image_height * iScale, image_transform, x - (image_width - SCENE_TILE_WIDTH) / 2 - 20 * i, y - (image_height - SCENE_TILE_WIDTH));
				}
			}
			else
			{
				if(oppPKActor->x > this->x)
				{
					this->x += 32;
				}
				else
				{
					this->x -= 32;
				}
				for(int i = 3; i > 0; i--)
				{
					g->drawRegion(imgCur, 0, image_height*image_inner_index * iScale, image_width * iScale, image_height * iScale, image_transform, x - (image_width - SCENE_TILE_WIDTH) / 2 + 20 * i, y - (image_height - SCENE_TILE_WIDTH));
				}
			}
		}
		else
		{
			image_width = image_height = 64;
			imgCur = imgAtk;
			image_inner_index = 11;
		}
		break;
	case PK_ACTOR_STATE_ATTACK_DEF:
		if(frame == 1)
		{
			playSound(SOUND_ATTACK_NO_HIT);
		}
		if(frame == 4)
		{
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}
		else if(frame == 8)
		{
			if(oppPKActor->x > this->x)
			{
				this->x += 16;
			}
			else
			{
				this->x -= 16;
			}
		}
		imgCur = imgSpc;
		image_inner_index = 2;
		break;
	case PK_ACTOR_STATE_DEADLY_ATTACK_DEF:
		if(frame == 1)
		{
			playSound(SOUND_DEADLY_ATTACK_NO_HIT);
		}
		if(frame == 4)
		{
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}
		else if(frame == 8)
		{
			if(oppPKActor->x > this->x)
			{
				this->x += 16;
			}
			else
			{
				this->x -= 16;
			}
		}
		imgCur = imgSpc;
		image_inner_index = 2;
		break;
	case PK_ACTOR_STATE_ATTACK_HITTED:
		if(frame == 1)
		{
			playSound(SOUND_ATTACK_HIT);
		}
		if(frame == 4)
		{
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}
		else if(frame == 8)
		{
			if(oppPKActor->x > this->x)
			{
				this->x += 16;
			}
			else
			{
				this->x -= 16;
			}
		}
		imgCur = imgSpc;
		image_inner_index = 3;
		break;
	case PK_ACTOR_STATE_DEADLY_ATTACK_HITTED:
		if(frame == 1)
		{
			playSound(SOUND_DEAD_ATTACK_HIT);
		}
		if(frame == 4)
		{
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}
		else if(frame == 8)
		{
			if(oppPKActor->x > this->x)
			{
				this->x += 16;
			}
			else
			{
				this->x -= 16;
			}
		}
		imgCur = imgSpc;
		image_inner_index = 3;
		break;
	case PK_ACTOR_STATE_ASSAULT:
		imgCur = imgMov;
		if(frame < 12)
		{
			if(frame % 6 < 3)
				image_inner_index = 4;
			else
				image_inner_index = 5;
			if(direction == 0)
				this->x += 24;
			else
				this->x -= 24;
			if(frame == 6)
			{
				playSound(SOUND_ATTACK_NO_HIT);
			}
		}
		else
		{
			image_inner_index = 5;
		}
		break;

	case PK_ACTOR_STATE_NORMAL:
		if(frame == 1)
		{
			playSound(SOUND_FAINT);
		}
		imgCur = imgMov;
		image_inner_index = 8;
		break;
	case PK_ACTOR_STATE_APPEAR:
		imgCur = imgMov;
		image_inner_index = 8;
		if(frame < 18)
			image_transform |= 1 << (13 - (frame / 2));
		break;
	case PK_ACTOR_STATE_DEF_BAC://格挡后退
		if(frame == 1)
		{
			playSound(SOUND_ATTACK_NO_HIT);
		}
		if(frame<3)
		{
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}

		imgCur = imgSpc;
		image_inner_index = 2;
		break;
	case PK_ACTOR_STATE_BAC: //后退
		imgCur = imgMov;
		if(frame < 6)
		{
			if(frame % 6 < 3)
				image_inner_index = 4;
			else
				image_inner_index = 5;
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}
		else
		{
			image_inner_index = 5;
		}
		break;
	case PK_ACTOR_STATE_BAC_TURN://躲闪绕前
		imgCur = imgMov;
		if(frame < 6)
		{
			if(frame % 6 < 3)
				image_inner_index = 4;
			else
				image_inner_index = 5;
			if(oppPKActor->x > this->x)
			{
				this->x -= 16;
			}
			else
			{
				this->x += 16;
			}
		}
		else
		{
			if(frame == 6)
			{
				if(oppPKActor->x > this->x)
				{
					this->x = oppPKActor->x + 100;
					direction = 1;
				}
				else
				{
					this->x = oppPKActor->x - 100;
					direction = 0;
				}
				if(this->x < 0)
				{
					this->x = 0;
				}
			}
			image_inner_index = 5;
		}
		break;
	case PK_ACTOR_STATE_HIDE:
		break;
	}
	if(imgCur)
	{
		g->drawRegion(imgCur, 0, image_height*image_inner_index * iScale, image_width * iScale, image_height * iScale, image_transform, x - (image_width - SCENE_TILE_WIDTH) / 2, y - (image_height - SCENE_TILE_WIDTH));
	}
	frame++;
}
int PKActor::setState(int state)
{
	int need_frame = 0;
	this->frame = 0;
	this->state = state;
	switch(state)
	{
	case PK_ACTOR_STATE_NORMAL:
		break;
	case PK_ACTOR_STATE_TURN:
		if(this->direction == 0)
			this->direction = 1;
		else
			this->direction = 0;
		break;
	case PK_ACTOR_STATE_FORWARD:
		need_frame = 6;
		break;
	case PK_ACTOR_STATE_MINI_FORWARD:
		need_frame = 8;
		break;
	case PK_ACTOR_STATE_MINI_BACKWARD:
		need_frame = 20;
		break;
	case PK_ACTOR_STATE_CELEBRATE:
		break;
	case PK_ACTOR_STATE_DEFENCE:
		break;
	case PK_ACTOR_STATE_ATTACKED:
		break;
	case PK_ACTOR_STATE_ATTACK_PREPARE:
		break;
	case PK_ACTOR_STATE_ATTACK:
		need_frame = 10;
		break;
	case PK_ACTOR_STATE_ATTACK_TWICE:
		need_frame = 18;
		break;
	case PK_ACTOR_STATE_FAINT:
		break;
	case PK_ACTOR_STATE_BREATHE:
		need_frame = TIMER_ACTOR_CHANGE_STYLE_FRAME * 4;
		break;
	case PK_ACTOR_STATE_RUN_AWAY:
		break;
	case PK_ACTOR_STATE_DANCE1:
		break;
	case PK_ACTOR_STATE_DANCE2:
		break;

	case PK_ACTOR_STATE_DEADLY_ATTACK:
		need_frame = 20;
		break;
	case PK_ACTOR_STATE_MOVE_ATTACK:
		need_frame = 6;
		break;
	case PK_ACTOR_STATE_ATTACK_DEF:
		need_frame = 10;
		break;
	case PK_ACTOR_STATE_ATTACK_HITTED:
		break;
	case PK_ACTOR_STATE_ASSAULT:
		need_frame = 12;
		break;

	case PK_ACTOR_STATE_APPEAR:
		need_frame = 50;
		break;

	case PK_ACTOR_STATE_DEF_BAC://格挡后退
		break;
	case PK_ACTOR_STATE_BAC: //后退
		break;
	case PK_ACTOR_STATE_BAC_TURN://躲闪绕前
		break;
	case PK_ACTOR_STATE_HIDE:
		break;
	}
	return need_frame;
}

void PKActor::setOppPKActor(PKActor* oppActor)
{
	oppPKActor = oppActor;
}

PKActor::~PKActor()
{
	SAFE_DELETE_IMAGE(imgMov);
	SAFE_DELETE_IMAGE(imgAtk);
	SAFE_DELETE_IMAGE(imgSpc);
}

void PKScreen::PKActorAppear(int relation, int action, DString info)
{
	this->pk_state = PK_ACTOR_APPEAR;
	this->talk_buf = info;
	if(relation == 0)
	{
		cur_actor = actor_oppo;
	}
	else
	{
		cur_actor = actor_own;
	}
	this->need_frame1 = cur_actor->setState(action);
	this->need_frame = need_frame1 + cur_actor->setState(PK_ACTOR_STATE_APPEAR);
	this->action1 = action;
}
//命中，格挡，格挡后退，后退，躲闪绕前
void PKScreen::PKActorAttack1(short relation, short attack_type, short is_deadly)
{
	pk_state = PK_ACTION;
	if(relation == 0)
	{
		cur_actor = actor_oppo;
		other_actor = actor_own;
	}
	else
	{
		cur_actor = actor_own;
		other_actor = actor_oppo;
	}
	if(is_deadly)
	{
		need_frame = cur_actor->setState(PK_ACTOR_STATE_DEADLY_ATTACK);
		other_actor->delay = 10;
	}
	else
	{
		need_frame = cur_actor->setState(PK_ACTOR_STATE_ATTACK);
	}
	switch(attack_type)
	{
	case 0:
	{
		if(is_deadly)
			other_actor->setState(PK_ACTOR_STATE_DEADLY_ATTACK_HITTED);
		else
			other_actor->setState(PK_ACTOR_STATE_ATTACK_HITTED);
	}
		break;
	case 1:
		other_actor->setState(PK_ACTOR_STATE_ATTACK_DEF);
		break;
	case 2:
		other_actor->setState(PK_ACTOR_STATE_DEF_BAC);
		break;
	case 3:
		other_actor->setState(PK_ACTOR_STATE_BAC);
		break;
	case 4:
		other_actor->setState(PK_ACTOR_STATE_BAC_TURN);
		break;
	}
}
//0原地攻击 1 移动攻击 2相互冲锋
void PKScreen::PKActorAttack2(short relation, short attack_type, short is_hitted)
{
	pk_state = PK_ACTION;
	if(relation == 0)
	{
		cur_actor = actor_oppo;
		other_actor = actor_own;
	}
	else
	{
		cur_actor = actor_own;
		other_actor = actor_oppo;
	}
	switch(attack_type)
	{
	case 0:
		need_frame = cur_actor->setState(PK_ACTOR_STATE_ATTACK);
		if(is_hitted)
			other_actor->setState(PK_ACTOR_STATE_ATTACK_DEF);
		else
			other_actor->setState(PK_ACTOR_STATE_ATTACK_HITTED);
		break;
	case 1:
		need_frame = cur_actor->setState(PK_ACTOR_STATE_MOVE_ATTACK);
		//other_actor->delay = 20;
		if(is_hitted)
			other_actor->setState(PK_ACTOR_STATE_ATTACK_DEF);
		else
			other_actor->setState(PK_ACTOR_STATE_ATTACK_HITTED);
		break;
	case 2:
		need_frame = actor_own->setState(PK_ACTOR_STATE_ASSAULT);
		actor_oppo->setState(PK_ACTOR_STATE_ASSAULT);
		break;
	}
}
void PKScreen::PKActorDie(short relation)
{
	pk_state = PK_ACTION;
	if(relation == 1)
	{
		cur_actor = actor_oppo;
	}
	else
	{
		cur_actor = actor_own;
	}
	cur_actor->is_dead = 1;
	need_frame = 10;
}
void PKScreen::PKActorAction(int relation, int action)
{
	pk_state = PK_ACTION;
	if(relation == 0)
	{
		cur_actor = actor_oppo;
	}
	else
	{
		cur_actor = actor_own;
	}
	need_frame = cur_actor->setState(action);
}
void PKScreen::PKActorTalk(short relation, DString buf, short delay)
{

}
void PKScreen::PKEnd()
{
	this->battle_scr->handler.battle_state = pre_state;
	removeScreen(this, 1);
	ChessPlayMp3(history->bg_music_index);
}
void PKScreen::PKShowKO()
{
	pk_state = PK_SHOW_K_O;
	need_frame = 30;
}

void PKScreen::onPaint(DGraphics* g)
{
	int src_x = (this->rect.width - this->m_pScreenBG->width) / 2;
	int src_y = (this->rect.height - this->m_pScreenBG->height) / 2;
	g->drawImage(this->m_pScreenBG, rect.x + src_x, rect.y + src_y, 0); //背景图

	int iLeftX = 100; // 左右武将头像的中心坐标
	int iRightX = this->rect.width - iLeftX;
	int iFaceY = rect.y + 5;

	g->drawImage(face_oppo, iLeftX - face_oppo->width / 2, iFaceY, actor_oppo->is_dead ? 1 | TRANS_DIE : 1);
	g->drawImage(nameLabel, iLeftX - nameLabel->width / 2, iFaceY + face_oppo->height - 5);

	g->drawImage(face_own, iRightX - face_own->width / 2, iFaceY, actor_own->is_dead ? 2 | TRANS_DIE : 2);
	g->drawImage(nameLabel, iRightX - nameLabel->width / 2, iFaceY + face_own->height - 5);

	g->setBgColor_RGB(0xff);
	g->setColor_RGB(0xffffff);
	g->setFont(Chess_FONT_SONG_BORDER_16);
	DString strName = trData->actors[actor_oppo->hisActor->actor_id].getActorName();
	int iLength = g->getStringWidth(strName->getChar());
	g->drawString(strName->getChar(), iLeftX - iLength / 2, iFaceY + face_oppo->height + 7);

	g->setBgColor_RGB(0xff0000);
	strName = trData->actors[actor_own->hisActor->actor_id].getActorName();
	iLength = g->getStringWidth(strName->getChar());
	g->drawString(strName->getChar(), iRightX - iLength / 2, iFaceY + face_own->height + 7);
	g->resetFont();

	if(pk_state == PK_START)
	{
		if(need_frame == 1)
		{
			Vec2 vspos = m_pPKLayer->GetVSPosition();
			iLastVsX = vspos.x - this->imgVS->width / 2 + 1;
			iLastVsY = this->rect.height - vspos.y - this->imgVS->height / 2;
			m_pPKLayer->m_pSpriteVS->setVisible(false);
			g->drawImage(imgVS, iLastVsX, iLastVsY);
		}
	}
	else
	{
		g->drawImage(imgVS, iLastVsX, iLastVsY);
		actor_own->onPaint(g);
		actor_oppo->onPaint(g);
	}

	Screen::onPaint(g);
}
void PKScreen::handleTimer()
{
	switch(pk_state)
	{
	case PK_NORMAL:
		battle_scr->handler.handler.RunScript();
		break;
	case PK_ACTION:
	case PK_START:
	case PK_SHOW_K_O:
		if(need_frame-- == 0)
		{
			pk_state = PK_NORMAL;
			battle_scr->handler.handler.RunScript();
		}
		break;
	case PK_ACTOR_APPEAR:
		need_frame--;
		if(need_frame == need_frame1)
		{
			cur_actor->setState(action1);
		}
		if(need_frame == 0)
		{
			pk_state = PK_ACTOR_TALK;
			if(talk_buf == "")
			{
				talk_buf = (char*)NULL;
				pk_state = PK_NORMAL;
			}
			else
			{

			}
		}
		break;
	case PK_ACTOR_TALK:
	{
		pk_state = PK_NORMAL;
		battle_scr->handler.handler.RunScript();
	}
		break;
	}
}
void PKScreen::onMessage(MessagePtr message)
{

}

PKScreen::PKScreen(int actor_id1, int actor_id2, int logoIndex) : Screen(SCREEN_PK)
{
	this->m_pScreenBG = CreateImage(ChessSprintf("%s%d", "img/Pkbg/logo", logoIndex + 1), 1);
	if(!m_pScreenBG)
	{
		m_pScreenBG = CreateImage("img/Pkbg/logo1", 1);
	}
	this->nameLabel = CreateGlobalImageFile("new_ui/img_global/pk_boxbg1.png");
	this->imgVS = CreateGlobalImageFile("new_ui/img_global/pk_vs.png");

	this->talk_buf = (char*)NULL;

	rect.x = 0;
	rect.y = 0;
	rect.width = screen_size_x;
	rect.height = screen_size_y;

	this->battle_scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	pre_state = this->battle_scr->handler.battle_state;
	this->battle_scr->handler.battle_state = BATTLE_STATE_PK;
	this->need_frame = 0;
	this->need_frame1 = 0;
	//////////////////////////////////////////////////////////////
	//PK_START
	this->pk_state = PK_START;
	this->need_frame = 60;
	this->iLastVsX = 0;
	this->iLastVsY = 0;

	if(history->actors[actor_id1].relation == ACTOR_RELATION_ENEMY)
	{
		actor_oppo = new PKActor(&history->actors[actor_id1], 0);
		actor_own = new PKActor(&history->actors[actor_id2], 1);
	}
	else
	{
		actor_oppo = new PKActor(&history->actors[actor_id2], 0);
		actor_own = new PKActor(&history->actors[actor_id1], 1);
	}
	actor_oppo->setOppPKActor(actor_own);
	actor_own->setOppPKActor(actor_oppo);
	actor_oppo->y = rect.y + rect.height / 2 + 20;
	actor_own->y = rect.y + rect.height / 2 + 20;
	actor_oppo->x = rect.x + rect.width / 2 - SCENE_TILE_WIDTH - 2 * SCENE_TILE_WIDTH;
	actor_own->x = rect.x + rect.width / 2 + SCENE_TILE_WIDTH;
	this->face_own = history->GetFaceImage(actor_own->hisActor->actor_id);
	this->face_oppo = history->GetFaceImage(actor_oppo->hisActor->actor_id);

	m_pPKLayer = PKLayer::create();
	ChessApplication::getInstance()->AddFunctionLayer(m_pPKLayer);
	m_pPKLayer->RunSpriteAction();

	ChessPlayMp3(10);
}

PKScreen::~PKScreen()
{
	m_pPKLayer->removeFromParent();

	SAFE_DELETE_IMAGE(this->m_pScreenBG);
	SAFE_DELETE_IMAGE(this->nameLabel);
	SAFE_DELETE_IMAGE(this->imgVS);
	SAFE_DELETE(actor_oppo);
	SAFE_DELETE(actor_own);
}