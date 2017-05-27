//
//  GQRankLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/12.
//
//

#include "GQRankLayer.h"

#define MAX_RANK_NUM 3

RankHeroSprite* RankHeroSprite::create(RankHero* ptrRank)
{
	RankHeroSprite *pRet = new RankHeroSprite();
	if(pRet && pRet->init(ptrRank))
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

bool RankHeroSprite::init(RankHero* ptrRank)
{
	if(!Sprite::init())
	{
		return false;
	}

	//玩家信息背景
	Sprite* spHeroBG = ReadSpriteName("new_ui/battle/battle_phb2.png");
	this->addChild(spHeroBG);

	Vec2 posCentre = Vec2(spHeroBG->getContentSize().width / 2, spHeroBG->getContentSize().height / 2);

	//名次
	__String* strRank = __String::createWithFormat("new_ui/battle/battle_tx%d.png", ptrRank->getRankID());
	Sprite* spRank = ReadSpriteName(strRank->getCString());
	spRank->setPosition(posCentre + Vec2(-108, 3));
	spHeroBG->addChild(spRank);

	if (ptrRank->getIsExist())
	{
		//武将造型
		map<int, int> meFashion;
		meFashion[2] = ptrRank->getFashion1();
		meFashion[3] = 100;
		meFashion[4] = ptrRank->getFashion2();
		int iFashion3 = ptrRank->getFashion3();
		int iFashion4 = ptrRank->getFashion4();

		if (iFashion3 > 0)
			meFashion[2] = iFashion3;
		if(iFashion4 > 0)
			meFashion[4] = iFashion4;
		meFashion[1] = ptrRank->getGender();
		if (meFashion[1] < 1) meFashion[1] = 1;

		BgAddFashionSImg(1, meFashion, posCentre + Vec2(-50, 3), 6, spHeroBG, NULL,NULL, "mov");

		//名字
		Label* labNameLv = LabelSystemFont(ptrRank->getName().c_str(), 16);
		labNameLv->setColor(Color3B(102, 0, 0));
		labNameLv->setAnchorPoint(Vec2(0.0f, 0.5f));
		labNameLv->setPosition(posCentre + Vec2(-20, 11));
		spHeroBG->addChild(labNameLv);

		//积分
		__String* strScore = __String::createWithFormat("%s%d", LL("rankscore"), ptrRank->getScore());
		Label* labScore = LabelSystemFont(strScore->getCString(), 16);
		labScore->setColor(Color3B(102, 0, 0));
		labScore->setAnchorPoint(Vec2(0.0f, 0.5f));
		labScore->setPosition(posCentre + Vec2(-20, -11));
		spHeroBG->addChild(labScore);
	}

	return true;
}

GQRankLayer* GQRankLayer::create(int iWarId)
{
	GQRankLayer *pRet = new GQRankLayer();
	if(pRet && pRet->init(iWarId))
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

bool GQRankLayer::init(int iWarId)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iWarId = iWarId;

	for(int i = 0; i < MAX_RANK_NUM; i++)
	{
		m_mRankHero[i] = new RankHero();
		m_mRankHero[i]->setRankID(i + 1);
		m_mRankHero[i]->setIsExist(0);
	}

	initControl();
	rankHttpRequest();

	return true;
}

bool GQRankLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}
void GQRankLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}
void GQRankLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

GQRankLayer::GQRankLayer()
{

}

GQRankLayer::~GQRankLayer()
{
	for(int i = 0; i < MAX_RANK_NUM; i++)
	{
		SAFE_DELETE(m_mRankHero[i]);
	}
}

void GQRankLayer::onEnter()
{
	Layer::onEnter();
}

void GQRankLayer::onExit()
{
	Layer::onExit();
}

void GQRankLayer::initControl()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/battle/battle_phb1.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	auto labTitle = LabelSystemFont(trData->m_mNetWarInfo[m_iWarId]->strName, 18);
	labTitle->setPosition(Vec2(spBG->getContentSize().width / 2, spBG->getContentSize().height - 20));
	labTitle->enableOutline(Color4B::BLACK, 1);
	spBG->addChild(labTitle);

	//玩家信息背景
	Sprite* spHeroBG = ReadSpriteName("new_ui/battle/battle_phb3.png");
	spHeroBG->setPosition(point + Vec2(0, -105));
	this->addChild(spHeroBG);

	//武将造型
	Vec2 posCentre = Vec2(spHeroBG->getContentSize().width / 2, spHeroBG->getContentSize().height / 2);

	BgAddFashionSImg(1, history->actors[0].mFashion, posCentre + Vec2(-50, 3), 6, spHeroBG, NULL, NULL, "mov");

	//名字
	Label* labNameLv = LabelSystemFont(history->m_pPlayerInfo->getName().c_str(), 16);
	labNameLv->setColor(Color3B::YELLOW);
	labNameLv->setAnchorPoint(Vec2(0.0f, 0.5f));
	labNameLv->setPosition(posCentre + Vec2(-20, 11));
	spHeroBG->addChild(labNameLv);

	//积分
	int iScore = 0;
	MapDateElem* ptrMap = history->mMapDateInfo->getMapDateElem(m_iWarId);
	if (ptrMap)
	{
		iScore = ptrMap->getSCORE();
	}
	__String* strScore = __String::createWithFormat("%s%d", LL("rankscore"), iScore);
	Label* labScore = LabelSystemFont(strScore->getCString(), 16);
	labScore->setAnchorPoint(Vec2(0.0f, 0.5f));
	labScore->setPosition(posCentre + Vec2(-20, -11));
	spHeroBG->addChild(labScore);
}

void GQRankLayer::rankHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("warID=%d&gameToken=%s", m_iWarId, strToken.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"warID\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iWarId, strToken.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("getRank",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GQRankLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void GQRankLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		onRankSucceed(value["result"]);
	}
}

bool GQRankLayer::onRankSucceed(Json::Value valRes)
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	Json::Value valRanks = valRes["ranks"];
	if(valRanks.isArray())
	{
		for(UInt i = 0; i < valRanks.size(); i++)
		{
			int iIndex = valRanks[i]["rank"].asInt() - 1;
			if (iIndex >= 0 && iIndex < MAX_RANK_NUM)
			{
				m_mRankHero[iIndex]->setIsExist(1);
				m_mRankHero[iIndex]->setFashion1(valRanks[i]["Fashion1"].asInt());
				m_mRankHero[iIndex]->setFashion2(valRanks[i]["Fashion2"].asInt());
				m_mRankHero[iIndex]->setFashion3(valRanks[i]["Fashion3"].asInt());
				m_mRankHero[iIndex]->setFashion4(valRanks[i]["Fashion4"].asInt());
				m_mRankHero[iIndex]->setGender(valRanks[i]["gender"].asInt()+1);
				m_mRankHero[iIndex]->setScore(valRanks[i]["score"].asInt());
				m_mRankHero[iIndex]->setName(valRanks[i]["name"].asString());
			}
		}
	}

	for(int i = 0; i < MAX_RANK_NUM; i++)
	{
		RankHeroSprite* spRankHero = RankHeroSprite::create(m_mRankHero[i]);
		spRankHero->setPosition(point + Vec2(0, 105 - i * 70));
		this->addChild(spRankHero);
	}

	return true;
}
