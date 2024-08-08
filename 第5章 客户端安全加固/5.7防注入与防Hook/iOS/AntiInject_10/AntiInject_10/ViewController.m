//
//  ViewController.m
//  AntiInject_10
//
//  Created by Ann on 2023/1/14.
//  Copyright © 2023 Ann. All rights reserved.
//

#import "ViewController.h"
#import "AntiInject.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (IBAction)checkInject:(UIButton *)sender {
    
    char *env = getenv("DYLD_INSERT_LIBRARIES");
    NSLog(@"Lib path === %s",env);
//    AntiInject* antiInject = [[AntiInject alloc] init];
//
//    [antiInject check_inject_by_env];
    //[antiInject checkWhiteList];
}

@end
