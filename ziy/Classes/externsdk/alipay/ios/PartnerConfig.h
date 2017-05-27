//
//  PartnerConfig.h
//  AlipaySdkDemo
//
//  Created by ChaoGanYing on 13-5-3.
//  Copyright (c) 2013年 RenFei. All rights reserved.
//
//  提示：如何获取安全校验码和合作身份者id
//  1.用您的签约支付宝账号登录支付宝网站(www.alipay.com)
//  2.点击“商家服务”(https://b.alipay.com/order/myorder.htm)
//  3.点击“查询合作者身份(pid)”、“查询安全校验码(key)”
//

#ifndef MQPDemo_PartnerConfig_h
#define MQPDemo_PartnerConfig_h

//合作身份者id，以2088开头的16位纯数字
#define PartnerID @"2088901132447379"
//收款支付宝账号
#define SellerID  @"hr@bantus.cn"

//安全校验码（MD5）密钥，以数字和字母组成的32位字符
//#define MD5_KEY @""

//商户私钥，自助生成
#define PartnerPrivKey @"MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAL6BUxsMvnsoORllCLfc+jirAtP11KL3iNpv7GH7SMRfxOPcD7bqYgiSVToN+BwjonUenbsqAdRZGiXtU4iP+M7RoXVxrD13N24bwFYgcZy4TdgIiCe3DvPysJYsB4abCsidvGTp/0+cfDRv1QDZmHEqN6b1OvkAAo7xLDTGseFFAgMBAAECgYB/SBLJkOM3j/FOeUWw6tC3yfJywd3XIIHsSDlsYaMFqWL4RtCtCr6DMZoF1s19p4VksRb7tR5RzVJ+IqncJS07f9we5BfcMLhsx0aMF0qpYcoshhz8XCnCkC0e0vktEi6/qXZscF0ZlGcZ6jEHplw82jE942Icusp0hNaVHKmnwQJBAPzvv5uc+sp31+Ev4JBxQtVzp+GfQwmNw4tXEWaecO1yQhHJUi0wK2xa/vyRMvVi16iyWZ7NyFa8cU8ytVMVzBECQQDA0ACciBsJwCsm9ApNyRkQQ7ic8/+zB2QQSSMEcoDLmFzfJ/qFy+KcZBQ6k+JtiEGICHwrjeOI9BT47ER+hUX1AkEAlaJ0FWhqCauagDLlxC3tHv/7+oNQkm9WlrFcrvvezOpCYela/za3CbRPkzWHExp1gOx6DnOpKH70/ah/EvcbMQJAJ8L9TUp4IPOjMn5DHbVp2MPOb3eV9IqCpSUf9ovSaan7BP4OmMbp7Yxp64aRvDrMIMx3Jtg04wpKxOacXWcIfQJALQsMwFLbPryx4FY9O5WLZ94leONTPelJ4haMo8vKMxzahgm19tTA4jGYYIRpMgd3d/lIDHnIlAKoIjgSZdlqVw=="


//支付宝公钥
#define AlipayPubKey   @"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRAFljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQEB/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5KsiNG9zpgmLCUYuLkxpLQIDAQAB"

#endif
