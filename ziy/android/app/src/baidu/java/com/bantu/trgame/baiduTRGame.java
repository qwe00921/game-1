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

import com.baidu.gamesdk.BDGameSDK;
import com.baidu.gamesdk.IResponse;
import com.baidu.gamesdk.OnGameExitListener;
import com.baidu.gamesdk.ResultCode;
import com.baidu.platformsdk.PayOrderInfo;


public class baiduTRGame extends TRGame {
	private static final String TAG = "baiduTRGame";

	private static boolean s_showFloatView = false;
	String m_uid;
	String m_session;
//	String mOrderId;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		BDGameSDK.getAnnouncementInfo(this);
		setSuspendWindowChangeAccountListener();


		setSessionInvalidListener(); // 设置会话失效监听



		doSDKLogin();
    }

	/**
	 * @Description: 监听session失效时重新登录
	 */
	private void setSessionInvalidListener() {
		BDGameSDK.setSessionInvalidListener(new IResponse<Void>() {

			@Override
			public void onResponse(int resultCode, String resultDesc,
								   Void extraData) {
				if (resultCode == ResultCode.SESSION_INVALID) {
					// 会话失效，开发者需要重新登录或者重启游戏
					doSDKLogin();
				}

			}

		});
	}
	@Override
	public void finish() {
		// TODO Auto-generated method stub
		super.finish();
		BDGameSDK.closeFloatView(this); // 关闭悬浮窗
	}


	private void setSuspendWindowChangeAccountListener() { // 设置切换账号事件监听（个人中心界面切换账号）
		BDGameSDK.setSuspendWindowChangeAccountListener(new IResponse<Void>() {

			@Override
			public void onResponse(int resultCode, String resultDesc,
								   Void extraData) {
				Log.d(TAG,"setSuspendWindowChangeAccountListener onResponse"+resultCode + resultDesc);
				switch (resultCode) {
					case ResultCode.LOGIN_SUCCESS:
						// TODO 登录成功，不管之前是什么登录状态，游戏内部都要切换成新的用户
						m_uid = BDGameSDK.getLoginUid(); // TODO 切换账号成功后必须更新uid给调用支付api使用
						m_session = BDGameSDK.getLoginAccessToken();
//						Toast.makeText(getApplicationContext(), "登录成功",
//								Toast.LENGTH_LONG).show();
						Log.d(TAG,"登录成功");
						getUserInfo();
						break;
					case ResultCode.LOGIN_FAIL:
						// TODO
						// 登录失败，游戏内部之前如果是已经登录的，要清除自己记录的登录状态，设置成未登录。如果之前未登录，不用处理。
//						Toast.makeText(getApplicationContext(), "登录失败",
//								Toast.LENGTH_LONG).show();
						Log.d(TAG,"登录失败");
						break;
					case ResultCode.LOGIN_CANCEL:
						// TODO 取消，操作前后的登录状态没变化
						break;
					default:
						// TODO
						// 此时当登录失败处理，参照ResultCode.LOGIN_FAIL（正常情况下不会到这个步骤，除非SDK内部异常）
//						Toast.makeText(getApplicationContext(), "登录失败",
//								Toast.LENGTH_LONG).show();
						break;

				}
			}

		});
	}


	private void startGame() {

	}

	private void paintGame(){
//		Toast.makeText(baiduTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.i(TAG, "paintGame");
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("platForm", "android");
			jsonObj.put("sdk", "baidu");
			jsonObj.put("uid", m_uid);
			jsonObj.put("sdkPayCallName","verifyBaiduPay");
			jsonObj.put("openId", m_session);
			jsonObj.put("iLoginId",10);//baidu
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


	private void getOrderStatus(String payId){
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

    }



	public void doMIpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");
		int iFee = Integer.parseInt(totalFee)/100;

		String cpOrderId =outTradeNo; // CP订单号
		String goodsName = subject;
		String totalAmount = totalFee; // 支付总金额 （以分为单位）
		int ratio = 1; // 该参数为非定额支付时生效 (支付金额为0时为非定额支付,具体参见使用手册)
		String extInfo = outTradeNo; // 扩展字段，该信息在支付成功后原样返回给CP


		PayOrderInfo payOrderInfo = new PayOrderInfo();
		payOrderInfo.setCooperatorOrderSerial(cpOrderId);
		payOrderInfo.setProductName(goodsName);
		long p = Long.parseLong(totalAmount);
		payOrderInfo.setTotalPriceCent(p); // 以分为单位
		payOrderInfo.setRatio(ratio);
		payOrderInfo.setExtInfo(extInfo); // 该字段将会在支付成功后原样返回给CP(不超过500个字符)
		payOrderInfo.setCpUid(m_uid); // 必传字段，需要验证uid是否合法,此字段必须是登陆后或者切换账号后保存的uid
		if (payOrderInfo == null) {
			return;
		}

		BDGameSDK.pay(payOrderInfo, null, new IResponse<PayOrderInfo>() {

			@Override
			public void onResponse(int resultCode, String resultDesc,
								   PayOrderInfo extraData) {

				String resultStr = "";
				switch (resultCode) {
					case ResultCode.PAY_SUCCESS: // 支付成功
						resultStr = "支付成功:" + resultDesc;
						payResult(extraData.getExtInfo(),true);
						break;
					case ResultCode.PAY_CANCEL: // 订单支付取消
						resultStr = "取消支付";
						payResult(extraData.getExtInfo(),false);
						break;
					case ResultCode.PAY_FAIL: // 订单支付失败
						resultStr = "支付失败：" + resultDesc;
						payResult(extraData.getExtInfo(),false);
						break;
					case ResultCode.PAY_SUBMIT_ORDER: // 订单已经提交，支付结果未知（比如：已经请求了，但是查询超时）
						resultStr = "订单已经提交，支付结果未知";
						getOrderStatus(extraData.getExtInfo());
						break;
					default:
						resultStr = "订单已经提交，支付结果未知";
						getOrderStatus(extraData.getExtInfo());
						break;
				}
				Log.d(TAG,resultStr);

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

	}
	private void doSDKLogin(){


		BDGameSDK.login(new IResponse<Void>() {

			@Override
			public void onResponse(int resultCode, String resultDesc, Void extraData) {
				Log.d("login", "this resultCode is " + resultCode);
				String hint = "";
				switch (resultCode) {
					case ResultCode.LOGIN_SUCCESS:
						hint = "登录成功";
						m_uid = BDGameSDK.getLoginUid(); // TODO 保存登陆后获取的uid到调用支付API时使用



						m_session = BDGameSDK.getLoginAccessToken();

						getUserInfo();

						break;
					case ResultCode.LOGIN_CANCEL:

						hint = "取消登录";
						break;
					case ResultCode.LOGIN_FAIL:
					default:

						hint = "登录失败:" + resultDesc;
				}
//				Toast.makeText(getApplicationContext(), hint, Toast.LENGTH_LONG).show();
				Log.d(TAG,hint);
			}
		});


	}

	private void getUserInfo(){
		if (!s_showFloatView){
			BDGameSDK.showFloatView(this); // 显示悬浮窗
			s_showFloatView = true;
		}

		handler.post(new Runnable() {

			@Override
			public void run() {
				paintGame();
			}
		});
	}
	private void doSDKQuit(){
		BDGameSDK.gameExit(this, new OnGameExitListener() {

			@Override
			public void onGameExit() {

				// TODO 在此处执行您的游戏退出逻辑
				finish();
			}

		});
	}

	@Override
	protected void onResume() {
		Log.d(TAG, "onResume");
		super.onResume();
		BDGameSDK.onResume(this);
		SDKExitCancel();
	}
	@Override
	protected void onPause() {
		super.onPause();
		BDGameSDK.onPause(this);
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
