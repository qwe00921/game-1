#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptForceLimit::HandleRScript(RedeScreenPtr scr)
{
	if(max_num > MAX_FORCE_LIMIT)  //出战总人数的限制最大16人
	{
		max_num = MAX_FORCE_LIMIT;
	}

	if(history->var2[5004] == 1 && history->var2[5005] > MAX_FORCE_LIMIT)
	{
		max_num = history->var2[5005];
	}

	if (limit && max_num > 0 && max_num < history->ownArmyNum)
	{
		history->max_amy_num = max_num;
	}
	else
	{
		history->max_amy_num = history->ownArmyNum;
	}

	history->max_amy_num = max_num;  //此处将出场人数限制设为指令指定人数，出兵时会再次比较出场人数限制和我军人数

	const int ONCE_LIMIT = 5;

	int out_temp[ONCE_LIMIT] = { -1 };
	int in_temp[ONCE_LIMIT] = { -1 };

	out_temp[0] = actor_out_id1;
	out_temp[1] = actor_out_id2;
	out_temp[2] = actor_out_id3;
	out_temp[3] = actor_out_id4;
	out_temp[4] = actor_out_id5;

	in_temp[0] = actor_in_id1;
	in_temp[1] = actor_in_id2;
	in_temp[2] = actor_in_id3;
	in_temp[3] = actor_in_id4;
	in_temp[4] = actor_in_id5;

	int temp1 = 0;
	int num = 0; //标记ID为零的武将的个数
	for (int i = 0; i < MAX_OWN_ARMY_NUM; i++)
	{
		if (-1 == history->out_amy[i] || num && 0 == history->out_amy[i])  //如果ID为零重复重现就覆盖掉
		{
			bool isOwn = false;
			for (int j = 0; j < MAX_OWN_ARMY_NUM; j++)
			{
				if (history->out_amy[j] == out_temp[temp1])
				{
					isOwn = true;
					temp1++;
					break;
				}
			}
			if (!isOwn)
			{
				history->out_amy[i] = out_temp[temp1++];
			}
			
		}
		else if (0 == history->out_amy[i] && num == 0)
		{
			num++;
		}
		if (ONCE_LIMIT == temp1)
		{
			break;
		}
	}

	int temp2 = 0;
	for (int i = 0; i < MAX_OWN_ARMY_NUM; i++)
	{
		if (-1 == history->in_amy[i])
		{
			bool isOwn = false;
			for (int j = 0; j < MAX_OWN_ARMY_NUM; j++)
			{
				if (history->in_amy[j] == in_temp[temp2])
				{
					isOwn = true;
					temp2++;
					break;
				}
			}
			if (!isOwn)
			{
				history->in_amy[i] = in_temp[temp2++];
			}
		}
		if (ONCE_LIMIT == temp2)
		{
			break;
		}
	}
	return 0;
}
int ScriptForceLimit::initPara(char *SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	limit = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	max_num = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_out_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_out_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_out_id3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_out_id4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_out_id5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_in_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown9 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_in_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown10 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_in_id3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown11 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_in_id4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown12 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_in_id5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
