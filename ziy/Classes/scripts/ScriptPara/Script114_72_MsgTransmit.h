#ifndef _Script114_72_MsgTransmit_H_
#define _Script114_72_MsgTransmit_H_
using namespace std;
class ScriptMsgTransmit : public Script
{
public:
	ScriptMsgTransmit()
	{
		cmd = CMD_MSG_TRANSMIT;
		magic_id = 0;
		gain_hp = 0;
		visible = 0;
		ActorOrPointList = new DList<DString>(true);
		actor_index = 0;
		actor_num = 0;
		point_x = -1;
		point_y = -1;
		actor = NULL;
		iNetGetGoodsIndex = 0;
		bNetEnableGetNext = false;
		iImageAListIndex = 0;
		m_listScript = new DList<Script>(true);
		m_iScriptIndex = 0;
		m_iBossHide = 0;
		m_iRandVisibal = 0;
	}
	~ScriptMsgTransmit();

	int HandleScript();
	int initPara(char*SceneData, int index);
private:
	void GetActorOrPoint(char *buf);
	void GetStringList();
	void SelectActor();
	void HPChange();
	void ActorAcquisitionSeid(int actorID, int add, char* buf);
	void GoodsAcquisitionSeid(int goodsId, int add, char* buf);
	void SetNetExternalEquip(int iActorId, int iEquipId);
	void NetAddActorOrGoods(DList<DString>* ptrStringList);
	void ShowGetGoodsAction();
	void NetBattleReduceGoods(map<int, int> reduceGoods);
	void RunSkillAction(int iSkillId, int iPointX, int iPointY);
	void RunSkillBossAction(int iType,int iActor, int iSkillId);
	void DispatchRandEnemy(DList<DString>* ptrStringList);
private:
	short unkown1;
	int function_id;
	short unkown2;
	DString msgData;
	map<int, map<int, int> > battleGoods;
	map<int, int> mImageActorList;
	int iImageAListIndex;
	int magic_id;
	int gain_hp;
	int visible;
	DList<DString>* ActorOrPointList;
	int actor_index;
	int actor_num;
	int point_x;
	int point_y;
	HistoryActorPtr actor;
	int iNetGetGoodsIndex;
	bool bNetEnableGetNext;

	DList<Script>* m_listScript;
	int m_iScriptIndex;

	int m_iBossHide;

	map<int, HistoryActorPtr> mRandVisibal;
	int m_iRandVisibal;
};
#endif