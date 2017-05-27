#include "DispatchLayer.h"
#include "Defines.h"
#include "StatConfig.h"
#include "Screen.h"
#include "RedeScreen.h"
#include "WinConditionTool.h"
#include <fstream>
#include "BattleReplay.h"

DispatchLayer* DispatchLayer::create(EventDelegate* ptrDelegate)
{
	DispatchLayer* pRet = new DispatchLayer();
	pRet->mDelegate = ptrDelegate;
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

bool DispatchLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//////

	sort_type = 1;
	out_actor_num = 0;
	m_fRatioId = 0;
	memset(m_actor_id_list, 0, sizeof(int)*MAX_OWN_ARMY_NUM);
	SetActorList();

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/battle/battle_bg10.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	int iPointX = 59;
	int iPointY = 375;
	//人数
	Sprite* num_bg = ReadSpriteName("new_ui/battle/battle_bg5.png");
	num_bg->setAnchorPoint(Vec2(0, 1));
	num_bg->setPosition(Vec2(iPointX, iPointY-15));
	bg->addChild(num_bg);

	Label* num_txt = LabelSystemFont(LL("chuzhanrenshu"), 16);
	num_txt->setAnchorPoint(Vec2(0, 0.5));
	num_txt->setPosition(Vec2(19, num_bg->getContentSize().height / 2));
	num_bg->addChild(num_txt);

	num_date = LabelSystemFont("3/3", 16);
	num_date->setColor(Color3B::YELLOW);
	num_date->setAnchorPoint(Vec2(0, 0.5));
	num_date->setPosition(Vec2(19 + num_txt->getContentSize().width, num_bg->getContentSize().height / 2));
	num_bg->addChild(num_date);

	wait_tbv = DisPatchWaitTableView::create(actor_cell_num, this);
	bg->addChild(wait_tbv);

	//分割线
	Sprite* split_line = ReadSpriteName("new_ui/battle/battle_line1.png");
	split_line->setPosition(Vec2(bg->getContentSize().width / 2, 338-15));
	bg->addChild(split_line);

	Sprite* top_arrow = ReadSpriteName("new_ui/battle/battle_arrow1.png");
	top_arrow->setPosition(Vec2(bg->getContentSize().width / 2, 338-15));
	bg->addChild(top_arrow);

	Sprite* below_arrow = ReadSpriteName("new_ui/battle/battle_arrow1.png");
	below_arrow->setFlippedY(true);
	below_arrow->setPosition(Vec2(bg->getContentSize().width / 2, 110-3));
	bg->addChild(below_arrow);

	Sprite* split_line2 = ReadSpriteName("new_ui/battle/battle_line1.png");
	split_line2->setPosition(Vec2(bg->getContentSize().width / 2, 110-3));
	bg->addChild(split_line2);

	mMenuSort = Menu::create();
	mMenuSort->setPosition(Vec2(51, 91));
	bg->addChild(mMenuSort);
	Sprite* sort_nomal = ReadSpriteName("new_ui/battle/battle_ban1.png");
	Sprite* sort_select = ReadSpriteName("new_ui/battle/battle_ban1.png");
	spr_sort = MenuItemSprite::create(sort_nomal, sort_select, CC_CALLBACK_1(DispatchLayer::clickSortMenu, this));
	spr_sort->setAnchorPoint(Vec2(0, 1));
	mMenuSort->addChild(spr_sort);

	//战场预览
	mMenuPreB = Menu::create();
	mMenuPreB->setPosition(Vec2(428, 65));
	Sprite* pre_nomal = ReadSpriteName("new_ui/battle/battle_ck.png");
	Sprite* pre_select = ReadSpriteName("new_ui/battle/battle_ck.png");
	pre_select->setScale(0.9f);
	mMenuPreB->setVisible(false);

	MenuItemSprite* pre_mitem = MenuItemSprite::create(pre_nomal, pre_select, CC_CALLBACK_1(DispatchLayer::clickPreBattleMenu, this));
	pre_select->setAnchorPoint(Vec2(0.5f, 0.5f));
	pre_select->setPosition(Vec2(pre_nomal->getContentSize().width / 2, pre_nomal->getContentSize().height / 2));
	mMenuPreB->addChild(pre_mitem);
	bg->addChild(mMenuPreB);

	//出战
	mMenuDispatch = Menu::create();
	mMenuDispatch->setPosition(Vec2(539, 65));
	Sprite* dispatch_nomal = ReadSpriteName("new_ui/battle/battle_zd.png");
	Sprite* dispatch_select = ReadSpriteName("new_ui/battle/battle_zd.png");
	Sprite* dispatch_disable = ReadSpriteName("new_ui/battle/battle_zd.png");
	dispatch_select->setScale(0.9f);
	addSpriteGray(dispatch_disable);
	
	dispatch_mitem = MenuItemSprite::create(dispatch_nomal, dispatch_select, dispatch_disable, CC_CALLBACK_1(DispatchLayer::clickDispatchMenu, this));
	dispatch_select->setAnchorPoint(Vec2(0.5f, 0.5f));
	dispatch_select->setPosition(Vec2(dispatch_select->getContentSize().width / 2, dispatch_select->getContentSize().height / 2));
	mMenuDispatch->addChild(dispatch_mitem);
	bg->addChild(mMenuDispatch);


	//关闭
	mMenuClose = Menu::create();
	mMenuClose->setPosition(Vec2(587, 372));
	bg->addChild(mMenuClose);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(DispatchLayer::clickCloseLayer, this));
	mMenuClose->addChild(close_menu);

	//副将系统
	Sprite* sprBg = ReadSpriteName("new_ui/img_global/fj_bg1.png");
	sprBg->setPosition(Vec2(226,62));
	bg->addChild(sprBg);
	sprBg->setVisible(false);

	Sprite* sprBgElem = ReadSpriteName("new_ui/img_global/icon_ztx.png");
	sprBgElem->setPosition(Vec2(184,62));
	sprBg->addChild(sprBgElem);

	int irPointY = sprBg->getContentSize().height - 4;
	Label* labRatio = LabelSystemFont("1",16);
	labRatio->setAnchorPoint(Vec2(0,1));
	labRatio->setPosition(Vec2(10, irPointY));
	sprBg->addChild(labRatio);
	irPointY -= 2 + labRatio->getContentSize().height;

	Label* labRatioLevel = LabelSystemFont("2", 16);
	labRatioLevel->setAnchorPoint(Vec2(0, 1));
	labRatioLevel->setPosition(Vec2(10, irPointY));
	sprBg->addChild(labRatioLevel);
	irPointY -= 2 + labRatioLevel->getContentSize().height;

	Label* labRatioInfo = LabelSystemFont("3asdasdadasdasda da s da dad ad", 16);
	labRatioInfo->setAnchorPoint(Vec2(0, 1));
	labRatioInfo->setPosition(Vec2(10, irPointY));
	labRatioInfo->setDimensions(192, 35);
	sprBg->addChild(labRatioInfo);

	Menu* ptrRatioMenu = Menu::create();
	ptrRatioMenu->setPosition(Vec2(sprBg->getContentSize().width/2, sprBg->getContentSize().height/2));
	sprBg->addChild(ptrRatioMenu);
	Sprite* spMNomal = ReadSpriteName("new_ui/img_global/fj_bg1.png");
	Sprite* spMSelect = ReadSpriteName("new_ui/img_global/fj_bg1.png");
	spMNomal->setOpacity(0);
	spMSelect->setOpacity(0);
	auto ptrRatiomenuItem = MenuItemSprite::create(spMNomal, spMSelect, CC_CALLBACK_1(DispatchLayer::clickRatioMenu,this));
	ptrRatioMenu->addChild(ptrRatiomenuItem);

	mRatioActorElem.setfBg(sprBg);
	mRatioActorElem.setRatio(0);
	mRatioActorElem.setfScore(labRatio);
	mRatioActorElem.setfLevel(labRatioLevel);
	mRatioActorElem.setfInfo(labRatioInfo);

	setBattleState();

	setRatioData();
	initShow();

	if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_READY_TO_REPLAY)
	{
		ITools::getInstance()->addWaitLayer();
		scheduleOnce(schedule_selector(DispatchLayer::preDispatch), 0.2f);
	}
	return true;
}

bool DispatchLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void DispatchLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void DispatchLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	CCLOG("Layer touch Ended");
}

void DispatchLayer::clickPreBattleMenu(Ref* sender)
{

}

void DispatchLayer::clickDispatchMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if(history->m_iBattleType == BATTLE_SIXWAR || history->m_iBattleType == BATTLE_HEROIC || history->m_iBattleType == BATTLE_ROMANCE|| history->m_iBattleType == BATTLE_CLANARMYBATTLE)
	{
		dispatchFunction();
	}
	else
	{
		onHttpRequest();
	}
}

void DispatchLayer::GetFitsEffect()
{
	for (int i = 0; i < trData->actor_fits_num; i++)
	{
		int effect_num = 0;
		for (int j = 0; j < trData->actor_fits[i].hid_num; j++)
		{
			for (int k = 0; k < history->battleOwnNum; k++)
			{
				if (history->battleOwn[k]->actor_id == trData->actor_fits[i].hid[j] - 1)
				{
					effect_num++;
					break;
				}
			}
		}
		if (trData->actor_fits[i].hid_num == effect_num)
		{
			for (int j = 0; j < trData->actor_fits[i].hid_num; j++)
			{
				HistoryActorPtr ptrActor = &history->actors[trData->actor_fits[i].hid[j] - 1];
				int arr_len = ptrActor->actor_fit_id.size();
				DShortArray temp(arr_len + 1);
				if (arr_len>0)
				{
					temp.copyFrom(ptrActor->actor_fit_id.getAddress(), arr_len);
				}
				ptrActor->actor_fit_id.resize(arr_len + 1);
				temp[arr_len] = i;
				ptrActor->actor_fit_id.copyFrom(temp.getAddress(), arr_len + 1);
			}
		}
	}
}

void DispatchLayer::clickSortMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (sort_type < 4)
	{
		SortActorList(++sort_type);
		wait_tbv->UpdateTableView();
	}
	else
	{
		sort_type = 1;
		SortActorList(sort_type);
		wait_tbv->UpdateTableView();
	}
	__String* sort_icon = __String::createWithFormat("new_ui/battle/battle_ban%d.png", sort_type);
	spr_sort->setNormalImage(ReadSpriteName(sort_icon->getCString()));
	spr_sort->setSelectedImage(ReadSpriteName(sort_icon->getCString()));
}

void DispatchLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ScreenPtr pScreen = Chess::getInstance()->getScreenByID(SCREEN_REDE);
	if(pScreen)
	{
		pScreen->setLayerVisible(true);
	}
	this->removeFromParent();
}

void DispatchLayer::clickRatioMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(PicturePopLayer::create("new_ui/img_global/fj_bg2.png", true));
}

DispatchLayer::DispatchLayer()
{
	
}
DispatchLayer::~DispatchLayer()
{

}
void DispatchLayer::onExit()
{
	Layer::onExit();
}

void DispatchLayer::onEnter()
{
	Layer::onEnter();
}

void DispatchLayer::SetActorList()
{
	out_actor_num = 0;
	out_list.clear();
	in_list.clear();
	int iDataMinNum = 0;
	int actor_num = 0;
	int i;


	if (history->m_iBattleType == BATTLE_WOLFCITY)
	{
		map<int, int> eAllGuradList = history->mMapDateInfo->GetBattleWolfData()->getAllGuardActor();
		for (i = 0; i < history->ownArmyNum; i++)
		{
			if (!eAllGuradList.count(history->ownArmy[i] + 1))
				actor_list[actor_num++] = history->ownArmy[i];
		}
	}
	else
	{
		for (i = 0; i < history->ownArmyNum; i++)
		{
			actor_list[actor_num++] = history->ownArmy[i];
		}
	}

	int out_num = 0;
	if (trData->net_war_hero.count(history->m_iCurWarId))
	{
		iDataMinNum = trData->net_war_hero[history->m_iCurWarId]->iDispatchActorMinNum;

		//强制出场
		for (i = 0; i < actor_num; i++)
		{
			if (trData->net_war_hero[history->m_iCurWarId]->fix_hid.count(actor_list[i] + 1))
			{
				out_list[actor_list[i]] = 1;
				m_actor_id_list[out_actor_num++] = actor_list[i] + 1;
			}
		}

		//强制不出场
		map<int, int> mAvoidHid = trData->net_war_hero[history->m_iCurWarId]->avoid_hid;
		for (i = 0; i < actor_num; i++)
		{
			if (out_list.count(actor_list[i]))  continue;

			if ((!mAvoidHid.count(99999) && mAvoidHid.count(actor_list[i] + 1))||
				(mAvoidHid.count(99999) && !trData->net_war_hero[history->m_iCurWarId]->allow_hid.count(actor_list[i] + 1)))
			{
				in_list[actor_list[i]] = 1;
			}
		}

		//出场等级限制
		int iAllowLv = trData->net_war_hero[history->m_iCurWarId]->allow_lv;

		if (iAllowLv > 0)
		{
			for (i = 0; i < actor_num; i++)
			{
				if (out_list.count(actor_list[i]))  continue;

				if (history->actors[actor_list[i]].GetActorLv()<iAllowLv)
				{
					in_list[actor_list[i]] = 1;
				}
			}
		}

	}


// 	for (j = 0; j < MAX_OWN_ARMY_NUM; j++)
// 	{
// 		for (i = 0; i < actor_num; i++)
// 		{
// 			if (history->out_amy[j] == actor_list[i] && 0 != actor_list[i])//强制出场
// 			{
// 				out_list[actor_list[i]] = 1;
// 				m_actor_id_list[out_actor_num++] = actor_list[i]+1;
// 				break;
// 			}
// 		}
// 	}

	int imax_amy_num = trData->m_mNetWarInfo[history->m_iCurWarId]->iNumLimit;
	max_num =  imax_amy_num ;
	min_num = 1;

	if (iDataMinNum > 0)
		min_num = iDataMinNum;

	float total_num = actor_num;
	actor_cell_num = ceil(total_num / 5.0);
	SortActorList(sort_type);
}
void DispatchLayer::setBattleState()
{
	__String* ptrNumS = __String::createWithFormat("%d/%d", out_actor_num, max_num);
	num_date->setString(ptrNumS->getCString());

	if (out_actor_num >= 1)
		dispatch_mitem->setEnabled(true);
	else
		dispatch_mitem->setEnabled(false);
}
void DispatchLayer::SortActorList(int iType)
{
	sort_list.clear();
	int iActor_list[MAX_OWN_ARMY_NUM] = { 0 };
	int actor_num = actor_list.size();
	int iNum = 0;
	for (int i = 0; i < actor_num;i++)
	{
		iActor_list[i] = actor_list[i];
	}
	switch (iType)
	{
	case 1:
		{
			int iActorId1 = -1, iActorId2 = -1,iSortActorId = 0;

			int iStarIndex = 1;
			if (history->m_iBattleType == BATTLE_WOLFCITY)
				iStarIndex = 0;

			for (int i = iStarIndex; i < actor_num; i++)
			{
				for (int j = actor_num - 1; j > i; j--)
				{
					iActorId1 = iActor_list[j];
					iActorId2 = iActor_list[j - 1];
					if (history->actors[iActorId1].actor_quality > history->actors[iActorId2].actor_quality ||
						(history->actors[iActorId1].actor_quality == history->actors[iActorId2].actor_quality&&history->actors[iActorId1].GetActorLv() > history->actors[iActorId2].GetActorLv()) ||
						(history->actors[iActorId1].actor_quality == history->actors[iActorId2].actor_quality&&history->actors[iActorId1].GetActorLv() == history->actors[iActorId2].GetActorLv()&& iActorId2<iActorId1))
					{
						iSortActorId = iActor_list[j];
						iActor_list[j] = iActor_list[j - 1];
						iActor_list[j - 1] = iSortActorId;
					}
				}
			}
		}
		break;
	case 2:
	{
			  int num1, num2;
			  int i = 0, j, k, h, t;

			  for (h = actor_num - 1; h>0; h = k) /*循环到没有比较范围*/
			  {
				  for (j = 0, k = 0; j<h; j++) /*每次预置k=0，循环扫描后更新k*/
				  {
					  num1 = history->actors[iActor_list[j]].GetActorLv();
					  num2 = history->actors[iActor_list[j + 1]].GetActorLv();
					  if (num1< num2) /*大的放在后面，小的放到前面*/
					  {
						  t = iActor_list[j];
						  iActor_list[j] = iActor_list[j + 1];
						  iActor_list[j + 1] = t;
						  k = j;
					  }
				  }
			  }
	}
		break;
	case 3:
	{
			  int num1, num2;
			  int i = 0, j, k, h, t;

			  for (h = actor_num - 1; h > 0; h = k) /*循环到没有比较范围*/
			  {
				  for (j = 0, k = 0; j < h; j++) /*每次预置k=0，循环扫描后更新k*/
				  {
					  num1 = history->actors[iActor_list[j]].getRealAbility(ACTOR_ABILITY_POWER);
					  num2 = history->actors[iActor_list[j + 1]].getRealAbility(ACTOR_ABILITY_POWER);
					  if (num1 < num2) /*大的放在后面，小的放到前面*/
					  {
						  t = iActor_list[j];
						  iActor_list[j] = iActor_list[j + 1];
						  iActor_list[j + 1] = t;
						  k = j;
					  }
				  }
			  }
	}
		break;
	case 4:
	{
			  int num1, num2;
			  int i = 0, j, k, h, t;

			  for (h = actor_num - 1; h > 0; h = k) /*循环到没有比较范围*/
			  {
				  for (j = 0, k = 0; j < h; j++) /*每次预置k=0，循环扫描后更新k*/
				  {
					  num1 = history->actors[iActor_list[j]].getRealAbility(ACTOR_ABILITY_POWER);
					  num2 = history->actors[iActor_list[j + 1]].getRealAbility(ACTOR_ABILITY_POWER);
					  if (num1 < num2) /*大的放在后面，小的放到前面*/
					  {
						  t = iActor_list[j];
						  iActor_list[j] = iActor_list[j + 1];
						  iActor_list[j + 1] = t;
						  k = j;
					  }
				  }
			  }
	}	{
		int num1, num2;
		int i = 0, j, k, h, t;

		for (h = actor_num - 1; h > 0; h = k) /*循环到没有比较范围*/
		{
			for (j = 0, k = 0; j < h; j++) /*每次预置k=0，循环扫描后更新k*/
			{
				num1 = history->actors[iActor_list[j]].getRealAbility(ACTOR_ABILITY_DEFENCE);
				num2 = history->actors[iActor_list[j + 1]].getRealAbility(ACTOR_ABILITY_DEFENCE);
				if (num1 < num2) /*大的放在后面，小的放到前面*/
				{
					t = iActor_list[j];
					iActor_list[j] = iActor_list[j + 1];
					iActor_list[j + 1] = t;
					k = j;
				}
			}
		}
	}
		break;
	}
	for (int i = 0; i < actor_num; i++)
	{
		sort_list[i] = iActor_list[i];
	}
	SortDispatchActorLiat();
}

map<int, map<int, int> > DispatchLayer::GetElem(int iIndex)
{
	map<int, map<int, int> > idx;
	int actor_id = 0;
	int is_out = 0;
	int is_in = 0;
	int force_out = 0;
	int list_index = iIndex*5;

	for (int i = 0; i < 5; i++)
	{
		if (list_index + i < (int)sort_list.size())
		{
			actor_id = sort_list[list_index + i];
			if (out_list.find(actor_id) != out_list.end())
			{
				is_out = 1;
				force_out = out_list[actor_id];
			}
			else
			{
				is_out = 0;
				force_out = 0;
			}
			if (in_list.find(actor_id) != in_list.end())
			{
				is_in = 1;
			}
			else
			{
				is_in = 0;
			}
		}
		else
		{
			actor_id = -1;
			force_out = 0;
			is_out = 0;
			is_in = 0;
		}
		idx[i][0] = actor_id;
		idx[i][1] = force_out;
		idx[i][2] = is_out;
		idx[i][3] = is_in;
	}
	return idx;
}
void DispatchLayer::initShow()
{
	auto _writepath = FileUtils::getInstance()->getWritablePath() + "dispatch.json";
	char* sData=NULL;
	unsigned long readLen = 0;
	if (!funConst->EncryptResource)
	{

		
		ChessFile::directReadFile(_writepath.c_str(), &sData, &readLen);
	}
	else
	{
		
		!ChessFile::encryptReadFile(_writepath.c_str(), &sData, &readLen);
	}

	if (sData == NULL)
	{
		string _outchar = "[{}]";
		std::ofstream fout(_writepath);
		fout.close();
		//FileUtils::getInstance()->writeStringToFile("", _writepath);
		//ChessWriteFile(_writepath.c_str(), (char *)_outchar.c_str(), _outchar.size());
		//sData = (char*)_outchar.c_str();
		sData = (char*)ChessAlloc(_outchar.size());
		memcpy(sData, _outchar.c_str(), _outchar.size());
	}
	Json::Reader reader;
	Json::Value line;
	if (!reader.parse(sData, line, false))return;
	int size = line.size();
	int iMapid = 0, iStar = 0;

	/*if (history->m_iCurWarId>=100)
	{
		for (int i = 1; i < max_num; i++)
		{
			
			updateShow(i, 0);
		}
	}*/
	if (history->m_iCurWarId >= 100 )
	{
		if (size == 0 || history->m_iBattleType == BATTLE_WOLFCITY)//出战排序：当出战设置不是灭六国的时候，一律默认将最强的武将出战
		{
			for (int i = out_actor_num; i < max_num; i++)
			{

				updateShow(i, 0);
			}
		}
		else
		{
			if (history->m_iBattleType == BATTLE_SIXWAR)//当战场为灭六国的时候，记忆之前的出战数序出战
			{
				int lockNum = out_actor_num;
				for (auto &k : line[iMapid])
				{

					if (lockNum < max_num &&out_actor_num < max_num && history->m_pPlayerInfo->m_mHero.find(k.asInt()+1)!= history->m_pPlayerInfo->m_mHero.end())
					{
						int flag = 0;
						for (int i=0;i<out_actor_num;i++)
						{
							if (m_actor_id_list[i] == k.asInt() + 1)
								flag = 1;
						}
						if (flag == 0)
						{
							lockNum++;
							m_actor_id_list[out_actor_num++] = k.asInt() + 1;
							out_list[k.asInt()] = 0;
							SortActorList(sort_type);
							wait_tbv->UpdateTableView();
							setBattleState();
						}
					}
				}
				if (out_actor_num < max_num)
				{
					for (int i = out_actor_num; i < max_num; i++)
					{

						updateShow(i, 0);
					}
				}
			}
			else
			{
				for (int i = out_actor_num; i < max_num; i++)
				{

					updateShow(i, 0);
				}
			}
		}
	}
	ChessFree(sData);
}

void DispatchLayer::updateShow(int index, int type)
{
	int flag = true;
	if (index >= (int)sort_list.size())
		return;
	int actor_id = sort_list[index];
	int iActionId = -1;
	if (type > 0)
	{
		out_list.erase(actor_id);
		for (int i = 0; i < out_actor_num; i++)
		{
			if (m_actor_id_list[i] == actor_id+1)
			{
				for (int j = i; j < out_actor_num -1; j++)
				{
					m_actor_id_list[j] = m_actor_id_list[j + 1];
				}
				m_actor_id_list[out_actor_num - 1] = 0;
				out_actor_num--;
				playSound(SOUND_RIGHT_MOUSE_CLICK);
				break;
			}
		}
	}
	else
	{
		if (out_actor_num >= max_num)
			flag = false;
		else
		{
			playSound(SOUND_ACTOR_APPEAR);
			m_actor_id_list[out_actor_num++] = actor_id + 1;
			out_list[actor_id] = 0;
			iActionId = actor_id;
		}
	}
	if (flag)
	{
		SortActorList(sort_type);
		wait_tbv->UpdateTableView();
		setBattleState();
		setRatioData();
		//存储
		auto _writepath = FileUtils::getInstance()->getWritablePath() + "dispatch.json";
		Json::FastWriter fast_writer; Json::Value root; 
		int a = 0;
		char* sData=NULL ;
		unsigned long readLen = 0;
		if ( !funConst->EncryptResource)
		{
			
			
			ChessFile::directReadFile(_writepath.c_str(), &sData, &readLen);
		}
		else
		{
			
			!ChessFile::encryptReadFile(_writepath.c_str(), &sData, &readLen);
		}

		if (sData == NULL) 
		{
			string _outchar = "[{}]";
			std::ofstream fout(_writepath);
			fout.close();
			//FileUtils::getInstance()->writeStringToFile("", _writepath);
			//ChessWriteFile(_writepath.c_str(), (char *)_outchar.c_str(), _outchar.size());
			sData = (char*)ChessAlloc(_outchar.size());
			memcpy(sData, _outchar.c_str(), _outchar.size());
		}
		Json::Reader reader;
		Json::Value line;
		if (!reader.parse(sData, line, false))
		{
			string _outchar = "[{}]";

			//ChessWriteFile(_writepath.c_str(), (char *)_outchar.c_str(), _outchar.size());
		}
		

		Json::Value::Members arrayMember = line[a].getMemberNames();
		
		int size = arrayMember.size();
		string _num= StringUtils::format("o%d", size+ rand());
		
		
		
		
		if (type==0)
		{
			int __flag=0;
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter)
			{
				string member_name = *iter;
				if (member_name == _num)
				{
					__flag = 1;
				}
			}
			if (__flag == 0)
			{
				line[a][_num] = actor_id;
			}
			else {
				string _num2 = StringUtils::format("o%d", size + rand());
				line[a][_num2] = actor_id;
			}

		}else if (type > 0)
		{
			
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter)
			{
				string member_name = *iter;
				if (line[a][member_name].asInt()== actor_id)
				{
					
					line[a].removeMember(member_name) ;
				}
			}
		}
		
		
		
		std::string out = fast_writer.write(line); 
		if (funConst->EncryptResource)
		{
			 ChessFile::encryptWriteFile(_writepath.c_str(), (char*)out.c_str(), out.size());
		}
		else
		{
			ChessFile::directWriteFile(_writepath.c_str(), (char*)out.c_str(), out.size());
		}
		if (*sData != '\0' && sData != NULL)
		{
			ChessFree(sData);
		}
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("czrsym"), false);
	}
}

void DispatchLayer::updateMoneyShow()
{
	if (mDelegate)
		mDelegate->updateMoneyShow();
}

void DispatchLayer::RemoveAllParentLayer()
{
	this->removeFromParent();
}

int DispatchLayer::GetElemInt(int iIndex)
{
	return m_actor_id_list[iIndex] -1;
}

void DispatchLayer::SortDispatchActorLiat()
{
	int iSortmax = sort_list.size(), iSortOutIndex = 0, iSortCIndex = 0, iSortNIndex =0;
	map<int, int> mapSortListOut;
	map<int, int> mapCanDispatch;
	map<int, int> mapNotDispatch;

	for (int i = 0; i < iSortmax; i++)
	{
		if (out_list.count(sort_list[i]))
		{
			mapSortListOut[iSortOutIndex++] = sort_list[i];
		}
		else if (!in_list.count(sort_list[i]))
		{
			mapCanDispatch[iSortCIndex++] = sort_list[i];
		}
		else
		{
			mapNotDispatch[iSortNIndex++] = sort_list[i];
		}
	}

	for (int i = 0; i < iSortCIndex; i++)
	{
		mapSortListOut[iSortOutIndex++] = mapCanDispatch[i];
	}
	for (int i = 0; i < iSortNIndex; i++)
	{
		mapSortListOut[iSortOutIndex++] = mapNotDispatch[i];
	}

	sort_list.clear();
	sort_list = mapSortListOut;
}

void DispatchLayer::dispatchFunction()
{
	int iReplayStatus = BattleReplay::getInstance()->getCurStatus();
	if (out_actor_num < min_num && iReplayStatus != REPLAY_STATUS_READY_TO_REPLAY)
	{
		ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("zsczdwj"), min_num)->getCString(), false);
		return;
	}

	RedeScreenPtr ptrScreen = (RedeScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_REDE);
	if(ptrScreen != NULL)
		ptrScreen->rede_state = REDE_STATE_START_BATTLE;

	history->battleOwnNum = 0;
	map<int, int> mActorId;
	int iIndex = 0;
	for(int i = 0; i < out_actor_num; i++)
	{
		if (m_actor_id_list[i] > 0)
		{
			mActorId[iIndex++] = m_actor_id_list[i] - 1;
		}
	}

	//if (iReplayStatus == REPLAY_STATUS_NONE)
	//{
	//	BattleReplay::getInstance()->setScriptIndex(history->m_iCurWarId);
	//	BattleReplay::getInstance()->setCurStatus(REPLAY_STATUS_READY_TO_SAVE);
	//	BattleReplay::getInstance()->saveBattleActor(mActorId);
	//}
	//else if (iReplayStatus == REPLAY_STATUS_READY_TO_REPLAY)
	//{
	//	BattleReplay::getInstance()->setScriptIndex(history->m_iCurWarId);
	//	BattleReplay::getInstance()->startReplay();
	//	mActorId = BattleReplay::getInstance()->getBattleActor();
	//}

	for (UInt i = 0; i < mActorId.size(); i++)
	{
		history->actors[mActorId[i]].battleTimes++;
		history->battleOwn[history->battleOwnNum++] = &history->actors[mActorId[i]];
	}

	if (m_fRatioId > 0)
	{
		for (int i = 0; i < history->battleOwnNum; i++)
		{
			history->setRatioList(history->battleOwn[i]->actor_id + 1, m_fRatioId);
			history->battleOwn[i]->UpdataGoodEffect();
		}
	}
	history->CalcBaseLevel();
	GetFitsEffect();
	WinConditionTool::getInstance()->InitBattleCondition();
	history->mWeather.clear();
	history->mRealWeather.clear();
	history->mLadderEActor.clear();
	history->mNet72EquipActor.clear();
	history->mEnemyElitePlus.clear();
	history->mNet72EquipLevel.clear();
	history->mMapDateInfo->GetBattleImageData()->ClearActorList();

	if (history->m_iBattleType == BATTLE_CLANARMYBATTLE)
		history->mArmyFuBenBattle->ClearBossGainHp();

	memset(history->var, 0, MAX_CHAR_VAR_NUM * sizeof(history->var[0]));
	if(history->m_mScriptCode.count(history->m_iCurWarId))
	{
		for(map<int, int>::iterator mIt = history->m_mScriptCode[history->m_iCurWarId].begin(); mIt != history->m_mScriptCode[history->m_iCurWarId].end(); mIt++)
		{
			history->var[mIt->first] = mIt->second;
		}
	}
	if(history->m_mScriptCode.count(GLOBAL_VAR_MAPID))
	{
		for(map<int, int>::iterator mIt = history->m_mScriptCode[GLOBAL_VAR_MAPID].begin(); mIt != history->m_mScriptCode[GLOBAL_VAR_MAPID].end(); mIt++)
		{
			history->var[mIt->first] = mIt->second;
		}
	}

	if(history->m_iBattleType == BATTLE_SIXWAR)
	{
		history->mMapDateInfo->setCurWarId(history->m_iCurWarId);
	}
	if(mDelegate != NULL)
		mDelegate->ChildCloseLayer(0);
	else
		this->removeFromParent();
	history->updateGuide();
}

void DispatchLayer::onHttpRequest()
{
	this->retain();
	string strToken = history->m_pLoginInfo->getGameToken();
	string strData = StringUtils::format("gameToken=%s&warID=%d", strToken.c_str(), history->m_iCurWarId);
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData);
	string data = StringUtils::format("{\"gameToken\":\"%s\",\"warID\":%d,\"validateCode\":\"%s\"}", strToken.c_str(), history->m_iCurWarId, validateCode.c_str());
	CCLOG("%s", data.c_str());

	ITools::getInstance()->connectingNetwork("isOpenWar",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(DispatchLayer::onHttpRequestCompleted),
		data,
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void DispatchLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string reseData(buffer->begin(), buffer->end());
	CCLOG("%s...", reseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(reseData, value) && value.size() > 0)
	{
		if(value["result"].isObject() && value["result"]["isOpen"].isBool() && value["result"]["isOpen"].asBool())
		{
			dispatchFunction();
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("dailynotopen"), false);
		}
	}
	this->release();
}

void DispatchLayer::setRatioData()
{
	int iScore = 0, iOutActorNum = 0;
	map<int, int> mmOutActorList;
	for (int i = 0; i < out_actor_num; i++)
	{
		if (m_actor_id_list[i] <= 0)
			continue;
		mmOutActorList[m_actor_id_list[i] - 1] = 1;
	}


	for (int i = 0; i < history->ownArmyNum; i++)
	{
		if (!mmOutActorList.count(history->ownArmy[i]))
		{
			iScore += history->actors[history->ownArmy[i]].actor_quality;
		}
	}

	int iPre = round(iScore / ((history->getRecruitTotle() - (int)mmOutActorList.size()) * 5.0f) * (1.0f*history->ownArmyNum / history->getRecruitTotle()) * 100);
	if (iPre > 100) iPre = 100;

	float fRatioId = 0;
	for (map<int, NetRatioBuff*>::iterator eIt = trData->net_ratio_buff.begin(); eIt != trData->net_ratio_buff.end(); eIt++)
	{
		if (iPre >=eIt->first)
		{
			fRatioId = eIt->first;
		}
	}

	if (fRatioId > 0&& m_fRatioId != fRatioId)
	{
		m_fRatioId = fRatioId;

		if (mRatioActorElem.getRatio() != m_fRatioId)
		{
			mRatioActorElem.setRatio(m_fRatioId);
			if (m_fRatioId>0)
			{
				NetRatioBuff* ptrNetRatioBuff = trData->net_ratio_buff[m_fRatioId];

				mRatioActorElem.getfBg()->setVisible(true);
				mRatioActorElem.getfScore()->setString(StringUtils::format("%s %.2f",LL("fujingjifen"), iPre /100.f));
				mRatioActorElem.getfLevel()->setString(StringUtils::format("%s %d", LL("zengyidengji"), ptrNetRatioBuff->level));

				string sSeidInfo = "";
				for (size_t i = 0; i < ptrNetRatioBuff->seid.size(); i++)
				{
					if (ptrNetRatioBuff->seid[i])
					{
						if (i)
							sSeidInfo += string(LL("dunhao"));
						sSeidInfo += string(trData->m_mSeid[ptrNetRatioBuff->seid[i]]->Info);
					}
				}

				mRatioActorElem.getfInfo()->setString(StringUtils::format("%s: %s", LL("xiaoguo"), sSeidInfo.c_str()));
			}
			else
			{
				mRatioActorElem.getfBg()->setVisible(false);
			}
		}
	}
	else if (m_fRatioId > 0)
	{
		NetRatioBuff* ptrNetRatioBuff = trData->net_ratio_buff[m_fRatioId];
		mRatioActorElem.getfScore()->setString(StringUtils::format("%s %.2f", LL("fujingjifen"), iPre /100.f));
	}
}

void DispatchLayer::preDispatch(float fDelay)
{
	ITools::getInstance()->removeWaitLayer();
	dispatchFunction();
}
