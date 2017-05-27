//
//  BoxSprite.cpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/29.
//
//

#include "BoxSprite.h"
#include "Defines.h"

BoxSprite* BoxSprite::create(boxModel* model, EventDelegate* delegate)
{
    BoxSprite *pRet = new BoxSprite();
    if (pRet && pRet->init(model, delegate))
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

bool BoxSprite::init(boxModel* model, EventDelegate* delegate)
{
    if (!Sprite::init())
    {
        return false;
    }
    
    this->delegate = delegate;
    this->model = model;
    
    //创建背景图
    if(!model->getBGPath().empty())
    {
        bg = ReadSpriteName(model->getBGPath().c_str());
        this->addChild(bg,0);
    }
    
    //创建图标
    if(!model->getTBPath().empty())
    {
        tb = ITools::getInstance()->getSpriteWithPath(model->getTBPath().c_str());
        tb->setScale(0.85f);
        this->addChild(tb,1);
    }
    
    //创建星级图标
    if(model->getStarLV() != 0)
    {
		string strFormat = model->getUIType() == 1 ? "new_ui/img_number/star_b_%d.png" : "new_ui/img_number/star_%d.png";
		__String* str = __String::createWithFormat(strFormat.c_str(), model->getStarLV());
        star = ReadSpriteName(str->getCString());
		star->setAnchorPoint(Vec2(0.5f, 0.0f));
		star->setPosition(Vec2(0, -bg->getContentSize().height / 2));
        this->addChild(star,3);
    }
    
    //创建选中框
    if(model->getIsSelected())
    {
        selected = ReadSpriteName(model->getSelectImagePath().c_str());
        this->addChild(selected,4);
    }
    
    //创建品质框
    if(model->getPZLV() != 0)
    {
		string strFormat = model->getUIType() == 1 ? "new_ui/img_global/q%d_1.png" : "new_ui/img_global/q%d.png";
		__String* str = __String::createWithFormat(strFormat.c_str(), model->getPZLV());
        pz = ReadSpriteName(str->getCString());
        this->addChild(pz,2);
    }

	//创建装备状态图标
	if (!model->getIsOwn().empty())
	{
		isOwn = ReadSpriteName(model->getIsOwn().c_str());
		isOwn->setPosition(Vec2(15, 14));
		this->addChild(isOwn,2);
	}

	//持有武将名
	m_labName = LabelSystemFont(model->getHeroName(), 14);
	m_labName->setPosition(Vec2(0, 21));
	m_labName->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labName, 3);

	//附魔灵石
	if (model->getStoneID())
	{
		m_spStone = history->getGoodsSprite(model->getStoneID());
		m_spStone->setScale(model->getUIType() == 1 ? 0.3f : 0.25f);
		m_spStone->setAnchorPoint(Vec2(0.0f, 0.5f));
		m_spStone->setPosition(Vec2(-bg->getContentSize().width / 2 + 2, 0));
		this->addChild(m_spStone, 3);
	}

    initTouches();
    return true;
}

void BoxSprite::updateBox(boxModel* model, bool bUpdatePre)
{
    //更新数据
	if (bUpdatePre)
	{
		this->model = model;
	}
	else
	{
		this->model->setStarLV(model->getStarLV());
		this->model->setPZLV(model->getPZLV());
		this->model->setTBPath(model->getTBPath());
		this->model->setIsSelected(model->getIsSelected());
		this->model->setIndex(model->getIndex());
	}
    
    //更新星级
    if(model->getStarLV() != 0)
    {
        if(star)
        {
            star->setVisible(true);
			string strFormat = model->getUIType() == 1 ? "new_ui/img_number/star_b_%d.png" : "new_ui/img_number/star_%d.png";
			__String* str = __String::createWithFormat(strFormat.c_str(), model->getStarLV());
            star->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(str->getCString())));
        }
        else
        {
			string strFormat = model->getUIType() == 1 ? "new_ui/img_number/star_b_%d.png" : "new_ui/img_number/star_%d.png";
			__String* str = __String::createWithFormat(strFormat.c_str(), model->getStarLV());
            star = ReadSpriteName(str->getCString());
			star->setAnchorPoint(Vec2(0.5f, 0.0f));
			star->setPosition(Vec2(0, -bg->getContentSize().height / 2));
            this->addChild(star, 3);
        }
    }
    else
    {
        if(star)
            star->setVisible(false);
    }
    
    //更新图标
    if(!model->getTBPath().empty())
    {
        if(tb)
        {
            tb->setVisible(true);
            tb->setTexture(ITools::getInstance()->getTextureWithPath(model->getTBPath().c_str()));
        }
        else
        {
            tb = ITools::getInstance()->getSpriteWithPath(model->getTBPath().c_str());
			if (model->getUIType() != 1)
			{
				tb->setScale(0.85f);
			}
            this->addChild(tb,1);
        }
        
    }
    else
    {
        if(tb)
            tb->setVisible(false);
    }
    
    //更新品质
    if(model->getPZLV() != 0)
    {
        if(pz)
        {
            pz->setVisible(true);
			string strFormat = model->getUIType() == 1 ? "new_ui/img_global/q%d_1.png" : "new_ui/img_global/q%d.png";
			__String* str = __String::createWithFormat(strFormat.c_str(), model->getPZLV());
            pz->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(str->getCString())));
        }
        else
        {
			string strFormat = model->getUIType() == 1 ? "new_ui/img_global/q%d_1.png" : "new_ui/img_global/q%d.png";
			__String* str = __String::createWithFormat(strFormat.c_str(), model->getPZLV());
            pz = ReadSpriteName(str->getCString());
            this->addChild(pz,2);
        }
    }
    else
    {
        if(pz)
            pz->setVisible(false);
    }
    
	//更新装备状态
	if (!model->getIsOwn().empty())
	{
		if (isOwn)
		{
			isOwn->setVisible(true);
		}
		else
		{
			isOwn = ReadSpriteName(model->getIsOwn().c_str());
			isOwn->setPosition(Vec2(15,14));
			this->addChild(isOwn, 2);
		}
	}
	else
	{
		if (isOwn)
			isOwn->setVisible(false);
	}

    //更新选中
    if(model->getIsSelected())
    {
        if(selected)
        {
            selected->setVisible(true);
        }
        else
        {
            selected = ReadSpriteName(model->getSelectImagePath().c_str());
            this->addChild(selected,3);
        }
    }
    else
    {
        if(selected)
            selected->setVisible(false);
    }

	m_labName->setString(model->getHeroName());

	//附魔灵石
	if(model->getStoneID())
	{
		if (m_spStone)
		{
			m_spStone->setTexture(history->getGoodsTexture(model->getStoneID()));
		}
		else
		{
			m_spStone = history->getGoodsSprite(model->getStoneID());
			m_spStone->setScale(model->getUIType() == 1 ? 0.3f : 0.25f);
			m_spStone->setAnchorPoint(Vec2(0.0f, 0.5f));
			m_spStone->setPosition(Vec2(-bg->getContentSize().width / 2 + 2, 0));
			this->addChild(m_spStone, 3);
		}
		m_spStone->setVisible(true);
	}
	else if (m_spStone)
	{
		m_spStone->setVisible(false);
	}
}

BoxSprite::BoxSprite()
{
    model = NULL;
    
    bg = NULL;
    lvbg = NULL;
    lvttf = NULL;
    star = NULL;
    pz = NULL;
    selected = NULL;
    tb = NULL;
	isOwn = NULL;
	m_spStone = nullptr;
    
    CCLOG("Init data succeed!");
}

BoxSprite::~BoxSprite()
{
    CCLOG("Free BoxSprite");
}

void BoxSprite::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        Rect TBBox = bg->getBoundingBox();
        if(Rect(0, 0, TBBox.size.width, TBBox.size.height).containsPoint(bg->convertToNodeSpace(touch->getLocation())))
        {
            if(!model->getTBPath().empty()){
                this->delegate->updateShow(model->getIndex(),model->getUIType());
            }
        }

        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}