#ifndef _ScriptDispatchOneEnemy_H_
#define _ScriptDispatchOneEnemy_H_

class ScriptDispatchOneEnemy : public Script
{
public:
	ScriptDispatchOneEnemy()
	{
		//	Script(cmd);
		m_iRandAddLv = 0;
	}

	short unkown1;

	short actor_id;

	short unkown2;

	short isAssist;

	short unkown3;

	short hide;

	short unkown4;

	int x;

	short unkown5;

	int y;

	short unkown6;

	short direction;

	short unkown7;

	short level_add;

	short unkown8;

	short occu_limit;

	short unkown9;

	short ai_type;

	short unkown10;

	short oppo_actor_id;

	short unkown11;

	int dest_x;

	short unkown12;

	int dest_y;

	int m_iRandAddLv;

	int initPara(char*SceneData, int index);
};
#endif