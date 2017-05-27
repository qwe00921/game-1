#include "SweepTableView.h"
#include "Defines.h"

SweepTableView* SweepTableView::create(Json::Value jsonResult, int iGoldNum)
{
	SweepTableView *pRet = new SweepTableView();
	if (pRet && pRet->init(jsonResult,iGoldNum))
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

bool SweepTableView::init(Json::Value jsonResult,int iGoldNum)
{
    if ( !Layer::init())
    {
        return false;
    }
	DSize size = ChessViewTool::getInstance()->getFullSize();
	GetElemDate(jsonResult, iGoldNum);
	mCellNum = elem_list.size();
	mCellWidth = 344;
	mCellHeight = 104;
	mCurElemIndex = 0;
	mTbActionInterval = 0.016f;
	mElemActionInterval = 0.12f;
	mRunActionInterval = mTbActionInterval * 5 + mElemActionInterval * 8;
	mTbRunActionNum = 0;

	mStopAllAction = false;
	mSweepMaskLayer = NULL;

	LayerColor* black_bg = LayerColor::create(Color4B(0,0,0,100));
	black_bg->setPosition(Vec2(0, 0));
	this->addChild(black_bg);

	Sprite* sdjg = ReadSpriteName("new_ui/img_global/heroz_txt2.png");
	sdjg->setPosition(Vec2(size.width/2, 350));
	this->addChild(sdjg);

	sureM = Menu::create();
	sureM->setPosition(Vec2(size.width/2, 65));
	this->addChild(sureM);
	sureM->setVisible(false);

	Sprite* sureNormalImage = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* sureTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	sureTXTN->setPosition(Vec2(40, 18));
	sureNormalImage->addChild(sureTXTN);

	Sprite* sureSelectImage = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* sureTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	sureTXTS->setPosition(Vec2(40, 18));
	sureSelectImage->addChild(sureTXTS);

	MenuItemSprite* ptrSureItem = MenuItemSprite::create(sureNormalImage, sureSelectImage, CC_CALLBACK_1(SweepTableView::clickSureMenu, this));
	sureM->addChild(ptrSureItem);
	sureSelectImage->setAnchorPoint(Vec2(0.5, 0.5));
	sureSelectImage->setPosition(Vec2(40, 18));





    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
	tableView = TableView::create(this, Size(mCellWidth, (mCellHeight*2)));
//GHB	tableView->setTouchPriority(LAYER_THIRD+1);
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
	tableView->setPosition(Vec2((size.width - mCellWidth)/2, 100));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();


	mSweepMaskLayer = SweepMaskLayer::create(this);
	mSweepMaskLayer->setPosition(Vec2(0,0));
	this->addChild(mSweepMaskLayer);

    return true;
}

SweepTableView::SweepTableView()
{

}

SweepTableView::~SweepTableView()
{

}

void SweepTableView::scrollViewDidScroll(ScrollView* view)
{

}

void SweepTableView::scrollViewDidZoom(ScrollView* view)
{

}

void SweepTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void SweepTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

}

void SweepTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

}

Size SweepTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(mCellWidth,mCellHeight);
}

TableViewCell* SweepTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	bool bEnableRun = idx >= (ssize_t)mCurElemIndex ? true : false;
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
		cell = SweepDateCell::create(elem_list[idx], bEnableRun);
    }
    else
    {
		SweepDateCell* c = (SweepDateCell*)cell;
		c->UpDateCell(elem_list[idx], bEnableRun);
    }
    return cell;
}

ssize_t SweepTableView::numberOfCellsInTableView(TableView *table)
{
	return mCellNum;
}

bool SweepTableView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	return true;
}
void SweepTableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}

void SweepTableView::clickSureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void SweepTableView::onExit()
{
	Layer::onExit();
}


void SweepTableView::onEnter()
{
	Layer::onEnter();
	this->schedule(schedule_selector(SweepTableView::TbOffsetRunAction), mRunActionInterval, mCellNum - 1, 0.2f);
}

void SweepTableView::GetElemDate(Json::Value jsonResult,int iGoldNum)
{
	if (!jsonResult.isArray())
		return;

	for (UInt i = 0; i < jsonResult.size(); i++)
	{
		SweepDateElem* ptrSDE = new SweepDateElem();
		ptrSDE->setINDEX(i + 1);
		ptrSDE->setGOLD(iGoldNum);
		Json::Value jsonGoods = jsonResult[i];
		for (UInt j = 0; j < jsonGoods.size(); j++)
		{
			if (jsonGoods[j]["goodID"].isInt() && jsonGoods[j]["count"].isInt())
				ptrSDE->GoodsList[jsonGoods[j]["goodID"].asInt()] = jsonGoods[j]["count"].asInt();
		}
		elem_list[i] = ptrSDE;
	}
}

void SweepTableView::TbOffsetRunAction(float f)
{
	mTbRunActionNum = 0;

	if (mStopAllAction)
	{
		this->unschedule(schedule_selector(SweepTableView::TbOffsetRunAction));
		mCurElemIndex = mCellNum;
		tableView->reloadData();
		if (mCellNum > 2)
		{
			Vec2 offsetPoint = tableView->getContentOffset();
			tableView->setContentOffset(offsetPoint + Vec2(0, (mCellHeight*(mCellNum-2))));
		}
		if (mSweepMaskLayer)
			mSweepMaskLayer->removeFromParent();
		mSweepMaskLayer = NULL;
		sureM->setVisible(true);
		return;
	}

	if (mCurElemIndex>1)
	{
		this->schedule(schedule_selector(SweepTableView::TBRunAction), mTbActionInterval, 4, 0);
	}
	else
	{
		TbElemRunAction();
	}
}

void SweepTableView::TBRunAction(float f)
{
	mTbRunActionNum++;
	Vec2 offsetPoint = tableView->getContentOffset();
	tableView->setContentOffset(offsetPoint + Vec2(0, mCellHeight / 5.0));
	if (mTbRunActionNum >= 5)
	{
		TbElemRunAction();
	}
}
void SweepTableView::TbElemRunAction()
{
	SweepDateCell* ptrSDCell = (SweepDateCell*)tableView->cellAtIndex(mCurElemIndex);
	if (ptrSDCell != NULL)
	{
		float fT = 0;
		ptrSDCell->title_bg->setVisible(true);
		for (int i = 0; i < (int)ptrSDCell->eGoodsList.size(); i++)
		{
			ptrSDCell->eGoodsList[i]->setScale(1.2f);
			float fDelayTime = (i + 1)*mElemActionInterval;
			DelayTime* ptrDelayTime = DelayTime::create(fDelayTime);
			CallFuncN* ptrCallFN = CallFuncN::create(CC_CALLBACK_1(SweepTableView::ElemActionCallF, this));
			ScaleTo* ptrScaleTo = ScaleTo::create(mElemActionInterval - 0.02f, 1.0f);
			Sequence* ptrSequence = Sequence::create(ptrDelayTime, ptrCallFN, ptrScaleTo, NULL);
			ptrSDCell->eGoodsList[i]->runAction(ptrSequence);
		}
	}

	mCurElemIndex++;
	if (mCurElemIndex >= mCellNum)
	{
		sureM->setVisible(true);
		if (mSweepMaskLayer)
			mSweepMaskLayer->removeFromParent();
		mSweepMaskLayer = NULL;
	}
}

void SweepTableView::ElemActionCallF(Node* sender)
{
	sender->setVisible(true);
}

void SweepTableView::setStopAction()
{
	mStopAllAction = true;
}

SweepDateCell* SweepDateCell::create(SweepDateElem* ptrSweepDate, bool bRunAction)
{
	SweepDateCell* pRet = new SweepDateCell();
	if (pRet&&pRet->init(ptrSweepDate,bRunAction))
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

bool SweepDateCell::init(SweepDateElem* ptrSweepDate, bool bRunAction)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	UpDateCell(ptrSweepDate, bRunAction);
	return true;
}

SweepDateCell::SweepDateCell()
{

}

void SweepDateCell::onEnter()
{
	TableViewCell::onEnter();
}
void SweepDateCell::onExit()
{
	TableViewCell::onExit();
}
bool SweepDateCell::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void SweepDateCell::UpDateCell(SweepDateElem* ptrSweepDate, bool bRunAction)
{
	this->removeAllChildren();
	title_bg = ReadSpriteName("new_ui/img_global/heroz_txtbg4.png");
	int title_width = title_bg->getContentSize().width;
	int title_height = title_bg->getContentSize().height;
	title_bg->setPosition(Vec2(172, 104 - title_height / 2));
	this->addChild(title_bg);
	title_bg->setVisible(!bRunAction);

	Label* sdcs = LabelSystemFont(__String::createWithFormat(LL("ddcsd"), ptrSweepDate->getINDEX())->getCString(), 18);
	sdcs->setAnchorPoint(Vec2(0, 0.5));
	sdcs->setPosition(Vec2(18, title_height / 2));
	title_bg->addChild(sdcs);

	Sprite* gold_bg = ReadSpriteName("new_ui/img_global/icon_gb.png");
	gold_bg->setPosition(Vec2(127, title_height / 2));
	title_bg->addChild(gold_bg);

	Label* gold_num = LabelSystemFont(IntToString_Sign(ptrSweepDate->getGOLD()), 18);
	gold_num->setColor(Color3B::YELLOW);
	gold_num->setAnchorPoint(Vec2(0, 0.5));
	gold_num->setPosition(Vec2(127 + gold_bg->getContentSize().width / 2, title_height / 2));
	title_bg->addChild(gold_num);

	Label* exp_bg = LabelSystemFont(LL("jinyanzengjia"), 18);
	exp_bg->setPosition(Vec2(gold_num ->getPositionX()+ gold_num->getContentSize().width/2+80, title_height / 2));
	title_bg->addChild(exp_bg);


	int m_exp = trData->m_mNetWarInfo[history->m_iCurWarId]->iExp;
	Label* exp_num = LabelSystemFont(IntToString_Sign(m_exp), 18);
	exp_num->setColor(Color3B::YELLOW);
	exp_num->setAnchorPoint(Vec2(0, 0.5));
	exp_num->setPosition(Vec2(exp_bg->getPositionX() + exp_bg->getContentSize().width / 2+5, title_height / 2));
	title_bg->addChild(exp_num);

	AddGoodsElem(ptrSweepDate, bRunAction);
}

void SweepDateCell::AddGoodsElem(SweepDateElem* ptrSweepDate, bool bRunAction)
{
	int iGoodsListIndex = 0;
	eGoodsList.clear();
	map<int, int> mGoodsList = ptrSweepDate->GoodsList;
	for (map<int, int>::iterator gIt = mGoodsList.begin(); gIt != mGoodsList.end(); gIt++)
	{
		int iGoodsId = gIt->first;
		int iCount = gIt->second;

		Sprite* eGoodsBg = ReadSpriteName("new_ui/img_global/item_box_bg.png");
		eGoodsBg->setPosition(Vec2(56 *iGoodsListIndex+32,35));
		this->addChild(eGoodsBg);
		eGoodsBg->setVisible(!bRunAction);
		eGoodsList[iGoodsListIndex++] = eGoodsBg;

		int bg_width = eGoodsBg->getContentSize().width;
		int bg_height = eGoodsBg->getContentSize().height;

		Sprite* img_goods = history->getGoodsSprite(iGoodsId);
		img_goods->setScale(0.8f);
		img_goods->setPosition(Vec2(bg_width / 2, bg_height / 2));
		eGoodsBg->addChild(img_goods);

		if (iGoodsId < 10000 && trData->m_mGoodsItem[iGoodsId]->iItId == 98)
		{
			Sprite* chip_img = ReadSpriteName("new_ui/img_global/icon_sp.png");
			chip_img->setAnchorPoint(Vec2(0, 1));
			chip_img->setScale(0.8f);
			chip_img->setPosition(Vec2(0, bg_height));
			eGoodsBg->addChild(chip_img);
		}

		if (iGoodsId<10000 && trData->m_mGoodsItem[iGoodsId]->iItId == 99)
		{
			Sprite* chip_img = ReadSpriteName("new_ui/hero/icon_soul.png");
			chip_img->setAnchorPoint(Vec2(0, 1));
			chip_img->setScale(0.8f);
			chip_img->setPosition(Vec2(0, bg_height));
			eGoodsBg->addChild(chip_img);
		}

		if (iCount > 1)
		{
			Sprite* numSp = ITools::getInstance()->getNumSprite(iCount, true);
			numSp->setAnchorPoint(Vec2(1, 0));
			numSp->setPosition(Vec2(bg_width, 0));
			eGoodsBg->addChild(numSp);
		}
	}
}

SweepMaskLayer* SweepMaskLayer::create(SweepTableView* ptrSweepTb)
{
	SweepMaskLayer *pRet = new SweepMaskLayer();
	if (pRet && pRet->init(ptrSweepTb))
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

bool SweepMaskLayer::init(SweepTableView* ptrSweepTb)
{
	if (!Layer::init())
	{
		return false;
	}

	mSweepTableView = ptrSweepTb;

	return true;
}

bool SweepMaskLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	mSweepTableView->setStopAction();
	return true;
}