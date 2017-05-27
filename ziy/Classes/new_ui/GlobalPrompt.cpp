//
//  GlobalPrompt.cpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/9.
//
//

#include "GlobalPrompt.h"
#include "Defines.h"

GlobalPrompt* GlobalPrompt::create(string str,bool flag)
{
    GlobalPrompt *pRet = new GlobalPrompt();
    if (pRet && pRet->init(str,flag))
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

bool GlobalPrompt::init(string str,bool flag)
{
    if (!Layer::init())
    {
        return false;
    }
    
    DSizeF size = ChessViewTool::getInstance()->getFullSize();
    
    Label* label = LabelSystemFont(str.c_str(),16);
    label->setAnchorPoint(Vec2(.5,.5));
    label->setCascadeOpacityEnabled(true);
    
    string imgPath;
    
    if(flag)
    {
        imgPath = "new_ui/global_boxbg1.png";
        label->setColor(Color3B(0, 255, 255));
    }
    else{
        imgPath = "new_ui/global_boxbg2.png";
        label->setColor(Color3B::RED);
    }
    
    Sprite* bg = ReadSpriteName(imgPath.c_str());
    bg->setPosition(Vec2(size.width / 2,300));
    bg->setCascadeOpacityEnabled(true);
    bg->setScaleY(0);
    this->addChild(bg);
    
    //将label加入bg
    label->setPosition(Vec2(bg->getContentSize().width / 2,bg->getContentSize().height / 2));
    bg->addChild(label);
    
    //动画
	FadeOut* fo = FadeOut::create(0.5f);
	MoveBy* mb = MoveBy::create(0.5f, Vec2(0, 50));
	CallFuncN* ad2 = CallFuncN::create(CC_CALLBACK_1(GlobalPrompt::removeSelf, this));
	ScaleTo* ss = ScaleTo::create(0.15f, 1);
	DelayTime* dd = DelayTime::create(0.5f);
	Sequence* seq = Sequence::create(ss, dd, CCSpawn::create(fo, mb, NULL), ad2, NULL);
    bg->runAction(seq);
    
    return true;
}

void GlobalPrompt::removeSelf(Node* node)
{
    this->removeFromParent();
}

GlobalPrompt::GlobalPrompt() : BTLayer(false)
{

}

GlobalPrompt::~GlobalPrompt()
{

}