#ifndef _ScriptChangeViewPoint_H_
#define _ScriptChangeViewPoint_H_
class ScriptChangeViewPoint : public Script
{
public:
	 ScriptChangeViewPoint() {
		cmd = CMD_CHANGE_VIEWPOINT;
	}
	 short unkown1;
	 int x;
	 short unkown2;
	 int y;
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif