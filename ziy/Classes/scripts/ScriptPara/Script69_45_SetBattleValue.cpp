#include "Common.h"
#include "Scripts.h"

int ScriptSetBattleValue::HandleScript() {
	CCLOG("====ScriptSetBattleValue=== maxRound = %d", maxRound);
	history->SetMaxRound(maxRound + history->iExtraAddMaxRound);
	history->enemy_skipper_index = enemy_skipper_index;
	if (air_type < 0)
	{
		air_type = 0;
	}
	if (start_air < 0)
	{
		start_air = 0;
	}
	history->m_iAirType = air_type;
	if(!history->mWeather.size())
	{
		history->updateWeather(history->m_iAirArray[air_type * 6 + start_air] + 1);
	}

	history->own_skipper_index = own_skipper_index;
	history->level_add = level_add;
	memset(history->var,0,256);


	//初始化used_skill_ids
	for (int i = 0; i < trData->actor_num; i++)
	{
		history->actors[i].used_skill_ids.resize(MAX_SKILL_NUM);
		history->actors[i].used_skill_ids.memset(0);
	}


	return 0;
}

int ScriptSetBattleValue::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	nouse1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	nouse2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	maxRound =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	level_add =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	nouse3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown6 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	enemy_skipper_index =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown7 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	nouse4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown8 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	own_skipper_index =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown9 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	air_type =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown10 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	start_air =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
