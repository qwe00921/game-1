#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#define test_result  test_res[test_index]

enum
{
	SCRIPT_HP = 0, //指令修改属性
	SCRIPT_MP = 1,
	SCRIPT_PW = 2,
	SCRIPT_DEF = 3,
	SCRIPT_INT = 4,
	SCRIPT_EXP = 5,
	SCRIPT_FOR = 6,
	SCRIPT_MAX = 7
};

class Script 
{
public:
	short cmd;
	int fineshed;
	int subScript;
	int frame;
	int need_frame;
	ChessPtr chess;
	static bool test_res[10000];
	static int test_index;
	static bool in_child_first_time;
	Script();
	Script(int cmd);
	bool get_test_result();
	void setSubScript(int subScript);
	virtual int HandleScript();
	virtual int HandleRScript(RedeScreenPtr scr);
	virtual int initPara(char* SceneData, int index);
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	Script(const Script&);
	Script& operator=(const Script&);
};
#endif