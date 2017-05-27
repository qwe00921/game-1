#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "PKScreen.h"
#include "BattleWinConditionLayer.h"
#include "ActorTalkLayer.h"
int ScriptCommonInfo::HandleRScript(RedeScreenPtr scr)
{
	if(frame++ ==0 && info->length()>0)
	{
		switch(cmd)
		{
		case  CMD_NORMAL_INFO:
		{
			scr->talk_layer = ShowInfoLayer::create(info->getChar(),scr);
			ChessApplication::getInstance()->AddFunctionLayer(scr->talk_layer);
		}
			break;
		case CMD_SCENE_NAME://场所名
			//strcpy(history->scene_name,info->getChar());
		{
			scr->SetScreenName(info->getChar());
		}
			break;
		case CMD_EVENT_NAME://事件名称设定)
			strcpy(history->event_name,info->getChar());
			break;
		}
		return 1;
	}
	return 0;
}
int ScriptCommonInfo::HandleScript()
{
	if(frame++ == 0 && info->length()>0)
	{
		BattleScreenPtr ptrScreen = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		switch(cmd)
		{
		case  CMD_NORMAL_INFO:
		{
			if (isScript)
			{
				ptrScreen->m_spLabel->updateSprite(info->getChar(), LABEL_LONG);
			}
			else
			{
				ptrScreen->m_spLabel->updateSprite(info->getChar(), m_iInfoType, 0.8f);
				ptrScreen->m_spLabel->setClickHide(!m_bCanNotClick);
			}
		}
			break;
		case CMD_SCENE_NAME://场所名
		{
			strcpy(history->scene_name, info->getChar());
			ptrScreen->m_spLabel->updateSprite(info->getChar(), LABEL_SHORT);
		}
			break;
		case CMD_EVENT_NAME://事件名称设定
		{
			strcpy(history->event_name, info->getChar());
			ptrScreen->m_spLabel->updateSprite(info->getChar(), LABEL_SHORT);
		}
			break;
		case CMD_VICTORY_CONDITION:
			strcpy(history->win_condition, info->getChar());
			break;
		case CMD_SHOW_VICTORY_CONDITION:
		{
			ptrScreen->WinStrList.clear();
			ptrScreen->LostStrList.clear();
			GenerateTalkList(info->getChar(), ptrScreen);
			if (history->m_iBattleType != BATTLE_PVP)
			{
				ptrScreen->mFunctionLayer = BattleWinConditionLayer::create(ptrScreen);
				ChessApplication::getInstance()->AddFunctionLayer(ptrScreen->mFunctionLayer);
			}
		}
			break;
		}
		return 1;
	}
	return 0;
}

void ScriptCommonInfo::GenerateTalkList(const char *buf,BattleScreenPtr scr)
{
	int str_type = 0, WinStrListIndex = 0, LostStrListIndex = 0;
	const char* point = 0;
	const char *lastPoint = 0;
	int add = 1;
	lastPoint = strstr(buf, "&");
	if (lastPoint == NULL)
	{
		return;
	}
	int talk_actor_num = 1;
	point = lastPoint;
	while (point < buf + strlen(buf))
	{
		if (*lastPoint == '&')
		{
			point = strstr(lastPoint, "\n");
			if (point == NULL)
			{
				point = strstr(lastPoint, "\\n");
				if (point == NULL)
				{
					point = buf + strlen(buf);
				}
				else
				{
					add = 2;
				}
			}

			if (!strncmp("win", lastPoint + 1, point - lastPoint - 1) && strlen("win") == point - lastPoint - 1)
			{
				str_type = 1;
			}
			else if (!strncmp("lost", lastPoint + 1, point - lastPoint - 1) && strlen("lost") == point - lastPoint - 1)
			{
				str_type = 2;
			}
		}
		else
		{
			point = strstr(lastPoint, "\n");
			if (point == NULL)
			{
				point = buf + strlen(buf);
			}
			if (str_type == 1)
			{
				scr->WinStrList[WinStrListIndex++] = string(lastPoint, point - lastPoint);
			}
			else if (str_type == 2)
			{
				scr->LostStrList[LostStrListIndex++] = string(lastPoint, point - lastPoint);
			}
		}
		lastPoint = point + add;
	}
}

int ScriptCommonInfo::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length()+1;
	return index;
}
