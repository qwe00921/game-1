#include "Common.h"
#include "Scripts.h"
//#include "ScriptMusicSet.h"


int ScriptMusicSet::HandleScript(){
	ChessPlayMp3(music_id+2, true);
	history->bg_music_index = music_id+2;
	return 0;
}

int ScriptMusicSet::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	music_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
