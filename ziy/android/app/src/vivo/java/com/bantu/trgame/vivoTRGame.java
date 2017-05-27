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

import com.vivo.unionsdk.open.VivoAccountCallback;
import com.vivo.unionsdk.open.VivoExitCallback;
import com.vivo.unionsdk.open.VivoPayCallback;
import com.vivo.unionsdk.open.VivoPayInfo;
import com.vivo.unionsdk.open.VivoRoleInfo;
import com.vivo.unionsdk.open.VivoUnionSDK;


public class vivoTRGame extends TRGame implements VivoAccountCallback,VivoPayCallback{
	private static final String TAG = "vivoTRGame";
	String mVivoOpenId;
	String m_session;
	String mUserName;
	String mVivoAppId = "bb93a17b25b56123ce02b7cf37582b79";
	String mOutTradeNo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		VivoUnionSDK.registerAccountCallback(this,this);

		doSDKLogin();
    }




	private void startGame() {

	}

	private void paintGame(){
//		Toast.makeText(miTRGame.this, "paintGame", Toast.LENGTH_SHORT).show();
		Log.i(TAG, "paintGame");
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("platForm", "android");
			jsonObj.put("sdk", "vivo");
			jsonObj.put("userName", mUserName);
			jsonObj.put("vivoOpenId", mVivoOpenId);
			jsonObj.put("sdkPayCallName","verifyVivoPay");
			jsonObj.put("openId", m_session);
			jsonObj.put("iLoginId",9);//vivo
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
		super.onStart();

	}
    @Override
    protected void onDestroy() {
		super.onDestroy();

	}

	public void doMIpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) throws Exception {
		String[] bodyAry = body.split(",");
		VivoPayInfo vivoPayInfo = new VivoPayInfo(subject,subject,totalFee,bodyAry[15],mVivoAppId,bodyAry[19],bodyAry[17]);
		VivoUnionSDK.pay(this,vivoPayInfo,this);
	}

	@Override
	public void doSDKpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) {
		mOutTradeNo=outTradeNo;
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
		try {
			String flag = jsonObj.getString("flag");
			long createdAt = jsonObj.getLong("createdAt");
			long level = jsonObj.getLong("level");
			if (createdAt <= 0 || level <= 0){
				return;
			}

			VivoUnionSDK.reportRoleInfo(new VivoRoleInfo(jsonObj.getString("roleId"),String.valueOf(level),jsonObj.getString("roleName"),"1","1"));
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	private void doSDKLogin(){
		VivoUnionSDK.login(this);
	}
	private void doSDKQuit(){
		VivoUnionSDK.exit(this, new VivoExitCallback() {
			@Override
			public void onExitCancel() {
				SDKExitCancel();
			}

			@Override
			public void onExitConfirm() {
				finish();
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


	@Override
	public void onVivoAccountLogin(String userName, String openId, String authToken) {
		mUserName = userName;
		m_session = authToken;
		mVivoOpenId = openId;
		getUserInfo();
		Log.d(TAG,"onVivoAccountLogin");

	}

	@Override
	public void onVivoAccountLogout(int requestCode) {
		Log.d(TAG,"onVivoAccountLogout");
		doSDKLogin();
	}

	@Override
	public void onVivoAccountLoginCancel() {
		Log.d(TAG,"onVivoAccountLoginCancel");
	}

	@Override
	public void onVivoPayResult(String s, boolean b, String s1) {
		Log.d(TAG,"onVivoPayResult");
		Log.d(TAG,s);
		Log.d(TAG,String.valueOf(b));
		Log.d(TAG,s1);
		payResult(mOutTradeNo,b);
	}
}
