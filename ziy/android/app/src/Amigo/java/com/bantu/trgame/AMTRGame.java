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


import org.json.JSONException;
import org.json.JSONObject;


import android.os.Bundle;
import android.util.Log;

import android.content.Intent;
import android.widget.Toast;

import com.gionee.gamesdk.GamePlatform;
import com.gionee.gamesdk.AccountInfo;
import com.gionee.gamesdk.GamePayer;
import com.gionee.gamesdk.GamePlatform.LoginListener;
import com.gionee.gamesdk.OrderInfo;
import com.gionee.gamesdk.QuitGameCallback;


public class AMTRGame extends TRGame {
	private static final String TAG = "AMTRGame";
	//am
	private String m_outOrderNo;
	public boolean mRepeatCreate = false;
    private GamePayer.GamePayCallback mGamePayCallback = null;
    // 开始支付
    private GamePayer mGamePayer = null;

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
    protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate");
        super.onCreate(savedInstanceState);
		if ((getIntent().getFlags() & Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT) != 0) {
			Log.i(TAG, "onCreate with flag FLAG_ACTIVITY_BROUGHT_TO_FRONT");
			mRepeatCreate = true;
			finish();
			return;
		}
		GamePlatform.init(this, Constants.API_KEY);

        mGamePayer = new GamePayer(this);
        mGamePayCallback = mGamePayer.new GamePayCallback() {

            //支付成功
            @Override
            public void onPaySuccess() {
                // 可以在这里处理自己的业务
				payResult(m_outOrderNo,true);
            }

            //支付取消
            @Override
            public void onPayCancel() {
                // 可以在这里处理自己的业务
				payResult(m_outOrderNo,false);
            }

            //支付失败，stateCode为支付失败状态码，详见接入指南
            @Override
            public void onPayFail(String stateCode) {
                // 可以在这里处理自己的业务
				payResult(m_outOrderNo,false);
            }
        };
		// 登录Amigo账号
		loginAccount();
    }



	private void startGame() {
		Log.i(TAG, "startGame");
	}

	private void paintGame(){
//		Toast.makeText(UCTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.i(TAG, "paintGame");

	}
	@Override
	protected void onResume() {
		super.onResume();
		// 在不能静默安装的手机，升级成功后自动调起收银台
		mGamePayer.onResume();
	}

	/**
	 * 如果登录过的，会自动登录
	 */
	private void loginAccount() {
		Log.i(TAG, "loginAccount");
		GamePlatform.loginAccount(this, true, new LoginListener() {

			@Override
			public void onSuccess(AccountInfo accountInfo) {
				// 登录成功，处理自己的业务。

				// 获取playerId
				String playerId = accountInfo.mPlayerId;

				// 获取amigoToken
				String amigoToken = accountInfo.mToken;

				//获取用户ID
				//      String userId = accountInfo.mUserId;

				Log.i(TAG, "登录成功！获取信息为：" + accountInfo.toString());
				JSONObject jsonObj = new JSONObject();
				try {
					jsonObj.put("platForm", "android");
					jsonObj.put("sdk", "am");
					jsonObj.put("playerId", "playerId");//am唯一用户id
					jsonObj.put("openId", amigoToken);
					jsonObj.put("sdkPayCallName","verifyAMPay");
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

			@Override
			public void onError(Exception e) {
				Toast.makeText(AMTRGame.this, "登录失败:" + e, Toast.LENGTH_SHORT).show();
			}

			@Override
			public void onCancel() {
				Toast.makeText(AMTRGame.this, "取消登录", Toast.LENGTH_SHORT).show();
			}
		});
	}

	@Override
	public void onStop(){
		super.onStop();
		Log.i(TAG, "onStop======================================================================:");

	}

	@Override
	public void onDestroy(){
		super.onDestroy();
		if (this.mRepeatCreate) {
			Log.i(TAG, "onDestroy is repeat activity!");
			return;
		}
		mGamePayer.onDestroy();
		Log.i(TAG, "onDestroy===================================================================:");

	}

	@Override
	public void onBackPressed() {
		GamePlatform.quitGame(this, new QuitGameCallback() {
			@Override
			public void onQuit() {
				Toast.makeText(AMTRGame.this, "结束游戏", Toast.LENGTH_SHORT).show();
				finish();
			}

			@Override
			public void onCancel() {
				Toast.makeText(AMTRGame.this, "取消退出", Toast.LENGTH_SHORT).show();
			}
		});
	}


    public void doAMpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");
        //创建订单信息
        OrderInfo orderInfo = new OrderInfo();
        //开发者后台申请的Apikey
        orderInfo.setApiKey(Constants.API_KEY);
        //商户订单号，与创建支付订单中的"out_order_no"值相同
        orderInfo.setOutOrderNo(outTradeNo);
		m_outOrderNo = outTradeNo;
        //支付订单提交时间，与创建支付订单中的"submit_time"值相同
        orderInfo.setSubmitTime(bodyAry[21]);


        mGamePayer.pay(orderInfo,mGamePayCallback);
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
                    doAMpay(ftrGameId, foutTradeNo, fsubject, fbody, ftotalFee);
                } catch (Exception e) {
                    e.printStackTrace();
                }


            }
		});
	}

	@Override
	public void annousCall(JSONObject jsonObj){
		String name = null;
		try {
			name = jsonObj.getString("name");
		} catch (JSONException e) {
			e.printStackTrace();
		}
		if (0 == name.compareTo("startSDKLogin")){
			loginAccount();
			return;
		}
		if (0 == name.compareTo("startSDKLogout")){
//			Toast.makeText(AMTRGame.this, "暂不支持", Toast.LENGTH_SHORT).show();
			return;
		}
	}
}
