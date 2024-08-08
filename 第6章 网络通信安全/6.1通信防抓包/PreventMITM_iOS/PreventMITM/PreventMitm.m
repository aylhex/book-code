//
//  PreventMitm.m
//  PreventMITM
//
//  Created by Ann on 2022/5/17.
//

#import "PreventMitm.h"
#import "AFNetworking/AFNetworking.h"

@implementation PreventMitm

//获取代理信息
-(BOOL) isProxy
{
    NSDictionary *proxySettings = (__bridge NSDictionary *)(CFNetworkCopySystemProxySettings());
    NSArray *proxies = (__bridge NSArray *)(CFNetworkCopyProxiesForURL((__bridge CFURLRef _Nonnull)([NSURL URLWithString:@"http://www.baidu.com"]), (__bridge CFDictionaryRef _Nonnull)(proxySettings)));
    
    NSDictionary *settings = [proxies objectAtIndex:0];
    
    if ([[settings objectForKey:(NSString *)kCFProxyTypeKey] isEqualToString:@"kCFProxyTypeNone"])
    {
        return NO;//没有设置代理
    }
    else
    {
        return YES;//设置代理了
    }
}
// 设置不走系统代理
-(void) setProxy
{
    NSURL *url = [NSURL URLWithString:@"http://www.baidu.com"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    NSURLSessionConfiguration *config = [NSURLSessionConfiguration ephemeralSessionConfiguration];
    config.connectionProxyDictionary = @{}; //置空 不走系统代理
    NSURLSession *urlSession = [NSURLSession sessionWithConfiguration:config];
    
    NSURLSessionDataTask *task = [urlSession dataTaskWithRequest:request];
    [task resume];
}

-(void) testSSLPinning
{
    // 设置URL地址
    NSURL *testURL = [NSURL URLWithString:@"http://www.baidu.com"];
    NSURLSessionConfiguration *seeConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    seeConfig.requestCachePolicy = NSURLRequestReloadIgnoringLocalCacheData;
    NSURLSession *session = [NSURLSession
                             sessionWithConfiguration:seeConfig
                             delegate:self
                             delegateQueue:nil];
    
    // 使用NSURLSessionTask发送请求，
    // 使用dataTaskWithURL:completionHandler:方法来进行SSL pinning 测试
    NSURLSessionDataTask *task =[session dataTaskWithURL:testURL
           completionHandler:^(NSData * _Nullable data,
                               NSURLResponse * _Nullable response,
                               NSError * _Nullable error){}];
    [task resume];
}


-(void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential * _Nullable))completionHandler {
    
    // 获取服务器端证书
    SecTrustRef serverTrust = challenge.protectionSpace.serverTrust;
    SecCertificateRef serverCert = SecTrustGetCertificateAtIndex(serverTrust, 0);
    
    // 主机名检测
    NSMutableArray *policies = [NSMutableArray array];
    [policies addObject:(__bridge_transfer id)SecPolicyCreateSSL(true, (__bridge CFStringRef)challenge.protectionSpace.host)];
    SecTrustSetPolicies(serverTrust, (__bridge CFArrayRef)policies);
    
    // Evaluate server certificate
    SecTrustResultType result;
    SecTrustEvaluate(serverTrust, &result);
    BOOL certificateIsValid = (result == kSecTrustResultUnspecified || result == kSecTrustResultProceed);
    
    // 解析获取本地存储的证书信息和服务端证书信息
    NSData *serverCertData = CFBridgingRelease(SecCertificateCopyData(serverCert));
    NSString *pathToCert = [[NSBundle mainBundle]pathForResource:@"test" ofType:@"cer"];
    NSData *localCertData = [NSData dataWithContentsOfFile:pathToCert];
    
    // 证书校验
    if ([serverCertData isEqualToData:localCertData] && certificateIsValid) {
        NSURLCredential *credential = [NSURLCredential credentialForTrust:serverTrust];
        completionHandler(NSURLSessionAuthChallengeUseCredential, credential);
    } else {
        completionHandler(NSURLSessionAuthChallengeCancelAuthenticationChallenge, NULL);
    }
}

// 发送请求
- (void)sendRequest {
    NSString *testUrl = @"https://github.com/AFNetworking/AFNetworking";
    [self.manager GET:testUrl
           parameters:nil
              headers:nil
             progress:nil
              success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject)
             {
               NSString *str = [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding];
               NSLog(@"%@",str);
             }
             failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error)
             {
               NSLog(@"%@", error);
             }];
}

-(AFHTTPSessionManager *) manager {
    // 设置BaseUrl
    NSURL *baseUrl = [NSURL URLWithString:@"https://github.com"];
    AFHTTPSessionManager *manager =[[AFHTTPSessionManager manager] initWithBaseURL:baseUrl];
    manager.securityPolicy = [self customSecurityPolicy];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"text/html"];
    return manager;
}

// 安全策略
-(AFSecurityPolicy *) customSecurityPolicy {
    
    // 获取证书
    NSString *cerPath = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"cer"];
    NSData *certData = [NSData dataWithContentsOfFile:cerPath];
    NSSet *pinnedCert = [[NSSet alloc] initWithObjects:certData, nil];
    /**
     安全模式
     AFSSLPinningModeNone：        完全信任服务器证书；
     AFSSLPinningModePublicKey：对比服务器证书和本地证书的Public Key，一致则信任服务器证书；
     AFSSLPinningModeCertificate：对比服务器证书和本地证书的所有内容，一致则信任服务器证书
     */
    AFSecurityPolicy *secPolicy =
    [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModePublicKey withPinnedCertificates:pinnedCert];
    /**
     allowInvalidCertificates 是否允许无效证书，默认为NO
     如需要验证自建证书（自建的证书），需要设置为YES
     */
    secPolicy.allowInvalidCertificates = YES;
    /**
    validatesDomainName 是否进行域名校验，默认值为YES
    如证书中的域名与实际请求的域名不一致，需关闭域名校验，即把值设为NO，否则无法通过校验。
     */
    secPolicy.validatesDomainName = YES;
    
    return secPolicy;
}
@end

