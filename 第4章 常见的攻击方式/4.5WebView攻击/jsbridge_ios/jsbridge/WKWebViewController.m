//
//  WebView.m
//  jsbridge
//
//  Created by AYL on 2022/11/17.
//

#import <WebKit/WebKit.h>
#import "WKWebViewController.h"


@interface WKWebViewController ()<WKNavigationDelegate,WKUIDelegate>

@property (strong, nonatomic) WKWebView *webView;

@end

@implementation WKWebViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    [self initWKWebView];
    
}

- (void)initWKWebView
{
    
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];

    WKPreferences *preferences = [WKPreferences new];
    preferences.javaScriptCanOpenWindowsAutomatically = YES;
    preferences.minimumFontSize = 30.0;
    configuration.preferences = preferences;

    self.webView = [[WKWebView alloc] initWithFrame:self.view.frame configuration:configuration];
    
    
    
    self.webView.navigationDelegate = self;
    self.webView.UIDelegate = self;
    
    [self.view addSubview:self.webView];

    
    //NSString *urlStr = @"http://www.baidu.com;
    NSString *urlStr = @"jsbridge://www.baidu.com/alert";
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:urlStr]];
    [self.webView loadRequest:request];
    
//    NSString *urlStr = [[NSBundle mainBundle] pathForResource:@"index.html" ofType:nil];
//    NSURL *fileURL = [NSURL fileURLWithPath:urlStr];
//    [self.webView loadFileURL:fileURL allowingReadAccessToURL:fileURL];
//

}

#pragma mark - private method

- (void)showAlert
{
    
     UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"JSBridge" message:@"正在进行JSBridge测试" preferredStyle:UIAlertControllerStyleAlert];

     UIAlertAction *conform = [UIAlertAction actionWithTitle:@"确认" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
         NSLog(@"点击了确认按钮");
     }];

     UIAlertAction *cancel = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
         NSLog(@"点击了取消按钮");
     }];
  
     [alert addAction:conform];
     [alert addAction:cancel];
     
     [self presentViewController:alert animated:YES completion:nil];
}




#pragma mark - WKNavigationDelegate
- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
    NSURL *URL = navigationAction.request.URL;
    NSString *scheme = [URL scheme];

    if ([scheme isEqualToString:@"jsbridge"]) {
        NSString *method = [URL path];
        if([method isEqualToString:@"/alert"]){
            [self showAlert];
            
        }
    }
    decisionHandler(WKNavigationActionPolicyAllow);
}

#pragma mark - WKUIDelegate
- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(void))completionHandler
{

    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提醒" message:message preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"知道了" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        completionHandler();
    }]];

    [self presentViewController:alert animated:YES completion:nil];
}

@end
