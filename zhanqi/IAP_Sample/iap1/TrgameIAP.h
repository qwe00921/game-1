//
//  TrgameIAP.h
//  iap1
//
//  Created by apache on 15/3/14.
//  Copyright (c) 2015年 apache. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
//#define DEBUG
@protocol TrgameIAPDelegate <NSObject>
@required
- (void)buyItemResult:(NSString *) productID result:(BOOL)result;
- (void)verifyAppResult:(BOOL)result;
@end

@interface TrgameIAP : NSObject

//- (int)verifyAppReceipt;
//- (BOOL)verifyInAppPurchase:(SKPaymentTransaction *) transaction;

- (void)setDelegate:(id <TrgameIAPDelegate>)delegate;
- (void)initItems:(NSSet *) productIDS;
- (BOOL)buyItem:(NSString *) productID;
- (void)verifyApp;

@end

