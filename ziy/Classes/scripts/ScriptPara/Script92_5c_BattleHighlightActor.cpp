#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
int ScriptBattleHighlightActor::HandleScript() {
	if(frame == 0)
	{
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		HistoryActorPtr actor = history->FindBattleActor(actor_id);
		if(scr!=NULL&&actor!=NULL&&actor->hide!=1)
		{
			scr->curPointActor = actor;
			scr->highlight_frame = 0;
			scr->rect_highlight.x = SCENE_TILE_WIDTH*actor->x;
			scr->rect_highlight.y = SCENE_TILE_WIDTH*actor->y;
			scr->rect_highlight.width = SCENE_TILE_WIDTH;
			scr->rect_highlight.height = SCENE_TILE_WIDTH;
			history->CenterActor(actor);
		}
		need_frame = 1;
	}
	return need_frame - frame++;
}

int ScriptBattleHighlightActor::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
