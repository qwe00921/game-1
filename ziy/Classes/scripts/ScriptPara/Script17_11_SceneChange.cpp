#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"

int ScriptSceneChange::HandleRScript(RedeScreenPtr scr)
{
	int scene = scene_id/2;
	scene_replace = scene;
	if (history->var2[4071]==123)
	{
		scene_replace = scene + 100;
	}
	if(scene_id%2 == 0)
	{
		history->game_scene = scene_replace;
	}
	else
	{
		history->game_scene = scene_replace + 1;
		scr->handler.LoadRFile(history->game_scene);
        return 0;
	}
	return 1-frame++;
}
int ScriptSceneChange::HandleScript() {
	int scene = scene_id / 2;
	scene_replace = scene;
	if(history->var2[4071] == 123)
	{
		scene_replace = scene + 100;
	}
	if (scene_id % 2 == 0)
	{
		history->game_scene = scene_replace;
	}
	else
	{
		history->game_scene = scene_replace + 1;
	}
	return 1 - frame++;
}

int ScriptSceneChange::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	scene_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
