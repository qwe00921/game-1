package com.bantu.trgame;

import com.excelliance.lbsdk.IQueryUpdateCallback;
import com.excelliance.lbsdk.LebianSdk;
import com.tendcloud.tenddata.TalkingDataGA;

import android.app.Application;
import android.util.Log;

public class TRApplication extends Application {
	
	@Override
	public void onCreate() {
		super.onCreate();
		TalkingDataGA.sPlatformType = TalkingDataGA.PLATFORM_TYPE_COCOS2DX;
		FunctionConst funConst = FunctionConst.getInstance(this);
		if (funConst.EnableTalkingData) {
			Log.d("funConst", String.format("TRApplication.onCreate.appId=%s,channelId=%s", funConst.TalkingDataAppId, funConst.TalkingDataChannelId));
			TalkingDataGA.init(this, funConst.TalkingDataAppId, funConst.TalkingDataChannelId);
		}

		//乐变更新
		final IQueryUpdateCallback callBack = new IQueryUpdateCallback() {
			public void onUpdateResult(int result) {
				Log.d("MainActivity", "onResume result=" + result);
			}
		};
		LebianSdk.queryUpdate(this, callBack, null);
		
	}
	
	@Override
	public void onTerminate() {
		super.onTerminate();
	}
}
