#include "Common.h"
#include "Scripts.h"
//#include "ScriptSoundSet.h"

int ScriptSoundSet::HandleScript(){
	playSound_Repeat(sound_id,repeat);
	return 0;
}

int ScriptSoundSet::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	sound_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	repeat =  bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
