#include "Common.h"
#include "main.h"
#include "StatConfig.h"
#include "PaymentTool.h"
#include "UserDate.h"

int cur_has_set = 0;
int minimized = 0;
#define MENU_HEIGHT 55
void SetWindowSize();
//*******************************************************************
//应用程序入口
int MyMain()
{
	return (new ChessApplication())->run();
}
void MyMainAndroid()
{
	new ChessApplication();
}

char* responseToWebView() {
    CCLOG("responseToWebView been called");
    DString str = "this is the response to js";
    char *response = str.getChar();
    return response;
}

void refreshProductListResultMain(const char* productListJson){
    
    //productListJson = "[{"priceLocal":\"CN\",\"productIdentifier\":\"com.bantu.fsyjz.jzthq\",\"price\":12,\"title\":\"全部特殊装备\",\"desc\":\"含有全部特殊装备,各种武器、护甲等\"}]";
    
    printf("here is result in refresh product list result: %s", productListJson);

	trData->trGamePay->loadItunesPrice(productListJson);
    
    if (trData->trGamePay->isItunesPriceLoadSuccess()) {//是否 “全部” 成功刷新价格
        StatConfig::onLoadItunesPriceResult(1);
    }else{
        StatConfig::onLoadItunesPriceResult(0);
    }
}

void refreshProductListErrorMain(int error){
    printf("here is log in refreshProduct list error: %d", error);

	

	if (100!=error){
		printf("refresh error！ code：%d",error);
		//StoreInfo刷新失败 显示按钮
		StatConfig::onLoadItunesPriceResult(0);
	}
	else{
		//StatConfig::onLoadItunesPriceResult(1);
       // data->isLoadItunesPriceSuccess = 1;
	}
	
}

void restoreCompletedTransactionResultInMain(bool result) {
    if (result) {
        //restore success
        CCLOG("LTC print restore success");
		StatConfig::onRestoreIosProductResult(1);
    } else {
        //restore failed with error
        CCLOG("LTC print restore failed");
		StatConfig::onRestoreIosProductResult(0);
    }
}

void buyItemResultMain(const char* productID, const char* chReceipt, const char* chRecNum, bool result, bool bRestored)
{
	if (result)
	{
		UserDate::getInstance()->addReceipt(productID, chReceipt, chRecNum);
		PaymentTool::getInstance()->setIOSReceipt(chReceipt, chRecNum);
	}
	else
	{
		UserDate::getInstance()->deleteReceipt(productID);
	}

	if (!bRestored && history->m_pLoginInfo) //不是恢复状态且已登录
	{
		StatConfig::onPayResult(StatConfig::PAY_TYPE_IOS_APPSTORE, productID, result ? 1 : 0, "IOSPAY");

#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS && !defined(HMW_PAY))
		extern void hideSpinView(void);
		hideSpinView();
#endif
	}
}
void verifyAppResultMain(bool result)
{

}
void StartChessTimer(int ticks)
{
    
/*	SetTimer(ChessWnd,1,ticks,NULL);*/
}
void StopChessTimer()
{
/*	KillTimer(ChessWnd,1);*/
}
void UpdateChessWindow(DRectPtr temp_rect)
{
/*	InvalidateRect(ChessWnd,(CONST RECT *)temp_rect,FALSE);*/
}
void ChessMin()
{
/*	CloseWindow(ChessWnd);*/
}
void ChessExit()
{
// 	DestroyWindow(ChessWnd);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}
//******************************************************************
//创建主窗口。
// HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
// {
//     HWND				hwnd;	
//     WNDCLASS			wc;
// 	ChessPtr chess = Chess::getInstance();
// 	//填充窗口类结构
//     wc.style = CS_HREDRAW | CS_VREDRAW;
//     wc.lpfnWndProc = WinProc;
//     wc.cbClsExtra = 0;
//     wc.cbWndExtra = 0;
//     wc.hInstance = hInstance;
//     wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON) );
//     wc.hCursor = LoadCursor( NULL, IDC_ARROW );//MAKEINTRESOURCE(IDC_CURSOR10) 
//     wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//     wc.lpszMenuName = NULL;
// 	wc.lpszClassName = "sss";
// 	//注册窗口类
//     RegisterClass( &wc );
// 	//创建主窗口
//     hwnd = CreateWindowEx(
// 		0,
// 		"sss",
// 		data->game_name,
// 		//chess_data.game_name,
// 		//WS_POPUP|WS_VISIBLE,
// 		WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CAPTION,
// 		(GetSystemMetrics(SM_CXFULLSCREEN)-screen_size_x)/2,
// 		(GetSystemMetrics(SM_CYFULLSCREEN)-screen_size_y)/2,
// 		//GetSystemMetrics(SM_CXSCREEN),
// 		//screen_size_x,
// 		//screen_size_y,
// 		screen_size_x+2*GetSystemMetrics(SM_CXFRAME),
// 		screen_size_y+GetSystemMetrics(SM_CYCAPTION)+2*GetSystemMetrics(SM_CYFRAME) + FRAME_Y_OFFSET,
//         //GetSystemMetrics(SM_CYSCREEN),
// 		NULL,
// 		NULL,
// 		hInstance,
// 		NULL );
// 	return hwnd;
// }
ChessString chessString[10];
int chessString_Num = 0;
int chessString_Index = 0;
int show_string = 0;
//*****************************
//处理主窗口消息
int show_hand = 0;

int cur_win_state = -1;
// LRESULT CALLBACK WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
// {    
// 	ChessPtr chess = Chess::getInstance();
// 	switch( message )
//     {
// 	case WM_NCLBUTTONDBLCLK:
// 		{
// 			if(cur_win_state == SCREEN_SIZE_BATTLE_MAX)
// 				ChessSetWindowSize(SCREEN_SIZE_BATTLE_NORMAL);
// 			else if(cur_win_state == SCREEN_SIZE_BATTLE_NORMAL)
// 				ChessSetWindowSize(SCREEN_SIZE_BATTLE_MAX);
// 			return 0;
// 		}
// 		break;
// 	case WM_SYSCOMMAND:
// 		if(wParam==SC_MAXIMIZE)
// 		{
// 			if(cur_win_state == SCREEN_SIZE_BATTLE_NORMAL)
// 				ChessSetWindowSize(SCREEN_SIZE_BATTLE_MAX);
// 			return 0;
// 		}
// 		else if(wParam ==SC_RESTORE)
// 		{
// 			if(minimized)
// 			{
// 				if(cur_win_state == SCREEN_SIZE_BATTLE_MAX)
// 				{
// 					ChessSetWindowSize(SCREEN_SIZE_BATTLE_MAX);	
// 					return 0;
// 				}
// 				minimized = 0;
// 			}
// 			else if(cur_win_state == SCREEN_SIZE_BATTLE_MAX)
// 			{
// 				ChessSetWindowSize(SCREEN_SIZE_BATTLE_NORMAL);
// 				return 0;
// 			}
// 		}
// 		else if(wParam == SC_MINIMIZE)
// 		{
// 			minimized = 1;
// 		}
// 		else if(wParam == SC_CLOSE&&history->game_state>GAME_STATE_MENU)
// 		{
// 			chess->appendScreen(new ComformScreen("确定要退出游戏吗？",(ScreenPtr)chess->bar_scr,130));
// 			return 0;
// 		}
// 		break;
// 	case WM_MOUSEMOVE:
// 		{
// 		//	POINT p1;
// 		//	RECT p2;
// 		//	GetCursorPos(&p1);
// 			POINT p1;
// 			RECT p2;
// 			GetCursorPos(&p1);
// 			GetWindowRect(hWnd,&p2);
// 			chess->OnPointerDragged(p1.x - p2.left - GetSystemMetrics(SM_CXFRAME),p1.y - p2.top
// 				- GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME));
// 		}
// 		break;
// 	case WM_RBUTTONDOWN:
// 		{
// 			POINT p1;
// 			RECT p2;
// 			GetCursorPos(&p1);
// 			GetWindowRect(hWnd,&p2);
// 			chess->OnRPointerPressed(p1.x - p2.left - GetSystemMetrics(SM_CXFRAME),p1.y - p2.top
// 				- GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME));
// 		}
// 		break;
// 	case WM_RBUTTONUP:
// 		{
// 			POINT p1;
// 			RECT p2;
// 			GetCursorPos(&p1);
// 			GetWindowRect(hWnd,&p2);
// 			chess->OnRPointerReleased(p1.x - p2.left - GetSystemMetrics(SM_CXFRAME),p1.y - p2.top
// 				- GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME));
// 		}
// 		break;
// 	case WM_LBUTTONDOWN:
// 		{
// 			POINT p1;
// 			RECT p2;
// 			GetCursorPos(&p1);
// 			GetWindowRect(hWnd,&p2);
// 			chess->OnPointerPressed(p1.x - p2.left - GetSystemMetrics(SM_CXFRAME),p1.y - p2.top
// 				- GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME));
// 		}
// 		break;
// 	case WM_LBUTTONUP:
// 		{
// 			POINT p1;
// 			RECT p2;
// 			GetCursorPos(&p1);
// 			GetWindowRect(hWnd,&p2);
// 			chess->OnPointerReleased(p1.x - p2.left - GetSystemMetrics(SM_CXFRAME),p1.y - p2.top
// 				- GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME));
// 		}
// 		break;
// 	case WM_PAINT:
// 		{
// 			{
// 				PAINTSTRUCT ps;
// 				HDC hdc = BeginPaint(hWnd,&ps);
// 				HDC hdcMem = CreateCompatibleDC(hdc);
// 				//if(chess->game_state>1)
// 				hbmp = CreateBitmap(frame_size_x,frame_size_y,1,byte_per_pixel*8,gc1.screenBuf);
// 				SelectObject(hdcMem,hbmp);
// 				BitBlt(hdc,0,0,frame_size_x,FRAME_Y_OFFSET,hdcMem,0,0,SRCCOPY);
// 				DeleteObject(hbmp);
// 				if(!video_playing)
// 				{
// 					hbmp = CreateBitmap(screen_size_x,screen_size_y,1,byte_per_pixel*8,gc.screenBuf);
// 					SelectObject(hdcMem,hbmp);
// 					BitBlt(hdc,FRAME_X_OFFSET,FRAME_Y_OFFSET,screen_size_x,screen_size_y,hdcMem,0,0,SRCCOPY);
// 				}
// 				//else
// 				//{
// 				//	hbmp = CreateBitmap(screen_size_x,screen_size_y,1,byte_per_pixel*8,gc.screenBuf);
// 				//	SelectObject(hdcMem,hbmp);
// 				//	BitBlt(hdc,0,0,screen_size_x,screen_size_y,hdcMem,0,0,SRCCOPY);
// 				//	DeleteObject(hbmp);
// 				//}
// 				DeleteObject(hbmp);
// 				DeleteDC(hdcMem);
// 				EndPaint(hWnd,&ps);
// 			}
// 			if(show_string)
// 			{
// 				int i;
// 				HDC hDC = GetDC(ChessWnd); 
// 				int caps = GetDeviceCaps(hDC,LOGPIXELSY);
// 				if(caps>72)
// 					caps = 72;
// 				HFONT hFont=CreateFont(caps,0,0,0,FW_HEAVY,0,0,0,GB2312_CHARSET, 
//                   OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,"宋体"); 
// 				SelectObject(hDC,hFont); 
// 				SetBkMode(hDC,TRANSPARENT);
// 				for(i = 0;i<chessString_Index;i++)
// 				{
// 					SetTextColor(hDC,chessString[i].color);
// 					TextOut(hDC,chessString[i].x,chessString[i].y,
// 						chessString[i].str, strlen(chessString[i].str));
// 				}
// 				DeleteObject(hFont);
// 			}
// 			return 0;
// 		}
// 	case WM_KEYUP:
// 		{
// 			//auto_run=0;
// 			chess->OnKeyUp(wParam);
// 		}
// 		break;
//     case WM_KEYDOWN://击键消息
//         {
// 			//auto_run = 1;
// 			chess->OnKeyDown(wParam);
// 		}
//         break;
// 	case 	WM_SETFOCUS :
// 		return 0 ;
// 	case 	WM_COMMAND :
// 		return 0 ;
// 	case WM_SETCURSOR:
// 		return TRUE;
// 	case WM_TIMER://定时器消息
// 		if((chessString_Num == chessString_Index)&&!show_string)
// 		{
// 			StopChessTimer();
// 			show_hand = 0;
// 			chess->OnTimer();
// 			cur_has_set = 0;
// 			//if(auto_run)
// 			//	chess->OnPointerReleased(p1.x - p2.left - GetSystemMetrics(SM_CXFRAME),p1.y - p2.top
// 			//		- GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME));
// 		}
// 		else
// 		{
// 			show_string = 1;
// 			StartChessTimer(1000);
// 			
// 			if(chessString_Num == chessString_Index)
// 			{
// 				int i;
// 				for(i = 0;i<chessString_Num;i++)
// 				{
// 					ChessFree(chessString[i].str);
// 				}
// 				chessString_Index = chessString_Num = 0;
// 				show_string = 0;
// 			}
// 			else
// 			{
// 				chessString_Index++;
// 				chess->OnPaint();
// 			}
// 		}
// 		break;
//     case WM_DESTROY://退出消息
// 		chess->ExitGame();
// 		PostQuitMessage(0);
//         break;
// 	case WM_DS_EVENT:
// 		{	
// 			if(pEvent)	
// 			{
// 				LONG eventCode = 0, wParam = 0, lParam = 0;
// 				while(SUCCEEDED(pEvent->GetEvent(&eventCode,&wParam,&lParam,0)))	
// 				{
// 					pEvent->FreeEventParams(eventCode,wParam,lParam);
// 					switch(eventCode)	
// 					{
// 					case EC_COMPLETE:
// 						if(mp3_playing)
// 						{
// 							ChessPlayMp3(mp3_playing);
// 						}
// 						else
// 						{
// 							ChessStopVideo();
// 						}
// 						break;
// 					}
// 				}
// 			}
// 		}
// 		break;
// 	}
// 	//调用缺省消息处理过程
//     return DefWindowProc(hWnd, message, wParam, lParam);
// }

void ChessAddString(ChessStringPtr chess_str)
{
// 	chessString[chessString_Num].x = chess_str->x;
// 	chessString[chessString_Num].y = chess_str->y;
// 	chessString[chessString_Num].str = (char *)ChessAlloc(strlen(chess_str->str)+1);
// 	chessString[chessString_Num].color = chess_str->color;
// 	strcpy(chessString[chessString_Num].str,chess_str->str);
// 	chessString_Num ++;
}

void ChessMoveScreen(int dx,int dy)
{
// 	RECT p2;
// 	GetWindowRect(ChessWnd,&p2);
// 	SetWindowPos(ChessWnd,HWND_TOP,p2.left+dx,p2.top+dy,p2.right - p2.left,p2.bottom - p2.top,0);
}
void ChessSetCursor(int cursor_id)
{
// 	if(cur_has_set)
// 		return;
// 	cur_has_set = 1;
// 	switch(cursor_id) {
// 	case CURSOR_MOVE:
// 		SetCursor(LoadCursor( NULL, IDC_SIZEALL));
// 		break;
// 	case CURSOR_ARROW:
// 		SetCursor(LoadCursor( NULL, IDC_ARROW ));
// 		break;
// 	case CURSOR_NORMAL:
// 		SetCursor(LoadCursor(ChessInstance,MAKEINTRESOURCE(IDC_CURSOR10)));
// 		break;
// 	case CURSOR_HAND:
// 		SetCursor(LoadCursor(ChessInstance,MAKEINTRESOURCE(IDC_CURSOR1)));
// 		break;
// 	case CURSOR_WAIT_1:
// 		SetCursor(LoadCursor(ChessInstance,MAKEINTRESOURCE(IDC_CURSOR11)));
// 		break;
// 	case CURSOR_WAIT_2:
// 		SetCursor(LoadCursor(ChessInstance,MAKEINTRESOURCE(IDC_CURSOR12)));
// 		break;
// 	default:
// 		break;
// 	}	
//	if(cursor_id&&show_hand == 0)
//		SetCursor(LoadCursor(ChessInstance,MAKEINTRESOURCE(IDC_CURSOR10)));
//	else
//	{
//		show_hand = 1;
//		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(32649)));
//	}
}

void SetWindowSize()
{
// 	int full_size_x = GetSystemMetrics(SM_CXFULLSCREEN);
// 	int full_size_y = GetSystemMetrics(SM_CYFULLSCREEN);
// 	switch(cur_win_state)
// 	{
// 	case SCREEN_SIZE_MENU:
// 		{
// 			screen_size_x = 640;
// 			screen_size_y = 400;
// 			frame_size_x = 640;
// 			frame_size_y = 455;
// 		}
// 		break;
// 	case SCREEN_SIZE_REDE:
// 		{
// 			screen_size_x = 640;
// 			screen_size_y = 400;
// 			frame_size_x = 640;
// 			frame_size_y = 455;
// 		}
// 		break;
// 	case SCREEN_SIZE_BATTLE_NORMAL:
// 		{
// 			frame_size_x = 624;
// 			frame_size_y = 439;
// 			screen_size_x = 624;
// 			screen_size_y = 384;
// 		}
// 		break;
// 	case SCREEN_SIZE_BATTLE_MAX:
// 		{
// 			int map_width = history->map_width*48;
// 			int map_height = history->map_height*48;
// 			int full_size_width = full_size_x/48*48;
// 			int full_size_height = (full_size_y-55)/48*48;
// 			screen_size_x = full_size_width > map_width?map_width:full_size_width;
// 			screen_size_y = full_size_height > map_height?map_height:full_size_height;
// 			frame_size_x = screen_size_x;
// 			frame_size_y = screen_size_y+55;
// 		}
// 		break;
// 	}
// 	int x = (full_size_x - frame_size_x - GetSystemMetrics(SM_CYCAPTION))/2;
// 	int y = (full_size_y - frame_size_y)/2;
// 	
// 	SetWindowPos(ChessWnd,HWND_TOP,x,y,frame_size_x+2*GetSystemMetrics(SM_CXFRAME),
// 		frame_size_y+GetSystemMetrics(SM_CYCAPTION)+2*GetSystemMetrics(SM_CYFRAME),0);
// 	byte_per_pixel = GetDeviceCaps(GetDC(ChessWnd),BITSPIXEL)/8;
// 	delete screenbuf;
// 	delete screenbuf1;
// 	screenbuf = new UUCHAR [screen_size_x*screen_size_y*byte_per_pixel];
// 	screenbuf1 = new UUCHAR [frame_size_y*frame_size_x*byte_per_pixel];
// 	memset(screenbuf,0,screen_size_x*screen_size_y*byte_per_pixel);
// 	memset(screenbuf1,0,frame_size_y*frame_size_x*byte_per_pixel);
// 	gc.screenBuf = (char*)screenbuf;
// 	gc.clipX2 = screen_size_x;
// 	gc.clipY2 = screen_size_y;
// 	gc1.screenBuf = (char*)screenbuf1;
// 	gc1.clipX2 = frame_size_x;
// 	gc1.clipY2 = frame_size_y;
// 	
// 	history->screen_max_x = screen_size_x/SCENE_TILE_WIDTH;
// 	history->screen_max_y = screen_size_y/SCENE_TILE_WIDTH;
// 	if(cur_win_state ==SCREEN_SIZE_BATTLE_NORMAL||cur_win_state == SCREEN_SIZE_BATTLE_MAX)
// 	{
// 		if(history->cur_actor!=NULL)
// 			history->CenterActor(history->cur_actor);
// 		else
// 		{
// 			history->scr_x = 0;
// 			history->scr_y = 0;
// 		}
// 	}
}
void ChessSetWindowSize(int win_state)
{
// 	cur_win_state = win_state;
// 	switch(win_state)
// 	{
// 	case SCREEN_SIZE_MENU:
// 	case SCREEN_SIZE_REDE:
// 		{
// 			SetWindowLong(ChessWnd,GWL_STYLE,GetWindowLong(ChessWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX);
// 			ShowWindow(ChessWnd,SW_SHOWNORMAL);
// 		}
// 		break;
// 	case SCREEN_SIZE_BATTLE_NORMAL:
// 		{
// 			SetWindowLong(ChessWnd,GWL_STYLE,GetWindowLong(ChessWnd,GWL_STYLE) |WS_MAXIMIZEBOX);
// 			ShowWindow(ChessWnd,SW_SHOWNORMAL);
// 		}
// 		break;
// 	case SCREEN_SIZE_BATTLE_MAX:
// 		{
// 			SetWindowLong(ChessWnd,GWL_STYLE,GetWindowLong(ChessWnd,GWL_STYLE) |WS_MAXIMIZEBOX);
// 			ShowWindow(ChessWnd,SW_SHOWMAXIMIZED);
// 		}
// 		break;
// 	}
// 	SetWindowSize();
}
// 
// HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010)
// {
// 	HRGN hRgn = NULL;
// 	if (hBmp)
// 	{
// 		// Create a memory DC inside which we will scan the bitmap content
// 		HDC hMemDC = CreateCompatibleDC(NULL);
// 		if (hMemDC)
// 		{
// 			// Get bitmap size
// 			BITMAP bm;
// 			GetObject(hBmp, sizeof(bm), &bm);
// 			// Create a 32 bits depth bitmap and select it into the memory DC 
// 			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
// 					sizeof(BITMAPINFOHEADER),	// biSize 
// 					bm.bmWidth,					// biWidth; 
// 					bm.bmHeight,				// biHeight; 
// 					1,							// biPlanes; 
// 					32,							// biBitCount 
// 					BI_RGB,						// biCompression; 
// 					0,							// biSizeImage; 
// 					0,							// biXPelsPerMeter; 
// 					0,							// biYPelsPerMeter; 
// 					0,							// biClrUsed; 
// 					0							// biClrImportant; 
// 			};
// 			VOID * pbits32; 
// 			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
// 			if (hbm32)
// 			{
// 				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);
// 				// Create a DC just to copy the bitmap into the memory DC
// 				HDC hDC = CreateCompatibleDC(hMemDC);
// 				if (hDC)
// 				{
// 					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
// 					BITMAP bm32;
// 					GetObject(hbm32, sizeof(bm32), &bm32);
// 					while (bm32.bmWidthBytes % 4)
// 						bm32.bmWidthBytes++;
// 					// Copy the bitmap into the memory DC
// 					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
// 					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
// 					// For better performances, we will use the ExtCreateRegion() function to create the
// 					// region. This function take a RGNDATA structure on entry. We will add rectangles by
// 					// amount of ALLOC_UNIT number in this structure.
// 					#define ALLOC_UNIT	100
// 					DWORD maxRects = ALLOC_UNIT;
// 					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
// 					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
// 					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
// 					pData->rdh.iType = RDH_RECTANGLES;
// 					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
// 					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
// 
// 					// Keep on hand highest and lowest values for the "transparent" pixels
// 					BYTE lr = GetRValue(cTransparentColor);
// 					BYTE lg = GetGValue(cTransparentColor);
// 					BYTE lb = GetBValue(cTransparentColor);
// 					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
// 					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
// 					BYTE hb = min(0xff, lb + GetBValue(cTolerance));
// 
// 					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
// 					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
// 					for (int y = 0; y < bm.bmHeight; y++)
// 					{
// 						// Scan each bitmap pixel from left to right
// 						for (int x = 0; x < bm.bmWidth; x++)
// 						{
// 							// Search for a continuous range of "non transparent pixels"
// 							int x0 = x;
// 							LONG *p = (LONG *)p32 + x;
// 							while (x < bm.bmWidth)
// 							{
// 								BYTE b = GetRValue(*p);
// 								if (b >= lr && b <= hr)
// 								{
// 									b = GetGValue(*p);
// 									if (b >= lg && b <= hg)
// 									{
// 										b = GetBValue(*p);
// 										if (b >= lb && b <= hb)
// 											// This pixel is "transparent"
// 											break;
// 									}
// 								}
// 								p++;
// 								x++;
// 							}
// 
// 							if (x > x0)
// 							{
// 								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
// 								if (pData->rdh.nCount >= maxRects)
// 								{
// 									GlobalUnlock(hData);
// 									maxRects += ALLOC_UNIT;
// 									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
// 									pData = (RGNDATA *)GlobalLock(hData);
// 								}
// 								RECT *pr = (RECT *)&pData->Buffer;
// 								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
// 								if (x0 < pData->rdh.rcBound.left)
// 									pData->rdh.rcBound.left = x0;
// 								if (y < pData->rdh.rcBound.top)
// 									pData->rdh.rcBound.top = y;
// 								if (x > pData->rdh.rcBound.right)
// 									pData->rdh.rcBound.right = x;
// 								if (y+1 > pData->rdh.rcBound.bottom)
// 									pData->rdh.rcBound.bottom = y+1;
// 								pData->rdh.nCount++;
// 								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
// 								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
// 								if (pData->rdh.nCount == 2000)
// 								{
// 									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
// 									if (hRgn)
// 									{
// 										CombineRgn(hRgn, hRgn, h, RGN_OR);
// 										DeleteObject(h);
// 									}
// 									else
// 										hRgn = h;
// 									pData->rdh.nCount = 0;
// 									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
// 								}
// 							}
// 						}
// 
// 						// Go to next row (remember, the bitmap is inverted vertically)
// 						p32 -= bm32.bmWidthBytes;
// 					}
// 
// 					// Create or extend the region with the remaining rectangles
// 					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
// 					if (hRgn)
// 					{
// 						CombineRgn(hRgn, hRgn, h, RGN_OR);
// 						DeleteObject(h);
// 					}
// 					else
// 						hRgn = h;
// 
// 					// Clean up
// 					SelectObject(hDC, holdBmp);
// 					DeleteDC(hDC);
// 				}
// 				DeleteObject(SelectObject(hMemDC, holdBmp));
// 			}
// 			DeleteDC(hMemDC);
// 		}	
// 	}
// 	return hRgn;
// }
void ChessChangeStyle(int game_style)
{
// 	if(game_style == 1)
// 	{
// 		SetWindowLong(ChessWnd,GWL_STYLE,WS_POPUP|WS_VISIBLE);//GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX
// 		HBITMAP hbmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(ChessWnd,GWL_HINSTANCE),
// 			"res/Images/logo.bmp",IMAGE_BITMAP,640,400,LR_LOADFROMFILE);
// 		HRGN region = BitmapToRegion(hbmp,RGB(255,0,255),0);
// 		SetWindowRgn(ChessWnd,region,TRUE);
// 	}
// 	else
// 	{
// 		SetWindowLong(ChessWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW|WS_CAPTION|WS_VISIBLE);//GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX
// 		//HRGN region=CreateRectRgn(0,0,640,400);//创建矩形区域
// 		SetWindowRgn(ChessWnd,NULL,TRUE);
// 	}
}
void ChessGetCurSorPos(int *x,int *y)
{
// 	POINT p1;
// 	RECT p2;
// 	GetCursorPos(&p1);
// 	GetWindowRect(ChessWnd,&p2);
// 	*x = p1.x - p2.left - GetSystemMetrics(SM_CXFRAME);
// 	*y = p1.y - p2.top - GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFRAME);
}
char* LL(const char* id, bool bIsReload /*= false*/, bool bIsSafe /*= false*/)
{
	static __Dictionary *strings = NULL;
	if (strings == NULL || bIsReload)
	{
        strings = __Dictionary::createWithContentsOfFileThreadSafe(getRespath("strings.xml", 0, 1));		
	}	

	if (bIsReload)
	{
		return nullptr;
	}
	else
	{
		__String* ptrStr = (__String*)strings->objectForKey(id);
		if (bIsSafe && !ptrStr) //找不到不报错，返回nullptr
		{
			return nullptr;
		}
		return (char*)(ptrStr->getCString());
	}
}

void addSpriteGray(Node* sprite)
{
	do
	{
		GLProgram* greyProgram = GLProgramCache::getInstance()->getGLProgram("grey_effect");
		if (greyProgram != nullptr) {
			sprite->setGLProgram(greyProgram);
			return;//修复shader内存泄漏
		}


		GLchar* pszFragSource =
		"#ifdef GL_ES \n \
			precision mediump float; \n \
		#endif \n \
		uniform sampler2D u_texture; \n \
		varying vec2 v_texCoord; \n \
		varying vec4 v_fragmentColor; \n \
		void main(void) \n \
		{ \n \
		// Convert to greyscale using NTSC weightings \n \
		vec4 col = texture2D(u_texture, v_texCoord); \n \
		float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
		gl_FragColor = vec4(grey, grey, grey, col.a); \n \
		}";

		GLProgram* pProgram = new GLProgram();
		pProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, pszFragSource);
		GLProgramCache::getInstance()->addGLProgram(pProgram, "grey_effect");
		sprite->setGLProgram(pProgram);
		CHECK_GL_ERROR_DEBUG();

		sprite->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		sprite->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		sprite->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
		CHECK_GL_ERROR_DEBUG();

		sprite->getGLProgram()->link();
		CHECK_GL_ERROR_DEBUG();

		sprite->getGLProgram()->updateUniforms();
		CHECK_GL_ERROR_DEBUG();
	}
	while(0);
}
void addSpriteGrayAndChild(Node* sprite, int child/*=-1*/)
{
	int i = 0;
	addSpriteGray((Sprite*)sprite);
	Vector<Node*> v_Child=sprite->getChildren();
	for (Vector<Node*>::iterator it=v_Child.begin();it!=v_Child.end();it++)
	{
		Node*  pChild = *it;
		CCLOG("child:%d",pChild->getChildren().size());
		if (i != child)//第child子精灵不变灰
		{
			if (pChild->getChildren().size())
				addSpriteGrayAndChild((Sprite*)pChild);
			else 
				addSpriteGray((Sprite*)pChild);
		}
		i++;
	}
}
void removeSpriteGray(Node* sprite)
{
    sprite->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
}

void BgAddFashionSImg(int iActorId, std::map<int, int> mFashion, Vec2 iPoint, int iImgIndex, Node* Bg, Sprite** fashionSp, Sprite** wapfashionSp, std::string typeS, int iXHalf,int iYHalf)
{
	int iGender = mFashion[1];
	if (iGender< 1 && iActorId > 0)
	{
		iGender = trData->actors[iActorId - 1].iGender;
	}
	else if(iGender< 1)
	{
		iGender = 1;
	}

	int iFashion1 = mFashion[2];
	int iFashion2 = mFashion[4];

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_%s.png", iGender, iFashion1,typeS.c_str());
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_%s.png", iFashion2, typeS.c_str());

	Sprite* spEquip = ReadSpriteNormal(strEquip->getCString());
	Size weaponSize = spEquip->getContentSize();

	if (iXHalf != 0)
	{
		iPoint += Vec2(weaponSize.width/ iXHalf, 0);
	}
	if (iYHalf != 0)
	{
		iPoint += Vec2(0, weaponSize.width / iYHalf);
	}

	int iOrder = 0;
	if (typeS == "mov")
		iOrder = trData->net_hero_dress[iFashion2]->mMov[iImgIndex];
	else if(typeS == "atk")
		iOrder = trData->net_hero_dress[iFashion2]->mAtk[iImgIndex];
	else
		iOrder = trData->net_hero_dress[iFashion2]->mSpc[iImgIndex];

	spEquip->setPosition(iPoint);
	spEquip->setTextureRect(Rect(0, weaponSize.width * iImgIndex, weaponSize.width, weaponSize.width));
	Bg->addChild(spEquip, iOrder);

	Sprite* spHero = ReadSpriteNormal(strHero->getCString());
	Size heroSize = spHero->getContentSize();
	spHero->setPosition(spEquip->getPosition());
	spHero->setTextureRect(Rect(0, heroSize.width * iImgIndex, heroSize.width, heroSize.width));
	Bg->addChild(spHero);

	if(fashionSp)
		*fashionSp = spHero;
	if(wapfashionSp)
		*wapfashionSp = spEquip;
}
