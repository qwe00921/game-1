#ifndef _Main_H_
#define _Main_H_


DFC_DCP1(HistoryGoods)
DFC_DCP1(History)
DFC_DCP1(HistoryActor)
DFC_DCP1(Chess)
DFC_DCP1(Screen)
DFC_DCP1(TRControl)
DFC_DCP1(Button)
DFC_DCP1(BattleScreen)
DFC_DCP1(RedeScreen)
DFC_DCP1(BattleControler)
DFC_DCP1(BattleAI)
DFC_DCP1(BattleActor)
DFC_DCP1(MapElem)
DFC_DCP1(PKScreen)
DFC_DCP1(ImageButton)
DFC_DCP1(MiniMap)
DFC_DCP1(BattleRoundStart)

#include "Data.h"
#include "HistoryGoods.h"
#include "HistoryActor.h"
#include "History.h"
#include "GameMain.h"
#include "GameRes.h"
#include "BattleActor.h"
#include "DControl.h"
#include "Screen.h"
#include "Message.h"
#include "BattleAI.h"
#include "scripts.h"
#include "ScriptHandler.h"
#include "BattleControler.h"
#include "FunctionClass.h"

extern void StartChessTimer(int ticks);
extern void StopChessTimer();
extern void ChessAddString(ChessStringPtr chess_str);

void ChessSetWindowSize(int i);
void ChessExit();
void ChessChangeStyle(int game_style);
void ChessMoveScreen(int dx,int dy);
void ChessSetCursor(int cursor_id);
void ChessMin();
void ChessGetCurSorPos(int *x,int *y);
char* LL(const char* id, bool bIsReload = false, bool bIsSafe = false);
void addSpriteGray(Node* sprite);
void addSpriteGrayAndChild(Node* sprite, int child=-1);
void removeSpriteGray(Node* sprite);
int MyMain();
void MyMainAndroid();
void buyItemResultMain(const char* productID, const char* chReceipt, const char* chRecNum, bool result, bool bRestored);
void BgAddFashionSImg(int iActorId, std::map<int, int> mFashion, Vec2 iPoint, int iImgIndex, Node* Bg, Sprite** fashionSp, Sprite** wapfashionSp, std::string typeS, int iXHalf = 0, int iYHalf = 0);


#define FRAME_X_OFFSET 0
#define FRAME_Y_OFFSET 55

#define LOGINLAYER 889
#define FAIL_RECORD "failrecord.info"
#define RES_PATH "/mnt/sdcard/bantu/Resources/"
#define RES_PATH2 "/sdcard/bantu/Resources/"
#define SWEEP_VOLUME_ID 2018

enum
{
	CURSOR_ARROW,
	CURSOR_MOVE,
	CURSOR_NORMAL,
		CURSOR_HAND,
		CURSOR_WAIT_1,
		CURSOR_WAIT_2
};

inline const char * getCurrentSenceTitle()
{
    return trData->m_mNetWarInfo[history->m_iCurWarId]->strName.c_str();	
}
#endif
