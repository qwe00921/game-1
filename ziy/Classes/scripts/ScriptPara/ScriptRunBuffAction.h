//
//  ScriptRunBuffAction.h
//  TRGame
//
//  Created by Jimmy on 16/07/26.
//
//

#ifndef _ScriptRunBuffAction_H_
#define _ScriptRunBuffAction_H_

class ScriptRunBuffAction : public Script
{
public:
	ScriptRunBuffAction(HistoryActor* ptrActor, int iBuffId);
	int HandleScript();
public:
	HistoryActor* m_ptrActor;
	int m_iBuffId;
};

#endif //_ScriptRunBuffAction_H_