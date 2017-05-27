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

import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;
import org.xmlpull.v1.XmlPullParser;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.util.Xml;
import android.view.WindowManager;
import android.widget.Toast;



import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.tendcloud.tenddata.TalkingDataGA;
import com.bantu.R;

public class TRGame extends Cocos2dxActivity {

    private static TRGame instance;
    protected Handler handler;
	private FunctionConst funConst;

	// 微信支付
//	private static final String TAG = "MicroMsg.SDKSample.PayActivity";
//	PayReq req;
//	final IWXAPI msgApi = WXAPIFactory.createWXAPI(this, null);
//	Map<String,String> resultunifiedorder;
//	StringBuffer sb;
//	public static String WXAPPID = "";
//	public static final String WXAPI_KEY = "bantu0742d39c26771545e95a8d0b4f6";
//	private static String wx_body;
//	private static String wx_totalFee;
//	private static String wx_tradeNo;
//	private static String wx_outTradeNo;
//	private static String wx_nonceStr;
	private static String wx_attach;	
	private static boolean wx_show = false;

	//QQ登陆
//	Tencent mTencent;
//	IUiListener QQlistener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		funConst = FunctionConst.getInstance(this);

        instance = this;

        handler = new Handler();

//        // 微信支付
//		req = new PayReq();
//		sb=new StringBuffer();
//		WXAPPID = funConst.WXAPPID;
//		msgApi.registerApp(WXAPPID);

        nativeInit(this);

        //QQ登陆SDK
        // Tencent类是SDK的主要实现类，开发者可通过Tencent类访问腾讯开放的OpenAPI。
        // 其中APP_ID是分配给第三方应用的appid，类型为String。
//        mTencent = Tencent.createInstance(funConst.QQAPPID, this.getApplicationContext());
        // 1.4版本:此处需新增参数，传入应用程序的全局context，可通过activity的getApplicationContext方法获取      
    }

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // TRGame should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    static {
        System.loadLibrary("MyGame");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d("TRGAMECocos2dx", "onResume");
        TalkingDataGA.onResume(this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	super.onActivityResult(requestCode, resultCode, data);
//    	Tencent.onActivityResultData(requestCode, resultCode, data, QQlistener);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d("TRGAMECocos2dx", "onPause");
        TalkingDataGA.onPause(this);
    }

    @Override
    protected void onDestroy() {
        mHandler.removeCallbacksAndMessages(null);
        instance = null;
        Log.e("123", "==========================TRGame onDestroy ===========");
        super.onDestroy();
    }

    @SuppressLint("NewApi")
    public static String getDeviceParams(int type) {
        switch (type) {
            case 0: {
                StringBuilder sb = new StringBuilder();
                if (instance != null) {
                    sb.append("platform=android").append("&package_name=")
                            .append(instance.getPackageName()).append("&system_version=")
                            .append(Build.VERSION.RELEASE);
                    try {
                        sb.append("&device_model=").append(URLEncoder.encode(Build.MODEL, "UTF-8"))
                                .append("&manufacturer=").append(Build.MANUFACTURER);
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD) {
                            sb.append("&serial=").append(Build.SERIAL);
                        }
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                }
                String result = sb.toString();
                Log.d("getDeviceParams", "result=" + result);
                return result;
            }
            case 1:
                return "android";
            case 2:
                if (instance != null) {
                    return instance.getPackageName();
                }
                return "";
            case 3:
                if (instance != null) {
                    String result = "1.0";
                    try {
                        PackageInfo info =
                                instance.getPackageManager().getPackageInfo(
                                        instance.getPackageName(), 0);
                        result = info.versionName;
                    } catch (NameNotFoundException e) {
                        e.printStackTrace();
                    }
                    return result;
                }
                return "1.0";
            case 4:
            	if (instance != null) {
            		return FunctionConst.getInstance(instance).MMChannelId;
            	}
            	return "";
            case 5:
            	if (instance != null) {
            		PackageManager pm = instance.getPackageManager();
    		        boolean feature = pm.hasSystemFeature(PackageManager.FEATURE_TELEPHONY);
            		if (feature) {
            			return "1";
            		}
            	}
            	return "0";
            default:
            	return "";
        }
    }

    public static int startAlipay(String trGameId, String outTradeNo, String subject, String body,
            String totalFee) {
        if (instance == null) {
            return 0;
        }

        Log.d("TRGame", String.format("Alipay,trGameId=%s,tradeNo=%s,subject=%s,body=%s,fee=%s",
                trGameId, outTradeNo, subject, body, totalFee));

//        AliPayAPI.startAlipay(instance, instance.mHandler, trGameId, outTradeNo, subject, body,
//                totalFee);
        
        return 1;
    }
    
    // 微信支付
    public static int startwxpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) {
        if (instance == null) {
            return 0;
        }     
        Log.d("TRGame", String.format("wxpay,trGameId=%s,tradeNo=%s,subject=%s,body=%s,fee=%s", trGameId, outTradeNo, subject, body, totalFee));
//        TRGame.wx_body = subject;
//		TRGame.wx_totalFee = totalFee;
//		TRGame.wx_tradeNo = outTradeNo;
//		TRGame.wx_attach = body;
		instance.runOnUiThread(new Runnable(){
			@Override
			public void run()
			{
//				if(instance.msgApi.isWXAppInstalled() && instance.msgApi.isWXAppSupportAPI())
//				{
//					GetPrepayIdTask getPrepayId = instance.new GetPrepayIdTask();
//					getPrepayId.execute();
//				}
//				else
//				{
					Toast.makeText(instance, "微信支付不可用，请选择其他支付方式", Toast.LENGTH_LONG).show();
//				}
			}
		});
		
        return 1;
    }

	// xxTRGame重写实现
	public void doSDKpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) {
//		runOnUiThread(new Runnable(){
//			@Override
//			public void run()
//			{
//				...
//			}
//		});
	}

	public void SDKExitCancel(){
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("name","SDKExitCancel");
		} catch (JSONException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
		final String jsonStr = jsonObj.toString();
		runOnGLThread(new Runnable() {
			@Override
			public void run() {
				TRGame.nativeJavaCallCppBridge(jsonStr);
			}
		});
	}

	public void SDKChangeAccount(){
		JSONObject jsonObj = new JSONObject();
		try {
			jsonObj.put("name","SDKChangeAccount");
		} catch (JSONException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
		final String jsonStr = jsonObj.toString();
		runOnGLThread(new Runnable() {
			@Override
			public void run() {
				TRGame.nativeJavaCallCppBridge(jsonStr);
			}
		});
	}

	//xxTRGame override me!
	public void annousCall(JSONObject jsonObj){

	}


	public void doCppBridgeCall(String strJson){
		Log.d("TRGame", "doCppBridgeCall");
		JSONObject jsonObj = null;
		try {
			jsonObj = new JSONObject(strJson);
		}catch (Exception e){
			Log.d("TRGame", e.getLocalizedMessage());
			e.printStackTrace();
			return;
		}
		final JSONObject fJsonObj = jsonObj;
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				String name = null;
				try {
					name = fJsonObj.getString("name");
				} catch (JSONException e) {
					e.printStackTrace();
				}

				if (0 == name.compareTo("keepScreenOn")){//屏幕是否常亮
					try {
						if (fJsonObj.getBoolean("keep")){
							getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
						}else{
							getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
						}
					} catch (JSONException e) {
						e.printStackTrace();
					}
					return;
				}
				annousCall(fJsonObj);
			}
		});


	}

	// sdk支付
	public static int startSDKpay(String trGameId, String outTradeNo, String subject, String body, String totalFee) {
		if (instance == null) {
			return 0;
		}
		//Log.d("TRGame", String.format("sdkpay,trGameId=%s,tradeNo=%s,subject=%s,body=%s,fee=%s", trGameId, outTradeNo, subject, body, totalFee));
		instance.doSDKpay(trGameId, outTradeNo, subject, body, totalFee);
		return 1;
	}

	public static void cppBridgeCall(String strJson){
		Log.d("TRGame", String.format("cppBridgeCall %s",strJson));
		if (instance == null) {
			return;
		}
		//Log.d("TRGame", String.format("cppBridgeCall %s",strJson));
		instance.doCppBridgeCall(strJson);
	}

    public static void onWXPayResult(BaseResp resp){
    	if(instance != null){
//			switch(resp.errCode){
//			case 0:
//				//Toast.makeText(instance, "支付成功", Toast.LENGTH_LONG).show();
//				nativewxpayResult(TRGame.wx_tradeNo, 1, "Succeed");
//				nativeWXHttpRequest(instance.funConst.WXAPPID, instance.funConst.WXMCHID, TRGame.wx_outTradeNo, TRGame.wx_nonceStr);
//				break;
//			case -1:
//				Toast.makeText(instance, "微信支付不可用，请选择其他支付方式", Toast.LENGTH_LONG).show();
//				nativewxpayResult(TRGame.wx_tradeNo, 0, "Failed");
//				break;
//			case -2:
//				//Toast.makeText(instance, "支付已取消", Toast.LENGTH_LONG).show();
//				nativewxpayResult(TRGame.wx_tradeNo, 0, "Cancel");
//				break;
//			}
    	}
    	wx_show = false;
    }
    
    //微信登陆
    public static void sendWXAuthRequest()
    {  
//		if(instance.msgApi.isWXAppInstalled() && instance.msgApi.isWXAppSupportAPI())
//		{
//	        final SendAuth.Req req = new SendAuth.Req();
//	        req.scope = "snsapi_userinfo";
//	        req.state = "wechat_sdk_demo_test";
//	        instance.msgApi.sendReq(req);
//	        return;
//		}
//
//		instance.runOnUiThread(new Runnable(){
//			@Override
//			public void run()
//			{
//				Toast.makeText(instance, "微信不可用，请选择其他登陆方式", Toast.LENGTH_LONG).show();
//			}
//		});
    }
    
    public static void GetWXAccessToken(String code)
    {
        Log.d("LOGIN", String.format("WXLOGIN,appid=%s, secret=%s, code=%s", instance.funConst.WXAPPID, instance.funConst.WXSecret,code));
        nativeGetWXAccessToken(instance.funConst.WXAPPID, instance.funConst.WXSecret, code);
    }
    
    //QQ登陆
    public static void sendQQAuthRequest()
    {
//    	instance.QQlistener = new BaseUiListener();
//     	instance.mTencent.login(instance, "get_user_info", instance.QQlistener);
    }
       
    public static void onHttpReqQQLogin(String openid, String token)
    {
        Log.d("LOGIN", String.format("QQLOGIN,openid=%s, token=%s", openid, token));
        nativeOnHttpReqQQLogin(openid, token);
    }
    public static void WXOrQQLoginFailed()
    {
        Log.d("nativeWXOrQQLoginFailed","");
        nativeWXOrQQLoginFailed();
    }

    @SuppressLint("HandlerLeak")
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(android.os.Message msg) {
//            String outTradeNo = msg.getData().getString(AliPayAPI.MSG_KEY_OUT_TRADE_NO);
//            String strResult = msg.getData().getString(AliPayAPI.MSG_KEY_RESULT);
//            AliPayResult result = new AliPayResult(strResult);
//
//            switch (msg.what) {
//                case AliPayAPI.RQF_PAY: {
//                    if (result.isSucceed()) {
//                        nativeAlipayResult(outTradeNo, 1, "succeed");
//                    } else {
//                        nativeAlipayResult(outTradeNo, 0, result.getResultStatusMsg());
//                    }
//                }
//                    break;
//                default:
//                    break;
//            }
        };
    };
    
    // 微信支付
	private String genPackageSign(List<NameValuePair> params) {
		StringBuilder sb = new StringBuilder();
		
		for (int i = 0; i < params.size(); i++) {
			sb.append(params.get(i).getName());
			sb.append('=');
			sb.append(params.get(i).getValue());
			sb.append('&');
		}
		sb.append("key=");
//		sb.append(WXAPI_KEY);
		

//		String packageSign = MD5Util.MD5Encode(sb.toString(), "UTF-8").toUpperCase();
//		Log.e("orion",packageSign);
//		return packageSign;
		return null;
	}
	private String genAppSign(List<NameValuePair> params) {
		StringBuilder sb = new StringBuilder();

		for (int i = 0; i < params.size(); i++) {
			sb.append(params.get(i).getName());
			sb.append('=');
			sb.append(params.get(i).getValue());
			sb.append('&');
		}
		sb.append("key=");
//		sb.append(WXAPI_KEY);

//       this.sb.append("sign str\n"+sb.toString()+"\n\n");
//		String appSign = MD5Util.MD5Encode(sb.toString(), "UTF-8").toUpperCase();
//		Log.e("orion",appSign);
//		return appSign;
		return null;
	}
	private String toXml(List<NameValuePair> params) {
		StringBuilder sb = new StringBuilder();
		sb.append("<xml>");
		for (int i = 0; i < params.size(); i++) {
			sb.append("<"+params.get(i).getName()+">");


			sb.append(params.get(i).getValue());
			sb.append("</"+params.get(i).getName()+">");
		}
		sb.append("</xml>");

		Log.e("orion",sb.toString());
		return sb.toString();
	}

	private class GetPrepayIdTask extends AsyncTask<Void, Void, Map<String,String>> {

		private ProgressDialog dialog;


		@Override
		protected void onPreExecute() {
			dialog = ProgressDialog.show(instance, getString(R.string.app_tip), getString(R.string.getting_prepayid));
		}

		@Override
		protected void onPostExecute(Map<String,String> result) {
			if (dialog != null) {
				dialog.dismiss();
			}
			
			if(result == null)
			{
				Toast.makeText(instance, "微信支付不可用，请检查网络是否异常", Toast.LENGTH_LONG).show();
				return;
			}
			
//			sb.append("prepay_id\n"+result.get("prepay_id")+"\n\n");
//			resultunifiedorder=result;

			wx_show = true;
			genPayReq();
			sendPayReq();
		}

		@Override
		protected void onCancelled() {
			super.onCancelled();
		}

		@Override
		protected Map<String,String>  doInBackground(Void... params) {

			String url = String.format("https://api.mch.weixin.qq.com/pay/unifiedorder");
			String entity = genProductArgs();

			Log.e("orion",entity);

//			byte[] buf = Util.httpPost(url, entity);
//
//			if(buf != null)
//			{
//				String content = new String(buf);
//				Log.e("orion", content);
//				Map<String,String> xml=decodeXml(content);
//				return xml;
//			}
//			else
//			{
//				return null;
//			}
			return null;
		}
	}

	public Map<String,String> decodeXml(String content) {

		try {
			Map<String, String> xml = new HashMap<String, String>();
			XmlPullParser parser = Xml.newPullParser();
			parser.setInput(new StringReader(content));
			int event = parser.getEventType();
			while (event != XmlPullParser.END_DOCUMENT) {

				String nodeName=parser.getName();
				switch (event) {
					case XmlPullParser.START_DOCUMENT:

						break;
					case XmlPullParser.START_TAG:

						if("xml".equals(nodeName)==false){
							//实例化student对象
							xml.put(nodeName,parser.nextText());
						}
						break;
					case XmlPullParser.END_TAG:
						break;
				}
				event = parser.next();
			}

			return xml;
		} catch (Exception e) {
			Log.e("orion",e.toString());
		}
		return null;

	}

	private String genNonceStr() {
		Random random = new Random();
//		return MD5Util.MD5Encode(String.valueOf(random.nextInt(10000)),"UTF-8");
		return null;
	}
	
	private long genTimeStamp() {
		return System.currentTimeMillis() / 1000;
	}
	
	private String genOutTradNo() {
		Random random = new Random();
//		return MD5Util.MD5Encode(String.valueOf(random.nextInt(10000)),"UTF-8");
		return null;
	}

	private String genProductArgs() {
		StringBuffer xml = new StringBuffer();
		
//		wx_outTradeNo = wx_tradeNo;
//		wx_nonceStr = genNonceStr();
//
//		try {
//			xml.append("</xml>");
//            List<NameValuePair> packageParams = new LinkedList<NameValuePair>();
//			packageParams.add(new BasicNameValuePair("appid", funConst.WXAPPID));
//			packageParams.add(new BasicNameValuePair("body", wx_body));
//			packageParams.add(new BasicNameValuePair("mch_id", funConst.WXMCHID));
//			packageParams.add(new BasicNameValuePair("nonce_str", wx_nonceStr));
//			packageParams.add(new BasicNameValuePair("notify_url", nativeGetNotifyUrl()));
//			packageParams.add(new BasicNameValuePair("out_trade_no",wx_outTradeNo));
//			packageParams.add(new BasicNameValuePair("spbill_create_ip","127.0.0.1"));
//			packageParams.add(new BasicNameValuePair("total_fee", wx_totalFee));
//			packageParams.add(new BasicNameValuePair("trade_type", "APP"));
//
//			String sign = genPackageSign(packageParams);
//			packageParams.add(new BasicNameValuePair("sign", sign));
//
//		   String xmlstring =toXml(packageParams);
//
//		   return new String(xmlstring.toString().getBytes(), "ISO8859-1");
//
//		} catch (Exception e) {
//			Log.e(TAG, "genProductArgs fail, ex = " + e.getMessage());
//			return null;
//		}
		return null;

	}
	private void genPayReq() {
//		req.appId = funConst.WXAPPID;
//		req.partnerId = funConst.WXMCHID;
//		req.prepayId = resultunifiedorder.get("prepay_id");
//		req.packageValue = "Sign=WXPay";
//		req.nonceStr = genNonceStr();
//		req.timeStamp = String.valueOf(genTimeStamp());
//
//		List<NameValuePair> signParams = new LinkedList<NameValuePair>();
//		signParams.add(new BasicNameValuePair("appid", req.appId));
//		signParams.add(new BasicNameValuePair("noncestr", req.nonceStr));
//		signParams.add(new BasicNameValuePair("package", req.packageValue));
//		signParams.add(new BasicNameValuePair("partnerid", req.partnerId));
//		signParams.add(new BasicNameValuePair("prepayid", req.prepayId));
//		signParams.add(new BasicNameValuePair("timestamp", req.timeStamp));
//
//		req.sign = genAppSign(signParams);
//		sb.append("sign\n"+req.sign+"\n\n");
//		Log.e("orion", signParams.toString());
	}
	private void sendPayReq() {
//		msgApi.registerApp(funConst.WXAPPID);
//		msgApi.sendReq(req);
	}
    
    public static native void nativeWXHttpRequest(String appid, String mch_id, String out_trade_no, String nonce_str);
    
    public static native void nativeAlipayResult(String tradeNo, int resultCode, String resultMessage);
    
    public static native void nativewxpayResult(String tradeNo, int resultCode, String resultMessage);

	public static native void nativeInit(Object context);

    public static native void nativeGetWXAccessToken(String appid, String secret, String code);
    public static native void nativeOnHttpReqQQLogin(String openid, String token);  
    public static native void nativeWXOrQQLoginFailed();
	public static native String nativeGetNotifyUrl();
	public static native void  nativeSetPlatformLoginInfo(String strJson);//平台登录成功后的用户数据
	public static native void nativeSDKPayResult(String strJson);//平台支付后的数据
	public static native void nativeJavaCallCppBridge(String strJson);//java call cpp
	public static native int nativeGetIntValue(String name);
	public static native String nativeGetStrValue(String name);

}
