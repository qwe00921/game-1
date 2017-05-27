package com.bantu.zqtx.wxapi;

import com.bantu.trgame.TRGame;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler{
	  	
    private IWXAPI api;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//    	api = WXAPIFactory.createWXAPI(this, TRGame.WXAPPID);
//        api.handleIntent(getIntent(), this);
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
        api.handleIntent(intent, this);
	}

	@Override
	public void onReq(BaseReq req) {
	}

	@Override
	public void onResp(BaseResp resp) {	
		if(resp.errCode == BaseResp.ErrCode.ERR_OK)
		{
		    String code = ((SendAuth.Resp) resp).code;
		    TRGame.GetWXAccessToken(code);
		}
		else
			TRGame.WXOrQQLoginFailed();
		this.finish();
	}
}