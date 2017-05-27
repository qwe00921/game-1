//
//  Defines.h
//  TRGame
//
//  Created by 袁文彬 on 16/2/25.
//
//

#ifndef Defines_h
#define Defines_h


#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "ITools.h"
#include "GlobalPrompt.h"
#include "main.h"
#include "CocoMd5.h"
#include "editor-support/cocostudio/CCArmature.h"
#include "BaseClass.h"
#include "PicturePopLayer.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define MaxStar 10 //强化最高星
#define MaxQuality 4//最高的品质级别
#define LEVELGROWTH 20//等级成长
#define IS_HALF_BASEABILITY 1//基础属性除2
#define LessGuideStatusWarID 100
#define DefaultFont getRespath("fonts/DFYuanW7-GB2312.ttf", 0, 1)

#ifdef GAME_RES_MINI
	#define ReadSpriteName(str1) MiniSupport::makeRemoteSprite(str1,Sprite::create(getRespath(str1, 0, 1)))
#else
	#define ReadSpriteName(str1) Sprite::create(getRespath(str1, 0, 1))
#endif // GAME_RES_MINI
#define ReadSpriteTexture(str1) Director::getInstance()->getTextureCache()->addImage(getRespath(str1, 0, 1))
#define ReadSpritePath(str1) getRespath(str1, 0, 1)
#define LabelSystemFont(str1, int1) Label::createWithTTF(str1, getRespath("fonts/DroidSansFallback.ttf", 0, 1), int1)

#define ReadSpriteNormal(str1) ITools::getInstance()->getSpriteWithPath(str1)
#define GetSpriteTexture(str1) ITools::getInstance()->getTextureWithPath(str1)

#define ParseInt(A, B) if (B.isInt()) A = B.asInt()
#define ParseDouble(A, B) if (B.isDouble()) A = B.asDouble()
#define ParseDoubleOrInt(A, B) if (B.isDouble()) A = B.asDouble(); else if (B.isInt()) A = B.asInt()
#define ParseString(A, B) if (B.isString()) A = B.asString()

#endif /* Defines_h */
