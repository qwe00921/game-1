#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
//#include "ScriptAddMapElem.h"
#include "MapElem.h"
int ScriptAddMapElem::HandleScript(){//添加战场上火障碍的逻辑处理
	if (delay1 < 0) //TODO 放火解析出来值为-1导致火出不来
	{
		delay1 = 0;
	}
	if (delay2 < 0)
	{
		delay2 = 0;
	}
	if(frame == 0)
	{
		need_frame = delay1 + 1;
	}
	if(frame == delay1)
	{
		if (elem_id)//移除障碍物
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
		}
		else
		{
			history->elem_list->addElement(new MapElem(x, y));
			playSound(SOUND_SET_FIRE);
		}
		need_frame = delay1 + delay2;
	}
	return need_frame - frame++;
}

int ScriptAddMapElem::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x =  bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y =  bytesToInt(SceneData, index);
	index += INT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	elem_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
