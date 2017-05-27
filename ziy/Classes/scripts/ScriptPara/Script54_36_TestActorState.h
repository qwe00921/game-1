#ifndef _ScriptTestActorState_H_
#define _ScriptTestActorState_H_



class ScriptTestActorState : public Script
{
public:
	 ScriptTestActorState() {
		cmd = CMD_TEST_ACTOR_STATE;
	}
	
	 short unkown1;

	 short actor_id;

	 short unkown2;

	 short which_state;// 0攻击力1防御力2精神力3爆发力4士气5HP6MP7当前HP

	 short unkown3;

	 unsigned short num; //强转为US类型，最大值扩充到65536

	 short unkown4;

	 short relation;//0：大于等于 1：小于 2：等于

	 short actor_id_replace;
	 void TestOneActorState(HistoryActorPtr actor);
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif