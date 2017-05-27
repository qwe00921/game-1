//
//  ActivityInfo.h
//  TRGame
//
//  Created by Jimmy on 16/06/14.
//
//

#ifndef _ACTIVITYINFO_H_
#define _ACTIVITYINFO_H_

#include "Common.h"
#include "PlayerInfo.h"

using namespace std;

#define EXCH_GOODS_GLL  2501
#define EXCH_GOODS_YXL  2502
#define EXCH_GOODS_JJL   2503

enum
{
	ACT_UNFINISH = 0, //未完成
	ACT_FINISH = 1, //完成
	ACT_RECEIVED = 2 //已领取
};

enum
{
	ACT_INIT = 0, //初始化活动数据
	ACT_LOGIN = 1, //登陆奖励
	ACT_RECHARGE = 2, //充值奖励
	ACT_LEVEL = 3, //等级奖励
	ACT_EXCHENGE = 4, //兑换奖励
	ACT_RMBGIFT = 5, //人民币礼包
	ACT_NOTICE = 996,//活动公告
	ACT_WXGIFT = 997,//微信绑定礼包
	ACT_EXCHCODE = 998, //兑换码
	ACT_CREDITCARD = 999, //信用卡
};

enum
{
	EXCH_JJL = 0, //将军令
	EXCH_YXL = 1, //英雄令
	EXCH_GLL = 2 //高狼令
};

class ActGiftElem
{
public:
	ActGiftElem();
	void loadJson(int iId, Json::Value valElem);

	CC_SYNTHESIZE(int, m_iId, ElemId); //礼包ID
	CC_SYNTHESIZE(int, m_iState, State); //领取状态
	CC_SYNTHESIZE(int, m_iCondition, Condition); //领取条件
	map<int, int> m_mGoods; //礼包内容
};

class ActExchElem
{
public:
	ActExchElem();
	void loadJson(int iId, Json::Value valElem);

	CC_SYNTHESIZE(int, m_iId, ElemId); //礼包ID
	CC_SYNTHESIZE(int, m_iState, State); //领取状态
	map<int, int> m_mUseGoods; //兑换消耗
	map<int, int> m_mGetGoods; //礼包内容
};


class ActivityElem
{
public:
	ActivityElem();
	void loadJson(Json::Value valElem);

	CC_SYNTHESIZE(int, m_iActId, ActId); //活动ID
	CC_SYNTHESIZE(int, m_iImgId, ImgId); //活动图片ID
	CC_SYNTHESIZE(int, m_iTypeId, TypeId); //活动类型ID
	CC_SYNTHESIZE(string, m_strTitle, Title); //活动名称
	CC_SYNTHESIZE(string, m_strMark, Mark); //活动标签
	CC_SYNTHESIZE(string, m_strContent, Content); //活动内容
	CC_SYNTHESIZE(string, m_strSTime, StartTime); //开始时间
	CC_SYNTHESIZE(string, m_strETime, EndTime); //结束时间
	CC_SYNTHESIZE(int, m_iRecharge, Recharge); //充值金额
	CC_SYNTHESIZE(string, m_strUrl1, CardUrl1); //信用卡URL1
	CC_SYNTHESIZE(string, m_strUrl2, CardUrl2); //信用卡URL2
	CC_SYNTHESIZE(int, m_iRmbPrice, RMBPrice); //人民币礼包价格
	CC_SYNTHESIZE(int, m_iRmbMark, RMBMark); //人民币礼包标签
	map<int, ActGiftElem*> m_mGiftGoods;
	map<int, ActExchElem*> m_mExchGoods;
	map<int, int> m_mCardGifts; //信用卡礼包
};

class SignGoods
{
public:
	SignGoods();
	void loadJson(Json::Value valElem);

	CC_SYNTHESIZE(int, m_iGoodsId, GoodsId); //物品ID
	CC_SYNTHESIZE(int, m_iCount, Count); //物品数目
	CC_SYNTHESIZE(int, m_iIsDouble, IsDouble); //是否双倍
};

class BTActivity : public Ref
{
public:
	static BTActivity* getInstance();
	bool init();

	//活动数据
	void updateAllActivity();
	void updateActById(int iActId);
	void updateActByType(int iType, EventDelegate* pDelegate = nullptr);
	void clearActivity();

	//签到数据
	bool isSignInit();
	bool loadSignJson(Json::Value valSign);
	void clearSignInfo();

	//商城兑换数据
	bool isExchInit();
	bool loadExchJson(Json::Value valExch);
	void clearExchInfo();
	int getMaterialId(int iType); //获取兑换材料的ID
	string getExchType(int iType);
private:
	BTActivity();
	bool loadJson(Json::Value valActInfo);
	void onHttpRequest(int iReqType, int iActId = 1);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onHttpReqSucceed(Json::Value valRes, string strTag);
public:
	map<int, ActivityElem*> m_mActivity;
	map<int, SignGoods*> m_mSignGoods;
	map<int, map<int, ActExchElem*> > m_mExchGoods; //商城兑换数据
private:
	static BTActivity* m_pInstance;
	EventDelegate* m_pDelegate;
	map<int, bool> m_mRefMoney; //充值奖励是否已刷新充值数
	map<int, string> m_mstrExch; //商城兑换类型标签

	CC_SYNTHESIZE(int, m_iMonth, SignMonth); //签到月份
	CC_SYNTHESIZE(int, m_iTimes, TotalTimes); //签到总次数
	CC_SYNTHESIZE(int, m_iIsSign, IsSign); //当天是否签到
	CC_SYNTHESIZE(int, m_iMissTimes, MissTimes); //漏签次数
};

#endif //_ACTIVITYINFO_H_