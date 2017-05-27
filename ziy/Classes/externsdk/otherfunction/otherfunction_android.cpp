#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include "cocos2d.h"
#include "otherfunction.h"
#include "common.h"
#include "LoginLayer.h"
#include "LogoLayer.h"
#include "main.h"
#include "StatConfig.h"
#include "PaymentTool.h"

USING_NS_CC;
void showAdWebView(){
}
void initAdGoogle(){

}
void showAdMobInterstitial(void (*callback)(bool result)){

}

DString statGetDeviceParams(StatDeviceParamType type)
{
	if (type == StatParamPlatform)
	{
		return "android";
	}
	if (type == StatParamPatchVersion)
        {
            return "";
        }
	
	JniMethodInfo minfo;//定义Jni函数信息结构体 
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
    bool isHave = JniHelper::getStaticMethodInfo(minfo,  "com/bantu/trgame/TRGame","getDeviceParams","(I)Ljava/lang/String;"); 
    if (!isHave) { 
        CCLog("jni:getDeviceParams not found"); 
        return "";
    }
    
    //调用此函数 
    jstring result = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID, (int)type); 
    const char* str = minfo.env->GetStringUTFChars(result, 0);  
    CCLog("DeviceParams=%s,type=%d", str, (int)type);
    DString ret = str;
    minfo.env->ReleaseStringUTFChars(result, str); 
    return ret;
}


void cppBridgeCall(const char* strJson)
{
	JniMethodInfo minfo;//定义Jni函数信息结构体 
    bool isHave = JniHelper::getStaticMethodInfo(minfo,  "com/bantu/trgame/TRGame","cppBridgeCall","(Ljava/lang/String;)V"); 
    if (!isHave) { 
        CCLog("jni:cppBridgeCall not found"); 
        return;
    }
    //调用此函数 
	jstring jmsg = minfo.env->NewStringUTF(strJson);
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jmsg); 
	minfo.env->DeleteLocalRef(jmsg);
}


void sendWXAuthRequest()
{
	JniMethodInfo minfo;//定义Jni函数信息结构体 
	//getStaticMethodInfo 此函数返回一个bool值表示是否找到此函数 
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/bantu/trgame/TRGame", "sendWXAuthRequest", "()V");
	if(!isHave)
	{
		CCLog("sendWXAuthRequest not found !");
		return;
	}
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
}

void sendQQAuthRequest()
{
	JniMethodInfo minfo;//定义Jni函数信息结构体 
	//getStaticMethodInfo 此函数返回一个bool值表示是否找到此函数 
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/bantu/trgame/TRGame", "sendQQAuthRequest", "()V");
	if(!isHave)
	{
		CCLog("sendQQAuthRequest not found !");
		return;
	}
	minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
}

extern "C"
{

EXPORT_LOCAL static void _localCheckApp(JNIEnv* env, jobject thiz, jobject context)
{
	//获得 Context 类  
	jclass native_clazz = env->GetObjectClass(context);

	// 得到 getPackageName 方法的 ID  
	jmethodID methodID_pack = env->GetMethodID(native_clazz, "getPackageName", "()Ljava/lang/String;");

	// 获得当前应用的包名  
	jstring application_package = (jstring)env->CallObjectMethod(context, methodID_pack);

	//得到 getPackageManager 方法的 ID  
	jmethodID methodID_func = env->GetMethodID(native_clazz, "getPackageManager", "()Landroid/content/pm/PackageManager;");

	//获得应用包的管理器  
	jobject package_manager = env->CallObjectMethod(context, methodID_func);

	// 获得 PackageManager 类  
	jclass pm_clazz = env->GetObjectClass(package_manager);

	// 得到 getPackageInfo 方法的 ID  
	jmethodID methodID_pm = env->GetMethodID(pm_clazz, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

	// 获得应用包的信息  
	jobject package_info = env->CallObjectMethod(package_manager, methodID_pm, application_package, 64);

	// 获得 PackageInfo 类
	jclass pi_clazz = env->GetObjectClass(package_info);

	// 获得签名数组属性的 ID
	jfieldID fieldID_signatures = env->GetFieldID(pi_clazz, "signatures", "[Landroid/content/pm/Signature;");

	// 得到签名数组
	jobjectArray signatures = (jobjectArray)env->GetObjectField(package_info, fieldID_signatures);

	// 得到签名
	jobject signature = env->GetObjectArrayElement(signatures, 0);

	// 获得 Signature 类
	jclass s_clazz = env->GetObjectClass(signature);

	// 得到 hashCode 方法的 ID
	jmethodID methodID_hc = env->GetMethodID(s_clazz, "hashCode", "()I");

	// 获得应用包的管理器，待修改
	int hash_code = env->CallIntMethod(signature, methodID_hc);
	setAppSignatureCode(hash_code);
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeInit(JNIEnv* env, jobject thiz, jobject context)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeVerifyApp");
	CCLog("nativeInit,thiz=%d,context=%d", (int)thiz, (int)context);
	
	_localCheckApp(env, thiz, context);
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeGetWXAccessToken(JNIEnv*  env, jobject thiz, jstring appid, jstring secret, jstring code)
{
	const char* strAppid = env->GetStringUTFChars(appid, 0);
	const char* strSecret = env->GetStringUTFChars(secret, 0);
	const char* strCode = env->GetStringUTFChars(code, 0);

	StatConfig::GetWXAccessToken(strAppid, strSecret, strCode);
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeOnHttpReqQQLogin(JNIEnv*  env, jobject thiz, jstring openid, jstring token)
{
	const char* strOpenid = env->GetStringUTFChars(openid, 0);
	const char* strToken = env->GetStringUTFChars(token, 0);

	DString open_id = strOpenid;
	LoginLayer::onHttpReqWXQQLogin(open_id->getChar(), "QQ");
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeWXOrQQLoginFailed(JNIEnv*  env, jobject thiz)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeWXOrQQLoginFailed");
	CCLog("nativeWXOrQQLoginFailed,env=%d,thiz=%d", (int)env, (int)thiz);
	
	LoginLayer::PlatRemoveWaitSprite();
}

JNIEXPORT jstring Java_com_bantu_trgame_TRGame_nativeGetNotifyUrl(JNIEnv* env, jobject thiz)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeGetNotifyUrl");
	CCLog("nativeGetNotifyUrl,env=%d,thiz=%d", (int)env, (int)thiz);

	return env->NewStringUTF(PaymentTool::getInstance()->getNotifyUrl().c_str());
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeSetPlatformLoginInfo(JNIEnv*  env, jobject thiz, jstring infoJsonStr)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeSetPlatformLoginInfo");
	const char* strinfoJsonStr = env->GetStringUTFChars(infoJsonStr, 0);
	LogoLayer::setLoginInfo(strinfoJsonStr);
	env->ReleaseStringUTFChars(infoJsonStr,strinfoJsonStr); 
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeSDKPayResult(JNIEnv*  env, jobject thiz, jstring infoJsonStr)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeSDKPayResult");
	const char* strinfoJsonStr = env->GetStringUTFChars(infoJsonStr, 0);
	PaymentTool::getInstance()->SDKPayResult(strinfoJsonStr);
	env->ReleaseStringUTFChars(infoJsonStr, strinfoJsonStr);
}

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeJavaCallCppBridge(JNIEnv*  env, jobject thiz, jstring infoJsonStr)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeJavaCallCppBridge");
	const char* strinfoJsonStr = env->GetStringUTFChars(infoJsonStr, 0);
	LogoLayer::javaCallCppBridge(strinfoJsonStr);
	env->ReleaseStringUTFChars(infoJsonStr, strinfoJsonStr);
}

JNIEXPORT jint Java_com_bantu_trgame_TRGame_nativeGetIntValue(JNIEnv*  env, jobject thiz, jstring name)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeGetIntValue");
	const char* strName = env->GetStringUTFChars(name, 0);
	int rs = LogoLayer::getIntValue(strName);
	env->ReleaseStringUTFChars(name, strName);
	return rs;
}

JNIEXPORT jstring Java_com_bantu_trgame_TRGame_nativeGetStrValue(JNIEnv* env, jobject thiz, jstring name)
{
	CCLog("Java_com_bantu_trgame_TRGame_nativeGetStrValue");
	const char* strName = env->GetStringUTFChars(name, 0);
	std::string rs = LogoLayer::getStrValue(strName);
	jstring jrs = env->NewStringUTF(rs.data());
	env->ReleaseStringUTFChars(name, strName);
	return jrs;
}
}
