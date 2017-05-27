#include "Common.h"
#include "Scripts.h"
//#include "ScriptTestActorPos.h"

	 int ScriptTestActorPos::HandleScript() {
		test_result = true;
		HistoryActorPtr curActor = history->FindActorPosition(x, y);
		if (curActor == NULL) {
			test_result = false;
		} else if (!((actor_id == curActor->actor_id)
				|| (actor_id == 1025 && curActor->relation < 2)
				|| (actor_id == 1026 && curActor->relation == 2)
				|| (actor_id == 1027 && curActor->relation == 0))) {
			test_result = false;
		}
		if (!get_test_result()) {
			return -1;
		}
		return 0;
	}

	 int ScriptTestActorPos::initPara(char* SceneData, int index) {
		unkown1 = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		actor_id = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		unkown2 = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		x = bytesToShort(SceneData, index);
		index += INT_NUM_LEN;
		unkown3 = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
		y = bytesToShort(SceneData, index);
		index += INT_NUM_LEN;
		return index;
	}
