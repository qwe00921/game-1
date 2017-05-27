#include "Common.h"
#include "main.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "PKScreen.h"
int ScriptShowChoice::HandleRScript(RedeScreenPtr scr)
{
	if(frame++ == 0)
	{
		if(scr != NULL)
		{
			if(actor_id < 0)
			{
				actor_id = 0;
			}
			history->m_bWaitCommand = false; //对话选择时禁用MenuBar
			scr->m_pChoiceBox = ChoiceBoxLayer::create(actor_id, info->getChar());
			scr->m_pRedeLayer->addChild(scr->m_pChoiceBox, 2);
		}
		return 1;
	}
	return 0;
}
int ScriptShowChoice::HandleScript()
{
	if(frame++ == 0)
	{
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		if(scr != NULL)
		{
			if(actor_id < 0)
			{
				actor_id = 0;
			}
			history->m_bWaitCommand = false; //对话选择时禁用MenuBar
			scr->m_pChoiceBox = ChoiceBoxLayer::create(actor_id, info->getChar());
			scr->m_pBattleLayer->addChild(scr->m_pChoiceBox, 2);
		}
		return 1;
	}
	return 0;
}

int ScriptShowChoice::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length() + 1;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
