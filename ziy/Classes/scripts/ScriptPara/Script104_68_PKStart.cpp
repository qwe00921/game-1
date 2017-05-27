#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"

int ScriptPKStart::HandleScript()
{
	if (frame == 0)
	{
		if (history->var2[4087] != 1)//代表剧本手动单挑开启
		{
			Chess::getInstance()->appendScreen(new PKScreen(actor_id1, actor_id2, logoIndex));
		}
	}
	return 1 - frame++;
}

int ScriptPKStart::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	logoIndex = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
