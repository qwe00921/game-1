#ifndef _ScriptDispatchEnemy_H_
#define _ScriptDispatchEnemy_H_

#include "ScriptDispatchOneEnemy.h"

#define SCRIPT_MAX_ENEMY_ARMY_NUM  80

class ScriptDispatchEnemy : public Script
{
public:
	ScriptDispatchEnemy()
	{
		cmd = CMD_DISPATCH_ENEMY;
		para = new ScriptDispatchOneEnemy[SCRIPT_MAX_ENEMY_ARMY_NUM];
		m_iIs72ScriptAdd = 0;
	}
	~ScriptDispatchEnemy()
	{
		SAFE_DELETE_ARRAY(para);
	}

	ScriptDispatchOneEnemy* para;
	void SetLadderEnemyData(int i,int iActorListIndex);
	int HandleScript();
	int initPara(char*SceneData, int index);
	void InitHeroElemData(HeroElem* ptrHeroElem,int i);
	void SetImageEnemyData(int i);
	void initPVPEnemyData();

	int mActorListIndex;

	int m_iIs72ScriptAdd;
};

#endif //_ScriptDispatchEnemy_H_