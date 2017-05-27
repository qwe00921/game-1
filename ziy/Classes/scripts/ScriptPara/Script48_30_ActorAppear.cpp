#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptActorAppear::HandleRScript(RedeScreenPtr scr){
	HistoryActorPtr actor = &history->actors[actor_id];
	actor->x = x;
	actor->y = y;
	actor->style = style>0?style+2:0;
	actor->direction = direction;
	if (history->RedeActorNum<0)
	{
		history->RedeActorNum = 0;
	}
	history->RedeActor[history->RedeActorNum++] = actor;
	history->LoadBattleActorImage(actor_id);
	scr->InitActorQueue();
	return 0;
}

int ScriptActorAppear::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	style =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
