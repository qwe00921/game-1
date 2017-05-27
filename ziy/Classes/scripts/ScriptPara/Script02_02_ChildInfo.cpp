#include "Common.h"
#include "Scripts.h"
#include "ScriptExtendTest.h"

ScriptChildInfo::ScriptChildInfo()
{
	cmd = CMD_CHILD_INFO_ACTION;
	m_ptrScript = nullptr;
}

int ScriptChildInfo::HandleScript()
{
	if (m_ptrScript)
	{
		return m_ptrScript->HandleScript();
	}
	return 0;
}

int ScriptChildInfo::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	m_strContent = bytesToString(SceneData, index);
	index += m_strContent->length() + 1;

	m_ptrScript = getExtendScript();

	return index;
}

Script* ScriptChildInfo::getExtendScript()
{
	Script* ptrScript = nullptr;
	int iScriptIndex = -1;
	map<int, string> mContent = trData->getStringMap(m_strContent->getChar(), '\n');
	for (UInt i = 0; i < mContent.size(); i++)
	{
		string strContent = mContent[i];
		if (strContent[0] == '#')
		{
			iScriptIndex = i;
			break;
		}
	}

	if (iScriptIndex >= 0)
	{
		string strContent = mContent[iScriptIndex];
		strContent = strContent.substr(1);
		int iScriptId = StringToInt(strContent.c_str());
		map<int, string> mStrScript;
		int iIndex = 0;
		for (UInt i = iScriptIndex + 1; i < mContent.size(); i++)
		{
			mStrScript[iIndex++] = mContent[i];
		}

		switch(iScriptId)
		{
		case CMD_EXTEND_TEST122:
			ptrScript = new ScriptExtendTest122(mStrScript);
			break;
		default:
			break;
		}
	}
	return ptrScript;
}
