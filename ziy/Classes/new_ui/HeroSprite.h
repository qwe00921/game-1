#ifndef __TRGAME_HERO_SPRITE_H__
#define  __TRGAME_HERO_SPRITE_H__

#include "Defines.h"

class HeroSprite : public BTSprite
{
public:
	static HeroSprite* create(int iHeroId, int iQuality = 0, float fScale = 0.8f);
	virtual bool init(int iHeroId, int iQuality, float fScale);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void setSupDrag(bool bIsDrag);
private:
	void initControl();
private:
	int m_iHeroId;
	int m_iQuality;
	float m_fScale;

	bool m_bIsClick;
	bool m_bIsDrag;
	Rect m_rcBG;
	Vec2 m_posTouch;
};

class HeroInfoLayer:public BTLayer
{
public:
	static HeroInfoLayer* create(int iHeroId,int iLv = 1);
	bool init(int iHeroId,int iLv = 1);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	int addSkillElem(Sprite* BgSp);
private:
	int m_iHeroid;
	int m_iLv;
};

#endif // __TRGAME_HERO_SPRITE_H__