#ifndef _ScriptSetActorAbility_H_
#define _ScriptSetActorAbility_H_



class ScriptSetActorAbility : public Script
{
public:
	 ScriptSetActorAbility() {
		cmd = CMD_SET_ACTOR_ABILITY;
	}
	 short unkown1;
	 short actor_id;
	 short unkown2;
	 short set_area; //0攻击力1防御力2精神力3爆发力4士气5HP6MP7当前HP
	 short unkown3;
	 short set_type;//0直接设定1增加2减少
	 short unkown4;
	 int set_num;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif