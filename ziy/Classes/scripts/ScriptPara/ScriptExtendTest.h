#ifndef _ScriptExtendTest_H_
#define _ScriptExtendTest_H_

class ScriptExtendTest122 : public Script
{
public:
	ScriptExtendTest122(map<int, string> mStrScript);
	int HandleScript();
	int initPara(char*SceneData, int index);
private:
	bool compareNum(int iValue1, int iValue2, int iOperator);
public:
	static map<int, map<int, int> > m_mOldAttr;
private:
	map<int, string> m_mStrScript;
};

#endif //_ScriptExtendTest_H_