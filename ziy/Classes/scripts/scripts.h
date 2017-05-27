#ifndef _Scripts_H_
#define _Scripts_H_
typedef enum
{
	CMD_ACTION_END,//0:事件结束
	CMD_CHILD_ACTION,//1:子事件设定
	CMD_CHILD_INFO_ACTION,//2:子事件设定带信息
	CMD_ELSE,//3:else
	CMD_REQUEST_TEST,//4:询问测试
	CMD_TEST_VALUE, //5:变量测试
	CMD_FORCE_LIMIT,//6:出场限制
	CMD_TEST_START_BATTLE,//7:出战测试
	CMD_MENU_VISIBLE,//8:菜单处理
	CMD_DELAY_SET,//9:延时
	CMD_INIT_VALUE,//a:初始化局部变量
	CMD_SET_VALUE,//b:变量赋值
	CMD_c,//c:结束Section
	CMD_BATTLE_END,//d:结束Scene
	CMD_BATTLE_FAIL,//e:战斗失败
	CMD_GAME_ENDING,//f:结局设定
	CMD_10,//10:许子将指导
	CMD_SCENE_CHANGE,//11:剧本跳转
	CMD_SHOW_CHOICE,//12:选择框
	CMD_CASE,//13:case
	CMD_ACTOR_TALK,//14:对话
	CMD_ACTOR_TALK2,//15:对话2
	CMD_NORMAL_INFO,//16:信息
	CMD_SCENE_NAME,//17:场所名
	CMD_EVENT_NAME,//18:事件名称设定
	CMD_VICTORY_CONDITION,//19:胜利条件
	CMD_SHOW_VICTORY_CONDITION,//1a:显示胜利条件
	CMD_LEAVE_MSG_SHOW_OR_NOT,//1b:撤退信息是否显示设定
	CMD_DRAW,//1c:绘图
	CMD_1d,//1d:调色板设定
	CMD_1e,//1e:武将重绘
	CMD_CHANGE_VIEWPOINT,//1f:地图视点切换
	CMD_HEAD_STATE_SET,//20:武将头像状态设置
	CMD_ADD_MAP_ELEM,//21:战场物体添加
	CMD_MOVIE_SET,//22:动画
	CMD_SOUND_SET,//23:音效
	CMD_MUSIC_SET,//24:CD音轨
	CMD_TEST_ACTOR_POS,//25:武将进入地点测试
	CMD_TEST_ACTOR_AREA,//26:武将进入指定区域测试
	CMD_BG_SET,//27:背景显示
	CMD_28,//28:?三国地图显示？
	CMD_MAP_FACE_APPEAR,//29:地图头像显示
	CMD_MAP_FACE_MOVE,//2a:地图头像移动
	CMD_MAP_FACE_DISAPPEAR,//2b:地图头像消失
	CMD_MAP_TELL_INFO,//2c:地图文字显示
	CMD_TEST_ACTOR_CLICKED,//2d:武将点击测试
	CMD_TEST_ACTOR_BORDER,//2e:武将相邻测试
	CMD_CLEAR_ACTOR,//2f:清除人物
	CMD_ACTOR_APPEAR,//30:武将出现
	CMD_ACTOR_DISAPPEAR,//31:武将消失
	CMD_ACTOR_MOVE,//32:武将移动
	CMD_ACTOR_TURN,//33:武将转向
	CMD_ACTOR_ACTION,//34:武将动作
	CMD_ACTOR_CHANGE_IMAGE,//35:武将形象改变
	CMD_TEST_ACTOR_STATE,//36:武将状态测试
	CMD_MONEY_SCENE_ROUTE_TEST,//37:钱、剧本跳转、忠奸测试
	CMD_SET_ACTOR_ABILITY,//38:武将能力设定
	CMD_ACTOR_LEVEL_UP,//39:武将等级提升
	CMD_MONEY_CHANGE,//3a:钱、剧本跳转、忠奸设置
	CMD_ARMY_CHANGE,//3b:武将加入和离开
	CMD_TEST_ARMY_COME,//3c:武将加入测试
	CMD_GET_GOOD,//3d:获得物品
	CMD_GOOD_SET,//3e:加入装备设定
	CMD_TEST_ROUND,//3f:回合测试
	CMD_TEST_ACTION_SIDE,//40:行动方测试
	CMD_TEST_ACTOR_NUM,//41:战斗人数测试
	CMD_TEST_VICTORY,//42:战斗胜利测试
	CMD_TEST_FAIL,//43:战斗失败测试
	CMD_BATTLE_INIT,//44:战斗初始化
	CMD_BATTLE_SET_VALUE,//45:战场全局变量
	CMD_DISPATCH_FRIEND,//46:友军出场设定
	CMD_DISPATCH_ENEMY,//47:敌军出场设定
	CMD_SET_ENEMY_GOODS,//48:敌军装备设定
	CMD_49,//49:战斗结束
	CMD_CONFIG_OWN_FORCE,//4a:我军出场强制设定
	CMD_DISPATCH_OWN,//4b:我军出场设定
	CMD_SET_ACTOR_VISIBLE,//4c:隐藏武将出现
	CMD_CHANGE_ACTOR_STATE,//4d:武将状态变更
	CMD_CHANGE_ACTOR_AI,//4e:武将方针变更
	CMD_BATTLE_ACTOR_TURN,//4f:战场转向设置
	CMD_BATTLE_ACTOR_ACTION,//50:战场动作设置
	CMD_BATTLE_FIGHT_END,//51:战场恢复行动权
	CMD_CHANGE_ACTOR_OCCU,//52:兵种改变
	CMD_BATTLE_ACTOR_LEAVE,//53:战场撤退
	CMD_BATTLE_LEAVE_COMFORM,//54:战场撤退确认
	CMD_BATTLE_ACTOR_RELIVE,//55:战场复活
	CMD_SET_AIR_TYPE,//56:天气类别设定
	CMD_SET_CUR_AIR,//57:当前天气设定
	CMD_SET_BATTLE_BARRIER,//58:战场障碍设定
	CMD_SET_BATTLE_BONUS,//59:战利品设定
	CMD_BATTLE_START,//5a:战场操作开始
	CMD_BATTLE_HIGHLIGHT_AREA,//5b:战场高亮区域
	CMD_BATTLE_HIGHLIGHT_ACTOR,//5c:战场高亮人物
	CMD_SET_MAX_ROUND,//5d:回合上限设定
	CMD_TEST_ACTOR_NOT_SAME,//5e:武将不同测试
	CMD_PK_END,//5f:单挑结束
	CMD_PK_ACTOR_APPEAR,//60:单挑武将出场
	CMD_PK_SHOW_K_O,//61:单挑胜负显示
	CMD_PK_ACTOR_DIE,//62:单挑阵亡
	CMD_PK_ACTOR_TALK,//63:单挑对话
	CMD_PK_ACTION,//64:单挑动作
	CMD_PK_ATTACK1,//65:单挑攻击1
	CMD_PK_ATTACK2,//66:单挑攻击2
	CMD_CHAPTER_NAME,//67:章名
	CMD_PK_START,//68:单挑开始
	CMD_69,//69:旁白
	CMD_6a,//6a:
	CMD_CALL_MAGIC,//6b:法术召唤
	CMD_ACTOR_ABILITY_COPY,//6c:武将能力复制
	CMD_RELA_REVIVEORMOVE,//6d:相对复活或移动
	CMD_TEST_PROBALITY,//6e:概率测试
	CMD_GOOD_THROW,//6f:丢弃物品
	CMD_ABILITY_CHOOSE_COPY,//70:能力选择复制
	CMD_ASK_EFFECTS,//71:特效请求
	CMD_MSG_TRANSMIT,//72:信息传送
	CMD_ACTOR_PROPERTY,//73:人物五围和测试
	CMD_SWITCH_SAVEFILE,//74:开/禁存档
	CMD_ASSIGN_SIMAGE,//75:S特殊形象指定
	CMD_SKIP_UNCONDITION,//76:无条件跳转
	CMD_VALUE_OPERATION,//77:变量运算
	CMD_VALUE_INT_VALUATION,//78:整形变量赋值
	CMD_TEST_VALUEPLUS,//79:加强版变量测试
	CMD_EXTEND_TEST122, //7a:测试BOSS血量变化
	CMD_MAX
}CMD_ID;

DFC_DCP1(Script)
DFC_DCP1(ScriptHandler)
DFC_DCP1(ScriptActorAction)
DFC_DCP1(ScriptActorAppear)
DFC_DCP1(ScriptActorMove)
DFC_DCP1(ScriptActorTalk2)
DFC_DCP1(ScriptActorTalk)
DFC_DCP1(ScriptActorTurn)
DFC_DCP1(ScriptArmyChange)
DFC_DCP1(ScriptBattleActorAction)
DFC_DCP1(ScriptBattleActorLeave)
DFC_DCP1(ScriptBattleActorTurn)
DFC_DCP1(ScriptBGSet)
DFC_DCP1(ScriptCallMagic)
DFC_DCP1(ScriptCase)
DFC_DCP1(ScriptChangeActorState)
DFC_DCP1(ScriptChildInfo)
DFC_DCP1(ScriptCommonInfo)
DFC_DCP1(ScriptConfigOwnForce)
DFC_DCP1(ScriptDelaySet)
DFC_DCP1(ScriptDispatchEnemy)
DFC_DCP1(ScriptDispatchFriend)
DFC_DCP1(ScriptSetEnemyGoods)
DFC_DCP1(ScriptDispatchOneEnemy)
DFC_DCP1(ScriptDispatchOneFriend)
DFC_DCP1(ScriptDispatchOwn)
DFC_DCP1(ScriptGoodSet)
DFC_DCP1(ScriptMenuVisible)
DFC_DCP1(ScriptMoneyChange)
DFC_DCP1(ScriptMusicSet)
DFC_DCP1(ScriptSetActorVisible)
DFC_DCP1(ScriptSetBattleValue)
DFC_DCP1(ScriptSetValue)
DFC_DCP1(ScriptShowChoice)
DFC_DCP1(ScriptSoundSet)
DFC_DCP1(ScriptTestActionSide)
DFC_DCP1(ScriptTestActorArea)
DFC_DCP1(ScriptTestActorBorder)
DFC_DCP1(ScriptTestActorClicked)
DFC_DCP1(ScriptTestActorPos)
DFC_DCP1(ScriptTestActorState)
DFC_DCP1(ScriptTestRound)
DFC_DCP1(ScriptTestValue)
DFC_DCP1(ScriptAddMapElem)
DFC_DCP1(ScriptDraw)
DFC_DCP1(ScriptPaletteSet)
DFC_DCP1(ScriptActorDisappear)
DFC_DCP1(ScriptGetGood)
DFC_DCP1(ScriptBattleActorRelive)
DFC_DCP1(ScriptClearActor)

DFC_DCP1(ScriptPKActorAction)
DFC_DCP1(ScriptPKActorAppear)
DFC_DCP1(ScriptPKActorAttack1)
DFC_DCP1(ScriptPKActorAttack2)
DFC_DCP1(ScriptPKActorDie)
DFC_DCP1(ScriptPKActorTalk)
DFC_DCP1(ScriptPKEnd)
DFC_DCP1(ScriptPKShowKO)
DFC_DCP1(ScriptPKStart)
DFC_DCP1(ScriptBattleEnd)
DFC_DCP1(ScriptBattleStart)
DFC_DCP1(ScriptChapterName)
DFC_DCP1(ScriptMapFaceAppear)
DFC_DCP1(ScriptMapFaceDisAppear)
DFC_DCP1(ScriptMapFaceMove)
DFC_DCP1(ScriptMapTellInfo)
DFC_DCP1(ScriptTestStartBattle)
DFC_DCP1(ScriptForceLimit)
DFC_DCP1(ScriptBattleFightEnd)
DFC_DCP1(ScriptBattleFail)
DFC_DCP1(ScriptGameEnding)

DFC_DCP1(ScriptSceneChange)

DFC_DCP1(ScriptLeaveMsgShowOrNot)
DFC_DCP1(ScriptChangeViewPoint)
DFC_DCP1(ScriptHeadStateSet)
DFC_DCP1(ScriptMovieSet)
DFC_DCP1(ScriptTestArmyCome)
DFC_DCP1(ScriptChangeActorAI)
DFC_DCP1(ScriptChangeActorOccu)
DFC_DCP1(ScriptBattleLeaveComform)
DFC_DCP1(ScriptSetAirType)

DFC_DCP1(ScriptSetCurAir)
DFC_DCP1(ScriptSetBattleBarrier)
DFC_DCP1(ScriptSetBattleBonus)
DFC_DCP1(ScriptBattleHighlightArea)
DFC_DCP1(ScriptBattleHighlightActor)
DFC_DCP1(ScriptSetMaxRound)
DFC_DCP1(ScriptTestActorNotSame)
DFC_DCP1(ScriptMoneyTest)
DFC_DCP1(ScriptActorChangeImage)
DFC_DCP1(ScriptSetActorAbility)
DFC_DCP1(ScriptActorLevelUp)
DFC_DCP1(ScriptTestActorNum)
DFC_DCP1(ScriptTestVictory)
DFC_DCP1(ScriptTestFail)
DFC_DCP1(ScriptElse)

DFC_DCP1(ScriptActorAbilityCopy)
DFC_DCP1(ScriptRelaReviveOrMove)
DFC_DCP1(ScriptTestProbality)
DFC_DCP1(ScriptAbilityChooseCopy)
DFC_DCP1(ScriptAskEffects)
DFC_DCP1(ScriptGoodThrow)
DFC_DCP1(ScriptActorProperty)
DFC_DCP1(ScriptSwitchSavefile)
DFC_DCP1(ScriptMsgTransmit)
DFC_DCP1(ScriptAssignSImage)
DFC_DCP1(ScriptSkipUncondition)
DFC_DCP1(ScriptValueOperation)
DFC_DCP1(ScriptValueIntValuation)
DFC_DCP1(ScriptTestValuePlus)
DFC_DCP1(ScriptFreeMove)
DFC_DCP1(ScriptTestAsk)
DFC_DCP1(ScriptInitValue)
DFC_DCP1(ScriptSectionEnd)
DFC_DCP1(ScriptRunAction)

#include "main.h"
#include "script.h"

#include "Script40_28_FreeMove.h" 
#include "Script02_02_ChildInfo.h" 
#include "Script03_03_Else.h" 
#include "Script04_04_TestAsk.h" 
#include "Script05_05_TestValue.h" 
#include "Script06_06_ForceLimit.h" 
#include "Script07_07_TestStartBattle.h" 
#include "Script08_08_MenuVisible.h" 
#include "Script09_09_DelaySet.h"
#include "Script10_0a_InitValue.h"
#include "Script12_0c_SectionEnd.h"
#include "Script100_64_PKActorAction.h" 
#include "Script101_65_PKActorAttack1.h" 
#include "Script102_66_PKActorAttack2.h" 
#include "Script103_67_ChapterName.h" 
#include "Script104_68_PKStart.h" 
#include "Script107_6b_CallMagic.h" 
#include "Script11_0b_SetValue.h" 
#include "Script13_0d_BattleEnd.h" 
#include "Script14_0e_BattleFail.h" 
#include "Script15_0f_GameEnding.h" 
#include "Script17_11_SceneChange.h" 
#include "Script18_12_ShowChoice.h" 
#include "Script19_13_Case.h" 
#include "Script20_14_ActorTalk.h" 
#include "Script21_15_ActorTalk2.h" 
#include "Script22_16_CommonInfo.h" 
#include "Script27_1b_LeaveMsgShowOrNot.h" 
#include "Script28_1c_Draw.h" 
#include "Script29_1d_PaletteSet.h" 
#include "Script31_1f_ChangeViewPoint.h" 
#include "Script32_20_HeadStateSet.h" 
#include "Script33_21_AddMapElem.h" 
#include "Script34_22_MovieSet.h" 
#include "Script35_23_SoundSet.h" 
#include "Script36_24_MusicSet.h" 
#include "Script37_25_TestActorPos.h" 
#include "Script38_26_TestActorArea.h" 
#include "Script39_27_BGSet.h" 
#include "Script41_29_MapFaceAppear.h" 
#include "Script42_2a_MapFaceMove.h" 
#include "Script43_2b_MapFaceDisAppear.h" 
#include "Script44_2c_MapTellInfo.h" 
#include "Script45_2d_TestActorClicked.h" 
#include "Script46_2e_TestActorBorder.h" 
#include "Script47_2f_ClearActor.h" 
#include "Script48_30_ActorAppear.h" 
#include "Script49_31_ActorDisappear.h" 
#include "Script50_32_ActorMove.h" 
#include "Script51_33_ActorTurn.h" 
#include "Script52_34_ActorAction.h" 
#include "Script53_35_ActorChangeImage.h" 
#include "Script54_36_TestActorState.h" 
#include "Script55_37_MoneyTest.h" 
#include "Script56_38_SetActorAbility.h" 
#include "Script57_39_ActorLevelUp.h" 
#include "Script58_3a_MoneyChange.h" 
#include "Script59_3b_ArmyChange.h" 
#include "Script60_3c_TestArmyCome.h" 
#include "Script61_3d_GetGood.h" 
#include "Script62_3e_GoodSet.h" 
#include "Script63_3f_TestRound.h" 
#include "Script64_40_TestActionSide.h" 
#include "Script65_41_TestActorNum.h" 
#include "Script66_42_TestVictory.h" 
#include "Script67_43_TestFail.h" 
#include "Script69_45_SetBattleValue.h" 
#include "Script70_46_DispatchFriend.h" 
#include "Script71_47_DispatchEnemy.h" 
#include "Script72_48_SetEnemyGoods.h" 
#include "Script74_4a_ConfigOwnForce.h" 
#include "Script75_4b_DispatchOwn.h" 
#include "Script76_4c_SetActorVisible.h" 
#include "Script77_4d_ChangeActorState.h" 
#include "Script78_4e_ChangeActorAI.h" 
#include "Script79_4f_BattleActorTurn.h"
#include "Script80_50_BattleActorAction.h" 
#include "Script81_51_BattleFightEnd.h" 
#include "Script82_52_ChangeActorOccu.h" 
#include "Script83_53_BattleActorLeave.h" 
#include "Script84_54_BattleLeaveComform.h" 
#include "Script85_55_BattleActorRelive.h" 
#include "Script86_56_SetAirType.h" 
#include "Script87_57_SetCurAir.h"
#include "Script88_58_SetBattleBarrier.h" 
#include "Script89_59_SetBattleBonus.h" 
#include "Script90_5a_BattleStart.h" 
#include "Script91_5b_BattleHighlightArea.h" 
#include "Script92_5c_BattleHighlightActor.h" 
#include "Script93_5d_SetMaxRound.h" 
#include "Script94_5e_TestActorNotSame.h" 
#include "Script95_5f_PKEnd.h" 
#include "Script96_60_PKActorAppear.h" 
#include "Script97_61_PKShowKO.h" 
#include "Script98_62_PKActorDie.h" 
#include "Script99_63_PKActorTalk.h" 
#include "Script108_6c_ActorAbilityCopy.h" 
#include "Script109_6d_RelaReviveOrMove.h" 
#include "Script110_6e_TestProbality.h" 
#include "Script111_6f_GoodThrow.h" 
#include "Script112_70_AbilityChooseCopy.h" 
#include "Script113_71_AskEffects.h" 
#include "Script114_72_MsgTransmit.h" 
#include "Script115_73_ActorProperty.h" 
#include "Script116_74_SwitchSavefile.h" 
#include "Script117_75_AssignSImage.h" 
#include "Script118_76_SkipUncondition.h" 
#include "Script119_77_ValueOperation.h" 
#include "Script120_78_ValueIntValuation.h" 
#include "Script121_79_TestValuePlus.h" 
#include "ScriptDispatchOneEnemy.h" 
#include "ScriptDispatchOneFriend.h" 
#include "ScriptRunAction.h"

#endif