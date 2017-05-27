#ifndef _ScriptActorDisappear_H_
#define _ScriptActorDisappear_H_



class ScriptActorDisappear : public Script
{
public:
	 ScriptActorDisappear() {
		cmd = CMD_ACTOR_DISAPPEAR;
	}
	
	 short unkown1;
	 
	 short which_index; // 0序号，1范围
	 
	 short unkown2;
	 
	 short actor_id;
	 
	 short unkown3;
	 
	 int left_top_x;
	 
	 short unkown4;
	 
	 int left_top_y;
	 
	 short unkown5;
	 
	 int right_bottom_x;
	 
	 short unkown6;
	 
	 int right_bottom_y;
	 
	 short unkown7;
	 
	 short relation; // 0我军，1友军，2敌军，3援军，4我军及友军，5敌军及援军，6所有部队


	 int actor_id_list[100];
	 int actor_num;
	 int actor_index;
	 short actor_id_replace;
	int HandleRScript(RedeScreenPtr scr);
	int HandleScript();
	int initPara(char*SceneData,int index);
	void getActorList();
};
#endif