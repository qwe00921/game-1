#include "ActorTableView.h"
#include "Defines.h"
#include "main.h"

ActorTableView* ActorTableView::create(HistoryActorPtr Actor, int iCellNum)
{
	ActorTableView *pRet = new ActorTableView();
	if (pRet && pRet->init(Actor, iCellNum))
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

bool ActorTableView::init(HistoryActorPtr Actor, int iCellNum)
{
    if ( !Layer::init() )
    {
        return false;
    }
	this->curActor = Actor;
	this->cell_num = iCellNum;
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(230,225));
//GHB	tableView->setTouchPriority(LAYER_SECOND - 4);
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(14,7));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();
    initTouches();
    
    return true;
}

void ActorTableView::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        return true;
    };
    listener->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

ActorTableView::ActorTableView():BTLayer(false)
{
	cell_num = 1;
	curActor = NULL;
}

ActorTableView::~ActorTableView()
{

}

void ActorTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ActorTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ActorTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ActorTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ActorTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ActorTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(225,358);
}

TableViewCell* ActorTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ActorInfoCell::create(curActor);
    }
    else
    {
		ActorInfoCell* c = (ActorInfoCell*)cell;
		c->UpDateCell(curActor);
    }
    return cell;
}
void ActorTableView::UpdateTableView(HistoryActorPtr ptrActor)
{
	ActorInfoCell* ptrCell  =  (ActorInfoCell*)tableView->cellAtIndex(0);
	ptrCell->UpDateCell(ptrActor);
}

ssize_t ActorTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ActorInfoCell* ActorInfoCell::create(HistoryActorPtr Actor)
{
	ActorInfoCell* pRet = new ActorInfoCell();
	if (pRet&&pRet->init(Actor))
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
void ActorInfoCell::PropertyElem(ShowActorInfo* ptrShow, const char* ptrPro, int iBase, int iGrown, int iDebuff, int x, int y)
{
	Label* txt1 = LabelSystemFont(ptrPro, 16);
	txt1->setAnchorPoint(Vec2(0, 1));
	txt1->setPosition(Vec2(2+x, y));
	this->addChild(txt1);
	ptrShow->setLAB(txt1);

	Label* txt2 = LabelSystemFont(IntToString(iBase), 16);
	txt2->setAnchorPoint(Vec2(0, 1));
	txt2->setPosition(Vec2(40+x, y));
	this->addChild(txt2);
	ptrShow->setPOW(txt2);

	int offset = 81+x;

	Label* txt4 = LabelSystemFont(IntToString_Sign(iDebuff), 16);
	if (iDebuff > 0)
		txt4->setColor(Color3B(0, 255, 255));
	else
		txt4->setColor(Color3B::MAGENTA);
	txt4->setAnchorPoint(Vec2(0, 1));
	txt4->setPosition(Vec2(offset, y));
	txt4->setVisible(false);
	this->addChild(txt4);
	ptrShow->setBUFF(txt4);

	offset = x + 176;
	Label* grownLab = LabelSystemFont("1", 16);
	grownLab->setColor(Color3B::YELLOW);
	grownLab->setAnchorPoint(Vec2(0, 1));
	grownLab->setPosition(Vec2(offset, y));
	grownLab->setVisible(false);
	this->addChild(grownLab);
	ptrShow->setGROWN(grownLab);
}
void ActorInfoCell::PropertyElemGrow(ShowActorInfo* ptrShow, const char* ptrPro, int iBase,int x, int y)
{
	Label* txt1 = LabelSystemFont(ptrPro, 16);
	txt1->setAnchorPoint(Vec2(0, 1));
	txt1->setPosition(Vec2(2 + x, y));
	this->addChild(txt1);
	ptrShow->setLAB(txt1);

	Label* txt2 = LabelSystemFont(IntToString(iBase), 16);
	txt2->setAnchorPoint(Vec2(0, 1));
	txt2->setPosition(Vec2(40 + x, y));
	this->addChild(txt2);
	ptrShow->setPOW(txt2);
	ptrShow->setBUFF(NULL);
}
bool ActorInfoCell::init(HistoryActorPtr Actor)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	int offset_y = 78;
	//武力
	elem[6] = new ShowActorInfo();
	PropertyElemGrow(elem[6], LL("wuli"), 0,5, 278 + offset_y);

	//智力
	elem[7] = new ShowActorInfo();
	PropertyElemGrow(elem[7], LL("zhili"), 0,5, 258 + offset_y);

	//统帅
	elem[8] = new ShowActorInfo();
	PropertyElemGrow(elem[8], LL("tongshuai"), 0, 5, 238 + offset_y);

	//敏捷
	elem[9] = new ShowActorInfo();
	PropertyElemGrow(elem[9], LL("minjie"), 80, 5, 218 + offset_y);

	//运气
	elem[10] = new ShowActorInfo();
	PropertyElemGrow(elem[10], LL("yunqi"), 80, 5, 198 + offset_y);

	Sprite* line = ReadSpriteName("new_ui/img_global/line_1.png");
	line->setPosition(Vec2(115, 176 + offset_y));
	this->addChild(line);

	//攻击力
	elem[0] = new ShowActorInfo();
	PropertyElem(elem[0], LL("gongji"), 0, 0, 0, 5, 171 + offset_y);

	//精神力
	elem[1] = new ShowActorInfo();
	PropertyElem(elem[1], LL("jingshen"), 0, 0, 0, 5, 151 + offset_y);

	//防御力
	elem[2] = new ShowActorInfo();
	PropertyElem(elem[2], LL("fangyu"), 0, 0, 0, 5, 131 + offset_y);

	//爆发力
	elem[3] = new ShowActorInfo();
	PropertyElem(elem[3], LL("baofa"), 0, 0, 0, 5, 111 + offset_y);

	//士气
	elem[4] = new ShowActorInfo();
	PropertyElem(elem[4], LL("shiqi"), 0, 0, 0, 5, 91 + offset_y);

	//移动力
	elem[5] = new ShowActorInfo();
	PropertyElem(elem[5], LL("yidong"), 0, 0, 0, 5, 71 + offset_y);

	Sprite* line1 = ReadSpriteName("new_ui/img_global/line_1.png");
	line1->setPosition(Vec2(115, 49 + offset_y));
	this->addChild(line1);

	//武将简介
	introduction = LabelSystemFont("", 16);
	introduction->setDimensions(220,122);
	introduction->setAnchorPoint(Vec2(0, 1));
	introduction->setPosition(Vec2(5, 44 + offset_y));
	introduction->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(introduction);

	//附魔效果
	Sprite* spEnchN = ReadSpriteName("new_ui/img_global/icon_fm3.png");
	Sprite* spEnchS = ReadSpriteName("new_ui/img_global/icon_fm3.png");
	spEnchS->setScale(0.90f);
	m_btnEnchant = MenuItemSprite::create(spEnchN, spEnchS, CC_CALLBACK_1(ActorInfoCell::callBackEnchant, this));
	spEnchS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spEnchS->setPosition(Vec2(spEnchS->getContentSize().width / 2, spEnchS->getContentSize().height / 2));
	Menu* ptrMenu = Menu::create(m_btnEnchant, nullptr);
	ptrMenu->setPosition(Vec2(196, 235 + offset_y));
	this->addChild(ptrMenu);

	UpDateCell(Actor);

	return true;
}
ActorInfoCell::ActorInfoCell()
{
}
void ActorInfoCell::UpDateCell(HistoryActorPtr Actor)
{
	m_ptrActor = Actor;
	int power[11][3] = { 0 };
	map<int, float> PowerGrown;
	for (int i = 0; i < 5; i++)
	{
		PowerGrown[i] = Actor->getGrownValue(i);
	}

	int cur_num = 0, add_num = 0;

	//攻击力
	cur_num = Actor->getRealAbility(ACTOR_ABILITY_POWER);
	if (Actor->state_power)
	{
		float iBuffEffect = Actor->getFiveAbilityBuffEffect(Actor->state_power, ACTOR_BUFF_POWUP);
		add_num = cur_num * iBuffEffect / 100;
	}
	power[0][0] = cur_num;
	power[0][1] = add_num;
	//精神力
	add_num = 0;
	cur_num = Actor->getRealAbility(ACTOR_ABILITY_INTELLEGENCE);
	if(Actor->state_intellegence)
	{
		float iBuffEffect = Actor->getFiveAbilityBuffEffect(Actor->state_intellegence, ACTOR_BUFF_INTUP);
		add_num = cur_num * iBuffEffect / 100;
	}
	power[1][0] = cur_num;
	power[1][1] = add_num;
	//防御
	add_num = 0;
	cur_num = Actor->getRealAbility(ACTOR_ABILITY_DEFENCE);
	if (Actor->state_defence)
	{
		float iBuffEffect = Actor->getFiveAbilityBuffEffect(Actor->state_defence, ACTOR_BUFF_DEFUP);
		add_num = cur_num * iBuffEffect / 100;
	}
	power[2][0] = cur_num;
	power[2][1] = add_num;
	//爆发力
	add_num = 0;
	cur_num = Actor->getRealAbility(ACTOR_ABILITY_EXPLOSION);
	if (Actor->state_explosion)
	{
		float iBuffEffect = Actor->getFiveAbilityBuffEffect(Actor->state_explosion, ACTOR_BUFF_EXPUP);
		add_num = cur_num * iBuffEffect / 100;
	}
	power[3][0] = cur_num;
	power[3][1] = add_num;
	//士气
	add_num = 0;
	cur_num = Actor->getRealAbility(ACTOR_ABILITY_FORTUNE);
	if (Actor->state_fortune)
	{
		float iBuffEffect = Actor->getFiveAbilityBuffEffect(Actor->state_fortune, ACTOR_BUFF_FORTUP);
		add_num = cur_num * iBuffEffect / 100;
	}
	power[4][0] = cur_num;
	power[4][1] = add_num;
	//移动
	power[5][0] = trData->occupations[Actor->occupation_id].base_speed+Actor->GetGoodsAddSpeed();
	power[5][1] = Actor->GetActorSpeed(2);

	OccupationAttrPtr occu = &trData->occupations[Actor->occupation_id];
	int is_double = 1 + IS_HALF_BASEABILITY;
	//武力
	power[6][0] = Actor->GetBasepower()*is_double;

	//智力
	power[7][0] = Actor->GetBaseIntellegence()*is_double;

	//统帅
	power[8][0] = Actor->GetBaseDefence()*is_double;

	//敏捷
	power[9][0] = Actor->GetBaseExplosion()*is_double;

	//士气
	power[10][0] = Actor->GetBaseFortune()*is_double;

	for (int i = 0; i < 11; i++)
	{
		elem[i]->getPOW()->setString(IntToString(power[i][0]));
		if (i < 6)
		{
			elem[i]->getBUFF()->setString(IntToString_Sign(power[i][1]));
			elem[i]->getBUFF()->setVisible((power[i][1] != 0 ? true : false));
			if (power[i][1]>0)
			{
				elem[i]->getBUFF()->setColor(Color3B(0,255,255));
			}
			else
			{
				elem[i]->getBUFF()->setColor(Color3B::MAGENTA);
			}
			if (PowerGrown.count(i))
			{
				elem[i]->getGROWN()->setVisible(true);
				elem[i]->getGROWN()->setString(__String::createWithFormat("(%.1f)", PowerGrown[i])->getCString());
			}
			else
			{
				elem[i]->getGROWN()->setVisible(false);
			}
		}
	}
	introduction->setString(trData->actors[Actor->actor_id].actor_info);
	m_btnEnchant->setVisible(Actor->m_mStoneSeid.size() != 0);
}

void ActorInfoCell::callBackEnchant(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(EnchantInforLayer::create(m_ptrActor));
}

////附魔信息/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EnchantInforLayer* EnchantInforLayer::create(HistoryActor* ptrActor)
{
	EnchantInforLayer* pRet = new EnchantInforLayer();
	if(pRet&&pRet->init(ptrActor))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool EnchantInforLayer::init(HistoryActor* ptrActor)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrActor = ptrActor;

	initControl();
	return true;
}

void EnchantInforLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spInfoBG = ReadSpriteName("new_ui/img_global/icon_fm2.png");
	spInfoBG->setPosition(point);
	this->addChild(spInfoBG);

	//套装属性
	map<int, int> mSeidId = m_ptrActor->m_mStoneSeid;
	for (int i = 0; i < (int)mSeidId.size(); i++)
	{
		string strInfo = trData->m_mSeid[mSeidId[i]]->Info;
		auto labInfo = LabelSystemFont(strInfo, 16);
		labInfo->setPosition(point + Vec2(0, 66 - i * 27));
		labInfo->enableOutline(Color4B(158, 116, 46, 255), 1);
		this->addChild(labInfo);
	}

	map<int, map<int, int> > mTExclGemSeid = m_ptrActor->getExclusiveGemEffect();
	if (mTExclGemSeid.size())
	{
		map<int, map<int,int> > mTHSeid;

		for (map<int, int>::iterator eIt = mTExclGemSeid[0].begin(); eIt != mTExclGemSeid[0].end(); eIt++)
		{
			if (mTExclGemSeid[1].count(eIt->second))
			{
				mTHSeid[eIt->second][0] = eIt->first;
				mTHSeid[eIt->second][1] = mTExclGemSeid[1][eIt->second];
			}

		}

		int iOffsetNum = mSeidId.size();
		for (map<int, map<int, int> >::iterator eIt = mTHSeid.begin(); eIt != mTHSeid.end(); eIt++)
		{
			string strInfo = StringUtils::format("%s %s %s", trData->m_mSeid[eIt->second[0]]->sename, LL("jinjie"), trData->m_mSeid[eIt->second[1]]->sename);
			auto labInfo = LabelSystemFont(strInfo, 16);
			labInfo->setPosition(point + Vec2(0, 66 - iOffsetNum * 27));
			labInfo->enableOutline(Color4B(158, 116, 46, 255), 1);
			this->addChild(labInfo);
			iOffsetNum++;
		}

		for (map<int, int>::iterator eIt = mTExclGemSeid[2].begin(); eIt != mTExclGemSeid[2].end(); eIt++)
		{
			auto labInfo = LabelSystemFont(trData->m_mSeid[eIt->second]->Info, 16);
			labInfo->setPosition(point + Vec2(0, 66 - iOffsetNum * 27));
			labInfo->enableOutline(Color4B(158, 116, 46, 255), 1);
			this->addChild(labInfo);
			iOffsetNum++;
		}
	}
	

	//关闭
	MenuItem* btnClose = MenuItem::create(CC_CALLBACK_1(EnchantInforLayer::callBackClose, this));
	btnClose->setContentSize(size);
	btnClose->setPosition(point);
	auto ptrMenu = Menu::create(btnClose, NULL);
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);
}

void EnchantInforLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}
