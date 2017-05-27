#include "Common.h"
#include "Scripts.h"
int ScriptMoneyChange::HandleScript(){
	if(frame==0)
	{
		switch(field)
		{
		case 0:
			{
				switch(set_type)
				{
				case 0:
					history->money = num;
					break;
				case 1:
					history->money +=num;
					break;
				case 2:
					history->money -=num;
					if(history->money<0)
						history->money = 0;
					break;
				}
			}
			break;
		case 1:
			{
				switch(set_type)
				{
				case 0:
					history->game_scene = num;
					break;
				case 1:
					history->game_scene +=num;
					break;
				case 2:
					history->game_scene -=num;
					if(history->game_scene<0)
						history->game_scene = 0;
					break;
				}
			}
			break;
		case 2:
			{
				switch(set_type)
				{
				case 0:
					need_frame = 3*(int)abs(history->extend_value - num);
					history->extend_value = num;
					break;
				case 1:
					need_frame = 3*(int)abs(num);
					history->extend_value +=num;
					if (history->extend_value > 100)
					{
						history->extend_value = 100;
					}
					break;
				case 2:
					need_frame = 3*(int)abs(num);
					history->extend_value -=num;
					if (history->extend_value<0)
						history->extend_value = 0;
					break;
				}
			}
			break;
		}
		if(history->BG_index == -1)
			need_frame = 1;
	}
	return need_frame - frame++;
}

int ScriptMoneyChange::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	field =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set_type =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	return index;
}
