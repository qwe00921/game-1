#include "Common.h"
#include "Scripts.h"
#include "MapElem.h"

int ScriptSetBattleBarrier::HandleScript() {
	if(frame == 0)
	{
		need_frame = delay1 + 1;
	}
	if(frame == delay1)
	{
// 		if (set)//移除障碍物
// 		{
// 			int size = history->elem_list->size();
// 			for (int index = 0; index < size; index++)
// 			{
// 				if (((MapElemPtr)(history->elem_list->elementAt(index)))->x == x && ((MapElemPtr)(history->elem_list->elementAt(index)))->y == y)
// 				{
// 					history->elem_list->removeElementAt(index);
// 					break;
// 				}
// 			}
// 		}

		int iIsFind = 1;
		MapElem* ptrMapElem = new MapElem(x, y, barrier_id, land_id, set, iIsFind);
		if (!iIsFind)
		{
			SAFE_DELETE(ptrMapElem);
		}
		else
		{
			int size = history->elem_list->size();
			for (int index = 0; index < size; index++)
			{
			 	if (((MapElemPtr)(history->elem_list->elementAt(index)))->x == x && ((MapElemPtr)(history->elem_list->elementAt(index)))->y == y)
			 	{
			 		history->elem_list->removeElementAt(index);
			 		break;
			 	}
			}
			history->elem_list->addElement(ptrMapElem);
		}

		need_frame = delay1 + delay2;
		playSound(SOUND_SET_FIRE);
	}
	return need_frame - frame++;
}

int ScriptSetBattleBarrier::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	barrier_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	land_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
