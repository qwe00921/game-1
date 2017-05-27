//
//  TalkLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/07/19.
//
//

#include "Common.h"
#include "main.h"
#include "TalkLayer.h"
#include "BattleSprite.h"

#define MAX_PAGE_LINE 3
#define LINE_HEIGHT 24

bool ActorTalkCell::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_iDirection = 0;

	initControl();
	return true;
}

void ActorTalkCell::updateSprite(TalkLabel* ptrLabel, int iPage)
{
	if (iPage == 1)
	{
		DSizeF size = ChessViewTool::getInstance()->getFullSize();
		Vec2 point = Vec2(size.width / 2, size.height / 2);

		HistoryActor* ptrActor = &history->actors[ptrLabel->getActorId()];
		m_iDirection = ptrActor->direction < 2 ? 0 : 1;

		//背景
		m_spCellBG->setFlippedX(!m_iDirection);
		int iBGHeight = m_spCellBG->getContentSize().height;
		int iBGY = m_iDirection ? (size.height - 50 - iBGHeight / 2) : (10 + iBGHeight / 2);
		m_spCellBG->setPosition(Vec2(size.width / 2, iBGY));

		//武将名
		int iTitleX = m_iDirection ? 161 : 435;
		m_labTitle->setString(ptrLabel->getTitle());
		m_labTitle->setPosition(Vec2(iTitleX, 101));

		//头像
		string strPath;
        strPath = StringUtils::format("img/Face_s/%d.png", ptrActor->face_id + 1);
		if (!ptrActor->face_id && history->m_iBattleType == BATTLE_PVP)
		{
			strPath = StringUtils::format("img/Face_s/%d_%d_%d.png", ptrActor->face_id + 1, ptrActor->mFashion[1], ptrActor->mFashion[2]);
		}
		else if((!ptrActor->actor_id || ((history->m_iBattleType == BATTLE_LADDER || history->m_iBattleType == BATTLE_IMAGE) && history->mLadderEActor[ptrActor->actor_id] == 1)) && ptrActor->mFashion[0] > 0)
		{
             strPath = StringUtils::format("img/Face_s/%d_%d_%d.png", ptrActor->face_id + 1, ptrActor->mFashion[1], ptrActor->mFashion[2]);
        }
		m_spFace->setTexture(GetSpriteTexture(strPath.c_str()));
		m_spFace->setFlippedX(m_iDirection != 0);
		int iFaceX = m_iDirection ? 506 : 60;
		m_spFace->setPosition(Vec2(iFaceX, 77));

		//对话剪辑框
		int iClipX = m_iDirection ? 95 : 135;
		m_clipNode->setPosition(Vec2(iClipX, (86 - LINE_HEIGHT * MAX_PAGE_LINE) / 2));

		//对话内容
		m_labText->setString(ptrLabel->getContent());
		m_labText->setPosition(Vec2(0, LINE_HEIGHT * MAX_PAGE_LINE - 2));
		int iMaxLine = m_labText->getContentSize().height / LINE_HEIGHT;
		ptrLabel->setTotalPage((iMaxLine - 1) / MAX_PAGE_LINE + 1);

		history->CenterActor(ptrActor);
	}
	else
	{
		m_labText->setPosition(Vec2(0, LINE_HEIGHT * MAX_PAGE_LINE * iPage - 2));
	}

	this->setVisible(true);
}

void ActorTalkCell::initControl()
{
	//背景
	m_spCellBG = ReadSpriteName("new_ui/battle/dialog_bg.png");
	this->addChild(m_spCellBG);

	//武将名
	m_labTitle = LabelSystemFont("", 18);
	m_labTitle->setColor(Color3B::BLACK);
	m_spCellBG->addChild(m_labTitle);

	//头像
	m_spFace = ReadSpriteNormal("img/Face_s/1_1_1.png");
    m_spFace->setScale(0.64f);
	m_spCellBG->addChild(m_spFace);

	//对话剪辑框
	DrawNode* dnShap = DrawNode::create();
	Vec2 rect[4] = {Vec2(0, LINE_HEIGHT * MAX_PAGE_LINE - 2), Vec2(327, LINE_HEIGHT * MAX_PAGE_LINE - 2), Vec2(327, 0), Vec2(0, 0)};
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	m_clipNode = ClippingNode::create();
	m_clipNode->setStencil(dnShap);
	m_spCellBG->addChild(m_clipNode);

	//对话内容
	m_labText = LabelSystemFont("", 18);
	m_labText->setDimensions(327, 0);
	m_labText->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_labText->setHorizontalAlignment(TextHAlignment::LEFT);
	m_labText->setLineHeight(LINE_HEIGHT);
	m_clipNode->addChild(m_labText);
}

bool LabelCell::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	initControl();
	return true;
}

void LabelCell::updateSprite(TalkLabel* ptrLabel, int iPage)
{
	if (iPage == 1)
	{
		DSizeF size = ChessViewTool::getInstance()->getFullSize();
		Vec2 point = Vec2(size.width / 2, size.height / 2);

		//对话剪辑框
		m_clipNode->setPosition(point + Vec2(-160, -LINE_HEIGHT * MAX_PAGE_LINE / 2 + 1));

		//对话内容
		m_labText->setString(ptrLabel->getContent());
		m_labText->setPosition(Vec2(0, LINE_HEIGHT * MAX_PAGE_LINE - 1));
		int iMaxLine = m_labText->getContentSize().height / m_labText->getLineHeight();
		ptrLabel->setTotalPage((iMaxLine - 1) / MAX_PAGE_LINE + 1);
	}
	else
	{
		m_labText->setPosition(Vec2(0, LINE_HEIGHT * MAX_PAGE_LINE * iPage - 1));
	}

	this->setVisible(true);
}

void LabelCell::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//背景
	Sprite* spCellBG = ReadSpriteName("new_ui/battle/battle_sbg2_1.png");
	spCellBG->setPosition(point);
	this->addChild(spCellBG);

	//对话剪辑框
	DrawNode* dnShap = DrawNode::create();
	Vec2 rect[4] = {Vec2(0, LINE_HEIGHT * MAX_PAGE_LINE - 1), Vec2(320, LINE_HEIGHT * MAX_PAGE_LINE - 1), Vec2(320, 0), Vec2(0, 0)};
	dnShap->drawPolygon(rect, 4, Color4F(355, 255, 255, 255), 2, Color4F(255, 255, 255, 255));
	m_clipNode = ClippingNode::create();
	m_clipNode->setStencil(dnShap);
	this->addChild(m_clipNode);

	//对话内容
	m_labText = LabelSystemFont("", 16);
	m_labText->setColor(Color3B(102, 51, 0));
	m_labText->setDimensions(320, 0);
	m_labText->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_labText->setHorizontalAlignment(TextHAlignment::LEFT);
	m_labText->setLineHeight(LINE_HEIGHT);
	m_clipNode->addChild(m_labText);
}

TalkLayer::TalkLayer()
{

}

TalkLayer::~TalkLayer()
{
	clearTalkLabel();
}

bool TalkLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (isVisible())
	{
		m_bIsPress = true;
		m_bIsClick = true;
		this->schedule(schedule_selector(TalkLayer::updateNextLayer), 0.2f);
		this->unschedule(schedule_selector(TalkLayer::autoNextLayer));
		if ((history->m_iCurWarId == 97 && history->var[143] == 1))
		{
			this->schedule(schedule_selector(TalkLayer::autoNextLayer), 1.0f);
		}
		else
		{
			this->schedule(schedule_selector(TalkLayer::autoNextLayer), 8.0f);
		}
	}
	return isVisible();
}

void TalkLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void TalkLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsPress)
	{
		if (m_bIsClick)
		{
			showNextLabel();
		}
		this->unschedule(schedule_selector(TalkLayer::updateNextLayer));
		m_bIsPress = false;
		m_bIsClick = false;
	}
}

void TalkLayer::updateLayer(string strContent)
{
	m_strContent = strContent;
	initTalkLabel();
	
	if (BattleControler::auto_run || history->m_bAutoRun )
	{
		this->schedule(schedule_selector(TalkLayer::autoNextLayer), 1.0f);
	}
	else {
		this->schedule(schedule_selector(TalkLayer::autoNextLayer), 8.0f);
	}
	
	
	
}

void TalkLayer::updateLayer(string strContent, int iActorId)
{
	clearTalkLabel();

	ActorAttr* pActorAttr = &trData->actors[iActorId];
	string strTitle = pActorAttr->getActorName();
	TalkLabel* ptrLabel = new TalkLabel();
	ptrLabel->setActorId(iActorId);
	ptrLabel->setTotalPage(1);
	ptrLabel->setTitle(strTitle);
	ptrLabel->setContent(strContent);
	m_mTalkLabel[0] = ptrLabel;

	this->setVisible(true);
	showNextLabel();
	
	if (BattleControler::auto_run || history->m_bAutoRun )
	{
		this->schedule(schedule_selector(TalkLayer::autoNextLayer), 1.0f);
	}
}

bool TalkLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_bIsPress = false;
	m_bIsClick = false;

	initControl();
	return true;
}

void TalkLayer::initTalkLabel()
{
	clearTalkLabel();
	map<int, string> mstrContent = trData->getStringMap(m_strContent, '&');
	int iLabelIndex = 0;
	for(UInt i = 0; i < mstrContent.size(); i++)
	{
		DString strContent = mstrContent[i].c_str();
		CCLOG("%s", strContent->getChar());
		int iIndex = strContent->indexOf('\n');
		string strName = strContent->substring(0, iIndex)->getChar();
		int iEndIndex = strContent->length();
		if(strContent->charAt(iEndIndex - 1) == '\n') //去除段尾回车符
		{
			iEndIndex -= 1;
		}
		string strLabel = strContent->substring(iIndex + 1, iEndIndex)->getChar();
		if (strLabel != "")
		{
			int iActorId = -1;
			string strTitle = "";
			for(int i = 0; i < trData->actor_num; i++)
			{
				ActorAttr* pActorAttr = &trData->actors[i];
				string strActName = pActorAttr->actor_name;
				if(strName == strActName)
				{
					HistoryActorPtr ptrActor = history->FindActor(pActorAttr->actor_id);
					if(ptrActor)
					{
						iActorId = pActorAttr->actor_id;
						strTitle = pActorAttr->getActorName();
					}
					break;
				}
			}

			if(iActorId == -1)
			{
				clearTalkLabel();
				return;
			}

			TalkLabel* ptrLabel = new TalkLabel();
			ptrLabel->setActorId(iActorId);
			ptrLabel->setTotalPage(1);
			ptrLabel->setTitle(strTitle);
			ptrLabel->setContent(strLabel);
			m_mTalkLabel[iLabelIndex++] = ptrLabel;
		}
	}

	this->setVisible(true);
	showNextLabel();
}

void TalkLayer::initControl()
{
	m_ActorTalk = ActorTalkCell::create();
	this->addChild(m_ActorTalk);
	m_ActorTalk->setVisible(false);
}

void TalkLayer::updateNextLayer(float fInterval)
{
	m_bIsClick = false;
	showNextLabel();
}

void TalkLayer::autoNextLayer(float fInterval)
{
	showNextLabel();
	if (!isVisible())
	{
		this->unschedule(schedule_selector(TalkLayer::autoNextLayer));
	}
}

void TalkLayer::clearTalkLabel()
{
	m_iIndex = 0;
	m_iPage = 1;
	for(UInt i = 0; i < m_mTalkLabel.size(); i++)
	{
		SAFE_DELETE(m_mTalkLabel[i]);
	}
	m_mTalkLabel.clear();
}

void TalkLayer::showNextLabel()
{
	m_ActorTalk->setVisible(false);
	if(m_iIndex < (int)m_mTalkLabel.size())
	{
		TalkLabel* ptrLabel = m_mTalkLabel[m_iIndex];
		m_ActorTalk->updateSprite(ptrLabel, m_iPage);
		if(ptrLabel->getTotalPage() > m_iPage)
		{
			m_iPage++;
		}
		else
		{
			m_iIndex++;
			m_iPage = 1;
		}
	}
	else
	{
		if ((history->m_iCurWarId == 98 && history->var[143] == 1)||(history->m_iCurWarId == 96 && history->var[143] == 1))
		{
			history->var[143] = 0;
			history->updateGuide();
		}
		this->setVisible(false);
	}
}
