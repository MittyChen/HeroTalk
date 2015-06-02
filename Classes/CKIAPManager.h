//
//  CKIAPManager.h
//  CKIAPManager
//
//  Created by chenwei on 15/5/26.
//
//

//网游版本 v1.0

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>


@interface CKIAPManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    NSString* purchasesProductID;
    NSArray* productInfoArray;
    SKProductsRequest* skProductRequest;
}

// informational variables
@property (nonatomic, readonly) BOOL enabled; // IAP is configured correctly within the app
@property (nonatomic, readonly) BOOL purchasing; // currently in the purchase process
@property (nonatomic, readonly) BOOL restoring; // currently in the restore process
@property (nonatomic, readonly) BOOL checkingIDs; // currently in the restore process


//检查商品列表
- (void)checkPaymentIDs:(NSArray*)aPaymentIDs;

//更新可支付状态
- (void)refreshEnabledStatus;

//开始购买
- (void)purchasesProduct:(NSString*)productID;

//恢复购买
- (void)restorePurchasesTransaction; // restore all previously purchased products

//订单主循环
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;

//收到订单信息
- (void)productsRequest:(SKProductsRequest*)request didReceiveResponse:(SKProductsResponse*)response ;

//弹出错误信息
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error;

//获取订单信息
- (NSArray*)getProductInfoArray;
@end
