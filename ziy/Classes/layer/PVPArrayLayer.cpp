//
//  PVPArrayLayer.cpp
//  TRGame
//
//  Created by Jimmy on 17/03/14.
//
//

#include "PVPArrayLayer.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "WinConditionTool.h"

#define ACTOR_ITEM_NUM 3
#define MAX_SEND_NUM 6
#define ACTOR_VIEW_W 318
#define ACTOR_VIEW_H 420
#define ACTOR_ITEM_W 106
#define ACTOR_ITEM_H 106

enum
{
	REQ_SAVE_DEFEND = 0,
	REQ_PRE_ENTRY = 1,
};

enum 
{
	ACTOR_ADD = 1,
	ACTOR_REMOVE = 2,
	ACTOR_ORDER =3
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ArraySprite::ArraySprite()
{

}

ArraySprite::~ArraySprite()
{

}

ArraySprite* ArraySprite::create(EventDelegate* ptrDelegate)
{
	ArraySprite* pRet = new ArraySprite();
	if (pRet&&pRet->init(ptrDelegate))
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

bool ArraySprite::init(EventDelegate* ptrDelegate)
{
	if (!Sprite::init())
	{
		return false;
	}
	m_ptrDel = ptrDelegate;
	m_bIsSelect = false;
	m_bIsOrder = false;
	m_iIndex = 0;

	initControl();
	return true;
}

bool ArraySprite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsOrder &&m_spBGHero->isVisible() && m_spBGHero->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_bIsSelect = true;
	}
	return true;
}

void ArraySprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsSelect && !m_spBGHero->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_bIsSelect = false;
	}
}

void ArraySprite::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsSelect)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_ptrDel->updateShow(m_iIndex, ACTOR_ORDER);
		m_bIsSelect = false;
	}
}

void ArraySprite::updateActor(int iActorId)
{
	if (iActorId)
	{
		HistoryActor* ptrActor = &history->actors[iActorId - 1];
		ptrActor->updateFashion(m_spImage, m_spWeapon, 0, DIRECT_RT);
		m_spBGNull->setVisible(false);
		m_spBGHero->setVisible(true);
	}
	else
	{
		m_spBGNull->setVisible(true);
		m_spBGHero->setVisible(false);
	}
}

void ArraySprite::updateOrder(int iOrderId)
{
	//出战序号
	if (iOrderId < 0)
	{
		m_spOrderId->setVisible(false);
		m_bIsOrder = true;
	}
	else
	{
		m_spOrderId->setVisible(true);
		m_bIsOrder = false;
		string strOrderId = StringUtils::format("new_ui/img_number/num4_%d.png", iOrderId + 1);
		m_spOrderId->setTexture(ReadSpriteTexture(strOrderId.c_str()));
	}
}

void ArraySprite::updateSprite(PVPArrayItem* ptrData, string strSeid)
{
	m_iIndex = ptrData->getPointID();

	//时装人物
	updateActor(ptrData->getActorID());

	//阵形序号
	string strPointId = StringUtils::format("new_ui/img_number/num5_%d.png", m_iIndex + 1);
	m_spPointId->setTexture(ReadSpriteTexture(strPointId.c_str()));

	//出战序号
	updateOrder(ptrData->getOrderID());

	//阵形特性
	m_labSeid->setString(strSeid);
}

void ArraySprite::initControl()
{
	//空背景
	m_spBGNull = ReadSpriteName("new_ui/pvp/px_bg2.png");
	this->addChild(m_spBGNull);
	Size szBG = m_spBGNull->getContentSize();

	//阵形序号
	m_spPointId = ReadSpriteName("new_ui/img_number/num5_1.png");
	m_spPointId->setPosition(Vec2(szBG.width / 2, szBG.height / 2));
	m_spBGNull->addChild(m_spPointId);

	//人物背景
	m_spBGHero = ReadSpriteName("new_ui/pvp/px_bg1.png");
	this->addChild(m_spBGHero);

	//时装人物
	m_spImage = ReadSpriteName("new_ui/battle/battle_48.png");
	m_spImage->setPosition(Vec2(szBG.width / 2, szBG.height / 2));
	m_spBGHero->addChild(m_spImage);

	//时装武器
	m_spWeapon = ReadSpriteName("new_ui/battle/battle_48.png");
	m_spWeapon->setPosition(Vec2(szBG.width / 2, szBG.height / 2));
	m_spBGHero->addChild(m_spWeapon);

	//出战序号
	m_spOrderId = ReadSpriteName("new_ui/img_number/num4_1.png");
	m_spOrderId->setAnchorPoint(Vec2(0.0f, 1.0f));
	m_spOrderId->setPosition(Vec2(1, szBG.height - 1));
	m_spBGHero->addChild(m_spOrderId, 5);

	//阵形特性
	m_labSeid = LabelSystemFont("", 12);
	m_labSeid->setAnchorPoint(Vec2(0.5f, 0.0f));
	m_labSeid->setPosition(Vec2(0, -szBG.height / 2));
	m_labSeid->setColor(Color3B::YELLOW);
	m_labSeid->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labSeid, 5);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPActorTableView* PVPActorTableView::create(map<int, int> mActorId, EventDelegate* ptrDelegate)
{
	PVPActorTableView* pRet = new PVPActorTableView();
	if (pRet&&pRet->init(mActorId, ptrDelegate))
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

bool PVPActorTableView::init(map<int, int> mActorId, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	m_ptrDel = ptrDelegate;
	m_bIsOrder = false;
	InitElemList(mActorId);

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
	tableView = TableView::create(this, Size(ACTOR_VIEW_W, ACTOR_VIEW_H));
	//设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 vecPos = Vec2(size.width / 2 - ACTOR_VIEW_W / 2 + 210, size.height / 2 - ACTOR_VIEW_H / 2);
	tableView->setPosition(vecPos);
	//设置代理对象
	tableView->setDelegate(this);
	// 填充顺序
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(tableView);

	updateElemList();
	return true;
}

PVPActorTableView::PVPActorTableView() : BTLayer(false)
{

}

PVPActorTableView::~PVPActorTableView()
{
	for (size_t i = 0; i < armor_list.size(); i++)
	{
		delete armor_list[i];
	}
}

void PVPActorTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

Size PVPActorTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(ACTOR_VIEW_W, ACTOR_ITEM_W);
}

TableViewCell* PVPActorTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();

	if (!cell) 
	{
		cell = PVPActorCell::create(this, idx);
	}
	else
	{
		PVPActorCell* c = (PVPActorCell*)cell;
		c->mALTB = this;
		c->UpDateCell(idx);
	}
	m_mPtrCell[idx] = (PVPActorCell*)cell;
	return cell;
}


ssize_t PVPActorTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ArmorListDate* PVPActorTableView::GetCellElem(int iIndex)
{
	return armor_list[iIndex];
}

void PVPActorTableView::removeActor(int iActorId)
{
	for (UInt i = 0; i < armor_list.size(); i++)
	{
		if (armor_list[i]->actorId == iActorId + 1)
		{
			armor_list[i]->bIsSend = false;
			Vec2 vecPos = tableView->getContentOffset();
			tableView->reloadData();
			tableView->setContentOffset(vecPos);
			break;
		}
	}
}

void PVPActorTableView::callBackListClick(int iIndex)
{
	if (m_bIsOrder)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ITools::getInstance()->createGlobalPrompt(LL("pvpisorder"), false);
		return;
	}

	bool bIsSend = armor_list[iIndex]->bIsSend;
	int iType = ACTOR_REMOVE;
	if (!bIsSend) //出兵
	{
		iType = ACTOR_ADD;
		int iSendNum = 0;
		for (UInt i = 0; i < armor_list.size(); i++)
		{
			if (armor_list[i]->bIsSend)
			{
				iSendNum++;
			}
		}

		if (iSendNum >= MAX_SEND_NUM)
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			ITools::getInstance()->createGlobalPrompt(LL("czrsym"), false);
			return;
		}
		playSound(SOUND_ACTOR_APPEAR);
	}
	else
	{
		playSound(SOUND_RIGHT_MOUSE_CLICK);
	}

	armor_list[iIndex]->bIsSend = !bIsSend;
	m_ptrDel->updateShow(armor_list[iIndex]->actorId - 1, iType);
	updateElemList();
}

void PVPActorTableView::InitElemList(map<int, int> mActorId)
{
	int armor_list_num = 0;
	for (int i = 0; i < history->ownArmyNum; i++)
	{
		ArmorListDate* ptrALD = new ArmorListDate();
		ptrALD->actorId = history->ownArmy[i] + 1;
		ptrALD->addOwnArmList = true;
		ptrALD->current_soul = 0;
		ptrALD->need_soul = 0;
		if (mActorId.count(ptrALD->actorId) > 0)
		{
			ptrALD->bIsSend = true;
		}
		armor_list[armor_list_num++] = ptrALD;
	}

	SortElemList();

	int null_elem = armor_list_num % ACTOR_ITEM_NUM;
	if (null_elem)
	{
		for (int i = 0; i < ACTOR_ITEM_NUM - null_elem; i++)
		{
			armor_list[armor_list_num++] = new ArmorListDate();
		}
	}
	cell_num = armor_list_num / ACTOR_ITEM_NUM;
}

void PVPActorTableView::SortElemList()
{
	int iActorId1 = -1, iActorId2 = -1;
	ArmorListDate* ptrArmorListDate = NULL;

	for (size_t i = 1; i < armor_list.size(); i++)
	{
		for (size_t j = armor_list.size() - 1; j > i; j--)
		{
			if ((int)j < history->ownArmyNum)
			{
				iActorId1 = armor_list[j]->actorId - 1;
				iActorId2 = armor_list[j - 1]->actorId - 1;
				if (history->actors[iActorId1].actor_quality > history->actors[iActorId2].actor_quality ||
					(history->actors[iActorId1].actor_quality == history->actors[iActorId2].actor_quality&&history->actors[iActorId1].GetActorLv() > history->actors[iActorId2].GetActorLv()))
				{
					ptrArmorListDate = armor_list[j];
					armor_list[j] = armor_list[j - 1];
					armor_list[j - 1] = ptrArmorListDate;
				}
			}
		}
	}
}

void PVPActorTableView::updateElemList()
{
	SortElemList();

	map<int, ArmorListDate*> mTempList;
	int iIndex = 0;
	for (UInt i = 0; i < armor_list.size(); i++)
	{
		ArmorListDate* ptrArmor = armor_list[i];
		if (ptrArmor->bIsSend)
		{
			mTempList[iIndex++] = ptrArmor;
		}
	}

	for (UInt i = 0; i < armor_list.size(); i++)
	{
		ArmorListDate* ptrArmor = armor_list[i];
		if (!ptrArmor->bIsSend)
		{
			mTempList[iIndex++] = ptrArmor;
		}
	}
	armor_list = mTempList;

	Vec2 point = tableView->getContentOffset();
	tableView->reloadData();
	tableView->setContentOffset(point);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPActorCell::PVPActorCell()
{

}

PVPActorCell* PVPActorCell::create(PVPActorTableView* ptrALTB, int idx)
{
	PVPActorCell* pRet = new PVPActorCell();
	pRet->mALTB = ptrALTB;
	if (pRet&&pRet->init(idx))
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

bool PVPActorCell::init(int idx)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	int iWidth = ACTOR_ITEM_W;
	int iHeight = ACTOR_ITEM_H;

	for (int i = 0; i < ACTOR_ITEM_NUM; i++)
	{
		Sprite* elem_bg = ReadSpriteName("new_ui/hero/herobox_bg1.png");
		elem_bg->setPosition(Vec2(iWidth / 2 + i * iWidth, iHeight / 2));
		this->addChild(elem_bg);

		int iBgWidth = elem_bg->getContentSize().width;
		int iBgHeight = elem_bg->getContentSize().height;

		Sprite* elem_fashion = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_fashion->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_fashion, 1);
		elem_fashion->setVisible(false);

		Sprite* elem_img = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_img->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_img, 1);

		Label* elem_name = LabelSystemFont("", 16);
		elem_name->setPosition(Vec2(iBgWidth / 2, 21));
		elem_bg->addChild(elem_name, 3);
		elem_name->enableOutline(Color4B::BLACK, 1);

		Label* elem_lv = LabelSystemFont("", 16);
		elem_lv->setAnchorPoint(Vec2(1, 1));
		elem_lv->setPosition(Vec2(83, 81));
		elem_bg->addChild(elem_lv, 3);
		elem_lv->setColor(Color3B::YELLOW);
		elem_lv->enableOutline(Color4B::BLACK, 1);

		Sprite* elem_quality = ReadSpriteName("new_ui/hero/herobox_q1.png");
		elem_quality->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_quality);

		Sprite* elem_soul_bg = ReadSpriteName("new_ui/hero/herobox_jiangh1.png");
		elem_soul_bg->setAnchorPoint(Vec2(0.5, 0.5));
		elem_soul_bg->setPosition(Vec2(iBgWidth / 2, 33));
		elem_bg->addChild(elem_soul_bg, 3);

		Sprite* elem_cursoul = ReadSpriteName("new_ui/hero/herobox_jiangh2.png");
		elem_cursoul->setAnchorPoint(Vec2(0, 0.5));
		elem_cursoul->setPosition(Vec2(6, elem_soul_bg->getContentSize().height / 2));
		elem_soul_bg->addChild(elem_cursoul, 3);

		Label* elem_soul_num = LabelSystemFont("20/80", 12);
		elem_soul_num->setAnchorPoint(Vec2(1, 0));
		elem_soul_num->setPosition(Vec2(elem_soul_bg->getContentSize().width - 5, elem_soul_bg->getContentSize().height / 2 + elem_cursoul->getContentSize().height / 2));
		elem_soul_num->enableOutline(Color4B::BLACK, 1);
		elem_soul_bg->addChild(elem_soul_num, 3);

		Sprite* elem_recruit = ReadSpriteName("new_ui/hero/herobox_xz1.png");
		elem_recruit->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_recruit);
		elem_recruit->setVisible(false);

		Sprite* elem_soul = ReadSpriteName("new_ui/hero/icon_soul.png");
		elem_soul->setAnchorPoint(Vec2(0, 0.5));
		elem_soul->setPosition(Vec2(10, 33));
		elem_soul->setScale(0.5f);
		elem_bg->addChild(elem_soul, 3);

		Sprite* elem_occupation = ReadSpriteName("new_ui/hero/job1.png");
		elem_occupation->setAnchorPoint(Vec2(0, 1));
		elem_occupation->setPosition(Vec2(4, iBgHeight - 5));
		elem_occupation->setScale(0.65f);
		elem_bg->addChild(elem_occupation, 4);

		//出战
		Sprite* spSend = ReadSpriteName("new_ui/battle/war_cz2.png");
		spSend->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(spSend, 5);

		elem[i] = new ArmorListInfo();
		elem[i]->setBG(elem_bg);
		elem[i]->setIMG(elem_img);
		elem[i]->setFASHION(elem_fashion);
		elem[i]->setQUALITY(elem_quality);
		elem[i]->setTNAME(elem_name);
		elem[i]->setLV(elem_lv);
		elem[i]->setSOUL(elem_soul_bg);
		elem[i]->setSOULNUM(elem_soul_num);
		elem[i]->setACTORID(-1);
		elem[i]->setMINDEX(idx * ACTOR_ITEM_NUM + i);
		elem[i]->setMISOWN(0);
		elem[i]->setMRECRUIT(elem_recruit);
		elem[i]->setOCCUPATION(elem_occupation);
		elem[i]->setMSOULICON(elem_soul);
		elem[i]->setMCURSOUL(elem_cursoul);
		elem[i]->setSpSend(spSend);
	}

	UpDateCell(idx);
	initTouches();

	return true;
}

void PVPActorCell::initTouches()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event) 
	{
		click_elem_index = -1;
		for (int i = 0; i < ACTOR_ITEM_NUM; i++)
		{
			Rect elemBox = elem[i]->getBG()->getBoundingBox();
			if (Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(elem[i]->getBG()->convertToNodeSpace(touch->getLocation())) && elem[i]->getACTORID() > 0)
			{
				click_elem_index = i;
				return true;
			}
		}
		return true;
	};
	listener->onTouchEnded = [&](Touch* touch, Event* event) 
	{
		if (click_elem_index > -1 && (abs((int)(touch->getStartLocation().y - touch->getLocation().y)) < 10 || history->m_bGuide)) //新手引导总能进入
		{
			mALTB->callBackListClick(elem[click_elem_index]->getMINDEX());
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PVPActorCell::UpDateCell(int idx)
{
	for (int i = 0; i < ACTOR_ITEM_NUM; i++)
	{
		ArmorListDate* ptrALD = mALTB->GetCellElem(idx * ACTOR_ITEM_NUM + i);
		elem[i]->setMINDEX(idx * ACTOR_ITEM_NUM + i);

		if (ptrALD->actorId <= 0)
		{
			elem[i]->getBG()->setVisible(false);
			elem[i]->setACTORID(-1);
		}
		else
		{
			elem[i]->getBG()->setVisible(true);

			HistoryActorPtr ptrActor = &history->actors[ptrALD->actorId - 1];

			int fashion_width = 48;
			if (ptrActor->mFashion[0] <= 0)
			{
				elem[i]->getFASHION()->setVisible(false);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov", 0));
				fashion_width = tex->getContentSize().width;
				elem[i]->getIMG()->setTexture(tex);
			}
			else
			{
				elem[i]->getFASHION()->setVisible(true);
				__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

				Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
				elem[i]->getIMG()->setTexture(texfa);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
				fashion_width = tex->getContentSize().width;
				elem[i]->getFASHION()->setTexture(tex);
				elem[i]->getFASHION()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
				int iZOrder = 0;
				if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6])
					iZOrder = 2;
				elem[i]->getFASHION()->setLocalZOrder(iZOrder);
				fashion_width = texfa->getContentSize().width;
			}
			elem[i]->getIMG()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));

			elem[i]->getTNAME()->setString(trData->actors[ptrActor->actor_id].getActorName());
			elem[i]->getTNAME()->setColor(ITools::getInstance()->getActorPzColor(ptrActor->actor_quality));

			elem[i]->setACTORID(ptrALD->actorId);

			elem[i]->getMRECRUIT()->setVisible(ptrALD->enable_recruit);

			elem[i]->getOCCUPATION()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId()).c_str())));


			__String* quality_icon_path = __String::createWithFormat("new_ui/hero/herobox_q%d.png", ptrActor->actor_quality);
			elem[i]->getQUALITY()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(quality_icon_path->getCString(), 0, 1)));

			elem[i]->getLV()->setVisible(true);
			elem[i]->getLV()->setString(IntToString(ptrActor->GetActorLv()));
			elem[i]->getSOUL()->setVisible(false);
			elem[i]->getMSOULICON()->setVisible(false);
			elem[i]->setMISOWN(1);

			//出战
			elem[i]->getSpSend()->setVisible(ptrALD->bIsSend);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPArrayLayer::PVPArrayLayer()
{
	m_pArrayInfo = nullptr;
}

PVPArrayLayer::~PVPArrayLayer()
{
	SAFE_DELETE(m_pArrayInfo);
}

PVPArrayLayer* PVPArrayLayer::create(int iType, bool bIsInit /*= true*/)
{
	PVPArrayLayer* pRet = new PVPArrayLayer();
	if(pRet&&pRet->init(iType, bIsInit))
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

bool PVPArrayLayer::init(int iType, bool bIsInit)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iType = iType;
	m_bIsInit = bIsInit;
	m_iArrayIndex = 1;

	initArrayData();
	initControl();
	updateArrayType();
	updateBtnEnabled();
	return true;
}

void PVPArrayLayer::updateShow(int iIndex, int iType)
{
	if (iType == ACTOR_ORDER)
	{
		int iMaxIndex = -1;
		for (int i = 0; i < MAX_SEND_NUM; i++)
		{
			if (m_mOrderId[i] > iMaxIndex)
			{
				iMaxIndex = m_mOrderId[i];
			}
		}
		m_mOrderId[iIndex] = iMaxIndex + 1;
		m_mSpArray[iIndex]->updateOrder(m_mOrderId[iIndex]);

		if (m_mOrderId[iIndex] == MAX_SEND_NUM - 1)
		{
			m_btnConfirm->setEnabled(true);
		}
	}
	else
	{
		for (int i = 0; i < MAX_SEND_NUM; i++)
		{
			bool bUpdate = false;
			if (iType == ACTOR_ADD && !m_pArrayInfo->m_mArray[i]->getActorID())
			{
				m_pArrayInfo->m_mArray[i]->setActorID(iIndex + 1);
				bUpdate = true;
			}
			else if (iType == ACTOR_REMOVE && m_pArrayInfo->m_mArray[i]->getActorID() == iIndex + 1)
			{
				m_pArrayInfo->m_mArray[i]->setActorID(0);
				bUpdate = true;
			}

			if (bUpdate)
			{
				m_mSpArray[i]->updateActor(m_pArrayInfo->m_mArray[i]->getActorID());
				updateBtnEnabled();
				break;
			}
		}
	}
}

void PVPArrayLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//背景
	int iWarId = PVPData::getInstance()->getPVPWarId();
	int iMapId = trData->m_mNetWarInfo[iWarId]->iMapId + rand() % 3;
	PVPData::getInstance()->setMapID(iMapId);
	string strMap = StringUtils::format("img/HM/%d.jpg", iMapId);
	m_spBG = ReadSpriteNormal(strMap);
	m_spBG->setPosition(point);
	this->addChild(m_spBG);

	//阵形背景，起始坐标（3，2）
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			auto spItemBG = ReadSpriteName("new_ui/pvp/px_bg3.png");
			int iPosX = (3 + i) * 48 + 24 - 25;
			int iPosY = (2 + j) * 48 + 24;
			spItemBG->setPosition(Vec2(iPosX, m_spBG->getContentSize().height - iPosY));
			m_spBG->addChild(spItemBG);
		}
	}

	//阵名背景
	auto spNameBG = ReadSpriteName("new_ui/pvp/px_bg4.png");
	spNameBG->setPosition(point + Vec2(-195, 184));
	this->addChild(spNameBG);

	//阵名
	m_labName = LabelSystemFont("", 18);
	m_labName->setPosition(spNameBG->getPosition());
	m_labName->setColor(Color3B::YELLOW);
	m_labName->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labName);

	//TV背景
	auto spTvBG = ReadSpriteName("new_ui/pvp/pvp_bg6.png");
	spTvBG->setPosition(point + Vec2(160, 0));
	this->addChild(spTvBG);

	map<int, int> mActorId;
	for (UInt i = 0; i < m_pArrayInfo->m_mArray.size(); i++)
	{
		int iActorId = m_pArrayInfo->m_mArray[i]->getActorID();
		mActorId[iActorId] = 1;
	}
	m_tvActorList = PVPActorTableView::create(mActorId, this);
	m_tvActorList->setScale(0.76f); 
	this->addChild(m_tvActorList);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//上一页
	Sprite* spLastN = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* spLastS = ReadSpriteName("new_ui/img_global/switch1.png");
	spLastS->setScale(fScale);
	MenuItemSprite* btnLast = MenuItemSprite::create(spLastN, spLastS, CC_CALLBACK_1(PVPArrayLayer::callBackLast, this));
	spLastS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLastS->setPosition(Vec2(spLastS->getContentSize().width / 2, spLastS->getContentSize().height / 2));
	btnLast->setPosition(point + Vec2(-330, 0));
	ptrMenu->addChild(btnLast);

	//下一页
	Sprite* spNextN = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextN->setFlippedX(true);
	Sprite* spNextS = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextS->setFlippedX(true);
	spNextS->setScale(fScale);
	MenuItemSprite* btnNext = MenuItemSprite::create(spNextN, spNextS, CC_CALLBACK_1(PVPArrayLayer::callBackNext, this));
	spNextS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNextS->setPosition(Vec2(spNextS->getContentSize().width / 2, spNextS->getContentSize().height / 2));
	btnNext->setPosition(point + Vec2(-60, 0));
	ptrMenu->addChild(btnNext);

	//调整战序
	Sprite* spOrderN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spOrderTextN = ReadSpriteName("new_ui/img_text/pvp_txt1_1.png");
	spOrderTextN->setPosition(Vec2(spOrderN->getContentSize().width / 2, spOrderN->getContentSize().height / 2));
	spOrderN->addChild(spOrderTextN);
	Sprite* spOrderS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spOrderTextS = ReadSpriteName("new_ui/img_text/pvp_txt1_2.png");
	spOrderTextS->setPosition(Vec2(spOrderS->getContentSize().width / 2, spOrderS->getContentSize().height / 2));
	spOrderS->addChild(spOrderTextS);
	Sprite* spOrderD = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spOrderTextD = ReadSpriteName("new_ui/img_text/pvp_txt1_1.png");
	spOrderTextD->setPosition(Vec2(spOrderD->getContentSize().width / 2, spOrderD->getContentSize().height / 2));
	spOrderD->addChild(spOrderTextD);
	addSpriteGray(spOrderD);
	addSpriteGray(spOrderTextD);
	m_btnOrder = MenuItemSprite::create(spOrderN, spOrderS, spOrderD, CC_CALLBACK_1(PVPArrayLayer::callBackOrder, this));
	m_btnOrder->setPosition(point + Vec2(-31, -92));
	ptrMenu->addChild(m_btnOrder);

	//保存守阵
	Sprite* spSaveN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spSaveTextN = ReadSpriteName("new_ui/img_text/pvp_txt1_3.png");
	spSaveTextN->setPosition(Vec2(spSaveN->getContentSize().width / 2, spSaveN->getContentSize().height / 2));
	spSaveN->addChild(spSaveTextN);
	Sprite* spSaveS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spSaveTextS = ReadSpriteName("new_ui/img_text/pvp_txt1_4.png");
	spSaveTextS->setPosition(Vec2(spSaveS->getContentSize().width / 2, spSaveS->getContentSize().height / 2));
	spSaveS->addChild(spSaveTextS);
	Sprite* spSaveD = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spSaveTextD = ReadSpriteName("new_ui/img_text/pvp_txt1_3.png");
	spSaveTextD->setPosition(Vec2(spSaveD->getContentSize().width / 2, spSaveD->getContentSize().height / 2));
	spSaveD->addChild(spSaveTextD);
	addSpriteGray(spSaveD);
	addSpriteGray(spSaveTextD);
	m_btnSave = MenuItemSprite::create(spSaveN, spSaveS, spSaveD, CC_CALLBACK_1(PVPArrayLayer::callBackSave, this));
	m_btnSave->setPosition(point + Vec2(-31, -148));
	m_btnSave->setVisible(false);
	ptrMenu->addChild(m_btnSave);

	//挑战
	Sprite* spFightN = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	Sprite* spFightS = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	Sprite* spFightD = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	spFightS->setScale(fScale);
	addSpriteGray(spFightD);
	m_btnFight = MenuItemSprite::create(spFightN, spFightS, spFightD, CC_CALLBACK_1(PVPArrayLayer::callBackFight, this));
	spFightS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spFightS->setPosition(Vec2(spFightN->getContentSize().width / 2, spFightN->getContentSize().height / 2));
	m_btnFight->setPosition(point + Vec2(-31, -136));
	m_btnFight->setVisible(false);
	ptrMenu->addChild(m_btnFight);
	 
	//确认
	Sprite* spConfirmN = ReadSpriteName("new_ui/battle/battle_confirm.png");
	Sprite* spConfirmS = ReadSpriteName("new_ui/battle/battle_confirm.png");
	Sprite* spConfirmD = ReadSpriteName("new_ui/battle/battle_confirm.png");
	spConfirmS->setScale(fScale);
	addSpriteGray(spConfirmD);
	m_btnConfirm = MenuItemSprite::create(spConfirmN, spConfirmS, spConfirmD, CC_CALLBACK_1(PVPArrayLayer::callBackConfirm, this));
	spConfirmS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spConfirmS->setPosition(Vec2(spConfirmN->getContentSize().width / 2, spConfirmN->getContentSize().height / 2));
	m_btnConfirm->setPosition(point + Vec2(-59, 54));
	m_btnConfirm->setVisible(false);
	ptrMenu->addChild(m_btnConfirm);

	//取消
	Sprite* spCancelN = ReadSpriteName("new_ui/battle/battle_down.png");
	Sprite* spCancelS = ReadSpriteName("new_ui/battle/battle_down.png");
	spCancelS->setScale(fScale);
	m_btnCancel = MenuItemSprite::create(spCancelN, spCancelS, CC_CALLBACK_1(PVPArrayLayer::callBackCancel, this));
	spCancelS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spCancelS->setPosition(Vec2(spCancelN->getContentSize().width / 2, spCancelN->getContentSize().height / 2));
	m_btnCancel->setPosition(point + Vec2(-3, 54));
	m_btnCancel->setVisible(false);
	ptrMenu->addChild(m_btnCancel);

	//返回
	Sprite* spCloseN = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/battle/flippage3.png");
	spCloseS->setScale(fScale);
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PVPArrayLayer::callBackClose, this));
	spCloseS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spCloseS->setPosition(Vec2(spCloseN->getContentSize().width / 2, spCloseN->getContentSize().height / 2));
	btnClose->setPosition(size.width - 32, size.height - 32);
	ptrMenu->addChild(btnClose);

	if (m_iType == ARRAY_ATTACK)
	{
		m_btnOrder->setPosition(point + Vec2(-31, -52));
		m_btnFight->setVisible(true);
	}
	else if (m_iType == ARRAY_DEFEND)
	{
		m_btnSave->setVisible(true);
	}
}

void PVPArrayLayer::initArrayData()
{
	PVPArrayInfo* ptrArray = PVPData::getInstance()->m_mArrayInfo[m_iType];
	m_pArrayInfo = new PVPArrayInfo();
	if (ptrArray->getIsInit())
	{
		m_pArrayInfo->updateArray(ptrArray);
	}
	else
	{
		m_pArrayInfo->setIsInit(true);
		m_pArrayInfo->setArrayID(1);
		for (int i = 0; i < MAX_SEND_NUM; i++)
		{
			PVPArrayItem* ptrArray = new PVPArrayItem();
			ptrArray->setPointID(i);
			ptrArray->setOrderID(i);
			ptrArray->setActorID(0);
			m_pArrayInfo->m_mArray[i] = ptrArray;
		}
	}

	m_iArrayIndex = m_pArrayInfo->getArrayID();
	for (int i = 0; i < MAX_SEND_NUM; i++)
	{
		m_mOrderId[i] = m_pArrayInfo->m_mArray[i]->getOrderID();
	}
}

void PVPArrayLayer::updateArrayType()
{
	int iSize = trData->m_mPVPArray.size();
	if (m_iArrayIndex < 1)
	{
		m_iArrayIndex = iSize;
	}
	else if (m_iArrayIndex > iSize)
	{
		m_iArrayIndex = 1;
	}

	PVPArray* ptrArray = trData->m_mPVPArray[m_iArrayIndex];
	m_labName->setString(ptrArray->strName);

	for (UInt i = 0; i < ptrArray->mPosGroup.size(); i++)
	{
		if (m_mSpArray.count(i) <= 0)
		{
			m_mSpArray[i] = ArraySprite::create(this);
			m_spBG->addChild(m_mSpArray[i]);
		}
		int iPosIndex = ptrArray->mPosGroup[i];
		int iPosX = trData->m_mPVPPoint[iPosIndex]->iPosX;
		int iPosY = trData->m_mPVPPoint[iPosIndex]->iPosY;
		m_mSpArray[i]->setPosition(Vec2((iPosX * 2 + 1) * 24 - 25, m_spBG->getContentSize().height - (iPosY * 2 + 1) * 24));
		int iSeidId = ptrArray->mSeidGroup[i][0];
		m_mSpArray[i]->updateSprite(m_pArrayInfo->m_mArray[i], trData->m_mSeid[iSeidId]->sename);
		m_mSpArray[i]->updateOrder(m_mOrderId[i]);
	}
}

void PVPArrayLayer::updateBtnEnabled()
{
	bool bIsFull = true;
	bool bIsOrder = false;
	for (int i = 0; i < MAX_SEND_NUM; i++)
	{
		if (!m_pArrayInfo->m_mArray[i]->getActorID())
		{
			bIsFull = false;
			break;
		}
	}

	if (bIsFull)
	{
		bIsOrder = true;
		for (int i = 0; i < MAX_SEND_NUM; i++)
		{
			if (m_mOrderId[i] < 0)
			{
				bIsOrder = false;
				break;
			}
		}
	}

	m_btnConfirm->setEnabled(bIsOrder);
	m_btnOrder->setEnabled(bIsFull);
	m_btnSave->setEnabled(bIsOrder);
	m_btnFight->setEnabled(bIsOrder);
}

void PVPArrayLayer::preEntryBattle()
{
	history->m_iBattleType = BATTLE_PVP;
	history->m_iCurWarId = PVPData::getInstance()->getPVPWarId();
	history->m_iBattleStage = 0;

	PVPData* ptrData = PVPData::getInstance();
	map<int, PVPArrayItem*> mArrayItem = ptrData->m_mArrayInfo[ARRAY_ATTACK]->m_mArray;
	history->battleOwnNum = mArrayItem.size();
	for (UInt i = 0; i < mArrayItem.size(); i++)
	{
		PVPArrayItem* ptrItem = mArrayItem[i];
		history->battleOwn[ptrItem->getOrderID()] = &history->actors[ptrItem->getActorID() - 1];
	}
	history->CalcBaseLevel();
	WinConditionTool::getInstance()->InitBattleCondition();
	history->mWeather.clear();
	history->mRealWeather.clear();
	history->mLadderEActor.clear();
	history->mNet72EquipActor.clear();
	history->mEnemyElitePlus.clear();
	history->mNet72EquipLevel.clear();
	history->mMapDateInfo->GetBattleImageData()->ClearActorList();
	memset(history->var, 0, MAX_CHAR_VAR_NUM * sizeof(history->var[0]));

	//进入战场
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

string PVPArrayLayer::getArrayData()
{
	m_pArrayInfo->setArrayID(m_iArrayIndex);

	string strRes;
	FastWriter fastWriter;
	strRes = fastWriter.write(m_pArrayInfo->saveJson());
	strRes.erase(strRes.end() - 1); //去掉末尾的'\n'
	return strRes;
}

void PVPArrayLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch (iReqType)
	{
	case REQ_SAVE_DEFEND:
	{
		strURL = "saveDefLineup";
		string strArray = getArrayData();
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s", strToken.c_str()));
		strData = StringUtils::format("{\"defLineup\":%s,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strArray.c_str(), strToken.c_str(), validCode.c_str());
		strTag = "REQ_SAVE_DEFEND";
	}
	break;
	case REQ_PRE_ENTRY:
	{
		strURL = "validateOnOff";
		int iSelectRank = PVPData::getInstance()->getSelectRank();
		string validCode = md5.ToMD5(StringUtils::format("num=%d&gameToken=%s", iSelectRank, strToken.c_str()));
		strData = StringUtils::format("{\"num\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iSelectRank, strToken.c_str(), validCode.c_str());
		strTag = "REQ_PRE_ENTRY";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(PVPArrayLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void PVPArrayLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
	if (reader.parse(responseData, value) && value.size() > 0)
	{
		if (strTag == "REQ_SAVE_DEFEND")
		{
			onSaveDefSucceed(value["result"]);
		}
		else if (strTag == "REQ_PRE_ENTRY")
		{
			onPreEntrySucceed(value["result"]);
		}
	}
}

void PVPArrayLayer::onSaveDefSucceed(Json::Value valRes)
{
	if (valRes["isTrue"].isInt() && valRes["isTrue"].asInt())
	{
		ITools::getInstance()->createGlobalPrompt(LL("savedefsucceed"), true);
		PVPData::getInstance()->m_mArrayInfo[m_iType]->updateArray(m_pArrayInfo);
		if (!m_bIsInit)
		{
			PVPData::getInstance()->startPVP();
		}
		this->removeFromParent();
	}
}

void PVPArrayLayer::onPreEntrySucceed(Json::Value valRes)
{
	if (valRes["isOpen"].isBool() && valRes["isOpen"].asBool())
	{
		string strMark = "";
		ParseString(strMark, valRes["tid"]);
		PVPData::getInstance()->setMarkID(strMark);
		preEntryBattle();
	}
}

void PVPArrayLayer::callBackLast(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_iArrayIndex--;
	updateArrayType();
}

void PVPArrayLayer::callBackNext(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_iArrayIndex++;
	updateArrayType();
}

void PVPArrayLayer::callBackOrder(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	for (int i= 0; i < MAX_SEND_NUM; i++)
	{
		m_mOrderId[i] = -1;
		m_mSpArray[i]->updateOrder(m_mOrderId[i]);
	}

	m_btnConfirm->setVisible(true);
	m_btnCancel->setVisible(true);
	updateBtnEnabled();
	m_tvActorList->setIsOrder(true);
}

void PVPArrayLayer::callBackSave(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	onHttpRequest(REQ_SAVE_DEFEND);
}

void PVPArrayLayer::callBackConfirm(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	for (int i = 0; i < MAX_SEND_NUM; i++)
	{
		m_pArrayInfo->m_mArray[i]->setOrderID(m_mOrderId[i]);
		m_mSpArray[i]->updateOrder(m_mOrderId[i]);
	}

	m_btnConfirm->setVisible(false);
	m_btnCancel->setVisible(false);
	updateBtnEnabled();
	m_tvActorList->setIsOrder(false);
}

void PVPArrayLayer::callBackCancel(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	for (int i = 0; i < MAX_SEND_NUM; i++)
	{
		m_mOrderId[i] = m_pArrayInfo->m_mArray[i]->getOrderID();
		m_mSpArray[i]->updateOrder(m_mOrderId[i]);
	}

	m_btnConfirm->setVisible(false);
	m_btnCancel->setVisible(false);
	updateBtnEnabled();
	m_tvActorList->setIsOrder(false);
}

void PVPArrayLayer::callBackFight(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_pArrayInfo->setArrayID(m_iArrayIndex);
	PVPData::getInstance()->updateAtcJson(m_pArrayInfo);
	onHttpRequest(REQ_PRE_ENTRY);
}

void PVPArrayLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	removeFromParent();
}

