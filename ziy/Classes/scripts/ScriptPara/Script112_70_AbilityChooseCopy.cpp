#include "Common.h"
#include "Scripts.h"

int ScriptAbilityChooseCopy::HandleScript() 
{
	switch (copy_what)
	{
	case 17:  //攻击力
	case 18:
		history->actors[actor_id2].SetCurpower(history->actors[actor_id1].GetCurpower());
		break;
	case 19:  //防御力
	case 20:
		history->actors[actor_id2].SetCurDefence(history->actors[actor_id1].GetCurDefence());
		break;
	case 21:  //精神力
	case 22:
		history->actors[actor_id2].SetCurIntellegence(history->actors[actor_id1].GetCurIntellegence());
		break;
	case 23:  //爆发力
	case 24:
		history->actors[actor_id2].SetCurExplosion(history->actors[actor_id1].GetCurExplosion());
		break;
	case 25:  //士气
	case 26:
		history->actors[actor_id2].SetCurFortune(history->actors[actor_id1].GetCurFortune());
		break;
	case 33:  //武力
		history->actors[actor_id2].SetBasepower(history->actors[actor_id1].GetBasepower());
		break;
	case 34:  //统率
		history->actors[actor_id2].SetBaseDefence(history->actors[actor_id1].GetBaseDefence());
		break;
	case 35:  //智力
		history->actors[actor_id2].SetBaseIntellegence(history->actors[actor_id1].GetBaseIntellegence());
		break;
	case 36:  //敏捷
		history->actors[actor_id2].SetBaseExplosion(history->actors[actor_id1].GetBaseExplosion());
		break;
	case 37:  //运气
		history->actors[actor_id2].SetBaseFortune(history->actors[actor_id1].GetBaseFortune());
		break;
	case 46:  //武器
		history->actors[actor_id2].weapon = history->actors[actor_id1].weapon;
		break;
	case 47:  //武器等级
		history->actors[actor_id2].weapon.level = history->actors[actor_id1].weapon.level;
		break;
	case 48:  //武器经验
		history->actors[actor_id2].weapon.exp = history->actors[actor_id1].weapon.exp;
		break;
	case 49:  //防具
		history->actors[actor_id2].coat = history->actors[actor_id1].coat;
		break;
	case 50:  //防具等级
		history->actors[actor_id2].coat.level = history->actors[actor_id1].coat.level;
		break;
	case 51:  //防具经验
		history->actors[actor_id2].coat.exp = history->actors[actor_id1].coat.exp;
		break;
	case 52:  //辅助
		history->actors[actor_id2].assist_good = history->actors[actor_id1].assist_good;
		break;
	}
	history->actors[actor_id2].force_save = 1;
	return 0;
}

int ScriptAbilityChooseCopy::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	copy_what = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
