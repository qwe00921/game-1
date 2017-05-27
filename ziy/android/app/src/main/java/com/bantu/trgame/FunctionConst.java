package com.bantu.trgame;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

import org.json.JSONException;
import org.json.JSONObject;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import android.content.Context;
import android.util.Log;

public class FunctionConst {

	private static FunctionConst instance;
	public static FunctionConst getInstance(Context context) {
		if (instance == null) {
			instance = new FunctionConst();
			instance.loadConfig(context);
		}
		return instance;
	}
	
	private FunctionConst() {
		
	}
	
	private String readAssetsFile(InputStream ins) {		
    	BufferedReader br = null;
		try {
			StringBuilder sb = new StringBuilder();
			br = new BufferedReader(new InputStreamReader(ins, "UTF-8"));
			String readLine = br.readLine();
	    	while (readLine != null) {
	    		sb.append(readLine);
	    		readLine = br.readLine();
	    	}
	    	return sb.toString();
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return "";
		} catch (IOException e) {
			e.printStackTrace();
			return "";
		} finally {
			if (br != null) {
				try {
					br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	private void loadConfig(Context context) {   	
    	String content = "";
		try {
			content = readAssetsFile(context.getAssets().open("function.txt"));
		} catch (IOException e1) {
			e1.printStackTrace();
			return;
		}
    	try {
			JSONObject jo = new JSONObject(content);
			
			EnableTalkingData = jo.optBoolean("EnableTalkingData");
			TalkingDataAppId = jo.optString("TalkingDataAppId");
			TalkingDataChannelId = jo.optString("TalkingDataChannelId");
			EncryptResource = jo.optBoolean("EncryptResource");
			ShowFrame = jo.optBoolean("ShowFrame");
			WXAPPID  = jo.optString("WXAPPID");
			WXMCHID  = jo.optString("WXMCHID");
			WXSecret  = jo.optString("WXSecret");
			QQAPPID  = jo.optString("QQAPPID");
			
			Log.d("funConst", String.format("FunctionConst.loadConfig.MMChannelId=%s", MMChannelId));
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	public boolean EnableTalkingData = false;
	public String TalkingDataAppId = "";
	public String TalkingDataChannelId = "";
	public boolean EncryptResource = false;
	public boolean ShowFrame = false;
	public String MMChannelId = "";
	public String WXAPPID = "";
	public String WXMCHID = "";
	public String WXSecret = "";
	public String QQAPPID = "";
}
