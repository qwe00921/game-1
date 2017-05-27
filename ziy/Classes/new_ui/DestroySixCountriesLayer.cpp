#include "DestroySixCountriesLayer.h"
#include "Defines.h"
#include "Net_HomePage.h"
#include "NetLevelInfoLayer.h"
#include "BattleScreen.h"
#include "TreasureBoxLayer.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "GQRankLayer.h"
#include "ActSceneLayer.h"
#include "HeroicAmbitionLayer.h"

DestroySixCountriesLayer* DestroySixCountriesLayer::create(int iMapWarId, int iModel)
{
	DestroySixCountriesLayer* pRet = new DestroySixCountriesLayer();
	if (pRet&&pRet->init(iMapWarId,iModel))
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

bool DestroySixCountriesLayer::init(int iMapWarId,int iModel)
{
	if (!Layer::init())
	{
		return false;
	}

	ChessPlayMp3(MUSIC_BATTLE_SCENE);

	mSize = ChessViewTool::getInstance()->getFullSize();
	mVelocity = 15.0;
	enable_click = true;
	war_index = -1;
	rank_index = -1;
	total_star_num = 0;
	get_star_num = 0;
	mSignWarId = iMapWarId;

	mNdModel = iModel;
	if (iMapWarId > -1&& trData->m_mNetWarInfo.count(iMapWarId))
	{
		mNdModel = trData->m_mNetWarInfo[iMapWarId]->iModel;
	}

	start_scene = 1;
	total_scene = GetModelTotalMap(mNdModel, start_scene);
	scene_index = start_scene;
	mMapWarId = iMapWarId;
	mCurMapIndex = -1;
	setCurScene();

	//宝箱
	jxbx = ReadSpriteName("new_ui/battle/goodbox_bg.png");
	jxbx->setPosition(Vec2(96, 30));
	this->addChild(jxbx, 1);
	if (mNdModel == DISSICULT_MODEL&&!trData->net_map_chest.count(scene_index))
		jxbx->setVisible(false);
	else
		jxbx->setVisible(true);

	jx_num = LabelSystemFont("1/18", 16);
	jx_num->setPosition(Vec2(93, 21));
	jxbx->addChild(jx_num);

	setMap();
	AddMapElement();
	setMapOffset();

	//534,30
	Menu* mChange = Menu::create();

	Sprite* m_nomal = ReadSpriteName("new_ui/battle/flippage1.png");
	Sprite* m_select = ReadSpriteName("new_ui/battle/flippage1.png");
	Sprite* m_Disable = ReadSpriteName("new_ui/battle/flippage2.png");

	miS = MenuItemSprite::create(m_nomal, m_select, m_Disable, CC_CALLBACK_1(DestroySixCountriesLayer::clickMenuPre, this));
	mChange->addChild(miS);
	if (scene_index<=start_scene)
		miS->setEnabled(false);

	Sprite* m_nomal1 = ReadSpriteName("new_ui/battle/flippage1.png");
	m_nomal1->setFlippedX(true);
	Sprite* m_select1 = ReadSpriteName("new_ui/battle/flippage1.png");
	m_select1->setFlippedX(true);
	Sprite* m_Disable1 = ReadSpriteName("new_ui/battle/flippage2.png");
	m_Disable1->setFlippedX(true);

	miS1 = MenuItemSprite::create(m_nomal1, m_select1, m_Disable1, CC_CALLBACK_1(DestroySixCountriesLayer::clickMenuNext, this));
	miS1->setPosition(Vec2(141, 0));
	mChange->addChild(miS1);
	if (scene_index >= total_scene+ start_scene-1)
		miS1->setEnabled(false);

	mChange->setPosition(Vec2(mSize.width - 188, 30));
	this->addChild(mChange,1);

	Sprite* c_name_bg = ReadSpriteName("new_ui/battle/flippage_bg1.png");
	c_name_bg->setPosition(Vec2(mSize.width - 119, 30));
	this->addChild(c_name_bg, 1);


	c_name = LabelSystemFont(getMapName(), 16);
	c_name->setColor(Color3B::YELLOW);
	c_name->setPosition(Vec2(c_name_bg->getContentSize().width / 2, c_name_bg->getContentSize().height / 2));
	c_name_bg->addChild(c_name);

	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(DestroySixCountriesLayer::clickMenuBack, this));

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8), mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mMenu, 1);
	mMenu->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5,0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width / 2, mb_select->getContentSize().height/2));

	//英雄志
	Sprite* yxz_nomal = ReadSpriteName("new_ui/battle/battle_icon3.png");
	Sprite* yxz_select = ReadSpriteName("new_ui/battle/battle_icon3.png");
	yxz_select->setScale(0.9f);
	MenuItemSprite* yxz_item = MenuItemSprite::create(yxz_nomal, yxz_select, CC_CALLBACK_1(DestroySixCountriesLayer::clickYXZMenu, this));

	yxz_item->setAnchorPoint(Vec2(0.5,0.5));
	yxz_item->setPosition(Vec2(-80,0));
	mMenu->addChild(yxz_item);
	yxz_select->setAnchorPoint(Vec2(0.5, 0.5));
	yxz_select->setPosition(Vec2(yxz_select->getContentSize().width / 2, yxz_select->getContentSize().height / 2));

	//普通难度
	Sprite* ordinary_nomal = ReadSpriteName("new_ui/battle/battle_icon1.png");
	Sprite* ordinary_select = ReadSpriteName("new_ui/battle/battle_icon1.png");
	ordinary_select->setScale(0.9f);
	MenuItemSprite* ordinary_item = MenuItemSprite::create(ordinary_nomal, ordinary_select, CC_CALLBACK_1(DestroySixCountriesLayer::clickOrdinaryMenu, this));

	mOMenu = Menu::create();
	mOMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8) - 150, mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mOMenu, 1);
	mOMenu->addChild(ordinary_item);
	ordinary_select->setAnchorPoint(Vec2(0.5, 0.5));
	ordinary_select->setPosition(Vec2(ordinary_select->getContentSize().width / 2, ordinary_select->getContentSize().height / 2));

	//困难难度
	Sprite* difficulty_nomal = ReadSpriteName("new_ui/battle/battle_icon2.png");
	Sprite* difficulty_select = ReadSpriteName("new_ui/battle/battle_icon2.png");
	difficulty_select->setScale(0.9f);
	MenuItemSprite* difficulty_item = MenuItemSprite::create(difficulty_nomal, difficulty_select, CC_CALLBACK_1(DestroySixCountriesLayer::clickDifficultyMenu, this));

	mDMenu = Menu::create();
	mDMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8) - 150, mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mDMenu, 1);
	mDMenu->addChild(difficulty_item);
	difficulty_select->setAnchorPoint(Vec2(0.5, 0.5));
	difficulty_select->setPosition(Vec2(difficulty_select->getContentSize().width / 2, difficulty_select->getContentSize().height / 2));

	if (mNdModel == DISSICULT_MODEL)
	{
		mOMenu->setVisible(true);
		mDMenu->setVisible(false);
	}
	else
	{
		mOMenu->setVisible(false);
		mDMenu->setVisible(true);
	}

	map<int, MenuItemSprite*> btnActScene;
	for(UInt i = 0; i < history->m_mActScene.size(); i++)
	{
		Menu* mActScene = Menu::create();
		int iImgId = trData->m_mNetWarInfo[history->m_mActScene[i]]->iImgId;
		__String* strPath = __String::createWithFormat("img/heroz/icon_fbs%d.png", iImgId);
		Sprite* spActiveN = ReadSpriteNormal(strPath->getCString());
		Sprite* spActiveS = ReadSpriteNormal(strPath->getCString());
		spActiveS->setScale(0.9f);
		btnActScene[i] = MenuItemSprite::create(spActiveN, spActiveS, CC_CALLBACK_1(DestroySixCountriesLayer::clickMenuActScene, this));
		spActiveS->setAnchorPoint(Vec2(0.5f, 0.5f));
		spActiveS->setPosition(Vec2(spActiveS->getContentSize().width / 2, spActiveS->getContentSize().height / 2));
		btnActScene[i]->setTag(i);
		mActScene->addChild(btnActScene[i]);
		mActScene->setPosition(Vec2(mSize.width - (8 + btnActScene[i]->getContentSize().width / 2), mSize.height - (68 + btnActScene[i]->getContentSize().height / 2 + i * 60)));
		this->addChild(mActScene, 2);
	}

	if(mNdModel == DISSICULT_MODEL)
		infoTitleSp = ReadSpriteName("new_ui/battle/battle_til2.png");
	else
		infoTitleSp = ReadSpriteName("new_ui/battle/battle_til1.png");
	infoTitleSp->setPosition(Vec2(0, mSize.height - 10));
	infoTitleSp->setAnchorPoint(Vec2(0,1));
	this->addChild(infoTitleSp, 1);

	return true;
}

void DestroySixCountriesLayer::setMap()
{
	__String* map_path = __String::createWithFormat("img/map/map%d_1.png", trData->net_map_name[scene_index]->mapBgId);
	__String* mountain_path = __String::createWithFormat("img/map/map%d_2.png", trData->net_map_name[scene_index]->mapBgId);
	__String* cloud_path = __String::createWithFormat("img/map/map%d_99.jpg", trData->net_map_name[scene_index]->mapBgId);

	//地图X轴锚点和地图初始化坐标有关
	mMap = ITools::getInstance()->getSpriteWithPath(map_path->getCString());
	mMap->setPosition(Vec2(0, mSize.height / 2));
	mMap->setAnchorPoint(Vec2(0, 0.5));
	mMap->setCascadeOpacityEnabled(true);

	mMountain = ITools::getInstance()->getSpriteWithPath(mountain_path->getCString());
	mMountain->setAnchorPoint(Vec2(1, 1));
	mMountain->setCascadeOpacityEnabled(true);

	float mountain_width = mMountain->getContentSize().width;
	mMountain->setPosition(Vec2(mSize.width, mSize.height / 2 + mMap->getContentSize().height / 2));

	float mountain_real_width = mountain_width*mVelocity;
	float map_real_width = mMap->getContentSize().width;

	if (map_real_width > mountain_real_width || mountain_width < mSize.width)
	{
		float mou_num = ceil(map_real_width / mountain_real_width);
		if (mountain_width < mSize.width)
		{
			float mou_num1 = ceil(mSize.width / mountain_width);
			if (mou_num1>mou_num)
				mou_num = mou_num1;
		}

		for (int i = 0; i < mou_num - 1; i++)
		{
			Sprite*  ptrMou = ITools::getInstance()->getSpriteWithPath(mountain_path->getCString());
			ptrMou->setAnchorPoint(Vec2(1, 0));
			ptrMou->setPosition(Vec2(-mountain_width*i, 0));
			ptrMou->setCascadeOpacityEnabled(true);
			if (i % 2 == 0)
			{
				ptrMou->setFlippedX(true);
			}
			mMountain->addChild(ptrMou);
		}
	}

	mCloud = ITools::getInstance()->getSpriteWithPath(cloud_path->getCString());
	mCloud->setAnchorPoint(Vec2(0, 1));
	mCloud->setPosition(Vec2(0, mSize.height / 2 + mMap->getContentSize().height / 2));
	mCloud->setCascadeOpacityEnabled(true);

	float cloud_width = mCloud->getContentSize().width;
	float cloud_real_width = cloud_width*mVelocity;

	if (map_real_width > cloud_real_width || cloud_width < mSize.width)
	{
		float cloud_num = ceil(map_real_width / cloud_real_width);
		if (cloud_width < mSize.width)
		{
			float cloud_num1 = ceil(mSize.width / cloud_width);
			if (cloud_num1>cloud_num)
				cloud_num = cloud_num1;
		}
		for (int i = 0; i < cloud_num - 1; i++)
		{
			Sprite*  ptrCloud = ITools::getInstance()->getSpriteWithPath(cloud_path->getCString());
			ptrCloud->setAnchorPoint(Vec2(0, 0));
			ptrCloud->setPosition(Vec2(cloud_width*(i + 1), 0));
			ptrCloud->setCascadeOpacityEnabled(true);
			if (i % 2 == 0)
			{
				ptrCloud->setFlippedX(true);
			}
			mCloud->addChild(ptrCloud);
		}
	}

	this->addChild(mCloud);
	this->addChild(mMountain);
	this->addChild(mMap);

	map_width = mMap->getContentSize().width;
	map_height = mMap->getContentSize().height;
}

void DestroySixCountriesLayer::setCurScene()
{
	if (mMapWarId < 0)
	{
		if(history->mMapDateInfo->getCurWarId(mNdModel) > 0)
		{
			mMapWarId = history->mMapDateInfo->getCurWarId(mNdModel);
		}
		else
		{
			mMapWarId = history->mMapDateInfo->getNextWarId(mNdModel);
		}
	}

	mCurMapIndex = mMapWarId;
	scene_index = trData->m_mNetWarInfo[mMapWarId]->iMapIndex;
}

void DestroySixCountriesLayer::AddMapElement()
{
	int elem_num = 0;
	for(map<int, NetWarInfo*>::iterator it = trData->m_mNetWarInfo.begin(); it != trData->m_mNetWarInfo.end(); it++)
	{
		NetWarInfo* ptrWarInfo = it->second;
		if(ptrWarInfo->iMapIndex == scene_index)
		{
			CCLOG("warid:%d", ptrWarInfo->iWarId);
			int mCityX = ptrWarInfo->iPosX;
			int mCityY = ptrWarInfo->iPosY;
			__String* img_path = __String::createWithFormat("img/map/%d.png", ptrWarInfo->iImgId);
			Sprite* mCity = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
			int city_width = mCity->getContentSize().width;
			int city_height = mCity->getContentSize().height;
			mCity->setPosition(Vec2(mCityX + city_width / 2, map_height - mCityY - city_height/2));
			mCity->setCascadeOpacityEnabled(true);
			mMap->addChild(mCity);
			if(ptrWarInfo->iWarId > history->mMapDateInfo->getNextWarId(mNdModel))
				addSpriteGray(mCity);

			int i_point_x = mCityX + city_width / 2;
			int i_point_y =map_height - mCityY + 15;
			Sprite* mLine = ReadSpriteName("new_ui/battle/flippage_bg2.png");
			mLine->setPosition(Vec2(i_point_x, i_point_y));
			mLine->setCascadeOpacityEnabled(true);
			mMap->addChild(mLine);

			Label* mName = LabelSystemFont(ptrWarInfo->strName.c_str(), 16);
			mName->setPosition(Vec2(mLine->getContentSize().width / 2, mLine->getContentSize().height / 2));
			mName->setCascadeOpacityEnabled(true);
			mLine->addChild(mName);

			Sprite* mRankSp = ReadSpriteName("new_ui/img_global/icon_phb.png");
			mRankSp->setPosition(Vec2(i_point_x + mLine->getContentSize().width / 2, i_point_y));
			mMap->addChild(mRankSp);

			if (mSignWarId == ptrWarInfo->iWarId)
			{
				Armature* ptrArmature = Armature::create("icon_eff");
				ptrArmature->setPosition(Vec2(i_point_x, i_point_y+18));
				ptrArmature->getAnimation()->play("war_tx99");
				mMap->addChild(ptrArmature);
			}

			MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(ptrWarInfo->iWarId);
			int iStarNum = 0;
			if (ptrMapDate != NULL)
				iStarNum = ptrMapDate->getSTAR();

			if (iStarNum < 1)
				mRankSp->setVisible(false);

			i_point_y = map_height - mCityY - city_height;
			Sprite* mStar1 = ReadSpriteName("new_ui/battle/s1.png");
			i_point_y +=mStar1->getContentSize().height / 2 -5;
			mStar1->setPosition(Vec2(i_point_x, i_point_y));
			mStar1->setCascadeOpacityEnabled(true);
			mMap->addChild(mStar1);
			if (iStarNum<1)
				addSpriteGray(mStar1);

			Sprite* mStar2 = ReadSpriteName("new_ui/battle/s2.png");
			mStar2->setPosition(Vec2(i_point_x, i_point_y));
			mStar2->setCascadeOpacityEnabled(true);
			mMap->addChild(mStar2);
			if (iStarNum<2)
				addSpriteGray(mStar2);

			Sprite* mStar3 = ReadSpriteName("new_ui/battle/s3.png");
			mStar3->setPosition(Vec2(i_point_x, i_point_y));
			mStar3->setCascadeOpacityEnabled(true);
			mMap->addChild(mStar3);
			if (iStarNum<3)
				addSpriteGray(mStar3);

			mElem[elem_num] = new DestroySixCountriesInfo();
			mElem[elem_num]->setCITY(mCity);
			mElem[elem_num]->setLINE(mLine);
			mElem[elem_num]->setMNAME(mName);
			mElem[elem_num]->setSTAR1(mStar1);
			mElem[elem_num]->setSTAR2(mStar2);
			mElem[elem_num]->setSTAR3(mStar3);
			mElem[elem_num]->setINDEX(ptrWarInfo->iWarId);
			mElem[elem_num]->setRANK(mRankSp);
			elem_num++;
		}
	}
	setTotalStarNum();
}

void DestroySixCountriesLayer::setMapOffset()
{
	if (mCurMapIndex > -1)
	{
		for (size_t i = 0; i < mElem.size(); i++)
		{
			if (mElem[i]->getINDEX() == mCurMapIndex)
			{
				float iCityX = mElem[i]->getCITY()->getPositionX();
				float iMapX = mSize.width / 2 - iCityX;
				if (iMapX>0)//offset_x <= 0 && offset_x >= mSize.width - map_width
					iMapX = 0;
				else if (iMapX < mSize.width - map_width)
					iMapX = mSize.width - map_width;
				float iOffsetX = iMapX - mMap->getPositionX();

				mMap->setPositionX(iMapX);

				float iCOffsetX = mCloud->getPosition().x + iOffsetX / 15.0;
				mCloud->setPositionX(iCOffsetX);

				float iMOffsetX = mMountain->getPosition().x - iOffsetX / 15.0;
				mMountain->setPositionX(iMOffsetX);
				break;
			}
		}
	}
}

void DestroySixCountriesLayer::setTotalStarNum()
{
	total_star_num = 0;
	get_star_num = 0;
	for (size_t i = 0; i < mElem.size(); i++)
	{
		int iWarId = mElem[i]->getINDEX();
		MapDateElem* ptrMDI = history->mMapDateInfo->getMapDateElem(iWarId);
		if (ptrMDI != NULL)
			get_star_num += ptrMDI->getSTAR();
		total_star_num += 3;
	}
	jx_num->setString(__String::createWithFormat("%d/%d", get_star_num, total_star_num)->getCString());
}

void DestroySixCountriesLayer::updateShow()
{
	if (scene_index == start_scene)
	{
		miS->setEnabled(false);
	}
	else if (!miS->isEnabled())
	{
		miS->setEnabled(true);
	}
	if (scene_index == total_scene + start_scene - 1)
	{
		miS1->setEnabled(false);
	}
	else if (!miS1->isEnabled())
	{
		miS1->setEnabled(true);
	}
	c_name->setString(getMapName());
	mMap->removeFromParent();
	mMountain->removeFromParent();
	mCloud->removeFromParent();
	for (size_t i = 0; i < mElem.size(); i++)
	{
		delete mElem[i];
	}
	mElem.clear();
	setMap();
	AddMapElement();

	if (mNdModel == DISSICULT_MODEL&&!trData->net_map_chest.count(scene_index))
		jxbx->setVisible(false);
	else
		jxbx->setVisible(true);
}

void DestroySixCountriesLayer::updateMoneyShow()
{
	
}

int DestroySixCountriesLayer::GetModelTotalMap(int iModel,int& iStartSceen)
{
	int iRet = 0;
	int eStartSceen = -1;
	for (map<int, NetMapNamePtr>::iterator iIt = trData->net_map_name.begin(); iIt != trData->net_map_name.end(); iIt++)
	{
		if (iIt->second->mModel == iModel)
		{
			iRet++;

			if (eStartSceen == -1)
				eStartSceen = iIt->first;
			else if (iIt->first < eStartSceen)
				eStartSceen = iIt->first;
		}
	}

	if (eStartSceen > -1)
	{
		iStartSceen = eStartSceen;
	}

	return iRet;
}

void DestroySixCountriesLayer::updataModel(int iModel)
{
	mNdModel = iModel;
	start_scene = 1;
	total_scene = GetModelTotalMap(mNdModel, start_scene);
	scene_index = start_scene;
	mMapWarId = -1;
	mCurMapIndex = -1;
	setCurScene();


	if (scene_index == start_scene)
	{
		miS->setEnabled(false);
	}
	else if (!miS->isEnabled())
	{
		miS->setEnabled(true);
	}
	if (scene_index == total_scene + start_scene - 1)
	{
		miS1->setEnabled(false);
	}
	else if (!miS1->isEnabled())
	{
		miS1->setEnabled(true);
	}
	c_name->setString(getMapName());
	mMap->removeFromParent();
	mMountain->removeFromParent();
	mCloud->removeFromParent();
	for (size_t i = 0; i < mElem.size(); i++)
	{
		delete mElem[i];
	}
	mElem.clear();

	setMap();
	AddMapElement();
	setMapOffset();

	if (mNdModel == DISSICULT_MODEL&&!trData->net_map_chest.count(scene_index))
		jxbx->setVisible(false);
	else
		jxbx->setVisible(true);
}

void DestroySixCountriesLayer::clickMenuPre(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (scene_index > start_scene)
	{
		scene_index--;
		updateShow();
	}
}
void DestroySixCountriesLayer::clickMenuNext(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (scene_index < total_scene + start_scene - 1)
	{
		scene_index++;
		updateShow();
	}
}

void DestroySixCountriesLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
	this->removeFromParent();
}

void DestroySixCountriesLayer::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if(trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
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

void DestroySixCountriesLayer::clickYXZMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	int iOpenWarId = 0;
	for (map<int, NetWarInfo*>::iterator it = trData->m_mNetWarInfo.begin(); it != trData->m_mNetWarInfo.end(); it++)
	{
		if (it->second->iType == BATTLE_HEROIC)
		{
			iOpenWarId = it->second->iOpenWarId;
			break;
		}
	}

	if (trData->isSystemOpen(SystemOpenIDS::E_YOULIYINGXIONGZHI, true))
	{
		ChessApplication::getInstance()->AddFunctionLayer(HeroicAmbitionLayer::create());
		this->removeFromParent();
	}
	else
	{
		//ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("tgztxddgkq"), trData->m_mNetWarInfo[iOpenWarId]->strName.c_str())->getCString(), false);
	}
}
void DestroySixCountriesLayer::clickOrdinaryMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	mDMenu->setVisible(true);
	mOMenu->setVisible(false);
	infoTitleSp->setTexture(ReadSpriteTexture("new_ui/battle/battle_til1.png"));
	updataModel(NORMAL_MODEL);
}

void DestroySixCountriesLayer::clickDifficultyMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (!trData->isSystemOpen(SystemOpenIDS::E_YOULIJINGYING, true))
	{
		//ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("tgztxddgkq"), trData->m_mNetWarInfo[129]->strName.c_str())->getCString(), false);
		return;
	}

	mDMenu->setVisible(false);
	mOMenu->setVisible(true);
	infoTitleSp->setTexture(ReadSpriteTexture("new_ui/battle/battle_til2.png"));
	updataModel(DISSICULT_MODEL);
}

bool DestroySixCountriesLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	for (size_t i = 0; i < mElem.size(); i++)
	{
		Rect elemBox = mElem[i]->getCITY()->getBoundingBox();
		Rect elemRank = mElem[i]->getRANK()->getBoundingBox();
		if (enable_click&&Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(mElem[i]->getCITY()->convertToNodeSpace(pTouch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			war_index = i;
			enable_click = false;
			ScaleTo* ptrScale = ScaleTo::create(0.1f, 0.9f);
			mElem[war_index]->getCITY()->runAction(ptrScale);
		}
		else if (enable_click&&mElem[i]->getRANK()->isVisible() && Rect(0, 0, elemRank.size.width, elemRank.size.height).containsPoint(mElem[i]->getRANK()->convertToNodeSpace(pTouch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			rank_index = i;
			enable_click = false;
			ScaleTo* ptrScale = ScaleTo::create(0.1f, 0.9f);
			mElem[rank_index]->getRANK()->runAction(ptrScale);
		}
	}
	Rect jxbxBox = jxbx->getBoundingBox();
	if (jxbx->isVisible()&&enable_click&&Rect(0, 0, jxbxBox.size.width, jxbxBox.size.height).containsPoint(jxbx->convertToNodeSpace(pTouch->getLocation())))
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ChessApplication::getInstance()->AddFunctionLayer(TreasureBoxLayer::create(scene_index,this));
	}
	return true;
}
void DestroySixCountriesLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	float offset_x = mMap->getPosition().x + pTouch->getDelta().x;
	if (offset_x <= 0 && offset_x >= mSize.width - map_width)
	{
		if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") == NULL) {
			mMap->setPositionX(offset_x);

			offset_x = mCloud->getPosition().x + pTouch->getDelta().x / 15.0;
			mCloud->setPositionX(offset_x);

			offset_x = mMountain->getPosition().x - pTouch->getDelta().x / 15.0;
			mMountain->setPositionX(offset_x);
		}
	}
}
void DestroySixCountriesLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	
	if (((war_index > -1 || rank_index>-1) && abs((int)(pTouch->getStartLocation().x - pTouch->getLocation().x))<3) || Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)
	{
		Sprite* RunActionSp = NULL;
		CallFunc* ptrCall = NULL;
		if (war_index > -1)
		{
			ptrCall = CallFunc::create(CC_CALLBACK_0(DestroySixCountriesLayer::showGKinfo, this));
			RunActionSp = mElem[war_index]->getCITY();
		}
		else
		{
			ptrCall = CallFunc::create(CC_CALLBACK_0(DestroySixCountriesLayer::showRankList, this));
			RunActionSp = mElem[rank_index]->getRANK();
		}

		ScaleTo* ptrScale = ScaleTo::create(0.1f, 1.0f);
		Sequence* ptrSequence = Sequence::create(ptrScale, ptrCall, NULL);
		RunActionSp->runAction(ptrSequence);
	}
	else if (war_index > -1 || rank_index>-1)
	{
		ScaleTo* ptrScale = ScaleTo::create(0.1f, 1.0f);
		if (war_index > -1)
			mElem[war_index]->getCITY()->runAction(ptrScale);
		else
			mElem[rank_index]->getRANK()->runAction(ptrScale);

		war_index = -1;
		rank_index = -1;
		enable_click = true;
	}
}

void DestroySixCountriesLayer::showGKinfo()
{
	enable_click = true;
	if (war_index > -1)
	{
		if(mElem[war_index]->getINDEX() > history->mMapDateInfo->getNextWarId(mNdModel))
			ITools::getInstance()->createGlobalPrompt(LL("gqwkq"), false);
 		else
			ChessApplication::getInstance()->AddFunctionLayer(NetLevelInfoLayer::create(mElem[war_index]->getINDEX(), this));
	}
	war_index = -1;
}

void DestroySixCountriesLayer::showRankList()
{
	enable_click = true;
	if (rank_index > -1)
	{
		ChessApplication::getInstance()->AddFunctionLayer(GQRankLayer::create(mElem[rank_index]->getINDEX()));
	}
	rank_index = -1;
}

char* DestroySixCountriesLayer::getMapName()
{
	if (trData->net_map_name.count(scene_index))
		return trData->net_map_name[scene_index]->name;
	return "";
}

void DestroySixCountriesLayer::onExit()
{
	Layer::onExit();
}
DestroySixCountriesLayer::DestroySixCountriesLayer()
{

}

DestroySixCountriesLayer::~DestroySixCountriesLayer()
{

}

void DestroySixCountriesLayer::clickMenuActScene(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* item = (MenuItemSprite*)sender;
	ChessApplication::getInstance()->AddFunctionLayer(ActSceneLayer::create(history->m_mActScene[item->getTag()]));
	history->m_iActivityEntry = 2;
}
