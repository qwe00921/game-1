#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "ActorTalkLayer.h"
#include "TalkLayer.h"
//#include "ScriptActorTalk.h"
bool Actor_zero_hide()//判断actor_id为0的武将在不在RedeActor里面
{
	for (int i = 0; i < history->RedeActorNum; i++)
	{
		if (history->RedeActor[i]->actor_id == 0)
			return true;
	}
	return false;
}
int ChiefCommander()//返回该R场景里面的主帅
{
	short actorId = -1;
	short min_x = -1;
	short max_x = -1;
	short min_y = -1;
	short max_y = -1;
	short xORy[4] = { 0 };//数组下标0,1,2,3对应min_x，max_x，min_y，max_y在所有RedeActor武将坐标里面的个数
	if (history->RedeActorNum > 0)
	{
		min_x = history->RedeActor[0]->x;
		max_x = history->RedeActor[0]->x;
		min_y = history->RedeActor[0]->y;
		max_y = history->RedeActor[0]->y;
	}
	for (int i = 0; i < history->RedeActorNum; i++)
	{
		if (history->RedeActor[i]->x>max_x)
			max_x = history->RedeActor[i]->x;
		if (history->RedeActor[i]->x < min_x)
			min_x = history->RedeActor[i]->x;
		if (history->RedeActor[i]->y>max_y)
			max_y = history->RedeActor[i]->y;
		if (history->RedeActor[i]->y < min_y)
			min_y = history->RedeActor[i]->y;
	}

	for (int j = 0; j < history->RedeActorNum; j++)
	{
		if (min_x == history->RedeActor[j]->x)
			xORy[0]++;
		if (history->RedeActor[j]->x == max_x)
			xORy[1]++;
		if (min_y == history->RedeActor[j]->y)
			xORy[2]++;
		if (max_y == history->RedeActor[j]->y)
			xORy[3]++;
	}
	for (int i = 0; i < history->RedeActorNum; i++)
	{
		if (((min_y == history->RedeActor[i]->y&&xORy[2] == 1) || (max_y == history->RedeActor[i]->y&&xORy[3] == 1)) && min_x<history->RedeActor[i]->x&&history->RedeActor[i]->x<max_x)
		{
			if (actorId == -1)
				actorId = history->RedeActor[i]->actor_id;
			else if(actorId>history->RedeActor[i]->actor_id)
				actorId = history->RedeActor[i]->actor_id;
		}
		if (((min_x==history->RedeActor[i]->x&&xORy[0]==1)||(history->RedeActor[i]->x==max_x&&xORy[1]==1)) && (min_y < history->RedeActor[i]->y || max_y > history->RedeActor[i]->y))
		{
			if (actorId == -1)
				actorId = history->RedeActor[i]->actor_id;
			else if (actorId > history->RedeActor[i]->actor_id)
				actorId = history->RedeActor[i]->actor_id;
		}
	}
	return actorId;
}
int ScriptActorTalk::HandleRScript(RedeScreenPtr scr)
{
	if(frame++ == 0 && info->length() > 0)
	{
		if(scr->rede_state == REDE_STATE_WAIT_COMMAND)
		{
			if(scr->cur_actor != NULL&&scr->cur_actor->direction % 2 != 0)
				scr->cur_actor->direction = 2;//朝南
		}
		scr->talk_layer = ActorTalkLayer::create(info->getChar(), scr);
		ChessApplication::getInstance()->AddFunctionLayer(scr->talk_layer);
		return 1;
	}
	return 0;
}

int ScriptActorTalk::HandleScript()
{
	if(frame++ == 0 && info->length() > 0)
	{
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		if(scr!=NULL)
		{
			if (actor_index1 >= 0)
			{
				scr->m_pTalkLayer->updateLayer(info->getChar(), actor_index1);
			}
			else
			{
				scr->m_pTalkLayer->updateLayer(info->getChar());
			}
		}
		return 1;
	}
	return 0;
}

int ScriptActorTalk::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length()+1;
	return index;
}
