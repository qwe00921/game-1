#include "Common.h"
#include "Scripts.h"

int ScriptGoodThrow::HandleScript()
{
	int delete_num = 1;
	if (history->var2[5000] >= 1 && history->var2[5000]<=9999)
		delete_num = history->var2[5000];

	bool isAssist = false;
	bool isEquip = false;
	int throwIndex = -1;

	int iNewGood_id = good_id + 1;
	HistoryGoods hisGoods(iNewGood_id);
	int iType = hisGoods.GetGoodsType();
	if(iType != GOOD_TYPE_CONSUME)
	{
		if(iType == GOOD_TYPE_ASSIST)
		{
			isAssist = true;
		}
		else
		{
			isEquip = true;
		}
	}
	else if (history->var2[5000] >= 1 && history->var2[5000] <= 9999)
	{
		if(history->m_mConsuNum[iNewGood_id] > delete_num)
			history->m_mConsuNum[iNewGood_id] -= delete_num;
		else
			history->m_mConsuNum[iNewGood_id] = 0;
		delete_num = 0;
	}

	for (int k = 0; k < delete_num; k++)
	{
		//////////////////////
		throwIndex = -1;
		for (int i = 0; i < history->goods->size(); i++)
		{
			if (((HistoryGoodsPtr)history->goods->elementAt(i))->good_id == iNewGood_id)
			{
				HistoryGoodsPtr ptrGood = (HistoryGoodsPtr)history->goods->elementAt(i);

				if (ptrGood->isNormalGood())
				{
					if (isAssist)
					{
						throwIndex = i;
						break;
					}
					else if (isEquip)
					{
						if (throwIndex == -1 || IsMaxLevel(ptrGood))
						{
							throwIndex = i;
						}
					}
					else
					{
						throwIndex = i;
						break;
					}
				}

			}
		}

		HistoryGoodsPtr hisgood = NULL;
		int actor_id = -1;
		int good_type = -1;

		for (int i = 0; i < history->ownArmyNum; i++)
		{
			HistoryActorPtr actor = &history->actors[history->ownArmy[i]];

			HistoryGoodsPtr hisgood = &actor->weapon;
			if (hisgood->isNormalGood() && hisgood->good_id == iNewGood_id)
			{
				if (throwIndex == -1 && actor_id == -1 || IsMaxLevel(hisgood))
				{
					actor_id = history->ownArmy[i];
					good_type = GOOD_TYPE_WEAPON;
					history->var2[4069] = hisgood->level;
					history->var2[4070] = hisgood->exp;
					continue;
				}
			}

			hisgood = &actor->coat;
			if (hisgood->isNormalGood() && hisgood->good_id == iNewGood_id)
			{
				if (throwIndex == -1 && actor_id == -1 || IsMaxLevel(hisgood))
				{
					actor_id = history->ownArmy[i];
					good_type = GOOD_TYPE_COAT;
					history->var2[4069] = hisgood->level;
					history->var2[4070] = hisgood->exp;
					continue;
				}
			}

			hisgood = &actor->assist_good;
			if (hisgood->isNormalGood() && hisgood->good_id == iNewGood_id)
			{
				if (throwIndex < 0)
				{
					actor_id = history->ownArmy[i];
					good_type = GOOD_TYPE_ASSIST;
				}
				break;
			}
		}

		if (actor_id >= 0)
		{
			switch (good_type)
			{
			case GOOD_TYPE_WEAPON:
				hisgood = &history->actors[actor_id].weapon;
				break;
			case GOOD_TYPE_COAT:
				hisgood = &history->actors[actor_id].coat;
				break;
			case GOOD_TYPE_ASSIST:
				hisgood = &history->actors[actor_id].assist_good;
				break;
			default:
				break;
			}

			hisgood->good_id = -1;
			history->actors[actor_id].UpdataGoodEffect();
			continue;
		}

		if (throwIndex >= 0)
		{
			history->goods->removeElementAt(throwIndex);
		}
		//////////////////////
	}

	return 0;
}

int ScriptGoodThrow::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}

bool ScriptGoodThrow::IsMaxLevel(HistoryGoodsPtr ptrGood)
{
	if(ptrGood->level > history->var2[4069] || ptrGood->level == history->var2[4069] && ptrGood->exp > history->var2[4070])
	{
		return true;
	}
	return false;
}
