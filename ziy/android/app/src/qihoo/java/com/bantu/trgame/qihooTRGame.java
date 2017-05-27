/****************************************************************************
 * Copyright (c) 2010-2011 cocos2d-x.org
 *
 * http://www.cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ****************************************************************************/
package com.bantu.trgame;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.content.Context;
import android.content.res.Configuration;
import android.widget.Toast;

import com.bantu.R;


import com.qihoo.gamecenter.sdk.common.IDispatcherCallback;
import com.qihoo.gamecenter.sdk.matrix.Matrix;
import com.qihoo.gamecenter.sdk.protocols.CPCallBackMgr.MatrixCallBack;
import com.qihoo.gamecenter.sdk.protocols.ProtocolConfigs;
import com.qihoo.gamecenter.sdk.protocols.ProtocolKeys;
import com.qihoo.gamecenter.sdk.activity.ContainerActivity;
import com.qihoo.gamecenter.sdk.common.IDispatcherCallback;
import android.text.TextUtils;

import java.lang.reflect.Array;
import java.util.HashMap;

public class qihooTRGame extends TRGame {
	private static final String TAG = "qihooTRGame";
	protected String mAccessToken = null;

	private int mRoleLevel;
	private  String mAppUserId;
	private String mNickName;
	private String mOrderId;
	protected QihooUserInfo mQihooUserInfo;
	protected boolean mIsLandscape;
	private boolean mIsInOffline = false;
	protected MatrixCallBack mSDKCallback = new MatrixCallBack() {
		@Override
		public void execute(Context context, int functionCode, String functionParams) {
			if (functionCode == ProtocolConfigs.FUNC_CODE_SWITCH_ACCOUNT) {
				doSdkSwitchAccount(getLandscape(context));
			}else if (functionCode == ProtocolConfigs.FUNC_CODE_INITSUCCESS) {
				//这里返回成功之后才能调用SDK 其它接口
				Log.d(TAG, "FUNC_CODE_INITSUCCESS");
				doSdkLogin(true);
			}else {
				Log.d(TAG, "初始化失败" + functionCode);
			}
		}

	};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		Log.d(TAG, "onCreate");
		Matrix.init(this, mSDKCallback);
    }
	// ---------------------------------回调接口------------------------------------
	protected boolean getLandscape(Context context) {
		if (context == null) {
			return false;
		}
		boolean landscape = (this.getResources().getConfiguration().orientation
				== Configuration.ORIENTATION_LANDSCAPE);
		return landscape;
	}
	/**
	 * 使用360SDK的登录接口
	 *
	 * @param isLandScape 是否横屏显示登录界面
	 */
	protected void doSdkLogin(boolean isLandScape) {
		Log.d(TAG, "doSdkLogin");
		Intent intent = getLoginIntent(isLandScape);
		Matrix.execute(this, intent, mLoginCallback);
	}

	/**
	 * 使用360SDK的切换账号接口
	 *
	 * @param isLandScape 是否横屏显示登录界面
	 */
	protected void doSdkSwitchAccount(boolean isLandScape) {
		preSubExtendData("exitServer");
		SDKChangeAccount();
		Intent intent = getSwitchAccountIntent(isLandScape);
		IDispatcherCallback callback = mAccountSwitchCallback;
//		if (getCheckBoxBoolean(R.id.isSupportOffline)) {
//			callback = mAccountSwitchSupportOfflineCB;
//		}
		Matrix.invokeActivity(this, intent, callback);
	}

	private void startGame() {

	}
	@Override
	protected void onDestroy() {
		// 游戏退出前，不再调用SDK其他接口时，需要调用Matrix.destroy接口
		Log.d(TAG, "onDestroy");
		Matrix.destroy(this);
		super.onDestroy();
	}

	private void payResult(String payId,Boolean success){
		JSONObject jsonObj = new JSONObject();
		try {

			jsonObj.put("payId",payId);
			if (success)
				jsonObj.put("rs", 1);
			else{
				jsonObj.put("rs", 0);
			}
		} catch (JSONException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
		final String jsonStr = jsonObj.toString();
		runOnGLThread(new Runnable() {
			@Override
			public void run() {
				TRGame.nativeSDKPayResult(jsonStr);
			}
		});
	}
	// 支付的回调
	protected IDispatcherCallback mPayCallback = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
           Log.d(TAG, "mPayCallback, data is " + data);
			if(TextUtils.isEmpty(data)) {
				return;
			}

			JSONObject jsonRes;
			try {
				jsonRes = new JSONObject(data);
				// error_code 状态码： 0 支付成功， -1 支付取消， 1 支付失败， -2 支付进行中, 4010201和4009911 登录状态已失效，引导用户重新登录
				// error_msg 状态描述
				int errorCode = jsonRes.optInt("error_code");

				switch (errorCode) {
					case 0:
					case 1:
					case -1:
					case -2: {
						if (errorCode == 0){
							payResult(mOrderId,true);
						}else if (errorCode == -1 || errorCode == 1){
							payResult(mOrderId,false);
						}
					}
					break;
					case -100:
					case 4010201://acess_token失效
					case 4009911:
					{//QT失效
						payResult(mOrderId,false);
						doSdkLogin(true);
					}
						break;
					default:
						payResult(mOrderId,false);
						break;
				}
			} catch (JSONException e) {
				e.printStackTrace();
			}

		}
	};

	// payment end
	@Override
	protected void onResume() {
		Log.d(TAG, "onResume");
		super.onResume();
		SDKExitCancel();
		Matrix.onResume(this);
	}
	@Override
	protected void onStart() {
		Log.d(TAG, "onStart");
		super.onStart();
		Matrix.onStart(this);
	}
	@Override
	protected void onRestart() {
		Log.d(TAG, "onRestart");
		super.onRestart();
		Matrix.onRestart(this);
	}
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.d(TAG, "onActivityResult");
		super.onActivityResult(requestCode, resultCode, data);
		Matrix.onActivityResult(this,requestCode, resultCode, data);
	}
	@Override
	protected void onPause() {
		Log.d(TAG, "onPause");
		super.onPause();
		Matrix.onPause(this);
	}
	@Override
	protected void onStop() {
		Log.d(TAG, "onStop");
		super.onStop();
		Matrix.onStop(this);
	}
	@Override
	protected void onNewIntent(Intent intent) {
		Log.d(TAG, "onNewIntent");
		super.onNewIntent(intent);
		Matrix.onNewIntent(this,intent);
	}

	private void paintGame(){
//		Toast.makeText(qihooTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.d(TAG, "paintGame");
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("platForm", "android");
			jsonObj.put("sdk", "qihoo");
			jsonObj.put("iLoginId",7);//qihoo
			jsonObj.put("sdkPayCallName","verifyQihooPay");
			jsonObj.put("openId", mAccessToken);
		} catch (JSONException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
		final String jsonStr = jsonObj.toString();
		runOnGLThread(new Runnable() {
			@Override
			public void run() {
				TRGame.nativeSetPlatformLoginInfo(jsonStr);
			}
		});
	}


	public void doQihoopay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");

		int iFee = Integer.parseInt(totalFee)/100;

		boolean isFixed = true;
		int functionCode = ProtocolConfigs.FUNC_CODE_PAY;
		// 支付基础参数
		String qihooUserId = (mQihooUserInfo != null) ? mQihooUserInfo.getId() : null;
		if (qihooUserId == null){
			Log.d(TAG, "whenPay");
			doSdkLogin(true);
			return;
		}
		// 创建QihooPay
		QihooPayInfo qihooPay = new QihooPayInfo();
		qihooPay.setQihooUserId(qihooUserId);
		qihooPay.setAccessToken(mAccessToken);
		qihooPay.setMoneyAmount(String.valueOf(iFee*100));
		//qihooPay.setMoneyAmount(String.valueOf(iFee));
		qihooPay.setExchangeRate("10");

		qihooPay.setProductName(subject);
		qihooPay.setProductId(bodyAry[3]);

		qihooPay.setNotifyUri(TRGame.nativeGetNotifyUrl());

		qihooPay.setAppName("战棋天下");
		qihooPay.setAppUserName(bodyAry[23]);
		qihooPay.setAppUserId(bodyAry[17]);
		assert(mAppUserId == bodyAry[17]);
		mNickName = bodyAry[23];
		mOrderId = outTradeNo;
		// 可选参数
		qihooPay.setAppExt1(String.valueOf(bodyAry[5]));
//		qihooPay.setAppExt2(getString(R.string.demo_pay_app_ext2));
		qihooPay.setAppOrderId(outTradeNo);


		Intent intent = getPayIntent(true, qihooPay,functionCode);



		Bundle bundle = new Bundle();

		// 可选参数，默认支付类型
		bundle.putString(ProtocolKeys.DEFAULT_PAY_TYPE, ProtocolKeys.PayType.ALIPAY);
		intent.putExtras(bundle);

		// 必需参数，使用360SDK的支付模块。
		intent.putExtra(ProtocolKeys.FUNCTION_CODE, functionCode);

		Matrix.invokeActivity(this, intent, mPayCallback);



		// 必需参数，使用360SDK的支付模块:CP可以根据需求选择使用 带有收银台的支付模块 或者 直接调用微信支付模块或者直接调用支付宝支付模块。
		//functionCode 对应三种支付模块：
		//ProtocolConfigs.FUNC_CODE_PAY;//表示 带有360收银台的支付模块。
		//ProtocolConfigs.FUNC_CODE_WEIXIN_PAY;//表示 微信支付模块。
		//ProtocolConfigs.FUNC_CODE_ALI_PAY;//表示支付宝支付模块。

	}

	/***
	 * 生成调用360SDK支付接口的Intent
	 *
	 * @param isLandScape
	 * @param pay
	 * @return Intent
	 */
	protected Intent getPayIntent(boolean isLandScape, QihooPayInfo pay,int functionCode) {
		Bundle bundle = new Bundle();

		// 界面相关参数，360SDK界面是否以横屏显示。
		bundle.putBoolean(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE, isLandScape);

		// *** 以下非界面相关参数 ***

		// 设置QihooPay中的参数。

		// 必需参数，360账号id，整数。
		bundle.putString(ProtocolKeys.QIHOO_USER_ID, pay.getQihooUserId());

		// 必需参数，所购买商品金额, 以分为单位。金额大于等于100分，360SDK运行定额支付流程； 金额数为0，360SDK运行不定额支付流程。
		bundle.putString(ProtocolKeys.AMOUNT, pay.getMoneyAmount());

		// 必需参数，所购买商品名称，应用指定，建议中文，最大10个中文字。
		bundle.putString(ProtocolKeys.PRODUCT_NAME, pay.getProductName());

		// 必需参数，购买商品的商品id，应用指定，最大16字符。
		bundle.putString(ProtocolKeys.PRODUCT_ID, pay.getProductId());

		// 必需参数，应用方提供的支付结果通知uri，最大255字符。360服务器将把支付接口回调给该uri，具体协议请查看文档中，支付结果通知接口–应用服务器提供接口。
		bundle.putString(ProtocolKeys.NOTIFY_URI, pay.getNotifyUri());

		// 必需参数，游戏或应用名称，最大16中文字。
		bundle.putString(ProtocolKeys.APP_NAME, pay.getAppName());

		// 必需参数，应用内的用户名，如游戏角色名。 若应用内绑定360账号和应用账号，则可用360用户名，最大16中文字。（充值不分区服，
		// 充到统一的用户账户，各区服角色均可使用）。
		bundle.putString(ProtocolKeys.APP_USER_NAME, pay.getAppUserName());

		// 必需参数，应用内的用户id。
		// 若应用内绑定360账号和应用账号，充值不分区服，充到统一的用户账户，各区服角色均可使用，则可用360用户ID最大32字符。
		bundle.putString(ProtocolKeys.APP_USER_ID, pay.getAppUserId());

		// 可选参数，应用扩展信息1，原样返回，最大255字符。
		bundle.putString(ProtocolKeys.APP_EXT_1, pay.getAppExt1());

		// 可选参数，应用扩展信息2，原样返回，最大255字符。
		//bundle.putString(ProtocolKeys.APP_EXT_2, pay.getAppExt2());

		// 可选参数，应用订单号，应用内必须唯一，最大32字符。
		bundle.putString(ProtocolKeys.APP_ORDER_ID, pay.getAppOrderId());

		// 必需参数，使用360SDK的支付模块:CP可以根据需求选择使用 带有收银台的支付模块 或者 直接调用微信支付模块或者直接调用支付宝支付模块。
		//functionCode 对应三种支付模块：
		//ProtocolConfigs.FUNC_CODE_PAY;//表示 带有360收银台的支付模块。
		//ProtocolConfigs.FUNC_CODE_WEIXIN_PAY;//表示 微信支付模块。
		//ProtocolConfigs.FUNC_CODE_ALI_PAY;//表示支付宝支付模块。
		bundle.putInt(ProtocolKeys.FUNCTION_CODE,functionCode);

		// 必需参数，商品数量（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putInt(ProtocolKeys.PRODUCT_COUNT, 1);

		// 必需参数，服务器id（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.SERVER_ID, "1");

		// 必需参数，服务器名称（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.SERVER_NAME, "1");

		// 必需参数，兑换比例（demo模拟数据）（游戏内虚拟币兑换人民币） ,游戏内逻辑请传递游戏内真实数据
		bundle.putInt(ProtocolKeys.EXCHANGE_RATE, 10);

		// 必需参数，货币名称（demo模拟数据）（比如：钻石）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.GAMEMONEY_NAME, "勾玉");

		// 必需参数，角色id（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.ROLE_ID, mAppUserId);

		// 必需参数，角色名称（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.ROLE_NAME, mNickName);

		// 必需参数，角色等级（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putInt(ProtocolKeys.ROLE_GRADE, mRoleLevel);

		// 必需参数，虚拟币余额（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putInt(ProtocolKeys.ROLE_BALANCE, TRGame.nativeGetIntValue("jade"));

		// 必需参数，vip等级（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.ROLE_VIP, String.valueOf(TRGame.nativeGetIntValue("vip")));

		// 必需参数，工会名称（demo模拟数据）,游戏内逻辑请传递游戏内真实数据
		bundle.putString(ProtocolKeys.ROLE_USERPARTY, TRGame.nativeGetStrValue("guildName"));

		Intent intent = new Intent(this, ContainerActivity.class);
		intent.putExtras(bundle);

		return intent;
	}

	@Override
	public void doSDKpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) {
		final String foutTradeNo = outTradeNo;
		final String ftrGameId = trGameId;
		final String fsubject = subject;
		final String fbody = body;
		final String ftotalFee = totalFee;
		runOnUiThread(new Runnable(){
			@Override
			public void run()
			{
				//调用sdk支付

				try {
					doQihoopay(ftrGameId, foutTradeNo, fsubject, fbody, ftotalFee);
				} catch (Exception e) {
					Log.d(TAG, e.getLocalizedMessage());
					e.printStackTrace();
				}

			}
		});
	}

	private String valueProc(String v){
		if (v == null){
			return "无";
		}else{
			return v == ""?"无":v;
		}
	}
	private void doSubmitRoleData(String type,JSONObject jsonObj){
		try {
			HashMap eventParams=new HashMap();
			mNickName = jsonObj.getString("roleName");
			eventParams.put("zoneid",1);//当前角色所在游戏区服id(具体要求参考文档)
			eventParams.put("zonename","1");//当前角色所在游戏区服名称(具体要求参考文档)
			eventParams.put("roleid",mAppUserId);//当前角色id(具体要求参考文档)
			eventParams.put("rolename",mNickName);//当前角色名称(具体要求参考文档)
			eventParams.put("type",type);//角色信息接口触发的场景(具体要求参考文档)
			eventParams.put("professionid",0);
			eventParams.put("profession","无");
			eventParams.put("gender",jsonObj.getString("gender"));
			eventParams.put("power",jsonObj.getInt("totalCP"));
			eventParams.put("vip",jsonObj.getInt("vip"));
			eventParams.put("partyid",TRGame.nativeGetIntValue("guildId"));
			eventParams.put("partyname",valueProc(TRGame.nativeGetStrValue("guildName")));
			int isPatriarch = TRGame.nativeGetIntValue("isPatriarch");
			eventParams.put("partyroleid",isPatriarch);
			eventParams.put("partyrolename",valueProc(TRGame.nativeGetStrValue("patriarch")));
			eventParams.put("friendlist","无");



			//参数eventParams相关的 key、value键值对 相关具体使用说明，请参考文档。

			Matrix.statEventInfo(this, eventParams);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void submitExtendData(JSONObject jsonObj){
		String type = "";
		try {
			String flag = jsonObj.getString("flag");
			long createdAt = jsonObj.getLong("createdAt");
			long level = jsonObj.getLong("level");
			if (createdAt <= 0 || level <= 0){
				return;
			}
			mRoleLevel = new Long(level).intValue();
//			sdkParams.put(SDKParamKey.LONG_ROLE_CTIME,createdAt);
//			sdkParams.put(SDKParamKey.STRING_ROLE_ID, jsonObj.getString("roleId"));
//			sdkParams.put(SDKParamKey.LONG_ROLE_LEVEL, level);
//			sdkParams.put(SDKParamKey.STRING_ROLE_NAME, jsonObj.getString("roleName"));
			if (flag == "levelChange")
			{
				type = "levelUp";
			}else if (flag == "login"){
				type = "enterServer";
			}else if (flag == "createRole"){
				type = flag;
			}else if (flag == "levelChange"){
				type = "levelUp";
			}else{
				type = flag;
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
		doSubmitRoleData(type,jsonObj);
	}

	@Override
	public void annousCall(JSONObject jsonObj){
		String name = null;
		try {
			name = jsonObj.getString("name");
		} catch (JSONException e) {
			e.printStackTrace();
		}
		Log.d(TAG, "annousCall" + name);
		if (0 == name.compareTo("startSDKLogin")){
			doSdkLogin(true);
			return;
		}
		if (0 == name.compareTo("startSDKLogout")){
			doSdkLogout();
			return;
		}
		if (0 == name.compareTo("submitExtendData")){
			submitExtendData(jsonObj);
			return;
		}
		if (0 == name.compareTo("startSDKQuitGame")){
			doSdkQuit(true);
			return;
		}
		if (0 == name.compareTo("setExtUserInfo")){
			setQihooUserInfo(jsonObj);
			return;
		}
	}

	// ------------------注销登录----------------
	protected void doSdkLogout(){

		Intent intent = getLogoutIntent();
		Matrix.execute(this, intent, new IDispatcherCallback() {
			@Override
			public void onFinished(String data) {
				Log.d(TAG, "doSdkLogout onFinished");
				doSdkLogin(true);
			}
		});
	}

	private Intent getLogoutIntent(){

        /*
         * 必须参数：
         *  function_code : 必须参数，表示调用SDK接口执行的功能
        */
		Intent intent = new Intent();
		intent.putExtra(ProtocolKeys.FUNCTION_CODE, ProtocolConfigs.FUNC_CODE_LOGOUT);
		return intent;
	}

	/**
	 * 生成调用360SDK登录接口的Intent
	 * @param isLandScape 是否横屏
	 * @return intent
	 */
	private Intent getLoginIntent(boolean isLandScape) {

		Intent intent = new Intent(this, ContainerActivity.class);

		// 界面相关参数，360SDK界面是否以横屏显示。
		intent.putExtra(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE, isLandScape);

		// 必需参数，使用360SDK的登录模块。
		intent.putExtra(ProtocolKeys.FUNCTION_CODE, ProtocolConfigs.FUNC_CODE_LOGIN);

		//是否显示关闭按钮
		intent.putExtra(ProtocolKeys.IS_LOGIN_SHOW_CLOSE_ICON, false);

		// 可选参数，是否支持离线模式，默认值为false
//		intent.putExtra(ProtocolKeys.IS_SUPPORT_OFFLINE, getCheckBoxBoolean(R.id.isSupportOffline));

		// 可选参数，是否在自动登录的过程中显示切换账号按钮
		intent.putExtra(ProtocolKeys.IS_SHOW_AUTOLOGIN_SWITCH, true);

		// 可选参数，是否隐藏欢迎界面
		intent.putExtra(ProtocolKeys.IS_HIDE_WELLCOME, true);

		// 可选参数，登录界面的背景图片路径，必须是本地图片路径
//		intent.putExtra(ProtocolKeys.UI_BACKGROUND_PICTRUE, getUiBackgroundPicPath());
		// 可选参数，指定assets中的图片路径，作为背景图
//		intent.putExtra(ProtocolKeys.UI_BACKGROUND_PICTURE_IN_ASSERTS, getUiBackgroundPathInAssets());
		// 可选参数，是否需要用户输入激活码，用于游戏内测阶段。如果不需激活码相关逻辑，客户传false或者不传入该参数。
//		intent.putExtra(ProtocolKeys.NEED_ACTIVATION_CODE, getCheckBoxBoolean(R.id.isNeedActivationCode));

		//-- 以下参数仅仅针对自动登录过程的控制
		// 可选参数，自动登录过程中是否不展示任何UI，默认展示。
//		intent.putExtra(ProtocolKeys.IS_AUTOLOGIN_NOUI, getCheckBoxBoolean(R.id.isAutoLoginHideUI));

		// 可选参数，静默自动登录失败后是否显示登录窗口，默认不显示
		intent.putExtra(ProtocolKeys.IS_SHOW_LOGINDLG_ONFAILED_AUTOLOGIN, true);


		//-- 测试参数，发布时要去掉
//		intent.putExtra(ProtocolKeys.IS_SOCIAL_SHARE_DEBUG, getCheckBoxBoolean(R.id.isDebugSocialShare));

		return intent;
	}

	/***
	 * 生成调用360SDK切换账号接口的Intent
	 *
	 * @param isLandScape 是否横屏
	 * @return Intent
	 */
	private Intent getSwitchAccountIntent(boolean isLandScape) {

		Intent intent = new Intent(this, ContainerActivity.class);

		// 必须参数，360SDK界面是否以横屏显示。
		intent.putExtra(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE, isLandScape);

		// 必需参数，使用360SDK的切换账号模块。
		intent.putExtra(ProtocolKeys.FUNCTION_CODE, ProtocolConfigs.FUNC_CODE_SWITCH_ACCOUNT);

		//是否显示关闭按钮
		intent.putExtra(ProtocolKeys.IS_LOGIN_SHOW_CLOSE_ICON, false);

		// 可选参数，是否支持离线模式，默认值为false
//		intent.putExtra(ProtocolKeys.IS_SUPPORT_OFFLINE, getCheckBoxBoolean(R.id.isSupportOffline));

		// 可选参数，是否隐藏欢迎界面
		intent.putExtra(ProtocolKeys.IS_HIDE_WELLCOME, true);

        /*
         * 指定界面背景（可选参数）：
         *  1.ProtocolKeys.UI_BACKGROUND_PICTRUE 使用的系统路径，如/sdcard/1.png
         *  2.ProtocolKeys.UI_BACKGROUND_PICTURE_IN_ASSERTS 使用的assest中的图片资源，
         *    如传入bg.png字符串，就会在assets目录下加载这个指定的文件
         *  3.图片大小不要超过5M，尺寸不要超过1280x720
         */
		// 可选参数，登录界面的背景图片路径，必须是本地图片路径
		// 可选参数，登录界面的背景图片路径，必须是本地图片路径
//		intent.putExtra(ProtocolKeys.UI_BACKGROUND_PICTRUE, getUiBackgroundPicPath());
		// 可选参数，指定assets中的图片路径，作为背景图
//		intent.putExtra(ProtocolKeys.UI_BACKGROUND_PICTURE_IN_ASSERTS, getUiBackgroundPathInAssets());

		// 可选参数，是否需要用户输入激活码，用于游戏内测阶段。如果不需激活码相关逻辑，客户传false或者不传入该参数。
//		intent.putExtra(ProtocolKeys.NEED_ACTIVATION_CODE, getCheckBoxBoolean(R.id.isNeedActivationCode));

		// 测试参数，发布时要去掉
//		intent.putExtra(ProtocolKeys.IS_SOCIAL_SHARE_DEBUG, getCheckBoxBoolean(R.id.isDebugSocialShare));

		return intent;
	}

	// ---------------------------------360SDK接口的回调-----------------------------------

	// 登录、注册的回调
	private IDispatcherCallback mLoginCallback = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
			// press back
			if (isCancelLogin(data)) {
				SDKExitCancel();
				return;
			}
			// 显示一下登录结果
			//Toast.makeText(qihooTRGame.this, data, Toast.LENGTH_LONG).show();

			mQihooUserInfo = null;
//            Log.d(TAG, "mLoginCallback, data is " + data);
			// 解析access_token
			mAccessToken = parseAccessTokenFromLoginResult(data);

			if (!TextUtils.isEmpty(mAccessToken)) {
				// 需要去应用的服务器获取用access_token获取一下带qid的用户信息
				getUserInfo();
			} else {
				Toast.makeText(qihooTRGame.this, "get access_token failed!", Toast.LENGTH_LONG).show();
			}
		}
	};


	private IDispatcherCallback mLoginCallbackSupportOffline = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
			if (isCancelLogin(data)) {
				SDKExitCancel();
				return;
			}

//            Log.d(TAG, "mLoginCallbackSupportOffline, data is " + data);
			try {
				JSONObject joRes = new JSONObject(data);
				JSONObject joData = joRes.getJSONObject("data");
				String mode = joData.optString("mode", "");
				if (!TextUtils.isEmpty(mode) && mode.equals("offline")) {
					Toast.makeText(qihooTRGame.this, "login success in offline mode", Toast.LENGTH_SHORT).show();
					mIsInOffline = true;
					// 显示一下登录结果
					Toast.makeText(qihooTRGame.this, data, Toast.LENGTH_LONG).show();
				} else {
					mLoginCallback.onFinished(data);
				}
			} catch (Exception e) {
				Log.e(TAG, "mLoginCallbackSupportOffline exception", e);
			}

		}
	};
	private boolean isCancelLogin(String data) {
		try {
			JSONObject joData = new JSONObject(data);
			int errno = joData.optInt("errno", -1);
			if (-1 == errno) {
				//Toast.makeText(qihooTRGame.this, data, Toast.LENGTH_LONG).show();
				return true;
			}
		} catch (Exception e) {}
		return false;
	}
	// 切换账号的回调
	private IDispatcherCallback mAccountSwitchCallback = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
			// press back
			if (isCancelLogin(data)) {
				SDKExitCancel();
				return;
			}

			// 显示一下登录结果
			//Toast.makeText(qihooTRGame.this, data, Toast.LENGTH_LONG).show();

//            Log.d(TAG, "mAccountSwitchCallback, data is " + data);
			// 解析access_token
			mAccessToken = parseAccessTokenFromLoginResult(data);

			if (!TextUtils.isEmpty(mAccessToken)) {
				// 需要去应用的服务器获取用access_token获取一下带qid的用户信息
				getUserInfo();
			} else {
				Toast.makeText(qihooTRGame.this, "get access_token failed!", Toast.LENGTH_LONG).show();
			}
		}
	};

	private void setQihooUserInfo(JSONObject json){
		try {
			String sdkAccountData = json.getString("sdkAccountData");
			JSONObject joRes = new JSONObject(sdkAccountData);
			mQihooUserInfo = QihooUserInfo.parseUserInfo(joRes);
			mAppUserId = mQihooUserInfo.getId();
		} catch (JSONException e) {
			e.printStackTrace();
			Toast.makeText(qihooTRGame.this, "解析用户数据异常", Toast.LENGTH_LONG).show();
		}

	}

	private void getUserInfo(){
		handler.post(new Runnable() {

			@Override
			public void run() {
				paintGame();
			}
		});
	}

	private String parseAccessTokenFromLoginResult(String loginRes) {
		try {

			JSONObject joRes = new JSONObject(loginRes);
			JSONObject joData = joRes.getJSONObject("data");
			return joData.getString("access_token");
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	/**
	 * 使用360SDK的退出接口
	 *
	 * @param isLandScape 是否横屏显示支付界面
	 */
	protected void doSdkQuit(boolean isLandScape) {

		Bundle bundle = new Bundle();

		// 界面相关参数，360SDK界面是否以横屏显示。
		bundle.putBoolean(ProtocolKeys.IS_SCREEN_ORIENTATION_LANDSCAPE, isLandScape);

		// 必需参数，使用360SDK的退出模块。
		bundle.putInt(ProtocolKeys.FUNCTION_CODE, ProtocolConfigs.FUNC_CODE_QUIT);

		// 可选参数，登录界面的背景图片路径，必须是本地图片路径
		bundle.putString(ProtocolKeys.UI_BACKGROUND_PICTRUE, "");

		Intent intent = new Intent(this, ContainerActivity.class);
		intent.putExtras(bundle);

		Matrix.invokeActivity(this, intent, mQuitCallback);
	}

	private void preSubExtendData(String type){
		JSONObject json;
		try {
			String data = TRGame.nativeGetStrValue("getDataForSDKAnalys");
			json = new JSONObject(data);
			json.put("flag",type);
			submitExtendData(json);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 退出的回调
	private IDispatcherCallback mQuitCallback = new IDispatcherCallback() {

		@Override
		public void onFinished(String data) {
//            Log.d(TAG, "mQuitCallback, data is " + data);
			JSONObject json;
			try {
				json = new JSONObject(data);
				int which = json.optInt("which", -1);
				String label = json.optString("label");

//				Toast.makeText(qihooTRGame.this,
//						"按钮标识：" + which + "，按钮描述:" + label, Toast.LENGTH_LONG)
//						.show();

				switch (which) {
					case 0: // 用户关闭退出界面
						return;
					default:// 退出游戏
					{
						preSubExtendData("exitServer");
						finish();
						return;
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	};
}
