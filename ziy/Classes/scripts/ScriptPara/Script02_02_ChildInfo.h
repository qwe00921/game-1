#ifndef _ScriptChildInfo_H_
#define _ScriptChildInfo_H_


class ScriptChildInfo : public Script
{
public:
	ScriptChildInfo();
	int HandleScript();
	int initPara(char*SceneData, int index);
private:
	Script* getExtendScript();
private:
	Script* m_ptrScript;
	short unkown1;
	DString m_strContent;
};

#endif