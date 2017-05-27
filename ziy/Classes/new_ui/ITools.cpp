//
//  ITools.cpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/2.
//
//

#include "ITools.h"
#include "Defines.h"
#include "WaitingLayer.h"
#include "PopLayer.h"
#include "ShortcutFunctionLayer.h"

#include "DispatchLayer.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "BattleTitleLayer.h"
Layer* ITools::m_pWaitLayer = NULL;
int ITools::m_iWaitCount = 0;
ITools* ITools::m_instance = NULL;

ITools* ITools::getInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new ITools();
	}
	return m_instance;
}

ITools::ITools()
{
	
	if (modConst->mIsTestService)
    {
		mHead.push_back("X-LC-Id: qxSbTRIi3zxUMn7Uia1sY0M4-gzGzoHsz");
		mHead.push_back("X-LC-Key: m7wUI3ybmTtv46JHehBz4QiN");
    }
    else
    {
		mHead.push_back("X-LC-Id: jQFYleem0BuPhjbkGscl9Hoa-gzGzoHsz");
		mHead.push_back("X-LC-Key: RtO7Ntbw4DpIIecMKLWaYsX2");
    }
	

	mHead.push_back("Content-Type: application/json");
}

ITools::~ITools()
{
	for(map<string, HttpReqData*>::iterator it = m_pHttpData.begin(); it != m_pHttpData.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_pHttpData.clear();
}

Sprite* ITools::getSpriteWithPath(string path)
{
//	unsigned long fileLen = 0;
//	char* game_data = ChessReadFile(path.c_str(), 0, 0, &fileLen);
//	CCImage img;
//	img.initWithImageData(game_data, fileLen);
//	CCTexture2D* text2d = CCTextureCache::sharedTextureCache()->addUIImage(&img, path.c_str());
//	ChessFree(game_data);
    Texture2D* text2d = Director::getInstance()->getTextureCache()->addImage(getRespath(path.c_str()));
	return Sprite::createWithTexture(text2d);
}

Color3B ITools::getPZColor(int q)
{
	Color3B color = Color3B::WHITE;
	switch(q)
	{
	case 0:
		color = Color3B::WHITE;
		break;
	case 1:
		color = Color3B::GREEN;
		break;
	case 2:
		color = Color3B(0, 255, 255);
		break;
	case 3:
		color = Color3B(255, 0, 255);
		break;
	case 4:
		color = Color3B::RED;
		break;
	default:
		break;
	}

	return color;
}

Color3B ITools::getActorPzColor(int iQuality)
{
	Color3B color = Color3B::WHITE;
	switch (iQuality)
	{
	case 1:
		color = Color3B::WHITE;
		break;
	case 2:
		color = Color3B::GREEN;
		break;
	case 3:
		color = Color3B(0, 255, 255);
		break;
	case 4:
		color = Color3B(255, 0, 255);
		break;
	case 5:
		color = Color3B(255,255,0);
		break;
	default:
		break;
	}

	return color;
}
Texture2D* ITools::getTextureWithPath(string path)
{
//	unsigned long fileLen = 0;
//	char* game_data = ChessReadFile(path.c_str(), 0, 0, &fileLen);
//    CCImage img;
//	img.initWithImageData(game_data, fileLen);
//	CCTexture2D* text2d = CCTextureCache::sharedTextureCache()->addUIImage(&img, path.c_str());
//	ChessFree(game_data);
//	return text2d;
    return Director::getInstance()->getTextureCache()->addImage(getRespath(path.c_str()));
}

void ITools::createGlobalPrompt(string s, bool b)
{
	Director::getInstance()->getRunningScene()->addChild(GlobalPrompt::create(s, b), 99);
}

void ITools::connectingNetwork(const char* urlSuffix, HttpRequest::Type type, Ref* target, SEL_CallFuncND callback, string data, std::vector<std::string> vh, const char *tag /*= NULL*/, bool bDealError /*= false*/)
{
	CCLOG("urlSuffix:%s", urlSuffix);
	const char* url = __String::createWithFormat("https://api.leancloud.cn/1.1/functions/%s", urlSuffix)->getCString();

	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setRequestType(type);
	request->setResponseCallback(this, callfuncND_selector(ITools::onHttpRequestCompleted));
	request->setHeaders(vh);
	string params = data;
	const char* postData = params.c_str();
	request->setRequestData(postData, strlen(postData));

	string strTag = "";
	if (tag == NULL)
	{
		DString strUrl = url;
		int iLastIndex = strUrl->lastIndexOf('/', strUrl->length());
		strTag = strUrl->substring(iLastIndex + 1)->getChar();
	}
	else
	{
		strTag = tag;
	}
	request->setTag(strTag.c_str());

	HttpClient::getInstance()->send(request);
	request->release();

	addHttpData(url, target, callback, data, strTag, bDealError);
}

void ITools::addWaitLayer(bool bIsDelay /*= true*/)
{
	if(m_pWaitLayer == NULL)
	{
		m_pWaitLayer = WaitingLayer::create(bIsDelay);
		Director::getInstance()->getRunningScene()->addChild(m_pWaitLayer, 998);
	}
	m_iWaitCount++;
}

void ITools::removeWaitLayer()
{
	if(m_iWaitCount > 0)
	{
		m_iWaitCount--;
		if(!m_iWaitCount && m_pWaitLayer)
		{
			m_pWaitLayer->removeFromParent();
			m_pWaitLayer = NULL;
		}
	}
}

Sprite* ITools::getNumSprite(int iNum, bool bIsLast /*= false*/, int iImgIndex /*= 1*/)
{
	Sprite* pRet = NULL;

	map<int, int> numarr;
	int i = 0;
	do
	{
		numarr[i++] = iNum % 10;
		iNum = iNum / 10;

	}
	while(iNum > 0);

	__String* img_path;
	Sprite* tempSp;

	if(bIsLast)
	{
		for(size_t j = 0; j < numarr.size(); j++)
		{
			img_path = __String::createWithFormat("new_ui/img_number/num%d_%d.png", iImgIndex, numarr[j]);
			Sprite* tempSp1 = ReadSpriteName(img_path->getCString());
			if(pRet == NULL)
			{
				pRet = tempSp1;
				tempSp = tempSp1;
			}
			else
			{
				tempSp1->setAnchorPoint(Vec2(1, 0));
				tempSp1->setPosition(Vec2(0, 0));
				tempSp->addChild(tempSp1);
				tempSp = tempSp1;
			}
		}
	}
	else
	{
		for(int j = (int)numarr.size() - 1; j >= 0; j--)
		{
			img_path = __String::createWithFormat("new_ui/img_number/num%d_%d.png", iImgIndex, numarr[j]);
			Sprite* tempSp1 = ReadSpriteName(img_path->getCString());
			if(pRet == NULL)
			{
				pRet = tempSp1;
				tempSp = tempSp1;
			}
			else
			{
				tempSp1->setAnchorPoint(Vec2(0, 0));
				tempSp1->setPosition(Vec2(tempSp->getContentSize().width, 0));
				tempSp->addChild(tempSp1);
				tempSp = tempSp1;
			}
		}
	}
	return pRet;
}

Sprite* ITools::getNumSprite(int iTotleNum, int iNeedNum, bool bIsRight)
{
	Sprite* spSlash = ReadSpriteName("new_ui/img_number/num1_x.png");
	Sprite* spTotle = getNumSprite(iTotleNum, bIsRight, 3);
	Sprite* spNeed = getNumSprite(iNeedNum, bIsRight, 1);
	Sprite* spRet = NULL;

	if(bIsRight)
	{
		string strNeed = IntToString(iNeedNum);
		int iSpWidth = spSlash->getContentSize().width;

		spSlash->setAnchorPoint(Vec2(0, 0));
		spSlash->setPosition(Vec2(-iSpWidth * (int)strNeed.size(), 0));
		spNeed->addChild(spSlash);

		spTotle->setAnchorPoint(Vec2(1, 0));
		spTotle->setPosition(Vec2(0, 0));
		spSlash->addChild(spTotle);

		spRet = spNeed;
	}
	else
	{
		string strTotle = IntToString(iTotleNum);
		int iSpWidth = spSlash->getContentSize().width;

		spSlash->setAnchorPoint(Vec2(0, 0));
		spSlash->setPosition(Vec2(iSpWidth * strTotle.size(), 0));
		spTotle->addChild(spSlash);

		spNeed->setAnchorPoint(Vec2(0, 0));
		spNeed->setPosition(Vec2(iSpWidth, 0));
		spSlash->addChild(spNeed);

		spRet = spTotle;
	}

	return spRet;
}

void ITools::addHttpData(const char* url, Ref* target, SEL_CallFuncND callback, string data, string strTag, bool bDealError)
{
	if (m_pHttpData.find(strTag) == m_pHttpData.end())
	{
		m_pHttpData[strTag] = new HttpReqData();
	}

	m_pHttpData[strTag]->strUrl = url;
	m_pHttpData[strTag]->pTarget = target;
	m_pHttpData[strTag]->callback = callback;
	m_pHttpData[strTag]->strData = data;
	m_pHttpData[strTag]->strTag = strTag;
	m_pHttpData[strTag]->bDealError = bDealError;
}

void ITools::connectingAgain(string strTag)
{
	HttpReqData* ptrReq = m_pHttpData[strTag];

	HttpRequest* request = new HttpRequest();
	request->setUrl(ptrReq->strUrl.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(this, callfuncND_selector(ITools::onHttpRequestCompleted));
	request->setHeaders(mHead);
	const char* postData = ptrReq->strData.c_str();
	CCLOG("Req : %s...", postData);
	request->setRequestData(postData, strlen(postData));
	request->setTag(ptrReq->strTag.c_str());
	HttpClient::getInstance()->send(request);
	request->release();
}

void ITools::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	removeWaitLayer();
	HttpResponse* response = (HttpResponse*)resultdata;
	if(!response)
	{
		CCLOG("Https:response = null, please check it.");
		return;
	}

	int iResCode = response->getResponseCode();
	string strTag = response->getHttpRequest()->getTag();
	vector<char>* buffer = response->getResponseData();
	string strData(buffer->begin(), buffer->end());
	HttpReqData* ptrReq = m_pHttpData[strTag];

	if(iResCode == -1 || strData == "") //连接超时，再次请求
	{
		CCLOG("code = %d, resData = %s", iResCode, strData.c_str());
		PopLayer* ptrPop = PopLayer::create(POP_REQ_AGAIN, NULL);
		ptrPop->setTag(strTag);
		Director::getInstance()->getRunningScene()->addChild(ptrPop, 98);
	}
	else if(iResCode == 200 || iResCode == 201 || ptrReq->bDealError) //请求成功，调用请求页的回调 || 有些页面需要处理错误
	{
		if(!VerificationNetData(strData))//MD5验证数据不通过退出
			return;
		Ref *pTarget = ptrReq->pTarget;
		SEL_CallFuncND pSelector = ptrReq->callback;
		if(pTarget && pSelector)
		{
			(pTarget->*pSelector)(sender, resultdata);
		}
	}
	else //显示错误提示
	{
		CCLOG("code = %d, resData = %s ,url = %s", iResCode, strData.c_str(), ptrReq->strUrl.c_str());

		Json::Reader reader;
		Json::Value value;
		if(reader.parse(strData, value) && value.size() > 0)
		{
			history->parseErrorMsg(value, strTag);
		}
	}
}

void ITools::startFight(int warId, int battleType, int battleStage, bool autoOut) {
	history->m_iBattleType = battleType;
	history->m_iCurWarId = warId;
	history->m_iBattleStage = battleStage;

	if (autoOut) {


		if (battleType == BATTLE_SIXWAR)
		{
			history->net_win_condition.clear();
			int win_num = 0;
			int war_id = trData->m_mNetWarInfo[warId]->iWarId;
			for (int i = 0; i < trData->win_condition_num; i++)
			{
				if (trData->win_condition[i].warid == war_id)
				{
					history->net_win_condition[win_num++] = i;
				}
			}
		}

		//tmp自动上阵
		auto dispatch = DispatchLayer::create();
		dispatch->setVisible(false);
		ChessApplication::getInstance()->AddFunctionLayer(dispatch);
		dispatch->clickDispatchMenu(nullptr);

		ChildCloseLayer(0);
		//history->updateGuide();
	}
	else {
		auto dispatch = DispatchLayer::create(this);
		ChessApplication::getInstance()->AddFunctionLayer(dispatch);
	}
}

void ITools::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if (trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
	{
		history->game_state = GAME_STATE_REDE;
		Chess::getInstance()->appendScreen(new RedeScreen());
	}
	else
	{
		history->game_state = GAME_STATE_BATTLE;
		history->mTitleLayer = BattleTitleLayer::create();
		ChessApplication::getInstance()->AddFunctionLayer(history->mTitleLayer);
		Chess::getInstance()->appendScreen(new BattleScreen());
	}
}

bool ITools::VerificationNetData(string NetDataS)
{
	Json::Reader VCreader;
	Json::Value VCvalue;
	if (VCreader.parse(NetDataS.c_str(), VCvalue))
	{
		//CCLOG("VCvalue %s,%d", NetDataS.c_str(), (int)VCvalue.size());
		if (!VCvalue["result"].isNull() && !VCvalue["result"]["validate"].isNull())
		{
			string nextS = "\"validate\":";
			size_t nextSIndex = NetDataS.find(nextS.c_str());
			string CurVCS = NetDataS.substr(10, nextSIndex - 11);
			CurVCS += '}';
			string validateCodeS = VCvalue["result"]["validate"].asCString();
			CocosMD5 md5;
			string VerificationS = md5.ToMD5(CurVCS.c_str());
			if (VerificationS != validateCodeS)
			{
				//CCLOG("%s", StringUtils::StringUTF8("非法数据"));
				ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(NULL, SHORTCUT_TYPE_JSYX, 0, 0));
				return false;
			}
		}
		else if (!VCvalue["result"].isNull() && VCvalue["result"]["validate"].isNull())
		{
			//CCLOG("ERROR VCvalue %s", NetDataS.c_str());
			ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(NULL, SHORTCUT_TYPE_JSYX, 0, 0));
			return false;
		}
	}
	return true;
}
