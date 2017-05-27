//
//  PopLayer.hpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/9.
//
//

#ifndef PopLayer_h
#define PopLayer_h

#include "Defines.h"

typedef enum 
{
	POP_ZM_WJ,//招募武将
	POP_WJ_JJ,//武将进阶
	POP_SP_REF,//商店刷新提示
	POP_PUB_REF,//酒馆刷新提示
	POP_FWQ_WH,//服务器维护
	POP_BUY_WARCOUNT,//购买副本次数
	POP_REQ_AGAIN,//再次请求服务器
	POP_JB_KYM,//剧本可以吗
	POP_EQ_RECAST,//是否重铸装备
	POP_YZ_BOX,//远征宝箱
	POP_UPDATE_FAIL,//更新失败
	POP_RESIGN,//补签提示
	POP_GLC_CHEST,//高狼城宝箱
	POP_GLC_REWARD,//高狼城守关奖励
	POP_CLAN_APPLY_CANCLE,//拒绝入会请求
	POP_CLAN_APPLY_SURE,//同意入会请求
	POP_CLAN_DELETE,//退出或解散工会
	POP_DELETE_CLAN,//不是工会成员，退出主界面
	POP_OPEN_CLANFB,//开启工会副本
	POP_CLAN_ADDGOODS,//公会本结算
	POP_PRAC_GROWN,//武将成长修炼
	POP_NOTICE,//仅仅提示
	POP_UNLOAD_ENCH,//卸下附魔石
} PopType;

class PopLayer : public BTLayer
{
public:
    
    /*
     pType 界面类型
     delegate
     fruitType 果子的类型//在果子界面中用到
     */
	static PopLayer* create(PopType pType, EventDelegate* delegate, int fruitType = 0, int iPriority = LAYER_POPUP);
	virtual bool init(PopType pType, EventDelegate* delegate, int fruitType, int iPriority);
    
    PopLayer();
    ~PopLayer();
    void onEnter();
    void onExit();
    
    void cancelMenu(Ref* node);
    void sureMenu(Ref* node);
	void OnlyRemoveThis();
	void setTitle(const char* chTitle);
	void setTag(string strTag);
private:
	PopType pType;
	EventDelegate* delegate;
	int mGoldId;
	int m_iPriority;

	Menu* cancel;
	Menu* sure;

	bool m_bIsRemove;
	Label* m_labTitle;
	string m_strTag;
};

#endif /* PopLayer_hpp */
