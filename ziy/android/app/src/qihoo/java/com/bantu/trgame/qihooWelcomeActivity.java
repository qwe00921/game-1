package com.bantu.trgame;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;

import com.bantu.R;

public class qihooWelcomeActivity extends Activity {

	private Handler handler = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {


        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.welcome_layout);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        handler = new Handler();

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
            	goWelcomeActivity();
            }
        }, 3000);
        super.onCreate(savedInstanceState);
    }

	@Override
	public void onBackPressed() {
		goWelcomeActivity();
	}

	private void goWelcomeActivity(){
		handler.removeCallbacksAndMessages(null);
		Intent intent = new Intent(qihooWelcomeActivity.this, qihooTRGame.class);
        startActivity(intent);
        finish();
	}
}
