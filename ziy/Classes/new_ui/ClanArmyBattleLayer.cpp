#include "ClanArmyBattleLayer.h"
#include "Defines.h"
#include "ClanArmyBattleTableView.h"
#include "PopLayer.h"

ClanArmyBattleLayer* ClanArmyBattleLayer::create(Json::Value vArmyData, EventDelegate* ptrDelegate,int iSelfJob, string guildIdS)
{
	ClanArmyBattleLayer* pRet = new ClanArmyBattleLayer();
	if(pRet&&pRet->init(vArmyData,ptrDelegate,iSelfJob,guildIdS))
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

bool ClanArmyBattleLayer::init(Json::Value vArmyData, EventDelegate* ptrDelegate,int iSelfJob, string guildIdS)
{
	if (!Layer::init())
	{
		return false;
	}
	mGuildIdS = guildIdS;
	mSelfJob = iSelfJob;
	mDelegate = ptrDelegate;
	m_bEnableUpdataFatherShow = false;

	GetBattleData(vArmyData);
	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg8.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2 - 10));
	this->addChild(bg);

	Sprite* bgInfo = ReadSpriteName("new_ui/gh/gh_tibox2.png");
	bgInfo->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 5));
	bg->addChild(bgInfo);

	bg->addChild(ClanArmyBattleTableView::create(&mArmyBattleData,this,mGuildIdS));

	Label* infoLab = LabelSystemFont(LL("kqdjtmtktfyc"), 16);
	infoLab->setPosition(Vec2(bg->getContentSize().width / 2, 28));
	infoLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	bg->addChild(infoLab);

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(580, 358));
	bg->addChild(mCloseMenu, 2);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ClanArmyBattleLayer::clickCloseMenu, this));
	mCloseMenu->addChild(close_menu);

	return true;
}

void ClanArmyBattleLayer::GetBattleData(Json::Value vArmyData)
{
	int iABIndex = 0;
	map<int, int> mArmyBattleIdList;
	if (trData->map_total_war.count(MAPID_SCENE_CLANARMYBATTLE))
	{
		mArmyBattleIdList = trData->map_total_war[MAPID_SCENE_CLANARMYBATTLE];
	}

	map<int, int> mWarJsonIndex;
	Json::Value vGuildWarArr = vArmyData["guildWarArr"];
	if (vGuildWarArr.isArray())
	{
		int iWarId = 0;
		for (UInt i = 0; i < vGuildWarArr.size(); i++)
		{
			iWarId = vGuildWarArr[i]["warID"].asInt();
			mWarJsonIndex[iWarId] = i;
		}
	}

	NetWarInfo* ptrNetWarInfo = NULL;
	int iIsOpen = 0,iBossCurHp = 0,iJsonIndex = 0,iGoodsId = 0,iCount = 0;
	for (map<int, int>::iterator eIt = mArmyBattleIdList.begin(); eIt != mArmyBattleIdList.end(); eIt++)
	{
		ptrNetWarInfo = trData->m_mNetWarInfo[eIt->first];

		mArmyBattleData[iABIndex] = new ClanArmyBattleElem();
		mArmyBattleData[iABIndex]->setEiWarId(eIt->first);
		mArmyBattleData[iABIndex]->setEiIndex(iABIndex);
		mArmyBattleData[iABIndex]->setEiBgImg(ptrNetWarInfo->iImgId);
		mArmyBattleData[iABIndex]->setEiSelfJob(mSelfJob);
		mArmyBattleData[iABIndex]->setEiBossMaxHp(ptrNetWarInfo->iBossHp);

		iBossCurHp = ptrNetWarInfo->iBossHp;
		iIsOpen = 0;
		if (mWarJsonIndex.count(eIt->first))
		{
			iIsOpen = 1;
			iJsonIndex = mWarJsonIndex[eIt->first];
			if (vGuildWarArr[iJsonIndex]["boosHP"].isInt())
				iBossCurHp = vGuildWarArr[iJsonIndex]["boosHP"].asInt();
			if (vGuildWarArr[iJsonIndex]["goods"].isArray())
			{
				Json::Value vGoodsList = vGuildWarArr[iJsonIndex]["goods"];
				for (UInt j = 0; j < vGoodsList.size(); j++)
				{
					iGoodsId = vGoodsList[j]["goodID"].asInt();
					iCount = vGoodsList[j]["count"].asInt();

					mArmyBattleData[iABIndex]->mGoodsList[iGoodsId] = iCount;
				}
			}

		}

		mArmyBattleData[iABIndex]->setEiIsOpen(iIsOpen);
		mArmyBattleData[iABIndex]->setEiBossCurHp(iBossCurHp);
		

		iABIndex++;
	}

	mArmyBattleData[iABIndex] = new ClanArmyBattleElem();
	mArmyBattleData[iABIndex]->setEiWarId(-1);
	mArmyBattleData[iABIndex]->setEiIsOpen(0);
	mArmyBattleData[iABIndex]->setEiBossCurHp(10);
	mArmyBattleData[iABIndex]->setEiBossMaxHp(10);
	mArmyBattleData[iABIndex]->setEiBgImg(999);
	mArmyBattleData[iABIndex]->setEiIndex(iABIndex);
	mArmyBattleData[iABIndex]->setEiSelfJob(mSelfJob);


	Json::Value vAddGoods = vArmyData["goods"];
	if (vAddGoods.isArray() && vAddGoods.size())
	{
		//结算
		history->onHttpGoodsAdd(vArmyData);
		int iRewardIndex = 0;
		map<int, map<int, int> > mClanGoods;
		for (UInt i = 0; i < vAddGoods.size(); i++)
		{
			int iGoodsId = vAddGoods[i]["goodID"].asInt();
			int iId = vAddGoods[i]["id"].asInt();
			int iCount = vAddGoods[i]["count"].asInt();

			if (mGoodsIndex.count(iGoodsId))
			{
				mClanGoods[mGoodsIndex[iGoodsId]][1] += iCount;
			}
			else
			{
				mClanGoods[iRewardIndex][0] = iGoodsId;
				mClanGoods[iRewardIndex][1] = iCount;
				mGoodsIndex[iGoodsId] = iRewardIndex;
				iRewardIndex++;
			}

		}

		int iListIndex = 0;
		for (int i = 0; i < iRewardIndex; i++)
		{
			if (mClanGoods[i][0] <= 10004 && mClanGoods[i][0] >= 10000)
			{
				mAddReward[iListIndex++] = mClanGoods[i];
			}
		}

		for (int i = 0; i < iRewardIndex; i++)
		{
			if (!(mClanGoods[i][0] <= 10004 && mClanGoods[i][0] >= 10000))
			{
				mAddReward[iListIndex++] = mClanGoods[i];
			}
		}

		if (history->m_layTitleBar)
		{
			history->m_layTitleBar->updateShow(0);
		}
		Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_CLAN_ADDGOODS, this));
	}
}

void ClanArmyBattleLayer::SetPopLayerElement(Sprite* bgSp)
{
	int iBgWidth = bgSp->getContentSize().width;
	int iBgHeight = bgSp->getContentSize().height;

	Sprite* tgjlSp = ReadSpriteName("new_ui/gh/gh_til15.png");
	tgjlSp->setAnchorPoint(Vec2(0.5, 1));
	tgjlSp->setPosition(Vec2(iBgWidth / 2, iBgHeight));
	bgSp->addChild(tgjlSp);

	int eGoodsListIndex = mAddReward.size();

	if (eGoodsListIndex > 8) eGoodsListIndex = 8;
	Sprite* eGoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	int ieGoodsBgWidth = eGoodsBG->getContentSize().width;
	int iWightNum = eGoodsListIndex;
	if (iWightNum > 4) iWightNum = 4;
	int iPointX = (iBgWidth - ieGoodsBgWidth*iWightNum - 10 * (iWightNum - 1)) / 2 + ieGoodsBgWidth / 2;
	int iOffPointY = 23;
	if (eGoodsListIndex > 4) iOffPointY = 55;

	int iGoodsId = 0, iCount = 0;
	for (int i = 0; i < eGoodsListIndex; i++)
	{
		iGoodsId = mAddReward[i][0];
		iCount = mAddReward[i][1];

		Sprite* GoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		GoodsBG->setPosition(Vec2(iPointX + (i%4)*(ieGoodsBgWidth + 10), iBgHeight / 2 + iOffPointY - i / 4*68));
		bgSp->addChild(GoodsBG);

		//物品ICON
		Sprite* spIcon = history->getGoodsSprite(iGoodsId);
		spIcon->setPosition(Vec2(33, 33));
		GoodsBG->addChild(spIcon);

		//数目
		if (iCount > 1)
		{
			Sprite* spNum = ITools::getInstance()->getNumSprite(iCount, true);
			spNum->setAnchorPoint(Vec2(1, 0));
			spNum->setPosition(Vec2(62, 4));
			GoodsBG->addChild(spNum);
		}
	}
}

void ClanArmyBattleLayer::updateShow(int iNomal)
{
	m_bEnableUpdataFatherShow = true;
}

void ClanArmyBattleLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (m_bEnableUpdataFatherShow&&mDelegate)
		mDelegate->updateShow(0);

	this->removeFromParent();
}

ClanArmyBattleLayer::ClanArmyBattleLayer()
{

}

ClanArmyBattleLayer::~ClanArmyBattleLayer()
{
	for (size_t i = 0; i < mArmyBattleData.size(); i++)
		delete mArmyBattleData[i];
}