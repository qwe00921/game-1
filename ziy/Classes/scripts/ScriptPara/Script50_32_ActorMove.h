#ifndef _ScriptActorMove_H_
#define _ScriptActorMove_H_

class ScriptActorMove  :public Script{
public:
	 ScriptActorMove() {
		cmd = CMD_ACTOR_MOVE;
	}
	
	 ScriptActorMove(int act_id, int pointX, int pointY, int direct)
	 {
		cmd = CMD_ACTOR_MOVE;
		which_index = 0;
		actor_id = act_id;
		out_index = 0;
		x = pointX;
		y = pointY;
		direction = direct;
	}
	 short unkown1; // 40 //决定武将的方式

	 short which_index;

	 short unkown2; // 2武将序号

	 short actor_id;

	 short unkown3; // 4出场顺序

	 int out_index;

	 short unkown4; // 4横坐标

	 int x;

	 short unkown5; // 4纵坐标

	 int y;

	 short unkown6;

	 short direction;

	 int HandleRScript(RedeScreenPtr scr);
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif