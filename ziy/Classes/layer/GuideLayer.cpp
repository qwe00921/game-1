//
//  GuideLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/07/28.
//
//


#include "Common.h"
#include "main.h"
#include "GuideLayer.h"
#include "BattleScreen.h"
#include "Data.h"
#include "GetNetDateTool.h"

GuideLayer* GuideLayer::m_pInstance[MAX_GUIDE] = {};


char * GuideLayer::m_path = "";
bool GuideLayer::reset_guide = false;


enum
{
	POS_CENTRE = 1,
	POS_BOTTOM_LEFT = 2,
	POS_BOTTOM_RIGHT = 3,
	POS_TOP_LEFT = 4,
	POS_TOP_RIGHT = 5,
	POS_BOTTOM_CENTRE = 6,
	POS_LEFT_CENTR,
	POS_RIGHT_CENTR,
	POS_TOP_CENTR
};

#define SCREEN_XX 710.00f
#define SCREEN_YY 400.00f

GuideRect::GuideRect(float fPosX, float fPosY, float fWidth, float fHeight, int iPosType, int iPass, int iSkipType /*= SKIP_NORMAL*/, float iTime,int PassOver)
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_iType = iPosType;
	m_iSkipType = iSkipType;
	m_pass = iPass;
	m_lock = false;
	m_time = iTime;
	m_passOver = PassOver;
}

Rect GuideRect::getRect()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();

	switch(m_iType)
	{
	case POS_CENTRE:
	{
		m_fPosX = m_fPosX - SCREEN_XX / 2 + size.width / 2;
		m_fPosY = m_fPosY - SCREEN_YY / 2 + size.height / 2;
	}
		break;
	case POS_BOTTOM_LEFT:
		break;
	case POS_BOTTOM_RIGHT:
	{
		m_fPosX = m_fPosX - SCREEN_XX + size.width;
	}
		break;
	case POS_TOP_LEFT:
	{
		m_fPosY = m_fPosY - SCREEN_YY + size.height;
	}
		break;
	case POS_TOP_RIGHT:
	{
		m_fPosX = m_fPosX - SCREEN_XX + size.width;
		m_fPosY = m_fPosY - SCREEN_YY + size.height;
	}
		break;
	case POS_BOTTOM_CENTRE:
	{
		m_fPosX = m_fPosX - SCREEN_XX / 2 + size.width / 2;
	}
		break;
	case POS_LEFT_CENTR:
	{
		m_fPosY = m_fPosY - SCREEN_YY/2 + size.height/2;
	}
	break;
	case POS_RIGHT_CENTR:
	{
		m_fPosX = m_fPosX - SCREEN_XX + size.width;
		m_fPosY = m_fPosY - SCREEN_YY / 2 + size.height / 2;
	}
	break;
	case POS_TOP_CENTR:
	{
		m_fPosX = m_fPosX - SCREEN_XX /2+ size.width/2;
		m_fPosY = m_fPosY - SCREEN_YY  + size.height ;
	}
	break;
	default:
		break;
	}

	return Rect(m_fPosX, m_fPosY, m_fWidth, m_fHeight);
}

GuideLayer::GuideLayer()
{

}

GuideLayer::~GuideLayer()
{
	for(UInt i = 0; i < m_mRcClick.size(); i++)
	{
		SAFE_DELETE(m_mRcClick[i]);
	}
	m_mRcClick.clear();
}

 GuideLayer* GuideLayer::create(int num) 
{ 
	GuideLayer *pRet = new(std::nothrow) GuideLayer();
	if (pRet && pRet->init(num))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

GuideLayer* GuideLayer::Factory(int num)
{
	if(m_pInstance[num] == NULL)
	{
		
		string _a = StringUtils::format("json/dic_guide%s.json", GuideLayer::IntToGuideString(num));
		m_path = (char*)_a.data();

		m_pInstance[num] = GuideLayer::create(num);
		
		Director::getInstance()->getRunningScene()->addChild(m_pInstance[num], 996,"GuideLayer");
	}
	return m_pInstance[num];
}



bool GuideLayer::init(int num)
{
	if(!Layer::init())
	{
		return false;
	}
	m_iGuideType = num;
	m_iCurIndex = 0;
	initControl();
	initClickArea();
	
	
	m_iMaxIndex = m_mRcClick.size();

	auto listener1 = EventListenerTouchOneByOne::create();//这个点击事件不会吞噬点击，点击会向下传递，通过listener2来将点击事件屏蔽
	listener1->setEnabled(true);
	listener1->setSwallowTouches(false);
	listener1->onTouchBegan = [this](Touch *touch, Event *event) {
		
		return true;
	};
	listener1->onTouchEnded = [this](Touch *touch, Event *event) {
		bool bClick = m_rcClick.containsPoint(convertToNodeSpace(touch->getLocation()));
		/*if (!(touch->getStartLocation() == touch->getLocation()))
		{
			return;
		}*/

		if (m_iSkipType == SKIP_NORMAL &&m_ilock == false)
		{
			if (m_iGuideType < EMIL_GUIDE)
			{
				m_ipassGuide++;
				if (m_ipassGuide >= 13)
				{
					string _Type = GuideLayer::IntToGuideString(m_iGuideType);
					history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);
					m_pInstance[m_iGuideType] = NULL;
					BattleScreenPtr ptrScreen = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
					if (ptrScreen)
					{
						ptrScreen->m_spLabel->updateSprite(LL("guideerro"), LABEL_LONG);

					}
					this->removeFromParent();
				}
			}
			else if (m_iGuideType >= EMIL_GUIDE)
			{
				m_ipassGuide++;
				if (m_ipassGuide >= 7)
				{
					string _Type = GuideLayer::IntToGuideString(m_iGuideType);
					auto netTool = GetNetDateTool::getInstance();
					history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);
					netTool->setGuideStatusReq(_Type.c_str(), 1);
					m_pInstance[m_iGuideType] = NULL;
					this->removeFromParent();
				}
			}
		}
		if (bClick)
		{
			if (m_iCurIndex < m_iMaxIndex)
			{



				if (m_iSkipType == SKIP_NORMAL)
				{
					m_ilock = false;
					m_ipassGuide = 0;
					waitNextGuide();
				}
				else if (m_iSkipType == SKIP_AUTO)
				{

					waitNextGuide();
					if (m_mRcClick[m_iCurIndex - 1]->m_time == 0)
					{
						scheduleOnce(schedule_selector(GuideLayer::skipNextGuide), 0.3f);
					}
					else
					{
						m_clipNode->setCascadeOpacityEnabled(true);
						m_clipNode->setOpacity(0);
						scheduleOnce(schedule_selector(GuideLayer::skipNextGuide), m_mRcClick[m_iCurIndex - 1]->m_time);
					}
				}
			}
			else
			{
				if (m_iGuideType <= FIRSET4_GUIDE&&m_iGuideType >= FIRSET_GUIDE)
				{
					string _Type = GuideLayer::IntToGuideString(m_iGuideType);
					auto netTool = GetNetDateTool::getInstance();
					history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);

				}
				else {
					string _Type = GuideLayer::IntToGuideString(m_iGuideType);
					auto netTool = GetNetDateTool::getInstance();
					history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);
					netTool->setGuideStatusReq(_Type.c_str(), 1);
				}


				m_pInstance[m_iGuideType] = NULL;

				if (m_iGuideType == EMIL_GUIDE)
				{
					//ITools::getInstance()->startFight(97, BATTLE_SIXWAR, 0,false);
				}

				BattleScreenPtr ptrScreen = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
				if (ptrScreen && m_iGuideType == FIRSET_GUIDE)
				{
					ptrScreen->m_spLabel->updateSprite(LL("guidenotice"), LABEL_LONG);
				}
				this->removeFromParent();

			}
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, this);

	auto listener2 = EventListenerTouchOneByOne::create();//这个点击事件用来屏蔽用户的点击
	listener2->setEnabled(true);
	listener2->setSwallowTouches(true);
	listener2->onTouchBegan = [this](Touch *touch, Event *event) {
		bool bClick = m_rcClick.containsPoint(convertToNodeSpace(touch->getLocation()));
		if (bClick)
		{
			return false;
		}
		return true;
	};
	listener2->onTouchEnded = [this](Touch *touch, Event *event)
	{
		if (!(touch->getStartLocation() == touch->getLocation()))
		{
			return;
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);

	this->schedule(schedule_selector(GuideLayer::updateRes),0.2);
	return true;
}
void GuideLayer::updateRes(float dt)//掉线时对引导进行处理
{
	if (reset_guide)
	{
		reset_guide = false;
		resetGuide();
	}
}

void GuideLayer::setreset_guide(Node* sender)
{
	reset_guide = true;
}

char *GuideLayer::IntToGuideString(int GuideType)
{
	char * _type = "1";

	if (GuideType >= EMIL_GUIDE&&GuideType <= ZHUJIEMIAN18_GUIDE)
	{
		
		string _a = StringUtils::format("_zhujiemian%d", GuideType- FIRSET5_GUIDE);
		_type = (char*)_a.data();

	}

	switch (GuideType) {
	case FIRSET_GUIDE:
		_type = "_fight1";
		break;
	case FIRSET2_GUIDE:
		_type = "_fight2";
		break;
	case FIRSET3_GUIDE:
		_type = "_fight3";
		break;
	case FIRSET4_GUIDE:
		_type = "_fight4";
		break;
	case FIRSET5_GUIDE:
		_type = "_fight5";
		break;
	case EMIL_GUIDE:
		_type = "_zhujiemian1";
		break;
	case STRENG_GUIDE:
		_type = "_zhujiemian2";
		break;
	case GOTOPubDrink_GUIDE:
		_type = "_zhujiemian3";
		break;
	case HECHENG_GUIDE:
		_type = "_zhujiemian4";
		break;
	case ZHUJIEMIAN5_GUIDE:
		_type = "_zhujiemian5";
		break;
	case ZHUJIEMIAN6_GUIDE:
		_type = "_zhujiemian6";
		break;


	case HELP2_GUIDE:
		_type = "_home";
		break;
	case HELP3_GUIDE:
		_type = "_home";
		break;
	case HELP4_GUIDE:
		_type = "_home";
		break;
	
	}
	CCAssert(!(_type == "1"), "请输入正确的文件路径");
	
	return _type;
}

bool GuideLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	bool bClick = m_rcClick.containsPoint(convertToNodeSpace(touch->getLocation()));
	if (bClick)
	{
		//onTouchEnded(touch, unused_event);
		return false;
	}
	return false;


	
}

void GuideLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void GuideLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	
}

void GuideLayer::updateGuide()
{
	if (m_mRcClick.empty())
	{
		CC_ASSERT(false);
		string _Type = GuideLayer::IntToGuideString(m_iGuideType);
		auto netTool = GetNetDateTool::getInstance();
		history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);
		netTool->setGuideStatusReq(_Type.c_str(), 1);
		m_pInstance[m_iGuideType] = NULL;
		this->removeFromParent();
		return;
	}
	CCAssert(m_mRcClick[m_iCurIndex] != NULL, "显示框元素为空，请确认是否有正常加载文件");
	m_clipNode->setOpacity(255);
	if(m_mRcClick[m_iCurIndex] != NULL)
	{
		if (m_mRcClick[m_iCurIndex]->m_lock == true )
		{
			for (int i = m_iCurIndex; i < m_mRcClick.size(); i++)
			{
				if (m_mRcClick[m_iCurIndex]->m_lock == true  )
				{
					m_iCurIndex++;
				}
				else {
					break;
				}
			}
		}
	}

		if (m_iCurIndex>= m_mRcClick.size())
		{
				string _Type = GuideLayer::IntToGuideString(m_iGuideType);
				auto netTool = GetNetDateTool::getInstance();
				history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);
				netTool->setGuideStatusReq(_Type.c_str(), 1);
			


			m_pInstance[m_iGuideType] = NULL;

			
			this->removeFromParent();
			return ;
		}

		if (m_mRcClick[m_iCurIndex ]->m_pass != 0)
		{

			string a1 = GuideLayer::IntToGuideString(m_iGuideType);
			string a2 = "_1";
			string _houzhui = a1 + a2;
			if (history->m_pPlayerInfo->getGuideStatus(_houzhui.c_str()) != m_mRcClick[m_iCurIndex ]->m_pass)
			{
				auto netTool = GetNetDateTool::getInstance();
				history->m_pPlayerInfo->setGuideStatus(_houzhui.c_str(), m_mRcClick[m_iCurIndex ]->m_pass);
				netTool->setGuideStatusReq(_houzhui.c_str(), m_mRcClick[m_iCurIndex ]->m_pass);
			}


		}
	m_ilock = true; 
	
	m_rcClick = m_mRcClick[m_iCurIndex]->getRect();
	m_iSkipType = m_mRcClick[m_iCurIndex++]->m_iSkipType;

	//模型
	m_spModel->setScaleX(m_rcClick.size.width / 2);
	m_spModel->setScaleY(m_rcClick.size.height / 2);
	m_spModel->setPosition(m_rcClick.origin + Vec2(m_rcClick.size.width / 2, m_rcClick.size.height / 2));
	m_clipNode->setStencil(m_spModel);

	//指示
	//Vec2 vecPoint = m_rcClick.origin + Vec2(m_rcClick.size.width, 0);
	//Size szPoint = m_armPoint->getContentSize(); 
	//if(vecPoint.x + szPoint.width * 2 / 3 > screen_size_x && vecPoint.y - szPoint.height * 2 / 3 < 0)
	//{
	//	vecPoint.x -= m_rcClick.size.width + szPoint.width;
	//	vecPoint.y += m_rcClick.size.height + szPoint.height;
	//	m_armPoint->setRotationSkewX(180.0f);
	//	m_armPoint->setRotationSkewY(180.0f);
	//}
	//else
	//{
	//	m_armPoint->setRotationSkewX(0.0f);
	//	m_armPoint->setRotationSkewY(0.0f);
	//}
	m_armPoint->setPosition(m_rcClick.origin + Vec2(m_rcClick.size.width*(3.0/4.0), m_rcClick.size.height*(1.0 / 4.0)));
	if (m_iSkipType== SKIP_FORBID)
	{
		m_armPoint->setPosition(m_rcClick.origin + Vec2(m_rcClick.size.width, 0));
	}
	
	m_armPoint->setVisible(true);
}

void GuideLayer::waitNextGuide()
{
	m_rcClick = Rect();

	//模型
	m_spModel->setScaleX(0);
	m_spModel->setScaleY(0);
	m_spModel->setPosition(Vec2::ZERO);
	m_clipNode->setStencil(m_spModel);
	
	if (m_iSkipType == SKIP_NORMAL && m_iGuideType<EMIL_GUIDE)
	{
		m_clipNode->setCascadeOpacityEnabled(true);
		m_clipNode->setOpacity(0);
	}
	

	//指示
	m_armPoint->setVisible(false);
}

void GuideLayer::GetGuideJson()
{

}

bool GuideLayer::CanUpdate()
{
	if (history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(0)) == 1);
	return false;
}

bool GuideLayer::isNULLGuide(int num)
{
	if (m_pInstance[num] == NULL)
	{
		return true;
	}
	return false;
}

void GuideLayer::resetGuide()
{
	m_iCurIndex= m_iCurIndex-1;

	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	float m_fPosX = 315;
	float m_fPosY = 122;
	m_fPosX = m_fPosX - SCREEN_XX / 2 + size.width / 2;
	m_fPosY = m_fPosY - SCREEN_YY / 2 + size.height / 2;
	m_rcClick = Rect(m_fPosX, m_fPosY, 81, 36);

	m_iSkipType = SKIP_AUTO;
	m_spModel->setScaleX(m_rcClick.size.width / 2);
	m_spModel->setScaleY(m_rcClick.size.height / 2);
	m_spModel->setPosition(m_rcClick.origin + Vec2(m_rcClick.size.width / 2, m_rcClick.size.height / 2));
	m_clipNode->setStencil(m_spModel);

	m_armPoint->setPosition(m_rcClick.origin + Vec2(m_rcClick.size.width, 0));
	m_armPoint->setVisible(true);
}

void GuideLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));

	//模型
	m_spModel = ReadSpriteName("new_ui/hyaline/2x2.png");

	//剪裁区域
	m_clipNode = ClippingNode::create();
	m_clipNode->setStencil(m_spModel); //设置裁剪模板
	m_clipNode->setInverted(true); //设置底板可见
	m_clipNode->setAlphaThreshold(0); //设置绘制底板的Alpha值为0

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 120));
	m_clipNode->addChild(layBlack); //添加底板
	this->addChild(m_clipNode);

	//指示
	m_armPoint = Armature::create("icon_eff");
	m_armPoint->getAnimation()->play("guide_1");
	this->addChild(m_armPoint);
}

#define JSON_READ_HEAD(A)char* sData = ChessReadFile(A);if (sData == NULL)return;Json::Reader reader;Json::Value line;if (!reader.parse(sData, line, false))return;int size = line.size()
#define JSON_READ_END ChessFree(sData)
#define FUZHIINT(A,B) A = B

void GuideLayer::initClickArea()
{

	//JSON_READ_HEAD("json/dic_guide.json");
	char* sData = ChessReadFile(m_path);
	if (sData == NULL)
	{
		CCAssert(true,"未找到文件,或文件加载错误");
		return;
	}
	Json::Reader reader;
	Json::Value line;
	if (!reader.parse(sData, line, false))return;
	int size = line.size();
	GuideInfoPtr ptrNMC = GuideInfoPtr();
	int iMapid = 0, iStar = 0;

	string a1 = GuideLayer::IntToGuideString(m_iGuideType);
	string a2 = "_1";
	string _houzhui = a1+a2;
	int _pass = history->m_pPlayerInfo->getGuideStatus(_houzhui.c_str());

	for (int i = 0; i < size; i++)
	{
		
		FUZHIINT(ptrNMC.x, line[i]["x"].asDouble());
		FUZHIINT(ptrNMC.y, line[i]["y"].asDouble());
		

		FUZHIINT(ptrNMC.wide, line[i]["wide"].asDouble());
		FUZHIINT(ptrNMC.high, line[i]["high"].asDouble());
		FUZHIINT(ptrNMC.Postype, line[i]["Postype"].asInt());
		FUZHIINT(ptrNMC.SkipType, line[i]["SkipType"].asInt());

		int __pass=0;
		float __time=0;
		int __passOver = 0;

		if (line[i]["Pass"].isInt())
		{
			__pass = line[i]["Pass"].asInt();
		}
		else {
			__pass = 0;
		}

		if (line[i]["Time"].isDouble()|| line[i]["Time"].isInt())
		{
			__time = line[i]["Time"].asDouble();
		}
		else {
			__time = 0;
		}

		if ( line[i]["PassOver"].isInt())
		{
			__passOver = line[i]["PassOver"].asInt();
		}
		else {
			__passOver = 0;
		}

		m_mRcClick[i] = new GuideRect(ptrNMC.x, ptrNMC.y, ptrNMC.wide, ptrNMC.high, ptrNMC.Postype, __pass, ptrNMC.SkipType, __time, __passOver); //邮件
	}

	JSON_READ_END;


	if (_pass==0)
	{
		return;
	}
	else if (_pass / 20 >0)
	{
		for (int i = 0; i<m_mRcClick.size(); i++)
		{
			if (m_mRcClick[i]->m_pass % 20 == 0 && m_mRcClick[i]->m_pass!=0 && m_mRcClick[i]->m_pass / 20== _pass/20)
			{
				m_iCurIndex = i;
				
			}
		}
	}

	
	if (_pass % 20 > 0 && _pass > 0)
	{
		int flagLock = 0;
		for (int i = m_iCurIndex; i<m_mRcClick.size(); i++)
		{
			if (m_mRcClick[i]->m_pass % 20 == 1)
			{
				for (int aa=i;  aa<m_mRcClick.size(); aa++)
				{
					if ( (_pass/20)== (m_mRcClick[aa]->m_pass/20))
					{
						if (m_mRcClick[aa]->m_pass != (_pass + 1)&& m_mRcClick[aa]->m_pass != _pass)
						{
							m_mRcClick[aa]->m_lock = true;
						}
						else if (m_mRcClick[aa]->m_pass == _pass)
						{
							if (flagLock == 0)
							{
								m_mRcClick[aa]->m_lock = true;
								flagLock++;
								
							}
							else if (flagLock == 1)
							{
								break;
							}
						}
						else
						{

							break;
						}

					
						
					}
					else
					{
						m_iCurIndex = aa - 1;
						break;
					}
				}
				
				break;
				
			}
		}
	}
	
	if (_pass != 0)
	{
		for (int i = 0; i < m_mRcClick.size(); i++)
		{
			if (m_mRcClick[i]->m_passOver <= _pass)
			{
				if (m_mRcClick[i]->m_passOver != 0)
				{
					
						m_mRcClick[i]->m_lock = true;
					
				}
			}
		}
	}
	

	//m_mRcClick[0] = new GuideRect(315, 38, 92, 76, POS_TOP_RIGHT); //邮件
	//m_mRcClick[1] = new GuideRect(217.5f, 262, 275, 82, POS_CENTRE);
	//m_mRcClick[2] = new GuideRect(300, 24, 110, 36, POS_CENTRE);
	//m_mRcClick[3] = new GuideRect(469, 349, 52, 52, POS_CENTRE);
	//m_mRcClick[4] = new GuideRect(638, 5, 64, 64, POS_BOTTOM_RIGHT); //菜单展开
	//m_mRcClick[5] = new GuideRect(569, 5, 62, 64, POS_BOTTOM_RIGHT); //武将
	//m_mRcClick[6] = new GuideRect(90, 207, 106, 106, POS_CENTRE);
	//m_mRcClick[7] = new GuideRect(365, 132, 66, 66, POS_CENTRE);
	//m_mRcClick[8] = new GuideRect(390, 32, 110, 36, POS_CENTRE); //装备
	//m_mRcClick[9] = new GuideRect(456, 132, 66, 66, POS_CENTRE);
	//m_mRcClick[10] = new GuideRect(390, 32, 110, 36, POS_CENTRE);
	//m_mRcClick[11] = new GuideRect(624, 338, 52, 52, POS_CENTRE);
	//m_mRcClick[12] = new GuideRect(624, 338, 52, 52, POS_CENTRE);
	//m_mRcClick[13] = new GuideRect(416, 82, 128, 120, POS_CENTRE); //演义堂
	//m_mRcClick[14] = new GuideRect(100, 198, 160, 148, POS_BOTTOM_CENTRE);
	//m_mRcClick[15] = new GuideRect(290, 41, 130, 36, POS_CENTRE);
	//m_mRcClick[16] = new GuideRect(269, 148, 172, 104, POS_CENTRE);
	//m_mRcClick[17] = new GuideRect(498, 52, 66, 66, POS_CENTRE);
	//m_mRcClick[18] = new GuideRect(539, 25, 80, 80, POS_CENTRE);
	//m_mRcClick[19] = new GuideRect(105, 50, 500, 300, POS_CENTRE, SKIP_FORBID);
	//m_mRcClick[20] = new GuideRect(144, 176, 48, 48, POS_TOP_LEFT, SKIP_AUTO);
	//m_mRcClick[21] = new GuideRect(432, 176, 48, 48, POS_TOP_LEFT, SKIP_AUTO);
	//m_mRcClick[22] = new GuideRect(480, 128, 48, 48, POS_TOP_LEFT);
	//m_mRcClick[23] = new GuideRect(380, 121, 80, 36, POS_CENTRE);
	//m_mRcClick[24] = new GuideRect(432, 176, 48, 48, POS_TOP_LEFT, SKIP_AUTO);
	//m_mRcClick[25] = new GuideRect(480, 224, 48, 48, POS_TOP_LEFT, SKIP_AUTO);
	//m_mRcClick[26] = new GuideRect(637, 216, 58, 58, POS_BOTTOM_RIGHT, SKIP_AUTO);
	//m_mRcClick[27] = new GuideRect(76, 221, 114, 112, POS_TOP_LEFT, SKIP_AUTO);
	//m_mRcClick[28] = new GuideRect(480, 176, 48, 48, POS_TOP_LEFT);
	//m_mRcClick[29] = new GuideRect(380, 121, 80, 36, POS_CENTRE);
}

void GuideLayer::skipNextGuide(float fDelay)
{
	updateGuide();
}
