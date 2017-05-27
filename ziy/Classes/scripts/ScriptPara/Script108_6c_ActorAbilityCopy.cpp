#include "Common.h"
#include "Scripts.h"

int ScriptActorAbilityCopy::HandleScript()
{
	history->actors[actor_id2].SetCurpower(history->actors[actor_id1].GetCurpower());
	history->actors[actor_id2].SetCurDefence(history->actors[actor_id1].GetCurDefence());
	history->actors[actor_id2].SetCurIntellegence(history->actors[actor_id1].GetCurIntellegence());
	history->actors[actor_id2].SetCurExplosion(history->actors[actor_id1].GetCurExplosion());
	history->actors[actor_id2].SetCurFortune(history->actors[actor_id1].GetCurFortune());
	history->actors[actor_id2].SetBaseHp(history->actors[actor_id1].GetBaseHp());
	history->actors[actor_id2].SetBaseMp(history->actors[actor_id1].GetBaseMp());
	history->actors[actor_id2].SetBasepower(history->actors[actor_id1].GetBasepower());
	history->actors[actor_id2].SetBaseDefence(history->actors[actor_id1].GetBaseDefence());
	history->actors[actor_id2].SetBaseIntellegence(history->actors[actor_id1].GetBaseIntellegence());
	history->actors[actor_id2].SetBaseExplosion(history->actors[actor_id1].GetBaseExplosion());
	history->actors[actor_id2].SetBaseFortune(history->actors[actor_id1].GetBaseFortune());
	history->actors[actor_id2].SetActorLv(history->actors[actor_id1].GetActorLv());
	history->actors[actor_id2].exp = history->actors[actor_id1].exp;
	history->actors[actor_id2].force_save = 1;	
	return 0;
}

int ScriptActorAbilityCopy::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
