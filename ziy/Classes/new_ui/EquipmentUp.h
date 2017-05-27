#ifndef _EQUIPMENTUP_H_
#define _EQUIPMENTUP_H_

#include "Defines.h"
#include "BoxListLayer.h"
#include "CheckBoxSprite.h"

class EquipLayer : public Layer
{
public:
	virtual void updateLayer(HistoryGoods* ptrGoods) = 0;
	virtual void updateLayer() {}
	virtual Sprite* getInfoBG() = 0;
	virtual Sprite* getCurStar() = 0;
	virtual int getSelectIndex() = 0;
};

//强化层
class EqStrengLayer : public EquipLayer
{
public:
	static EqStrengLayer* create();
	virtual bool init();
	virtual void updateLayer(HistoryGoods* ptrGoods);
	virtual Sprite* getInfoBG();
	virtual Sprite* getCurStar();
	virtual int getSelectIndex();
private:
	void initControl();
private:
	HistoryGoods* m_ptrGoods;
	Sprite* m_spInfo;
	Sprite* m_spImg;
	Armature* m_armQualy;
	Sprite* m_spArrow;
	Sprite* m_spGoldUse;
	Sprite* m_spStoneUse;

	Label* m_labStone;
	Label* m_labGold;
	Label* m_labName;
	Label* m_labType;
	Label* m_labAttr;
	Label* m_labAttrVal;
	Label* m_labEffVal;
	Label* m_labStoneUse;
	Label* m_labGoldUse;
	Label* m_labSucceed;
	Label* m_labSucVal;

	map<int, Sprite*> m_spStars; //星星数组
	map<int, Label*> m_mNorAttr; //普通属性加成
	map<int, Label*> m_mSpeAttr;
	map<int, string> m_mSattrText;
};

//重铸层
class EqRecastLayer : public EquipLayer
{
public:
	static EqRecastLayer* create();
	virtual bool init();
	virtual void updateLayer(HistoryGoods* ptrGoods);
	virtual void updateLayer();
	virtual Sprite* getInfoBG();
	virtual Sprite* getCurStar();
	virtual int getSelectIndex();
private:
	void initControl();
	void updateCheckBox();
	void callBackCheck(Ref *sender);
private:
	HistoryGoods* m_ptrGoods;
	int m_iSelectIndex;
	Sprite* m_spInfo;
	Sprite* m_spImg;
	Sprite* m_spStar;
	Armature* m_armQualy;
	Sprite* m_spJadeUse;

	Label* m_labJade;
	Label* m_labName;
	Label* m_labType;
	Label* m_labAttr;
	Label* m_labAttrVal;
	Label* m_labEffVal;
	Label* m_labJadeUse;

	map<int, Label*> m_mSpeAttr;
	map<int, string> m_mSattrText;
	map<int, CheckBoxSprite*> m_mCheckBox;
};

//附魔层
class EqEnchantLayer : public EquipLayer, public EventDelegate
{
public:
	static EqEnchantLayer* create();
	virtual bool init();
	virtual void updateLayer(HistoryGoods* ptrGoods);
	virtual void updateLayer();
	virtual Sprite* getInfoBG();
	virtual Sprite* getCurStar();
	virtual int getSelectIndex();
	virtual void updateShow(int index);
private:
	void initControl();

	void callBackStone(Ref *sender);
	void callBackSelect(Ref *sender);
private:
	HistoryGoods* m_ptrGoods;
	int m_iSelectIndex;
	int m_iStoneId;
	Armature* m_armQualy;
	Sprite* m_spBG;
	Sprite* m_spInfo;
	Sprite* m_spImg;
	Sprite* m_spStar;
	Sprite* m_spStoneBG;
	Sprite* m_spStone;
	Sprite* m_spSelect;

	Label* m_labStone;
	Label* m_labGold;
	Label* m_labName;
	Label* m_labType;
	Label* m_labAttr;
	Label* m_labAttrVal;
	Label* m_labEffVal;
	Label* m_labStoneNum;
	Label* m_labNotice;

	MenuItemSprite* m_btnStone;
	MenuItemSprite* m_btnReplace;
};

class EquipmentUp : public BTLayer
{
public:
	static EquipmentUp* create(HistoryGoodsPtr curGoods = NULL);
    virtual bool init(HistoryGoodsPtr curGoods = NULL);
	EquipmentUp();
	~EquipmentUp();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void updateShow(int index);
	virtual void callBackConfirm();
	virtual void updateMoneyShow();
	virtual void RunFunction(int iNomal);
private:
	void initControl();
	map<int, boxModel*> getModels(int iSetBox = -1);
	void updateEquipBox(int iSetBox = -1);
	void updateCurModel(int iModel, HistoryGoodsPtr curGoods = NULL);
	int SetCurGoodsPage(HistoryGoodsPtr curGoods);
	void setEqTouchEnabled(bool bEnabled);

	void onHttpRequest();
	void UnLoadEnchantonHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onStrengSucceed(Json::Value resVal);
	void onRecastSucceed(Json::Value resVal);
	void onEnchantSucceed(Json::Value resVal);
	void onUnLoadEnchantSucceed(Json::Value resVal);

	void callBackLast(Ref *sender);
	void callBackNext(Ref *sender);
	void callBackModel(Ref *sender);
	void callBackStreng(Ref *sender);
	void callBackStreng10(Ref *sender);
	void callBackRecast(Ref *sender);
	void callBackEnchant(Ref *sender);
	void callBackUnLoadEnchant(Ref *sender);
	void callBackClose(Ref *sender);
	void callBackNull(Ref *sender);
private:
	int m_times;
	HistoryGoods* m_ptrGoods;
	int m_iModel;
	int m_iCurIndex;
	int m_iCurPage;
	int m_iMaxPage;
	Label* m_labPage;
	BoxListLayer* m_layBox;
	map<int, MenuItemSprite*> m_mMenuType;
	map<int, HistoryGoods*> m_mTreasure;
	map<int, EquipLayer*> m_mEqLayer;
	map<int, MenuItemSprite*> m_mEquip;
	MenuItemSprite* m_UnloadEnchItem;

	Menu* m_menuNull;
	int m_iTouchEnabled;
};

#endif //_EQUIPMENTUP_H_
