#include "Common.h"
#include "Scripts.h"

	 int ScriptBattleActorTurn::HandleScript(){
		 if(frame == 0)
		 {
			 need_frame = delay1 + delay2;
		 }
		 if(frame == delay1)
		 {
			 HistoryActorPtr actor = &history->actors[actor_id1];
			 HistoryActorPtr actor2;

			if(actor_id2==actor_id1)
			{
			}
			 else if(actor_id2>=0&&actor_id2<trData->actor_num)
			 {
				 actor2 = &history->actors[actor_id2];
				 actor->direction = actor->getActionDirect(actor2->x, actor2->y);
			 }
			 else if(direction<4&&direction>=0)
				actor->direction = direction;
			 if(delay2)
			 {
				 history->CenterActor(actor);
				 playSound(SOUND_ACTOR_TURN);
			 }
		 }
		return need_frame - frame++;
    }
	
	 int ScriptBattleActorTurn::initPara(char*SceneData,int index){
		unkown1 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		actor_id1 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		unkown2 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		actor_id2 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;	
		unkown3 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		direction =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;	
		unkown4 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		is_opposite =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		unkown5 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		delay1 =  bytesToShort(SceneData, index)*TIMER_DELAY_FRAME;
		index += SHORT_NUM_LEN;	
		unkown6 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		delay2 =  bytesToShort(SceneData, index)*TIMER_DELAY_FRAME;
		index += SHORT_NUM_LEN;	
		return index;
	}
