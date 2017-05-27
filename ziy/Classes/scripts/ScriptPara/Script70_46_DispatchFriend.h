#ifndef _ScriptDispatchFriend_H_
#define _ScriptDispatchFriend_H_

#include "ScriptDispatchOneFriend.h"

#define SCRIPT_MAX_FRIEND_ARMY_NUM  20

class ScriptDispatchFriend : public Script
{
public:
	ScriptDispatchFriend()
	{
		cmd = CMD_DISPATCH_FRIEND;
		para = new ScriptDispatchOneFriend[SCRIPT_MAX_FRIEND_ARMY_NUM];
	}
	~ScriptDispatchFriend()
	{
		SAFE_DELETE_ARRAY(para);
	}

	ScriptDispatchOneFriend* para;
	int HandleScript();
	int initPara(char*SceneData, int index);
};

#endif // _ScriptDispatchFriend_H_