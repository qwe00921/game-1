//
//  CheckBoxSprite.h
//  TRGame
//
//  Created by Jimmy on 16/5/31.
//
//

#ifndef _CHECKBOXSPRITE_H_
#define  _CHECKBOXSPRITE_H_

#include "Defines.h"

class CheckBoxSprite : public Sprite
{
public:
	static CheckBoxSprite* create(Label* labText, Ref* target, SEL_MenuHandler selector);
	bool init(Label* labText, Ref* target, SEL_MenuHandler selector);
	static CheckBoxSprite* create(Sprite* spCheckN, Sprite* spCheckS, Label* labText, Ref* target, SEL_MenuHandler selector);
	virtual bool init(Sprite* spCheckN, Sprite* spCheckS, Label* labText, Ref* target, SEL_MenuHandler selector);
	virtual void onEnter();
	virtual void onExit();

	void setTextColor(const Color3B& color3);
	void setChecked(bool bChecked);
	bool getChecked();
private:
	void initControl();
    void initTouches();
	void updateCheck();
private:
	Sprite* m_spCheckN;
	Sprite* m_spCheckS;
	Label* m_labText;

	Ref*       m_pListener;
	SEL_MenuHandler    m_pfnSelector;

	bool m_bIsClick;
	bool m_bChecked;
};


#endif // _CHECKBOXSPRITE_H_