#ifndef _ScriptChangeActorState_H_
#define _ScriptChangeActorState_H_


class ScriptChangeActorState : public Script
{
public:
	ScriptChangeActorState()
	{
		cmd = CMD_CHANGE_ACTOR_STATE;
		bIsScript = true;
	}
	ScriptChangeActorState(short actor_id, int hp_change, int mp_change, int iSkillMiss, short coat_exp_change, short weapon_exp_change, short unnormal_state);
	ScriptChangeActorState(short actor_id, int change_item, int change_type);
	ScriptChangeActorState(short actor_id, short unnormal_state);
	short unkown1; // 41 //决定武将的方式

	short which_index; // 0武将序号，1出场顺序，2区域内全部武将

	short unkown2;

	short actor_id;

	short unkown3;

	int out_index;

	short unkown4;

	int left_top_x;

	short unkown5;

	int left_top_y;

	short unkown6;

	int right_bottom_x;

	short unkown7;

	int right_bottom_y;

	short unkown8;

	short relation; // 0我军，1友军，2敌军，3援军，4我军及友军，5敌军及援军，6所有部队

	short unkown9;

	short change_item;// 0攻击力1防御力2精神力3爆发力4士气5移动力

	short unkown10;

	short change_type;// 0下降1正常2上升;

	short unkown11;

	short unnormal_state;

	short unkown12;

	int hp_change;

	short unkown13;

	int mp_change;

	int iSkillMiss;
	int coat_exp_change;
	int weapon_exp_change;
	int show_change;

	int actor_id_list[100];
	int actor_num;
	int actor_index;

	int hp_change_replace;
	int mp_change_replace;
	bool bIsScript;
	int HandleScript();
	int initPara(char*SceneData, int index);
	void getActorList();
};
//异常状态1
//32
//0
//正常
//1
//正常
//2
//麻痹
//3
//麻痹
//4
//禁咒
//5
//禁咒
//6
//麻痹+禁咒
//7
//麻痹+禁咒
//8
//混乱
//9
//混乱
//10
//麻痹+混乱
//11
//麻痹+混乱
//12
//禁咒+混乱
//13
//禁咒+混乱
//14
//混乱+麻痹
//15
//混乱+麻痹
//16
//中毒
//17
//中毒
//18
//麻痹+中毒
//19
//麻痹+中毒
//20
//禁咒+中毒
//21
//禁咒+中毒
//22
//麻痹+禁咒+中毒
//23
//麻痹+禁咒+中毒
//24
//混乱+中毒
//25
//混乱+中毒
//26
//麻痹+混乱+中毒
//27
//麻痹+混乱+中毒
//28
//禁咒+混乱+中毒
//29
//禁咒+混乱+中毒
//30
//禁咒+麻痹+混乱+中毒
//31
//禁咒+麻痹+混乱+中毒
#endif