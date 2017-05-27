#include "Common.h"
#include "Scripts.h"
//1024 任何部队
//1025 我军或友军
//1026 敌军
//1027 我军当前部队
int ScriptTestActorArea::HandleScript(){
	int i,j;
	test_result = false;
	HistoryActorPtr actor = NULL; 
	for(i = y1;i<=y2&&test_result == false;i++)
	{
		for(j = x1;j<=x2&&test_result == false;j++)
		{
			actor = history->FindActorPosition(j,i);
			if (actor != NULL && actor->hide != 1)//隐藏任务不进行指定区域判断
			{
				if(actor_id<1024)
				{
					if(actor->actor_id == actor_id)
					{
						test_result = true;
					}
				}
				else
				{
					switch(actor_id) {
					case 1024:
						test_result = true;
						break;
					case 1025:
						if(actor->relation<2)
							test_result = true;
						break;
					case 1026:
						if(actor->relation==2)
							test_result = true;
						break;
					case 1027:
						if (history->cur_actor && history->cur_actor->actor_id == actor_id&&
							history->cur_actor->relation == 0)
							test_result = true;
						break;
					}
				}
			}
		}
	}
	if(!get_test_result())
		return  -1;
	return 0;
}

int ScriptTestActorArea::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x1 =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y1 =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x2 =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y2 =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
