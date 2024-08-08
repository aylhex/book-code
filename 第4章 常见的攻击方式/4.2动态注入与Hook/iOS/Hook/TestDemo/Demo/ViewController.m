//
//  ViewController.m
//  Demo
//
//  Created by AYL on 2022/12/19.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}
- (void)testSwizzle{
    NSLog(@"Hello Swizzle");
}

- (void)testFishHook{
    int fd = open("/Library/MobileSubstrate/DynamicLibraries/Swizzle.dylib", O_RDONLY);
    uint32_t magic_number = 0;
    read(fd, &magic_number, 4);
    NSLog(@"Mach-O Magic Number: %x \n", magic_number);
    close(fd);
}

- (void)testCydiaSubstrate{
    NSLog(@"Hello Cydia Substrate");
}

- (IBAction)testHook:(UIButton *)sender {
    [self testSwizzle];
//    [self testFishHook];
//    [self testCydiaSubstrate];
}
@end
