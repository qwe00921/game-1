#include "NetPromptTool.h"
#include "Defines.h"
#include "CocoMd5.h"
#include "PaymentTool.h"

NetPromptTool* NetPromptTool::m_instance = NULL;
NetPromptTool* NetPromptTool::getInstance()
{
	if(m_instance == NULL)
	{
		m_instance = NetPromptTool::create();
		Director::getInstance()->getRunningScene()->addChild(m_instance);
	}
	return m_instance;
}

NetPromptTool* NetPromptTool::create()
{
	NetPromptTool* pRet = new NetPromptTool();
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool NetPromptTool::init()
{
	mOrderId = "";
	mGetOrderStatus = false;
	mQueryCount = 0;
	if (!Node::init())
	{
		return false;
	}
	return true;
}


NetPromptTool::NetPromptTool()
{
	mNetPromptLayer = NetPromptLayer::create();
	Director::getInstance()->getRunningScene()->addChild(mNetPromptLayer, 98);
	m_iPListIndex = -1;
}

void NetPromptTool::StartGetNetPrompt()
{
	StartSendNetData(0);
	this->schedule(schedule_selector(NetPromptTool::StartSendNetData), SEND_NET_INTERVAL);

	this->schedule(schedule_selector(NetPromptTool::getOrderStatus), 8);
}

void NetPromptTool::StopGetNetPrompt()
{
	this->unschedule(schedule_selector(NetPromptTool::StartSendNetData));

	this->unschedule(schedule_selector(NetPromptTool::getOrderStatus));
}

void NetPromptTool::setGetOrderStatus(bool start, const char* orderId) {
	mGetOrderStatus = start;
	mOrderId = orderId;
	mQueryCount = 0;
}


void NetPromptTool::AddNetPrompt(int iType, int iGoodsId, int iNomal)
{
	NetPromptElem* ptrNetPromptElem = new NetPromptElem();
	ptrNetPromptElem->seteINDEX(0);
	ptrNetPromptElem->seteTYPE(iType);
	ptrNetPromptElem->seteISSYS(false);
	map<string,string> mInfoE;

	string contentS = "";
	int iNum = 0;
	string heroName = history->m_pPlayerInfo->getName();
	if (iType == PROMPT_QH)
	{
		contentS = LL("gxsdsqhddx");
		iNum = 3;
		mInfoE["Q1"] = heroName;
		mInfoE["Q2"] = trData->m_mGoodsItem[iGoodsId]->strName;
		mInfoE["Q3"] = IntToString(iNomal);
	}
	else if (iType == PROMPT_WJJJ)
	{
		contentS = LL("gxsdspztswss");
		iNum = 3;
		mInfoE["Q1"] = heroName;
		mInfoE["Q2"] = trData->actors[iGoodsId - 1].getActorName();
		if(iNomal == 2)
			mInfoE["Q3"] = LL("lvse");
		else if (iNomal == 3)
			mInfoE["Q3"] = LL("lanse");
		else if (iNomal == 4)
			mInfoE["Q3"] = LL("zise");
		else if (iNomal == 5)
			mInfoE["Q3"] = LL("jinse");
	}
	else if (iType == PROMPT_WJZM)
	{
		contentS = LL("sywsxqmzn");
		iNum = 2;
		mInfoE["Q1"] = trData->actors[iGoodsId - 1].getActorName();
		mInfoE["Q2"] = heroName;
	}
	else if (iType == PROMPT_WQHC)
	{
		contentS = LL("gxshdls");
		iNum = 2;
		mInfoE["Q1"] = heroName;
		mInfoE["Q2"] = trData->m_mGoodsItem[iGoodsId]->strName;
	}
	else if (iType == PROMPT_YYTKQ)
	{
		contentS = LL("wjskqlyyts");
		iNum = 2;
		mInfoE["Q1"] = heroName;
		mInfoE["Q2"] = trData->net_story[iGoodsId]->name;
	}

	ptrNetPromptElem->seteCONTENTS(contentS);
	ptrNetPromptElem->seteNUM(iNum);
	ptrNetPromptElem->mInfoElem = mInfoE;


	int iListSize = m_mPromptList.size();
	m_mPromptList[iListSize] = ptrNetPromptElem;
	StartLoopPromptList();
}

void NetPromptTool::UpdataOrdinaryTxt(float f)
{
	if (m_iPListIndex >= (int)m_mPromptList.size())
	{
		StopUpdataOrdinaryTxt();
		return;
	}
	mNetPromptLayer->UpdataTxtLab(m_mPromptList[m_iPListIndex]);
	m_iPListIndex++;
}

void NetPromptTool::StopUpdataOrdinaryTxt()
{
	this->unschedule(schedule_selector(NetPromptTool::UpdataOrdinaryTxt));
	clearAllPromptList();
	mNetPromptLayer->StopPlay();
}

void NetPromptTool::StartLoopPromptList()
{
	if (m_iPListIndex < 0)
	{
		m_iPListIndex = 0;
		UpdataOrdinaryTxt(0);
		this->schedule(schedule_selector(NetPromptTool::UpdataOrdinaryTxt), LOOP_TXT_INTERVAL);
	}
}

void NetPromptTool::clearAllPromptList()
{
	m_iPListIndex = -1;
	for (UInt i = 0; i < m_mPromptList.size(); i++)
	{
		if(m_mPromptList[i])
			delete m_mPromptList[i];
	}
	m_mPromptList.clear();
}

void NetPromptTool::StartSendNetData(float f)
{
	const char* url = "https://api.leancloud.cn/1.1/functions/heartbeat";
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(this, callfuncND_selector(NetPromptTool::onHttpRequestCompleted));
	request->setHeaders(ITools::getInstance()->mHead);
	HttpClient::getInstance()->send(request);
	request->release();

}

void NetPromptTool::getOrderStatus(float f) {
	if (mGetOrderStatus) {
		mQueryCount++;
		PaymentTool::getInstance()->getOrderStatus(mOrderId.c_str());
		if (mQueryCount > 20) {
			setGetOrderStatus(false, "");
		}
	}
}


void NetPromptTool::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	HttpResponse* response = (HttpResponse*)resultdata;
	int iResCode = response->getResponseCode();
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	//CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (iResCode == 200 || iResCode == 201)
	{
		if (reader.parse(responseData->getCString(), value) && value.size() > 0)
		{
			LoadPromptTxt(value["result"]);
		}
	}

}

void NetPromptTool::LoadPromptTxt(Json::Value jsonData)
{
	Json::Value barrageArr = jsonData["barrageArr"];

	if (jsonData["sysMes"].isString())
	{
		NetPromptElem* ptrNetPromptElem = new NetPromptElem();
		ptrNetPromptElem->seteCONTENTS(jsonData["sysMes"].asCString());
		ptrNetPromptElem->seteISSYS(true);
		int iDataListSize = m_mPromptList.size();
		m_mPromptList[iDataListSize] = ptrNetPromptElem;
	}

	if (barrageArr.isArray())
	{
		string heroName = history->m_pPlayerInfo->getName();
		map<int, int> mPIdList;

		for(UInt i = 0;i < barrageArr.size();i++)
		{
			int iIndex = 0, iType = 0, iNum = 0;
			string playernameS = "";
			string contentS = "";
			ParseInt(iIndex, barrageArr[i]["index"]);
			ParseInt(iType, barrageArr[i]["type"]);
			ParseInt(iNum, barrageArr[i]["num"]);
			ParseString(playernameS, barrageArr[i]["playerName"]);
			ParseString(contentS, barrageArr[i]["content"]);
			
			if (!m_mPromptIdlist.count(iIndex) && playernameS != heroName)
			{
				NetPromptElem* ptrNetPromptElem = new NetPromptElem();
				ptrNetPromptElem->seteINDEX(iIndex);
				ptrNetPromptElem->seteNUM(iNum);
				ptrNetPromptElem->seteTYPE(iType);
				ptrNetPromptElem->seteCONTENTS(contentS);
				ptrNetPromptElem->seteISSYS(false);

				if (!barrageArr[i]["info"].isNull())
				{
					Json::Value promptInfo = barrageArr[i]["info"];

					for (Json::Value::iterator it = promptInfo.begin(); it != promptInfo.end(); it++)
					{
						string pInfoKey = it.key().asCString();
						string pInfoData = promptInfo[pInfoKey].asCString();
						ptrNetPromptElem->mInfoElem[pInfoKey] = pInfoData;
					}
				}

				int iDataListSize = m_mPromptList.size();
				m_mPromptList[iDataListSize] = ptrNetPromptElem;
				mPIdList[iIndex] = 1;
			}
		}

		m_mPromptIdlist.clear();
		m_mPromptIdlist = mPIdList;
	}

	if (m_mPromptList.size())
	{
		StartLoopPromptList();
	}
}

//////////////////////////////

bool NetPromptLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	m_bg = ReadSpriteName("new_ui/img_global/global_txt_bg1.png");
	m_bg->setPosition(Vec2(size.width/2,size.height*0.86f));
	this->addChild(m_bg);
	m_bg->setVisible(false);

	m_iBgWidth = m_bg->getContentSize().width;
	m_iBgHeight = m_bg->getContentSize().height;

	Vec2 DrawNp[4];
	DrawNp[0] = Vec2(0, 0);
	DrawNp[1] = Vec2(0, m_iBgHeight);
	DrawNp[2] = Vec2(m_iBgWidth, m_iBgHeight);
	DrawNp[3] = Vec2(m_iBgWidth, 0);

	DrawNode* ptrDrawN = DrawNode::create();
	ptrDrawN->drawPolygon(DrawNp, 4, Color4F(0, 0, 0, 0), 1, Color4F(0, 0, 0, 0));

	mClippingNode = ClippingNode::create(ptrDrawN);
	mClippingNode->setAnchorPoint(Vec2(0, 0));
	mClippingNode->setPosition(Vec2(0, 0));
	m_bg->addChild(mClippingNode);

	m_bIsVisibal = false;

	m_pTxtLabSp[1] = ReadSpriteName("new_ui/img_global/global_txt_bg1.png");
	m_pTxtLabSp[1]->setPosition(Vec2(0, 0));
	m_pTxtLabSp[1]->setAnchorPoint(Vec2(0,0));
	m_pTxtLabSp[1]->setOpacity(0);
	mClippingNode->addChild(m_pTxtLabSp[1]);

	m_pTxtLabSp[2] = ReadSpriteName("new_ui/img_global/global_txt_bg1.png");
	m_pTxtLabSp[2]->setPosition(Vec2(0, -m_iBgHeight));
	m_pTxtLabSp[2]->setAnchorPoint(Vec2(0, 0));
	m_pTxtLabSp[2]->setOpacity(0);
	mClippingNode->addChild(m_pTxtLabSp[2]);

	m_iCurLabNum = 0;
	m_fOffSetX = 0;
	m_fOffSetY = 0;

	return true;
}

void NetPromptLayer::StopPlay()
{
	m_pTxtLabSp[1]->removeAllChildren();
	m_pTxtLabSp[2]->removeAllChildren();
	m_iCurLabNum = 0;
	m_bIsVisibal = false;

	FadeOut* fo = FadeOut::create(0.08f);
	CallFuncN* ad2 = CallFuncN::create(CC_CALLBACK_1(NetPromptLayer::BgFadeOut, this));
	Sequence* seq = Sequence::create(fo,ad2, NULL);
	m_bg->runAction(seq);
}

void NetPromptLayer::BgFadeOut(Node* node)
{
	m_bg->setVisible(false);
}

void NetPromptLayer::BgFadein(Node* node)
{
	m_bg->setVisible(true);
}

void NetPromptLayer::UpdataTxtLab(NetPromptElem* ptrNPE)
{
	bool bMoveLeft = false;
	if (!m_bIsVisibal)
	{
		m_bIsVisibal = true;
		FadeIn* fi = FadeIn::create(0.08f);
		CallFuncN* ad2 = CallFuncN::create(CC_CALLBACK_1(NetPromptLayer::BgFadein, this));
		Sequence* seq = Sequence::create(fi, ad2, NULL);
		m_bg->runAction(seq);
		//下面3行位置不可换
		m_pTxtLabSp[1]->setPosition(Vec2(0, 0));
		m_pTxtLabSp[2]->setPosition(Vec2(0, -m_iBgHeight));
		bMoveLeft = UpdataPromptSpShow(ptrNPE, 1);
		m_iCurLabNum = 1;
	}
	else if (m_iCurLabNum == 1)
	{
		//下面2行位置不可换
		m_pTxtLabSp[2]->setPosition(Vec2(0, -m_iBgHeight));
		bMoveLeft = UpdataPromptSpShow(ptrNPE, 2);
		m_iCurLabNum = 2;

		MoveTo* ptrMoveTo1 = MoveTo::create(TXT_UP_TIME, Vec2(0, m_iBgHeight));
		m_pTxtLabSp[1]->runAction(ptrMoveTo1);

		MoveTo* ptrMoveTo2 = MoveTo::create(TXT_UP_TIME, Vec2(0, 0));
		m_pTxtLabSp[2]->runAction(ptrMoveTo2);
	}
	else if (m_iCurLabNum == 2)
	{
		//下面2行位置不可换
		m_pTxtLabSp[1]->setPosition(Vec2(0, -m_iBgHeight));
		bMoveLeft = UpdataPromptSpShow(ptrNPE, 1);
		m_iCurLabNum = 1;

		MoveTo* ptrMoveTo1 = MoveTo::create(TXT_UP_TIME, Vec2(0, m_iBgHeight));
		m_pTxtLabSp[2]->runAction(ptrMoveTo1);

		MoveTo* ptrMoveTo2 = MoveTo::create(TXT_UP_TIME, Vec2(0, 0));
		m_pTxtLabSp[1]->runAction(ptrMoveTo2);
	}

	if (bMoveLeft)
	{
		this->scheduleOnce(schedule_selector(NetPromptLayer::DelayMoveLeft), TXT_UP_TIME + 0.5f);
	}
}

void NetPromptLayer::DelayMoveLeft(float f)
{
	MoveTo* ptrMoveTo = MoveTo::create(MOVE_LEFT_TIME, Vec2(-m_fOffSetX, 0));
	m_pTxtLabSp[m_iCurLabNum]->runAction(ptrMoveTo);
}

bool NetPromptLayer::UpdataPromptSpShow(NetPromptElem* ptrNPE, int iSpIndex)
{
	bool iRet = false;
	m_pTxtLabSp[iSpIndex]->removeAllChildren();

	if (ptrNPE->geteISSYS())
	{
		Label* txtLab = LabelSystemFont(ptrNPE->geteCONTENTS().c_str(),14);

		int iElemWidth = txtLab->getContentSize().width;

		if (iElemWidth > m_iBgWidth)
		{
			txtLab->setAnchorPoint(Vec2(0,0.5));
			txtLab->setPosition(Vec2(0,m_iBgHeight/2));
			iRet = true;
			m_fOffSetX = iElemWidth - m_iBgWidth + 8;
		}
		else
		{
			txtLab->setAnchorPoint(Vec2(0.5, 0.5));
			txtLab->setPosition(Vec2(m_iBgWidth / 2, m_iBgHeight / 2));
		}
		m_pTxtLabSp[iSpIndex]->addChild(txtLab);
	}
	else
	{
		{
			map<int, string> mStringMap;
			string chData = ptrNPE->geteCONTENTS();
			DString strData = chData.c_str();
			DString strCur = "";
			int i = 0, index1 = -1, index2 = -1;
			bool addQ = false;
			do
			{
				index1 = index2 + 1;
				index2 = strData->indexOf("Q", index1);
				addQ = false;

				if (index2 != -1)
				{
					strCur = strData->substring(index1, index2);
					addQ = true;
				}
				else
				{
					strCur = strData->substring(index1, chData.length());
				}

				if (strCur != "")
				{
					mStringMap[i++] = strCur->getChar();
				}

				if (addQ)
				{
					mStringMap[i++] = "Q";
				}

			} while (index2 != -1);

			int elemIndex = 1;
			int iElemWidth = 0;
			Label* ptrSLabel = NULL;
			float StartPX = 0;
			map<int, Label*> allTxtLab;
			int allTxtLabIndex = 0;
			map<int, float> labStartX;
			for (size_t j = 0; j < mStringMap.size();j++)
			{
				if (mStringMap[j] == "Q")
				{
					string sKey = StringUtils::format("Q%d", elemIndex);
					if (ptrNPE->mInfoElem.count(sKey))
					{
						ptrSLabel = LabelSystemFont(ptrNPE->mInfoElem[sKey].c_str(),14);
						ptrSLabel->setColor(GetStringColor(ptrNPE->geteTYPE(), elemIndex));
						ptrSLabel->setAnchorPoint(Vec2(0,0.5f));
						//ptrSLabel->setPosition(Vec2(StartPX,m_iBgHeight/2));
						m_pTxtLabSp[iSpIndex]->addChild(ptrSLabel);

						labStartX[allTxtLabIndex] = StartPX + 2;
						allTxtLab[allTxtLabIndex++] = ptrSLabel;
						StartPX += ptrSLabel->getContentSize().width+2;
					}
					elemIndex++;
				}
				else
				{
					ptrSLabel = LabelSystemFont(mStringMap[j].c_str(), 14);
					ptrSLabel->setAnchorPoint(Vec2(0, 0.5f));
					//ptrSLabel->setPosition(Vec2(StartPX, m_iBgHeight / 2));
					m_pTxtLabSp[iSpIndex]->addChild(ptrSLabel);
					labStartX[allTxtLabIndex] = StartPX;
					allTxtLab[allTxtLabIndex++] = ptrSLabel;
					StartPX += ptrSLabel->getContentSize().width+2;
				}
			}

			float iBgStartX = 0;
			StartPX -= 2;
			if (StartPX > m_iBgWidth)
			{
				iRet = true;
				m_fOffSetX = StartPX - m_iBgWidth + 8;
			}
			else
			{
				iBgStartX = (m_iBgWidth - StartPX) / 2;
			}

			for (size_t k = 0; k < allTxtLab.size(); k++)
			{
				allTxtLab[k]->setPosition(Vec2(iBgStartX + labStartX[k], m_iBgHeight / 2));
			}
		}
	}

	return iRet;
}

Color3B NetPromptLayer::GetStringColor(int iType, int iIndex)
{
	Color3B cRet = Color3B::WHITE;

	if (iType == PROMPT_WJZM)
	{
		if (iIndex == 1)
		{
			cRet = Color3B::GREEN;
		}
		else if (iIndex == 2)
		{
			cRet = Color3B::YELLOW;
		}
	}
	else
	{
		if (iIndex == 1)
		{
			cRet = Color3B::YELLOW;
		}
		else if (iIndex == 2)
		{
			cRet = Color3B::GREEN;
		}
	}

	return cRet;
}

NetPromptLayer::NetPromptLayer()
{

}

NetPromptLayer::~NetPromptLayer()
{

}