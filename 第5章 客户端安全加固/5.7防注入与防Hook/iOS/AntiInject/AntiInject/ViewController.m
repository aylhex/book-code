//
//  ViewController.m
//  AntiInject
//
//  Created by AYL on 2023/1/12.
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
    
    AntiInject* antiInject = [[AntiInject alloc] init];
    
    [antiInject check_inject_by_env];
    //[antiInject checkWhiteList];
    
    
}

@end
