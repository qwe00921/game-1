#include "Common.h"
#include "Scripts.h"

int ScriptMovieSet::HandleScript() {
	if(frame++ == 0)
	{
		return 1;
	}
	return 0;
}

int ScriptMovieSet::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	movie_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
