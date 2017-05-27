#include "Common.h"
#include "Scripts.h"

int ScriptConfigOwnForce::scene_flag = -1;

int ScriptConfigOwnForce::HandleScript()
{
	if (out_actor_index1 < -1)//变量武将
	{
		out_actor_index1_replace = history->var2[(int)abs(out_actor_index1) - 2];
	}
	else
	{
		out_actor_index1_replace = out_actor_index1;
	}
	if (out_actor_index2 < -1)//变量武将
	{
		out_actor_index2_replace = history->var2[(int)abs(out_actor_index2) - 2];
	}
	else
	{
		out_actor_index2_replace = out_actor_index2;
	}
	if (out_actor_index3 < -1)//变量武将
	{
		out_actor_index3_replace = history->var2[(int)abs(out_actor_index3) - 2];
	}
	else
	{
		out_actor_index3_replace = out_actor_index3;
	}
	if (out_actor_index4 < -1)//变量武将
	{
		out_actor_index4_replace = history->var2[(int)abs(out_actor_index4) - 2];
	}
	else
	{
		out_actor_index4_replace = out_actor_index4;
	}
	if (out_actor_index5 < -1)//变量武将
	{
		out_actor_index5_replace = history->var2[(int)abs(out_actor_index5) - 2];
	}
	else
	{
		out_actor_index5_replace = out_actor_index5;
	}
	if (in_actor_index1 < -1)//变量武将
	{
		in_actor_index1_replace = history->var2[(int)abs(in_actor_index1) - 2];
	}
	else
	{
		in_actor_index1_replace = in_actor_index1;
	}
	if (in_actor_index2 < -1)//变量武将
	{
		in_actor_index2_replace = history->var2[(int)abs(in_actor_index2) - 2];
	}
	else
	{
		in_actor_index2_replace = in_actor_index2;
	}
	if (in_actor_index3 < -1)//变量武将
	{
		in_actor_index3_replace = history->var2[(int)abs(in_actor_index3) - 2];
	}
	else
	{
		in_actor_index3_replace = in_actor_index3;
	}
	if (in_actor_index4 < -1)//变量武将
	{
		in_actor_index4_replace = history->var2[(int)abs(in_actor_index4) - 2];
	}
	else
	{
		in_actor_index4_replace = in_actor_index4;
	}
	if (in_actor_index5 < -1)//变量武将
	{
		in_actor_index5_replace = history->var2[(int)abs(in_actor_index5) - 2];
	}
	else
	{
		in_actor_index5_replace = in_actor_index5;
	}
	int i, j = 0;
	if (scene_flag != history->game_scene)
	{
		history->battleOwnNum = 0;
		scene_flag = history->game_scene;
	}
	j = history->battleOwnNum;

	if (out_actor_index1_replace >= 0 && out_actor_index1_replace < trData->actor_num)
	{
		if (history->AddBattleOwn(out_actor_index1_replace) != 0)
			j++;
	}
	if (out_actor_index2_replace >= 0 && out_actor_index2_replace < trData->actor_num)
	{
		if (history->AddBattleOwn(out_actor_index2_replace) != 0)
			j++;
	}
	if (out_actor_index3_replace >= 0 && out_actor_index3_replace < trData->actor_num)
	{
		if (history->AddBattleOwn(out_actor_index3_replace) != 0)
			j++;
	}
	if (out_actor_index4_replace >= 0 && out_actor_index4_replace < trData->actor_num)
	{
		if (history->AddBattleOwn(out_actor_index4_replace) != 0)
			j++;
	}
	if (out_actor_index5_replace >= 0 && out_actor_index5_replace < trData->actor_num)
	{
		if (history->AddBattleOwn(out_actor_index5_replace) != 0)
			j++;
	}
	for (i = 0; i < history->ownArmyNum && j < num; i++)
	{
		if ((history->ownArmy[i] != out_actor_index1_replace)
			&& (history->ownArmy[i] != out_actor_index2_replace)
			&& (history->ownArmy[i] != out_actor_index3_replace)
			&& (history->ownArmy[i] != out_actor_index4_replace)
			&& (history->ownArmy[i] != out_actor_index5_replace)
			&& (history->ownArmy[i] != in_actor_index1_replace)
			&& (history->ownArmy[i] != in_actor_index2_replace)
			&& (history->ownArmy[i] != in_actor_index3_replace)
			&& (history->ownArmy[i] != in_actor_index4_replace)
			&& (history->ownArmy[i] != in_actor_index5_replace)
			&& j < num)
		{
			if (history->AddBattleOwn(history->ownArmy[i]) != 0)
				j++;
		}
	}
	history->CalcBaseLevel();
	return 0;
}

int ScriptConfigOwnForce::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_actor_index1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_actor_index2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_actor_index3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_actor_index4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_actor_index5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	in_actor_index1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	in_actor_index2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown9 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	in_actor_index3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown10 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	in_actor_index4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown11 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	in_actor_index5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
