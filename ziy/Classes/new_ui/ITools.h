//
//  ITools.hpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/2.
//
//

#ifndef ITools_h
#define ITools_h

#include <stdio.h>
#include "network/HttpClient.h"
#include "cocos2d.h"
USING_NS_CC;
#include "EventDelegate.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::network;
class HttpReqData
{
public:
	HttpReqData()
	{
		strUrl = "";
		pTarget = NULL;
		callback = NULL;
		strData = "";
		strTag = "";
		bDealError = false;
	}
public:
	string strUrl;
	Ref* pTarget;
	SEL_CallFuncND callback;
	string strData;
	string strTag;
	bool bDealError; //错误是否向下派发
};

class ITools : public Ref, public EventDelegate
{
public:
	static ITools* getInstance();
	~ITools();
	Sprite* getSpriteWithPath(string path);
	Color3B getPZColor(int q);
	Color3B getActorPzColor(int iQuality);
	Texture2D* getTextureWithPath(string path);
	void createGlobalPrompt(string s, bool b);

	void connectingNetwork(const char* urlSuffix, HttpRequest::Type type, Ref* target, SEL_CallFuncND callback, string data, std::vector<std::string> vh, const char *tag = NULL, bool bDealError = false);
	void connectingAgain(string strTag);
	void onHttpRequestCompleted(Node *sender, void *resultdata);

	void addWaitLayer(bool bIsDelay = true); //连接中是否延迟显示
	void removeWaitLayer();
	Sprite* getNumSprite(int iNum, bool bIsLast = false, int iImgIndex = 1);
	Sprite* getNumSprite(int iTotleNum, int iNeedNum, bool bIsRight);
	//直接开战斗
	void startFight(int warId, int battleType, int battleStage,bool autoOut = true);

	//delegate
	void ChildCloseLayer(int iNomal);
	/**
	* @brief 遮罩
	*/
	Sprite* maskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite);
	/************************************************************************/
	/* 返回字符串的字节数（汉字为2个字节，其他为1个字节）					*/
	/************************************************************************/
	int getTextLength(const string & text);
	vector<string> mHead;
private:
	ITools();
	void addHttpData(const char* url, Ref* target, SEL_CallFuncND callback, string data, string strTag, bool bDealError);
	bool VerificationNetData(string NetDataS);
private:
	static ITools* m_instance;
	static Layer* m_pWaitLayer;
	static int m_iWaitCount; //等待层计数器

	map<string, HttpReqData*> m_pHttpData; //请求数据
};
#endif /* ITools_hpp */
