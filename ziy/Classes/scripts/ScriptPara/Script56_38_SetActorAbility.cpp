#include "Common.h"
#include "Scripts.h"

// 0攻击力1防御力2精神力3爆发力4士气5HP6MP7当前HP 8当前MP 9 等级 10 武力 11统帅 12智力 13 敏捷 14运气 15 头像

int ScriptSetActorAbility::HandleScript() {
	HistoryActorPtr actor = &history->actors[actor_id];
	if (actor == NULL)
		return 0;

	switch (set_area) {
	case 0:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurpower(set_num);
			break;
		case 1:
			history->actors[actor_id].SetCurpower(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetCurpower(-1, -set_num);
			break;
		}
		break;
	case 1:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurDefence(set_num);
			break;
		case 1:
			history->actors[actor_id].SetCurDefence(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetCurDefence(-1, -set_num);
			break;
		}
		break;
	case 2:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurIntellegence(set_num);
			break;
		case 1:
			history->actors[actor_id].SetCurIntellegence(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetCurIntellegence(-1, -set_num);
			break;
		}
		break;
	case 3:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurExplosion(set_num);
			break;
		case 1:
			history->actors[actor_id].SetCurExplosion(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetCurExplosion(-1, -set_num);
			break;
		}
		break;
	case 4:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurFortune(set_num);
			break;
		case 1:
			history->actors[actor_id].SetCurFortune(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetCurFortune(-1, -set_num);
			break;
		}
		break;
	case 5:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBaseHp(set_num);
			break;
		case 1:
			history->actors[actor_id].SetBaseHp(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetBaseHp(-1, -set_num);
			break;
		}
		
		break;
	case 6:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBaseMp(set_num);
			break;
		case 1:
			if (history->actors[actor_id].GetCurMp() + set_num <= history->actors[actor_id].GetBaseMp() + history->actors[actor_id].GetGoodsAddMp()){
				history->actors[actor_id].SetCurMp(-1, set_num);
			}
			else{
				history->actors[actor_id].SetCurMp(history->actors[actor_id].GetBaseMp() + history->actors[actor_id].GetGoodsAddMp());
			}
			break;
		case 2:
			if (history->actors[actor_id].GetCurMp() - set_num >= 0){
				history->actors[actor_id].SetCurMp(-1, -set_num);
			}
			else{
				history->actors[actor_id].SetCurMp(0);
			}
			
			break;
		}
		break;
	case 7:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurHp(set_num);
			break;
		case 1:
			if (history->actors[actor_id].GetCurHp() + set_num <= history->actors[actor_id].GetBaseHp() + history->actors[actor_id].GetGoodsAddHp()){
				history->actors[actor_id].SetCurHp(-1, set_num);
			}
			else{
				history->actors[actor_id].SetCurHp(history->actors[actor_id].GetBaseHp() + history->actors[actor_id].GetGoodsAddHp());
			}
			break;
		case 2:
			if (history->actors[actor_id].GetCurHp() - set_num >= 0){
				history->actors[actor_id].SetCurHp(-1, -set_num);
			}
			else{
				history->actors[actor_id].SetCurHp(0);
			}
			break;
		}

		break;
	case 8:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetCurMp(set_num);
			break;
		case 1:
			history->actors[actor_id].SetCurMp(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetCurMp(-1, -set_num);
			break;
		}

		break;
	case 9:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetActorLv(set_num);
			break;
		case 1:
			history->actors[actor_id].SetActorLv(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetActorLv(-1, -set_num);
			break;
		}

		break;
	case 10:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBasepower(set_num);
			break;
		case 1:
			history->actors[actor_id].SetBasepower(-1,set_num);
			break;
		case 2:
			history->actors[actor_id].SetBasepower(-1, -set_num);
			break;
		}

		break;
	case 11:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBaseDefence(set_num);
			break;
		case 1:
			history->actors[actor_id].SetBaseDefence(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetBaseDefence(-1, -set_num);
			break;
		}

		break;
	case 12:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBaseIntellegence(set_num);
			break;
		case 1:
			history->actors[actor_id].SetBaseIntellegence(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetBaseIntellegence(-1, -set_num);
			break;
		}

		break;
	case 13:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBaseExplosion(set_num);
			break;
		case 1:
			history->actors[actor_id].SetBaseExplosion(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetBaseExplosion(-1, -set_num);
			break;
		}

		break;
	case 14:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].SetBaseFortune(set_num);
			break;
		case 1:
			history->actors[actor_id].SetBaseFortune(-1, set_num);
			break;
		case 2:
			history->actors[actor_id].SetBaseFortune(-1, -set_num);
			break;
		}

		break;
	case 15:
		switch (set_type)
		{
		case 0:
			history->actors[actor_id].face_id = set_num;
			break;
		case 1:
			history->actors[actor_id].face_id += set_num;
			break;
		case 2:
			history->actors[actor_id].face_id -= set_num;
			break;
		}

		break;
	}
	actor->force_save = 1;
	return 0;
}

int ScriptSetActorAbility::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set_area = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set_type = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set_num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
