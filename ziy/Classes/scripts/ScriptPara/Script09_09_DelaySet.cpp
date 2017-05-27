#include "Common.h"
#include "Scripts.h"
//#include "ScriptDelaySet.h"

	 int ScriptDelaySet::HandleScript(){
			if(frame == 0)
				need_frame = delay; 
    		return need_frame - frame++;
    }
	
	 int ScriptDelaySet::initPara(char*SceneData,int index){
		unkown1 =  bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		delay =  bytesToShort(SceneData, index);
		index += INT_NUM_LEN;
		return index;
	}
