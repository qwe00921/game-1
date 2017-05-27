//
//  GlobalPrompt.hpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/9.
//
//

#ifndef GlobalPrompt_h
#define GlobalPrompt_h

#include "Defines.h"
#include "BaseClass.h"

class GlobalPrompt : public BTLayer
{
public:
    //flag 用于判断提示框颜色 true是蓝色成功框 false是红色失败框
    static GlobalPrompt* create(string str,bool flag);
    virtual bool init(string str,bool flag);
    
    GlobalPrompt();
    ~GlobalPrompt();
    
    void removeSelf(Node* node);
};

#endif /* GlobalPrompt_hpp */
