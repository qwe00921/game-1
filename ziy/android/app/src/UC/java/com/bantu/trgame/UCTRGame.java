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


import java.util.HashMap;

import java.util.Map;


import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import android.widget.Toast;



import cn.uc.gamesdk.UCGameSdk;
import cn.uc.gamesdk.even.SDKEventKey;
import cn.uc.gamesdk.even.SDKEventReceiver;
import cn.uc.gamesdk.even.Subscribe;
import cn.uc.gamesdk.exception.AliLackActivityException;
import cn.uc.gamesdk.exception.AliNotInitException;
import cn.uc.gamesdk.open.GameParamInfo;
import cn.uc.gamesdk.open.OrderInfo;
import cn.uc.gamesdk.open.UCOrientation;
import cn.uc.gamesdk.param.SDKParamKey;
import cn.uc.gamesdk.param.SDKParams;

import com.uc.UCSdkConfig;
import com.uc.account.AccountInfo;
import com.uc.util.APNUtil;


public class UCTRGame extends TRGame {
	private static final String TAG = "UCTRGame";
	//uc
	public boolean mRepeatCreate = false;
	private String mPullUpInfo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		if ((getIntent().getFlags() & Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT) != 0) {
			Log.i(TAG, "onCreate with flag FLAG_ACTIVITY_BROUGHT_TO_FRONT");
			mRepeatCreate = true;
			finish();
			return;
		}
		//uc
		mPullUpInfo = getPullupInfo(getIntent());
		ucNetworkAndInitUCGameSDK(mPullUpInfo);
		UCGameSdk.defaultSdk().registerSDKEventReceiver(receiver);
    }

//	public void onBackPressed() {
//		ucSdkExit();
//	}

	void ucSdkLogin() {
		try {
			UCGameSdk.defaultSdk().login(this, null);
		} catch (AliLackActivityException e) {
			e.printStackTrace();
		} catch (AliNotInitException e) {
			e.printStackTrace();
		}
	}

	void ucSdkLogout() {

		try {
			UCGameSdk.defaultSdk().logout(this, null);
		} catch (AliLackActivityException e) {
			e.printStackTrace();
		} catch (AliNotInitException e) {
			e.printStackTrace();
		}
	}

	void ucSdkExit(){
		try {
			UCGameSdk.defaultSdk().exit(this, null);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}



	public void ucNetworkAndInitUCGameSDK(String pullUpInfo) {
		//!!!在调用SDK初始化前进行网络检查
		//当前没有拥有网络
		if (false == APNUtil.isNetworkAvailable(this)) {
			AlertDialog.Builder ab = new AlertDialog.Builder(this);
			ab.setMessage("网络未连接,请设置网络");
			ab.setPositiveButton("设置", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					Intent intent = new Intent("android.settings.SETTINGS");
					startActivityForResult(intent, 0);
				}
			});
			ab.setNegativeButton("退出", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					System.exit(0);
				}
			});
			ab.show();
		} else {
			ucSdkInit(pullUpInfo);//执行UCGameSDK初始化
		}
	}

	private String getPullupInfo(Intent intent) {
		if(intent == null){
			return null;
		}
		String pullupInfo = intent.getDataString();
		return pullupInfo;
	}

	private void ucSdkInit(String pullupInfo) {
		GameParamInfo gameParamInfo = new GameParamInfo();
		//gameParamInfo.setCpId(UCSdkConfig.cpId);已废用
		gameParamInfo.setGameId(UCSdkConfig.gameId);
		//gameParamInfo.setServerId(UCSdkConfig.serverId);已废用
		gameParamInfo.setOrientation(UCOrientation.LANDSCAPE);

		SDKParams sdkParams = new SDKParams();

		sdkParams.put(SDKParamKey.GAME_PARAMS, gameParamInfo);


		//FIXME always debug
		sdkParams.put(SDKParamKey.DEBUG_MODE, UCSdkConfig.debugMode);

		sdkParams.put(SDKParamKey.PULLUP_INFO,pullupInfo);

		try {
			UCGameSdk.defaultSdk().initSdk(this, sdkParams);
		} catch (AliLackActivityException e) {
			e.printStackTrace();
		}
	}


	private void startGame() {
		ucSdkLogin();
	}

	private void paintGame(){
//		Toast.makeText(UCTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.i(TAG, "paintGame");
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("platForm", "android");
			jsonObj.put("sdk", "uc");
			jsonObj.put("sdkPayCallName","verifyUCPay");
			jsonObj.put("openId", AccountInfo.instance().getSid());
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



	SDKEventReceiver receiver = new SDKEventReceiver() {
		@Subscribe(event = SDKEventKey.ON_INIT_SUCC)
		private void onInitSucc() {
			//初始化成功
			handler.post(new Runnable() {

				@Override
				public void run() {
					startGame();
				}
			});
		}

		@Subscribe(event = SDKEventKey.ON_INIT_FAILED)
		private void onInitFailed(String data) {
			//初始化失败
			//Toast.makeText(UCTRGame.this, "init failed", Toast.LENGTH_SHORT).show();
			ucNetworkAndInitUCGameSDK(mPullUpInfo);
		}

		@Subscribe(event = SDKEventKey.ON_LOGIN_SUCC)
		private void onLoginSucc(String sid) {
			//Toast.makeText(UCTRGame.this, "login succ,sid=" + sid, Toast.LENGTH_SHORT).show();
			final UCTRGame me = UCTRGame.this;
			AccountInfo.instance().setSid(sid);
			handler.post(new Runnable() {

				@Override
				public void run() {
					paintGame();
				}
			});

		}


		@Subscribe(event = SDKEventKey.ON_LOGIN_FAILED)
		private void onLoginFailed(String desc) {
//			Toast.makeText(UCTRGame.this,desc, Toast.LENGTH_SHORT).show();
//            printMsg(desc);
		}

		@Subscribe(event = SDKEventKey.ON_CREATE_ORDER_SUCC)
		private void onCreateOrderSucc(OrderInfo orderInfo) {
//			dumpOrderInfo(orderInfo);
//			if (orderInfo != null) {
//				String txt = orderInfo.getOrderAmount() + "," + orderInfo.getOrderId() + "," + orderInfo.getPayWay();
//				Toast.makeText(UCTRGame.this, "订单已生成，获取支付结果请留意服务端回调"+txt, Toast.LENGTH_SHORT).show();
//			}
			Log.i(TAG, "pay create succ");
		}
		private void dumpOrderInfo(OrderInfo orderInfo) {
			if (orderInfo != null) {
				StringBuilder sb = new StringBuilder();
				sb.append(String.format("'orderId':'%s'", orderInfo.getOrderId()));
				sb.append(String.format("'orderAmount':'%s'", orderInfo.getOrderAmount()));
				sb.append(String.format("'payWay':'%s'", orderInfo.getPayWay()));
				sb.append(String.format("'payWayName':'%s'", orderInfo.getPayWayName()));

				Log.i(TAG, "callback orderInfo = " + sb);
			}
		}



		private void payResult(String payId){
			JSONObject jsonObj = new JSONObject();
			try {
				jsonObj.put("payId",payId);
				jsonObj.put("getOrderStatus", true);
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

		@Subscribe(event = SDKEventKey.ON_PAY_USER_EXIT)
		private void onPayUserExit(OrderInfo orderInfo) {
			//dumpOrderInfo(orderInfo);
			if (orderInfo != null) {
				String txt = orderInfo.getOrderAmount() + "," + orderInfo.getOrderId() + "," + orderInfo.getPayWay();
//				Toast.makeText(UCTRGame.this, "支付界面关闭"+txt, Toast.LENGTH_SHORT).show();
				Log.i(TAG, "支付界面关闭"+txt);
				payResult(orderInfo.getOrderId());
			}
			Log.i(TAG, "pay exit");

		}


		@Subscribe(event = SDKEventKey.ON_LOGOUT_SUCC)
		private void onLogoutSucc() {
			//Toast.makeText(UCTRGame.this, "logout succ", Toast.LENGTH_SHORT).show();
			ucSdkLogin();
		}

		@Subscribe(event = SDKEventKey.ON_LOGOUT_FAILED)
		private void onLogoutFailed() {
			Toast.makeText(UCTRGame.this, "注销失败", Toast.LENGTH_SHORT).show();
//            printMsg("注销失败");
		}

		@Subscribe(event = SDKEventKey.ON_EXIT_SUCC)
		private void onExit(String desc) {
			//Toast.makeText(UCTRGame.this, desc, Toast.LENGTH_SHORT).show();
			Log.i(TAG, "onExit "+desc);
			UCTRGame.this.finish();

			// 退出程序
//            Intent intent = new Intent(Intent.ACTION_MAIN);
//            intent.addCategory(Intent.CATEGORY_HOME);
//            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
//            startActivity(intent);
			android.os.Process.killProcess(android.os.Process.myPid());
//            printMsg(desc);
		}

		@Subscribe(event = SDKEventKey.ON_EXIT_CANCELED)
		private void onExitCanceled(String desc) {
			Toast.makeText(UCTRGame.this, desc, Toast.LENGTH_SHORT).show();

			SDKExitCancel();
		}



	};


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	super.onActivityResult(requestCode, resultCode, data);

		if (this.mRepeatCreate) {
			Log.i(TAG, "onActivityResult is repeat activity!");
			return;
		}
		ucNetworkAndInitUCGameSDK(mPullUpInfo);
    }
	@Override
	protected void onStart() {
		super.onStart();
		if (this.mRepeatCreate) {
			Log.i(TAG, "onStart is repeat activity!");
			return;
		}
	}
    @Override
    protected void onDestroy() {
		super.onDestroy();
		if (this.mRepeatCreate) {
			Log.i(TAG, "onDestroy is repeat activity!");
			return;
		}
		Log.d("UCTRGame", "----------onDestroy---------");
		UCGameSdk.defaultSdk().unregisterSDKEventReceiver(receiver);
    }

	public void doUCpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");
		Map<String, String> paramMap = new HashMap<String, String>();
		paramMap.put(SDKParamKey.CALLBACK_INFO, outTradeNo);
		int iFee = Integer.parseInt(totalFee)/100;
		Log.d("UCTRGame",String.valueOf(iFee));
		paramMap.put(SDKParamKey.NOTIFY_URL, TRGame.nativeGetNotifyUrl());
		paramMap.put(SDKParamKey.AMOUNT, String.valueOf(iFee));
		paramMap.put(SDKParamKey.CP_ORDER_ID, outTradeNo);
		paramMap.put(SDKParamKey.ACCOUNT_ID, bodyAry[17]);
		paramMap.put(SDKParamKey.SIGN_TYPE, "MD5");

		SDKParams sdkParams = new SDKParams();

		Map<String, Object> map = new HashMap<String, Object>();
		map.putAll(paramMap);
		sdkParams.putAll(map);


		sdkParams.put(SDKParamKey.SIGN, bodyAry[15]);
		System.out.println("sdkParams:"+sdkParams.toString());

		UCGameSdk.defaultSdk().pay(this, sdkParams);
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
					doUCpay(ftrGameId, foutTradeNo, fsubject, fbody, ftotalFee);
				} catch (Exception e) {
					Log.d(TAG, e.getLocalizedMessage());
					e.printStackTrace();
				}

			}
		});
	}

	private void submitExtendData(JSONObject jsonObj){
		SDKParams sdkParams = new SDKParams();
		sdkParams.put(SDKParamKey.STRING_ZONE_ID, "1");
		sdkParams.put(SDKParamKey.STRING_ZONE_NAME, "1");
		try {
			String flag = jsonObj.getString("flag");
			long createdAt = jsonObj.getLong("createdAt");
			long level = jsonObj.getLong("level");
			if (createdAt <= 0 || level <= 0){
				return;
			}
			sdkParams.put(SDKParamKey.LONG_ROLE_CTIME,createdAt);
			sdkParams.put(SDKParamKey.STRING_ROLE_ID, jsonObj.getString("roleId"));
			sdkParams.put(SDKParamKey.LONG_ROLE_LEVEL, level);
			sdkParams.put(SDKParamKey.STRING_ROLE_NAME, jsonObj.getString("roleName"));

		} catch (JSONException e) {
			e.printStackTrace();
		}
		try {
			UCGameSdk.defaultSdk().submitRoleData(this, sdkParams);
		} catch (AliNotInitException e) {
			e.printStackTrace();
		} catch (AliLackActivityException e) {
			e.printStackTrace();
		}
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
			ucSdkLogin();
			return;
		}
		if (0 == name.compareTo("startSDKLogout")){
			ucSdkLogout();
			return;
		}
		if (0 == name.compareTo("submitExtendData")){
			submitExtendData(jsonObj);
			return;
		}
		if (0 == name.compareTo("startSDKQuitGame")){
			ucSdkExit();
			return;
		}

	}


}
