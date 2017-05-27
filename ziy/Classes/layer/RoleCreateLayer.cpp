//
//  RoleCreateLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/14.
//
//

#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "RoleCreateLayer.h"
#include "CocoMd5.h"
#include "Net_HomePage.h"
#include "StatConfig.h"
#include "LogoLayer.h"

#define TEXT_MIN_NUM 2
#define TEXT_MAX_NUM 15
#define EDIT_MAX_NUM 8
#define GUIDE_INIT_LEVEL 94

RoleCreateLayer::RoleCreateLayer()
{

}

RoleCreateLayer::~RoleCreateLayer()
{

}

bool RoleCreateLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void RoleCreateLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void RoleCreateLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

RoleCreateLayer* RoleCreateLayer::create(EventDelegate* ptrDelegate)
{
	RoleCreateLayer* pRet = new RoleCreateLayer();
	if(pRet&&pRet->init(ptrDelegate))
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

bool RoleCreateLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_iSex = 1;

	initControl();
	updateSkill(0);
	updateRandName();
	return true;
}

void RoleCreateLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	float fScale = 0.90f;

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/r_female.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/r_male.ExportJson", 0, 1));

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/main/logo_bg.jpg");
	spLayerBG->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(spLayerBG);

	//右背景
	Sprite* spRightBG = ReadSpriteName("new_ui/main/xr_bg1.png");
	Vec2 vecRight = Vec2(size.width - spRightBG->getContentSize().width / 2, size.height / 2);
	Vec2 vecLeft = Vec2((size.width - spRightBG->getContentSize().width) / 2, size.height / 2);
	spRightBG->setPosition(vecRight);
	this->addChild(spRightBG);

	//角色形象
	m_mArmRole[0] = Armature::create("r_female");
	m_mArmRole[0]->getAnimation()->play("R2_1_1");
	m_mArmRole[0]->setScale(0.83f);
	m_mArmRole[0]->setPosition(vecLeft + Vec2(25, -120));
	this->addChild(m_mArmRole[0]);

	m_mArmRole[1] = Armature::create("r_male");
	m_mArmRole[1]->getAnimation()->play("R1_1_1");
	m_mArmRole[1]->setScale(0.83f);
	m_mArmRole[1]->setPosition(vecLeft + Vec2(25, -120));
	m_mArmRole[1]->setVisible(false);
	this->addChild(m_mArmRole[1]);

	//职业信息
	Sprite* spJob = ReadSpriteName("new_ui/main/xr_bg2.png");
	spJob->setPosition(vecLeft + Vec2(-144, 25));
	this->addChild(spJob);

	//名字背景
	auto spName = ReadSpriteName("new_ui/main/main_bg5.png");
	spName->setPosition(vecLeft + Vec2(30, -135));
	this->addChild(spName);

	//名字输入框
	auto spUserBG = ui::Scale9Sprite::create(getRespath("new_ui/hyaline/92x30.png", 0, 1));
	m_pEditBox = ui::EditBox::create(spUserBG->getContentSize(), spUserBG);
	m_pEditBox->setPosition(vecLeft + Vec2(35, -135));
	m_pEditBox->setFontSize(16);
	m_pEditBox->setFontColor(Color3B::WHITE);
	m_pEditBox->setMaxLength(EDIT_MAX_NUM);
	m_pEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_pEditBox->setDelegate(this);
	this->addChild(m_pEditBox);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//随机名字
	Sprite* spRandN = ReadSpriteName("new_ui/main/icon_dice.png");
	Sprite* spRandS = ReadSpriteName("new_ui/main/icon_dice.png");
	spRandS->setScale(fScale);
	MenuItemSprite* btnRand = MenuItemSprite::create(spRandN, spRandS, CC_CALLBACK_1(RoleCreateLayer::callBackRand, this));
	spRandS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRandS->setPosition(Vec2(spRandS->getContentSize().width / 2, spRandS->getContentSize().height / 2));
	btnRand->setPosition(vecLeft + Vec2(115, -135));
	m_pBtnMenu->addChild(btnRand);

	//右背景栏布局
	//性别男
	Sprite* spMaleN = ReadSpriteName("new_ui/main/icon_m2.png");
	Sprite* spMaleS = ReadSpriteName("new_ui/main/icon_m1.png");
	m_btnMale = MenuItemSprite::create(spMaleN, spMaleS, CC_CALLBACK_1(RoleCreateLayer::callBackMale, this));
	m_btnMale->setPosition(vecRight + Vec2(-25, 150));
	m_pBtnMenu->addChild(m_btnMale);

	//性别女
	Sprite* spFemaleN = ReadSpriteName("new_ui/main/icon_f2.png");
	Sprite* spFemaleS = ReadSpriteName("new_ui/main/icon_f1.png");
	m_btnFemale = MenuItemSprite::create(spFemaleN, spFemaleS, CC_CALLBACK_1(RoleCreateLayer::callBackFemale, this));
	m_btnFemale->setPosition(vecRight + Vec2(68, 150));
	m_btnFemale->selected();
	m_pBtnMenu->addChild(m_btnFemale);

	//技能
	for(int i = 0; i < trData->m_mNetJob[0]->skillNum; i++)
	{
		float iCurScale = 0.727f;
		int iSkillId = trData->m_mNetJob[0]->skillId[i];
		SkillAttrPtr ptrSkill = &trData->skills[iSkillId - 1];
		string strPath = StringUtils::format("img/Skill/%d.png", trData->skills[iSkillId - 1].image_id + 1);
		Sprite* spImgN = ReadSpriteNormal(strPath);
		Sprite* spImgS = ReadSpriteNormal(strPath);
		spImgN->setScale(iCurScale);
		spImgS->setScale(fScale * iCurScale);
		m_mbtnSkill[i] = MenuItemSprite::create(spImgN, spImgS, CC_CALLBACK_1(RoleCreateLayer::callBackSKill, this));
		m_mbtnSkill[i]->setTag(i);
		spImgS->setAnchorPoint(Vec2(0.5f, 0.5f));
		spImgS->setPosition(Vec2(spImgS->getContentSize().width / 2, spImgS->getContentSize().height / 2) * iCurScale);
		m_mbtnSkill[i]->setPosition(vecRight + Vec2(-45 + i * 60, 25));
		m_mSkillPos[i] = m_mbtnSkill[i]->getPosition() - Vec2(spImgS->getContentSize().width / 2, spImgS->getContentSize().height / 2) * (1 - iCurScale);
		m_pBtnMenu->addChild(m_mbtnSkill[i]);
	}

	//技能选中框
	m_spSkillBG = ReadSpriteName("new_ui/main/icon_skillbg1.png");
	m_spSkillBG->setPosition(vecRight + Vec2(-45, 25));
	this->addChild(m_spSkillBG);

	//技能名称
	m_labSkillName = LabelSystemFont("", 16);
	m_labSkillName->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_labSkillName->setPosition(vecRight + Vec2(-75, -15));
	m_labSkillName->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labSkillName);

	//技能介绍
	m_labSkillInfo = LabelSystemFont("", 16);
	m_labSkillInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_labSkillInfo->setPosition(vecRight + Vec2(-75, -37));
	m_labSkillInfo->setDimensions(185, 0);
	m_labSkillInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	m_labSkillInfo->setLineHeight(24);
	m_labSkillInfo->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labSkillInfo);

	//创建角色
	Sprite* spCreateN = ReadSpriteName("new_ui/img_global/global_ban9.png");
	Sprite* spCreateTextN = ReadSpriteName("new_ui/main/main_txt1.png");
	spCreateN->addChild(spCreateTextN);
	Sprite* spCreateS = ReadSpriteName("new_ui/img_global/global_ban10.png");
	Sprite* spCreateTextS = ReadSpriteName("new_ui/main/main_txt2.png");
	spCreateS->addChild(spCreateTextS);
	MenuItemSprite* btnCreate = MenuItemSprite::create(spCreateN, spCreateS, CC_CALLBACK_1(RoleCreateLayer::callBackCreate, this));
	btnCreate->setPosition(vecRight + Vec2(20, -156));
	spCreateTextN->setPosition(Vec2(spCreateN->getContentSize().width / 2, spCreateN->getContentSize().height / 2));
	spCreateTextS->setPosition(Vec2(spCreateS->getContentSize().width / 2, spCreateS->getContentSize().height / 2));
	m_pBtnMenu->addChild(btnCreate);
	callBackMale(this);
}

void RoleCreateLayer::callBackCreate(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	//检查昵称是否合法
	string strName = m_pEditBox->getText();
	if(strName.length() < TEXT_MIN_NUM)
	{
		ITools::getInstance()->createGlobalPrompt(LL("nichengcuowu"), false);
		return;
	}
	else if(strName.length() > TEXT_MAX_NUM)
	{
		ITools::getInstance()->createGlobalPrompt(LL("nichengguochang"), false);
		return;
	}
	else
	{
		DString dsName = strName.c_str();
		if(dsName->indexOf(' ') != -1)
		{
			ITools::getInstance()->createGlobalPrompt(LL("feifanicheng"), false);
			return;
		}

		for(UInt i = 0; i < trData->m_mNameForbid.size(); i++)
		{
			if(dsName->indexOf(trData->m_mNameForbid[i].c_str()) != -1)
			{
				ITools::getInstance()->createGlobalPrompt(LL("feifanicheng"), false);
				return;
			}
		}
	}

	string strToken = history->m_pLoginInfo->getGameToken();
	__String* strData = __String::createWithFormat("gameToken=%s&gender=%d", strToken.c_str(), m_iSex);
	CCLOG("%s", strData->getCString());
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"name\":\"%s\",\"gender\":%d,\"validateCode\":\"%s\"}", strToken.c_str(), m_pEditBox->getText(), m_iSex, validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("initInfo",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(RoleCreateLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead, "initInfo");

	ITools::getInstance()->addWaitLayer();
}

void RoleCreateLayer::callBackMale(Ref *sender)
{
	if(!m_btnMale->isSelected())
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_mArmRole[0]->setVisible(false);
		m_mArmRole[1]->setVisible(true);
	}
	m_btnMale->selected();
	m_btnFemale->unselected();
	m_iSex = 0;
}

void RoleCreateLayer::callBackFemale(Ref *sender)
{

	if (!m_btnFemale->isSelected())
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_mArmRole[0]->setVisible(true);
		m_mArmRole[1]->setVisible(false);
	}
	m_btnMale->unselected();
	m_btnFemale->selected();
	m_iSex = 1;
}

void RoleCreateLayer::callBackSKill(Ref *sender)
{
	int iCurIndex = ((MenuItemSprite*)sender)->getTag();
	playSound(SOUND_LEFT_MOUSE_CLICK);
	updateSkill(iCurIndex);
}

void RoleCreateLayer::updateSkill(int iIndex)
{
	for(int i = 0; i < (int)m_mbtnSkill.size(); i++)
	{
		m_mbtnSkill[i]->setEnabled(i != iIndex);
	}
	m_spSkillBG->setPosition(m_mSkillPos[iIndex]);
	int iSkillId = trData->m_mNetJob[0]->skillId[iIndex];
	m_labSkillName->setString(trData->skills[iSkillId - 1].skill_name);
	m_labSkillInfo->setString(trData->skills[iSkillId - 1].skill_info);
}

void RoleCreateLayer::callBackRand(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	updateRandName();
}

void RoleCreateLayer::updateRandName()
{
	int index1 = rand() % trData->m_mFirstName.size();
	int index2 = rand() % trData->m_mSecName.size();
	string strName = trData->m_mFirstName[index1] + trData->m_mSecName[index2];
	m_pEditBox->setText(strName.c_str());
}

void RoleCreateLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	string strTag = response->getHttpRequest()->getTag();
	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (strTag == "buyPlotScript") {

		}
		else if (strTag == "initInfo") {
			Json::Value result = value["result"];
			history->m_pPlayerInfo->loadJson(result["playerInfo"]);
			
			StatConfig::submitExtendData(LogoLayer::getDataForSDKAnalys("createRole").data());

			Net_HomePage::InitHistory();
			//ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create());
			if (modConst->m_bGuideSwitch)
			{
				history->m_bGuide[0] = true;
			}
			m_pDelegate->closeLayer(NULL);

			if (history->m_pPlayerInfo->m_mHero.size())
			{
				for (int i = GUIDE_INIT_LEVEL; i < (GUIDE_INIT_LEVEL+1); i++)
				{
					if (history->mMapDateInfo->isPassed(i)) {

					}
					else {
						ITools::getInstance()->startFight(i, BATTLE_SIXWAR, 0);
						return;
					}
				}

				ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create());
				this->removeFromParent();
			}


		}
	}
}


void RoleCreateLayer::editBoxReturn(ui::EditBox* editBox)
{

}
