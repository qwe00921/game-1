//
//  AliPayOrderResult.h
//

extern "C" {
#import "PartnerConfig.h"
#import "DataSigner.h"
#import "AlixPayResult.h"
#import "DataVerifier.h"
#import "AliPayOrder.h"
}

#import "StatConfig.h"
#import "AliPayOrderResult.h"
#import <AlipaySDK/AlipaySDK.h>


static AliPayOrder* gLocalOrder = NULL;


//wap回调函数
void AliPaymentResult(NSDictionary *resultDic)
{
    if (resultDic) {
        NSLog(@"AliPaymentResult,dic=%@",resultDic);
        AlixPayResult* result = [[AlixPayResult alloc] initWithDictionary:resultDic];
        if (result.statusCode == 9000) {
            
            NSString* key = AlipayPubKey;//签约帐户后获取到的支付宝公钥
            id<DataVerifier> verifier = CreateRSADataVerifier(key);
            if ([verifier verifyString:result.resultString withSign:result.signString]) {
                //验证签名成功，交易结果无篡改
                NSLog(@"paymentResult succeed,resultString:%@", result.resultString);
                
                StatConfig::onPayResult(StatConfig::PAY_TYPE_ALIPAY, [gLocalOrder.tradeNO UTF8String], 1, "succeed");
                
            } else {
                NSLog(@"paymentResult failed,verify error");
                StatConfig::onPayResult(StatConfig::PAY_TYPE_ALIPAY, [gLocalOrder.tradeNO UTF8String], 0, "failed");
            }
        }
        else {
            NSLog(@"paymentResult failed,statusCode=%d", result.statusCode);
            StatConfig::onPayResult(StatConfig::PAY_TYPE_ALIPAY, [gLocalOrder.tradeNO UTF8String], 0, "failed");
            
        }
    } else {
        NSLog(@"paymentResult failed,resultDic nil");
        StatConfig::onPayResult(StatConfig::PAY_TYPE_ALIPAY, [gLocalOrder.tradeNO UTF8String], 0, "failed");
    }
}

int iosPayStartAlipay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
    NSLog(@"iosPayStartAlipay=%s",outTradeNo);

    AliPayOrder *order = [[AliPayOrder alloc] init];
    
    order.partner = PartnerID;
    order.seller = SellerID;
    
    order.tradeNO = [NSString stringWithFormat:@"%s", outTradeNo]; //订单ID（由商家自行制定）
    //order.productName = [NSString stringWithFormat:@"%s", subject]; //商品标题
    //order.productName = [NSString stringWithCString:subject encoding:NSUTF8StringEncoding];
    order.productName = [NSString stringWithUTF8String:subject];
    order.productDescription = [NSString stringWithFormat:@"%s", body]; //商品描述
    order.amount = [NSString stringWithFormat:@"%s", totalFee]; //商品价格
    order.notifyURL =  @"https://dqfyl.leanapp.cn/alipayCallBack"; //回调URL
    
    order.service = @"mobile.securitypay.pay";
    order.paymentType = @"1";
    order.inputCharset = @"utf-8";
    order.itBPay = @"30m";
    order.showUrl = @"m.alipay.com";
    
    //应用注册scheme,在AlixPayDemo-Info.plist定义URL types
    NSString *appScheme = @"AlipayAppScheme";//[[NSBundle mainBundle].infoDictionary objectForKey:@"AlipayAppScheme"];
    
    //将商品信息拼接成字符串
    NSString *orderSpec = [order description];
    NSLog(@"orderSpec = %@",orderSpec);
    
    //获取私钥并将商户信息签名,外部商户可以根据情况存放私钥和签名,只需要遵循RSA签名规范,并将签名字符串base64编码和UrlEncode
    id<DataSigner> signer = CreateRSADataSigner(PartnerPrivKey);
    NSString *signedString = [signer signString:orderSpec];
    
    //将签名成功字符串格式化为订单字符串,请严格按照该格式
    if (signedString != nil) {
       
        gLocalOrder = order;
        
        NSString *orderString = [NSString stringWithFormat:@"%@&sign=\"%@\"&sign_type=\"%@\"",
                       orderSpec, signedString, @"RSA"];
        
        [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
            NSLog(@"reslut = %@",resultDic);
            AliPaymentResult(resultDic);
        }];
        
        return 1;
    }

    return 0;
}






