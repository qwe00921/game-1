#ifndef _ScriptTestActorClicked_H_
#define _ScriptTestActorClicked_H_

class ScriptTestActorClicked  : public Script
{
public:
	 ScriptTestActorClicked()
	 {
		cmd = CMD_TEST_ACTOR_CLICKED;
	 }
	 short unkown1;
	 short actor_id;
	 int HandleRScript(RedeScreenPtr scr);
	 int initPara(char*SceneData,int index);
};
#endif