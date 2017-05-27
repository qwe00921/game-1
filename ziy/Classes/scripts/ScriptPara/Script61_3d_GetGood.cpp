#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
//23
//物品代码（大排列）3
//73
//物品等级（0表默认）
//38
//是否显示信息0
//2
//武将序号2

int ScriptGetGood::HandleRScript(RedeScreenPtr scr)
{
	return 0;
}

int ScriptGetGood::HandleScript()
{
	if(frame == 0)
	{
		BattleActor* ptrActor = BattleControler::getBattleActor(m_iActorId);
		if(ptrActor)
		{
			HistoryGoods hisGoods(m_iGoodsId);
			BattleActor::SetGoodImage(m_iGoodsId);
			if(hisGoods.GetGoodsType() == GOOD_TYPE_FASHING || hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
			{
				need_frame = ptrActor->setState(ACTOR_STATE_CHANGE_FASHION);
			}
			else
			{
				need_frame = ptrActor->setState(ACTOR_STATE_GOOD_GIVE);
			}
		}
	}
	else if(need_frame == frame + 1)
	{
		BattleScreenPtr ptrScreen = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		HistoryGoods hisGoods(m_iGoodsId);
		string strNotice = StringUtils::format(LL("dedaos"), hisGoods.GetGoodsName(m_iCount));
		ptrScreen->m_spLabel->updateSprite(strNotice, LABEL_SHORT, 0.8f);

		if(hisGoods.GetGoodsType() == GOOD_TYPE_FASHING || hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
		{
			HistoryActor* ptrActor = BattleControler::getBattleActor(m_iActorId)->hisActor;
			int iLostHp = ptrActor->getTotalAbility(TOTAL_HP) - ptrActor->GetCurHp();
			int iLostMp = ptrActor->getTotalAbility(TOTAL_MP) - ptrActor->GetCurMp();
			if(hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
			{
				ptrActor->fashion.setGood(&hisGoods);
			}
			else if(hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
			{
				ptrActor->wap_fashion.setGood(&hisGoods);
			}
			ptrActor->UpdataGoodEffect();
			ptrActor->SetCurHp(ptrActor->getTotalAbility(TOTAL_HP) - iLostHp);
			ptrActor->SetCurMp(ptrActor->getTotalAbility(TOTAL_MP) - iLostMp);
		}
	}
	return need_frame - frame++;
}

int ScriptGetGood::initPara(char* SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	m_iGoodsId = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_level = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	show_action = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	m_iActorId = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
