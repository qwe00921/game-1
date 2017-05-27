#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include "cocos2d.h"
#include "StatConfig.h"

USING_NS_CC;


int payStartAlipay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
	JniMethodInfo minfo;//定义Jni函数信息结构体 
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数 
    bool isHave = JniHelper::getStaticMethodInfo(minfo,  "com/bantu/trgame/TRGame","startAlipay","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I"); 
    if (!isHave) { 
        CCLog("jni:startYeepay not found"); 
        return 0;
    }
    
    jstring jGameId = minfo.env->NewStringUTF(trGameId);
    jstring jTradeNo = minfo.env->NewStringUTF(outTradeNo);
    jstring jSubject = minfo.env->NewStringUTF(subject);
    jstring jBody = minfo.env->NewStringUTF(body);
    jstring jTotalFee = minfo.env->NewStringUTF(totalFee);
    
    jint result = (jint)minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID, jGameId, jTradeNo, jSubject, jBody, jTotalFee); 
    
    minfo.env->DeleteLocalRef(jGameId);
    minfo.env->DeleteLocalRef(jTradeNo);
    minfo.env->DeleteLocalRef(jSubject);
    minfo.env->DeleteLocalRef(jBody);
    minfo.env->DeleteLocalRef(jTotalFee);
    
    return result;
}

extern "C"
{

JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeAlipayResult(JNIEnv*  env, jobject thiz, jstring tradeNo, jint resultCode, jstring resultMessage)
{
    const char* strTradeNo = env->GetStringUTFChars(tradeNo, 0);
    const char* strResultMessage = env->GetStringUTFChars(resultMessage, 0);
    
    StatConfig::onPayResult(StatConfig::PAY_TYPE_ALIPAY, strTradeNo, resultCode, strResultMessage);
    
    //env->ReleaseStringUTFChars(tradeNo, 0); 
    //env->ReleaseStringUTFChars(resultMessage, 0); 
}

}
















