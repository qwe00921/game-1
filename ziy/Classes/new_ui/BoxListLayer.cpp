//
//  BoxListLayer.cpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/29.
//
//

#include "BoxListLayer.h"
#include "Defines.h"

#define ROW_EQUIP_NUM 3
#define MAX_EQUIP_NUM 9

BoxListLayer* BoxListLayer::create(map<int,boxModel*> data,EventDelegate* delegate)
{
    BoxListLayer *pRet = new BoxListLayer();
    if (pRet && pRet->init(data,delegate))
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

bool BoxListLayer::init(map<int,boxModel*> data,EventDelegate* delegate)
{
    if (!Layer::init())
    {
        return false;
    }
    
    Sprite* bg = ReadSpriteName("new_ui/img_global/hero_bg2.png");
    this->addChild(bg);
    
//    data[0]->setIsSelected(true);
//    selectIndex = 0;
    
    this->delegate = delegate;
    this->data = data;
    
	for(int i = 0; i < MAX_EQUIP_NUM; i++)
    {
        if(data[i])
        {
			BoxSprite* bs = BoxSprite::create(data[i], this);
			bs->setPosition(Vec2(i % ROW_EQUIP_NUM * 78, -i / ROW_EQUIP_NUM * 78) + Vec2(-78, 78));
            this->addChild(bs);
            boxs[i] = bs;
            
            if(data[0]->getIsSelected())
            {
				selectIndex = data[0]->getIndex() % MAX_EQUIP_NUM;
            }
        }
    }
    
    return true;
}

void BoxListLayer::updateShow(int index,int type)
{
    playSound(SOUND_LEFT_MOUSE_CLICK);
    
    if(type == 1)//锻造界面
    {
        //更新选中
        data[selectIndex]->setIsSelected(false);
        boxs[selectIndex]->updateBox(data[selectIndex]);
        
		int i = index % MAX_EQUIP_NUM;//转换下标
        
        data[i]->setIsSelected(true);
        selectIndex = i;
        
        boxs[i]->updateBox(data[i]);
        
        this->delegate->updateShow(index);
    }
    else if(type == 2)//装备界面
    {
        this->delegate->createPop(index);
    }
}

void BoxListLayer::updateAll(map<int,boxModel*> data,int type)
{
    if(type == 1)
    {
        data[0]->setIsSelected(true);
        selectIndex = 0;
    }
    
    this->data = data;
    
    for(UInt i=0;i<boxs.size();i++)
    {
        boxs[i]->updateBox(data[i]);
    }
    
    if(type == 1)
    {
        this->delegate->updateShow(data[selectIndex]->getIndex());
    }
}
BoxListLayer::BoxListLayer() : BTLayer(false)
{
    
}

BoxListLayer::~BoxListLayer()
{
    
}