#include "Common.h"
#include "main.h"
#include "PasswordLib.h"
#include "GameMain.h"
#include "SimpleAudioEngine.h"
#include "StatConfig.h"
#include "DRunnable.h"
#include "jsons.h"
#include "LoginLayer.h"
#include "LogoLayer.h"
#include "ActionLayer.h"
#include "PopLayer.h"
#include "AntiAddictionLayer.h"

ChessData* trData = NULL;
FunctionConst* funConst = NULL;
ModConst* modConst = NULL;
HistoryPtr history = NULL;
int ads_selection = 1;

USING_NS_CC;
using namespace CocosDenshion;
using namespace DebugPerformance;

////////////////////////////////////////////////////////////////////////////////////////////////

ChessLayer::ChessLayer()
{

}

ChessLayer::~ChessLayer()
{
}

long key_time = 0;
int last_x = 0;
int last_y = 0;

void ChessLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	//setTouchEnabled(true);

	//if (last_time++ % 2 == 0)
	//if (getCurrentTimeMS() - last_time > 30)
	{
		Chess::getInstance()->OnTimer();
		//last_time = getCurrentTimeMS();
	}
	key_time++;
	if (key_time == -1)
	{
		Chess::getInstance()->OnRPointerPressed(last_x, last_y);
	}

	Chess::getInstance()->OnPaint();
}

bool ChessLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	key_time = -20;
	Vec2 point = touch->getLocationInView();
	int x = point.x;
	int y = point.y;
	if (this->getPositionX() != 0)
	{
	    x = point.x - this->getPositionX();
		if (x < 0)
		{
			x = 0;
		}
	}
	if (this->getPositionY() != 0)
	{
		y = point.y - this->getPositionY();
		if (y < 0)
		{
			y = 0;
		}
	}
	Chess::getInstance()->OnPointerPressed(x, y);
	last_x = x;
	last_y = y;
    return true;
}

void ChessLayer::onTouchMoved(Touch *touch, Event *pEvent)
{
	Vec2 point = touch->getLocationInView();
	int x = point.x;
	int y = point.y;
	if (this->getPositionX() != 0)
	{
		x = point.x - this->getPositionX();
		if (x < 0)
		{
			x = 0;
		}
	}
	if (this->getPositionY() != 0)
	{
		y = point.y - this->getPositionY();
		if (y < 0)
		{
			y = 0;
		}
	}
	if ((last_x - x) > -20 && (last_x - x)<20
		&& (last_y - y)>-20 && (last_y - y) < 20)
	{

	}
	else
	{
		key_time = 0;
		Chess::getInstance()->OnPointerPressDragged(x, y);
	}
}

void ChessLayer::onTouchEnded(Touch *touch, Event *pEvent)
{
	key_time = 0;
	//CCTouch* pTouch = (CCTouch*)(*pTouches->begin());
	Vec2 point = touch->getLocationInView();
	int x = point.x;
	int y = point.y;
	if (this->getPositionX() != 0)
	{
		x = point.x - this->getPositionX();
		if (x < 0)
		{
			x = 0;
		}
	}
	if (this->getPositionY() != 0)
	{
		y = point.y - this->getPositionY();
		if (y < 0)
		{
			y = 0;
		}
	}
	Chess::getInstance()->OnPointerReleased(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////
ChessApplication* ChessApplication::instance = NULL;
ChessApplication::ChessApplication() {
	instance = this;
}

ChessApplication::~ChessApplication()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void ChessApplication::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool ChessApplication::applicationDidFinishLaunching()
{
	auto fileUtils = FileUtils::getInstance();
#ifdef GAME_RES_MINI
	std::vector<string> resDirOrders;
	std::string writeAblePath = fileUtils->getWritablePath();
	resDirOrders.push_back(writeAblePath + "minipatch_dir/globalres/");
	resDirOrders.push_back(writeAblePath + "minipatch_dir/");
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	resDirOrders->push_back(writeAblePath + "patch_dir/");
#else
	resDirOrders.push_back(writeAblePath + "patch_dir/GameResources/");
#endif
	for (int i = 0; i < resDirOrders.size(); i++) {
		fileUtils->addSearchPath(resDirOrders[i],true);
	}
	FunctionConst::initInstance();//may be need search path

	const std::vector<std::string>& searchPath = fileUtils->getSearchPaths();

	resDirOrders.clear();
	resDirOrders.assign(searchPath.begin(),searchPath.end());
	std::string gamePath = funConst->GamePath->getChar();
	
	std::string patchGameDir = writeAblePath + "patch_dir/" + gamePath;
	resDirOrders.insert(resDirOrders.begin(), patchGameDir);
	std::string patchMiniGameDir = writeAblePath + "minipatch_dir/" + gamePath;
	resDirOrders.insert(resDirOrders.end()-1,patchMiniGameDir);
	fileUtils->setSearchPaths(resDirOrders);

	MiniSupport::initWithPath(gamePath, resDirOrders.at(resDirOrders.size()-1));
	/************************************************************************/
	/* 
+		[0]	"patch_dir/zqtx"	
+		[1]	"patch_dir/GameResources/"	
+		[2]	"minipatch_dir/"	
+		[3]	"minipatch_dir/globalres/"	
+		[4]	"minipatch_dir/zqtx"	
+		[5]	"zqtxmini/client/Resources/"
	*/
	/************************************************************************/
#else
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
		fileUtils->addSearchPath(fileUtils->getWritablePath() + "patch_dir/", true);
	#else
		fileUtils->addSearchPath(fileUtils->getWritablePath() + "patch_dir/GameResources/", true);
	#endif
	FunctionConst::initInstance();
#endif // GAME_RES_MINI

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	Win32Const::initInstance();
#endif

	srand(DebugPerformance::getCurrentTick());
	//适配不同项目的分辨率
	float w = 710;
	float h = 400;
	// initialize director
	Director* pDirector = Director::getInstance();
	auto glView = pDirector->getOpenGLView();
	if (!glView)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glView = GLViewImpl::createWithRect("TRGE3", Rect(0, 0, w, h));
#else
		glView = GLViewImpl::create("TRGE3");
#endif
		pDirector->setOpenGLView(glView);
	}
#ifdef WIN32
	glView->setViewName(win32Const->packageName->getChar());
	glView->setFrameSize(w, h);
	glView->setFrameZoomFactor(1.0f);
#endif
	pDirector->setProjection(Director::Projection::_2D);

	ChessViewTool* vTool = ChessViewTool::getInstance();
	vTool->computeFullSize(glView->getFrameSize().width, glView->getFrameSize().height);
	DSizeF scrSize = vTool->getFullSize();
	glView->setDesignResolutionSize(scrSize.width, scrSize.height, ResolutionPolicy::SHOW_ALL);

	scene = Scene::create();
	chessLayer = ChessLayer::create();
	scene->addChild(chessLayer);

	mActionLayer = ActionLayer::create();
	scene->addChild(mActionLayer);

	FunctionLayer = Layer::create();
	FunctionLayer->setPosition(Vec2(0, 0));
	scene->addChild(FunctionLayer);


    Chess::getInstance()->graph = new DGraphics(scrSize.width, scrSize.height);
    vTool->setScreenDefaultSize();

    pDirector->runWithScene(scene);
	Chess::getInstance()->didFinishLaunching();

	if (funConst->ShowFrame)
	{
		pDirector->setDisplayStats(true);
	}

	// set FPS. the default value is 1.0/60 if you don't call this
	//pDirector->setAnimationInterval(1.0 / 30);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void ChessApplication::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void ChessApplication::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

	GLProgram* greyProgram = GLProgramCache::getInstance()->getGLProgram("grey_effect");
	if (greyProgram != nullptr) {
		greyProgram->reset();

		auto sprite = Sprite::create();
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


		greyProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, pszFragSource);

		sprite->setGLProgram(greyProgram);
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

}

ChessApplication* ChessApplication::getInstance()
{
	return instance;
}

void ChessApplication::setChessLayerPointX(float x)
{
	if (chessLayer != NULL)
	{
		chessLayer->setPositionX(x);
	}
}
void ChessApplication::setChessLayerPointY(float y)
{
	if (chessLayer != NULL)
	{
		chessLayer->setPositionY(y);
	}
}
bool ChessApplication::isChessLayerTouchEnabled()
{
	return chessLayer->isTouchEnabled();
}

void ChessApplication::AddFunctionLayer(Layer* ptrLayer)
{
	FunctionLayer->addChild(ptrLayer);
}
void ChessApplication::removeAllFunctionLayer()
{
	FunctionLayer->removeAllChildren();
}
Layer* ChessApplication::getFounctionLayerByTag(int layerTag)
{
	Layer* layer = dynamic_cast<Layer*>(FunctionLayer->getChildByTag(layerTag));
	if (layer) {
		return layer;
	}
	return nullptr;
}

Layer* ChessApplication::GetActionLayer()
{
	return mActionLayer;
}
////////////////////////////////////////////////////////////////////////////////////////////////

Chess::Chess() 
{
	Init();
}

Chess::~Chess()
{
	SAFE_DELETE(statConfig);
	SAFE_DELETE(threadRun);
	removeAllScreen(-1);
	SAFE_DELETE(screenList);
	SAFE_DELETE(deleteScreenList);
}

void Chess::didFinishLaunching()
{
	if (activeScreen == NULL)
	{
		ChessApplication::getInstance()->AddFunctionLayer(AntiAddictionLayer::create());
	}
}

void Chess::Init()
{
	graph = NULL;

	trData = new ChessData();
	modConst = new ModConst();
	history = new History();
	memset(history->setting_flag,1,9);
	
	this->screenList = new DList<Screen>(false);
	deleteScreenList = new DList<Screen>(true);
	this->activeScreen = NULL;
	this->currentScreen = NULL;
	this->auto_run1 = 0;
	this->talk_buf = NULL;
	this->is_load = 0;
	this->r_point_pressed = 0;
	this->l_point_pressed = 0;
	this->isNeedGuide = false;

	threadRun = new DThreadRun();
	statConfig = new StatConfig();

	choice = new DVector<int>();
}

void Chess::OnPaint()
{
	int i;

	threadRun->runList();

	//if(!video_playing)
	{
		for(i=0;i<screenList->size();i++)  
		{
			currentScreen = (ScreenPtr)screenList->elementAt(i);
			if(currentScreen!=NULL)
			{
				currentScreen->onPaint(graph);
			}	
		}
	}

	graph->postScreen();
}
void Chess::OnTimer()
{
	deleteScreenList->removeAllElements();
	if(activeScreen!=NULL&&!video_playing)
	{
		activeScreen->onSysTimer();
	}
	if(activeScreen!=NULL&&!video_playing)
	{
		int i,j;
		ChessGetCurSorPos(&i,&j);
		i-=FRAME_X_OFFSET;
		j-=FRAME_Y_OFFSET;
		if(i<0||j<0||i>screen_size_x||j>screen_size_y)
		{
			
		}
		else if(activeScreen->screenId == SCREEN_REDE||activeScreen->screenId == SCREEN_BATTLE)
		{
			ChessSetCursor(CURSOR_NORMAL);
		}
	}
	ChessSetCursor(CURSOR_ARROW);
}
void Chess::OnKeyDown(int keyCode)
{
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->onKeyPressed(keyCode);
	}
}
void Chess::OnKeyUp(int keyCode)
{
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->onKeyReleased(keyCode);
	}
}
void Chess::OnPointerPressed(int x, int y)
{
	l_point_pressed = 1;
	point_moved = 0;
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->pointerPressed(x,y);
	}
	else if(activeScreen)
	{
		activeScreen->pointerPressed(x,y);
	}
}

void Chess::OnPointerReleased(int x, int y)
{
	int ret = 0;
	l_point_pressed = 0;
	r_point_pressed = 0;

	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        ret = activeScreen->pointerReleased(x,y);
	}
	else if(activeScreen)
	{
		ret = activeScreen->pointerReleased(x,y);
	}
	if (ret == 0&&point_moved==0&&activeScreen)
	{
		activeScreen->pointerClicked(x, y);
	}
	point_moved = 0;
}
void Chess::OnRPointerPressed(int x, int y)
{
	r_point_pressed = 1;
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->RpointerPressed(x,y);
	}
	else if(activeScreen)
	{
		activeScreen->RpointerPressed(x,y);
	}
}
void Chess::OnRPointerReleased(int x, int y)
{
	r_point_pressed = 0;
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->RpointerReleased(x,y);
	}
	else if(activeScreen)
	{
		activeScreen->RpointerReleased(x,y);
	}
}
void Chess::OnPointerDragged(int x, int y)
{
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->pointerDragged(x,y);
	}
	else if(activeScreen)
	{
		activeScreen->pointerDragged(x,y);
	}
	
}
void Chess::OnPointerPressDragged(int x, int y)
{
	point_moved = 1;
	if(activeScreen!=NULL&&BattleControler::auto_run==0)
	{
        activeScreen->pointerPressDragged(x,y);
	}
	else if(activeScreen)
	{
		activeScreen->pointerPressDragged(x,y);
	}
	
}

void Chess::OnMsg(MessagePtr message){
    if (activeScreen != NULL) {
        activeScreen->onMessage(message);
    }
    DelMessage(message);
}

void Chess::OnResponse(ScreenResponse* res)
{
	if (activeScreen != NULL)
	{
		if (activeScreen->onResponse(res))
		{
			return;
		}
	}

	for (int i = screenList->size() - 1; i >= 0; i--)
	{
		ScreenPtr scr = (ScreenPtr)screenList->elementAt(i);
		if (scr != NULL &&  scr != activeScreen)
		{
			if (scr->onResponse(res))
			{
				return;
			}
		}
	}
}

void Chess::ExitGame()
{
	ChessStopMp3();
}

////////////////////////////////////////////////////////////////////////
//窗口系统
void Chess::removeScreen(ScreenPtr gameScreen, int changeActiveScreen)
{
    int bChangeActiveScreen = 0;
	gameScreen = getScreenByID(gameScreen->screenId);
    if(gameScreen==NULL) return;
    if(activeScreen==NULL || gameScreen==activeScreen)
	{
        bChangeActiveScreen = 1;
    }	
	screenList->removeElement(gameScreen);
	gameScreen->onRemove();
	deleteScreenList->addElement(gameScreen);
    if(bChangeActiveScreen)
	{
        if(screenList->size()>0 && changeActiveScreen)
		{
            //默认激活屏幕下移
            activeScreen = (ScreenPtr)screenList->elementAt(screenList->size()-1);
        }
		else
		{
            activeScreen = NULL;
        }
    }
}
void Chess::removeAllScreen(int screen_id)
{
	ScreenPtr cur_scr;
	int i;
	for(i = screenList->size() - 1;i>=0;i--)
	{
		cur_scr = (ScreenPtr)screenList->elementAt(i);
		if (cur_scr->screenId != screen_id
			&& cur_scr != activeScreen)
		{
			removeScreen(cur_scr, 0);
		}
	}
}

void Chess::removeAllScreen()
{
	for(int i = screenList->size() - 1; i >= 0; i--)
	{
		removeScreen(screenList->elementAt(i), 0);
	}
}

void Chess::appendScreen(ScreenPtr gameScreen)
{
	if (gameScreen == NULL)
	{
		return;
	}
    activeScreen = gameScreen;
    screenList->addElement(gameScreen);
 	activeScreen->onAppend();
}
ScreenPtr Chess::getScreenByID(int scr_id)
{
	ScreenPtr cur_scr;
	int i;
	for(i = 0;i<screenList->size();i++)
	{
		cur_scr = (ScreenPtr)screenList->elementAt(i);
		if(cur_scr->screenId == scr_id)
			return cur_scr;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////
ChessPtr Chess::_instance = NULL ;   

ChessPtr Chess::getInstance()   
{   
	if(_instance == NULL)   
	{
		_instance = new Chess();
	}
	return  _instance;   
}
void Chess::SetTalkBuf(char* buf)
{
	if(talk_buf)
		ChessFree(talk_buf);
	talk_buf = buf;
}
void Chess::ClearTalkBuf()
{
	ChessFree(talk_buf);
	talk_buf = NULL;
}

StatConfig* Chess::getStatConfig()
{
	return statConfig;
}

DThreadRun* Chess::getThreadRun()
{
	return threadRun;
}

DGraphics* Chess::getGraphics()
{
	return graph;
}
