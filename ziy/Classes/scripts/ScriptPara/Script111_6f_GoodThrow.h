#ifndef _Script111_6f_GoodThrow_H_
#define _Script111_6f_GoodThrow_H_
class ScriptGoodThrow : public Script
{
public:
	ScriptGoodThrow() {
		cmd = CMD_GOOD_THROW;
	}

	short unkown1;
	short good_id;

	int HandleScript();
	int initPara(char*SceneData, int index);
private:
	bool IsMaxLevel(HistoryGoodsPtr ptrGood);
};
#endif