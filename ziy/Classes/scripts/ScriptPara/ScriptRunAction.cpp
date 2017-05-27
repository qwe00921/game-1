#include "Common.h"
#include "Scripts.h"
#include "ActionLayer.h"

int ScriptRunAction::HandleScript(){
	if (frame == 0)
	{
		ActionLayer* ptrActionLayer = (ActionLayer*)ChessApplication::getInstance()->GetActionLayer();
		ptrActionLayer->RunAction(skill_anim_id, m_skill_type, m_curactor,m_oppoactor,mAnimName, m_bIsOwn);
		//need_frame = BattleControler::getBattleActor(m_actor->actor_id)->setState(m_state, m_frame);
		if (m_sound > 0)
		{
			ChessPlayNetSound(m_sound,"skill");
		}
	}
	return 0;
}
	
int ScriptRunAction::initPara(char*SceneData, int index){
	return index;
}
