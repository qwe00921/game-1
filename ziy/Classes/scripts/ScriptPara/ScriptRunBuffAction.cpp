//
//  ScriptRunBuffAction.cpp
//  TRGame
//
//  Created by Jimmy on 16/07/26.
//
//

#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "ScriptRunBuffAction.h"


ScriptRunBuffAction::ScriptRunBuffAction(HistoryActor* ptrActor, int iBuffId)
{
	m_ptrActor = ptrActor;
	m_iBuffId = iBuffId;
}

int ScriptRunBuffAction::HandleScript()
{
	BattleScreen* ptrScreen = (BattleScreen*)chess->getScreenByID(SCREEN_BATTLE);
	if(ptrScreen)
	{
		ptrScreen->addArmBuff(m_ptrActor, m_iBuffId);
	}
	return 0;
}