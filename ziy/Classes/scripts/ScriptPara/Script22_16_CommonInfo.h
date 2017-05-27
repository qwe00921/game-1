#ifndef _ScriptCommonInfo_H_
#define _ScriptCommonInfo_H_

class ScriptCommonInfo : public Script
{
public:
	ScriptCommonInfo(short cmd) :Script(cmd)
	{
		info = "";
		isScript = true;
		m_bCanNotClick = false;
	}
	ScriptCommonInfo(char *str, int iInfoType = 1, bool bCanNotClick = false)
	{
		cmd = CMD_NORMAL_INFO;
		info = str;
		m_iInfoType = iInfoType;
		isScript = false;
		m_bCanNotClick = bCanNotClick;
	}

	short unkown1; // 05
	DString info;
	void GenerateTalkList(const char *buf,BattleScreenPtr scr);
	int HandleRScript(RedeScreenPtr scr);
	int HandleScript();
	int initPara(char*SceneData, int index);
private:
	bool isScript;
	bool m_bCanNotClick;
	int m_iInfoType;
};
#endif