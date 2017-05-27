//
//  PVPRankLayer.cpp
//  TRGame
//
//  Created by zwy on 17/03/31.
//
//

#include "PVPRankLayer.h"
#include "PVPRewardLayer.h"
#include "PVPData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPRankTableView* PVPRankTableView::create(Json::Value JsonValue)
{
	PVPRankTableView* pRet = new PVPRankTableView();
	if (pRet&&pRet->init(JsonValue))
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

bool PVPRankTableView::init(Json::Value JsonValue)
{
	if (!Layer::init())
	{
		return false;
	}

	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	mScore = 0;
	mRank = 0;
	getElemData(JsonValue);
	this->cell_num = mElemData.size();
	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
	tableView = TableView::create(this, Size(430 , 240 ));
	//设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	tableView->setPosition(point + Vec2(-260, -143));
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
void PVPRankTableView::getElemData(Json::Value JsonValue)
{
	Json::Value JsonRank = JsonValue["ranks"];
	if (JsonRank.isArray())
	{
		string heroNameS = history->m_pPlayerInfo->getName();
		int iGender = 0;
		map<int, int> meFashion;
		for (UInt i = 0; i < JsonRank.size(); i++)
		{
			meFashion.clear();
			int iGender = 0, iFashion1 = 0, iFashion2 = 0, iFashion3 = 0, iFashion4 = 0, iRank = 0, iScore = 0, iIsVip = 0, iLevel;
			string nameS = "";

			ParseInt(iGender, JsonRank[i]["gender"]);
			ParseInt(iFashion1, JsonRank[i]["Fashion1"]);
			ParseInt(iFashion2, JsonRank[i]["Fashion2"]);
			ParseInt(iFashion3, JsonRank[i]["Fashion3"]);
			ParseInt(iFashion4, JsonRank[i]["Fashion4"]);
			ParseInt(iRank, JsonRank[i]["rank"]);
			ParseInt(iScore, JsonRank[i]["winNum"]);
			ParseInt(iIsVip, JsonRank[i]["isVip"]);
			ParseInt(iLevel, JsonRank[i]["lv"]);
			ParseString(nameS, JsonRank[i]["name"]);

			meFashion[1] = iGender + 1;
			meFashion[2] = iFashion1;
			meFashion[3] = 100;
			meFashion[4] = iFashion2;
			if (iFashion3 > 0)
				meFashion[2] = iFashion3;
			if (iFashion4 > 0)
				meFashion[4] = iFashion4;
			PVPRankData* ptrPVPRankData = new PVPRankData();
			ptrPVPRankData->seteLEVEL(iLevel);
			ptrPVPRankData->seteNAME(nameS.c_str());
			ptrPVPRankData->seteRANK(iRank);
			ptrPVPRankData->seteSCORE(iScore);
			ptrPVPRankData->seteISVIP(iIsVip);
			ptrPVPRankData->mFashion = meFashion;

			mElemData[i] = ptrPVPRankData;
		}
	}
	if (JsonValue["myScore"].isInt())
	{
		mScore = JsonValue["myScore"].asInt();
	}
	if (JsonValue["myRank"].isInt())
	{
		mRank = JsonValue["myRank"].asInt();
	}
}

PVPRankData* PVPRankTableView::GetCellData(int iDx)
{
	return mElemData[iDx];
}

void PVPRankTableView::scrollViewDidScroll(ScrollView* view)
{

}

void PVPRankTableView::scrollViewDidZoom(ScrollView* view)
{

}

void PVPRankTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void PVPRankTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void PVPRankTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size PVPRankTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(430, 62);
}

TableViewCell* PVPRankTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();

	if (!cell) {
		cell = PVPRankCell::create(GetCellData(idx));
	}
	else
	{
		PVPRankCell* c = (PVPRankCell*)cell;
		c->UpDateCell(GetCellData(idx));
	}
	return cell;
}

ssize_t PVPRankTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}
/////////////////////////////////////////////////////////////
PVPRankCell* PVPRankCell::create(PVPRankData* ptrLRData)
{
	PVPRankCell* pRet = new PVPRankCell();
	if (pRet&&pRet->init(ptrLRData))
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

bool PVPRankCell::init(PVPRankData* ptrLRData)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	m_iCellWidth = 430;
	m_iCellHeight = 62;
	mNumSp = NULL;
	bg = ReadSpriteName("new_ui/img_global/global_ph_bg2.png");
	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;
	bg->setPosition(Vec2(m_iCellWidth / 2, m_iCellHeight / 2));
	this->addChild(bg);

	GetNumSprite(bg, ptrLRData->geteRANK());
	//78,王族
	vipSp = ReadSpriteName("new_ui/main/vip1.png");
	vipSp->setPosition(Vec2(78, bg->getContentSize().height / 2));
	bg->addChild(vipSp);

	int iFashion1 = ptrLRData->mFashion[2];
	int iFashion2 = ptrLRData->mFashion[4];
	int iGender = ptrLRData->mFashion[1];

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	int iOrder = trData->net_hero_dress[iFashion2]->mMov[6];

	spEquip = ReadSpriteNormal(strEquip->getCString());
	Size weaponSize = spEquip->getContentSize();

	spEquip->setPosition(Vec2(130, bg->getContentSize().height / 2));
	spEquip->setTextureRect(Rect(0, weaponSize.width * 6, weaponSize.width, weaponSize.width));
	bg->addChild(spEquip, iOrder);

	spHero = ReadSpriteNormal(strHero->getCString());
	Size heroSize = spHero->getContentSize();
	spHero->setPosition(spEquip->getPosition());
	spHero->setTextureRect(Rect(0, heroSize.width * 6, heroSize.width, heroSize.width));
	bg->addChild(spHero, 1);
	//姓名,等级
	nameLab = LabelSystemFont(ptrLRData->geteNAME().c_str(), 16);
	nameLab->setColor(Color3B(102, 0, 0));
	nameLab->setAnchorPoint(Vec2(0, 0));
	nameLab->setPosition(Vec2(158, bg->getContentSize().height / 2));
	bg->addChild(nameLab);
	levelLab = LabelSystemFont(StringUtils::format("%s%d", LL("dengji"), ptrLRData->geteLEVEL()).c_str(), 16);
	levelLab->setColor(Color3B(102, 51, 0));
	levelLab->setAnchorPoint(Vec2(0, 1));
	levelLab->setPosition(Vec2(158, bg->getContentSize().height / 2));
	bg->addChild(levelLab);
	//分数
	scoreLab = LabelSystemFont(IntToString(ptrLRData->geteSCORE()), 16);
	scoreLab->setPosition(Vec2(366, bg->getContentSize().height / 2));
	scoreLab->setColor(Color3B(102, 0, 0));
	bg->addChild(scoreLab);
	UpDateCell(ptrLRData);
	return true;
}
PVPRankCell::PVPRankCell()
{

}

void PVPRankCell::GetNumSprite(Sprite* bgSp, int iNum)
{
	if (mNumSp)
		mNumSp->removeFromParent();
	mNumSp = NULL;
	int iNumWidth = 0;
	if (iNum > 0 && iNum < 4)
	{
		string rankS = StringUtils::format("new_ui/img_global/global_ph_icon%d.png", iNum);
		mNumSp = ReadSpriteName(rankS.c_str());
	}
	else
	{
		map<int, int> numarr;
		int iNumIndex = 0;
		do
		{
			numarr[iNumIndex++] = iNum % 10;
			iNum = iNum / 10;

		} while (iNum > 0);

		for (int i = iNumIndex - 1; i >= 0; i--)
		{
			if (!mNumSp)
				mNumSp = ReadSpriteName(StringUtils::format("new_ui/img_number/num10_%d.png", numarr[i]).c_str());
			else
			{
				Sprite* numSp = ReadSpriteName(StringUtils::format("new_ui/img_number/num10_%d.png", numarr[i]).c_str());
				iNumWidth += numSp->getContentSize().width;
				numSp->setAnchorPoint(Vec2(0, 0));
				numSp->setPosition(Vec2(iNumWidth, 0));
				mNumSp->addChild(numSp);
			}
		}
	}
	int iNumPX = 35 - (mNumSp->getContentSize().width + iNumWidth) / 2;
	mNumSp->setAnchorPoint(Vec2(0, 0.5));
	mNumSp->setPosition(Vec2(iNumPX, bgSp->getContentSize().height / 2));
	bgSp->addChild(mNumSp);
}

void PVPRankCell::UpDateCell(PVPRankData* ptrLRData)
{
	GetNumSprite(bg, ptrLRData->geteRANK());

	int iFashion1 = ptrLRData->mFashion[2];
	int iFashion2 = ptrLRData->mFashion[4];
	int iGender = ptrLRData->mFashion[1];

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	int iOrder = 0;
	if (trData->net_hero_dress[iFashion2]->mMov[6])
	{
		iOrder = 2;
	}
	Texture2D* ptrTexture2D = GetSpriteTexture(strEquip->getCString());
	Size weaponSize = ptrTexture2D->getContentSize();
	spEquip->setTexture(ptrTexture2D);
	spEquip->setTextureRect(Rect(0, weaponSize.width * 6, weaponSize.width, weaponSize.width));
	spEquip->setLocalZOrder(iOrder);

	ptrTexture2D = GetSpriteTexture(strHero->getCString());
	Size heroSize = ptrTexture2D->getContentSize();
	spHero->setTexture(ptrTexture2D);
	spHero->setTextureRect(Rect(0, heroSize.width * 6, heroSize.width, heroSize.width));

	nameLab->setString(ptrLRData->geteNAME().c_str());

	levelLab->setString(StringUtils::format("%s%d", LL("dengji"), ptrLRData->geteLEVEL()).c_str());

	scoreLab->setString(IntToString(ptrLRData->geteSCORE()));

	if (ptrLRData->geteISVIP())
		vipSp->setVisible(true);
	else
		vipSp->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PVPRankLayer::PVPRankLayer()
{

}

PVPRankLayer::~PVPRankLayer()
{

}

PVPRankLayer* PVPRankLayer::create(Json::Value JsonVal)
{
	PVPRankLayer* pRet = new PVPRankLayer();
	if(pRet&&pRet->init(JsonVal))
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

bool PVPRankLayer::init(Json::Value JsonVal)
{
	if(!Layer::init())
	{
		return false;
	}

	m_JsonVal = JsonVal;
	initControl();

	return true;
}

void PVPRankLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
    auto spBG = ReadSpriteName("new_ui/pvp/pvp_bg4.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	auto spTitle = ReadSpriteName("new_ui/pvp/pvp_txt3.png");
	spTitle->setPosition(point + Vec2(0, 152));
	this->addChild(spTitle);

	//标签
	auto spMark = ReadSpriteName("new_ui/pvp/pvp_txt99.png");
	spMark->setPosition(point + Vec2(0, 118));
	this->addChild(spMark);

	//我的排名
	Label* labMark = LabelSystemFont(LL("wodepaiming"), 16);
	labMark->setAnchorPoint(Vec2(0.0f, 0.5f));
	labMark->setPosition(point + Vec2(-215, -153));
	labMark->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(labMark);

	//排名
	auto labRank = LabelSystemFont(StringUtils::format("%d", PVPData::getInstance()->getRandID()), 16);
	labRank->setAnchorPoint(Vec2(0.0f, 0.5f));
	labRank->setPosition(labMark->getPosition() + Vec2(labMark->getContentSize().width + 10, 0));
	labRank->setColor(Color3B::YELLOW);
	labRank->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(labRank);

	//排行榜
	spBG->addChild(PVPRankTableView::create(m_JsonVal), 2);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	spBG->addChild(ptrMenu,100);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PVPRankLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(230, 155));
	ptrMenu->addChild(btnClose,10);
}

void PVPRankLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}