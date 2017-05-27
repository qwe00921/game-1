#include "BSelectGoodsOrSkillTableView.h"
#include "Defines.h"

BSelectGoodsOrSkillTableView* BSelectGoodsOrSkillTableView::create(map<int, GoodsOrSkillDatePtr>* ptrDate,EventDelegate* ptrDelegate)
{
	BSelectGoodsOrSkillTableView *pRet = new BSelectGoodsOrSkillTableView();
	if (pRet && pRet->init(ptrDate,ptrDelegate))
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

bool BSelectGoodsOrSkillTableView::init(map<int, GoodsOrSkillDatePtr>* ptrDate, EventDelegate* ptrDelegate)
{
    if ( !Layer::init() )
    {
        return false;
    }
	float fBattleColumn = BATTLE_TABLEVIEW_COLUMN;
	this->mCellNum = ceil(ptrDate->size() / fBattleColumn);
	this->mDelegate = ptrDelegate;
	this->mElemDate = ptrDate;
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(590,300));
	if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)
	{
		tableView->setTouchEnabled(false);
		
	}
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(15,25));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();
    
    return true;
}

map<int, GoodsOrSkillDatePtr> BSelectGoodsOrSkillTableView::getDateElem(int iDx)
{
	map<int, GoodsOrSkillDatePtr> pRet;

	int iIndex = iDx * BATTLE_TABLEVIEW_COLUMN;
	int iMaxIndex = (int)mElemDate->size();
	for (int i = 0; i < BATTLE_TABLEVIEW_COLUMN && iIndex + i < iMaxIndex; i++)
	{
		pRet[i] = mElemDate->at(iIndex + i);
	}
	return pRet;
}

BSelectGoodsOrSkillTableView::BSelectGoodsOrSkillTableView():BTLayer(false)
{
	mCellNum = 1;
}

BSelectGoodsOrSkillTableView::~BSelectGoodsOrSkillTableView()
{

}

void BSelectGoodsOrSkillTableView::scrollViewDidScroll(ScrollView* view)
{

}

void BSelectGoodsOrSkillTableView::scrollViewDidZoom(ScrollView* view)
{

}

void BSelectGoodsOrSkillTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void BSelectGoodsOrSkillTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void BSelectGoodsOrSkillTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size BSelectGoodsOrSkillTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(590,116);
}

TableViewCell* BSelectGoodsOrSkillTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = GoodsOrSkillCell::create(getDateElem(idx),mDelegate);
    }
    else
    {
		GoodsOrSkillCell* c = (GoodsOrSkillCell*)cell;
		c->UpDateCell(getDateElem(idx), mDelegate);
    }
    return cell;
}

ssize_t BSelectGoodsOrSkillTableView::numberOfCellsInTableView(TableView *table)
{
	return mCellNum;
}

GoodsOrSkillCell* GoodsOrSkillCell::create(map<int, GoodsOrSkillDatePtr> ptrDate, EventDelegate* ptrDelegate)
{
	GoodsOrSkillCell* pRet = new GoodsOrSkillCell();
	if (pRet&&pRet->init(ptrDate,ptrDelegate))
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

bool GoodsOrSkillCell::init(map<int, GoodsOrSkillDatePtr> ptrDate, EventDelegate* ptrDelegate)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	this->mDelegate = ptrDelegate;
	int iCellWidth = 148;
	int iCellHeight = 116;
	mClickElemIndex = -1;

	string nameSS = "";
	string iconPathSS = "";
	string effectSS = "";
	bool isSkill = false;
	bool isVisibal = true;
	int iDataIndex = -1;
	bool bEnableClick = true;
	bool bCopyMark = false;

	for (int i = 0; i < BATTLE_TABLEVIEW_COLUMN; i++)
	{
		if (i < (int)ptrDate.size())
		{
			isVisibal = true;
			int iId = ptrDate[i]->getEID();
			if (ptrDate[i]->getESHOWTYPE() == BSELECT_SHOW_GOODS)
			{
				GoodsItem* ptrGoods = trData->m_mGoodsItem[iId];
				nameSS = ptrGoods->strName;
				iconPathSS = history->GetGoodsImagePath(ptrGoods->iGoodsId);
				effectSS = ptrGoods->strUeid;
			}
			else
			{
				isSkill = true;
				SkillAttr* ptrSkill = &trData->skills[iId - 1];
				nameSS = ptrSkill->skill_name;
				iconPathSS = StringUtils::format("img/Skill/%d.png", ptrSkill->image_id+1);
				effectSS = ptrSkill->skill_effect;
			}
			iDataIndex = ptrDate[i]->getEINDEX();
			bEnableClick = ptrDate[i]->getEENABLECLICK();
			bCopyMark = ptrDate[i]->getIsCopy();
		}
		else
		{
			isVisibal = false;
			iconPathSS = "img/Items/point.png";
			iDataIndex = -1;
		}
		Sprite* bg = ReadSpriteName("new_ui/battle/use_bg2.png");
		bg->setPosition(iCellWidth / 2 + i * iCellWidth -1, iCellHeight / 2);
		this->addChild(bg);
		bg->setVisible(isVisibal);

		int iBgWidth = bg->getContentSize().width;
		int iBgHeight = bg->getContentSize().height;

		Label* nameLab = LabelSystemFont(nameSS.c_str(), 18);
		nameLab->setColor(Color3B(153, 51, 0));
		nameLab->setPosition(Vec2(iBgWidth / 2, iBgHeight - 18));
		bg->addChild(nameLab);

		Sprite* iconSp = ReadSpriteNormal(iconPathSS.c_str());
		iconSp->setScale(0.9f);
		iconSp->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2 - 5));
		bg->addChild(iconSp);

		Label* effectLab = LabelSystemFont(effectSS.c_str(), 14);
		effectLab->setColor(Color3B(255,153,0));
		effectLab->setPosition(Vec2(iBgWidth/2,17));
		bg->addChild(effectLab);

		Sprite* numSp = NULL;
		if (isVisibal)
		{
			int iNum = ptrDate[i]->getENUM();
			int iOffsetX = 0;

			if (iNum >= 100)
				iOffsetX = 5;
			else if (iNum >= 10 && iNum < 100)
				iOffsetX = 8;
			else if (iNum >= 0)
			{
				iOffsetX = 11;
			}

			numSp = ITools::getInstance()->getNumSprite(iNum, true);
			numSp->setAnchorPoint(Vec2(1, 0.5));
			numSp->setPosition(Vec2(iBgWidth - iOffsetX, 36));
			bg->addChild(numSp);
		}

		if (isSkill)
		{
			string sImagepath = "new_ui/battle/use_mp.png";
			if (isVisibal&&trData->skills[ptrDate[i]->getEID() - 1].iTimes>0)
			{
				sImagepath = "new_ui/battle/use_sycs.png";
			}

			Sprite* mpSp = ReadSpriteName(sImagepath.c_str());
			mpSp->setPosition(Vec2(iBgWidth - 5,58));
			mpSp->setAnchorPoint(Vec2(1,0.5));
			bg->addChild(mpSp);
		}

		Sprite* spCopy = ReadSpriteName("new_ui/battle/use_skxs.png");
		spCopy->setPosition(Vec2(14, iBgHeight / 2 - 5));
		spCopy->setVisible(bCopyMark);
		bg->addChild(spCopy);

		Sprite* graySp = ReadSpriteName("new_ui/battle/use_bg2_1.png");
		graySp->setPosition(Vec2(iBgWidth/2,iBgHeight/2));
		bg->addChild(graySp, 1);
		graySp->setVisible(!bEnableClick);

		GoodsOrSkillInfo* ptrGOSInfo = new GoodsOrSkillInfo();
		ptrGOSInfo->setEBG(bg);
		ptrGOSInfo->setENAME(nameLab);
		ptrGOSInfo->setEICON(iconSp);
		ptrGOSInfo->setEEFFECT(effectLab);
		ptrGOSInfo->setCopyMark(spCopy);
		ptrGOSInfo->setENUM(numSp);
		ptrGOSInfo->setEDATAINDEX(iDataIndex);
		ptrGOSInfo->setEGRAY(graySp);
		ptrGOSInfo->setEENABLECLICK(bEnableClick);
		mElemShow[i] = ptrGOSInfo;
	}
	inittouches();
	return true;
}

void GoodsOrSkillCell::UpDateCell(map<int, GoodsOrSkillDatePtr> ptrDate, EventDelegate* ptrDelegate)
{
	this->mDelegate = ptrDelegate;

	string nameSS = "";
	string iconPathSS = "";
	string effectSS = "";
	bool isSkill = false;
	bool isVisibal = true;

	for (int i = 0; i < BATTLE_TABLEVIEW_COLUMN; i++)
	{
		if (i < (int)ptrDate.size())
		{
			mElemShow[i]->getEBG()->setVisible(true);
			int iId = ptrDate[i]->getEID();
			if (ptrDate[i]->getESHOWTYPE() == BSELECT_SHOW_GOODS)
			{
				GoodsItem* ptrGoods = trData->m_mGoodsItem[iId];
				nameSS = ptrGoods->strName;
				iconPathSS = history->GetGoodsImagePath(ptrGoods->iGoodsId);
				effectSS = ptrGoods->strUeid;
			}
			else
			{
				isSkill = true;
				SkillAttr* ptrSkill = &trData->skills[iId - 1];
				nameSS = ptrSkill->skill_name;
				iconPathSS = StringUtils::format("img/Skill/%d.png", ptrSkill->image_id+1);
				effectSS = ptrSkill->skill_effect;
			}
			mElemShow[i]->setEDATAINDEX(ptrDate[i]->getEINDEX());

			mElemShow[i]->setEENABLECLICK(ptrDate[i]->getEENABLECLICK());

			mElemShow[i]->getEGRAY()->setVisible(!ptrDate[i]->getEENABLECLICK());

			mElemShow[i]->getENAME()->setString(nameSS.c_str());

			mElemShow[i]->getEICON()->setTexture(GetSpriteTexture(iconPathSS.c_str()));

			mElemShow[i]->getEEFFECT()->setString(effectSS.c_str());

			mElemShow[i]->getCopyMark()->setVisible(ptrDate[i]->getIsCopy());

			if(mElemShow[i]->getENUM())
				mElemShow[i]->getENUM()->removeFromParent();

			int iNum = ptrDate[i]->getENUM(), iOffsetX = 0;

			if (iNum >= 100)
				iOffsetX = 5;
			else if (iNum >= 10 && iNum < 100)
				iOffsetX = 8;
			else if (iNum >= 0)
			{
				iOffsetX = 11;
			}

			Sprite* numSp = ITools::getInstance()->getNumSprite(iNum, true);
			numSp->setAnchorPoint(Vec2(1, 0.5));
			numSp->setPosition(Vec2(mElemShow[i]->getEBG()->getContentSize().width - iOffsetX, 36));
			mElemShow[i]->getEBG()->addChild(numSp);
			mElemShow[i]->setENUM(numSp);
		}
		else
		{
			mElemShow[i]->getEBG()->setVisible(false);
			mElemShow[i]->setEDATAINDEX(-1);
		}
	}

}

void GoodsOrSkillCell::onEnter()
{
	TableViewCell::onEnter();
}

void GoodsOrSkillCell::onExit()
{
	TableViewCell::onExit();
}

void GoodsOrSkillCell::inittouches()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* even)
	{
		mClickElemIndex = -1;
		for (int i = 0; i < (int)mElemShow.size(); i++)
		{
			Rect elemBox = mElemShow[i]->getEBG()->getBoundingBox();
			if (mElemShow[i]->getEBG()->isVisible()&& mElemShow[i]->getEENABLECLICK()&&Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(mElemShow[i]->getEBG()->convertToNodeSpace(touch->getLocation())))
			{
				mClickElemIndex = i;
				return true;
			}
		}
		return true;
	};

	listener->onTouchEnded = [&](Touch* touch, Event* even)
	{
		if (mClickElemIndex > -1 && mDelegate&& abs((int)(touch->getLocation().y - touch->getStartLocation().y))<5 || Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)
		{
			mDelegate->RunFunction(mElemShow[mClickElemIndex]->getEDATAINDEX());
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}

GoodsOrSkillCell::GoodsOrSkillCell()
{

}

GoodsOrSkillCell::~GoodsOrSkillCell()
{
	for (int i = 0; i < (int)mElemShow.size(); i++)
	{
		delete mElemShow[i];
	}
}