#include "DisPatchOutTableView.h"
#include "Defines.h"

DisPatchOutTableView* DisPatchOutTableView::create(EventDelegate* ptrDelegate,int iCellNum)
{
	DisPatchOutTableView *pRet = new DisPatchOutTableView();
	pRet->mDelegate = ptrDelegate;
	pRet->mCellNum = iCellNum;
	if (pRet && pRet->init())
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

bool DisPatchOutTableView::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	mActionId = -1;
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(512,80));
//GHB		tableView->setTouchPriority(LAYER_SECOND - 1);
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    //设置位置
    tableView->setPosition(Vec2(59,277));
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

DisPatchOutTableView::DisPatchOutTableView():BTLayer(false)
{

}

DisPatchOutTableView::~DisPatchOutTableView()
{

}

void DisPatchOutTableView::scrollViewDidScroll(ScrollView* view)
{

}

void DisPatchOutTableView::scrollViewDidZoom(ScrollView* view)
{

}

void DisPatchOutTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void DisPatchOutTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void DisPatchOutTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size DisPatchOutTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(64,80);
}

TableViewCell* DisPatchOutTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
		cell = DisPatchOutCell::create(mDelegate->GetElemInt(idx), mActionId);
    }
    else
    {
		DisPatchOutCell* c = (DisPatchOutCell*)cell;
		c->UpDateCell(mDelegate->GetElemInt(idx), mActionId);
    }
    return cell;
}
void DisPatchOutTableView::UpdateTableView(int iCellNum,int iActionId)
{
	mActionId = iActionId;
	Vec2 p = tableView->getContentOffset();
	int offset_x = (mCellNum - iCellNum) * 64 + p.x;
	mCellNum = iCellNum;

	if (offset_x > 0)
		offset_x = 0;
	tableView->reloadData();
	tableView->setContentOffset(Vec2(offset_x, 0));
	mActionId = -1;
}

ssize_t DisPatchOutTableView::numberOfCellsInTableView(TableView *table)
{
	return mCellNum;
}

DisPatchOutCell* DisPatchOutCell::create(int iActorId,int iActionId)
{
	DisPatchOutCell* pRet = new DisPatchOutCell();
	pRet->actor_id = iActorId;
	pRet->mActionId = iActionId;
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
bool DisPatchOutCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}
	int m_wight = 64,m_height = 80;
	img_index = 0;

	actor_wep = NULL;

	bg = ReadSpriteName("new_ui/battle/battle_bg2.png");
	bg->setAnchorPoint(Vec2(0,0.5));
	bg->setPosition(Vec2((m_wight - bg->getContentSize().width) / 2, bg->getContentSize().height));
	this->addChild(bg);

	int addFashion = 0;

	if (actor_id > -1)
	{
		removeSpriteGray(bg);

		HistoryActorPtr ptrActor = &history->actors[actor_id];

		if (ptrActor->mFashion[0] <= 0)
		{
			if (mActionId == actor_id)
			{
				actor_img = ITools::getInstance()->getSpriteWithPath(ptrActor->getActorSImgPath("mov"));
				actor_img->setTextureRect(Rect(0, 48*6, 48, 48));

				actor_wep = ITools::getInstance()->getSpriteWithPath(ptrActor->getActorSImgPath("spc"));
				actor_wep->setTextureRect(Rect(0, 48 * 4, 48, 48));
			}
			else
			{
				actor_img = ITools::getInstance()->getSpriteWithPath(ptrActor->getActorSImgPath("spc"));
				actor_img->setTextureRect(Rect(0, actor_img->getContentSize().width * 4, actor_img->getContentSize().width, actor_img->getContentSize().width));

				actor_wep = ReadSpriteName("new_ui/battle/battle_48.png");
			}
		}
		else
		{
			__String* img_path = NULL;
			__String* img_wep_path = NULL;
			int offset_y = 0;
			if (mActionId == actor_id)
			{
				addFashion = 1;
				img_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				img_wep_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				offset_y = 6;
			}
			else
			{
				addFashion = 2;
				img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				img_wep_path = NULL;
				offset_y = 4;
			}
			actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
			actor_img->setTextureRect(Rect(0, actor_img->getContentSize().width*offset_y, actor_img->getContentSize().width, actor_img->getContentSize().width));

			if (img_wep_path != NULL)
			{
				actor_wep = ITools::getInstance()->getSpriteWithPath(img_wep_path->getCString());
				actor_wep->setTextureRect(Rect(0, actor_wep->getContentSize().width*offset_y, actor_wep->getContentSize().width, actor_wep->getContentSize().width));
			}
			else
			{
				actor_wep = ReadSpriteName("new_ui/battle/battle_48.png");
			}
		}
	}
	else
	{
		addSpriteGray(bg);
		actor_img = ReadSpriteName("new_ui/battle/battle_48.png");
		actor_wep = ReadSpriteName("new_ui/battle/battle_48.png");
	}

	int img_zorder = 0;
	int img_wep = 0;

	actor_img->setAnchorPoint(Vec2(0.5, 0));
	actor_img->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
	bg->addChild(actor_img);

	actor_wep->setAnchorPoint(Vec2(0.5, 0.5));
	actor_wep->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2 + actor_img->getContentSize().height / 2));
	bg->addChild(actor_wep);
	actor_wep->setVisible(false);

	if (actor_id > -1 && mActionId == actor_id)
	{
		this->scheduleOnce(schedule_selector(DisPatchOutCell::runAction),0.2f);
	}

	return true;
}
DisPatchOutCell::DisPatchOutCell()
{

}
void DisPatchOutCell::UpDateCell(int iActorId,int iActionId)
{
	actor_img->setVisible(true);
	actor_wep->setVisible(false);
	this->unschedule(schedule_selector(DisPatchOutCell::runAction));
	mActionId = iActionId;
	actor_id = iActorId;
	if (actor_id > -1)
	{
		HistoryActorPtr ptrActor = &history->actors[actor_id];
		removeSpriteGray(bg);
		if (ptrActor->mFashion[0]<=0)
		{
			if (mActionId == actor_id)
			{
				actor_img->setTexture(ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov")));
				actor_img->setTextureRect(Rect(0, 48 * 6, 48, 48));

				actor_wep->setTexture(ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("spc")));
				actor_wep->setTextureRect(Rect(0, 48 * 4, 48, 48));
			}
			else
			{
				actor_img->setTexture(ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("spc")));
				actor_img->setTextureRect(Rect(0, actor_img->getContentSize().width * 4, actor_img->getContentSize().width, actor_img->getContentSize().width));

				actor_wep->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath("new_ui/battle/battle_48.png", 0, 1)));
			}
			if (mActionId == actor_id)
				this->scheduleOnce(schedule_selector(DisPatchOutCell::runAction), 0.2f);
		}
		else
		{
			__String* img_path = NULL;
			__String* img_wep_path = NULL;
			int offsetindex = 6;
			if (mActionId == actor_id)
			{
				img_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				img_wep_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
			}
			else
			{
				offsetindex = 4;
				img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				img_wep_path = NULL;
			}

			actor_img->setTexture(ITools::getInstance()->getTextureWithPath(img_path->getCString()));
			actor_img->setTextureRect(Rect(0, actor_img->getContentSize().width*offsetindex, actor_img->getContentSize().width, actor_img->getContentSize().width));

			if (img_wep_path==NULL)
			{
				actor_wep->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath("new_ui/battle/battle_48.png", 0, 1)));
			}
			else
			{
				actor_wep->setTexture(ITools::getInstance()->getTextureWithPath(img_wep_path->getCString()));
				int img_width = actor_wep->getContentSize().width;
				actor_wep->setTextureRect(Rect(0, img_width*4, img_width, img_width));
			}
			if (mActionId == actor_id)
				this->scheduleOnce(schedule_selector(DisPatchOutCell::runAction), 0.1f);
		}
	}
	else
	{
		addSpriteGray(bg);
		actor_img->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath("new_ui/battle/battle_48.png", 0, 1)));
		actor_wep->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath("new_ui/battle/battle_48.png", 0, 1)));
	}
}

void DisPatchOutCell::runAction(float fNum)
{
	actor_img->setVisible(false);
	actor_wep->setVisible(true);
}

void DisPatchOutCell::onExit()
{
	TableViewCell::onExit();
}
void DisPatchOutCell::onEnter()
{
	TableViewCell::onEnter();
}