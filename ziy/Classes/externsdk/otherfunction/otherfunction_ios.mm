#import "otherfunction.h"
#import "AppController.h"
#import "RootViewController.h"
#import <map>
#import <string>
#import "cocos2d.h"
#import "Common.h"
#import "Data.h"
#import "WXiosPay.h"
#import "AliPayOrderResult.h"
#import "PaymentTool.h"
//#import "ConfigHeader.h"    //youmi spot ad
//#import "CocoBVideo.h"      //youmi video ad
//#import <GoogleMobileAds/GoogleMobileAds.h>

#define LOGD CCLOG

DString statGetDeviceParams(StatDeviceParamType type)
{
    switch (type)
    {
        case StatParamAccount:
        {
            const int bufferSize = 1024;
            char* buffer = new char[1024];
            snprintf(buffer, bufferSize, "package_name=%s&platform=%s", "com.bantu.trgame", "win32");
            DString result = buffer;
            SAFE_DELETE_ARRAY(buffer);
            return result;
        }
        case StatParamPackageName:
        {
            //return "com.bantu.trgame";
            NSString *identifier = [[NSBundle mainBundle] bundleIdentifier];
            DString ret = [identifier UTF8String];
            return ret;
        }
        case StatParamPlatform:
            return "ios";
        case StatParamVersion:
        {
            //CFBundleShortVersionString
            //kCFBundleVersionKey
            NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
            DString ret = [version UTF8String];
            return ret;
        }
        case StatParamPatchVersion:
        {
            return "";
        }
        case StatParamMMChannelId:
        {
            return "";
        }
        default:
        {
            return "";
        }
    }
}

int payStartAlipay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
    LOGD("payStartAlipay");
    return iosPayStartAlipay(trGameId, outTradeNo, subject, body, totalFee);
}

int payStartwxpay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
    LOGD("payStartwxpay");
    return iosPayStartWXpay(trGameId, outTradeNo, subject, body, totalFee);
}

void gotoFullVersionAppStore()
{
    NSString *externalAppStoreURL  = [[NSBundle mainBundle].infoDictionary objectForKey:@"externalAppStoreURL"];
    if(externalAppStoreURL == nil){ //if we make a mistake
        externalAppStoreURL = @"https://www.trgame.cn";
    }
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString: externalAppStoreURL]];
}
extern void showSpinView();
bool iosBuyItem(const char* tradeNo, const char* productID)
{
    showSpinView();
    return [[AppController getTrgameIAP] buyItem:[NSString stringWithUTF8String:productID] appUserName:[NSString stringWithUTF8String:tradeNo]];
}
extern void showAppStore(const char * appId);
void iosShowAppStore(const char* appId)
{
   showAppStore(appId);
}

void iosInitItems(DList<DString>* productIDs)
{
    NSMutableSet * mutableSet = [[NSMutableSet alloc] init];
    for (int i = 0; i<productIDs->size(); i++) {
      [mutableSet addObject:[NSString stringWithUTF8String:productIDs->elementAt(i)->getChar()]];
    }
    NSSet * set = [NSSet setWithSet:mutableSet];
    [[AppController getTrgameIAP] initItems:set];
}

void iosRefreshProductList(DList<DString>* productIDs)
{
    NSMutableSet * mutableSet = [[NSMutableSet alloc] init];
    for (int i = 0; i < productIDs->size(); i++) {
        [mutableSet addObject:[NSString stringWithUTF8String:productIDs->elementAt(i)->getChar()]];
    }
    NSSet *set = [NSSet setWithSet:mutableSet];
    [[AppController getTrgameIAP] refreshProductsList:set];
}

void iosRestoreCompletedTransactions()
{
   [[AppController getTrgameIAP] restoreCompletedIAP];
}

bool checkProductRestored(const char* productID) {
    if ([[AppController getTrgameIAP] isProductRestored:productID])
        return true;
    return false;
}

void reportScoreToGameCenterLeaderboard(int score, DString leaderboardID, bool isSortHighToLow) {
//    BOOL isAvailable = [[GameCenterManager sharedManager] checkGameCenterAvailability];
//    if (isAvailable) {
//        if (isSortHighToLow) {
//            [[GameCenterManager sharedManager] saveAndReportScore:score leaderboard:[NSString stringWithUTF8String:leaderboardID->getNativeCharPtr()] sortOrder:GameCenterSortOrderHighToLow];
//        } else {
//            [[GameCenterManager sharedManager] saveAndReportScore:score leaderboard:[NSString stringWithUTF8String:leaderboardID->getNativeCharPtr()] sortOrder:GameCenterSortOrderLowToHigh];
//        }
//    } else {
//        NSLog(@"Game center is not available");
//    }
}

void reportAchievementToGameCenterLeaderboard(int percent, DString achievementID, bool shouldDisplayNotification) {
//    BOOL isAvailable = [[GameCenterManager sharedManager] checkGameCenterAvailability];
//    if (isAvailable) {
//            [[GameCenterManager sharedManager] saveAndReportAchievement:[NSString stringWithUTF8String:achievementID->getNativeCharPtr()] percentComplete:percent shouldDisplayNotification:shouldDisplayNotification];
//    } else {
//        NSLog(@"Game center is not available");
//    }
}

void showLeaderboard() {
//    BOOL isAvailable = [[GameCenterManager sharedManager] checkGameCenterAvailability];
//    [[GameCenterManager sharedManager] presentLeaderboardsOnViewController:[AppController getRootViewController]];
//    if (isAvailable) {
//        NSLog(@"game center is available");
//    } else {
//        NSLog(@"game center is not available");
//    }
}

extern void ChessExit();

void gameExit()
{
    ChessExit();
}

static NSString *youmiAppIDStr;
static NSString *youmiSecretKeyStr;
//static bool isShowingAd = false;
static bool youmiSpotAdEnable = false;
static bool youmiVideoAdEnable = false;

void initIOSAdYouMi(char *youmiAppID, char * youmiSecretKey) {
    NSLog(@"in initIOSAdYouMi appID char is %s, key char is %s", youmiAppID, youmiSecretKey);
    youmiAppIDStr = [[NSString alloc] initWithCString:youmiAppID encoding:NSUTF8StringEncoding];
    youmiSecretKeyStr = [[NSString alloc] initWithCString:youmiSecretKey encoding:NSUTF8StringEncoding];
    NSLog(@"in initIOSAdYouMi appID: %@, key: %@", youmiAppIDStr, youmiSecretKeyStr);
}
void initAdYoumi(char * id, char * key)
{
    initIOSAdYouMi(id, key);
}

void initIOSYouMiSpot(){
//    youmiSpotAdEnable = true;
//
//    [NewWorldSpt initQQWDeveloperParams:youmiAppIDStr QQ_SecretId:youmiSecretKeyStr];
//    //使用前先初始化一下插屏
//    [NewWorldSpt initQQWDeveLoper:kTypeLandscape];//填上你对应的横竖屏模式
//
//    //设置NewWorldSpt点击后的回调，可以不写。
//    [NewWorldSpt clickQQWSPTAction:^(BOOL flag){
//        //广告被点击的回调。
//        NSLog(@"clickYouMiSpotAction %d", flag);
//    }];
}
void enableAdYoumiSpot()
{
    initIOSYouMiSpot();
}

void initIOSYouMiVideo(){
//    youmiVideoAdEnable = true;
//    [CocoBVideo cBVideoInitWithAppID:youmiAppIDStr cBVideoAppIDSecret:youmiSecretKeyStr];
}
void enableAdYoumiVideo()
{
    initIOSYouMiVideo();
}

void playIOSYouMiSpot(void (*youmiSpotCallback)(bool result)) {
////    if (youmiSpotAdEnable && !isShowingAd) {
////        isShowingAd = true;
//        [NewWorldSpt showQQWSPTAction:^(BOOL flag){
////            isShowingAd = false;
//            youmiSpotCallback(flag);
//            if (flag) {
//                NSLog(@"log添加展示成功的逻辑");
//            }
//            else{
//                NSLog(@"log添加展示失败的逻辑");
//            }
//        }];
////    }
}
void showAdYoumiSpot(void (*callback)(bool result))
{
    playIOSYouMiSpot(callback);
}

void playIOSYouMiVideo(void (*youmiVideoCallback)(bool result)){
////    if (youmiVideoAdEnable && !isShowingAd) {
////        isShowingAd = true;
//        [CocoBVideo cBVideoPlay:[AppController getRootViewController] cBVideoPlayFinishCallBackBlock:^(BOOL isFinishPlay){
//            NSLog(@"视频播放结束");
////            isShowingAd = false;
//            youmiVideoCallback(isFinishPlay);
//        } cBVideoPlayConfigCallBackBlock:^(BOOL isLegal){
//            NSLog(@"此次播放是否有效：%d",isLegal);
//        }];
////    }
}
void showAdYoumiVideo(void (*callback)(bool result))
{
    playIOSYouMiVideo(callback);
}

bool getYoumiVideoAdEnable() {
    return youmiVideoAdEnable;
}


//static void (*AdMobcallback)(bool result);
//static GADInterstitial * adMob_interstitial;
//GADInterstitial * createAndLoadInterstitial();
//@interface AdModDelegate:NSObject <GADInterstitialDelegate>
//@end
//@implementation AdModDelegate
///// Called when an interstitial ad request succeeded.
//- (void)interstitialDidReceiveAd:(GADInterstitial *)ad {
//    NSLog(@"interstitialDidReceiveAd");
//}
//
///// Called when an interstitial ad request failed.
//- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error {
//    NSLog(@"interstitialDidFailToReceiveAdWithError: %@", [error localizedDescription]);
//}
//
///// Called just before presenting an interstitial.
//- (void)interstitialWillPresentScreen:(GADInterstitial *)ad {
//    NSLog(@"interstitialWillPresentScreen");
//}
//
///// Called before the interstitial is to be animated off the screen.
//- (void)interstitialWillDismissScreen:(GADInterstitial *)ad {
//    NSLog(@"interstitialWillDismissScreen");
//}
//
///// Called just after dismissing an interstitial and it has animated off the screen.
//- (void)interstitialDidDismissScreen:(GADInterstitial *)ad {
//    NSLog(@"interstitialDidDismissScreen");
//    adMob_interstitial = createAndLoadInterstitial();
//    if(AdMobcallback !=0 )
//        AdMobcallback(true);
//}
//
///// Called just before the application will background or terminate because the user clicked on an
///// ad that will launch another application (such as the App Store).
//- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad {
//    NSLog(@"interstitialWillLeaveApplication");
//}
//
//@end
//
//static AdModDelegate * adMob_delegate;
//GADInterstitial * createAndLoadInterstitial() {
//
//    GADInterstitial *interstitial = [[GADInterstitial alloc] initWithAdUnitID:[NSString stringWithCString:modConst->GOOGLE_ADMOB_IOS_UNITID->getNativeCharPtr() encoding:NSUTF8StringEncoding]];
//    GADRequest *request = [GADRequest request];
//    request.gender = kGADGenderMale;
//   // [request setLocationWithLatitude:0 longitude:0 accuracy:0];
//
//    interstitial.delegate = adMob_delegate;
//   // request.testDevices = @[@"2077ef9a63d2b398840261c8221a0c9b"];
//    [interstitial loadRequest: request];
//    return interstitial;
//}
//void initAdGoogle(){
//    adMob_delegate = [[AdModDelegate alloc]init];
//    adMob_interstitial = createAndLoadInterstitial();
//}
//
//void showAdMobInterstitial(void (*callback)(bool result)){
//    AdMobcallback = callback;
//    //CCLOG("###showAdMobInterstitial1");
//    if ([adMob_interstitial isReady]) {
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
//            [adMob_interstitial presentFromRootViewController:[AppController getRootViewController]];
//        });
//    } else {
//        callback(false);
//    }
//}

void sendWXAuthRequest()
{
//    if ([WXApi isWXAppInstalled] && [WXApi isWXAppSupportApi])
//    {
//        //构造SendAuthReq结构体
//        SendAuthReq* req =[[[SendAuthReq alloc ] init ] autorelease ];
//        req.scope = @"snsapi_userinfo" ;
//        req.state = @"123" ;
//        //第三方向微信终端发送一个SendAuthReq消息结构
//        [WXApi sendReq:req];
//    }
//    else
//    {
//        [AppController showLoginWarn:@"未安装微信，请选择其他登陆方式"];
//    }
}

void sendQQAuthRequest()
{
//    extern TencentOAuth *_tencentOAuth;
//    
//    NSArray *_permissions =  [[NSArray arrayWithObjects:@"get_user_info", @"get_simple_userinfo", @"add_t", nil] retain];
//    [_tencentOAuth authorize:_permissions inSafari:NO];
}

DString getNotifyUrl()
{
    return PaymentTool::getInstance()->getNotifyUrl().c_str();
}

void showWarning(const char* strWarn)
{
    PaymentTool::getInstance()->showWarning(strWarn);
}
