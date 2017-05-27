#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
//#include "ScriptActorTalk2.h"
int ScriptActorTalk2::HandleRScript(RedeScreenPtr scr)
{
	return 0;
}
int ScriptActorTalk2::HandleScript()
{
	if(frame++ == 0 && info->length() > 0)
	{	
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		if(scr!=NULL)
		{
			HistoryActorPtr ptrActor1 = history->FindBattleActor(actor_index1);
			if(ptrActor1)
			{
				scr->m_pTalkLayer->updateLayer(info->getChar(), actor_index1);
			}
			else
			{
				if(actor_index2 >= 0 && actor_index2 < trData->actor_num)
				{
					HistoryActorPtr ptrActor2 = history->FindBattleActor(actor_index2);
					if(ptrActor2)
					{
						scr->m_pTalkLayer->updateLayer(info->getChar(), actor_index2);
					}
				}
			}
		}
		return 1;
	}
	return 0;
}

int ScriptActorTalk2::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_index1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_index2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length()+1;
	return index;
}
