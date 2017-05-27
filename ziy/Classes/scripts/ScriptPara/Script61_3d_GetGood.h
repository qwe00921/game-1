#ifndef _ScriptGetGood_H_
#define _ScriptGetGood_H_

class ScriptGetGood : public Script
{
public:
	ScriptGetGood()
	{
		cmd = CMD_GET_GOOD;
	}

	ScriptGetGood(int iActorId, int iGoodsId, int iCount)
	{
		cmd = CMD_GET_GOOD;
		m_iActorId = iActorId;
		m_iGoodsId = iGoodsId;
		m_iCount = iCount;
	}

	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData, int index);
private:
	int m_iCount;

	short unkown1;
	short m_iGoodsId;
	short unkown2;
	short good_level;
	short unkown3;
	short show_action;
	short unkown4;
	short m_iActorId;
};

#endif