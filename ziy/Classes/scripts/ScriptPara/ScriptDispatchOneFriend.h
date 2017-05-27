#ifndef _ScriptDispatchOneFriend_H_
#define _ScriptDispatchOneFriend_H_


class ScriptDispatchOneFriend  : public Script
{
public:
	 ScriptDispatchOneFriend() {
		//Script(cmd);
	}

	 short unkown1;

	 short actor_id;

	 short unkown2;

	 short hide;

	 	short unkown3;

	 	int x;

	 	short unkown4;

	 	int y;

	 	short unkown5;

	 short direction;

	 short unkown6;

	 short level_add;

	 	short unkown7;

	 	short occu_limit;

	 	short unkown8;

	 short ai_type;

	 	short unkown9;

	 	short oppo_actor_id;

	 	short unkown10;

	 int dest_x;

	 short unkown11;

	 int dest_y;
	
	 int initPara(char*SceneData,int index);
};
#endif