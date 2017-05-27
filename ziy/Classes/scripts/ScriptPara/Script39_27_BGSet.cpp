#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptBGSet::HandleRScript(RedeScreenPtr scr)
{
	if(history->BG_index == 115)
	{
		history->RedeActorNum= 0;
		scr->buf[0] = (char*)NULL;
		scr->buf[1] = (char*)NULL;
		scr->buf[2] = (char*)NULL;
		scr->cur_index = 0;
		scr->cur_line = 0;
	}
	scr->rede_state = REDE_STATE_NORMAL;
	waijing_id_replace = waijing_id;
	neijing_id_replace = neijing_id;
	if (history->var2[4071] == 123)
	{
		waijing_id_replace = waijing_id + 100;
		neijing_id_replace = neijing_id + 100;
	}
	switch(bg_type)
	{
	case 0:				//外景
		{
			history->BG_index = waijing_id_replace + 1;
		}
		break;
	case 2:				//内景
		{
			history->BG_index = neijing_id_replace + 41;
		}
		break;
	case 1:				//三国地图
		{
			scr->rede_state = REDE_STATE_MAP_NORMAL;
			history->BG_index = 115;
		}
		break;
	case 3:				//战场
		break;
	}

	SAFE_DELETE_IMAGE(history->imgBG);
	history->imgBG = CreateImage(ChessSprintf("img/Mmap/%d", history->BG_index), 1);

	if (history->isFreeMove)
	{
		scr->setMenuVisible(true);
	}
	return 0;
}
int ScriptBGSet::HandleScript() {
	return 0;
	zhanchang_id_replace = zhanchang_id;
	if (history->var2[4071] == 123)
	{
		zhanchang_id_replace = zhanchang_id + 100;
	}
	history->BG_index = zhanchang_id_replace + 1;
	history->GenerateMapArray(zhanchang_id_replace);

	SAFE_DELETE_IMAGE(history->imgBG);
	
    history->imgBG = CreateImage(ChessSprintf("img/HM/%d", history->BG_index), 1);
	return 0;
}

int ScriptBGSet::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	bg_type = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	waijing_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	sanguoditu_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	neijing_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	zhanchang_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
