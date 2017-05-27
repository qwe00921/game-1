#ifndef _Script120_78_ValueIntValuation_H_
#define _Script120_78_ValueIntValuation_H_
class ScriptValueIntValuation : public Script
{
public:
	ScriptValueIntValuation() {
		cmd = CMD_VALUE_INT_VALUATION;
	}

	short unkown1;
	int num;
	short unkown2;
	short operation_type;
	short unkown3;
	short actor_id;
	short unkown4;
	short attr_id;
	short actor_id_replace;
	int HandleScript();
	int initPara(char*SceneData, int index);
	void valuation1();
	void valuation2();
};
#endif