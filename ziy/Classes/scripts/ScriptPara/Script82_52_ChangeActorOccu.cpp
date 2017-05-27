#include "Common.h"
#include "Scripts.h"

int ScriptChangeActorOccu::HandleScript() {

	HistoryActorPtr actor = &history->actors[actor_id];
	if (actor == NULL)
		return 0;

	if (actor->history_image_fight_id < 400){
		//actor->image_change_occu_id = data->occupation_image_id[occu_id];
		actor->history_image_fight_id = trData->occupation_image_id[occu_id];
	}
	//else{
	//	actor->image_change_occu_id = history->getBattleActorImageId(actor_id);//有特造 转职后特造不变
	//}
	actor->force_save = 1;
	if (!actor->enable_set_occupation_id)
		actor->occupation_id = occu_id;
	actor->history_occupation_id = actor->GetActorInitialJob();
    actor->UpdataGoodEffect();
	history->LoadBattleActorImage(actor_id);
	if (history->var2[4081])//重新计算人物五维
	{
		actor->InitFiveBaseAbility();
	}
	return 0;
}

int ScriptChangeActorOccu::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	occu_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
