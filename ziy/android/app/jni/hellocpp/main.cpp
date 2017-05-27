#include "AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "TDGAJniHelper.h"
#include "TDGALog.h"
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;
extern int MyMain();
extern int MyMainAndroid();

void cocos_android_app_init (JNIEnv* env) {
    LOGD("cocos_android_app_init");
//CCLOG("G11");
//CCFileUtils::sharedFileUtils()->getWritablePath();
//CCLOG("G21");
//    AppDelegate *pAppDelegate = new AppDelegate();
//	MyMain();
	TDGAJniHelper::setJavaVM(JniHelper::getJavaVM());
	MyMainAndroid();
}
