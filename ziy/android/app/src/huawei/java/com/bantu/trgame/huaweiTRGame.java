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
import android.util.Base64;
import android.util.Log;

import android.widget.Toast;

import com.bantu.R;
import com.huawei.gameservice.sdk.GameServiceSDK;
import com.huawei.gameservice.sdk.api.GameEventHandler;
import com.huawei.gameservice.sdk.api.Result;
import com.huawei.gameservice.sdk.api.UserResult;
import com.huawei.gameservice.sdk.model.RoleInfo;
import com.huawei.gameservice.sdk.util.StringUtil;
import com.huawei.gameservice.sdk.view.GameServiceBaseActivity;
import com.huawei.gameservice.sdk.api.PayResult;


public class huaweiTRGame extends TRGame {
	private static final String TAG = "huaweiTRGame";
	String m_uid;
	String m_ts;
	String m_session;
	String mOrderId;
	String buoyPrivateKey = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		checkSDKKey();
    }

	/**
	 * 支付回调handler
	 */
	/**
	 * pay handler
	 */
	private GameEventHandler payHandler = new GameEventHandler()
	{
		@Override
		public String getGameSign(String appId, String cpId, String ts) {
			Log.d(TAG,"payHandler getGameSign");
			return createGameSign(appId + cpId + ts);
		}

		@Override
		public void onResult(Result result)
		{
			Log.d(TAG,"payHandler onResult");

			Map<String, String> payResp = ((PayResult)result).getResultMap();
			//String pay = "pay_result_failed";
			// 支付成功，进行验签
			// payment successful, then check the response value
			if ("0".equals(payResp.get("returnCode")))
			{
				if ("success".equals(payResp.get("errMsg")))
				{
//					// 支付成功，验证信息的安全性；待验签字符串中如果有isCheckReturnCode参数且为yes，则去除isCheckReturnCode参数
//					// If the response value contain the param "isCheckReturnCode" and its value is yes, then remove the param "isCheckReturnCode".
//					if (payResp.containsKey("isCheckReturnCode") && "yes".equals(payResp.get("isCheckReturnCode")))
//					{
//						payResp.remove("isCheckReturnCode");
//
//					}
//					// 支付成功，验证信息的安全性；待验签字符串中如果没有isCheckReturnCode参数活着不为yes，则去除isCheckReturnCode和returnCode参数
//					// If the response value does not contain the param "isCheckReturnCode" and its value is yes, then remove the param "isCheckReturnCode".
//					else
//					{
//						payResp.remove("isCheckReturnCode");
//						payResp.remove("returnCode");
//					}
//					// 支付成功，验证信息的安全性；待验签字符串需要去除sign参数
//					// remove the param "sign" from response
//					String sign = payResp.remove("sign");
//
//					String noSigna = GameBoxUtil.getSignData(payResp);
//
//					// 使用公钥进行验签
//					// check the sign using RSA public key
//					boolean s = RSAUtil.doCheck(noSigna, sign, GlobalParam.PAY_RSA_PUBLIC);

					handler.postDelayed(new Runnable() {
						@Override
						public void run() {
							payResult(mOrderId,true);
						}
					},1000);

					return;
				}

			}
			else if ("30002".equals(payResp.get("returnCode")))
			{
//				pay = getString(R.string.pay_result_timeout);
			}
			handler.postDelayed(new Runnable() {
				@Override
				public void run() {
					payResult(mOrderId,false);
				}
			},1000);

		}
	};


	private void startGame() {

	}

	private void paintGame(){
//		Toast.makeText(huaweiTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.i(TAG, "paintGame");
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("platForm", "android");
			jsonObj.put("sdk", "huawei");
			jsonObj.put("uid", m_uid);
			jsonObj.put("ts",m_ts);
			jsonObj.put("sdkPayCallName","verifyHuaweiPay");
			jsonObj.put("openId", m_session);
			jsonObj.put("iLoginId",11);//huawei
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


	/**
	 * 生成游戏签名 generate the game sign
	 */
	private String createGameSign(String data) {

		// 为了安全把浮标密钥放到服务端，并使用https的方式获取下来存储到内存中，CP可以使用自己的安全方式处理
		// For safety, buoy key put into the server and use the https way to get
		// down into the client's memory.
		// By the way CP can also use their safe approach.
		String finalRs;
		String str = data;
		try {
			String result = RSAUtil.sha256WithRsa(str.getBytes("UTF-8"), buoyPrivateKey);
			finalRs = result;
		} catch (Exception e) {
			e.printStackTrace();
			finalRs = null;
		}
		return finalRs;
	}

	private void checkSDKKey(){
		if (buoyPrivateKey == null) {
			// 为了安全把浮标密钥放到服务端，并使用https的方式获取下来存储到内存中，CP可以使用自己的安全方式处理
			// For safety, buoy key put into the server and use the https way to get
			// down into the client's memory.
			// By the way CP can also use their safe approach.
			ReqTask getBuoyPrivate = new ReqTask(new ReqTask.Delegate() {

				@Override
				public void execute(String result) {
					/**
					 * 从服务端获取的浮标私钥，由于没有部署最终的服务端，所以返回值写死一个值，CP需要从服务端获取，服务端代码参考华为Demo
					 * The demo app did not deployed the server, so the return value
					 * is written fixed.For real app,the CP need to get the buoy key
					 * from server.
					 */
					try {
						JSONObject resultJs = new JSONObject(result);
						buoyPrivateKey = resultJs.getString("login");
						GlobalParam.PAY_RSA_PUBLIC = resultJs.getString("pay");
						// SDK初始化
						// SDK initialization
						initSDK();
					} catch (JSONException e) {
						Log.d(TAG, e.getLocalizedMessage());
					}
				}
			}, null, GlobalParam.GET_BUOY_PRIVATE_KEY);
			getBuoyPrivate.execute();
		}else{
			initSDK();
		}
	}

	private void initSDKFail(){
		AlertDialog.Builder ab = new AlertDialog.Builder(this);
		ab.setMessage("初始化SDK失败");
		ab.setPositiveButton("重试", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				checkSDKKey();
			}
		});
		ab.setNegativeButton("退出游戏", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				System.exit(0);
			}
		});
		ab.show();
	}

	/**
	 * 初始化 initialization
	 */
	private void initSDK() {
		GameServiceSDK.init(huaweiTRGame.this, GlobalParam.APP_ID, GlobalParam.PAY_ID,
				"com.huawei.gb.huawei.installnewtype.provider", new GameEventHandler() {

					@Override
					public void onResult(Result result) {
						if (result.rtnCode != Result.RESULT_OK) {
							handleError("init the game service SDK failed:" + result.rtnCode);
							initSDKFail();
							return;
						}
						login(1);
						checkUpdate();
					}

					@Override
					public String getGameSign(String appId, String cpId, String ts) {
						return createGameSign(appId + cpId + ts);
					}

				});
	}


	/**
	 * 检测游戏更新 check the update for game
	 */
	private void checkUpdate() {
		GameServiceSDK.checkUpdate(huaweiTRGame.this, new GameEventHandler() {

			@Override
			public void onResult(Result result) {
				if (result.rtnCode != Result.RESULT_OK) {
					handleError("check update failed:" + result.rtnCode);
				}
			}

			@Override
			public String getGameSign(String appId, String cpId, String ts) {
				return createGameSign(appId + cpId + ts);
			}

		});
	}


	private void getPlayerLevel() {

		GameServiceSDK.getPlayerLevel(huaweiTRGame.this, new GameEventHandler() {

			@Override
			public void onResult(Result result) {
				if (result.rtnCode != Result.RESULT_OK) {
					handleError("get player level failed:" + result.rtnCode);
					return;
				}
				UserResult userResult = (UserResult) result;
				handleError("player level:" + userResult.playerLevel);
			}

			@Override
			public String getGameSign(String appId, String cpId, String ts) {
				return createGameSign(appId + cpId + ts);
			}

		});
	}
	/**
	 * 帐号登录 Login
	 */
	private void login(int authType) {
		GameServiceSDK.login(huaweiTRGame.this, new GameEventHandler() {

			@Override
			public void onResult(Result result) {
				if (result.rtnCode != Result.RESULT_OK) {
					handleError("login failed:" + result.toString());
					if (result.rtnCode == Result.RESULT_ERR_NOT_INIT){
						checkSDKKey();
					}
				} else {
					UserResult userResult = (UserResult) result;
					if (userResult.isAuth != null && userResult.isAuth == 1) {
							m_session = userResult.gameAuthSign;
							m_uid = userResult.playerId;
							m_ts = userResult.ts;
							getUserInfo();

					} else if (userResult.isChange != null && userResult.isChange == 1) {
						login(1);
					} else {
						handleError("login success:" + userResult.toString());

					}

				}
			}

			@Override
			public String getGameSign(String appId, String cpId, String ts) {
				return createGameSign(appId + cpId + ts);
			}

		}, authType);
	}

	private void handleError(String errorMsg) {
		Log.d(TAG,errorMsg);
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



    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	super.onActivityResult(requestCode, resultCode, data);


    }
	@Override
	protected void onStart() {
		super.onStart();

	}
    @Override
    protected void onDestroy() {
		super.onDestroy();
		if(this.isTaskRoot()) {
			GameServiceSDK.destroy(this);
		}
    }

	public void doMIpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");
		int iFee = Integer.parseInt(totalFee)/100;
		mOrderId = outTradeNo;
		String signStr = bodyAry[15];
		GameBoxUtil.pay(this,String.valueOf(iFee)+".00",subject,subject,mOrderId,signStr,payHandler);
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
					doMIpay(ftrGameId, foutTradeNo, fsubject, fbody, ftotalFee);
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

	private void submitExtendData(JSONObject jsonObj){

		HashMap<String, String> playerInfo = new HashMap<String, String>();

		try {

			long createdAt = jsonObj.getLong("createdAt");
			long level = jsonObj.getLong("level");
			if (createdAt <= 0 || level <= 0){
				return;
			}


			/**
			 * 将用户的等级等信息保存起来，必须的参数为RoleInfo.GAME_RANK(等级)/RoleInfo.GAME_ROLE(角色名称)
			 * /RoleInfo.GAME_AREA(角色所属区)/RoleInfo.GAME_SOCIATY(角色所属公会名称)
			 * 全部使用String类型存放
			 */
			/**
			 * the CP save the level, role, area and sociaty of the game player into
			 * the SDK
			 */
			playerInfo.put(RoleInfo.GAME_RANK, String.valueOf(level));
			playerInfo.put(RoleInfo.GAME_ROLE, jsonObj.getString("roleName"));
//			playerInfo.put(RoleInfo.GAME_AREA, "1");
			String guildName = valueProc(TRGame.nativeGetStrValue("guildName"));
			if (guildName != "无"){
				playerInfo.put(RoleInfo.GAME_SOCIATY, guildName);
			}

		} catch (JSONException e) {
			e.printStackTrace();
		}

		GameServiceSDK.addPlayerInfo(huaweiTRGame.this, playerInfo, new GameEventHandler() {

			@Override
			public void onResult(Result result) {
				if (result.rtnCode != Result.RESULT_OK) {
					handleError("add player info failed:" + result.rtnCode);
				}

			}

			@Override
			public String getGameSign(String appId, String cpId, String ts) {
				return createGameSign(appId + cpId + ts);
			}

		});
	}
	private void doSDKLogin(){
		login(1);
	}

	private void getUserInfo(){
		handler.post(new Runnable() {

			@Override
			public void run() {
				paintGame();
			}
		});
	}
	private void doSDKQuit(){

	}

	@Override
	protected void onResume() {
		Log.d(TAG, "onResume");
		super.onResume();
		GameServiceSDK.showFloatWindow(this);
		SDKExitCancel();
	}

	protected void onPause() {
		super.onPause();
		GameServiceSDK.hideFloatWindow(this);
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
			doSDKLogin();
			return;
		}
		if (0 == name.compareTo("startSDKLogout")){

			return;
		}
		if (0 == name.compareTo("submitExtendData")){
			submitExtendData(jsonObj);
			return;
		}
		if (0 == name.compareTo("startSDKQuitGame")){
			doSDKQuit();
			return;
		}

	}


}
