#include "ActorTalkLayer.h"
#include "Defines.h"

ActorTalkLayer* ActorTalkLayer::create(string stringinfo,RedeScreen* ptrRead)
{
	ActorTalkLayer* pRet = new ActorTalkLayer();
	pRet->mStringBuff = stringinfo;
	pRet->mReadScreen = ptrRead;
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
bool ActorTalkLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_iLineHeight = 24;
	m_iStringWidth = 372;
	string_list_index = 0;
	mCurTalkElem = NULL;
	DSize size = ChessViewTool::getInstance()->getFullSize();
	GenerateTalkList(mStringBuff.c_str());

	Sprite* p_img = ReadSpriteName("new_ui/hyaline/440.png");
	p_img->setAnchorPoint(Vec2(1, 0));
	p_img->setPosition(Vec2(size.width / 2, 0));
	this->addChild(p_img);
	p_img->setVisible(false);

	Sprite* p_talk = ReadSpriteName("new_ui/battle/dialog_bg.png");
	p_talk->setAnchorPoint(Vec2(0.5, 0));
	p_talk->setPosition(Vec2(size.width / 2, 8));
	this->addChild(p_talk,1);
	p_talk->setVisible(false);

	Label* p_name = LabelSystemFont("", 18);
	p_name->setColor(Color3B::BLACK);
	p_name->setPosition(Vec2(161, 101));
	p_talk->addChild(p_name);

	int iPointX = (p_talk->getContentSize().width - m_iStringWidth) / 2;
	int iPointY = 77;
	Vec2 DrawNp[4];
	DrawNp[0] = Vec2(iPointX, iPointY);
	DrawNp[1] = Vec2(iPointX + m_iStringWidth, iPointY);
	DrawNp[2] = Vec2(iPointX + m_iStringWidth, iPointY - m_iLineHeight * 3);
	DrawNp[3] = Vec2(iPointX, iPointY - m_iLineHeight * 3);

	DrawNode* ptrDrawN = DrawNode::create();
	ptrDrawN->drawPolygon(DrawNp, 4, Color4F(0, 0, 0, 0), 1, Color4F(0, 0, 0, 0));

	ClippingNode* ptrClip = ClippingNode::create(ptrDrawN);
	ptrClip->setAnchorPoint(Vec2(0,0));
	ptrClip->setPosition(Vec2(0, 0));
	p_talk->addChild(ptrClip);

	Label* p_string = LabelSystemFont("", 18);
	p_string->setDimensions(m_iStringWidth, 0);
	p_string->setHorizontalAlignment(TextHAlignment::LEFT);
	p_string->setAnchorPoint(Vec2(0, 1));
	p_string->setPosition(DrawNp[0]);
	p_string->setLineHeight(m_iLineHeight);
	ptrClip->addChild(p_string);

	left_talk.setMIMG(p_img);
	left_talk.setMNAME(p_name);
	left_talk.setMSTRBG(p_talk);
	left_talk.setMTALKSTR(p_string);
	left_talk.setMACTORID(-1);
	left_talk.setMARMATURE(NULL);
	left_talk.setMTALKING(true);
	left_talk.setMPAGE(1);
	left_talk.setMCURPAGE(0);

	//右
	Sprite* p_imgr = ReadSpriteName("new_ui/hyaline/440.png");
	p_imgr->setAnchorPoint(Vec2(0, 0));
	p_imgr->setFlippedX(true);
	p_imgr->setPosition(Vec2(size.width / 2, 0));
	this->addChild(p_imgr);
	p_imgr->setVisible(false);

	Sprite* p_talkr = ReadSpriteName("new_ui/battle/dialog_bg.png");
	p_talkr->setAnchorPoint(Vec2(0.5, 0));
	p_talkr->setFlippedX(true);
	p_talkr->setPosition(Vec2(size.width / 2, 8));
	this->addChild(p_talkr,1);
	p_talkr->setVisible(false);

	Label* p_namer = LabelSystemFont("", 18);
	p_namer->setColor(Color3B::BLACK);
	p_namer->setPosition(Vec2(435, 101));
	p_talkr->addChild(p_namer);

	DrawNode* ptrDrawN1 = DrawNode::create();
	ptrDrawN1->drawPolygon(DrawNp, 4, Color4F(0, 0, 0, 0), 1, Color4F(0, 0, 0, 0));

	ClippingNode* ptrClip1 = ClippingNode::create(ptrDrawN1);
	ptrClip1->setAnchorPoint(Vec2(0, 0));
	ptrClip1->setPosition(Vec2(0, 0));
	p_talkr->addChild(ptrClip1);

	Label* p_stringr = LabelSystemFont("", 18);
	p_stringr->setDimensions(m_iStringWidth, 0);
	p_stringr->setHorizontalAlignment(TextHAlignment::LEFT);
	p_stringr->setAnchorPoint(Vec2(0, 1));
	p_stringr->setPosition(DrawNp[0]);
	p_stringr->setLineHeight(m_iLineHeight);
	ptrClip1->addChild(p_stringr);

	right_talk.setMIMG(p_imgr);
	right_talk.setMNAME(p_namer);
	right_talk.setMSTRBG(p_talkr);
	right_talk.setMTALKSTR(p_stringr);
	right_talk.setMACTORID(-1);
	right_talk.setMARMATURE(NULL);
	right_talk.setMTALKING(false);
	right_talk.setMPAGE(1);
	right_talk.setMCURPAGE(0);

	if (string_list.size() > 0)
	{
		int left_actor_index = -1;
		int right_actor_index = -1;
		for (size_t i = 0; i < string_list.size(); i++)
		{
			if (string_list[i]->getDIRECTION() == 1 && left_actor_index == -1)
				left_actor_index = i;
			if (string_list[i]->getDIRECTION() == 2 && right_actor_index == -1)
				right_actor_index = i;
			if (left_actor_index>-1 && right_actor_index>-1)
				break;
		}
		if (left_actor_index > -1)
		{
			int iActorId = string_list[left_actor_index]->getMACTORID();
			HistoryActorPtr ptrActor = &history->actors[iActorId];
			ActorAttrPtr dateActor = &trData->actors[iActorId];
			if (!iActorId&&ptrActor->mFashion[0] > 0)
			{
				if (left_talk.getMARMATURE() != NULL)
					left_talk.getMARMATURE()->removeFromParent();
				string file_path = "";
				if (ptrActor->mFashion[1] == 1)
					file_path = "r_male";
				else
					file_path = "r_female";
				Armature* hero = Armature::create(file_path.c_str());
				__String* arm_path = __String::createWithFormat("R%d_%d_2", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				hero->setPosition(Vec2(size.width - hero->getContentSize().width / 2, 0));
				hero->getAnimation()->play(arm_path->getCString());
				hero->setAnchorPoint(Vec2(0.5,0));
				Vec2 p_p = left_talk.getMIMG()->getPosition();
				hero->setPosition(Vec2(p_p.x-270,p_p.y));
				this->addChild(hero);
				left_talk.setMARMATURE(hero);
			}
			else
			{
				__String* img_path = __String::create("");
				img_path = __String::createWithFormat("img/Face/%d.png", ptrActor->face_id + 1);
				left_talk.getMIMG()->setTexture(ITools::getInstance()->getTextureWithPath(img_path->getCString()));
				left_talk.getMIMG()->setVisible(true);
			}
			left_talk.getMSTRBG()->setVisible(true);
			left_talk.getMNAME()->setString(dateActor->getActorName());
			if (string_list[left_actor_index]->talk_list.size() > 0)
			{
				left_talk.getMTALKSTR()->setString(string_list[left_actor_index]->talk_list[0].c_str());
				left_talk.setMPAGE(ceil(left_talk.getMTALKSTR()->getContentSize().height / (m_iLineHeight * 3)));
			}
			left_talk.setMACTORID(iActorId);
			string_list[left_actor_index]->talk_list_index++;
			mCurTalkElem = &left_talk;
		}

		if (right_actor_index > -1)
		{
			int iActorId = string_list[right_actor_index]->getMACTORID();
			HistoryActorPtr ptrActor = &history->actors[iActorId];
			ActorAttrPtr dateActor = &trData->actors[iActorId];
			if (!iActorId&&ptrActor->mFashion[0] > 0)
			{
				if (right_talk.getMARMATURE() != NULL)
					right_talk.getMARMATURE()->removeFromParent();
				string file_path = "";
				if (ptrActor->mFashion[1] == 1)
					file_path = "r_male";
				else
					file_path = "r_female";
				Armature* hero = Armature::create(file_path.c_str());
				__String* arm_path = __String::createWithFormat("R%d_%d_1", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				hero->setPosition(Vec2(size.width - hero->getContentSize().width / 2, 0));
				hero->getAnimation()->play(arm_path->getCString());
				hero->setAnchorPoint(Vec2(0.5, 0));
				Vec2 p_p = right_talk.getMIMG()->getPosition();
				hero->setPosition(Vec2(p_p.x + 270, p_p.y));
				this->addChild(hero);
				hero->setRotationSkewY(180.0f);
				right_talk.setMARMATURE(hero);
			}
			else
			{
				__String* img_path = __String::create("");
                img_path = __String::createWithFormat("img/Face/%d.png", ptrActor->face_id + 1);
				right_talk.getMIMG()->setTexture(ITools::getInstance()->getTextureWithPath(img_path->getCString()));
				right_talk.getMIMG()->setVisible(true);
			}

			right_talk.getMIMG()->setColor(Color3B::GRAY);
			right_talk.setMACTORID(iActorId);
			right_talk.getMNAME()->setString(dateActor->getActorName());
		}
	}
	return true;
}

void ActorTalkLayer::GenerateTalkList(const char *buf, int actor_id)
{
	const char* point = 0;
	const char *lastPoint = 0;
	int add = 1;
	ActorTalkInfo* redetalk = new ActorTalkInfo();
	point = lastPoint = buf;
	int talk_list_index = 0;
	while (point < buf + strlen(buf))
	{
		add = 1;

		point = buf + strlen(buf);

		redetalk->setMACTORID(actor_id);
		redetalk->setDIRECTION(1);
		redetalk->talk_list[talk_list_index++] = string(lastPoint, point - lastPoint);
		lastPoint = point + add;
	}

	string_list[string_list.size()] = redetalk;
}
void ActorTalkLayer::GenerateTalkList(const char *buf)
{
	int i;
	ActorTalkInfo* redetalk = NULL;
	const char* point = 0;
	const char *lastPoint = 0;
	const char* linePoint = 0;
	int add = 1;
	lastPoint = strstr(buf, "&");
	if (lastPoint == NULL)
	{
		GenerateTalkList(buf, 0);
		return;
	}
	point = lastPoint;
	int talk_info_index = 0;
	while (point < buf + strlen(buf))
	{
		if (*lastPoint == '&')
		{
			redetalk = new ActorTalkInfo();
			talk_info_index = 0;
			point = strstr(lastPoint, "\n");
			if (point == NULL)
			{
				point = strstr(lastPoint, "\\n");
				if (point == NULL)
				{
					point = buf + strlen(buf);
				}
				else
				{
					add = 2;
				}
			}
			
			linePoint = strstr(lastPoint, "|");
			if (linePoint == NULL)
			{
				linePoint = point;
			}

			bool find_actor = false;

			for (i = 0; i < trData->actor_num; i++)
			{
				ActorAttrPtr actor = (ActorAttrPtr)&trData->actors[i];

				if (!strncmp(actor->actor_name, lastPoint + 1, linePoint - lastPoint - 1) && strlen(actor->actor_name) == linePoint - lastPoint - 1)
				{
					redetalk->setMACTORID(actor->actor_id);
					find_actor = true;
					break;
				}
			}

			if (!find_actor)
			{
				redetalk->setMACTORID(0);
			}

			redetalk->setDIRECTION(StringToInt(linePoint+1));
			string_list[string_list.size()] = redetalk;
		}
		else
		{
			point = strstr(lastPoint, "\n");

			while (point&&*(point + add) != '&')
			{
				point = strstr(point + add, "\n");
			}

			if (point == NULL)
			{
				point = buf + strlen(buf);
			}
			if (redetalk != NULL)
			{
				redetalk->talk_list[talk_info_index++] = string(lastPoint, point - lastPoint);
			}
		}
		lastPoint = point + add;
	}
}

void ActorTalkLayer::nextTalk()
{
	ActorTalkInfo* ptrTalk = string_list[string_list_index];
	if (ptrTalk->talk_list_index < (int)ptrTalk->talk_list.size())
	{
		if (ptrTalk->getDIRECTION() == 1)
		{
			left_talk.getMTALKSTR()->setString(ptrTalk->talk_list[ptrTalk->talk_list_index++].c_str());
			SetMaxPage(&left_talk);
			mCurTalkElem = &left_talk;
		}
		else
		{
			right_talk.getMTALKSTR()->setString(ptrTalk->talk_list[ptrTalk->talk_list_index++].c_str());
			SetMaxPage(&right_talk);
			mCurTalkElem = &right_talk;
		}
	}
	else if (string_list_index + 1<(int)string_list.size())
	{
		string_list_index++;
		ActorTalkElem* ptrTalkElem = NULL;
		ActorTalkInfo* ptrTalkInfo = string_list[string_list_index];
		bool is_left = true;
		if (ptrTalkInfo->getDIRECTION() == 1)
		{
			ptrTalkElem = &left_talk;
		}
		else
		{
			is_left = false;
			ptrTalkElem = &right_talk;
		}

		if (ptrTalkElem->getMACTORID() == ptrTalkInfo->getMACTORID())
		{
			if (ptrTalkInfo->talk_list.size() > 0)
			{
				ptrTalkElem->getMTALKSTR()->setString(ptrTalkInfo->talk_list[0].c_str());
			}
			else
			{
				ptrTalkElem->getMTALKSTR()->setString("");
			}
			SetMaxPage(ptrTalkElem);
			ptrTalkInfo->talk_list_index++;
		}
		else
		{
			int iActorId = ptrTalkInfo->getMACTORID();
			HistoryActorPtr ptrActor = &history->actors[iActorId];
			ActorAttrPtr dateActor = &trData->actors[iActorId];
			if (!iActorId&&ptrActor->mFashion[0] > 0)
			{
				if (ptrTalkElem->getMARMATURE() != NULL)
					ptrTalkElem->getMARMATURE()->removeFromParent();
				string file_path = "";
				if (ptrActor->mFashion[1] == 1)
					file_path = "r_male";
				else
					file_path = "r_female";
				Armature* hero = Armature::create(file_path.c_str());
				__String* arm_path = __String::createWithFormat("R%d_%d_2", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				hero->getAnimation()->play(arm_path->getCString());
				hero->setAnchorPoint(Vec2(0.5, 0));
				Vec2 p_p = ptrTalkElem->getMIMG()->getPosition();
				hero->setPosition(Vec2(p_p.x + (is_left?-1:1)*270, p_p.y));
				this->addChild(hero);
				if (!is_left)
					hero->setRotationSkewY(180.0f);
				ptrTalkElem->setMARMATURE(hero);
			}
			else
			{
				if (ptrTalkElem->getMARMATURE() != NULL)
				{
					ptrTalkElem->getMARMATURE()->removeFromParent();
					ptrTalkElem->setMARMATURE(NULL);
				}
				__String* img_path = __String::create("");
                 img_path = __String::createWithFormat("img/Face/%d.png", ptrActor->face_id + 1);
				ptrTalkElem->getMIMG()->setTexture(ITools::getInstance()->getTextureWithPath(img_path->getCString()));
			}
			ptrTalkElem->getMNAME()->setString(dateActor->getActorName());
			if (ptrTalkInfo->talk_list.size() > 0)
			{
				ptrTalkElem->getMTALKSTR()->setString(ptrTalkInfo->talk_list[0].c_str());
			}
			else
			{
				ptrTalkElem->getMTALKSTR()->setString("");
			}
			SetMaxPage(ptrTalkElem);
			ptrTalkElem->setMACTORID(iActorId);
			ptrTalkInfo->talk_list_index++;
			ptrTalkElem->setMTALKING(true);
		}
		if (is_left)
		{
			mCurTalkElem = &left_talk;
			if (left_talk.getMARMATURE() != NULL)
			{
				left_talk.getMIMG()->setVisible(false);
				if (left_talk.getMTALKING() == false)
				{
					left_talk.getMARMATURE()->setColor(Color3B::WHITE);
					HistoryActorPtr ptrActor = &history->actors[left_talk.getMACTORID()];
					__String* play_index = __String::createWithFormat("R%d_%d_2", ptrActor->mFashion[1], ptrActor->mFashion[2]);
					left_talk.getMARMATURE()->getAnimation()->play(play_index->getCString());
				}
			}
			else
			{
				left_talk.getMIMG()->setColor(Color3B::WHITE);
				left_talk.getMIMG()->setVisible(true);
			}
			left_talk.getMSTRBG()->setVisible(true);
			left_talk.setMTALKING(true);

			if (right_talk.getMARMATURE() != NULL)
			{
				if (right_talk.getMTALKING() == true)
				{
					right_talk.getMARMATURE()->setColor(Color3B::GRAY);
					HistoryActorPtr ptrActor = &history->actors[right_talk.getMACTORID()];
					__String* play_index = __String::createWithFormat("R%d_%d_1", ptrActor->mFashion[1], ptrActor->mFashion[2]);
					right_talk.getMARMATURE()->getAnimation()->play(play_index->getCString());
				}
			}
			else if (right_talk.getMIMG()->isVisible())
			{
				right_talk.getMIMG()->setColor(Color3B::GRAY);
			}
			right_talk.getMSTRBG()->setVisible(false);
			right_talk.setMTALKING(false);
		}
		else
		{
			mCurTalkElem = &right_talk;
			if (right_talk.getMARMATURE() != NULL)
			{
				right_talk.getMIMG()->setVisible(false);
				if (right_talk.getMTALKING() == false)
				{
					right_talk.getMARMATURE()->setColor(Color3B::WHITE);
					HistoryActorPtr ptrActor = &history->actors[right_talk.getMACTORID()];
					__String* play_index = __String::createWithFormat("R%d_%d_2", ptrActor->mFashion[1], ptrActor->mFashion[2]);
					right_talk.getMARMATURE()->getAnimation()->play(play_index->getCString());
				}
			}
			else
			{
				right_talk.getMIMG()->setColor(Color3B::WHITE);
				right_talk.getMIMG()->setVisible(true);
			}
			right_talk.getMSTRBG()->setVisible(true);
			right_talk.setMTALKING(true);

			if (left_talk.getMARMATURE() != NULL)
			{
				if (left_talk.getMTALKING() == true)
				{
					left_talk.getMARMATURE()->setColor(Color3B::GRAY);
					HistoryActorPtr ptrActor = &history->actors[left_talk.getMACTORID()];
					__String* play_index = __String::createWithFormat("R%d_%d_1", ptrActor->mFashion[1], ptrActor->mFashion[2]);
					left_talk.getMARMATURE()->getAnimation()->play(play_index->getCString());
				}
			}
			else if (left_talk.getMIMG()->isVisible())
			{
				left_talk.getMIMG()->setColor(Color3B::GRAY);
			}
			left_talk.getMSTRBG()->setVisible(false);
			left_talk.setMTALKING(false);
		}
	}
	else
	{
		RemoveThis();
	}
}

void ActorTalkLayer::RemoveThis()
{
	mReadScreen->talk_layer = NULL;
    this->removeFromParent();
}

bool ActorTalkLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (mCurTalkElem)
	{
		if (mCurTalkElem->getMCURPAGE() < mCurTalkElem->getMPAGE()-1)
		{
			mCurTalkElem->setMCURPAGE(mCurTalkElem->getMCURPAGE() + 1);
			mCurTalkElem->getMTALKSTR()->setPositionY(mCurTalkElem->getMTALKSTR()->getPositionY() + m_iLineHeight * 3);
		}
		else
		{
			nextTalk();
		}
	}
	else
		nextTalk();
	return true;
}
void ActorTalkLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void ActorTalkLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void ActorTalkLayer::onExit()
{
	Layer::onExit();
}

void ActorTalkLayer::SetMaxPage(ActorTalkElem* ptrTalkElem)
{
	ptrTalkElem->setMPAGE(ceil(ptrTalkElem->getMTALKSTR()->getContentSize().height / (m_iLineHeight * 3)));
	ptrTalkElem->setMCURPAGE(0);
	ptrTalkElem->getMTALKSTR()->setPositionY(77);
}

ActorTalkLayer::ActorTalkLayer()
{

}
ActorTalkLayer::~ActorTalkLayer()
{
	for (size_t i = 0; i < string_list.size(); i++)
	{
		delete string_list[i];
	}
}

//////////ShowInfoLayer

ShowInfoLayer* ShowInfoLayer::create(string infoStr, EventDelegate* ptrDelegate, int iType)
{
	ShowInfoLayer* pRet = new ShowInfoLayer();
	if (pRet&&pRet->init(infoStr, ptrDelegate, iType))
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
bool ShowInfoLayer::init(string infoStr, EventDelegate* ptrDelegate, int iType)
{
	if (!Layer::init())
	{
		return false;
	}
	mDelegate = ptrDelegate;
	m_iType = iType;
	m_iTotalPage = 1;
	m_iCurPage = 1;
	m_iLineHeight = 26;
	m_bRemoveThis = false;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	CCLOG("%s",infoStr.c_str());

	Sprite* bg = ReadSpriteName("new_ui/img_global/r_bg2.png");
	bg->setAnchorPoint(Vec2(0.5,0));
	bg->setPosition(Vec2(size.width/2,0));
	this->addChild(bg);

	int iLabViewWidth = 630;
	int iLabViewHeight = 73;
	float fPointX = (bg->getContentSize().width - iLabViewWidth) / 2;

	Vec2 VPointArr[4];
	VPointArr[0] = Vec2(fPointX, 5);
	VPointArr[1] = Vec2(fPointX, 5 + iLabViewHeight);
	VPointArr[2] = Vec2(fPointX + iLabViewWidth, 5 + iLabViewHeight);
	VPointArr[3] = Vec2(fPointX + iLabViewWidth, 5);
	DrawNode* ptrDrawNode = DrawNode::create();
	ptrDrawNode->drawPolygon(VPointArr, 4, Color4F(0, 0, 0, 0), 1, Color4F(0, 0, 0, 255));

	ClippingNode* ptrClip = ClippingNode::create(ptrDrawNode);
	ptrClip->setAnchorPoint(Vec2(0,0));
	ptrClip->setPosition(0, 0);
	bg->addChild(ptrClip);

	m_labInfo = LabelSystemFont(infoStr.c_str(), 18);
	m_labInfo->setDimensions(630, 0);
	m_labInfo->setAnchorPoint(Vec2(0, 1));
	m_labInfo->setLineHeight(m_iLineHeight);
	m_labInfo->setPosition(Vec2(fPointX, 5 + iLabViewHeight));
	ptrClip->addChild(m_labInfo);

	m_iTotalPage = ceil(m_labInfo->getContentSize().height / (m_labInfo->getLineHeight()*3));
	return true;
}

bool ShowInfoLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_bRemoveThis = false;
	if (m_iCurPage < m_iTotalPage)
	{
		m_labInfo->setPositionY(m_labInfo->getPositionY()+3*m_iLineHeight);
		m_iCurPage++;
	}
	else
	{
		m_bRemoveThis = true;
	}
	return true;
}

void ShowInfoLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}
void ShowInfoLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (m_bRemoveThis)
	{
		if (mDelegate)
			mDelegate->ChildCloseLayer(0);
		this->removeFromParent();
	}
}

ShowInfoLayer::ShowInfoLayer()
{

}
ShowInfoLayer::~ShowInfoLayer()
{

}