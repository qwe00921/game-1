#include "RomanceInfoLayer.h"
#include "Defines.h"
#include "GoodsSprite.h"
#include "RomanceDetailsLayer.h"
#include "HeroSprite.h"
#include "NetPromptTool.h"
#include "GuideLayer.h"

RomanceInfoLayer* RomanceInfoLayer::create(int iRMapId,EventDelegate* ptrDelegate, bool bIsAddSureM)
{
	RomanceInfoLayer* pRet = new RomanceInfoLayer();
	if (pRet&&pRet->init(iRMapId,ptrDelegate,bIsAddSureM))
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

bool RomanceInfoLayer::init(int iRMapId,EventDelegate* ptrDelegate, bool bIsAddSureM)
{
	if (!Layer::init())
	{
		return false;
	}
	this->mMapId = iRMapId;
	this->mDelegate = ptrDelegate;

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2());
	this->addChild(black_layer);

	DSize size = ChessViewTool::getInstance()->getFullSize();

	NetStory* ptrNetStory = trData->net_story[iRMapId];

	Sprite* bg = ReadSpriteName("new_ui/img_global/imgs_bg7.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	Label* infoLab = LabelSystemFont(ptrNetStory->describe,18);
	infoLab->setDimensions(453, 110);
	infoLab->setLineHeight(26);
	infoLab->setPosition(Vec2(29, bg->getContentSize().height - 52));
	infoLab->setAnchorPoint(Vec2(0,1));
	infoLab->setColor(Color3B(102,51,0));
	bg->addChild(infoLab);
	for (map<int, map<int, int> >::iterator mMIt = ptrNetStory->txt_color.begin(); mMIt != ptrNetStory->txt_color.end(); mMIt++)
	{
		infoLab->getLetter(mMIt->first)->setColor(Color3B(mMIt->second[0], mMIt->second[1], mMIt->second[2]));
	}

	AddGoodsElem(bg);

	Sprite* authorSp = ReadSpriteName(StringUtils::format("new_ui/img_global/author_name_%d.png",ptrNetStory->author).c_str());
	authorSp->setAnchorPoint(Vec2(0, 0));
	authorSp->setPosition(Vec2(42,40));
	bg->addChild(authorSp);

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(0, 0));
	bg->addChild(mMenu);

	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(RomanceInfoLayer::clickCloseLayer, this));
	close_menu->setPosition(Vec2(bg->getContentSize().width - closeNormalImage->getContentSize().width / 2, bg->getContentSize().height - closeNormalImage->getContentSize().height / 2));
	mMenu->addChild(close_menu);

	if (bIsAddSureM)
	{
		map<int, Sprite*> sureMenuSp;
		sureMenuSp[0] = ReadSpriteName("new_ui/img_global/global_ban13.png");
		sureMenuSp[1] = ReadSpriteName("new_ui/img_global/global_ban14.png");

		Sprite* numBg = NULL;
		for (int i = 0; i < 2; i++)
		{
			numBg = sureMenuSp[i];
			int iNumBgWidth = numBg->getContentSize().width;
			int iNumBgHeight = numBg->getContentSize().height;

			if (ptrNetStory->story_type == 1)
			{
				Label* freeLab = LabelSystemFont(LL("mianfeikaiqi"), 16);
				freeLab->setPosition(Vec2(iNumBgWidth / 2, iNumBgHeight / 2));
				freeLab->enableOutline(Color4B::BLACK, 1);
				numBg->addChild(freeLab);
			}
			else if (ptrNetStory->story_type == 2)
			{
				Sprite* sealSp = ReadSpriteName("new_ui/img_global/icon_yx.png");
				sealSp->setAnchorPoint(Vec2(0, 0.5));
				sealSp->setPosition(Vec2(5, iNumBgHeight / 2));
				numBg->addChild(sealSp);

				Label* sealLab = LabelSystemFont(IntToString(ptrNetStory->jadeSeal), 16);
				sealLab->setAnchorPoint(Vec2(0, 0.5));
				sealLab->setPosition(Vec2(5 + sealSp->getContentSize().width, iNumBgHeight / 2));
				sealLab->enableOutline(Color4B::BLACK, 1);
				sealLab->setColor(Color3B::GREEN);
				numBg->addChild(sealLab);

				Label* jadeLab = LabelSystemFont(IntToString(ptrNetStory->jade), 16);
				jadeLab->setAnchorPoint(Vec2(1, 0.5));
				jadeLab->setPosition(Vec2(iNumBgWidth - 5, iNumBgHeight / 2));
				jadeLab->enableOutline(Color4B::BLACK, 1);
				jadeLab->setColor(Color3B::GREEN);
				numBg->addChild(jadeLab);

				Sprite* jadeSp = ReadSpriteName("new_ui/img_global/icon_gy.png");
				jadeSp->setAnchorPoint(Vec2(1, 0.5));
				jadeSp->setPosition(Vec2(iNumBgWidth - 3 - jadeLab->getContentSize().width, iNumBgHeight / 2));
				numBg->addChild(jadeSp);
			}
			else if (ptrNetStory->story_type == 3)
			{
				Sprite* vipSp = ReadSpriteName("new_ui/main/vip1.png");
				vipSp->setAnchorPoint(Vec2(0, 0.5));
				vipSp->setPosition(Vec2(5, iNumBgHeight / 2));
				numBg->addChild(vipSp);

				if (ptrNetStory->jade)
				{
					Label* jadeLab = LabelSystemFont(IntToString(ptrNetStory->jade), 16);
					jadeLab->setAnchorPoint(Vec2(1, 0.5));
					jadeLab->setPosition(Vec2(iNumBgWidth - 5, iNumBgHeight / 2));
					jadeLab->enableOutline(Color4B::BLACK, 1);
					jadeLab->setColor(Color3B::GREEN);
					numBg->addChild(jadeLab);

					Sprite* jadeSp = ReadSpriteName("new_ui/img_global/icon_gy.png");
					jadeSp->setAnchorPoint(Vec2(1, 0.5));
					jadeSp->setPosition(Vec2(iNumBgWidth - 8 - jadeLab->getContentSize().width, iNumBgHeight / 2));
					numBg->addChild(jadeSp);
				}
				else
				{
					Label* freeLab = LabelSystemFont(LL("mianfeikaiqi"), 16);
					freeLab->setAnchorPoint(Vec2(1, 0.5));
					freeLab->setPosition(Vec2(iNumBgWidth - 8, iNumBgHeight / 2));
					freeLab->enableOutline(Color4B::BLACK, 1);
					numBg->addChild(freeLab);
				}
			}
			else if (ptrNetStory->story_type == 4 && ptrNetStory->condition)
			{
				int iHeroId = ptrNetStory->condition;
				NetHeroNoDress* ptrNHND = NULL;
				Sprite* arms_fashion = NULL;
				__String* img_path = NULL;

				if (trData->net_hero_no_dress.count(iHeroId))
				{
					ptrNHND = trData->net_hero_no_dress[iHeroId];
					__String* f_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", 100, ptrNHND->fashion2);
					img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrNHND->gender, ptrNHND->fashion1);
					arms_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
					int fashion_width = arms_fashion->getContentSize().width;
					arms_fashion->setTextureRect(Rect(0, fashion_width * 4, fashion_width, fashion_width));
					int iZorder = trData->net_hero_dress[ptrNHND->fashion2]->mSpc[4];
					if (iZorder) iZorder++;
					arms_fashion->setPosition(20, numBg->getContentSize().height / 2 + 12);
					numBg->addChild(arms_fashion, iZorder);
				}
				else
				{
					img_path = __String::create(history->actors[iHeroId - 1].getActorSImgPath("spc", 0));
				}

				//武将
				Sprite* actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
				int img_width = actor_img->getContentSize().width;
				//actor_img->setAnchorPoint(Vec2(0.5, 0));
				actor_img->setTextureRect(Rect(0, img_width * 4, img_width, img_width));
				actor_img->setPosition(Vec2(20, numBg->getContentSize().height / 2 + 12));
				numBg->addChild(actor_img);

				Label* nameLab = LabelSystemFont(__String::createWithFormat("%s %s", trData->actors[iHeroId - 1].getActorName(), LL("kekaiqi"))->getCString(), 16);
				nameLab->setAnchorPoint(Vec2(0, 0.5));
				nameLab->setPosition(Vec2(45, numBg->getContentSize().height / 2));
				nameLab->enableOutline(Color4B::BLACK, 1);
				numBg->addChild(nameLab);

			}
		}

		MenuItemSprite* sureItemM = MenuItemSprite::create(sureMenuSp[0], sureMenuSp[1], CC_CALLBACK_1(RomanceInfoLayer::clickSureMenu, this));
		sureItemM->setPosition(Vec2(bg->getContentSize().width / 2, 52));
		mMenu->addChild(sureItemM);
	}



	history->updateGuide(EMIL_GUIDE);
	return true;
}

void RomanceInfoLayer::AddGoodsElem(Sprite* BgSp)
{
	NetStory* ptrNetStory = trData->net_story[mMapId];
	int iElemIndex = 0;

	for (size_t i = 0; i < ptrNetStory->hid.size(); i++)
	{
		if (ptrNetStory->hid[i])
		{
			HeroSprite* ptrHeroSp = HeroSprite::create(ptrNetStory->hid[i]);
			ptrHeroSp->setPosition(Vec2(67 + 64 * (iElemIndex % 7), 164 - 56 * (iElemIndex / 7)));
			BgSp->addChild(ptrHeroSp);
			iElemIndex++;
			if (iElemIndex >= 14)  break;
		}
	}

	if (iElemIndex < 14)
	{
		for (size_t i = 0; i < ptrNetStory->good_id.size(); i++)
		{
			if (ptrNetStory->good_id[i])
			{
				GoodsSprite* ptrGoodsSp = GoodsSprite::create(ptrNetStory->good_id[i]);
				ptrGoodsSp->setPosition(Vec2(67 + 64 * (iElemIndex % 7), 164 - 56 * (iElemIndex / 7)));
				BgSp->addChild(ptrGoodsSp);
				iElemIndex++;
				if (iElemIndex >= 14)  break;
			}
		}
	}
}

void RomanceInfoLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void RomanceInfoLayer::clickSureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	SendNetData();
}

void RomanceInfoLayer::SendNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s&mapID=%d", GameTokenS.c_str(),mMapId);
	string validateCode = md5.ToMD5(validateCs->getCString());
	CCLOG("%s",validateCs->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"mapID\":%d,\"validateCode\":\"%s\"}", GameTokenS.c_str(), mMapId, validateCode.c_str());
	CCLOG("%s", data->getCString());
	ITools::getInstance()->connectingNetwork("buyPlotScript",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(RomanceInfoLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void RomanceInfoLayer::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (value["result"]["isTrue"].isObject())
		{
			if (value["result"]["jade"].isInt())
				history->m_iJade = value["result"]["jade"].asInt();
			if (value["result"]["jadeSeal"].isInt())
				history->m_iJadeSeal = value["result"]["jadeSeal"].asInt();

			if (value["result"]["warIDs"].isArray())
			{
				Json::Value WarValue = value["result"]["warIDs"];
				for (UInt i = 0; i < WarValue.size(); i++)
				{
					history->mRomanceOpenData[mMapId][WarValue[i].asInt()] = 1;
				}
			}

			if (value["result"]["heroes"].isArray())
			{
				Json::Value JsonHero = value["result"]["heroes"];
				for (UInt i = 0; i < JsonHero.size(); i++)
				{
					int iHeroId = JsonHero[i]["heroID"].asInt();
					{
						HistoryActorPtr PtrCurActor = &history->actors[iHeroId - 1];
						PtrCurActor->init(iHeroId - 1);
						HeroElem InitHeroE;
						InitHeroE.loadJson(JsonHero[i]);
						history->initNetHeroElem(&InitHeroE);
						//history->popOpenNotice(1);
					}
				}
			}

			if (value["result"]["addGoods"].isArray())
			{
				Json::Value valGoods = value["result"]["addGoods"];
				for(UInt i = 0; i < valGoods.size(); i++)
				{
					ConsumeElem consuElem;
					consuElem.loadJson(valGoods[i]);
					history->packGoodsAdd(consuElem.getGoodID(), consuElem.getNetID(), consuElem.getCount());
				}
			}

			if (value["result"]["heroGoods"].isArray())
			{
				Json::Value jsonGoods = value["result"]["heroGoods"];
				for (int i = 0; i < (int)jsonGoods.size(); i++)
				{
					GoodsElem tempGoogsItem;
					tempGoogsItem.loadJson(jsonGoods[i]);
					HistoryActorPtr ptrActor = &history->actors[tempGoogsItem.getHeroID() - 1];
					HistoryGoods hisGoods(&tempGoogsItem);
					if (hisGoods.GetGoodsType() == GOOD_TYPE_WEAPON)
					{
						ptrActor->weapon.setGood(&hisGoods);
					}
					else  if (hisGoods.GetGoodsType() == GOOD_TYPE_COAT)
					{
						ptrActor->coat.setGood(&hisGoods);
					}
					else  if (hisGoods.GetGoodsType() == GOOD_TYPE_ASSIST)
					{
						ptrActor->assist_good.setGood(&hisGoods);
					}
					else  if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
					{
						ptrActor->fashion.setGood(&hisGoods);
					}
					else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
					{
						ptrActor->wap_fashion.setGood(&hisGoods);
					}
					ptrActor->UpdataGoodEffect();
				}
			}

		}
		ChessApplication::getInstance()->AddFunctionLayer(RomanceDetailsLayer::create(mMapId,false,true));
		NetPromptTool::getInstance()->AddNetPrompt(PROMPT_YYTKQ, mMapId);
		if (mDelegate)
			mDelegate->ChildCloseLayer(0);
		this->removeFromParent();
	}
}

RomanceInfoLayer::RomanceInfoLayer()
{

}

RomanceInfoLayer::~RomanceInfoLayer()
{

}