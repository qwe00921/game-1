//
//  BoxSprite.h
//  TRGame
//
//  Created by 袁文彬 on 16/3/29.
//
//

#ifndef BoxSprite_h
#define BoxSprite_h

#include "stdio.h"

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

#include "EventDelegate.h"

class boxModel
{
public:
	boxModel()
	{
		iStoneId = 0;
	}

    CC_SYNTHESIZE(int, _starLv, StarLV);//星级
    CC_SYNTHESIZE(string, _tbPath, TBPath);//图标图片路径
    CC_SYNTHESIZE(string, _bgPath, BGPath);//背景图片路径
    CC_SYNTHESIZE(int, _pzLv, PZLV);//品质等级
    CC_SYNTHESIZE(string, _selectImagePath, SelectImagePath);//选中框图片路径
    CC_SYNTHESIZE(bool, _isSelected, IsSelected);//是否选中
    CC_SYNTHESIZE(int,_index,Index);//等级
    CC_SYNTHESIZE(int,_uiType,UIType);//界面类型  1锻造界面   2装备界面
	CC_SYNTHESIZE(string, _isOwn, IsOwn);//是否已装备自身
	CC_SYNTHESIZE(string, _heroName, HeroName); //持有武将名字
	CC_SYNTHESIZE(int, iStoneId, StoneID); //附魔宝石类型
};

class BoxSprite : public Sprite
{
public:
	static BoxSprite* create(boxModel* model, EventDelegate* delegate);
	virtual bool init(boxModel* model, EventDelegate* delegate);
    
    BoxSprite();
    ~BoxSprite();
    
    void updateBox(boxModel* model,bool bUpdatePre = false);
    
    boxModel* model;
    EventDelegate* delegate;
    
    Sprite* bg;
    Sprite* lvbg;
    Label* lvttf;
    Sprite* star;
    Sprite* pz;
    Sprite* selected;
    Sprite* tb;
	Sprite* isOwn;
	Sprite* m_spStone;

	Label* m_labName;
private:
    void initTouches();
};

#endif /* BoxSprite_h */
