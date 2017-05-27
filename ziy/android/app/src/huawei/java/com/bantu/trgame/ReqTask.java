package com.bantu.trgame;

import android.os.AsyncTask;
import android.util.Base64;


import com.huawei.gameservice.sdk.util.LogUtil;

import org.json.JSONObject;

import java.util.Map;

public class ReqTask extends AsyncTask<Void, Void, String>
{
    private static final String TAG = ReqTask.class.getSimpleName();
    
    private Delegate delegate = null;
    
    private Map<String, String> reqParams = null;
    
    private String reqUrl = null;
    
    public ReqTask(Delegate deg, Map<String, String> params, String url)
    {
        delegate = deg;
        reqParams = params;
        reqUrl = url;
    }
    
    @Override
    protected String doInBackground(Void... params)
    {
        String result = null;
        try
        {

            JSONObject jsonReq = new JSONObject();
            jsonReq.put("who","long");
            //jsonReq.put("cmds",reqParams);
            result = HttpUtils.doHttpsPost(reqUrl,jsonReq);
            /**
             * 请求地址为reqUrl，请求的POST参数为reqParams，使用UTF-8编码格式
             */
        }
        catch (Exception e)
        {
            LogUtil.d(TAG, e.getMessage());
        }
        return result;
    }
    
    @Override
    protected void onPostExecute(String result)
    {
        delegate.execute(result);
    }
    
    public interface Delegate
    {
        public void execute(String result);
    }
    
}