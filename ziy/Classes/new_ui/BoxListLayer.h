//
//  BoxListLayer.hpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/29.
//
//

#ifndef BoxListLayer_h
#define BoxListLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "stdio.h"
using namespace std;
#include "BaseClass.h"

#include "BoxSprite.h"

class BoxListLayer : public BTLayer
{
public:
    static BoxListLayer* create(map<int,boxModel*> data,EventDelegate* delegate);
    virtual bool init(map<int,boxModel*> data,EventDelegate* delegate);
    
    BoxListLayer();
    ~BoxListLayer();
    
    virtual void updateShow(int index,int type);//更新选中
    void updateAll(map<int,boxModel*> data,int type);
    
    map<int,boxModel*> data;
    map<int,BoxSprite*> boxs;
    
    EventDelegate* delegate;
    int selectIndex;
};

#endif /* BoxListLayer_h */
