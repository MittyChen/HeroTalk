//
//  CKIAPManager.mm
//  CKIAPManager
//
//  Created by chenwei on 15/5/26.
//
//

//网游版本 v1.0

#import "CKIAPManager.h"
#include "Reachability.h" 


#define kVerifyTransactionTimeout 30.0
#define debug_bypassInAppPurchase 0

@class CKIAPManager;



@implementation CKIAPManager

@synthesize enabled = _enabled;
@synthesize purchasing = _purchasing;
@synthesize restoring = _restoring;
@synthesize checkingIDs = _checkingIDs;




#pragma mark -
#pragma constructor/decontructor

- (id)init
{
    self = [super init];
    
    if ( self != nil )
    {
        // Initialization code
        productInfoArray = nil;
        
        _purchasing = NO;
        _restoring = NO;
        _checkingIDs = NO;
        // check to see if In-App purchase is setup so that we can make payments
        _enabled = [SKPaymentQueue canMakePayments];
        
        if ( self.enabled == YES )
        {
            // begin observing updates on the transaction queue immediately
            // this allows us to catch and process any pending transactions
            [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        }
        else
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
                                                            message:@"IAP不可用，请配置以进行应用内购买"
                                                           delegate:self
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
            [alert show];
            [alert release];
            
            
            // log error
            NSLog( @"[InAppPurchase] Error: IAP not configured correctly" );
        }
    }
    
    return self;
}

- (void)dealloc
{
    [productInfoArray release];
    productInfoArray = nil;

    [super dealloc];
}

- (void)checkPaymentIDs:(NSArray*)aPaymentIDs
{
    _checkingIDs = YES;
    //将所有的商品id放进去 在到appstore验证
    NSSet *productids = [NSSet setWithArray:aPaymentIDs];
    skProductRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productids];
    skProductRequest.delegate = self;
    [skProductRequest start];
}


#pragma mark -
#pragma mark Helpers
- (void)refreshEnabledStatus
{
	_enabled = [SKPaymentQueue canMakePayments];
}

#pragma mark -
#pragma mark Request Info
// one or more transactions have been removed from the queue
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
    // purchasing is now complete
    _purchasing = NO;
}

// the payment queue has finished sending restored transactions
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue {
    
    // restoring is now complete
    _restoring = NO;
    
    // note that the logic for restoring purchases is actually handled above in: (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
}

// an error occurred while restoring transactions
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error {
    // restoring is now complete
    _restoring = NO;
    
    // note that the logic for purchase errors is actually handled above in: (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
}

#pragma mark -
#pragma mark Restore Purchases

// restore all previously purchased products
- (void)restorePurchasesTransaction {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
    // is IAP enabled for this app?
    if (self.enabled) {
        // yes, are we already processing a restore?
        if (_restoring == NO) {
            // no, start restoring
            _restoring = YES;
            
            
            // add restore previous purchases to current payment queue
            [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
            
        } else {
            // yes, log error, we can only do 1 restore at a time
            NSLog(@"[InAppPurchase] Already restoring, request ignored");
        }
        
    } else {
        // no ,log error
        NSLog(@"[InAppPurchase] Error: In-App Purchase is not enabled");
    }
    
    [pool drain];
}

///请求商品信息
- (void)purchasesProduct:(NSString*)productID {
    
    //显示菊花
    // add by mcn
    if (!HUD) {
        HUD = [JGProgressHUD progressHUDWithStyle:JGProgressHUDStyleDark];
        HUD.textLabel.text = @"Loading";
    }
    
    UIViewController *vc = [(UINavigationController *)[[UIApplication sharedApplication].delegate window].rootViewController visibleViewController];
    [HUD showInView: vc.view];
    
    
    Reachability* curReach = [Reachability reachabilityForInternetConnection];
    NSParameterAssert([curReach isKindOfClass:[Reachability class]]);
    //监测网络是否可用
    if ([curReach currentReachabilityStatus] == NotReachable) {
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@""
                                                        message:@"网络不可用"
                                                       delegate:self
                                              cancelButtonTitle:@"YES"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
        return;
    }
    //监测 IAP是否可用
    if ([SKPaymentQueue canMakePayments] == NO) {
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
                                                        message:@"IAP不可用"
                                                       delegate:self
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
        return;
    }
    //显示网络连接状态
    [UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
    
    purchasesProductID = productID;
    NSSet *productid = [NSSet setWithObject:productID];
    skProductRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productid];
    skProductRequest.delegate = self;
    [skProductRequest start];
}

//弹出错误信息
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    
    UIAlertView *alerView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",NULL)
                                                       message:[error localizedDescription]
                                                      delegate:nil cancelButtonTitle:NSLocalizedString(@"Close",nil) otherButtonTitles:nil];
    [alerView show];
    [alerView release];
    
    //隐藏网络连接状态
    [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
}

#pragma mark -
#pragma mark SKProductsRequestDelegate methods
- (void)productsRequest:(SKProductsRequest*)request didReceiveResponse:(SKProductsResponse*)response {
    if (response == nil) {
        NSLog(@"Product Response is nil");
    } else {
        
        if(_checkingIDs)
        {
            //实现检查失败的游戏逻辑 设定无法支付等等
            
            
            for (NSString *identifier in response.invalidProductIdentifiers) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
                                                                message:@"商品列表错误"
                                                               delegate:self
                                                      cancelButtonTitle:@"OK"
                                                      otherButtonTitles:nil];
                [alert show];
                [alert release];
            
            }
            _checkingIDs = NO;
            return;
        }
        
    
        
        for (NSString *identifier in response.invalidProductIdentifiers) {
            NSLog(@"商品 ： %@ 不存在，请核对iTunesConnect配置 ",identifier);
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
                                                            message:@"错误的商品"
                                                           delegate:self
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
            [alert show];
            [alert release];
        }
        NSLog(@"Products count in received response: %d", [response.products count]);
        
        [productInfoArray release];
        productInfoArray = nil;
        
        productInfoArray = [[NSArray alloc] initWithArray:response.products];
        
        for (SKProduct *product in response.products){
            
            if([product.productIdentifier isEqualToString:purchasesProductID]) {
                SKPayment *payment = [SKPayment paymentWithProduct:product];
                [[SKPaymentQueue defaultQueue] addPayment:payment];
                break;
            }
        }
    }
    
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    BOOL purchasing = YES;
    
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
                // 购买中
            case SKPaymentTransactionStatePurchasing:
            {
                purchasing = YES;
                break;
            }
                // 购买成功
            case SKPaymentTransactionStatePurchased:
            {
                //隐藏菊花
                 [HUD dismissAfterDelay:1];
                
                purchasing = NO;
                
                
                // 购买失败的回调实现 向服务器发起验证
                
            }
            case SKPaymentTransactionStateFailed: {
                 //隐藏菊花
                
                
                purchasing = NO;
                [queue  finishTransaction:transaction];
                 [HUD dismissAfterDelay:1];
                
                //购买失败的游戏逻辑
                
                
                break;
            } 
                
                // 购买纪录恢复（购买过）
            case SKPaymentTransactionStateRestored: {
                 //隐藏菊花
                
                 [HUD dismissAfterDelay:1];
                purchasing = NO;
                [queue finishTransaction:transaction];
                //恢复购买的游戏逻辑
                
                
                break;
            }
                
                
            default:{
                break;
            }
                
                
         
        }
    
    }
        //隐藏网络连接状态
    if (purchasing == NO)
    {
        [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
    }
     
}

- (NSArray*)getProductInfoArray {
    return productInfoArray;
}

@end
