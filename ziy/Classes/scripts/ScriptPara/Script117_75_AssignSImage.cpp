#include "Common.h"
#include "Scripts.h"

int ScriptAssignSImage::HandleScript()
{
	if (image_fight_id>400)
	{
		history->actors[actor_id1].history_image_fight_id = image_fight_id;
	}
	else if (image_fight_id > 0 && image_fight_id <= 7)
	{
		history->actors[actor_id1].image_change_occu_id = image_fight_id;
	}
	else if (image_fight_id <= 0)
	{
		history->actors[actor_id1].image_change_occu_id = 0;
	}

	if (image_fight_id == 0)
	{
		history->actors[actor_id1].history_image_fight_id = trData->occupation_image_id[history->actors[actor_id1].occupation_id];
	}
	history->actors[actor_id1].force_save = 1;
	history->LoadBattleActorImage(actor_id1);
	return 0;
}

int ScriptAssignSImage::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	image_fight_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
