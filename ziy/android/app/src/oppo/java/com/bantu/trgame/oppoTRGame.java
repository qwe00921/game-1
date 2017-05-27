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

import com.nearme.game.sdk.GameCenterSDK;
import com.nearme.game.sdk.callback.ApiCallback;
import com.nearme.game.sdk.callback.GameExitCallback;
import com.nearme.game.sdk.common.model.ApiResult;
import com.nearme.game.sdk.common.model.biz.PayInfo;
import com.nearme.game.sdk.common.model.biz.ReportUserGameInfoParam;
import com.nearme.game.sdk.common.util.AppUtil;


public class oppoTRGame extends TRGame{
	private static final String TAG = "oppoTRGame";
	String m_uid;
	String m_session;
	String mOrderId;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		String appSecret = "aB8970d64343223b1BD2C7eC5c09900a";
		GameCenterSDK.init(appSecret, this);
		doSDKLogin();
    }

	/**
	 接口中的toast提示，只是demo中提示所用，游戏使用过程中请去掉这些toast提示语
	 */
	private void getVerifiedInfo() {
		GameCenterSDK.getInstance().doGetVerifiedInfo(new ApiCallback() {
			@Override
			public void onSuccess(String resultMsg) {
				try {
					//解析年龄age
					int age=Integer.parseInt(resultMsg);
					if (age < 18) {
						Toast.makeText(oppoTRGame.this, "已实名但未成年，CP开始处理防沉迷",Toast.LENGTH_SHORT).show();
					} else {
						Toast.makeText(oppoTRGame.this, "已实名且已成年，尽情玩游戏吧~",Toast.LENGTH_SHORT).show();
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}

			@Override
			public void onFailure(String resultMsg, int resultCode) {
				if(resultCode == ApiResult.RESULT_CODE_VERIFIED_FAILED_AND_RESUME_GAME){
					Toast.makeText(oppoTRGame.this, resultMsg+"，还可以继续玩游戏",Toast.LENGTH_SHORT).show();
				}else if(resultCode == ApiResult.RESULT_CODE_VERIFIED_FAILED_AND_STOP_GAME){
					Toast.makeText(oppoTRGame.this, resultMsg+",CP自己处理退出游戏",Toast.LENGTH_SHORT).show();
				}
			}
		});
	}

	public void doGetTokenAndSsoid() {
		GameCenterSDK.getInstance().doGetTokenAndSsoid(new ApiCallback() {

			@Override
			public void onSuccess(String resultMsg) {
				try {
					JSONObject json = new JSONObject(resultMsg);
					m_session = json.getString("token");
					m_uid = json.getString("ssoid");
					getUserInfo();
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}

			@Override
			public void onFailure(String content, int resultCode) {
				Log.i(TAG, content);
			}
		});
	}


	private void startGame() {

	}

	private void paintGame(){
//		Toast.makeText(oppoTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.i(TAG, "paintGame");
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("platForm", "android");
			jsonObj.put("sdk", "oppo");
			jsonObj.put("uid", m_uid);
			jsonObj.put("sdkPayCallName","verifyOppoPay");
			jsonObj.put("openId", m_session);
			jsonObj.put("iLoginId",12);//oppo
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
		Log.d(TAG, "onStart");
		super.onStart();

	}
    @Override
    protected void onDestroy() {
		Log.d(TAG, "onDestroy");
		super.onDestroy();

    }

	public void doMIpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");

		mOrderId = outTradeNo;

// CP 支付参数
		int amount =  Integer.parseInt(totalFee); // 支付金额，单位分
		PayInfo payInfo = new PayInfo(outTradeNo, outTradeNo, amount);
		payInfo.setProductDesc(subject);
		payInfo.setProductName(subject);
		payInfo.setCallbackUrl(TRGame.nativeGetNotifyUrl());

		GameCenterSDK.getInstance().doPay(oppoTRGame.this, payInfo, new ApiCallback() {

			@Override
			public void onSuccess(String resultMsg) {
				//Log.d(TAG, resultMsg);
				Log.d(TAG, "onSuccess");
				handler.postDelayed(new Runnable() {
					@Override
					public void run() {
						payResult(mOrderId,true);
					}
				}, 1000);
			}

			@Override
			public void onFailure(String resultMsg, int resultCode) {
				//Log.d(TAG, resultMsg);
				payResult(mOrderId,false);
//				if (PayResponse.CODE_CANCEL != resultCode) {
//					Toast.makeText(oppoTRGame.this, "支付失败",
//							Toast.LENGTH_SHORT).show();
//
//				} else {
//					// 取消支付处理
//					Toast.makeText(oppoTRGame.this, "支付取消",
//							Toast.LENGTH_SHORT).show();
//				}
			}
		});
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

	private void submitExtendData(JSONObject jsonObj){
		String roleName = "";
		long level = 0;
		try {
			long createdAt = jsonObj.getLong("createdAt");
			level = jsonObj.getLong("level");
			if (createdAt <= 0 || level <= 0){
				return;
			}
			roleName = jsonObj.getString("roleName");
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (level <= 0){
			return;
		}
		GameCenterSDK.getInstance().doReportUserGameInfoData(
				new ReportUserGameInfoParam("3567177", "1",roleName ,
						String.valueOf(level)), new ApiCallback() {

					@Override
					public void onSuccess(String resultMsg) {
//						Log.d(TAG,resultMsg);
//						Toast.makeText(oppoTRGame.this, "success",
//								Toast.LENGTH_LONG).show();
					}

					@Override
					public void onFailure(String resultMsg, int resultCode) {
						Log.d(TAG,resultMsg);
//						Toast.makeText(oppoTRGame.this, resultMsg,
//								Toast.LENGTH_LONG).show();
					}
				});
	}
	private void doSDKLogin(){
		GameCenterSDK.getInstance().doLogin(this, new ApiCallback() {

			@Override
			public void onSuccess(String resultMsg) {
				//Log.d(TAG,resultMsg);
//				Toast.makeText(oppoTRGame.this, resultMsg, Toast.LENGTH_LONG)
//						.show();
				doGetTokenAndSsoid();
			}

			@Override
			public void onFailure(String resultMsg, int resultCode) {
				Log.d(TAG,resultMsg);
				Toast.makeText(oppoTRGame.this, resultMsg, Toast.LENGTH_LONG)
						.show();
			}
		});
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
		GameCenterSDK.getInstance().onExit(oppoTRGame.this,
				new GameExitCallback() {

					@Override
					public void exitGame() {
						// CP 实现游戏退出操作，也可以直接调用
						// AppUtil工具类里面的实现直接强杀进程~
						AppUtil.exitGameProcess(oppoTRGame.this);
					}
				});
	}

	@Override
	protected void onResume() {
		Log.d(TAG, "onResume");
		super.onResume();
		SDKExitCancel();
		GameCenterSDK.getInstance().onResume(this);
	}

	@Override
	protected void onPause() {
		GameCenterSDK.getInstance().onPause();
		super.onPause();
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
