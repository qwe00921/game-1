//
//  ViewController.swift
//  iap1
//
//  Created by apache on 15/3/11.
//  Copyright (c) 2015年 apache. All rights reserved.
//

import UIKit
import StoreKit

//class ViewController: UIViewController, SKProductsRequestDelegate, SKRequestDelegate {
class ViewController: UIViewController, TrgameIAPDelegate {
    //var productID:NSSet!@IBOutlet var view: UIView!
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    var request:SKReceiptRefreshRequest? = nil
    var productID:NSString!
    var iap = TrgameIAP()
    
    func enterProcessing() {
        spinner.hidden = false
        spinner.startAnimating()
        for i in view.subviews {
            if i is UIButton {
                (i as UIButton).enabled = false
            }
        }
    }
    func endProcessing(){
        spinner.hidden = true
        spinner.stopAnimating()
        for i in view.subviews {
            if i is UIButton {
                (i as UIButton).enabled = true
            }
        }
    }
    @IBAction func purchaseBtnPushed(sender: AnyObject) {
        switch sender.tag {
        case 1: productID = "com.bantu.iap.wuliguo1"
        case 2: productID = "com.bantu.ipa.zhiliguo"
        case 3: productID = "com.bantu.iap.allweapons"
        case 4: productID = "com.bantu.iap.fthj"
        default:
            println("err tag")
            return
        }
        
        if iap.buyItem(productID) {
            enterProcessing()
        } else {
            
        }
        
        //let ids = NSSet(objects: "com.bantu.iap.allweapons", "com.bantu.iap.wuliguo1")
//        validateProductIdentifiers(productID)
    }
    
    @IBAction func restoreBtnPushed(sender: AnyObject) {
//        request = SKReceiptRefreshRequest()
//        request!.delegate = self
//        request!.start()
//        enterProcessing()

    }
    
    func request(request: SKRequest!, didFailWithError error: NSError!) {
        println("request")
        println(request)
        println(error)
        println(error.localizedDescription)
        if request is SKReceiptRefreshRequest{
            endProcessing()
        }
        if error != nil {
            endProcessing()
        }
    }
    func requestDidFinish(request: SKRequest!) {
        println("requestDidFinish")
        println(request)
        if request is SKReceiptRefreshRequest{
            endProcessing()
        }

        //request.cancel()
    }
    override func viewDidLoad() {
        super.viewDidLoad()
//        spinner.hidden = true
//        spinner.stopAnimating()
        iap.setDelegate(self)
        enterProcessing()
        iap.verifyApp()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func buyItemResult(productID: String!, result: Bool) {
        println("buyItem:" + productID + " result:" + result.description)
        endProcessing()
    }
    func verifyAppResult(result: Bool) {
        println("VerfiryAppResult " + result.description)
        if result == true {
            endProcessing()
            iap.initItems(NSSet(objects: "com.bantu.iap.wuliguo1", "com.bantu.ipa.zhiliguo", "com.bantu.iap.allweapons","com.bantu.iap.fthj"));
           
        }
    }

//    func validateProductIdentifiers(productIDS:NSSet)
//    {
//        let productsRequest:SKProductsRequest = SKProductsRequest(productIdentifiers: productIDS)
//        //initWithProductIdentifiers:[NSSet setWithArray:productIdentifiers]];
//        productsRequest.delegate = self;
//        productsRequest.start()
//    }
//    
//    // SKProductsRequestDelegate protocol method
//    //- (void)productsRequest:(SKProductsRequest *)request
//    func productsRequest(request: SKProductsRequest!, didReceiveResponse response: SKProductsResponse!) {
//        for i:SKProduct in response.products as [SKProduct] {
//            println(i.price)
//            println(i.productIdentifier)
//            println(i.localizedDescription)
//            println(i.localizedTitle)
//            
//            let numberFormatter = NSNumberFormatter()
//            numberFormatter.formatterBehavior = .Behavior10_4
//            numberFormatter.numberStyle = .CurrencyStyle
//            numberFormatter.locale = i.priceLocale
//            let formattedPrice = numberFormatter.stringFromNumber(i.price)
//            println(formattedPrice!)
//            
//        }
//        let payment:SKMutablePayment = SKMutablePayment(product: response.products[0] as SKProduct)
//        payment.quantity = 1
//        SKPaymentQueue.defaultQueue().addPayment(payment)
//    }

}

