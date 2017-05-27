/*
Copyright (C) Huawei Technologies Co., Ltd. 2015. All rights reserved.
See LICENSE.txt for this sample's licensing information.
*/
package com.bantu.trgame;

public class GlobalParam
{
    /**
     * 联盟为应用分配的应用ID
     */
    /**
     * APP ID
     */
    public static final String APP_ID = "100004273";

    /**
     * 支付ID
     */
	 /**
     * Pay ID
     */
    public static final String PAY_ID = "40086000020633230";
    
 /**
     * 支付公钥
     */
    /**
     * public key for pay
     */
    public static String PAY_RSA_PUBLIC = "";
     /*
     * 支付页面横竖屏参数：1表示竖屏，2表示横屏，默认竖屏
     */
    // portrait view for pay UI
	public static final int PAY_ORI = 1;
	// landscape view for pay UI
	public static final int PAY_ORI_LAND = 2;


	/**
	 * 调用浮标时需要使用浮标的私钥，安全考虑，必须放到服务端，通过此接口使用安全通道获取
	 */
	/**
	 * the server url for getting the buoy private key.The CP need to modify the
	 * value for the real server.
	 */
	public static final String GET_BUOY_PRIVATE_KEY = "https://zqtxqd.leanapp.cn/getHuaweiBuoyPrivate";
    public interface PayParams
    {
        public static final String USER_ID = "userID";
        
        public static final String APPLICATION_ID = "applicationID";
        
        public static final String AMOUNT = "amount";
        
        public static final String PRODUCT_NAME = "productName";
        
        public static final String PRODUCT_DESC = "productDesc";
        
        public static final String REQUEST_ID = "requestId";
        
        public static final String USER_NAME = "userName";
        
        public static final String SIGN = "sign";
        
        public static final String NOTIFY_URL = "notifyUrl";
        
        public static final String SERVICE_CATALOG = "serviceCatalog";
        
        public static final String SHOW_LOG = "showLog";
        
        public static final String SCREENT_ORIENT = "screentOrient";
        
        public static final String SDK_CHANNEL = "sdkChannel";
        
        public static final String URL_VER = "urlver";
    }
    
}
