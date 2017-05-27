#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptChapterName::HandleRScript(RedeScreenPtr scr)
{

	if(frame == 0)
	{
		need_frame = 100;
		scr->chapter_index = chapt_index;
		scr->chapter_name = info;
		scr->rede_state = REDE_STATE_NORMAL;
		SAFE_DELETE_IMAGE(history->imgBG);
	}
	else if (frame == need_frame)
	{
		scr->chapter_name = (char*)NULL;
	}
	
	return need_frame - frame++;
}
int ScriptChapterName::initPara(char*SceneData,int index)
{
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	chapt_index = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length()+1;
	return index;
}
