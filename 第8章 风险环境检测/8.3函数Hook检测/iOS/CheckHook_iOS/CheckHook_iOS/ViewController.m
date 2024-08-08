//
//  ViewController.m
//  CheckHook_iOS
//
//  Created by AYL on 2023/2/6.
//

#import "ViewController.h"
#import "CheckHook.h"
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



void testMSHook()
{
    NSLog(@"Hello Substrate Hook");
}

- (IBAction)detectHook:(id)sender {
    
    //[self testSwizzle];
    [self testFishHook];
    
//    BOOL result = checkSwizzleHook("ViewController", "testSwizzle");
//    if (result) {
//        NSLog(@"Method is hooked");
//    } else {
//        NSLog(@"Method is not hooked");
//    }
//
//
//    BOOL ret = checkMSHook(testMSHook);
//    if (ret) {
//        NSLog(@"Method is hooked");
//    } else {
//        NSLog(@"Method is not hooked");
//    }
    
    BOOL ret = checkHook("open");
    if (ret) {
        NSLog(@"Method is hooked");
    } else {
        NSLog(@"Method is not hooked");
    }
}
@end
