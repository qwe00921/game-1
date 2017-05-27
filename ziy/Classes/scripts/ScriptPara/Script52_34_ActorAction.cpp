#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptActorAction::HandleRScript(RedeScreenPtr scr)
{
	if(need_frame == 0)
		history->actors[actor_id].bust_style = style >= 0 ? style + 1 : 0;
	if(style == -1)
	{
		if(need_frame == 0)
		{
			history->readscreen_bust_one = actor_id;
			history->readscreen_bust_paint = 1;
		}
		if((need_frame / 2) % 2 == 0)
		{
			scr->paint_bust_offset += 2;
		}
		else
			scr->paint_bust_offset -= 2;
		if(need_frame++ < 22)
			return 1;
		else
		{
			history->readscreen_bust_one = -1;
			history->readscreen_bust_paint = 0;
			scr->paint_bust_offset = 0;
			return 0;
		}
	}
	return 0;
}
int ScriptActorAction::HandleScript()
{
	if(need_frame == 0)
		history->actors[actor_id].bust_style = style >= 0 ? style + 1 : 0;
	if(style == -1)
	{
		if(need_frame == 0)
		{
			history->readscreen_bust_one = actor_id;
			history->readscreen_bust_paint = 1;
			history->is_paint_bust = 1;
		}
		if((need_frame / 2) % 2 == 0)
		{
			history->battlescreen_paint_bust_offset += 2;
		}
		else
			history->battlescreen_paint_bust_offset -= 2;
		if(need_frame++ < 22)
			return 1;
		else
		{
			history->readscreen_bust_one = -1;
			history->readscreen_bust_paint = 0;
			history->battlescreen_paint_bust_offset = 0;
			history->is_paint_bust = 0;
			return 0;
		}
	}
	return 0;
}

int ScriptActorAction::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	style = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
