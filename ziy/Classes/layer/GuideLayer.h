//
//  GuideLayer.h
//  TRGame
//
//  Created by Jimmy on 16/07/28.
//
//

#ifndef _GuideLayer_H_
#define _GuideLayer_H_

#include "Defines.h"

enum 
{
	SKIP_NORMAL = 0,
	SKIP_FORBID = 1,
	SKIP_AUTO
};
enum
{
	FIRSET_GUIDE = 0,
	FIRSET2_GUIDE,
	FIRSET3_GUIDE,
	FIRSET4_GUIDE,
	FIRSET5_GUIDE,
	EMIL_GUIDE=7,//穿戴装备
	STRENG_GUIDE,//强化
	GOTOPubDrink_GUIDE=9,//加技能
	HECHENG_GUIDE,//进入酒馆
	ZHUJIEMIAN5_GUIDE,//合成
	ZHUJIEMIAN6_GUIDE,//勾玉召唤
	ZHUJIEMIAN7_GUIDE,
	ZHUJIEMIAN8_GUIDE,
	ZHUJIEMIAN9_GUIDE,
	ZHUJIEMIAN10_GUIDE,
	ZHUJIEMIAN11_GUIDE,
	ZHUJIEMIAN12_GUIDE,
	ZHUJIEMIAN13_GUIDE,
	ZHUJIEMIAN14_GUIDE,
	ZHUJIEMIAN15_GUIDE,
	ZHUJIEMIAN16_GUIDE,
	ZHUJIEMIAN17_GUIDE,
	ZHUJIEMIAN18_GUIDE,
	
	

	STARTLEVEL3_GUIDE,//引导进入剧情3
	STARTLEVEL4_GUIDE,//引导进入剧情4
	

	HELP2_GUIDE,//引导失败进入剧情2
	HELP3_GUIDE,//引导失败进入剧情3
	HELP4_GUIDE,//引导失败进入剧情4
	MAX_GUIDE
};

class GuideRect
{
public:
	GuideRect(float fPosX, float fPosY, float fWidth, float fHeight, int iPosType, int iPass = 0, int iSkipType = SKIP_NORMAL,float iTime=0,int passOver=0);
	Rect getRect();
public:
	float m_fPosX;
	float m_fPosY;
	float m_fWidth;
	float m_fHeight;
	int m_iType;
	int m_iSkipType;
	int m_pass; //是否通过
	int m_passOver;
	float m_time; //等待时间
	bool m_lock;
};

class GuideInfoPtr
{
public:
	GuideInfoPtr()
	{
		x = 0;
		y = 0;
		wide = 0;
		high = 0;
		Postype = 0;
		SkipType = SKIP_NORMAL;
	}
public:
	float x;
	float y;
	float wide;
	float high;
	int Postype;
	int SkipType;
};

class GuideLayer : public BTLayer
{
public:
	//CREATE_FUNC(GuideLayer);
	static GuideLayer* create(int num);
	static GuideLayer* Factory(int num);//除了history中的updateGuide（）函数以外其他类禁止使用该函数

	virtual bool init(int num);
	GuideLayer();
	~GuideLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	static char *IntToGuideString(int GuideType);//将一个int转换为引导需要的字符串
	void updateGuide();//自动更新引导界面
	void waitNextGuide();
	void GetGuideJson();
	static bool CanUpdate();
	static bool isNULLGuide(int num);
	virtual void updateRes(float dt);//掉线时对引导进行处理

	static void setreset_guide(Node* sender);

	void resetGuide();

	bool m_ilock;
	static bool reset_guide;
private:
	void initControl();
	void initClickArea();
	void skipNextGuide(float fDelay);
private:
	static GuideLayer* m_pInstance[MAX_GUIDE];//引导工厂，通过传人的值，决定产生哪个引导，使用Factory（int num）来创建

	Sprite* m_spModel;
	ClippingNode* m_clipNode;
	Armature* m_armPoint;
	Rect m_rcClick;
	map<int, GuideRect*> m_mRcClick;
	int m_iCurIndex;
	int m_iMaxIndex;
	int m_iSkipType;
	int m_iGuideType;
	int m_ipassGuide;
	static char *m_path;
};

#endif //_GuideLayer_H_