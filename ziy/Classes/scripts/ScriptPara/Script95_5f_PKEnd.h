#ifndef _ScriptPKEnd_H_
#define _ScriptPKEnd_H_

class ScriptPKEnd  : public Script
{
public:
	 ScriptPKEnd() {
		 cmd = CMD_PK_END;
	 }
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif