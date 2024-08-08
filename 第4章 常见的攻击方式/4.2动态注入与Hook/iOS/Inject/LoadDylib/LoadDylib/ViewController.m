//
//  ViewController.m
//  LoadDylib
//
//  Created by Ann on 2022/11/20.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (IBAction)loadDylib:(UIButton *)sender {
    TestDylib *obj = [[TestDylib alloc] init];
    [obj startInject];
}

@end
