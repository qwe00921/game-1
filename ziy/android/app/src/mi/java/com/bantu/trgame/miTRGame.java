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

import com.xiaomi.gamecenter.sdk.GameInfoField;
import com.xiaomi.gamecenter.sdk.MiCommplatform;
import com.xiaomi.gamecenter.sdk.MiErrorCode;
import com.xiaomi.gamecenter.sdk.OnExitListner;
import com.xiaomi.gamecenter.sdk.OnLoginProcessListener;
import com.xiaomi.gamecenter.sdk.OnPayProcessListener;
import com.xiaomi.gamecenter.sdk.entry.MiAccountInfo;
import com.xiaomi.gamecenter.sdk.entry.MiAppInfo;
import com.xiaomi.gamecenter.sdk.entry.MiBuyInfo;

public class miTRGame extends TRGame implements OnPayProcessListener {
	private static final String TAG = "miTRGame";
	String m_uid;
	String m_session;
	String mOrderId;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		MiAppInfo appInfo = new MiAppInfo();
		appInfo.setAppId("2882303761517573650");
		appInfo.setAppKey("5611757369650");
		MiCommplatform.Init( this, appInfo );
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
			jsonObj.put("sdk", "mi");
			jsonObj.put("uid", m_uid);
			jsonObj.put("sdkPayCallName","verifyMIPay");
			jsonObj.put("openId", m_session);
			jsonObj.put("iLoginId",8);//mi
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
		int iFee = Integer.parseInt(totalFee)/100;

		MiBuyInfo miBuyInfo= new MiBuyInfo();
		miBuyInfo.setCpOrderId(outTradeNo);//订单号唯一（不为空）
		miBuyInfo.setCpUserInfo(outTradeNo); //此参数在用户支付成功后会透传给CP的服务器
		miBuyInfo.setAmount(iFee); //必须是大于1的整数，10代表10米币，即10元人民币（不为空）
//		miBuyInfo.setCount(2);
		mOrderId = outTradeNo;
//用户信息，网游必须设置、单机游戏或应用可选

		JSONObject json;
		try {
			String data = TRGame.nativeGetStrValue("getDataForSDKAnalys");
			json = new JSONObject(data);

			Bundle mBundle = new Bundle();
			mBundle.putString( GameInfoField.GAME_USER_BALANCE, String.valueOf(TRGame.nativeGetIntValue("jade")) );   //用户余额
			mBundle.putString( GameInfoField.GAME_USER_GAMER_VIP, "vip"+json.getInt("vip") );  //vip等级
			mBundle.putString( GameInfoField.GAME_USER_LV, String.valueOf(json.getInt("level")) );           //角色等级
			mBundle.putString( GameInfoField.GAME_USER_PARTY_NAME, TRGame.nativeGetStrValue("patriarch"));  //工会，帮派
			mBundle.putString( GameInfoField.GAME_USER_ROLE_NAME, json.getString("roleName") ); //角色名称
			mBundle.putString( GameInfoField.GAME_USER_ROLEID, String.valueOf(json.getInt("roleId")) );    //角色id
			mBundle.putString( GameInfoField.GAME_USER_SERVER_NAME, "1" );  //所在服务器
			miBuyInfo.setExtraInfo( mBundle ); //设置用户信息
		} catch (Exception e) {
			e.printStackTrace();
		}




		MiCommplatform.getInstance().miUniPay(this, miBuyInfo, this);
	}

	@Override
	public void finishPayProcess( int code )
	{
		switch( code ) {
			case MiErrorCode.MI_XIAOMI_PAYMENT_SUCCESS:
				//购买成功
				payResult(mOrderId,true);
				break;
			case MiErrorCode.MI_XIAOMI_PAYMENT_ERROR_PAY_CANCEL:
				//取消购买
				payResult(mOrderId,false);
				break;
			case MiErrorCode.MI_XIAOMI_PAYMENT_ERROR_PAY_FAILURE:
				//购买失败
				payResult(mOrderId,false);
				break;
			case  MiErrorCode.MI_XIAOMI_PAYMENT_ERROR_ACTION_EXECUTED:
				//操作正在进行中
				break;
			default:
				//购买失败
				payResult(mOrderId,false);
				break;
		}
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
		MiCommplatform.getInstance().miLogin( this,
				new OnLoginProcessListener()
				{
					@Override
					public void finishLoginProcess( int code ,MiAccountInfo arg1)
					{
						switch( code )
						{
							case MiErrorCode.MI_XIAOMI_PAYMENT_SUCCESS:
								// 登陆成功
								//获取用户的登陆后的UID（即用户唯一标识）
								m_uid = String.valueOf(arg1.getUid());

								/**以下为获取session并校验流程，如果是网络游戏必须校验，如果是单机游戏或应用可选**/
								//获取用户的登陆的Session（请参考5.3.3流程校验Session有效性）
								m_session = arg1.getSessionId();
								//请开发者完成将uid和session提交给开发者自己服务器进行session验证
								getUserInfo();
								break;
							case MiErrorCode.MI_XIAOMI_PAYMENT_ERROR_LOGIN_FAIL:
								// 登陆失败
								break;
							case MiErrorCode.MI_XIAOMI_PAYMENT_ERROR_CANCEL:
								// 取消登录
								break;
							case MiErrorCode.MI_XIAOMI_PAYMENT_ERROR_ACTION_EXECUTED:
								//登录操作正在进行中
								break;
							default:
								// 登录失败
								break;
						}
					}
				} );
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
		MiCommplatform.getInstance().miAppExit( miTRGame.this, new OnExitListner()
		{
			@Override
			public void onExit( int code )
			{
				if ( code == MiErrorCode.MI_XIAOMI_EXIT )
				{
//					android.os.Process.killProcess( android.os.Process.myPid() );
					finish();
				}else{
					SDKExitCancel();
				}
			}
		} );
	}

	@Override
	protected void onResume() {
		Log.d(TAG, "onResume");
		super.onResume();
		SDKExitCancel();
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
