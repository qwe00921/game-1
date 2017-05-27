#ifndef __TRGAME_NETPROMPTTOOL_
#define __TRGAME_NETPROMPTTOOL_

#include <stdio.h>
#include "Common.h"
#include "main.h"

#define SEND_NET_INTERVAL 60.0
#define LOOP_TXT_INTERVAL 4.0
#define TXT_UP_TIME 0.5
#define MOVE_LEFT_TIME 0.5
enum
{
	PROMPT_QH = 1,//强化
	PROMPT_WJJJ = 2,//武将进阶
	PROMPT_WJZM = 3,//武将招募
	PROMPT_WQHC = 4,//武器合成
	PROMPT_YYTKQ = 5,//演义堂开启
};

class NetPromptElem
{
	CC_SYNTHESIZE(int, iIndex, eINDEX);
	CC_SYNTHESIZE(int, iNum, eNUM);
	CC_SYNTHESIZE(int, iType, eTYPE);
	CC_SYNTHESIZE(string,contentS,eCONTENTS);
	CC_SYNTHESIZE(bool,bIsSys,eISSYS);//系统消息
public:
	NetPromptElem() {};
	map<string, string> mInfoElem;
};

class NetPromptLayer:public Layer
{
public:
	CREATE_FUNC(NetPromptLayer);
	bool init();
	NetPromptLayer();
	~NetPromptLayer();

	void UpdataTxtLab(NetPromptElem* ptrNPE);
	void StopPlay();
private:
	bool UpdataPromptSpShow(NetPromptElem* ptrNPE,int iSpIndex);
	void DelayMoveLeft(float f);
	Color3B GetStringColor(int iType,int iIndex);
	void BgFadeOut(Node* node);
	void BgFadein(Node* node);
private:
	Sprite* m_bg;
	map<int, Sprite*> m_pTxtLabSp;
	ClippingNode* mClippingNode;
	int m_iCurLabNum;
	bool m_bIsVisibal;

	int m_iBgWidth;
	int m_iBgHeight;
	float m_fOffSetX;
	float m_fOffSetY;
};

class NetPromptTool:public Node
{
public:
	~NetPromptTool(){};
	static NetPromptTool* getInstance();

	void StartGetNetPrompt();
	void StopGetNetPrompt();
	void setGetOrderStatus(bool start,const char* orderId);
	void AddNetPrompt(int iType,int iGoodsId,int iNomal = -1);
private:
	static NetPromptTool* create();
	bool init();
	void getOrderStatus(float f);
	void StartSendNetData(float f);
	void UpdataOrdinaryTxt(float f);
	void StopUpdataOrdinaryTxt();

	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void LoadPromptTxt(Json::Value jsonData);

	void clearAllPromptList();
	void StartLoopPromptList();
private:
	NetPromptTool();
	static NetPromptTool* m_instance;
	std::string mOrderId;
	NetPromptLayer* mNetPromptLayer;
	bool mGetOrderStatus;
	int m_iPListIndex;
	int mQueryCount;
	map<int, NetPromptElem*> m_mPromptList;
	map<int, int> m_mPromptIdlist;
};
#endif /* __TRGAME_NETPROMPTTOOL_ */
