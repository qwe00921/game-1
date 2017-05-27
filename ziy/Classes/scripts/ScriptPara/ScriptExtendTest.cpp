#include "Common.h"
#include "Scripts.h"
#include "ScriptExtendTest.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
map<int, map<int, int> > ScriptExtendTest122::m_mOldAttr;

ScriptExtendTest122::ScriptExtendTest122(map<int, string> mStrScript)
{
	cmd = CMD_EXTEND_TEST122;
	m_mStrScript = mStrScript;
}

int ScriptExtendTest122::HandleScript()
{
	test_result = false;

	for (UInt i = 0; i < m_mStrScript.size(); i++)
	{
		map<int, int> mScript = trData->getSeidData(m_mStrScript[i].c_str());
		int iActorId = mScript[0] - 1;
		int iAttrType = mScript[1] - 1;
		int iOperator = mScript[2];
		int iType = mScript[3];
		int iValue = mScript[4];

		HistoryActor* ptrActor = history->FindBattleActor(iActorId);
		if (ptrActor)
		{
			map<int, int> mCurAttr;
			mCurAttr[SCRIPT_HP] = ptrActor->GetCurHp();
			mCurAttr[SCRIPT_MP] = ptrActor->GetCurMp();

			if (m_mOldAttr.count(iActorId))
			{
				int iReduce = m_mOldAttr[iActorId][iAttrType] - mCurAttr[iAttrType];
				if(iType == 1) //百分比
				{
					map<int, int> mTotalAttr;
					mTotalAttr[SCRIPT_HP] = ptrActor->getTotalAbility(TOTAL_HP);
					mTotalAttr[SCRIPT_MP] = ptrActor->getTotalAbility(TOTAL_MP);
					iReduce = iReduce * 100 / mTotalAttr[iAttrType];
				}
				else if(iType == 2) //数值
				{
				}

				if(compareNum(iReduce, iValue, iOperator))
				{
					m_mOldAttr[iActorId] = mCurAttr;
					test_result = true;
				}
			}
			else
			{
				m_mOldAttr[iActorId] = mCurAttr;
			}
		}
	}

	if(!get_test_result())
	{
		return -1;
	}
	return 0;
}

int ScriptExtendTest122::initPara(char*SceneData, int index)
{
	return 0;
}

bool ScriptExtendTest122::compareNum(int iValue1, int iValue2, int iOperator)
{
	switch(iOperator)
	{
	case 1: // >=
		return iValue1 >= iValue2;
	case 2: // <
		return iValue1 < iValue2;
	case 3: // ==
		return iValue1 == iValue2;
	default:
		break;
	}
	return false;
}
