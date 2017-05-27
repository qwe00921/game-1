#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptMapTellInfo::HandleRScript(RedeScreenPtr scr)
{
	if(frame == 0)
	{
		bool maxLine = false;
		maxLine = scr->cur_line >= 2;

		if (!page || maxLine)
		{
			scr->cur_line = 0;
			scr->wait_click = 0;
			scr->buf[0] = (char*)NULL;
			scr->buf[1] = (char*)NULL;
			scr->buf[2] = (char*)NULL;
		}
		if(line||scr->cur_line == 0)
		{
			scr->cur_line++;
			scr->cur_index = 0;
		}
		if(scr->buf[scr->cur_line-1]!=NULL)
			scr->buf[scr->cur_line-1] = scr->buf[scr->cur_line-1]+info;
		else
			scr->buf[scr->cur_line-1] = info;
		scr->rede_state = REDE_STATE_MAP_TELL_INFO;
		scr->wait_click = delay*100;
	}
	return 1 - frame++;
}
int ScriptMapTellInfo::initPara(char*SceneData,int index)
{
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length()+1;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	page =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	line =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
