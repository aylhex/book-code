//
//  ViewController.m
//  jsbridge
//
//  Created by AYL on 2022/11/18.
//

#import "ViewController.h"
#import "WKWebViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initUI];
    // Do any additional setup after loading the view.
}

- (void)initUI {
    
    self.title = @"JsBridge";
    self.view.backgroundColor = [UIColor whiteColor];
    
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    [button setTitle:@"JsBridge 测试" forState:UIControlStateNormal];
    button.backgroundColor = [UIColor blueColor];
    [self.view addSubview:button];
    [button sizeToFit];
    button.center = self.view.center;
    [button addTarget:self action:@selector(jsbrdageBtnClicked:) forControlEvents:UIControlEventTouchUpInside];

}

- (void)jsbrdageBtnClicked:(UIButton *)sender {

    WKWebViewController *webview = [[WKWebViewController alloc] init];
    [self.navigationController pushViewController:webview animated:YES];
}


@end
