//
//  RoleCreateLayer.h
//  TRGame
//
//  Created by Jimmy on 16/04/14.
//
//

#ifndef _ROLECREATELAYER_H_
#define _ROLECREATELAYER_H_

USING_NS_CC;
USING_NS_CC_EXT;

class RoleCreateLayer : public BTLayer, public ui::EditBoxDelegate
{
public:
	static RoleCreateLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	RoleCreateLayer();
	~RoleCreateLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void editBoxReturn(ui::EditBox* editBox);
private:
	void initControl();
	void updateSkill(int iIndex);
	void updateRandName();
	void onHttpRequestCompleted(Node *sender, void *resultdata);

	void callBackCreate(Ref *sender);
	void callBackMale(Ref *sender);
	void callBackFemale(Ref *sender);
	void callBackSKill(Ref *sender);
	void callBackRand(Ref *sender);
private:
	int m_iSex;
	EventDelegate* m_pDelegate;
	Menu* m_pBtnMenu;
	Label* m_labSkillName;
	Label* m_labSkillInfo;
	MenuItemSprite* m_btnMale;
	MenuItemSprite* m_btnFemale;
	map<int, Armature*> m_mArmRole;
	map<int, MenuItemSprite*> m_mbtnSkill;
	map<int, Vec2> m_mSkillPos;
	Sprite* m_spSkillBG;
	ui::EditBox* m_pEditBox;
};

#endif //_ROLECREATELAYER_H_
