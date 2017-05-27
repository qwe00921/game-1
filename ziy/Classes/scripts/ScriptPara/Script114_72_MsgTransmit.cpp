#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "WinConditionTool.h"
#include "ActorJoinOwnLayer.h"
#include "ActionLayer.h"
#include "Script71_47_DispatchEnemy.h"

enum
{
	FUNC_SKILL_TORANGE = 3,
	FUNC_SKILL_TODO,
	FUNC_SKILL_LEARNED,
	FUNC_MOVE_RANGE,
	FUNC_SETUP_TIMEELEM,
	FUNC_SETUP_ARRAYSYS,
	FUNC_SEID_TOACTOR,
	FUNC_SEID_TOGOODS,
	FUNC_BATTLE_FACELIFT,
	FUNC_ENEMYABILITYSET = 80,
	FUNC_NETEQUIPLEVEL = 87,
	FUNC_NETEXTERNALEQUIP = 88,
	FUNC_NETELITEEQUIP = 89,
	FUNC_ACOTORDELECTED = 99,
	FUNC_ADD_ACTORORGOODS = 100,
	FUNC_RAND_ENEMY = 111,
	FUNC_RANGE_VISIBAL = 112,
	FUNC_INIT_FASHION = 120,
	FUNC_CHANGE_FASHION = 121,
	FUNC_CHANGE_ABILITY = 160,
	FUN_ADD_RUNSKILLACTION = 200,
	FUN_SET_BATTLEIMAGEDATA = 201,
	FUN_SET_BATTLEIMAGEHIDE = 202,
	FUN_ADD_RUNSKILLACTION_BYBOSS = 210,
	FUN_SET_WEATHER = 300,
	FUN_SET_REALWEATHER = 301,
	FUN_TEST_EQUIP_NUM = 333,
	FUN_TEST_QUALITY_ACTOR_NUM = 334,
	FUNC_CHANGE_STATE = 400,
	FUNC_SCRIPT_VALIDATE = 999,
	FUNC_MAX
};

/********************************************************/
//1、对指定人物释放法术
//2、对指定地点释放法术
//3、对指定范围释放法术
//4、未定义
//5、指定武将习得指定法术
//6、指定武将在指定范围移动
//7、指定当前时间元素
//8、指定当前阵法系统
//11、战场换装备
//80、敌军属性调整
//88、自动配装
//89、精英自动配装
//99针对78指令强制将友军变为可控，移除武将
//100、战场获得武将物品等
//111、添加随机武将
//112、显示范围武将
//120、战场初始化时装
//121、战场变更时装
//200、播放技能动画
//201、设置镜像关卡数据
//202、设置镜像关卡武将撤退
//210、在武将身上播放动画
//400、战场武将状态变更HP、MP
//999、剧本数据校验
/********************************************************/

int ScriptMsgTransmit::HandleScript()
{
	if(!frame)
	{
		GetStringList();
	}

	switch(function_id)
	{
	case FUNC_ADD_ACTORORGOODS:
		ShowGetGoodsAction();
		break;
	case FUNC_CHANGE_FASHION:
	case FUNC_CHANGE_STATE:
	{
		if (frame == 0)
		{
			m_iScriptIndex = 0;
			need_frame = 9999;
		}

		if(m_iScriptIndex < m_listScript->size())
		{
			Script* ptrScript = m_listScript->elementAt(m_iScriptIndex);
			if(!ptrScript->HandleScript())
			{
				m_iScriptIndex++;
			}
		}
		else
		{
			need_frame = frame;
		}
	}
	break;
	case FUN_SET_BATTLEIMAGEHIDE:
	{
		if(frame == need_frame)
		{
			if(iImageAListIndex != mImageActorList.size())
			{
				frame = 0;
				BattleActorPtr battle_actor = BattleControler::getBattleActor(mImageActorList[iImageAListIndex++]);
				if(battle_actor && battle_actor->hisActor != NULL && battle_actor->hisActor->hide != 1)//隐藏或死亡的不进行重新消失
				{
					need_frame = battle_actor->setState(ACTOR_STATE_LEAVE_HIDE);
				}
			}
		}
	}
		break;
	case FUN_ADD_RUNSKILLACTION_BYBOSS:
	{
		if (frame == need_frame&&m_iBossHide)
		{
			need_frame = 2;
			frame = 1;
		}
	}
		break;
	case FUNC_RANGE_VISIBAL:
	{
		if (frame == need_frame && m_iRandVisibal < (int)mRandVisibal.size())
		{
			history->FindNearestBlackPos(mRandVisibal[m_iRandVisibal], mRandVisibal[m_iRandVisibal]->x, mRandVisibal[m_iRandVisibal]->y);
			mRandVisibal[m_iRandVisibal]->hide = 0;
			mRandVisibal[m_iRandVisibal]->is_leave = 0;
			history->CenterActor(mRandVisibal[m_iRandVisibal]);
			m_iRandVisibal++;
			playSound(SOUND_ACTOR_APPEAR);
			need_frame += TIMER_ACTOR_APPEAR;
		}
	}
		break;
	default:
		break;
	}

	return need_frame - frame++;
}

void ScriptMsgTransmit::GetActorOrPoint(char *buf)
{
	char* point = 0, *lastPoint = 0;
	int add = 1;
	point = lastPoint = buf;
	while(point < buf + strlen(buf))
	{
		add = 1;
		if(function_id == 6)
		{
			point = strstr(lastPoint, ",");
		}
		else
		{
			point = strstr(lastPoint, "|");
		}
		if(point == NULL)
		{
			point = buf + strlen(buf);
		}
		ActorOrPointList->addElement(new DString(lastPoint, point - lastPoint));
		lastPoint = point + add;
	}
	actor_num = ActorOrPointList->size();
}
void ScriptMsgTransmit::GetStringList()
{
	char* point = 0, *lastPoint = 0;
	int add = 1;
	char *buf = msgData->getChar();
	DList<DString>* StringList = new DList<DString>(false);
	point = lastPoint = buf;
	while(point < buf + strlen(buf))
	{
		add = 1;
		point = strstr(lastPoint, "\n");
		if(point == NULL)
		{
			point = buf + strlen(buf);
		}
		StringList->addElement(new DString(lastPoint, point - lastPoint));
		lastPoint = point + add;
	}

	switch(function_id)
	{
	case FUNC_SKILL_LEARNED:
	{
		if(StringList->size() < 2)
		{
			for(int i = StringList->size() - 1; i >= 0; i--)
				ChessFree(StringList->removeElementAt(i));
			return;
		}
		short instructions_5_actor_id = StringToInt(StringList->elementAt(0)->getChar());
		char *buf = StringList->elementAt(1)->getChar();

		HistoryActorPtr instructions_5_actor = NULL;
		if(instructions_5_actor_id - 1 >= 0 && instructions_5_actor_id - 1 < trData->actor_num)
		{
			for(int k = 0; k < history->battleEnemyNum; k++)
			{
				if(history->battleEnemy[k]->actor_id == instructions_5_actor_id - 1 && instructions_5_actor == NULL)
				{
					instructions_5_actor = history->battleEnemy[k];
					break;
				}
			}
			for(int k = 0; k < history->battleFriendNum; k++)
			{
				if(history->battleFriend[k]->actor_id == instructions_5_actor_id - 1 && instructions_5_actor == NULL)
				{
					instructions_5_actor = history->battleFriend[k];
					break;
				}
			}
			for(int k = 0; k < history->battleOwnNum; k++)
			{
				if(history->battleOwn[k]->actor_id == instructions_5_actor_id - 1 && instructions_5_actor == NULL)
				{
					instructions_5_actor = history->battleOwn[k];
					break;
				}
			}
		}
		if(instructions_5_actor)
		{
			int next_seid = 0;
			int test_len = strlen(buf);
			int j = 0, k = 0, m = 0, l = 0;
			while(j < test_len)
			{
				next_seid = StringToInt((char*)buf + j);
				j++;
				j++;
				if(next_seid >= 10)
				{
					j++;
				}
				if(next_seid >= 100)
				{
					j++;
				}
				m++;
			}
			instructions_5_actor->set_hero_seid.resize(m);
			while(k < test_len)
			{
				next_seid = StringToInt((char*)buf + k);
				k++;
				k++;
				if(next_seid >= 10)
				{
					k++;
				}
				if(next_seid >= 100)
				{
					k++;
				}
				instructions_5_actor->set_hero_seid[l++] = next_seid;
			}
			instructions_5_actor->UpdataGoodEffect();
		}
	}
		break;
	case FUNC_MOVE_RANGE:
	{
		if(StringList->size() < 2)
		{
			for(int i = StringList->size() - 1; i >= 0; i--)
				ChessFree(StringList->removeElementAt(i));
			return;
		}
		int limit_min_x = 0, limit_max_x = 0, limit_min_y = 0, limit_max_y = 0, temp = 0, num = 0, isenable = 0;
		char* point = NULL;
		char* buf = StringList->elementAt(0)->getChar();
		for(int i = 0; i < (int)strlen(buf) + 1; i++)
		{
			if(i < (int)strlen(buf) && *(buf + i) >= '0'&&*(buf + i) <= '9')
			{
				temp = temp * 10 + (*(buf + i) - '0');
				isenable = 1;
			}
			else if(isenable)
			{
				switch(num)
				{
				case 0:
					limit_min_x = temp;
					num++;
					break;
				case 1:
					limit_min_y = temp;
					num++;
					break;
				case 2:
					limit_max_x = temp;
					num++;
					break;
				case 3:
					limit_max_y = temp;
					num++;
					break;
				default:
					break;
				}
				isenable = 0;
				temp = 0;
			}
		}

		if(num > 3)
		{
			if(limit_max_x < limit_min_x)
			{
				temp = limit_max_x;
				limit_max_x = limit_min_x;
				limit_min_x = temp;
			}
			if(limit_max_y < limit_min_y)
			{
				temp = limit_max_y;
				limit_max_y = limit_min_y;
				limit_min_y = temp;
			}
			int actor_id = 0;
			GetActorOrPoint(StringList->elementAt(1)->getChar());
			for(int j = 0; j < ActorOrPointList->size(); j++)
			{
				actor_id = StringToInt(ActorOrPointList->elementAt(j)->getChar());
				if(actor_id < trData->actor_num&&actor_id >= 0)
				{
					for(int k = 0; k < history->battleEnemyNum; k++)
					{
						if(history->battleEnemy[k]->actor_id == actor_id)
						{
							history->battleEnemy[k]->limit_actor_move = history->game_scene + 1;
							history->battleEnemy[k]->limit_actor_move_x1 = limit_min_x;
							history->battleEnemy[k]->limit_actor_move_y1 = limit_min_y;
							history->battleEnemy[k]->limit_actor_move_x2 = limit_max_x;
							history->battleEnemy[k]->limit_actor_move_y2 = limit_max_y;
						}
					}
					for(int k = 0; k < history->battleFriendNum; k++)
					{
						if(history->battleFriend[k]->actor_id == actor_id)
						{
							history->battleFriend[k]->limit_actor_move = history->game_scene + 1;
							history->battleFriend[k]->limit_actor_move_x1 = limit_min_x;
							history->battleFriend[k]->limit_actor_move_y1 = limit_min_y;
							history->battleFriend[k]->limit_actor_move_x2 = limit_max_x;
							history->battleFriend[k]->limit_actor_move_y2 = limit_max_y;
						}
					}
					for(int k = 0; k < history->battleOwnNum; k++)
					{
						if(history->battleOwn[k]->actor_id == actor_id)
						{
							history->battleOwn[k]->limit_actor_move = history->game_scene + 1;
							history->battleOwn[k]->limit_actor_move_x1 = limit_min_x;
							history->battleOwn[k]->limit_actor_move_y1 = limit_min_y;
							history->battleOwn[k]->limit_actor_move_x2 = limit_max_x;
							history->battleOwn[k]->limit_actor_move_y2 = limit_max_y;
						}
					}
				}
			}
			ActorOrPointList->removeAllElements();
		}
	}
		break;
	case FUNC_SETUP_TIMEELEM:
		break;
	case FUNC_SETUP_ARRAYSYS:
		break;
	case FUNC_SEID_TOACTOR:
	{
							  if (StringList->size() < 3)
							  {
								  for (int i = StringList->size() - 1; i >= 0; i--)
									  ChessFree(StringList->removeElementAt(i));
								  return;
							  }
							  char* seidbuf = StringList->elementAt(0)->getChar();
							  int add_or_cancel = StringToInt(StringList->elementAt(1)->getChar());//0取消，1习得
							  int actorID = StringToInt(StringList->elementAt(2)->getChar());

							  if (actorID - 1 >= 0 && actorID - 1 < trData->actor_num)
							  {
								  ActorAcquisitionSeid(actorID, add_or_cancel, seidbuf);
							  }
	}
		break;
	case FUNC_SEID_TOGOODS:
	{
							  if (StringList->size() < 3)
							  {
								  for (int i = StringList->size() - 1; i >= 0; i--)
									  ChessFree(StringList->removeElementAt(i));
								  return;
							  }
							  char* seidbuf = StringList->elementAt(0)->getChar();
							  int add_or_cancel = StringToInt(StringList->elementAt(1)->getChar());//0取消，1习得
							  int goodsId = StringToInt(StringList->elementAt(2)->getChar());

							  if(trData->m_mGoodsItem[goodsId])
							  {
								  GoodsAcquisitionSeid(goodsId-1, add_or_cancel, seidbuf);
							  }
	}
		break;
	case FUNC_BATTLE_FACELIFT:
		break;
	case FUNC_ENEMYABILITYSET:
	{
		for (int i = 0; i < StringList->size(); i++)
		{
			map<int, int> SetNumM = trData->getSeidData(StringList->elementAt(i)->getChar());
			if ((int)SetNumM.size() > 0)
				history->mEnemyElitePlus[SetNumM[0]] = SetNumM;
		}

		int ieActor = 0;
		for (int i = 0; i < history->battleEnemyNum; i++)
		{
			ieActor = history->battleEnemy[i]->actor_id+1;
			if (history->mNet72EquipActor.count(ieActor))
			{
				history->battleEnemy[i]->InitFiveAllAbility();
			}
		}
		for (int i = 0; i < history->battleFriendNum; i++)
		{
			ieActor = history->battleFriend[i]->actor_id + 1;
			if (history->mNet72EquipActor.count(ieActor))
			{
				history->battleFriend[i]->InitFiveAllAbility();
			}
		}
	}
		break;
	case FUNC_NETEQUIPLEVEL:
	{
		if (StringList->size()>0)
		{
			history->mNet72EquipLevel = trData->getSeidData(StringList->elementAt(0)->getChar());
		}
	}
		break;
	case FUNC_NETEXTERNALEQUIP:
	{
		for (int i = 0; i<StringList->size(); i++)
		{
			char* ptrBuff = StringList->elementAt(i)->getChar();
			char* ptrPoint = strstr(ptrBuff, "&");
			if (!ptrPoint)
				continue;
			int iActorId = StringToInt(ptrBuff) - 1;
			int iEquipId = StringToInt(ptrPoint+1);
			history->mNet72EquipActor[iActorId + 1] = 1;
			history->SetNetExternalEquip(iActorId, iEquipId);
		}
	}
		break;
	case FUNC_NETELITEEQUIP:
	{
		for (int i = 0; i < StringList->size(); i++)
		{
			char* ptrBuff = StringList->elementAt(i)->getChar();
			char* ptrPoint = strstr(ptrBuff, "&");
			if (!ptrPoint)
				continue;
			int iActorId = StringToInt(ptrBuff) - 1;
			int iEquipId = StringToInt(ptrPoint + 1);
			history->mNet72EquipActor[iActorId + 1] = 1;
			history->SetNetExternalEquip(iActorId, iEquipId,1);
		}
	}
		break;
	case FUNC_SCRIPT_VALIDATE:
	{
			if (StringList->size() > 0)
			{
				int iScriptV = StringToInt(StringList->elementAt(0)->getChar());
				if (iScriptV >= 0)
					history->mScriptValidate = iScriptV;
			}
	}
		break;
    case FUNC_ACOTORDELECTED:
    {
		map<int, int> iLeaveActorList;
		int iActorId = 0;
        for(int i=0;i<StringList->size();i++)
        {
			iActorId = StringToInt(StringList->elementAt(i)->getChar());
			iLeaveActorList[iActorId] = 1;
        }

		for (int i = 0; i < history->battleOwnNum; i++)
		{
			if (iLeaveActorList.count(history->battleOwn[i]->actor_id+1))
			{
				history->battleOwn[i]->recoverState();
				if (history->battleOwn[i]->controlled_by_enemy)
					history->battleOwn[i]->delete_controlled_by_enemy();
				else
				{
					for (int j = i; j < history->battleOwnNum - 1; j++)
					{
						history->battleOwn[j] = history->battleOwn[j + 1];
					}
					history->battleOwn[history->battleOwnNum - 1] = NULL;
					history->battleOwnNum--;
				}
				i--;
			}
		}
    }
        break;
	case FUNC_ADD_ACTORORGOODS:
	{
		if (StringList->size())
		{
			iNetGetGoodsIndex = 0;
			bNetEnableGetNext = false;
			NetAddActorOrGoods(StringList);
		}

	}
		break;
	case FUNC_RAND_ENEMY:
	{
		if (StringList->size() > 3)
		{
			DispatchRandEnemy(StringList);
		}
	}
		break;
	case FUNC_RANGE_VISIBAL:
	{
		if (StringList->size())
		{
			map<int, map<int, int> > mPoint = trData->getMapJsonData(StringList->elementAt(0)->getChar());

			if (mPoint.size())
			{
				int iMinX = 0, iMaxX = 0, iMinY = 0, iMaxY = 0;

				iMinX = mPoint[0][0];
				iMaxX = mPoint[1][0];

				iMinY = mPoint[0][1];
				iMaxY = mPoint[1][1];

				if (iMaxX < iMinX)
				{
					iMaxX = mPoint[0][0];
					iMinX = mPoint[1][0];
				}

				if (iMaxY < iMinY)
				{
					iMaxY = mPoint[0][1];
					iMinY = mPoint[1][1];
				}

				HistoryActorPtr oppoActor;
				int iIndex = 0;
				for (int i = iMinX; i <= iMaxX; i++)
				{
					for (int j = iMinY; j <= iMaxY; j++)
					{
						for (int k = 0; k < history->battleEnemyNum; k++)
						{
							if (history->battleEnemy[k]->x == i&&history->battleEnemy[k]->y == j)
							{
								oppoActor = history->battleEnemy[k];
								if (oppoActor&&oppoActor->hide&&oppoActor->GetCurHp() > 0)
								{
// 									history->FindNearestBlackPos(oppoActor, oppoActor->x, oppoActor->y);
// 									oppoActor->hide = 0;
// 									oppoActor->is_leave = 0;
// 									history->CenterActor(oppoActor);
									mRandVisibal[iIndex++] = oppoActor;
								}
								//break;
							}
						}
					}
				}
			}
			frame = 0;
			need_frame = 0;
			m_iRandVisibal = 0;
		}
		
	}
		break;
	case FUNC_INIT_FASHION:
	{
		for (int i = 0; i < StringList->size(); i++)
		{
			string strContent = StringList->elementAt(i)->getChar();
			map<int, string> mStrTemp = trData->getStringMap(strContent, '|');
			int iActorId = StringToInt(mStrTemp[0].c_str());
			map<int, int> mFashion = trData->getSeidData(mStrTemp[1].c_str());

			HistoryActor* ptrActor = history->FindBattleActor(iActorId - 1);
			if (ptrActor && ptrActor->relation != ACTOR_RELATION_OWN)
			{
				for(UInt i = 0; i < mFashion.size(); i++)
				{
					HistoryGoods hisGoods(mFashion[i]);
					if(hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
					{
						ptrActor->fashion.setGood(&hisGoods);
					}
					else if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
					{
						ptrActor->wap_fashion.setGood(&hisGoods);
					}
				}
				ptrActor->UpdataGoodEffect();
			}
		}
	}
		break;
	case FUNC_CHANGE_FASHION:
	{
		for(int i = 0; i < StringList->size(); i++)
		{
			string strContent = StringList->elementAt(i)->getChar();
			map<int, string> mStrTemp = trData->getStringMap(strContent, '|');
			int iActorId = StringToInt(mStrTemp[0].c_str());
			map<int, int> mFashion = trData->getSeidData(mStrTemp[1].c_str());

			HistoryActor* ptrActor = history->FindBattleActor(iActorId - 1);
			if(ptrActor && ptrActor->relation != ACTOR_RELATION_OWN)
			{
				for(UInt i = 0; i < mFashion.size(); i++)
				{
					HistoryGoods hisGoods(mFashion[i]);
					if(hisGoods.GetGoodsType() == GOOD_TYPE_FASHING || hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
					{
						m_listScript->addElement(new ScriptGetGood(ptrActor->actor_id, hisGoods.good_id, 1));
					}
				}
			}
		}
	}
		break;
	case FUNC_CHANGE_ABILITY:
	{
		for(int i = 0; i < StringList->size(); i++)
		{
			string strContent = StringList->elementAt(i)->getChar();
			map<int, string> mStrTemp = trData->getStringMap(strContent, '|');

			int iRelation = StringToInt(mStrTemp[0].c_str());
			int iAddType = StringToInt(mStrTemp[1].c_str());
			map<int, int> mActorId = trData->getSeidData(mStrTemp[2].c_str());
			map<int, int> mAbility = trData->getSeidData(mStrTemp[3].c_str());

			map<int, int> mChangeId;
			if(iRelation == 0)
			{
				mChangeId = mActorId;
			}
			else if(iRelation < 5)
			{
				int iStartId = mActorId[0];
				int iEndId = mActorId[1];

				int arrInt[5] = {0, 1, 2, 4, 7};
				int iValue = arrInt[iRelation];
				int iIndex = 0;
				if (iValue | 1)
				{
					for(int i = 0; i < history->battleOwnNum; i++)
					{
						int iActorId = history->battleOwn[i]->actor_id + 1;
						if (iActorId >= iStartId && iActorId <= iEndId)
						{
							mChangeId[iIndex++] = iActorId;
						}
					}
				}
				if (iValue | 2)
				{
					for(int i = 0; i < history->battleFriendNum; i++)
					{
						int iActorId = history->battleFriend[i]->actor_id + 1;
						if(iActorId >= iStartId && iActorId <= iEndId)
						{
							mChangeId[iIndex++] = iActorId;
						}
					}
				}
				if (iValue | 4)
				{
					for(int i = 0; i < history->battleEnemyNum; i++)
					{
						int iActorId = history->battleEnemy[i]->actor_id + 1;
						if(iActorId >= iStartId && iActorId <= iEndId)
						{
							mChangeId[iIndex++] = iActorId;
						}
					}
				}
			}

			for (UInt i = 0; i < mChangeId.size(); i++)
			{
				HistoryActor* ptrActor = history->FindBattleActor(mChangeId[i] - 1);
				if (ptrActor)
				{
					ptrActor->addBaseAbility(mAbility, iAddType);
				}
			}
		}
	}
	break;
	case FUN_ADD_RUNSKILLACTION:
	{
		if (StringList->size())
		{
			char* ptrBuff = StringList->elementAt(0)->getChar();
			char* ptrPoint1 = strstr(ptrBuff, "&");
			char* ptrPoint2 = strstr(ptrPoint1+1, "&");
			if (!ptrPoint1 || !ptrPoint2)
				break;
			RunSkillAction(StringToInt(ptrBuff), StringToInt(ptrPoint1+1), StringToInt(ptrPoint2+1));
		}
	}
		break;
	case FUN_ADD_RUNSKILLACTION_BYBOSS:
	{
		if (StringList->size())
		{
			char* ptrBuff = StringList->elementAt(0)->getChar();
			char* ptrPoint1 = strstr(ptrBuff, "|");
			char* ptrPoint2 = strstr(ptrPoint1 + 1, "|");
			if (!ptrPoint1 || !ptrPoint2)
				break;
			RunSkillBossAction(StringToInt(ptrBuff), StringToInt(ptrPoint1 + 1), StringToInt(ptrPoint2 + 1));
		}
	}
		break;
	case FUN_SET_BATTLEIMAGEDATA:
	{
		char* ptrBuff = NULL;
		map<int, int> mEnemyActor;
		for (int i = 0; i < StringList->size(); i++)
		{
			ptrBuff = StringList->elementAt(i)->getChar();
			map<int,int> ptrBuffMap = trData->getSeidData(ptrBuff);

			if(!ptrBuffMap[0]|| ptrBuffMap[0] > history->battleOwnNum)
				continue;

			for (size_t j = 1; j < ptrBuffMap.size(); j++)
			{
				mEnemyActor[ptrBuffMap[j]] = history->battleOwn[ptrBuffMap[0] - 1]->actor_id+1;
			}
		}
		history->mMapDateInfo->GetBattleImageData()->SetImageActorList(mEnemyActor);
	}
		break;
	case FUN_SET_BATTLEIMAGEHIDE:
	{
		int iOutId = 0, iType = 0;
		int iActorIndex = 0;
		for (int i = 0; i < StringList->size(); i++)
		{
			char* ptrBuff = StringList->elementAt(i)->getChar();
			char* ptrPoint1 = strstr(ptrBuff, "&");

			iOutId = StringToInt(ptrBuff);
			iType = StringToInt(ptrPoint1 + 1);

			if (iType == 1 && iOutId <= history->battleOwnNum)
			{
				mImageActorList[iActorIndex++] = history->battleOwn[iOutId - 1]->actor_id;
			}
// 			else
// 			{
// 
// 			}
		}

		if(iActorIndex)
			need_frame = 20;
	}
		break;
	case FUN_SET_WEATHER:
	{
		if (StringList->size())
		{
			for (int i = 0; i < StringList->size(); i++)
			{
				char* ptrBuff = StringList->elementAt(i)->getChar();
				map<int,int> mDataWea = trData->getGoodsMapData(ptrBuff);

				for (map<int, int>::iterator it = mDataWea.begin(); it != mDataWea.end(); it++)
				{
					history->mWeather[it->first] = it->second;
				}
			}
			//if (history->GetCurRound() <= 1)
			{
				history->changeAir(-1);
			}
		}
	}
		break;
	case FUN_SET_REALWEATHER:
	{
		if (StringList->size())
		{
			history->mRealWeather.clear();
			for (int i = 0; i < StringList->size(); i++)
			{
				char* ptrBuff = StringList->elementAt(i)->getChar();
				map<int, int> mDataWea = trData->getGoodsMapData(ptrBuff);

				for (map<int, int>::iterator it = mDataWea.begin(); it != mDataWea.end(); it++)
				{
					if(it->first)
						history->mRealWeather[it->first] = it->second;
				}
			}
			if (history->mRealWeather.size())
			{
				history->mRealWeather[0] = history->GetCurRound();
				history->changeAir(-1);
			}
		}
	}
		break;
	case FUN_TEST_EQUIP_NUM:
	{
		if (StringList->size())
		{
			int iValueNum = 0, iGoodsId = 0, iRelation = 0, iItid = 0,iNum = 0,iIndex = 0;
			HistoryActorPtr ptrActor = NULL;
			map<int, HistoryActorPtr> mHeroList;
			for (int i = 0; i < StringList->size(); i++)
			{
				map<int, int> mTestData = trData->getSeidData(StringList->elementAt(i)->getChar());
				mHeroList.clear();
				iValueNum = mTestData[0];
				iRelation = mTestData[1];
				iItid = mTestData[2];
				iGoodsId = mTestData[3];

				if (iRelation == 1|| iRelation == 4 || iRelation == 5)
				{
					for (int j = 0; j < history->battleOwnNum; j++)
					{
						ptrActor = history->battleOwn[j];
						if (!ptrActor->hide&&!ptrActor->controlled_by_enemy)
						{
							mHeroList[iIndex++] = ptrActor;
						}
					}
				}

				if (iRelation == 2 || iRelation == 4 || iRelation == 5)
				{
					for (int j = 0; j < history->battleFriendNum; j++)
					{
						ptrActor = history->battleFriend[j];
						if (!ptrActor->hide)
						{
							mHeroList[iIndex++] = ptrActor;
						}
					}
				}

				if (iRelation == 3 || iRelation == 5)
				{
					for (int j = 0; j < history->battleEnemyNum; j++)
					{
						ptrActor = history->battleEnemy[j];
						if (!ptrActor->hide)
						{
							mHeroList[iIndex++] = ptrActor;
						}
					}
					for (int j = 0; j < history->battleOwnNum; j++)
					{
						ptrActor = history->battleOwn[j];
						if (!ptrActor->hide && ptrActor->controlled_by_enemy)
						{
							mHeroList[iIndex++] = ptrActor;
						}
					}
				}

				for (int k = 0; k < iIndex; k++)
				{
					ptrActor = mHeroList[k];

					if (ptrActor->weapon.isNormalGood() && (iGoodsId > 0 && iGoodsId == ptrActor->weapon.good_id || iGoodsId == 0 && iItid == ptrActor->weapon.getItId()))
					{
						iNum++;
					}
					if (ptrActor->coat.isNormalGood() && (iGoodsId > 0 && iGoodsId == ptrActor->coat.good_id || iGoodsId == 0 && iItid == ptrActor->coat.getItId()))
					{
						iNum++;
					}
					if (ptrActor->assist_good.isNormalGood() && (iGoodsId > 0 && iGoodsId == ptrActor->assist_good.good_id || iGoodsId == 0 && iItid == ptrActor->assist_good.getItId()))
					{
						iNum++;
					}
					if (ptrActor->fashion.isNormalGood() && (iGoodsId > 0 && iGoodsId == ptrActor->fashion.good_id || iGoodsId == 0 && iItid == ptrActor->fashion.getItId()))
					{
						iNum++;
					}
					if (ptrActor->wap_fashion.isNormalGood() && (iGoodsId > 0 && iGoodsId == ptrActor->wap_fashion.good_id || iGoodsId == 0 && iItid == ptrActor->wap_fashion.getItId()))
					{
						iNum++;
					}
				}

				history->var2[iValueNum] = iNum;
			}
			
		}
	}
		break;
	case FUN_TEST_QUALITY_ACTOR_NUM:
	{
		if (StringList->size())
		{
			int iValueNum = 0,iRelation = 0,iNum = 0,iQuality = 0;
			HistoryActorPtr ptrActor = NULL;
			for (int i = 0; i < StringList->size(); i++)
			{
				map<int, int> mTestData = trData->getSeidData(StringList->elementAt(i)->getChar());

				iValueNum = mTestData[0];
				iRelation = mTestData[1];
				iQuality = mTestData[2];

				if (iRelation == 1 || iRelation == 4 || iRelation == 5)
				{
					for (int j = 0; j < history->battleOwnNum; j++)
					{
						ptrActor = history->battleOwn[j];
						if (!ptrActor->hide && !ptrActor->controlled_by_enemy&&ptrActor->actor_quality>= iQuality)
						{
							iNum++;
						}
					}
				}

				if (iRelation == 2 || iRelation == 4 || iRelation == 5)
				{
					for (int j = 0; j < history->battleFriendNum; j++)
					{
						ptrActor = history->battleFriend[j];
						if (!ptrActor->hide&&ptrActor->actor_quality >= iQuality)
						{
							iNum++;
						}
					}
				}

				if (iRelation == 3 || iRelation == 5)
				{
					for (int j = 0; j < history->battleEnemyNum; j++)
					{
						ptrActor = history->battleEnemy[j];
						if (!ptrActor->hide&&ptrActor->actor_quality >= iQuality)
						{
							iNum++;
						}
					}
					for (int j = 0; j < history->battleOwnNum; j++)
					{
						ptrActor = history->battleOwn[j];
						if (!ptrActor->hide && ptrActor->controlled_by_enemy&&ptrActor->actor_quality >= iQuality)
						{
							iNum++;
						}
					}
				}

				history->var2[iValueNum] = iNum;
			}
		}
	}
		break;
	case FUNC_CHANGE_STATE:
	{
		if (StringList->size() >= 4)
		{
			//HP、MP
			int iAttrId = StringToInt(StringList->elementAt(0)->getChar());
			//数值百分比
			string strValue1 = StringList->elementAt(1)->getChar();
			map<int, int> mValue1 = trData->getSeidData(strValue1.c_str());
			//坐标范围
			string strValue2 = StringList->elementAt(2)->getChar();
			map<int, string> mstrPoint = trData->getStringMap(strValue2, '|');
			map<int, int> mStrart = trData->getSeidData(mstrPoint[0].c_str());
			map<int, int> mEnd = trData->getSeidData(mstrPoint[1].c_str());
			Vec2 vecStart = Vec2(mStrart[0], mStrart[1]);
			Vec2 vecEnd = Vec2(mEnd[0], mEnd[1]);
			//武将ID范围
			string strValue3 = StringList->elementAt(3)->getChar();
			map<int, int> mValue3 = trData->getSeidData(strValue3.c_str());
			int iStartId = mValue3[0] - 1;
			int iEndId = mValue3[1] - 1;

			map<int, HistoryActor*> mBattleActor;
			int iIndex = 0;
			for (int x = vecStart.x; x <= vecEnd.x; x++)
			{
				for (int y = vecStart.y; y <= vecEnd.y; y++)
				{
					HistoryActor* ptrActor = history->FindActorPosition(x, y);
					if (ptrActor && ptrActor->actor_id >= iStartId && ptrActor->actor_id <= iEndId)
					{
						mBattleActor[iIndex++] = ptrActor;
					}
				}
			}

			for (UInt i = 0; i < mBattleActor.size(); i++)
			{
				HistoryActor* ptrActor = mBattleActor[i];
				int iAddHp = 0;
				int iAddMp = 0;
				if (iAttrId == 1) //HP变化
				{
					iAddHp = mValue1[1];
					if (mValue1[0] == 1) //百分比
					{
						iAddHp = ptrActor->getTotalAbility(TOTAL_HP) * iAddHp / 100;
					}
				}
				else if (iAttrId == 2) //MP变化
				{
					iAddMp = mValue1[1];
					if (mValue1[0] == 1) //百分比
					{
						iAddMp = ptrActor->getTotalAbility(TOTAL_MP) * iAddMp / 100;
					}
				}

				if (iAddHp + ptrActor->GetCurHp() <= 0) //减少血量保护，最多还剩1血
				{
					iAddHp = -ptrActor->GetCurHp() + 1;
				}

				if (iAddHp || iAddMp)
				{
					m_listScript->addElement(new ScriptChangeActorState(ptrActor->actor_id, -iAddHp, -iAddMp, 0, 0, 0, 0));
				}
			}
		}
	}
	break;
	default:
		break; 
	}

	//////clean StringList
	{
		int i;
		for(i = StringList->size() - 1; i >= 0; i--)
			ChessFree(StringList->removeElementAt(i));
		delete StringList;
	}
	///////
}
void ScriptMsgTransmit::SelectActor()
{
	if(actor_num&&function_id == 1)
	{
		if(actor == NULL)
		{
			for(int j = 0; j < history->battleEnemyNum; j++)
			{
				if(!history->battleEnemy[j]->hide&&history->battleEnemy[j]->actor_id == StringToInt(ActorOrPointList->elementAt(actor_index)->getChar()) - 1)
				{
					actor = history->battleEnemy[j];
					break;
				}
			}
		}
		if(actor == NULL)
		{
			for(int j = 0; j < history->battleFriendNum; j++)
			{
				if(!history->battleFriend[j]->hide&&history->battleFriend[j]->actor_id == StringToInt(ActorOrPointList->elementAt(actor_index)->getChar()) - 1)
				{
					actor = history->battleFriend[j];
					break;
				}
			}
		}
		if(actor == NULL)
		{
			for(int j = 0; j < history->battleOwnNum; j++)
			{
				if(!history->battleOwn[j]->hide&&history->battleOwn[j]->actor_id == StringToInt(ActorOrPointList->elementAt(actor_index)->getChar()) - 1)
				{
					actor = history->battleOwn[j];
					break;
				}
			}
		}
	}
	else if(actor_num&&function_id == 2)
	{
		int dest_x = -1, dest_y = -1;
		char* point = NULL;
		char* buf = ActorOrPointList->elementAt(actor_index)->getChar();
		int str_num = strlen(buf);
		point = strstr(buf, ",");
		if(point)
		{
			DStringPtr StringPoint_X = new DString(buf, point - buf);
			DStringPtr StringPoint_Y = new DString(point + 1, str_num - (point - buf) - 1);
			dest_x = StringToInt(StringPoint_X->getChar());
			dest_y = StringToInt(StringPoint_Y->getChar());
			point_x = dest_x;
			point_y = dest_y;
			delete StringPoint_X;
			delete StringPoint_Y;
		}
		if(actor == NULL)
		{
			for(int j = 0; j < history->battleEnemyNum; j++)
			{
				if(!history->battleEnemy[j]->hide&&history->battleEnemy[j]->x == dest_x&&history->battleEnemy[j]->y == dest_y)
				{
					actor = history->battleEnemy[j];
					break;
				}
			}
		}
		if(actor == NULL)
		{
			for(int j = 0; j < history->battleFriendNum; j++)
			{
				if(!history->battleFriend[j]->hide&&history->battleFriend[j]->x == dest_x&&history->battleFriend[j]->y == dest_y)
				{
					actor = history->battleFriend[j];
					break;
				}
			}
		}
		if(actor == NULL)
		{
			for(int j = 0; j < history->battleOwnNum; j++)
			{
				if(!history->battleOwn[j]->hide&&history->battleOwn[j]->x == dest_x&&history->battleOwn[j]->y == dest_y)
				{
					actor = history->battleOwn[j];
					break;
				}
			}
		}
	}
}

void ScriptMsgTransmit::HPChange()
{
	if (actor&&gain_hp&&actor->GetCurHp() > 0)
	{
		if (gain_hp + actor->GetCurHp() > actor->GetBaseHp() + actor->GetGoodsAddHp())
			gain_hp = actor->GetBaseHp() + actor->GetGoodsAddHp() - actor->GetCurHp();

		if (-gain_hp >= actor->GetCurHp())
		{
			actor->SetCurHp(0);
		}
		else
		{
			actor->SetCurHp(-1, gain_hp);
		}

	}
}
void ScriptMsgTransmit::ActorAcquisitionSeid(int actorID, int add, char* buf)
{
	HistoryActorPtr acquisitionSeidActor = NULL;
	acquisitionSeidActor = &history->actors[actorID - 1];
	if (acquisitionSeidActor == NULL)
		return;
	if (acquisitionSeidActor->AcquisitionSeid.size() == 0)
	{
		acquisitionSeidActor->AcquisitionSeid.resize(50);
		acquisitionSeidActor->AcquisitionSeid.memset(0);
	}
	if (acquisitionSeidActor->force_save <= 0)
		acquisitionSeidActor->force_save = 1;

	int next_seid = 0;
	int test_len = strlen(buf);
	int j = 0;
	while (j < test_len)
	{
		next_seid = StringToInt((char*)buf + j);
		j++;
		j++;
		if (next_seid >= 10)
		{
			j++;
		}
		if (next_seid >= 100)
		{
			j++;
		}
		if (add&&next_seid > 0)
		{
			for (int i = 0; i < acquisitionSeidActor->AcquisitionSeid.size(); i++)
			{
				if (acquisitionSeidActor->AcquisitionSeid[i] == next_seid) break;
				else if (acquisitionSeidActor->AcquisitionSeid[i] == 0)
				{
					acquisitionSeidActor->AcquisitionSeid[i] = next_seid;
					break;
				}
			}
		}
		else if (next_seid>0)
		{
			for (int i = 0; i < acquisitionSeidActor->AcquisitionSeid.size(); i++)
			{
				if (acquisitionSeidActor->AcquisitionSeid[i] == 0) break;
				else if (acquisitionSeidActor->AcquisitionSeid[i] == next_seid)
				{
					acquisitionSeidActor->AcquisitionSeid[i] = 0;
					for (int k = i; k < acquisitionSeidActor->AcquisitionSeid.size(); k++)
					{
						acquisitionSeidActor->AcquisitionSeid[k] = acquisitionSeidActor->AcquisitionSeid[k + 1];
						if (acquisitionSeidActor->AcquisitionSeid[k] == 0)
							break;
					}
					break;
				}
			}
		}
	}
	acquisitionSeidActor->UpdataGoodEffect();
}
void ScriptMsgTransmit::GoodsAcquisitionSeid(int goodsId, int add, char* buf)
{
	int next_seid = 0;
	int test_len = strlen(buf);
	int j = 0;
	while (j < test_len)
	{
		next_seid = StringToInt((char*)buf + j);
		j++;
		j++;
		if (next_seid >= 10)
		{
			j++;
		}
		if (next_seid >= 100)
		{
			j++;
		}
		if (add&&next_seid > 0)
		{

		}
		else if (next_seid>0)
		{
		}
	}
	if (history->game_state == GAME_STATE_BATTLE)
	{
		for (int i = 0; i < history->battleOwnNum; i++)
			history->battleOwn[i]->UpdataGoodEffect();
		for (int i = 0; i < history->battleFriendNum; i++)
			history->battleFriend[i]->UpdataGoodEffect();
		for (int i = 0; i < history->battleEnemyNum; i++)
			history->battleEnemy[i]->UpdataGoodEffect();
	}
	else if (history->game_state == GAME_STATE_REDE)
	{
		for (int i = 0; i < history->ownArmyNum; i++)
			history->actors[history->ownArmy[i]].UpdataGoodEffect();
	}
}

void ScriptMsgTransmit::SetNetExternalEquip(int iActorId, int iEquipId)
{
// 	if (iActorId >= 0 && trData->net_external_equip.find(iEquipId) != trData->net_external_equip.end())
// 	{
// 		HistoryActorPtr ptrActor = &history->actors[iActorId];
// 		NetExternalEquipPtr ptrNetExtEqu = trData->net_external_equip[iEquipId];
// 
// 		bool bInitActorAbility = false;
// 
// 		if (ptrNetExtEqu->boss.size()>1)
// 		{
// 			ptrActor->mElitePlus.clear();
// 			ptrActor->mElitePlus = ptrNetExtEqu->boss;
// 			if (ptrActor->mElitePlus[1] <= -100)  ptrActor->mElitePlus[1] = -99;
// 			if (ptrNetExtEqu->boss[0] == 1|| ptrNetExtEqu->boss[0] == 3)
// 			{
// 				history->m_mBossActor[ptrActor->actor_id +1] = 1;
// 			}
// 			bInitActorAbility = true;
// 		}
// 
// 		if (ptrNetExtEqu->quality > 0)
// 		{
// 			ptrActor->actor_quality = ptrNetExtEqu->quality;
// 			bInitActorAbility = true;
// 		}
// 
// 		if(bInitActorAbility)
// 			ptrActor->InitFiveAllAbility();
// 
// 		if (ptrNetExtEqu->weapon[0] > 0)
// 		{
// 			ptrActor->weapon.init(ptrNetExtEqu->weapon[0], 1);
// 			ptrActor->weapon.GetOrSetQuality(ptrNetExtEqu->weapon[1], false);
// 			for (int i = 2; i < 5; i++)
// 			{
// 				int streffid = ptrNetExtEqu->weapon[i];
// 				if (streffid>0)
// 				{
// 					ptrActor->weapon.AddStrengEffect(streffid);
// 				}
// 			}
// 		}
// 		if (ptrNetExtEqu->armour[0] > 0)
// 		{
// 			ptrActor->coat.init(ptrNetExtEqu->armour[0], 1);
// 			ptrActor->coat.GetOrSetQuality(ptrNetExtEqu->armour[1], false);
// 			for (int i = 2; i < 5; i++)
// 			{
// 				int streffid = ptrNetExtEqu->armour[i];
// 				if (streffid>0)
// 				{
// 					ptrActor->coat.AddStrengEffect(streffid);
// 				}
// 			}
// 		}
// 		if (ptrNetExtEqu->assist[0] > 0)
// 		{
// 			ptrActor->assist_good.init(ptrNetExtEqu->assist[0], 1);
// 			ptrActor->assist_good.GetOrSetQuality(ptrNetExtEqu->assist[1], false);
// 			for (int i = 2; i < 5; i++)
// 			{
// 				int streffid = ptrNetExtEqu->assist[i];
// 				if (streffid>0)
// 				{
// 					ptrActor->assist_good.AddStrengEffect(streffid);
// 				}
// 			}
// 		}
// 		ptrActor->UpdataGoodEffect();
// 
// 		int iSkillId = 0, iSkillLvMax = ptrNetExtEqu->skill_lv;
// 		if (iSkillLvMax < 0)
// 		{
// 			iSkillLvMax += ptrActor->GetActorLv();
// 		}
// 		if (iSkillLvMax > 0)
// 		{
// 			int iActorSkillIndex = trData->actors[ptrActor->actor_id].skill;
// 			if (iActorSkillIndex > 0)
// 			{
// 				NetHeroSkillPtr ptrNHSP = trData->net_hero_skill[iActorSkillIndex];
// 				map<int, map<int, int> > mSkillLevel = ptrActor->GetCurSkillLvList();
// 				for (map<int, map<int, int> >::iterator it = mSkillLevel.begin(); it != mSkillLevel.end(); it++)
// 				{
// 					iSkillId = it->first;
// 					int iRealMaxLv = iSkillLvMax > ptrActor->GetActorLv() ? ptrActor->GetActorLv() : iSkillLvMax;
// 					int iRealSkillLv = iRealMaxLv - ptrNHSP->skill[iSkillId] + 1;
// 					if (iRealSkillLv > 0)
// 					{
// 						ptrActor->SetCurSkillLevel(iSkillId, iRealSkillLv);
// 						ptrActor->SetSelfSkillLevel(iSkillId, iRealSkillLv);
// 						ptrActor->m_iExternalEquip = 1;
// 					}
// 				}
// 			}
// 		}
// 	}

}

void ScriptMsgTransmit::NetAddActorOrGoods(DList<DString>* StringList)
{
	int addType = StringToInt(StringList->elementAt(0)->getChar());
	int iBGoodsIndex = 0;
	map<int, int> mReduceGoods;
	map<int, int> mCellIndex;
	for (int i = 1; i < StringList->size(); i++)
	{
		char* ptrBuff = StringList->elementAt(i)->getChar();
		char* ptrPoint = strstr(ptrBuff, "&");
		if (!ptrPoint)
			continue;
		int iElemId = StringToInt(ptrBuff);
		int iEquipId = StringToInt(ptrPoint + 1);

		if (addType == 1)
		{
			int iActorId = iElemId - 1;
			bool bEnableAdd = true;
			for (int i = 0; i < history->ownArmyNum; i++)
			{
				if (iActorId == history->ownArmy[i])
				{
					bEnableAdd = false;
					break;
				}
			}

			if (bEnableAdd)
			{
				char* ptrPointLv = strstr(ptrPoint + 1, "&");
				HistoryActorPtr ptrActor = &history->actors[iActorId];
				int iOldRelation = ptrActor->relation;
				if (ptrPointLv != NULL)
					ptrActor->SetActorLv(StringToInt(ptrPointLv + 1));
				else
					ptrActor->SetActorLv(1);
				ptrActor->exp = 0;
				ptrActor->actor_quality = trData->actors[iActorId].actor_quality;
				ptrActor->occupation_id = trData->actors[iActorId].occupation_id;
				ptrActor->history_occupation_id = ptrActor->GetActorInitialJob();
				ptrActor->relation = ACTOR_RELATION_OWN;
				ptrActor->ai_type = 0;
				ptrActor->InitAbility();
				ptrActor->weapon.good_id = -1;
				ptrActor->coat.good_id = -1;
				ptrActor->assist_good.good_id = -1;
				ptrActor->fashion.good_id = -1;
				ptrActor->wap_fashion.good_id = -1;
				if (ptrActor->history_image_fight_id < 400)
				{
					ptrActor->history_image_fight_id = trData->occupation_image_id[ptrActor->occupation_id];
				}
				history->LoadBattleActorImage(iActorId);

				ptrActor->ClearSelfSkillLevel();

				ptrActor->UpdateSkills();

				history->battleOwn[history->battleOwnNum++] = ptrActor;
				history->ownArmy[history->ownArmyNum++] = iActorId;

				if (iOldRelation == ACTOR_RELATION_ENEMY)
				{
					for (int j = 0; j < history->battleEnemyNum; j++)
					{
						if (iActorId == history->battleEnemy[j]->actor_id)
						{
							for (int k = j; k < history->battleEnemyNum - 1; k++)
							{
								history->battleEnemy[j] = history->battleEnemy[j + 1];
							}
							history->battleEnemy[history->battleEnemyNum - 1] = NULL;
							history->battleEnemyNum--;
							break;
						}
					}
				}
				else if (iOldRelation == ACTOR_RELATION_FRIEND)
				{
					for (int j = 0; j < history->battleFriendNum; j++)
					{
						if (iActorId == history->battleFriend[j]->actor_id)
						{
							for (int k = j; k < history->battleFriendNum - 1; k++)
							{
								history->battleFriend[j] = history->battleFriend[j + 1];
							}
							history->battleFriend[history->battleFriendNum - 1] = NULL;
							history->battleFriendNum--;
							break;
						}
					}
				}
				SetNetExternalEquip(iActorId, iEquipId);
				WinConditionTool::getInstance()->AddScriptActorElem(iActorId + 1, history->ownArmyNum - 1);

				BattleScreenPtr bScr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
				if (bScr)
				{
					bScr->mFunctionLayer = ActorJoinOwnLayer::create(iActorId + 1, FUNCTION_JOIN, bScr);
					ChessApplication::getInstance()->AddFunctionLayer(bScr->mFunctionLayer);
				}
			}
			else
			{
				if (trData->net_hero_list.count(iElemId))
				{
					int iGoodsId = trData->net_hero_list[iElemId]->good_id;
					int iNeedActorSoul = trData->net_hero_quality[trData->actors[iActorId].actor_quality]->call_soul;

					battleGoods[iBGoodsIndex][0] = iGoodsId;
					battleGoods[iBGoodsIndex][1] = iNeedActorSoul;
					iBGoodsIndex++;
				}
			}
		}
		else if (addType == 2)
		{
			battleGoods[iBGoodsIndex][0] = iElemId;
			battleGoods[iBGoodsIndex][1] = iEquipId;
			iBGoodsIndex++;
		}
		else if (addType == 3)
		{
			mReduceGoods[iElemId] = iEquipId;
		}
		else if (addType == 4)
		{
			int iCaseIndex = 0;
			map<int, int> mTestGoods;
			char* ptrCaseIndex = strstr(ptrPoint + 1, "&");
			if (ptrCaseIndex != NULL)
				iCaseIndex = StringToInt(ptrCaseIndex + 1);
			mTestGoods[iElemId] = iEquipId;

			if (!history->ReduceNetGoods(mTestGoods, true))
			{
				mCellIndex[iCaseIndex] = 0;
			}
			else
			{
				mCellIndex[iCaseIndex] = 1;
			}
		}
	}
	if (addType == 3 && mReduceGoods.size())
	{
		NetBattleReduceGoods(mReduceGoods);
	}
	if (addType == 4)
	{
		history->m_m12ScriptCon.clear();
		history->m_m12ScriptCon = mCellIndex;
	}
}

void ScriptMsgTransmit::ShowGetGoodsAction()
{
	if (!battleGoods.size())
		return;

	if ((frame == 1 && iNetGetGoodsIndex) || (!iNetGetGoodsIndex&&!frame))
	{
		BattleActor::SetGoodImage(battleGoods[iNetGetGoodsIndex][0]);
		int iActorId = -1;
		if (history->cur_actor)
			iActorId = history->cur_actor->actor_id;
		else if (history->battleOwnNum&&history->battleOwn[0])
			iActorId = history->battleOwn[0]->actor_id;
		if (iActorId>-1)
		{
			BattleActorPtr bactor = BattleControler::getBattleActor(iActorId);
			if (bactor)
			{
				need_frame = bactor->setState(ACTOR_STATE_GOOD_GIVE);
				bNetEnableGetNext = false;
			}
		}
	}
	else if (need_frame == (frame + 1))
	{
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		if (scr != NULL)
		{
			if (!bNetEnableGetNext)
			{
				HistoryGoods hisGoods(battleGoods[iNetGetGoodsIndex][0]);
				string strNotice = StringUtils::format(LL("dedaos"), hisGoods.GetGoodsName(battleGoods[iNetGetGoodsIndex][1]));
				scr->m_spLabel->updateSprite(strNotice, LABEL_SHORT, 0.8f);
				need_frame += 5;
				bNetEnableGetNext = true;
			}
			else if (iNetGetGoodsIndex < (int)battleGoods.size()-1)
			{
				iNetGetGoodsIndex++;
				frame = 0;
				need_frame = 1;
				bNetEnableGetNext = false;
			}
			else if (iNetGetGoodsIndex >= (int)battleGoods.size()-1)
			{
				frame++;
			}
		}
	}
}

void ScriptMsgTransmit::NetBattleReduceGoods(map<int, int> reduceGoods)
{
	history->ReduceNetGoods(reduceGoods);
}

void ScriptMsgTransmit::RunSkillAction(int iSkillId, int iPointX, int iPointY)
{
	if (iSkillId <= trData->skill_num)
	{
		SkillAttrPtr ptrSkill = &trData->skills[iSkillId - 1];
		if (ptrSkill->animation[1])
		{
			ActionLayer* ptrActionLayer = (ActionLayer*)ChessApplication::getInstance()->GetActionLayer();
			ptrActionLayer->ScriptRunAction(ptrSkill->animation[1], ptrSkill->animation[2], iPointX, iPointY);
			if (ptrSkill->skill_mp3)
				ChessPlayNetSound(ptrSkill->skill_mp3, "skill");
		}
	}
}

void ScriptMsgTransmit::RunSkillBossAction(int iType, int iActor, int iSkillId)
{
	if (iSkillId <= trData->skill_num && iActor <= trData->actor_num)
	{
		HistoryActorPtr ptrOppoActor = &history->actors[iActor - 1];
		SkillAttrPtr ptrSkill = &trData->skills[iSkillId - 1];
		if (ptrSkill->animation[1])
		{
			history->CenterPos(ptrOppoActor->x, ptrOppoActor->y);
			ActionLayer* ptrActionLayer = (ActionLayer*)ChessApplication::getInstance()->GetActionLayer();
			if (iType == 2)
			{
				string AniNameS = "";
				ptrActionLayer->RunAction(ptrSkill->animation[1], ptrSkill->animation[2], NULL, ptrOppoActor, AniNameS);
				if (ptrSkill->skill_mp3)
					ChessPlayNetSound(ptrSkill->skill_mp3, "skill");
			}
			else if (iType == 1)
			{
				m_iBossHide = 1;
				ptrActionLayer->ScriptRunBossHideAction(ptrSkill->animation[1], ptrSkill->animation[2], iActor,&m_iBossHide);
				if (ptrSkill->skill_mp3)
					ChessPlayNetSound(ptrSkill->skill_mp3, "skill");
			}
		}
	}
}

void ScriptMsgTransmit::DispatchRandEnemy(DList<DString>* ptrStringList)
{
	//初始化数据
	int iAddEnemyNum = StringToInt(ptrStringList->elementAt(0)->getChar());

	int iPointType = StringToInt(ptrStringList->elementAt(1)->getChar());

	map<int, map<int, int> > mEPoint = trData->getMapJsonData(ptrStringList->elementAt(1)->getChar() + 2);

	int iHidType = StringToInt(ptrStringList->elementAt(2)->getChar());

	map<int, int> mHidList = trData->getSeidData(ptrStringList->elementAt(2)->getChar() + 2);

	int iActorHide = StringToInt(ptrStringList->elementAt(3)->getChar());

	int iDirection = 0,iRandAddLv = 0;

	if (ptrStringList->size() > 4)
		iDirection = StringToInt(ptrStringList->elementAt(4)->getChar());
	if (ptrStringList->size() > 5)
		iRandAddLv = StringToInt(ptrStringList->elementAt(5)->getChar());

	if (iDirection != 0 && iDirection <= 4 && iDirection >= 1)
		iDirection -= 1;
	else
		iDirection = rand() % 4;

	map<int, int> iEnemyIdList;
	for (int i = 0; i < history->battleEnemyNum; i++)
	{
		iEnemyIdList[history->battleEnemy[i]->actor_id+1] = 1;
	}

	if ((iPointType == 2 || mHidList.size() == 1) && iAddEnemyNum > 1)
	{
		iAddEnemyNum = 1;
	}

	if (iHidType == 1&& mHidList.size()>1&& mHidList[1]< mHidList[0])
	{
		int iTemp = mHidList[0];
		mHidList[0] = mHidList[1];
		mHidList[1] = iTemp;
	}

	//坐标
	map<int, map<int, int> > mEnemyPoint;

	int iPointIndex = 0;

	int iRandNumX = 1,iRandNumY = 1;
	int iPointX = 0,iPointY = 0;

	if (iPointType == 1)
	{
		if (mEPoint[1][0] < mEPoint[0][0])
		{
			iPointX = mEPoint[1][0];
			mEPoint[1][0] = mEPoint[0][0];
			mEPoint[0][0] = iPointX;
		}

		if (mEPoint[1][1] < mEPoint[0][1])
		{
			iPointY = mEPoint[1][1];
			mEPoint[1][1] = mEPoint[0][1];
			mEPoint[0][1] = iPointY;
		}

		iRandNumX = mEPoint[1][0] - mEPoint[0][0];

		iRandNumY = mEPoint[1][1] - mEPoint[0][1];

		if (!iRandNumX && !iRandNumY) return;


		for (int i = mEPoint[0][0]; i <= mEPoint[1][0]; i++)
		{
			for (int j = mEPoint[0][1]; j <= mEPoint[1][1]; j++)
			{
				mEnemyPoint[iPointIndex][0] = i;
				mEnemyPoint[iPointIndex][1] = j;
				iPointIndex++;
			}
		}

	}
	else
	{
		mEnemyPoint = mEPoint;
		iPointIndex = mEnemyPoint.size();
	}

	if (iAddEnemyNum > 0)
	{
		map<int, int> mAlreadyChooseHid,mACHidIndex;
		int iChooseId = 0, iChooseIndex = 0,isCon = 0;

		for (int i = 0; i < iAddEnemyNum; i++)
		{
			iChooseId = 0;

			do 
			{
				isCon = 0;
				if (iHidType == 1 && mHidList.size() > 1)
				{
					iChooseId = rand() % (mHidList[1] - mHidList[0] + 1) + mHidList[0];
				}
				else if (iHidType == 2)
				{
					iChooseId = mHidList[i];
				}
				else
				{
					iChooseId = 0;
				}

 				if (iEnemyIdList.count(iChooseId))
 				{
 					isCon = 1;
 				}

			} while (mACHidIndex.count(iChooseId) || isCon);

			if (iChooseId > 0) 
			{
				mACHidIndex[iChooseId] = 1;
				mAlreadyChooseHid[iChooseIndex++] = iChooseId;
			}
		}

		ScriptDispatchEnemy* ptrSDE = new ScriptDispatchEnemy();
		ptrSDE->m_iIs72ScriptAdd = iChooseIndex;

		HistoryActorPtr ptrActor = NULL;
		int iActorId = 0,iChoosePIndex = 0, iPointX = 0,iPointY = 0,iIsContinue = 0;
		map<int, int> mChoosePoint;
		for (int i = 0;i < iChooseIndex;i++)
		{
			iActorId = mAlreadyChooseHid[i];
			ptrActor = &history->actors[iActorId - 1];

			do 
			{
				iIsContinue = 0;

				iChoosePIndex = rand() % iPointIndex;

				if (!mChoosePoint.count(iChoosePIndex))
				{
					iPointX = mEnemyPoint[iChoosePIndex][0];
					iPointY = mEnemyPoint[iChoosePIndex][1];

					HistoryActorPtr oppoActor = history->FindActorPosition(iPointX, iPointY);
					if (
						!(oppoActor != NULL || history->HasElem(iPointX, iPointY, ptrActor->occupation_id) ||
							trData->occupation_landmove_info[ptrActor->occupation_id][history->map_array[iPointY][iPointX]] == 255))

						break;
					else
						iIsContinue = 1;
				}

			} while (mChoosePoint.count(iChoosePIndex) || iIsContinue);

			mChoosePoint[iChoosePIndex] = 1;

			//赋值
			ptrSDE->para[i].actor_id = iActorId - 1;
			ptrSDE->para[i].isAssist = 0;
			ptrSDE->para[i].hide = iActorHide;
			ptrSDE->para[i].x = iPointX;
			ptrSDE->para[i].y = iPointY;
			ptrSDE->para[i].direction = iDirection;
			ptrSDE->para[i].level_add = 0;
			ptrSDE->para[i].ai_type = 0;
			ptrSDE->para[i].oppo_actor_id = -1;
			ptrSDE->para[i].dest_x = 0;
			ptrSDE->para[i].dest_y = 0;
			ptrSDE->para[i].m_iRandAddLv = iRandAddLv;
		}

		if(iChooseIndex)
			ptrSDE->HandleScript();

		SAFE_DELETE(ptrSDE);
	}


}

int ScriptMsgTransmit::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	function_id = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	msgData = bytesToString(SceneData, index);
	index += msgData->length() + 1;
	return index;
}

ScriptMsgTransmit::~ScriptMsgTransmit()
{
	SAFE_DELETE(ActorOrPointList);
	SAFE_DELETE(m_listScript);
}
