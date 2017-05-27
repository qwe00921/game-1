#include "Common.h"
#include "Scripts.h"

int ScriptHeadStateSet::HandleScript() {
	//TODO 武将头像变更
	int index = 0;
	int actor_id = 0;
	switch (id)
	{
		case 0://曹操普通
			history->actors[0].face_id = 0;
			actor_id = 0;
			break;
		case 1://曹操惊讶
			history->actors[0].face_id = -3;//不改之前头像的处理逻辑 这里把曹操其他表情对应的face_id设为负值
			actor_id = 0;
			break;
		case 2://曹操愤怒
			history->actors[0].face_id = -2;
			actor_id = 0;
			break;
		case 3://曹操欣喜
			history->actors[0].face_id = -1;
			actor_id = 0;
			break;
		case 4://夏侯惇蒙目
			history->actors[1].face_id = 217;
			actor_id = 1;
			break;
		case 16://孔明邪恶
			history->actors[35].face_id = 207;
			actor_id = 35;
			break;
		case 32://曹丕
			history->actors[8].face_id = 219;
			actor_id = 8;
			break;
		case 128://夏侯惇独目
			history->actors[1].face_id = 1;
			actor_id = 1;
			break;
		case 255://孔明正常
			history->actors[35].face_id = 35;
			actor_id = 35;
			break;
		default:
			actor_id = 0;
			break;
	}
	//index = history->actors[actor_id].face_id > 0 ? history->actors[actor_id].face_id + 8 : history->actors[actor_id].face_id + 1;
	if (history->actors[actor_id].face_id<0){
		index = history->actors[actor_id].face_id +5;
	}
	else{
		index = history->actors[actor_id].face_id > 0 ? history->actors[actor_id].face_id + 8 : history->actors[actor_id].face_id + 1;
	}
	
	SAFE_DELETE_IMAGE(history->imgHead[index]);
   history->imgHead[index] = CreateImage(ChessSprintf("%s%d", "img/Face/", index));
	return 0;
}

int ScriptHeadStateSet::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
