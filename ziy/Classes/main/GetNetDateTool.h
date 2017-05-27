#ifndef __TRGAME_GETNETDATETOOL_
#define __TRGAME_GETNETDATETOOL_

#include <stdio.h>
#include "Common.h"
#include "main.h"

enum
{
	SEND_NET_SWEEP,//扫荡
	SEND_NET_REFRESH,//重置关卡
	SEND_NET_UPDATA_REFNUM//刷新英雄志关卡购买次数
};

class GetNetDateTool:public Ref
{
public:
	~GetNetDateTool(){};
	static GetNetDateTool* getInstance();
	void GetMapStarInfo();
	void SendSweepDate(int iWarId, int iType, EventDelegate* ptrDelegate);
	void SendRefWarCount(int iWarId, EventDelegate* ptrDelegate);
	void SendUpdataWarRefNum(int iWarId, EventDelegate* ptrDelegate);
	void SendRomanceOpenData();
	void SendLadderRankData();
	void SendWorldRankData();
	void SendChongZhiRankData();
	void SendClanInfoData();
	void SendClanHomePageData();
	void SendGreatBattleData();

	__String* getCommData();
	void SendReq(const char* funcName, __String* data);

	//保存引导变量值到服务器
	void setGuideStatusReq(const char*, int value);
private:
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	GetNetDateTool();
	static GetNetDateTool* m_instance;

	EventDelegate* mDelegate;
};
#endif /* __TRGAME_GETNETDATETOOL_ */
