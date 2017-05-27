//
//  ActorPracticeLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/12/26.
//
//

#include "ActorPracticeLayer.h"
#include "PopLayer.h"

enum
{
	REQ_PRACTICE = 0
};

#define MAX_PRAC_ATTR 5
#define MAX_GROWN 5.0f
#define GROWN_VALUE 0.1f
#define SOUL_USED 10

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PracticeCell* PracticeCell::create(HistoryActor* ptrActor, int iAttrType, EventDelegate* ptrDel)
{
	PracticeCell *pRet = new PracticeCell();
	if(pRet && pRet->init(ptrActor, iAttrType, ptrDel))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool PracticeCell::init(HistoryActor* ptrActor, int iAttrType, EventDelegate* ptrDel)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_ptrActor = ptrActor;
	m_iAttrType = iAttrType;
	m_ptrDel = ptrDel;

	m_iGoodsId = trData->net_hero_list[m_ptrActor->actor_id + 1]->good_id;
	map<int, string> mstrAttr;
	mstrAttr[0] = LL("gongji");
	mstrAttr[1] = LL("jingshen");
	mstrAttr[2] = LL("fangyu");
	mstrAttr[3] = LL("baofa");
	mstrAttr[4] = LL("shiqi");
	m_strAttrKey = mstrAttr[m_iAttrType];

	initControl();
	updateCell();
	return true;
}

void PracticeCell::SetPopLayerElement(Sprite* spBG)
{
	string strNotice1 = StringUtils::format(LL("practicenotice2"), SOUL_USED, m_strAttrKey.c_str());
	string strNotice2 = StringUtils::format(LL("practicenotice3"), GROWN_VALUE);

	Label* labNotice1 = LabelSystemFont(strNotice1, 18);
	labNotice1->setPosition(Vec2(spBG->getContentSize().width / 2, 137));
	spBG->addChild(labNotice1);

	Label* labNotice2 = LabelSystemFont(strNotice2, 16);
	labNotice2->setPosition(Vec2(spBG->getContentSize().width / 2, 114));
	labNotice2->setColor(Color3B::YELLOW);
	spBG->addChild(labNotice2);
}

void PracticeCell::callBackConfirm()
{
	onHttpRequest(REQ_PRACTICE);
}

void PracticeCell::updateCell()
{
	//属性值
	m_labAttrVal->setString(StringUtils::format("%d", m_ptrActor->getRealAbility(m_iAttrType)));

	//原始成长
	m_labGrown1->setString(StringUtils::format("(%.1f)", m_ptrActor->getGrownValue(m_iAttrType, false)));

	//修炼成长
	float fPracGrown = m_ptrActor->getPracGrown(m_iAttrType);
	m_labGrown2->setString(StringUtils::format("+ %.1f", fPracGrown));

	//MAX
	m_labMax->setVisible(fPracGrown >= MAX_GROWN);

	//修炼
	m_btnPractice->setVisible(fPracGrown < MAX_GROWN);
}

void PracticeCell::initControl()
{
	float fScale = 0.90f;

	//背景
	Sprite* spCellBG = ReadSpriteName("new_ui/hero/hero_xl_bg2.png");
	this->addChild(spCellBG);

	Vec2 vecPos = Vec2(0, spCellBG->getContentSize().height / 2);

	//属性名
	m_labAttr = LabelSystemFont(m_strAttrKey, 18);
	m_labAttr->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labAttr->setPosition(vecPos + Vec2(24, 0));
	m_labAttr->enableOutline(Color4B(102, 50, 0, 255), 1);
	spCellBG->addChild(m_labAttr);

	//属性值
	m_labAttrVal = LabelSystemFont("", 18);
	m_labAttrVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labAttrVal->setPosition(vecPos + Vec2(72, 0));
	m_labAttrVal->enableOutline(Color4B(102, 50, 0, 255), 1);
	spCellBG->addChild(m_labAttrVal);

	//原始成长
	m_labGrown1 = LabelSystemFont("", 18);
	m_labGrown1->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labGrown1->setPosition(vecPos + Vec2(148, 0));
	m_labGrown1->setColor(Color3B::YELLOW);
	m_labGrown1->enableOutline(Color4B(102, 50, 0, 255), 1);
	spCellBG->addChild(m_labGrown1);

	//修炼成长
	m_labGrown2 = LabelSystemFont("", 18);
	m_labGrown2->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labGrown2->setPosition(vecPos + Vec2(218, 0));
	m_labGrown2->setColor(Color3B::GREEN);
	m_labGrown2->enableOutline(Color4B(102, 50, 0, 255), 1);
	spCellBG->addChild(m_labGrown2);

	//MAX
	m_labMax = LabelSystemFont(StringUtils::format("(%s)", LL("alreadyfull")), 18);
	m_labMax->setPosition(vecPos + Vec2(325, 0));
	m_labMax->setColor(Color3B::YELLOW);
	m_labMax->enableOutline(Color4B(102, 50, 0, 255), 1);
	spCellBG->addChild(m_labMax);

	//修炼
	Sprite* spPracN = ReadSpriteName("new_ui/hero/hero_add.png");
	Sprite* spPracS = ReadSpriteName("new_ui/hero/hero_add.png");
	spPracS->setScale(fScale);
	m_btnPractice = MenuItemSprite::create(spPracN, spPracS, CC_CALLBACK_1(PracticeCell::callBackPractice, this));
	spPracS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spPracS->setPosition(Vec2(spPracS->getContentSize().width / 2, spPracS->getContentSize().height / 2));
	Menu* ptrMenu = Menu::create(m_btnPractice, nullptr);
	ptrMenu->setPosition(vecPos + Vec2(325, 0));
	spCellBG->addChild(ptrMenu);
}

void PracticeCell::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case REQ_PRACTICE:
	{
		strURL = "practice";
		string validCode = md5.ToMD5(StringUtils::format("goodID=%d&id=%d&gameToken=%s", m_iGoodsId, m_iAttrType, strToken.c_str()));
		strData = StringUtils::format("{\"goodID\":%d,\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iGoodsId, m_iAttrType, strToken.c_str(), validCode.c_str());
		strTag = "REQ_PRACTICE";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(PracticeCell::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void PracticeCell::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	string responseData = temp;
	CCLOG("%s...", responseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData, value) && value.size() > 0)
	{
		if(strTag == "REQ_PRACTICE")
		{
			onStoreInitSucceed(value["result"]);
		}
	}
}

void PracticeCell::onStoreInitSucceed(Json::Value valRes)
{
	if(valRes["minus"].isInt() && valRes["minus"].asInt()) //扣除消耗材料
	{
		history->mChipSoulDate->ReduceActorSoulNumByGoodsId(m_iGoodsId, valRes["minus"].asInt());
	}

	if(valRes["isTrue"].isInt() && valRes["isTrue"].asInt())
	{
		map<int, double> mPracGrown;
		ParseDoubleOrInt(mPracGrown[0], valRes["wl"]);
		ParseDoubleOrInt(mPracGrown[1], valRes["zl"]);
		ParseDoubleOrInt(mPracGrown[2], valRes["ts"]);
		ParseDoubleOrInt(mPracGrown[3], valRes["mj"]);
		ParseDoubleOrInt(mPracGrown[4], valRes["yq"]);
		for(int i = 0; i < MAX_PRAC_ATTR; i++)
		{
			m_ptrActor->setPracGrown(i, (float)mPracGrown[i]);
		}
		m_ptrActor->UpdataGoodEffect();
		ITools::getInstance()->createGlobalPrompt(LL("practicesucceed"), true);
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("practicefailed"), false);
	}
	m_ptrDel->updateState();
}

void PracticeCell::callBackPractice(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if(history->mChipSoulDate->GetActorSoulNumByGoodsId(m_iGoodsId) >= SOUL_USED)
	{
		ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_PRAC_GROWN, this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("soulnotenough"), false);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActorPracticeLayer::ActorPracticeLayer()
{

}

ActorPracticeLayer::~ActorPracticeLayer()
{

}

ActorPracticeLayer* ActorPracticeLayer::create(HistoryActor* ptrActor, EventDelegate* ptrDel)
{
	ActorPracticeLayer* pRet = new ActorPracticeLayer();
	if(pRet&&pRet->init(ptrActor, ptrDel))
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

bool ActorPracticeLayer::init(HistoryActor* ptrActor, EventDelegate* ptrDel)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrActor = ptrActor;
	m_ptrDel = ptrDel;

	initControl();
	updateSoul();
	return true;
}

void ActorPracticeLayer::updateState()
{
	updateSoul();
	for (int i = 0; i < MAX_PRAC_ATTR; i++)
	{
		m_mPracCell[i]->updateCell();
	}
	m_ptrDel->updateShow(0);
}

void ActorPracticeLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* spBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	spBlack->setPosition(Vec2(0, 0));
	this->addChild(spBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/hero/hero_xl_bg1.png");
	spBG->setPosition(point + Vec2(0, -8));
	this->addChild(spBG);

	//标题
	Sprite* spTitle = ReadSpriteName("new_ui/hero/hero_xl_txt1.png");
	spTitle->setPosition(point + Vec2(0, 168));
	this->addChild(spTitle);

	//武将背景
	Sprite* spHeroBG = ReadSpriteName("new_ui/hero/hero_xl_txt2.png");
	spHeroBG->setPosition(point + Vec2(0, 120));
	this->addChild(spHeroBG);

	//形象时装
	int iImgWidth = 48;
	if(m_ptrActor->mFashion[0] <= 0)
	{
		Sprite* spFace = ReadSpriteNormal(m_ptrActor->getActorSImgPath("mov", 0));
		int iWidth = spFace->getContentSize().width;
		spFace->setTextureRect(Rect(0, 6 * iWidth, iWidth, iWidth));
		spFace->setPosition(Vec2(15, 24));
		spHeroBG->addChild(spFace);
	}
	else
	{
		string strFace = StringUtils::format("img/Nunits/%d_%d_mov.png", m_ptrActor->mFashion[1], m_ptrActor->mFashion[2]);
		string strFashion = StringUtils::format("img/Nunits/%d_%d_mov.png", m_ptrActor->mFashion[3], m_ptrActor->mFashion[4]);

		Sprite* spFace = ReadSpriteNormal(strFace);
		int iWidth = spFace->getContentSize().width;
		spFace->setTextureRect(Rect(0, 6 * iWidth, iWidth, iWidth));
		spFace->setPosition(Vec2(15, 24));
		spHeroBG->addChild(spFace, 1);

		Sprite* spFashion = ReadSpriteNormal(strFashion);
		iWidth = spFashion->getContentSize().width;
		spFashion->setTextureRect(Rect(0, 6 * iWidth, iWidth, iWidth));
		spFashion->setPosition(Vec2(15, 24));
		spHeroBG->addChild(spFashion, trData->net_hero_dress[m_ptrActor->mFashion[4]]->mMov[6] ? 2 : 0);
	}

	//武将名
	auto labName = LabelSystemFont(trData->actors[m_ptrActor->actor_id].getActorName(), 18);
	labName->setAnchorPoint(Vec2(0.0f, 0.5f));
	labName->setPosition(Vec2(38, 18));
	labName->setColor(Color3B::YELLOW);
	labName->enableOutline(Color4B::BLACK, 1);
	spHeroBG->addChild(labName);

	//将魂ICON
	Sprite* spSoul = ReadSpriteName("new_ui/hero/icon_soul.png");
	spSoul->setPosition(Vec2(156, 18));
	spSoul->setScale(0.8f);
	spHeroBG->addChild(spSoul);

	//将魂数
	m_labSoul = LabelSystemFont("", 18);
	m_labSoul->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labSoul->setPosition(Vec2(168, 18));
	m_labSoul->enableOutline(Color4B::BLACK, 1);
	spHeroBG->addChild(m_labSoul);

	//五维属性
	for (int i = 0; i < MAX_PRAC_ATTR; i++)
	{
		m_mPracCell[i] = PracticeCell::create(m_ptrActor, i, this);
		m_mPracCell[i]->setPosition(point + Vec2(0, 72 - i * 50));
		this->addChild(m_mPracCell[i]);
	}

	//提示
	auto labNotice = LabelSystemFont(StringUtils::format(LL("practicenotice1"), MAX_GROWN), 15);
	labNotice->setPosition(point + Vec2(0, -168));
	labNotice->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(labNotice);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(ActorPracticeLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(222, 168));
	ptrMenu->addChild(btnClose);
}

void ActorPracticeLayer::updateSoul()
{
	int iSoulCount = history->mChipSoulDate->GetActorChipNumByActorId(m_ptrActor->actor_id + 1);
	m_labSoul->setString(StringUtils::format("%d", iSoulCount));
}

void ActorPracticeLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	removeFromParent();
}