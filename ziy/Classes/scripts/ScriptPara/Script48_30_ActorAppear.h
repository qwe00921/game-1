#ifndef _ScriptActorAppear_H_
#define _ScriptActorAppear_H_

class ScriptActorAppear  :public Script
{
public:
	ScriptActorAppear() {
		cmd = CMD_ACTOR_APPEAR;
	}
	
	short unkown1; // 2武将序号
	
	short actor_id;
	
	short unkown2; // 04横坐标
	
	int x;
	
	short unkown3; // 04纵坐标
	
	int y;
	
	short unkown4; // 43朝向0右上1右下2左下3左上
	
	short direction;
	
	short unkown5; // 0d 动作形态<19
	
	short style;
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif