//
//  ViewController.m
//  antidebug
//
//  Created by AYL on 2023/1/5.
//

#import "ViewController.h"
#import "AntiDebug.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (IBAction)checkDebug:(UIButton *)sender {
    AntiDebug* antidebug = [[AntiDebug alloc] init];
    [antidebug check_debug_by_ioctl];
}

@end
