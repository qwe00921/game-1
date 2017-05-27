package com.bantu.trgame;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;
import com.baidu.gamesdk.BDGameSDK;
import com.baidu.gamesdk.BDGameSDKSetting;
import com.baidu.gamesdk.IResponse;
import com.baidu.gamesdk.ResultCode;
import com.bantu.R;

public class baiduWelcomeActivity extends Activity {

	private Handler handler = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {


        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.welcome_layout);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        handler = new Handler();


        super.onCreate(savedInstanceState);

        initBDGameSDK();
    }

	@Override
	public void onBackPressed() {
		goWelcomeActivity();
	}

	private void goWelcomeActivity(){
		handler.removeCallbacksAndMessages(null);
		Intent intent = new Intent(baiduWelcomeActivity.this, baiduTRGame.class);
        startActivity(intent);
        finish();
	}
    private void initBDGameSDK() { // 初始化游戏SDK
        BDGameSDKSetting mBDGameSDKSetting = new BDGameSDKSetting();
        mBDGameSDKSetting.setAppID(9621216); // APPID设置
        mBDGameSDKSetting.setAppKey("0Me33iT6fXjp4ZG60xQxtiWL"); // APPKEY设置
        mBDGameSDKSetting.setDomain(BDGameSDKSetting.Domain.RELEASE); // 设置为正式模式
        mBDGameSDKSetting.setOrientation(Utils.getOrientation(this));

        BDGameSDK.init(this, mBDGameSDKSetting, new IResponse<Void>() {

            @Override
            public void onResponse(int resultCode, String resultDesc, Void extraData) {
                switch (resultCode) {
                    case ResultCode.INIT_SUCCESS:
                        // 初始化成功
                        handler.postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                goWelcomeActivity();
                            }
                        }, 0);
                        break;

                    case ResultCode.INIT_FAIL:
                    default:
                        Toast.makeText(baiduWelcomeActivity.this, "启动失败", Toast.LENGTH_LONG).show();
                        finish();
                        // 初始化失败
                }

            }

        });
    }
    @Override
    protected void onResume() {
        super.onResume();
        BDGameSDK.onResume(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        BDGameSDK.onPause(this);
    }
}
