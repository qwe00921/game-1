#include "DisPatchWaitTableView.h"
#include "Defines.h"
#include "ActorInfoLayer.h"

DisPatchWaitTableView* DisPatchWaitTableView::create(int cell_num,EventDelegate* ptrDelegate)
{
	DisPatchWaitTableView *pRet = new DisPatchWaitTableView();
	pRet->mDelegate = ptrDelegate;
	pRet->m_cell_num = cell_num;
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

bool DisPatchWaitTableView::init()
{
    if (!Layer::init() )
    {
        return false;
    }
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(530,212));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(50,118-8));
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

DisPatchWaitTableView::DisPatchWaitTableView():BTLayer(false)
{

}

DisPatchWaitTableView::~DisPatchWaitTableView()
{

}

void DisPatchWaitTableView::scrollViewDidScroll(ScrollView* view)
{

}

void DisPatchWaitTableView::scrollViewDidZoom(ScrollView* view)
{

}

void DisPatchWaitTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void DisPatchWaitTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void DisPatchWaitTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size DisPatchWaitTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(530,106);
}

TableViewCell* DisPatchWaitTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		DisPatchWaitCell* ptrCell = DisPatchWaitCell::create(mDelegate->GetElem(idx), idx);
		ptrCell->mDelegate = this->mDelegate;
		cell = ptrCell;
    }
    else
    {
		DisPatchWaitCell* c = (DisPatchWaitCell*)cell;
		c->mDelegate = this->mDelegate;
		c->UpDateCell(mDelegate->GetElem(idx), idx);
    }
    return cell;
}
void DisPatchWaitTableView::UpdateTableView()
{
	Vec2 p = tableView->getContentOffset();
	tableView->reloadData();
	tableView->setContentOffset(p);
}

ssize_t DisPatchWaitTableView::numberOfCellsInTableView(TableView *table)
{
	return m_cell_num;
}

DisPatchWaitCell* DisPatchWaitCell::create(map<int, map<int, int> > elem_slit, int iIndex)
{
	DisPatchWaitCell* pRet = new DisPatchWaitCell();
	if (pRet&&pRet->init(elem_slit,iIndex))
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

bool DisPatchWaitCell::init(map<int, map<int, int> > elem_slit, int iIndex)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	this->mDelegate = NULL;
	this->tbvDelegate = NULL;
	int m_wight = 106, m_height = 106;

	for (int i = 0; i < 5; i++)
	{
		Sprite* elem_bg = ReadSpriteName("new_ui/hero/herobox_bg1.png");
		elem_bg->setPosition(Vec2(m_wight / 2 + i*m_wight, m_height / 2));
		this->addChild(elem_bg);

		int iBgWidth = elem_bg->getContentSize().width;
		int iBgHeight = elem_bg->getContentSize().height;

		Sprite* elem_fashion = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_fashion->setPosition(Vec2(iBgWidth/2,57));
		elem_bg->addChild(elem_fashion,1);
		elem_fashion->setVisible(false);

		Sprite* elem_img = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_img->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_img,1);

		Label* elem_name = LabelSystemFont("", 16);
		elem_name->setPosition(Vec2(iBgWidth / 2, 21));
		elem_bg->addChild(elem_name,3);
		elem_name->enableOutline(Color4B::BLACK, 1);

		Label* elem_lv = LabelSystemFont("", 16);
		elem_lv->setAnchorPoint(Vec2(1, 1));
		elem_lv->setPosition(Vec2(83, 81));
		elem_bg->addChild(elem_lv,3);
		elem_lv->setColor(Color3B::YELLOW);
		elem_lv->enableOutline(Color4B::BLACK, 1);

		Sprite* elem_quality = ReadSpriteName("new_ui/hero/herobox_q1.png");
		elem_quality->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_quality);

		Sprite* elem_occupation = ReadSpriteName("new_ui/hero/job1.png");
		elem_occupation->setAnchorPoint(Vec2(0, 1));
		elem_occupation->setPosition(Vec2(4, iBgHeight - 5));
		elem_occupation->setScale(0.65f);
		elem_bg->addChild(elem_occupation,3);

		Sprite* elem_button = ReadSpriteName("new_ui/battle/war_cz1.png");
		elem_button->setPosition(Vec2(iBgWidth/2, iBgHeight / 2 - 5));
		elem_bg->addChild(elem_button,3);

		elem[i] = new DisPatchWaitInfo();
		elem[i]->setBG(elem_bg);
		elem[i]->setIMG(elem_img);
		elem[i]->setFASHION(elem_fashion);
		elem[i]->setBUTTON(elem_button);
		elem[i]->setTNAME(elem_name);
		elem[i]->setLV(elem_lv);
		elem[i]->setQuality(elem_quality);
		elem[i]->setOccupation(elem_occupation);
		elem[i]->setACTORID(-1);
		elem[i]->setIOUT(0);
		elem[i]->setFORCEOUT(0);
		elem[i]->setFORCEIN(0);
		elem[i]->setMINDEX(iIndex*5+i);
	}

	UpDateCell(elem_slit, iIndex);
	click_img_index = 0;
    initTouches();
	return true;
}

void DisPatchWaitCell::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        click_img_index = -1;

        for (int i = 0; i < 5; i++)
        {
            Rect bgBox = elem[i]->getBG()->getBoundingBox();
            Rect ButtonBox = elem[i]->getBUTTON()->getBoundingBox();
            if (elem[i]->getACTORID()>-1 && Rect(0, 0, bgBox.size.width, bgBox.size.height).containsPoint(elem[i]->getBG()->convertToNodeSpace(touch->getLocation())))
            {
                click_img_index = i;
				return true;
            }
        }
        return true;
    };
    listener->onTouchMoved = [&](Touch* touch, Event* event) {

    };
    listener->onTouchEnded = [&](Touch* touch, Event* event) {
        if (click_img_index > -1 && abs((int)(touch->getStartLocation().y - touch->getLocation().y)) < 4&&
            abs((int)(touch->getStartLocation().x - touch->getLocation().x)) < 6)
        {
			if (elem[click_img_index]->getFORCEIN())
			{
				ITools::getInstance()->createGlobalPrompt(LL("nowar"), true);
				playSound(SOUND_LEFT_MOUSE_CLICK);
			}
			else if (elem[click_img_index]->getFORCEOUT())
			{
				ITools::getInstance()->createGlobalPrompt(LL("yeswar"), false);
				playSound(SOUND_LEFT_MOUSE_CLICK);
			}
			else
			{
				if (mDelegate != NULL)
				{
					mDelegate->updateShow(elem[click_img_index]->getMINDEX(), elem[click_img_index]->getIOUT());
				}
			}
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

DisPatchWaitCell::DisPatchWaitCell()
{

}

void DisPatchWaitCell::UpDateCell(map<int, map<int, int> > elem_slit, int iIndex)
{
	int total_num = elem_slit.size();
	HistoryActorPtr ptrActor;
	int actor_id = -1;
	int is_out = 0;
	int force_in = 0;
	int force_out = 0;

	for (int i = 0; i < 5; i++)
	{
		actor_id = elem_slit[i][0];
		if (actor_id>-1)
		{
			force_out = elem_slit[i][1];
			is_out = elem_slit[i][2];
			force_in = elem_slit[i][3];

			ptrActor = &history->actors[elem_slit[i][0]];
			elem[i]->getBG()->setVisible(true);
			int fashion_width = 48;
			if (ptrActor->mFashion[0] <= 0)
			{
				elem[i]->getFASHION()->setVisible(false);
				Texture2D* texfa = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov"));
				elem[i]->getIMG()->setTexture(texfa);
				fashion_width = texfa->getContentSize().width;
			}
			else
			{
				elem[i]->getFASHION()->setVisible(true);
				__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

				Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
				fashion_width = tex->getContentSize().width;
				elem[i]->getFASHION()->setTexture(tex);
				elem[i]->getFASHION()->setTextureRect(Rect(0, 0, fashion_width, fashion_width));
				int zOrder = 0;
				if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[0])
					zOrder = 2;
				elem[i]->getFASHION()->setLocalZOrder(zOrder);

				Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
				elem[i]->getIMG()->setTexture(texfa);
				fashion_width = texfa->getContentSize().width;
			}

			elem[i]->getIMG()->setTextureRect(Rect(0, 0, fashion_width, fashion_width));
			
			elem[i]->getBUTTON()->setVisible(true);
			string battle_icon_path;
			if (force_out)
				battle_icon_path = "new_ui/battle/war_cz1.png";
			else if (force_in)
				battle_icon_path = "new_ui/battle/war_cz3.png";
			else if (is_out)
				battle_icon_path = "new_ui/battle/war_cz2.png";
			else
				elem[i]->getBUTTON()->setVisible(false);
			elem[i]->getBUTTON()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(battle_icon_path.c_str(), 0, 1)));

			elem[i]->getTNAME()->setString(trData->actors[ptrActor->actor_id].getActorName());
			elem[i]->getTNAME()->setColor(ITools::getInstance()->getActorPzColor(ptrActor->actor_quality));

			elem[i]->getOccupation()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId()).c_str())));

			__String* quality_icon_path = __String::createWithFormat("new_ui/hero/herobox_q%d.png", ptrActor->actor_quality);
			elem[i]->getQuality()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(quality_icon_path->getCString(), 0, 1)));

			elem[i]->getLV()->setString(IntToString(ptrActor->GetActorLv()));

			elem[i]->setACTORID(actor_id);
			elem[i]->setIOUT(is_out);
			elem[i]->setFORCEOUT(force_out);
			elem[i]->setFORCEIN(force_in);
			elem[i]->setMINDEX(iIndex * 5 + i);
		}
		else
		{
			elem[i]->getBG()->setVisible(false);
			elem[i]->setACTORID(actor_id);
			elem[i]->setIOUT(is_out);
			elem[i]->setFORCEOUT(0);
			elem[i]->setFORCEIN(0);
			elem[i]->setMINDEX(iIndex * 5 + i);
		}
	}
}

void DisPatchWaitCell::onEnter()
{
	TableViewCell::onEnter();
}

void DisPatchWaitCell::onExit()
{
	TableViewCell::onExit();
}