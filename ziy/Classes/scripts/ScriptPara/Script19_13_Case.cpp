#include "Common.h"
#include "Scripts.h"
//#include "ScriptCase.h"

int ScriptCase::HandleScript(){
	if(id != chess->choice->popup())
	{
		test_result = false;
		Script::test_index--;
		return -1;
	}
	test_result = true;
	return 0;
}
	
int ScriptCase::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	id =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
