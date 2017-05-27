#include "ActorSourceLayer.h"
#include "Defines.h"
#include "DestroySixCountriesLayer.h"
#include "PubDrinkLayer.h"
#include "HeroicAmbitionLayer.h"
#include "editor-support/cocostudio/CCArmature.h"
#include "main.h"
#include "Net_HomePage.h"

using namespace cocostudio;

ActorSourceLayer* ActorSourceLayer::create(int iActorId, int iCurrentSoul, int iNeedSoul, EventDelegate* ptrDelegate)
{
	ActorSourceLayer* pRet = new ActorSourceLayer();
	pRet->mActorId = iActorId;
	pRet->mDelegate = ptrDelegate;
	if (pRet&&pRet->init(iCurrentSoul,iNeedSoul))
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

bool ActorSourceLayer::init(int iCurrentSoul, int iNeedSoul)
{
	if (!Layer::init())
	{
		return false;
	}
	bRemoveThis = false;
	mClickElemIndex = -1;

	HistoryActorPtr ptrActor = &history->actors[mActorId - 1];

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_bg = LayerColor::create(Color4B(0, 0, 0, 100));
	black_bg->setPosition(Vec2(0, 0));
	this->addChild(black_bg);

	bg = ReadSpriteName("new_ui/img_global/box_bg1.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	//武将背景
	Sprite* img_bg = ReadSpriteName("new_ui/hero/arms_bg.png");
	img_bg->setPosition(Vec2(73, 306));
	bg->addChild(img_bg);

	__String* f_path = NULL;
	__String* img_path = NULL;
	//时装
	if (ptrActor->mFashion[0])
	{
		f_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);
		img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
		Sprite* actor_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
		int fashion_width = actor_fashion->getContentSize().width;
		actor_fashion->setTextureRect(Rect(0, fashion_width*4, fashion_width, fashion_width));
		actor_fashion->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2+24));
		actor_fashion->setLocalZOrder(trData->net_hero_dress[ptrActor->mFashion[4]]->mSpc[4]);
		img_bg->addChild(actor_fashion);
	}
	else
	{
		img_path = __String::create(ptrActor->getActorSImgPath("spc", 0));
	}

	//武将
	Sprite* actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
	int img_width = actor_img->getContentSize().width;
	actor_img->setTextureRect(Rect(0, img_width*4, img_width, img_width));
	actor_img->setPosition(Vec2(img_bg->getContentSize().width/2-2, img_bg->getContentSize().height/2+24));
	img_bg->addChild(actor_img);

	//品质
	__String* a_qualityCs = __String::createWithFormat("new_ui/hero/job%d.png", trData->actors[mActorId -1].job_img);
	Sprite* a_quality = ReadSpriteName(a_qualityCs->getCString());
	a_quality->setPosition(Vec2(33,293));
	a_quality->setScale(0.8f);
	bg->addChild(a_quality);

	//名字
	Label* a_name = LabelSystemFont(trData->actors[mActorId-1].getActorName(),16);
	a_name->setAnchorPoint(Vec2(0, 1));
	a_name->setColor(Color3B(255, 255, 153));
	a_name->setPosition(Vec2(122,350));
	bg->addChild(a_name);

	OccupationAttrPtr occu = &trData->occupations[trData->actors[mActorId - 1].occupation_id];
	//兵种
	Label* a_occu = LabelSystemFont(occu->occupation_name, 16);
	a_occu->setColor(Color3B::YELLOW);
	a_occu->setAnchorPoint(Vec2(0, 1));
	a_occu->setPosition(Vec2(122,330));
	bg->addChild(a_occu);

	//碎片
	Sprite* a_soul_icon = ReadSpriteName("new_ui/hero/icon_soul.png");
	a_soul_icon->setAnchorPoint(Vec2(0, 1));
	a_soul_icon->setPosition(Vec2(122,310));
	bg->addChild(a_soul_icon);

	__String* a_soulCs = __String::createWithFormat("%d/%d", iCurrentSoul, iNeedSoul);
	Label* a_soul = LabelSystemFont(a_soulCs->getCString(),16);
	a_soul->setAnchorPoint(Vec2(0,1));
	a_soul->setPosition(Vec2(124+a_soul_icon->getContentSize().width,310));
	bg->addChild(a_soul);

	//攻击范围
	Sprite* hitArea = ITools::getInstance()->getSpriteWithPath(__String::createWithFormat("img/HitArea/%d.png", occu->attack_type_id + 1)->getCString());
	hitArea->setPosition(Vec2(238, 322));
	bg->addChild(hitArea);

	//获得方法153，265
	Sprite* a_obtain = ReadSpriteName("new_ui/hero/hero_txt2.png");
	a_obtain->setPosition(Vec2(bg->getContentSize().width / 2, 265));
	bg->addChild(a_obtain);

	addElement(bg);

	return true;
}

void ActorSourceLayer::addElement(Sprite* ptrBg)
{
	bool is_null = false;
	NetHeroListPtr ptrHeroL = trData->net_hero_list[mActorId];
	int elen_num = 0;
	for (size_t i = 0; i <ptrHeroL->obtain_method.size(); i++)
	{
		if (i==0&&ptrHeroL->obtain_method[i][0] == 0)
		{
			is_null = true;
			break;
		}

		ActorSourceElem* ptrASE = new ActorSourceElem();
		//219
		Sprite* e_bg = ReadSpriteName("new_ui/img_global/box_bg2.png");
		e_bg->setPosition(Vec2(ptrBg->getContentSize().width / 2, 219 - elen_num*59));
		ptrBg->addChild(e_bg);
		ptrASE->setMEBG(e_bg);
		elem[elen_num++] = ptrASE;

		if (ptrHeroL->obtain_method[i][0] == 1)
		{
			int iWarId = ptrHeroL->obtain_method[i][2];
			__String* map_name = __String::createWithFormat(LL("ztxddz"), trData->m_mNetWarInfo[iWarId]->iMapIndex);
			Label* map_ttf = LabelSystemFont(map_name->getCString(), 16);
			map_ttf->setPosition(Vec2(22,42));
			map_ttf->setAnchorPoint(Vec2(0, 1));
			e_bg->addChild(map_ttf);

			if(!history->mMapDateInfo->getMapDateElem(iWarId))
			{
				Label* wkq = LabelSystemFont(LL("weikaiqi"), 16);
				wkq->setPosition(Vec2(22 + map_ttf->getContentSize().width, 42));
				wkq->setAnchorPoint(Vec2(0, 1));
				wkq->setColor(Color3B::YELLOW);
				e_bg->addChild(wkq);
			}

			Label* war_name = LabelSystemFont(trData->m_mNetWarInfo[iWarId]->strName.c_str(), 16);
			war_name->setPosition(Vec2(22, 22));
			war_name->setAnchorPoint(Vec2(0, 1));
			e_bg->addChild(war_name);
			ptrASE->setMEGETTYPE(1);
			ptrASE->setMEWINDEX(iWarId);
		}
		else if (ptrHeroL->obtain_method[i][0] == 2)
		{
			//TODO
			Label* hero_ttf = LabelSystemFont(LL("yingxiongzhi"), 16);
			hero_ttf->setPosition(Vec2(22, 42));
			hero_ttf->setAnchorPoint(Vec2(0, 1));
			e_bg->addChild(hero_ttf);

			int iOpenWarId = trData->m_mNetWarInfo[ptrHeroL->obtain_method[i][1]]->iOpenWarId;
			if (history->mMapDateInfo->getMapDateElem(iOpenWarId) == NULL)
			{
				Label* wkq = LabelSystemFont(LL("weikaiqi"), 16);
				wkq->setPosition(Vec2(22 + hero_ttf->getContentSize().width, 42));
				wkq->setAnchorPoint(Vec2(0, 1));
				wkq->setColor(Color3B::YELLOW);
				e_bg->addChild(wkq);
			}
			Label* war_name = LabelSystemFont(trData->m_mNetWarInfo[ptrHeroL->obtain_method[i][1]]->strName.c_str(), 16);
			war_name->setPosition(Vec2(22, 22));
			war_name->setAnchorPoint(Vec2(0, 1));
			e_bg->addChild(war_name);
			ptrASE->setMEGETTYPE(2);
			ptrASE->setMEWINDEX(ptrHeroL->obtain_method[i][1]);
		}
		else if (ptrHeroL->obtain_method[i][0] == 3)
		{
			Label* inn = LabelSystemFont(LL("yuelaikezhan"), 16);
			inn->setPosition(Vec2(22, 42));
			inn->setAnchorPoint(Vec2(0, 1));
			e_bg->addChild(inn);

			Label* war_name = LabelSystemFont(LL("nehmj"), 16);
			war_name->setPosition(Vec2(22, 22));
			war_name->setAnchorPoint(Vec2(0, 1));
			e_bg->addChild(war_name);
			ptrASE->setMEGETTYPE(3);
			ptrASE->setMEWINDEX(0);
		}

	}
	if (is_null)
	{
		Label* zwhd = LabelSystemFont(LL("yxhdzwkf"),18);
		zwhd->setColor(Color3B(102, 0, 0));
		zwhd->setPosition(Vec2(ptrBg->getContentSize().width/2,215));
		ptrBg->addChild(zwhd);
	}
}

bool ActorSourceLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	bRemoveThis = false;
	mClickElemIndex = -1;
	playSound(SOUND_LEFT_MOUSE_CLICK);
	Rect bgBox = bg->getBoundingBox();
	if (Rect(0, 0, bgBox.size.width, bgBox.size.height).containsPoint(bg->convertToNodeSpace(pTouch->getLocation())))
	{
		for (size_t i = 0; i < elem.size(); i++)
		{
			Rect elemBox = elem[i]->getMEBG()->getBoundingBox();
			if (Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(elem[i]->getMEBG()->convertToNodeSpace(pTouch->getLocation())))
			{
				if (elem[i]->getMEGETTYPE() == 1)
				{
					int iWarId = elem[i]->getMEWINDEX();
					if (!history->mMapDateInfo->getMapDateElem(iWarId))
						ITools::getInstance()->createGlobalPrompt(LL("gqwkq"), false);
					else
					{
						mClickElemIndex = i;
						return true;
					}
				}
				else if (elem[i]->getMEGETTYPE() == 2)
				{
					int iWarId = elem[i]->getMEWINDEX();
					int iOpenWarId = trData->m_mNetWarInfo[iWarId]->iOpenWarId;
					if (history->mMapDateInfo->getMapDateElem(iOpenWarId) == NULL)
					{
						ITools::getInstance()->createGlobalPrompt(LL("gqwkq"), false);
					}
					else
					{
						mClickElemIndex = i;
						return true;
					}
				}
				else
				{
					mClickElemIndex = i;
					return true;
				}
			}
		}
	}
	else
	{
		bRemoveThis = true;
	}
	return true;
}

void ActorSourceLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void ActorSourceLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

	if (mClickElemIndex > -1)
	{
		if (elem[mClickElemIndex]->getMEGETTYPE() == 1)
		{
			int iWarId = elem[mClickElemIndex]->getMEWINDEX();
			ChessApplication::getInstance()->removeAllFunctionLayer();
			ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create(iWarId));
		}
		else if (elem[mClickElemIndex]->getMEGETTYPE() == 2)
		{
			int iWarId = elem[mClickElemIndex]->getMEWINDEX();
			ChessApplication::getInstance()->removeAllFunctionLayer();
			ChessApplication::getInstance()->AddFunctionLayer(HeroicAmbitionLayer::create(iWarId));
		}
		else
		{
			if(history->ownArmyNum > 3)
			{
				if(mDelegate)
					mDelegate->RemoveAllParentLayer();
				ChessApplication::getInstance()->AddFunctionLayer(PubDrinkLayer::create());
				this->removeFromParent();
			}
			else
			{
				ITools::getInstance()->createGlobalPrompt(LL("pubdrinkopen"), true);
			}
		}
	}

	if (bRemoveThis == true)
	{
		if (mDelegate)
			mDelegate->ChildCloseLayer(0);

		
		this->removeFromParent();
	}
}

void ActorSourceLayer::onExit()
{
	Layer::onExit();
	//HongDian::getInstence()->autoUpadteInfo(mActorId, ANNIU_HONGDIAN);
}

ActorSourceLayer::ActorSourceLayer()
{

}

ActorSourceLayer::~ActorSourceLayer()
{
	for (size_t i = 0; i < elem.size(); i++)
		delete elem[i];
}