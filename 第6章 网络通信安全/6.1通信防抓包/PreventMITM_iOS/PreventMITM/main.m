//
//  main.m
//  PreventMITM
//
//  Created by TAL on 2022/5/13.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "PreventMitm.h"


//

//
//// 代理回调
//URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential * _Nullable))completionHandler {
//
//    // 得到远程证书
//    SecTrustRef serverTrust = challenge.protectionSpace.serverTrust;
//    SecCertificateRef certificate = SecTrustGetCertificateAtIndex(serverTrust, 0);
//
//    // 设置ssl政策来检测主域名
//    NSMutableArray *policies = [NSMutableArray array];
//    [policies addObject:(__bridge_transfer id)SecPolicyCreateSSL(true, (__bridge CFStringRef)challenge.protectionSpace.host)];
//
//    // 验证服务器证书
//    SecTrustResultType result;
//    SecTrustEvaluate(serverTrust, &result);
//    BOOL certificateIsValid =
//    (result == kSecTrustResultUnspecified || result == kSecTrustResultProceed);
//
//    // 得到远程和本地证书data
//    NSData *remoteCertificateData = CFBridgingRelease(SecCertificateCopyData(certificate));
//    NSString *pathToCert = [[NSBundle mainBundle] pathForResource:@"github2018" ofType:@"cer"];
//    NSData *localCertificate = [NSData dataWithContentsOfFile:pathToCert];
//
//    // 检查
//    if (certificateIsValid && [remoteCertificateData isEqualToData:localCertificate]) {
//        // 验证通过
//        NSURLCredential *credential = [NSURLCredential credentialForTrust:serverTrust];
//        completionHandler(NSURLSessionAuthChallengeUseCredential,credential);
//    }else {
//        // 验证不通过
//        completionHandler(NSURLSessionAuthChallengeCancelAuthenticationChallenge,NULL);
//    }
//}
//
//
//
//AFHTTPSessionManager *manager()
//{
//    static AFHTTPSessionManager *manager = nil;
//    static dispatch_once_t onceToken;
//    dispatch_once(&onceToken, ^{
//
//        //1.创建manager对象
//        NSURLSessionConfiguration *config = [NSURLSessionConfiguration defaultSessionConfiguration];
//        manager =  [[AFHTTPSessionManager alloc] initWithBaseURL:[NSURL URLWithString:@"你的访问的地址的domian"] sessionConfiguration:config];
//        //2.设置接收的response类型
//        [[manager responseSerializer]setAcceptableContentTypes:[NSSet setWithObjects:@"application/json",@"text/plain",@"text/html", nil]];
//
//        //3.https 证书配置
//        //3.1 将证书拖进项目
//        //3.2 获取证书路径
//        NSString *certPath = [[NSBundle mainBundle] pathForResource:@"你的证书名字" ofType:@"cer"];
//        //3.3 获取证书data
//        NSData *certData = [NSData dataWithContentsOfFile:certPath];
//        //3.4 创建AFN 中的securityPolicy
//        AFSecurityPolicy *securityPolicy = [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModePublicKey withPinnedCertificates
//                                                                                  :[[NSSet alloc] initWithObjects:certData,nil]];
//        //3.5 这里就可以添加多个server证书
//        NSSet *dataSet = [[NSSet alloc]initWithObjects:certData, nil];
//        //3.6 绑定证书（不止一个证书）
//        [securityPolicy setPinnedCertificates:dataSet];
//        //3.7 是否允许无效证书
//        [securityPolicy setAllowInvalidCertificates:NO];
//        //3.8 是否需要验证域名
//        /*
//        validatesDomainName 是否需要验证域名，默认为YES；
//        假如证书的域名与你请求的域名不一致，需把该项设置为NO；
//        如设成NO的话，即服务器使用其他可信任机构颁发的证书，也可以建立连接，这个非常危险，建议打开。
//        置为NO，主要用于这种情况：客户端请求的是子域名，而证书上的是另外一个域名。
//        因为SSL证书上的域名是独立的，假如证书上注册的域名是www.google.com，那么mail.google.com是无法验证通过的；
//        当然，有钱可以注册通配符的域名*.google.com，但这个还是比较贵的。
//        如置为NO，建议自己添加对应域名的校验逻辑。
//         */
//        [securityPolicy setValidatesDomainName:YES];
//
//        manager.securityPolicy = securityPolicy;
//    });
//    return manager;
//}

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    
    if ([PreventMitm isProxy] == YES) {
        NSLog(@"proxy = %@", @"test");
    }else{
        NSLog(@"proxy = %@", @"test2");
    }
    
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
