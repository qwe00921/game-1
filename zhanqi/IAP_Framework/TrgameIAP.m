//
//  TrgameIAP.m
//  iap1
//
//  Created by apache on 15/3/14.
//  Copyright (c) 2015年 apache. All rights reserved.
//

#import "TrgameIAP.h"
#import <StoreKit/StoreKit.h>

#import <Security/Security.h>
#import <UIKit/UIKit.h>

#include <openssl/pkcs7.h>
#include <openssl/objects.h>
#include <openssl/sha.h>
#include <openssl/x509.h>
#include <openssl/err.h>
typedef void(^finish_callback_t)(BOOL);
@interface TrgameIAP () <SKPaymentTransactionObserver, SKProductsRequestDelegate>

@property (strong, atomic) id <TrgameIAPDelegate> IAPdelegate;
@property (assign, atomic) BOOL isRequestingProducts;
@property (strong, atomic) NSArray *products;
@property (strong, atomic) NSSet * productIDS;
@property (strong, atomic) SKProductsRequest * productRequest;
@property (strong, atomic) SKReceiptRefreshRequest * receiptRefreshRequest;
@property (strong, atomic) finish_callback_t receiptRefreshCallback;


//- (void) validateProductIdentifiers: (NSSet *) productsIDS;
@end

// link with Foundation.framework, Security.framework, libssl and libCrypto (via -lssl -lcrypto in Other Linker Flags)



/*
 #ifndef YES_I_HAVE_READ_THE_WARNING_AND_I_ACCEPT_THE_RISK
 
 #warning --- DON'T USE THIS CODE AS IS! IF EVERYONE USES THE SAME CODE
 #warning --- IT IS PRETTY EASY TO BUILD AN AUTOMATIC CRACKING TOOL
 #warning --- FOR APPS USING THIS CODE!
 #warning --- BY USING THIS CODE YOU ACCEPT TAKING THE RESPONSIBILITY FOR
 #warning --- ANY DAMAGE!
 #warning ---
 #warning --- YOU HAVE BEEN WARNED!
 
 // if you want to take that risk, add "-DYES_I_HAVE_READ_THE_WARNING_AND_I_ACCEPT_THE_RISK" in the build settings at "Other C Flags"
 
 #endif // YES_I_HAVE_READ_THE_WARNING_AND_I_ACCEPT_THE_RISK
 
 */
#define VRCFRelease(object) if(object) CFRelease(object)

NSString *kReceiptBundleIdentifier				= @"BundleIdentifier";
NSString *kReceiptBundleIdentifierData			= @"BundleIdentifierData";
NSString *kReceiptVersion						= @"Version";
NSString *kReceiptOpaqueValue					= @"OpaqueValue";
NSString *kReceiptHash							= @"Hash";
NSString *kReceiptInApp							= @"InApp";
NSString *kReceiptOriginalVersion               = @"OrigVer";
NSString *kReceiptExpirationDate                = @"ExpDate";

NSString *kReceiptInAppQuantity					= @"Quantity";
NSString *kReceiptInAppProductIdentifier		= @"ProductIdentifier";
NSString *kReceiptInAppTransactionIdentifier	= @"TransactionIdentifier";
NSString *kReceiptInAppPurchaseDate				= @"PurchaseDate";
NSString *kReceiptInAppOriginalTransactionIdentifier	= @"OriginalTransactionIdentifier";
NSString *kReceiptInAppOriginalPurchaseDate		= @"OriginalPurchaseDate";
NSString *kReceiptInAppSubscriptionExpirationDate = @"SubExpDate";
NSString *kReceiptInAppCancellationDate         = @"CancelDate";
NSString *kReceiptInAppWebOrderLineItemID       = @"WebItemId";


NSData *appleRootCert(void) {
    // Obtain the Apple Inc. root certificate from http://www.apple.com/certificateauthority/
    // Download the Apple Inc. Root Certificate ( http://www.apple.com/appleca/AppleIncRootCertificate.cer )
    // Add the AppleIncRootCertificate.cer to your app's resource bundle.
    
    NSData *cert = [NSData dataWithContentsOfURL:[[NSBundle mainBundle] URLForResource:@"AppleIncRootCertificate" withExtension:@"cer"]];
    
    return cert;
}

// ASN.1 values for In-App Purchase values
#define INAPP_ATTR_START	1700
#define INAPP_QUANTITY		1701
#define INAPP_PRODID		1702
#define INAPP_TRANSID		1703
#define INAPP_PURCHDATE		1704
#define INAPP_ORIGTRANSID	1705
#define INAPP_ORIGPURCHDATE	1706
#define INAPP_ATTR_END		1707
#define INAPP_SUBEXP_DATE   1708
#define INAPP_WEBORDER      1711
#define INAPP_CANCEL_DATE   1712



// ASN.1 values for the App Store receipt
#define ATTR_START 1
#define BUNDLE_ID 2
#define VERSION 3
#define OPAQUE_VALUE 4
#define HASH 5
#define ATTR_END 6
#define INAPP_PURCHASE 17
#define ORIG_VERSION 19
#define EXPIRE_DATE 21



//error number
#define VERIFY_APP_SUCCESS              100
#define NO_RECEIPT_ERR                  101
#define SIGNATURE_FAILED_ERR            102
#define BUNDLE_IDENTIFIER_ERR           103
#define BUNDLE_VERSION_ERR              104
#define VERIFY_RECEIPT_HASH_ERR         105

int signature_err = 0;
int bundle_identifier_err = 0;
int bundle_version_err = 0;
int hash_err = 0;


char signatureRawValue[] = {
    0xcf, 0x7d, 0xfb, 0x44, 0xcf, 0x7d, 0xfc, 0x5c, 0x5f, 0xfc, 0xfd, 0xfe, 0xfd, 0xfd, 0xfe, 0xfd,
    0xcf, 0xf2, 0xf9, 0xf6, 0xd5, 0x79, 0xb7, 0x79, 0x08, 0xf2, 0xfe, 0xfe, 0xfa, 0xfa, 0xff, 0xcf,
    0x9d, 0xce, 0xf4, 0xcf, 0xf6, 0xf9, 0xfc, 0xaa, 0xfb, 0xf9, 0xec, 0xfd, 0xaa, 0xac, 0xce, 0xec,
    0xcf, 0xee, 0xf9, 0xfc, 0xaa, 0xfb, 0xf5, 0xec, 0xf5, 0xbe, 0x8f, 0x8f, 0x93, 0x9a, 0xdf, 0xb6,
    0x91, 0x9c, 0xd1, 0xce, 0xd9, 0xcf, 0xdb, 0xf9, 0xfc, 0xaa, 0xfb, 0xf4, 0xec, 0xe2, 0xbe, 0x8f,
    0x8f, 0x93, 0x9a, 0xdf, 0xbc, 0x9a, 0x8d, 0x8b, 0x96, 0x99, 0x96, 0x9c, 0x9e, 0x8b, 0x96, 0x90,
    0x91, 0xdf, 0xbe, 0x8a, 0x8b, 0x97, 0x90, 0x8d, 0x96, 0x8b, 0x86, 0xce, 0xe9, 0xcf, 0xeb, 0xf9,
    0xfc, 0xaa, 0xfb, 0xfc, 0xec, 0xf2, 0xbe, 0x8f, 0x8f, 0x93, 0x9a, 0xdf, 0xad, 0x90, 0x90, 0x8b,
    0xdf, 0xbc, 0xbe, 0xcf, 0xe1, 0xe8, 0xf2, 0xcf, 0xc9, 0xcf, 0xcb, 0xcd, 0xca, 0xcd, 0xce, 0xcb,
    0xcf, 0xcc, 0xc9, 0xa5, 0xe8, 0xf2, 0xcc, 0xca, 0xcf, 0xcd, 0xcf, 0xc6, 0xcd, 0xce, 0xcb, 0xcf,
    0xcc, 0xc9, 0xa5, 0xcf, 0x9d, 0xce, 0xf4, 0xcf, 0xf6, 0xf9, 0xfc, 0xaa, 0xfb, 0xf9, 0xec, 0xfd,
    0xaa, 0xac, 0xce, 0xec, 0xcf, 0xee, 0xf9, 0xfc, 0xaa, 0xfb, 0xf5, 0xec, 0xf5, 0xbe, 0x8f, 0x8f,
    0x93, 0x9a, 0xdf, 0xb6, 0x91, 0x9c, 0xd1, 0xce, 0xd9, 0xcf, 0xdb, 0xf9, 0xfc, 0xaa, 0xfb, 0xf4,
    0xec, 0xe2, 0xbe, 0x8f, 0x8f, 0x93, 0x9a, 0xdf, 0xbc, 0x9a, 0x8d, 0x8b, 0x96, 0x99, 0x96, 0x9c,
    0x9e, 0x8b, 0x96, 0x90, 0x91, 0xdf, 0xbe, 0x8a, 0x8b, 0x97, 0x90, 0x8d, 0x96, 0x8b, 0x86, 0xce,
    0xe9, 0xcf, 0xeb, 0xf9, 0xfc, 0xaa, 0xfb, 0xfc, 0xec, 0xf2, 0xbe, 0x8f, 0x8f, 0x93, 0x9a, 0xdf,
    0xad, 0x90, 0x90, 0x8b, 0xdf, 0xbc, 0xbe, 0xcf, 0x7d, 0xfe, 0xdd, 0xcf, 0xf2, 0xf9, 0xf6, 0xd5,
    0x79, 0xb7, 0x79, 0x08, 0xf2, 0xfe, 0xfe, 0xfe, 0xfa, 0xff, 0xfc, 0x7d, 0xfe, 0xf0, 0xff, 0xcf,
    0x7d, 0xfe, 0xf5, 0xfd, 0x7d, 0xfe, 0xfe, 0xff, 0x1b, 0x6e, 0x56, 0xf6, 0xe0, 0x6e, 0x24, 0xe1,
    0xb8, 0xaf, 0x14, 0xfa, 0x12, 0xa1, 0x86, 0x7b, 0xd2, 0x14, 0xc9, 0x5d, 0xa8, 0xb3, 0xaa, 0x13,
    0x74, 0xe6, 0x76, 0x21, 0x06, 0xb4, 0x93, 0x0a, 0xf8, 0x54, 0xdd, 0xcf, 0xfd, 0x17, 0xe7, 0xc1,
    0x07, 0xaf, 0xf6, 0x2c, 0x80, 0xbe, 0x57, 0x67, 0x06, 0x2e, 0x35, 0x99, 0x63, 0xdb, 0x94, 0xee,
    0x2f, 0x5c, 0x44, 0x1b, 0xe4, 0xd5, 0x3c, 0xe0, 0x6a, 0x61, 0x85, 0xf3, 0x5b, 0xb8, 0x74, 0xa4,
    0x2b, 0xe9, 0xc8, 0xcc, 0x34, 0x3b, 0xf0, 0xb2, 0x31, 0xeb, 0x96, 0x2e, 0x36, 0xe6, 0x8d, 0x0a,
    0xa2, 0xf1, 0x2a, 0x80, 0xa0, 0x64, 0x0d, 0xda, 0xfc, 0x45, 0xaa, 0x70, 0xb2, 0xa2, 0xf2, 0x0e,
    0x9b, 0xca, 0xdc, 0xea, 0xb4, 0xea, 0xa6, 0xe2, 0x4c, 0x6b, 0x08, 0x09, 0x63, 0x61, 0x30, 0xaf,
    0x45, 0x3e, 0xa7, 0xaf, 0x98, 0x70, 0xf7, 0x4b, 0xdf, 0x08, 0x34, 0x53, 0xd3, 0xdf, 0x90, 0x8f,
    0x49, 0xc0, 0xfe, 0xcf, 0x73, 0x48, 0xbc, 0x30, 0xf0, 0x62, 0xc2, 0x0c, 0xd4, 0xb6, 0xd7, 0xe5,
    0x37, 0x01, 0x31, 0x4a, 0x46, 0xf1, 0x26, 0xa1, 0xe3, 0x29, 0x34, 0xc2, 0x4a, 0xc5, 0x52, 0x0b,
    0xf0, 0xf1, 0xff, 0x6d, 0xf4, 0x4e, 0xde, 0xe9, 0xd1, 0x8b, 0x2a, 0xc3, 0xf2, 0x24, 0x9d, 0xe9,
    0x54, 0x5c, 0x8e, 0x6d, 0xb8, 0xac, 0xaa, 0x3e, 0x50, 0xd0, 0xbe, 0x4c, 0x07, 0x04, 0x1c, 0x8f,
    0x32, 0x19, 0x5c, 0xb3, 0xba, 0x81, 0xe0, 0xb3, 0x94, 0xaf, 0x69, 0xbe, 0x76, 0x3b, 0x8b, 0x9d,
    0xf4, 0xef, 0x7c, 0xbe, 0x78, 0xcc, 0x75, 0x7e, 0x4e, 0xcf, 0xa7, 0x13, 0xa5, 0xfb, 0xcd, 0x73,
    0x97, 0x4c, 0x70, 0xe2, 0x21, 0x9a, 0x8c, 0x00, 0x98, 0xa1, 0x9a, 0x43, 0xb6, 0x27, 0x89, 0x60,
    0xcc, 0xeb, 0x9a, 0x5e, 0x88, 0x6b, 0x36, 0xd2, 0xfd, 0xfc, 0xfe, 0xff, 0xfe, 0x5c, 0x7d, 0xfe,
    0x85, 0xcf, 0x7d, 0xfe, 0x89, 0xcf, 0xf1, 0xf9, 0xfc, 0xaa, 0xe2, 0xf0, 0xfe, 0xfe, 0x00, 0xfb,
    0xfb, 0xfc, 0xfd, 0xfe, 0xf9, 0xcf, 0xf0, 0xf9, 0xfc, 0xaa, 0xe2, 0xec, 0xfe, 0xfe, 0x00, 0xfb,
    0xfa, 0xcf, 0xfc, 0xfe, 0xfe, 0x00, 0xcf, 0xe2, 0xf9, 0xfc, 0xaa, 0xe2, 0xf1, 0xfb, 0xe9, 0xfb,
    0xeb, 0xd4, 0x2f, 0x96, 0xb8, 0x6b, 0x89, 0xf6, 0x01, 0x0b, 0x94, 0x72, 0xd1, 0xbf, 0x59, 0x08,
    0xb8, 0xb2, 0x80, 0xf7, 0xa1, 0xcf, 0xe0, 0xf9, 0xfc, 0xaa, 0xe2, 0xdc, 0xfb, 0xe7, 0xcf, 0xe9,
    0x7f, 0xeb, 0xd4, 0x2f, 0x96, 0xb8, 0x6b, 0x89, 0xf6, 0x01, 0x0b, 0x94, 0x72, 0xd1, 0xbf, 0x59,
    0x08, 0xb8, 0xb2, 0x80, 0xf7, 0xa1, 0xcf, 0x7d, 0xfe, 0xee, 0xf9, 0xfc, 0xaa, 0xe2, 0xdf, 0xfb,
    0x7d, 0xfe, 0xf7, 0xcf, 0x7d, 0xfe, 0xfb, 0xcf, 0x7d, 0xfe, 0xff, 0xf9, 0xf6, 0xd5, 0x79, 0xb7,
    0x79, 0x08, 0x9c, 0x9b, 0xfa, 0xfe, 0xcf, 0x7e, 0x0d, 0xcf, 0xd5, 0xf9, 0xf7, 0xd4, 0xf9, 0xfe,
    0xfa, 0xfa, 0xf8, 0xfd, 0xfe, 0xe9, 0xe1, 0x97, 0x8b, 0x8b, 0x8f, 0x8c, 0xc5, 0xd0, 0xd0, 0x88,
    0x88, 0x88, 0xd1, 0x9e, 0x8f, 0x8f, 0x93, 0x9a, 0xd1, 0x9c, 0x90, 0x92, 0xd0, 0x9e, 0x8f, 0x8f,
    0x93, 0x9a, 0x9c, 0x9e, 0xd0, 0xcf, 0x7e, 0x3c, 0xf9, 0xf7, 0xd4, 0xf9, 0xfe, 0xfa, 0xfa, 0xf8,
    0xfd, 0xfd, 0xcf, 0x7e, 0x49, 0xe5, 0x7e, 0x4c, 0xad, 0x9a, 0x93, 0x96, 0x9e, 0x91, 0x9c, 0x9a,
    0xdf, 0x90, 0x91, 0xdf, 0x8b, 0x97, 0x96, 0x8c, 0xdf, 0x9c, 0x9a, 0x8d, 0x8b, 0x96, 0x99, 0x96,
    0x9c, 0x9e, 0x8b, 0x9a, 0xdf, 0x9d, 0x86, 0xdf, 0x9e, 0x91, 0x86, 0xdf, 0x8f, 0x9e, 0x8d, 0x8b,
    0x86, 0xdf, 0x9e, 0x8c, 0x8c, 0x8a, 0x92, 0x9a, 0x8c, 0xdf, 0x9e, 0x9c, 0x9c, 0x9a, 0x8f, 0x8b,
    0x9e, 0x91, 0x9c, 0x9a, 0xdf, 0x90, 0x99, 0xdf, 0x8b, 0x97, 0x9a, 0xdf, 0x8b, 0x97, 0x9a, 0x91,
    0xdf, 0x9e, 0x8f, 0x8f, 0x93, 0x96, 0x9c, 0x9e, 0x9d, 0x93, 0x9a, 0xdf, 0x8c, 0x8b, 0x9e, 0x91,
    0x9b, 0x9e, 0x8d, 0x9b, 0xdf, 0x8b, 0x9a, 0x8d, 0x92, 0x8c, 0xdf, 0x9e, 0x91, 0x9b, 0xdf, 0x9c,
    0x90, 0x91, 0x9b, 0x96, 0x8b, 0x96, 0x90, 0x91, 0x8c, 0xdf, 0x90, 0x99, 0xdf, 0x8a, 0x8c, 0x9a,
    0xd3, 0xdf, 0x9c, 0x9a, 0x8d, 0x8b, 0x96, 0x99, 0x96, 0x9c, 0x9e, 0x8b, 0x9a, 0xdf, 0x8f, 0x90,
    0x93, 0x96, 0x9c, 0x86, 0xdf, 0x9e, 0x91, 0x9b, 0xdf, 0x9c, 0x9a, 0x8d, 0x8b, 0x96, 0x99, 0x96,
    0x9c, 0x9e, 0x8b, 0x96, 0x90, 0x91, 0xdf, 0x8f, 0x8d, 0x9e, 0x9c, 0x8b, 0x96, 0x9c, 0x9a, 0xdf,
    0x8c, 0x8b, 0x9e, 0x8b, 0x9a, 0x92, 0x9a, 0x91, 0x8b, 0x8c, 0xd1, 0xcf, 0xf2, 0xf9, 0xf6, 0xd5,
    0x79, 0xb7, 0x79, 0x08, 0xf2, 0xfe, 0xfe, 0xfa, 0xfa, 0xff, 0xfc, 0x7d, 0xfe, 0xfe, 0xff, 0xa3,
    0xc9, 0x66, 0xb3, 0xd2, 0x87, 0x48, 0x12, 0x73, 0x64, 0x23, 0x0c, 0x88, 0x64, 0x0d, 0x89, 0x2d,
    0x88, 0xcf, 0xb0, 0x3e, 0xe0, 0x7a, 0x7c, 0x7a, 0xe4, 0x66, 0xc2, 0xb8, 0xc8, 0x0d, 0x56, 0x64,
    0xbf, 0x71, 0xd3, 0x2b, 0x4e, 0x6f, 0xed, 0x27, 0x41, 0x0b, 0x8c, 0x64, 0x11, 0x2d, 0x9b, 0xf0,
    0x34, 0x86, 0xb0, 0xcb, 0x27, 0x5d, 0xc1, 0x06, 0x87, 0x00, 0x94, 0x37, 0xf8, 0x13, 0x82, 0xc6,
    0x7c, 0x74, 0xac, 0xdf, 0x2c, 0xc7, 0x3b, 0x4e, 0x40, 0x65, 0xb0, 0xf5, 0x94, 0x00, 0xd4, 0x03,
    0xa6, 0x58, 0xfa, 0xf6, 0x83, 0xe8, 0xbf, 0xa9, 0xee, 0xe1, 0x8b, 0x2c, 0x48, 0x74, 0xdc, 0xc4,
    0xb8, 0x5c, 0x2a, 0x90, 0xdb, 0x1d, 0x14, 0x2e, 0x48, 0x8f, 0x20, 0xf0, 0xba, 0x1e, 0xd8, 0x35,
    0x0e, 0x92, 0x87, 0x12, 0x18, 0x4a, 0xe8, 0xe8, 0x57, 0x23, 0x81, 0xdd, 0xca, 0x35, 0xda, 0x2a,
    0x26, 0xf0, 0x29, 0x94, 0x2b, 0x5d, 0xdb, 0xdc, 0xee, 0x08, 0x5e, 0x53, 0x70, 0x8c, 0x7e, 0x9f,
    0x39, 0xe4, 0xa4, 0xf6, 0xd0, 0x6d, 0x4d, 0x07, 0xbb, 0xb7, 0x0f, 0x9f, 0xc7, 0x61, 0xea, 0x0a,
    0xc2, 0xd9, 0x98, 0xdf, 0x75, 0xcc, 0x95, 0x08, 0xf2, 0x7d, 0x30, 0x21, 0x14, 0x5c, 0xd0, 0x06,
    0xac, 0x95, 0xa4, 0x9b, 0x3f, 0x9c, 0xcc, 0x88, 0x08, 0xc5, 0xf8, 0xd3, 0xa9, 0x14, 0x25, 0xf0,
    0xde, 0xf1, 0x25, 0x45, 0x8c, 0xe6, 0xb0, 0x4a, 0x26, 0xc9, 0x80, 0x3e, 0x78, 0xaa, 0x26, 0x58,
    0x66, 0x46, 0xcd, 0xbd, 0x04, 0x27, 0x2a, 0x8e, 0x61, 0x81, 0x5e, 0xad, 0x48, 0xe4, 0x42, 0x6c,
    0xbd, 0xdb, 0xed, 0xd5, 0x38, 0xf0, 0xe2, 0x49, 0xb2, 0x63, 0xa1, 0x9c, 0x37, 0xb4, 0x7f, 0xe8,
    0xaf, 0x55, 0x75, 0x2a, 0x25, 0x1b, 0x03, 0x2f, 0xf6, 0xf8, 0xc8, 0x4f, 0x8a, 0x8a, 0xde};
#define SIGNATURE_RAW_VALUE_LENGTH 1215

//extern const NSString * global_bundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
//extern const NSString * global_bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];

// in your project define those two somewhere as such:
// const NSString * global_bundleVersion = @"1.0.2";
// const NSString * global_bundleIdentifier = @"com.example.SampleApp";

@implementation TrgameIAP


- (void)setDelegate:(id <TrgameIAPDelegate>)delegate {
    self.IAPdelegate = delegate;
    //SKPaymentQueue.defaultQueue().addTransactionObserver(self)
    [[SKPaymentQueue defaultQueue] addTransactionObserver: self];
}

- (void)initItems:(NSSet *) productIDS {
    self.productIDS  = [productIDS copy];
    [self getProductsByIdentifiers:productIDS];
}

- (BOOL) buyItem:(NSString *) productID {
    if (self.products == nil) {
        return FALSE;
    }
    
    for (int i=0; i<self.products.count; i++) {
        SKProduct * product = self.products[i];
        if([product.productIdentifier isEqualToString: productID]){
            SKMutablePayment * payment = [[SKMutablePayment alloc] init];
            payment.productIdentifier = productID;
            payment.quantity = 1;
            [[SKPaymentQueue defaultQueue] addPayment:payment];
            return TRUE;
        }
    }
    return FALSE;
}

- (void)verifyApp {
    if(verifyAppReceipt() == 100){
        [self.IAPdelegate verifyAppResult:TRUE];
    } else {
     [self startRefreshReceipt:^(BOOL result) {
#ifdef DEBUG
         NSLog(@"refreshReceiptCallback %d %d", result, verifyAppReceipt() );
#endif
         if (result && verifyAppReceipt() == 100) {
             [self.IAPdelegate verifyAppResult:TRUE];
         } else {
             [self.IAPdelegate verifyAppResult:FALSE];
         }
     }];
    }
}


- (void) getProductsByIdentifiers: (NSSet *)productIDS {
    if (self.isRequestingProducts)
        return;
    self.isRequestingProducts = YES;
    self.productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIDS];
    self.productRequest.delegate = self;
    [self.productRequest start];
    
}

#pragma mark callback

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (int i=0; i<transactions.count; i++) {
        SKPaymentTransaction * tran = transactions[i];
#ifdef DEBUG
        NSLog(@"UpdatedTranscation no.%d state:%d", i, tran.transactionState);
#endif
        if (tran.transactionState == SKPaymentTransactionStatePurchased) {
            if(verifyInAppPurchase(tran)){
                [self.IAPdelegate buyItemResult:tran.payment.productIdentifier result:TRUE];
            } else {
                [self.IAPdelegate buyItemResult:tran.payment.productIdentifier result:FALSE];
            }
        }
        
        if (tran.transactionState == SKPaymentTransactionStateFailed) {
            [self.IAPdelegate buyItemResult:tran.payment.productIdentifier result:FALSE];
        }
        
        if (tran.transactionState == SKPaymentTransactionStatePurchased || tran.transactionState == SKPaymentTransactionStateFailed) {
            [[SKPaymentQueue defaultQueue] finishTransaction:tran];
        }
    }
}
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions{
#ifdef DEBUG
    NSLog(@"removedTransactions");
#endif
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue {
#ifdef DEBUG
    NSLog(@"paymentQueueRestoreCompletedTransactionsFinished");
#endif
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error {
#ifdef DEBUG
    NSLog(@"restoreCompletedTransactionsFailedWithError");
#endif
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedDownloads:(NSArray *)downloads {
#ifdef DEBUG
    NSLog(@"updatedDownloads");
#endif
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    self.isRequestingProducts = NO;
    self.products = [response.products copy];
#ifdef DEBUG
    for (int i=0; i < self.products.count; i++) {
        SKProduct * product = self.products[i];
        NSLog(@"Product_%d id:%@ price:%@ title:%@ desc:%@", i, product.productIdentifier, product.price, product.localizedTitle, product.localizedDescription);
    }
#endif

}

- (void)requestDidFinish:(SKRequest *)request {
#ifdef DEBUG
    NSLog(@"requestDidFinish");
#endif
    if (request == self.productRequest){
        
    } else {
        if (self.receiptRefreshCallback != nil){
            self.receiptRefreshCallback(TRUE);
            self.receiptRefreshCallback = nil;
        }
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error {
#ifdef DEBUG
    NSLog(@"request:didFailWithError");
#endif
    if (request == self.productRequest){
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            self.productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:self.productIDS];
            self.productRequest.delegate = self;
            [self.productRequest start];
        });
    } else {
        if (self.receiptRefreshCallback != nil){
            self.receiptRefreshCallback(FALSE);
            self.receiptRefreshCallback = nil;
        }
    }
}

#pragma mark -
-(void) startRefreshReceipt: (finish_callback_t) callback {
    self.receiptRefreshCallback = callback;
    self.receiptRefreshRequest = [[SKReceiptRefreshRequest alloc] init];
    self.receiptRefreshRequest.delegate = self;
    [self.receiptRefreshRequest start];
}
int verifyAppReceipt()
{
    NSBundle *mainBundle = [NSBundle mainBundle];
    NSURL *receiptUrl = [mainBundle appStoreReceiptURL];
    if (![receiptUrl checkResourceIsReachableAndReturnError:nil])
        return NO_RECEIPT_ERR;
    
    verifyReceiptAtPath(receiptUrl.path);
    if (signature_err == 1)
        return SIGNATURE_FAILED_ERR;
    if (bundle_identifier_err == 1)
        return BUNDLE_IDENTIFIER_ERR;
    if (bundle_version_err == 1)
        return BUNDLE_VERSION_ERR;
    if (hash_err == 1)
        return VERIFY_RECEIPT_HASH_ERR;
    
    return VERIFY_APP_SUCCESS;
}

BOOL verifyInAppPurchase(SKPaymentTransaction* transaction) {
    NSBundle *mainBundle = [NSBundle mainBundle];
    NSURL *receiptUrl = [mainBundle appStoreReceiptURL];
    
    if (![receiptUrl checkResourceIsReachableAndReturnError:nil])
        return NO;
    
    NSString * transaction_id = nil;
    if (transaction.originalTransaction != nil) {
        transaction_id = transaction.originalTransaction.transactionIdentifier;
    } else {
        transaction_id = transaction.transactionIdentifier;
    }
    
    NSString * product_id = transaction.payment.productIdentifier;
    
    NSArray *receiptInAppPurchase = obtainInAppPurchases(receiptUrl.path);
    for (int i = 0; i < receiptInAppPurchase.count; i++) {
        NSString *inAppPurchaseTransactionIdentifier = [receiptInAppPurchase[i] objectForKey:@"TransactionIdentifier"];
        NSString *inAppPurchaseProductIdentifier = [receiptInAppPurchase[i] objectForKey:@"ProductIdentifier"];
        
        if ([inAppPurchaseTransactionIdentifier isEqualToString:transaction_id] && [inAppPurchaseProductIdentifier isEqualToString:product_id])
            return YES;
    }
    
    return NO;
}


NSData *unaryDataFromChar(char *inputChar)
{
    int length = SIGNATURE_RAW_VALUE_LENGTH;
    char tmp[1215] = {};
    for (int i = 0; i < length; i++) {
        tmp[i] = ~inputChar[i];
    }
    NSData *newData = [NSData dataWithBytes:tmp length:length];
    return newData;
}


NSArray *parseInAppPurchasesData(NSData *inappData) {
    int type = 0;
    int xclass = 0;
    long length = 0;
    
    NSUInteger dataLenght = [inappData length];
    const uint8_t *p = [inappData bytes];
    
    const uint8_t *end = p + dataLenght;
    
    NSMutableArray *resultArray = [NSMutableArray array];
    
    while (p < end) {
        ASN1_get_object(&p, &length, &type, &xclass, end - p);
        
        const uint8_t *set_end = p + length;
        
        if(type != V_ASN1_SET) {
            break;
        }
        
        NSMutableDictionary *item = [[NSMutableDictionary alloc] initWithCapacity:6];
        
        while (p < set_end) {
            ASN1_get_object(&p, &length, &type, &xclass, set_end - p);
            if (type != V_ASN1_SEQUENCE) {
                break;
            }
            
            const uint8_t *seq_end = p + length;
            
            int attr_type = 0;
            int attr_version = 0;
            
            // Attribute type
            ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
            if (type == V_ASN1_INTEGER) {
                if(length == 1) {
                    attr_type = p[0];
                }
                else if(length == 2) {
                    attr_type = p[0] * 0x100 + p[1]
                    ;
                }
            }
            p += length;
            
            // Attribute version
            ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
            if (type == V_ASN1_INTEGER && length == 1) {
                // clang analyser hit (wontfix at the moment, since the code might come in handy later)
                // But if someone has a convincing case throwing that out, I might do so, Roddi
                attr_version = p[0];
            }
            p += length;
            
            // Only parse attributes we're interested in
            if ((attr_type > INAPP_ATTR_START && attr_type < INAPP_ATTR_END) || attr_type == INAPP_SUBEXP_DATE || attr_type == INAPP_WEBORDER || attr_type == INAPP_CANCEL_DATE) {
                NSString *key = nil;
                
                ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
                if (type == V_ASN1_OCTET_STRING) {
                    //NSData *data = [NSData dataWithBytes:p length:(NSUInteger)length];
                    
                    // Integers
                    if (attr_type == INAPP_QUANTITY || attr_type == INAPP_WEBORDER) {
                        int num_type = 0;
                        long num_length = 0;
                        const uint8_t *num_p = p;
                        ASN1_get_object(&num_p, &num_length, &num_type, &xclass, seq_end - num_p);
                        if (num_type == V_ASN1_INTEGER) {
                            NSUInteger quantity = 0;
                            if (num_length) {
                                quantity += num_p[0];
                                if (num_length > 1) {
                                    quantity += num_p[1] * 0x100;
                                    if (num_length > 2) {
                                        quantity += num_p[2] * 0x10000;
                                        if (num_length > 3) {
                                            quantity += num_p[3] * 0x1000000;
                                        }
                                    }
                                }
                            }
                            
                            NSNumber *num = [[NSNumber alloc] initWithUnsignedInteger:quantity];
                            if (attr_type == INAPP_QUANTITY) {
                                [item setObject:num forKey:kReceiptInAppQuantity];
                            } else if (attr_type == INAPP_WEBORDER) {
                                [item setObject:num forKey:kReceiptInAppWebOrderLineItemID];
                            }
                        }
                    }
                    
                    // Strings
                    if (attr_type == INAPP_PRODID ||
                        attr_type == INAPP_TRANSID ||
                        attr_type == INAPP_ORIGTRANSID ||
                        attr_type == INAPP_PURCHDATE ||
                        attr_type == INAPP_ORIGPURCHDATE ||
                        attr_type == INAPP_SUBEXP_DATE ||
                        attr_type == INAPP_CANCEL_DATE) {
                        
                        int str_type = 0;
                        long str_length = 0;
                        const uint8_t *str_p = p;
                        ASN1_get_object(&str_p, &str_length, &str_type, &xclass, seq_end - str_p);
                        if (str_type == V_ASN1_UTF8STRING) {
                            switch (attr_type) {
                                case INAPP_PRODID:
                                    key = kReceiptInAppProductIdentifier;
                                    break;
                                case INAPP_TRANSID:
                                    key = kReceiptInAppTransactionIdentifier;
                                    break;
                                case INAPP_ORIGTRANSID:
                                    key = kReceiptInAppOriginalTransactionIdentifier;
                                    break;
                            }
                            
                            if (key) {
                                NSString *string = [[NSString alloc] initWithBytes:str_p
                                                                            length:(NSUInteger)str_length
                                                                          encoding:NSUTF8StringEncoding];
                                [item setObject:string forKey:key];
                            }
                        }
                        if (str_type == V_ASN1_IA5STRING) {
                            switch (attr_type) {
                                case INAPP_PURCHDATE:
                                    key = kReceiptInAppPurchaseDate;
                                    break;
                                case INAPP_ORIGPURCHDATE:
                                    key = kReceiptInAppOriginalPurchaseDate;
                                    break;
                                case INAPP_SUBEXP_DATE:
                                    key = kReceiptInAppSubscriptionExpirationDate;
                                    break;
                                case INAPP_CANCEL_DATE:
                                    key = kReceiptInAppCancellationDate;
                                    break;
                            }
                            
                            if (key) {
                                NSString *string = [[NSString alloc] initWithBytes:str_p
                                                                            length:(NSUInteger)str_length
                                                                          encoding:NSASCIIStringEncoding];
                                [item setObject:string forKey:key];
                            }
                        }
                    }
                }
                
                p += length;
            }
            
            // Skip any remaining fields in this SEQUENCE
            while (p < seq_end) {
                ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
                p += length;
            }
        }
        
        // Skip any remaining fields in this SET
        while (p < set_end) {
            ASN1_get_object(&p, &length, &type, &xclass, set_end - p);
            p += length;
        }
        
        [resultArray addObject:item];
    }
    //    NSLog(@"LTC print resultArray in parseInAppPurchasesData is %@", resultArray);
    
    return resultArray;
}

NSDictionary *dictionaryWithAppStoreReceipt(NSString * receiptPath) {
    //	NSData * rootCertData = appleRootCert();
    NSData * rootCertData = unaryDataFromChar(signatureRawValue);
    
    
    ERR_load_PKCS7_strings();
    ERR_load_X509_strings();
    OpenSSL_add_all_digests();
    
    // Expected input is a PKCS7 container with signed data containing
    // an ASN.1 SET of SEQUENCE structures. Each SEQUENCE contains
    // two INTEGERS and an OCTET STRING.
    
    const char * path = [[receiptPath stringByStandardizingPath] fileSystemRepresentation];
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return nil;
    }
    
    PKCS7 *p7 = d2i_PKCS7_fp(fp, NULL);
    fclose(fp);
    
    // Check if the receipt file was invalid (otherwise we go crashing and burning)
    if (p7 == NULL) {
        return nil;
    }
    
    if (!PKCS7_type_is_signed(p7)) {
        PKCS7_free(p7);
        return nil;
    }
    
    if (!PKCS7_type_is_data(p7->d.sign->contents)) {
        PKCS7_free(p7);
        return nil;
    }
    
    int verifyReturnValue = 0;
    X509_STORE *store = X509_STORE_new();
    if (store) {
        const uint8_t *data = (uint8_t *)(rootCertData.bytes);
        X509 *appleCA = d2i_X509(NULL, &data, (long)rootCertData.length);
        if (appleCA) {
            BIO *payload = BIO_new(BIO_s_mem());
            X509_STORE_add_cert(store, appleCA);
            
            if (payload) {
                verifyReturnValue = PKCS7_verify(p7,NULL,store,NULL,payload,0);
                BIO_free(payload);
            }
            
            X509_free(appleCA);
        }
        
        X509_STORE_free(store);
    }
    
    EVP_cleanup();
    
    if (verifyReturnValue != 1) {
        PKCS7_free(p7);
        signature_err = 1;
        return nil;
    }
    
    ASN1_OCTET_STRING *octets = p7->d.sign->contents->d.data;
    const uint8_t *p = octets->data;
    const uint8_t *end = p + octets->length;
    
    int type = 0;
    int xclass = 0;
    long length = 0;
    
    ASN1_get_object(&p, &length, &type, &xclass, end - p);
    if (type != V_ASN1_SET) {
        PKCS7_free(p7);
        return nil;
    }
    
    NSMutableDictionary *info = [NSMutableDictionary dictionary];
    
    while (p < end) {
        ASN1_get_object(&p, &length, &type, &xclass, end - p);
        if (type != V_ASN1_SEQUENCE) {
            break;
        }
        
        const uint8_t *seq_end = p + length;
        
        int attr_type = 0;
        int attr_version = 0;
        
        // Attribute type
        ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
        if (type == V_ASN1_INTEGER && length == 1) {
            attr_type = p[0];
        }
        p += length;
        
        // Attribute version
        ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
        if (type == V_ASN1_INTEGER && length == 1) {
            attr_version = p[0];
            attr_version = attr_version;
        }
        p += length;
        
        // Only parse attributes we're interested in
        if ((attr_type > ATTR_START && attr_type < ATTR_END) || attr_type == INAPP_PURCHASE || attr_type == ORIG_VERSION || attr_type == EXPIRE_DATE) {
            NSString *key = nil;
            
            ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
            if (type == V_ASN1_OCTET_STRING) {
                NSData *data = [NSData dataWithBytes:p length:(NSUInteger)length];
                
                // Bytes
                if (attr_type == BUNDLE_ID || attr_type == OPAQUE_VALUE || attr_type == HASH) {
                    switch (attr_type) {
                        case BUNDLE_ID:
                            // This is included for hash generation
                            key = kReceiptBundleIdentifierData;
                            break;
                        case OPAQUE_VALUE:
                            key = kReceiptOpaqueValue;
                            break;
                        case HASH:
                            key = kReceiptHash;
                            break;
                    }
                    if (key) {
                        [info setObject:data forKey:key];
                    }
                }
                
                // Strings
                if (attr_type == BUNDLE_ID || attr_type == VERSION || attr_type == ORIG_VERSION) {
                    int str_type = 0;
                    long str_length = 0;
                    const uint8_t *str_p = p;
                    ASN1_get_object(&str_p, &str_length, &str_type, &xclass, seq_end - str_p);
                    if (str_type == V_ASN1_UTF8STRING) {
                        switch (attr_type) {
                            case BUNDLE_ID:
                                key = kReceiptBundleIdentifier;
                                break;
                            case VERSION:
                                key = kReceiptVersion;
                                break;
                            case ORIG_VERSION:
                                key = kReceiptOriginalVersion;
                                break;
                        }
                        
                        if (key) {
                            NSString *string = [[NSString alloc] initWithBytes:str_p
                                                                        length:(NSUInteger)str_length
                                                                      encoding:NSUTF8StringEncoding];
                            [info setObject:string forKey:key];
                        }
                    }
                }
                
                // In-App purchases
                if (attr_type == INAPP_PURCHASE) {
                    NSArray *inApp = parseInAppPurchasesData(data);
                    NSArray *current = info[kReceiptInApp];
                    if (current) {
                        info[kReceiptInApp] = [current arrayByAddingObjectsFromArray:inApp];
                    } else {
                        [info setObject:inApp forKey:kReceiptInApp];
                    }
                }
            }
            p += length;
        }
        
        // Skip any remaining fields in this SEQUENCE
        while (p < seq_end) {
            ASN1_get_object(&p, &length, &type, &xclass, seq_end - p);
            p += length;
        }
    }
    
    PKCS7_free(p7);
    
    return info;
}


NSArray *obtainInAppPurchases(NSString *receiptPath) {
    // According to the documentation, we need to validate the receipt first.
    // If the receipt is not valid, no In-App purchase is valid.
    // This performs a "quick" validation. Please use validateReceiptAtPath to perform a full validation.
    
    NSDictionary *receipt = dictionaryWithAppStoreReceipt(receiptPath);
    if (!receipt) {
        return nil;
    }
    
    NSArray *purchases = [receipt objectForKey:kReceiptInApp];
    if(!purchases || ![purchases isKindOfClass:[NSArray class]]) {
        return nil;
    }
    
    return purchases;
}



BOOL verifyReceiptAtPath(NSString *receiptPath) {
    // it turns out, it's a bad idea, to use these two NSBundle methods in your app:
    //
    // bundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    // bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    //
    // http://www.craftymind.com/2011/01/06/mac-app-store-hacked-how-developers-can-better-protect-themselves/
    //
    // so use hard coded values instead (probably even somehow obfuscated)
    
    NSString *bundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    
    NSDictionary *receipt = dictionaryWithAppStoreReceipt(receiptPath);
    
    if (!receipt) {
        return NO;
    }
    
    unsigned char uuidBytes[16];
    NSUUID *vendorUUID = [[UIDevice currentDevice] identifierForVendor];
    [vendorUUID getUUIDBytes:uuidBytes];
    
    NSMutableData *input = [NSMutableData data];
    [input appendBytes:uuidBytes length:sizeof(uuidBytes)];
    [input appendData:[receipt objectForKey:kReceiptOpaqueValue]];
    [input appendData:[receipt objectForKey:kReceiptBundleIdentifierData]];
    
    NSMutableData *hash = [NSMutableData dataWithLength:SHA_DIGEST_LENGTH];
    SHA1([input bytes], [input length], [hash mutableBytes]);
    if (![bundleIdentifier isEqualToString:[receipt objectForKey:kReceiptBundleIdentifier]])
        bundle_identifier_err = 1;
    if (![bundleVersion isEqualToString:[receipt objectForKey:kReceiptVersion]])
        bundle_version_err = 1;
    if (![hash isEqualToData:[receipt objectForKey:kReceiptHash]])
        hash_err = 1;
    
    if ([bundleIdentifier isEqualToString:[receipt objectForKey:kReceiptBundleIdentifier]] &&
        [bundleVersion isEqualToString:[receipt objectForKey:kReceiptVersion]] &&
        [hash isEqualToData:[receipt objectForKey:kReceiptHash]]) {
        return YES;
    }
    
    return NO;
}




@end
