//
//  Net_HomePage.hpp
//  TRGame
//
//  Created by 袁文彬 on 16/4/13.
//
//

#ifndef Net_HomePage_h
#define Net_HomePage_h

#include "Defines.h"
#include "ArmorListTableView.h"

#ifndef ENABLE_GM
	#define ENABLE_GM//客户端GM指令开关
#endif // !ENABLE_GM

enum
{
	_DestroySixCountries=0 ,//游历天下
	_Romance ,//演义堂
	_Mail,//邮件
	_GlobalPrompt,//英雄志
	_GoodsShop,//商店
	_PubDrink,//酒馆
	_DailyScene,//日常副本
	_BattleLadder,//过关斩将
	_WorldRank,//排行榜
	_GoodsStore,//货郎
};

class HongDian;

class Net_HomePage : public BTLayer, public ui::EditBoxDelegate
{
public:
	static Net_HomePage* create(bool enable_initdate=true);
	static void InitHistory();
    virtual bool init();
	Net_HomePage();
	~Net_HomePage();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void editBoxReturn(ui::EditBox* editBox);
	virtual void updateState();
	virtual void updateRTDate(map<int, string> mStrNotice);
	void playAnimation(string name, int type);
private:
	void initControl();
	void menuBack(Ref* obj);
	void addActSceneMenu();
	void callBackMenu();
	void updateMenu(bool bVisible);
	void updateMailState();

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onVipGiftSucceed(Json::Value valRes);
	bool onActSceneSucceed(Json::Value valRes);
	bool onMailInfoSucceed(Json::Value valRes);
	void PlayHomeBgMp3(float f);
	void setEnterStatus();
	
private:
#ifdef ENABLE_GM
	ui::EditBox* m_pEditBox;
#endif // ENABLE_GM

    MenuItemSprite* generals;
    MenuItemSprite* forging;
    MenuItemSprite* knapsack;
	MenuItemSprite* add;
	MenuItemSprite* shoppingMall;
	MenuItemSprite* m_btnVipGift;
	MenuItemSprite* clan;
	Sprite* m_spBG1;
	Sprite* m_spBG2;
	Sprite* m_spBG3;
	Sprite* m_spBG4;
	
	Sprite* m_spMailMark; //邮件未领取状态
	//图标否开启
	bool IsOpen(int type,bool popTips);


	Menu* m_pBtnMenu;
	map<int, Armature*> m_mArmMode;
	map<int, Rect> m_mArmRect;

	bool isOpen;//用于判断＋菜单的状态
	bool mEnableInitDate;
	int m_iClickMode;
	static float ms_fOffsetX;
	Size m_szScreen;

	HongDian* m_hongdian;
};
/*
小红点系统使用观察者模式
使用方法：addHongDianNode(Node *node,int TypeNum)将节点加入到观察者队列当中，@node表示需要加入的节点@type表示节点的索引key值
使用autoupdate更新主界面
使用autoUpdateWuJiang更新武将的红点
*/
class HongDianNode : public Node
{
public:
	CREATE_FUNC(HongDianNode);
	virtual void onExit();
};
enum
{
	JINENG_HONGDIAN, //0技能红点 
	ANNIU_HONGDIAN,//1技能按钮红点
	WUJIANG_JIEMIAN, // 2武将界面 
	ZHUJIEMIANN_HONGDIAN//3主界面
};
class HongDian : public BTSprite
{
public:
	CREATE_FUNC(HongDian);
	virtual bool init();
	static HongDian* getInstence();
	~HongDian();
	/*给一个图标添加红点节点
	node需要添加红点节点的对象
	type红点节点的map-key编号
	*/
	void addHongDianNode(Node *node,int TypeNum);
	/*
	通过key显示红点
	*/
	void showHongDian(int TypeNum);

	/*
	通过key隐藏红点
	*/
	void invisibleHongDian(int TypeNum);

	void autoUpdate();//自动更新主界面红点
	void autoUpdateBaoguo();//自动更新包裹红点
	void autoUpdateWuJiang(int type );//自动更新武将红点  2武将界面 3主界面
	void autoUpadteInfo(int heroID,int type);//技能更新  0技能红点 1技能按钮红点

	/*更新装备升级界面红点 
		#type0装备升级界面 1主界面
	*/
	void autoUpdateEquep(int type=0);
	
	void autoUpdateHeCheng(int type=0);//合成

	void WuJiangVisible();
	void BeiBaoVisible();
	void DuanZaoVisible();
	void GongHuiVisible();
	void HeChengVisible();

	static map<int, int> novisible;
	map<int, int > HongDianNodeisVisibol;
	map<int, Node *> HongDianNode;
	map<int, ArmorListInfo*> HongDianelem;
private:
	static HongDian *m_iHongDian;
	//static HongDian *
	

};

#endif /* Net_HomePage_h */
