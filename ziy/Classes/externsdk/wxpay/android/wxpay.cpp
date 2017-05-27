#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include "cocos2d.h"
#include "StatConfig.h"

USING_NS_CC;

static DString g_strSubject = "";
static DString g_strBody = "";

int payStartwxpay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
	g_strSubject = subject;
	g_strBody = body;

	JniMethodInfo minfo;//定义Jni函数信息结构体 
	//getStaticMethodInfo 此函数返回一个bool值表示是否找到此函数 
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/bantu/trgame/TRGame", "startwxpay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
	if(!isHave)
	{
		CCLog("jni:startwxpay not found");
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
	JNIEXPORT void Java_com_bantu_trgame_TRGame_nativewxpayResult(JNIEnv*  env, jobject thiz, jstring tradeNo, jint resultCode, jstring resultMessage)
	{
		const char* strTradeNo = env->GetStringUTFChars(tradeNo, 0);
		const char* strResultMessage = env->GetStringUTFChars(resultMessage, 0);

		StatConfig::onPayResult(StatConfig::PAY_TYPE_WXPAY, strTradeNo, resultCode, strResultMessage);
	}

	JNIEXPORT void Java_com_bantu_trgame_TRGame_nativeWXHttpRequest(JNIEnv*  env, jobject thiz, jstring appid, jstring mch_id, jstring out_trade_no, jstring nonce_str)
	{
		const char* strAppid = env->GetStringUTFChars(appid, 0);
		const char* strMch_id = env->GetStringUTFChars(mch_id, 0);
		const char* strOut_trade_no = env->GetStringUTFChars(out_trade_no, 0);
		const char* strNonce_str = env->GetStringUTFChars(nonce_str, 0);

		StatConfig::startWXHttpRequest(strAppid, strMch_id, strOut_trade_no, strNonce_str, g_strSubject, g_strBody);
	}
}