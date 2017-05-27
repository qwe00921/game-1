#include "SkillTableView.h"
#include "Defines.h"
#include "ActorInfoLayer.h"
#include "Net_HomePage.h"

SkillTableView* SkillTableView::create(HistoryActorPtr Actor,Layer* parent)
{
	SkillTableView *pRet = new SkillTableView();
	if (pRet && pRet->init(Actor, parent))
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

bool SkillTableView::init(HistoryActorPtr Actor, Layer* parent)
{
    if ( !Layer::init() )
    {
        return false;
    }
	this->curActor = Actor;
	this->ptrParent = parent;
	setSkillArr();
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(230,230));
//GHB	tableView->setTouchPriority(LAYER_SECOND - 4);
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(15,7));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    tableView->reloadData();
	
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, JINENG_HONGDIAN);
	
    
    return true;
}

SkillTableView::SkillTableView() :BTLayer(false)
{
	curActor = NULL;
}

SkillTableView::~SkillTableView()
{

}

void SkillTableView::scrollViewDidScroll(ScrollView* view)
{

}

void SkillTableView::scrollViewDidZoom(ScrollView* view)
{

}

void SkillTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void SkillTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void SkillTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size SkillTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(230,56);
}

TableViewCell* SkillTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		SkillInfoCell* c = SkillInfoCell::create(skill_elem, getIds(idx));
		c->upperLayer = ptrParent;
		cell = c;
		
    }
    else
    {
		SkillInfoCell* c = (SkillInfoCell*)cell;
		c->upperLayer = ptrParent;
		c->UpDateCell(getIds(idx));
    }
    return cell;
}

void SkillTableView::setSkillArr()
{

	int skill_elem_index = 0;

	int iSkillIndex = trData->actors[curActor->actor_id].skill;

	if (trData->net_hero_skill.count(iSkillIndex))
	{
		NetHeroSkillPtr ptrNHS = trData->net_hero_skill[iSkillIndex];
		for (map<int, int>::iterator mIt = ptrNHS->skill.begin(); mIt != ptrNHS->skill.end(); mIt++)
		{
			int iSkillId = mIt->first;
			if (trData->skill_num >= iSkillId)
			{
				bool isGray = curActor->CurSkillLevelIsNull(iSkillId);
				addSkillElem(skill_elem_index++, iSkillId, SKILL_ACTIVE, trData->skills[iSkillId - 1].image_id + 1, isGray);
			}
		}
	}
	//人物特技
	Seid* seid = NULL;
	if (curActor->actor_id < trData->actor_num)
	{
		map<int, int> mTHeroSeid = curActor->getDicHeroSeid(true);
		for (size_t i = 0; i < mTHeroSeid.size(); i++)
		{
			seid = trData->m_mSeid[mTHeroSeid[i]];
			if (!seid->seid || seid->type == 385) continue;

			bool isGray = false;
			if (seid->mOpenCondition[0] > 0 && !curActor->GetSeidIsOpen(seid->seid))
				isGray = true;

			addSkillElem(skill_elem_index++, seid->seid, SKILL_SPECIAL, seid->imgid, isGray);
		}
	}
}

void SkillTableView::addSkillElem(int index, int skillId, int SkillType, int iconId, bool is_gray)
{
	skill_elem[index] = new SkillInfo();
	skill_elem[index]->setSkillId(skillId);
	skill_elem[index]->setSkillType(SkillType);
	skill_elem[index]->setIconId(iconId);
	skill_elem[index]->setISGRAY(is_gray);
	if (SkillType == SKILL_ACTIVE)
		skill_elem[index]->setTypePath("new_ui/hero/skill_active.png");
	else if (SkillType == SKILL_PASSIVE)
		skill_elem[index]->setTypePath("new_ui/hero/skill_passive.png");
	else
		skill_elem[index]->setTypePath("new_ui/hero/skill_special.png");
}

ssize_t SkillTableView::numberOfCellsInTableView(TableView *table)
{
	int iCell = ceil(skill_elem.size() / 4.0);
	if (iCell < 4)
		iCell = 4;
	return iCell;
}

map<int, int> SkillTableView::getIds(int idx)
{
	map<int, int> ids;

	int row = ceil(skill_elem.size() / 4.0) - 1;
	int yu = skill_elem.size() % 4;

	if (row == idx && yu > 0)
	{
		for (int i = 0; i < yu; i++)
		{
			ids[i] = row * 4 + i + 1;
		}
	}
	else
	{
		ids[0] = idx * 4 + 1;
		ids[1] = idx * 4 + 2;
		ids[2] = idx * 4 + 3;
		ids[3] = idx * 4 + 4;
	}
	return ids;
}

SkillInfoCell* SkillInfoCell::create(map<int, SkillInfo*> skill_elem, map<int, int> ids)
{
	SkillInfoCell* pRet = new SkillInfoCell();
	pRet->totleelem = skill_elem;
	if (pRet&&pRet->init(ids))
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

bool SkillInfoCell::init(map<int, int> ids)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	CCLOG("INITCELL:%d,%d,%d,%d", ids[0], ids[1], ids[2], ids[3]);
	for (int i = 0; i < 4; i++)
	{
		Sprite* bg = ReadSpriteName("new_ui/img_global/item_box_bg.png");
		bg->setPosition(Vec2(25 + i * 59, 25));
		this->addChild(bg);
		Sprite* ptrIcon = NULL;
		Sprite* ptrType = NULL;
		int iSkillId = -1;
		int iSkillType = 0;
		if (ids[i] >0 && ids[i] <= (int)totleelem.size())
		{
			__String* ptrIconPath = NULL;
			if (totleelem[ids[i] - 1]->getSkillType() == SKILL_SPECIAL)
				ptrIconPath = __String::createWithFormat("img/Seid/%d.png", totleelem[ids[i] - 1]->getIconId());
			else
				ptrIconPath = __String::createWithFormat("img/Skill/%d.png", totleelem[ids[i] - 1]->getIconId());
			ptrIcon = ITools::getInstance()->getSpriteWithPath(ptrIconPath->getCString());
			if (totleelem[ids[i] - 1]->getISGRAY())
			{
				addSpriteGray(ptrIcon);
			}
			ptrIcon->setScale(0.81f);
			ptrIcon->setPosition(Vec2(25 + i * 59, 25));
			ptrType = ReadSpriteName(totleelem[ids[i] - 1]->getTypePath().c_str());
			ptrType->setPosition(Vec2(38 + i * 59, 12));
			iSkillId = totleelem[ids[i] - 1]->getSkillId();
			iSkillType = totleelem[ids[i] - 1]->getSkillType();
			if (totleelem[ids[i] - 1]->getSkillType() == SKILL_ACTIVE)
			{
				/*for (auto vk =HongDian::getInstence()->HongDianNode.begin(); vk != HongDian::getInstence()->HongDianNode.begin();vk++)
				{
					if (vk->first > 10000)
					{
						auto skilla = HongDian::getInstence()->HongDianNode;
						skilla.erase(vk);
					}
				}*/
				HongDian::getInstence()->addHongDianNode(ptrIcon, 10000 + iSkillId);
				
			}
		}
		else
		{
			ptrIcon = ITools::getInstance()->getSpriteWithPath("img/Skill/1.png");
			ptrIcon->setScale(0.81f);
			ptrIcon->setPosition(Vec2(25 + i * 59, 25));
			ptrIcon->setVisible(false);
			ptrType = ReadSpriteName("new_ui/hero/skill_active.png");
			ptrType->setPosition(Vec2(38 + i * 59, 12));
			ptrType->setVisible(false);
		}
		
		this->addChild(ptrIcon);
		this->addChild(ptrType);
		elem[i] = new ShowSkillInfo();
		elem[i]->setBG(bg);
		elem[i]->setICON(ptrIcon);
		elem[i]->setTYPE(ptrType);
		elem[i]->setPRESS(0);
		elem[i]->setSKILLID(iSkillId);
		elem[i]->setCASER(iSkillType);
	}
    initTouches();
	
	return true;
}
SkillInfoCell::SkillInfoCell()
{
	
}
SkillInfoCell::~SkillInfoCell()
{
	
}
void SkillInfoCell::onExit()
{
	TableViewCell::onExit();
}
void SkillInfoCell::onEnter()
{
	TableViewCell::onEnter();
}
void SkillInfoCell::UpDateCell(map<int, int> ids)
{
	for (int i = 0; i < 4; i++)
	{
		CCLOG("UPDATECELL:%d,%d,%d,%d", ids[0], ids[1], ids[2], ids[3]);
		int iSkillId = -1;
		int iSkillType = 0;
		if (ids[i]>0 && ids[i] <= (int)totleelem.size())
		{
			elem[i]->getICON()->setVisible(true);
			elem[i]->getTYPE()->setVisible(true);
			__String* ptrIconPath = NULL;
			if (totleelem[ids[i] - 1]->getSkillType() == SKILL_SPECIAL)
				ptrIconPath = __String::createWithFormat("img/Seid/%d.png", totleelem[ids[i] - 1]->getIconId());
			else
				ptrIconPath = __String::createWithFormat("img/Skill/%d.png", totleelem[ids[i] - 1]->getIconId());

			elem[i]->getICON()->setTexture(ITools::getInstance()->getTextureWithPath(ptrIconPath->getCString()));

			if (!totleelem[ids[i] - 1]->getISGRAY())
			{
				removeSpriteGray(elem[i]->getICON());
			}
			else
			{
				addSpriteGray(elem[i]->getICON());
			}

			elem[i]->getTYPE()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(totleelem[ids[i] - 1]->getTypePath().c_str())));
			iSkillId = totleelem[ids[i] - 1]->getSkillId();
			iSkillType = totleelem[ids[i] - 1]->getSkillType();
		}
		else
		{
			elem[i]->getICON()->setVisible(false);
			elem[i]->getTYPE()->setVisible(false);
		}
		elem[i]->setSKILLID(iSkillId);
		elem[i]->setCASER(iSkillType);
	}
}
void SkillInfoCell::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (((ActorInfoLayer*)upperLayer)->isSkilltbv() == false)
            return false;
        for (int i = 0; i < 4; i++)
        {
            Rect elemBox = elem[i]->getBG()->getBoundingBox();
            if (elem[i]->getSKILLID()>0 && Rect(2, 2, elemBox.size.width-2, elemBox.size.height-2).containsPoint(elem[i]->getBG()->convertToNodeSpace(touch->getLocation())))
            {
                ((ActorInfoLayer*)upperLayer)->AddSkillInfoLayer(elem[i]->getSKILLID(), elem[i]->getCASER());
                return true;
            }
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}