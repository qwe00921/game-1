#ifndef _ScriptLeaveMsgShowOrNot_H_
#define _ScriptLeaveMsgShowOrNot_H_
class ScriptLeaveMsgShowOrNot : public Script
{
public:
	 ScriptLeaveMsgShowOrNot() {
		cmd = CMD_LEAVE_MSG_SHOW_OR_NOT;
	}
	 short unkown1;
	 short actor_id;
	 short unkown2;
	 short show;
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif