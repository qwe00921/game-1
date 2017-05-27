#include "Common.h"
#include "Scripts.h"

int ScriptActorChangeImage::HandleScript()
{
	history->actors[actor_id].image_id = img_id;
	history->actors[actor_id].force_save = 1;
	history->LoadBattleActorImage(actor_id);
	return 0;
}

int ScriptActorChangeImage::initPara(char* SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	img_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
