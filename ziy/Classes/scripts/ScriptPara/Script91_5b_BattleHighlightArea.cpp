#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
int ScriptBattleHighlightArea::HandleScript() {
	if (frame == 0)
	{
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		if (scr != NULL)
		{
			scr->curPointActor = NULL;
			scr->highlight_frame = 0;
			scr->rect_highlight.x = x1*SCENE_TILE_WIDTH;
			scr->rect_highlight.y = y1*SCENE_TILE_WIDTH;
			scr->rect_highlight.width = (x2 - x1 + 1)*SCENE_TILE_WIDTH;
			scr->rect_highlight.height = (y2 - y1 + 1)*SCENE_TILE_WIDTH;
			history->CheckPos(x1, y1);
			history->CheckPos(x2, y2);
		}
		if (is_in_battle == -1 || is_in_battle == 0)
		{
			history->is_victory_area = 1;
			history->victory_x1 = x1;
			history->victory_y1 = y1;
			history->victory_x2 = x2;
			history->victory_y2 = y2;

		}
		need_frame = 1;
	}
	return need_frame - frame++;
}

int ScriptBattleHighlightArea::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x1 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y1 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x2 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y2 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	is_in_battle = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
