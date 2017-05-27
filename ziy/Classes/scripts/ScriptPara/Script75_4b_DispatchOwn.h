#ifndef _ScriptDispatchOwn_H_
#define _ScriptDispatchOwn_H_


class ScriptDispatchOwn  : public Script
{
public:
	 ScriptDispatchOwn() {
		cmd = CMD_DISPATCH_OWN;
	}

	 short unkown1;

	 	int out_index;

	 short unkown2;

	 int x;

	 short unkown3;

	 	int y;

	 short unkown4;

	 short direction;

	 short unkown5;

	 short hide;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
private:
	void initPVPOwnData();
};
#endif