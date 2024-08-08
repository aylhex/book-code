#import "SwizzleHook.h" 
#include <objc/runtime.h> 
#import <UIKit/UIKit.h> 
@implementation SwizzleHook 
static IMP originalImp  = nil;
- (void) showAlert
{ 
    NSLog(@"Im the fake function!!!");
    UIAlertController *alertVC = [UIAlertController alertControllerWithTitle:@"Im the fake function!!!" message:nil preferredStyle:UIAlertControllerStyleAlert]; 
    [alertVC addAction:[UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleCancel handler:nil]]; 
    [[[UIApplication sharedApplication] keyWindow].rootViewController presentViewController:alertVC animated:true completion:nil]; 
    // 伪造方法执行结束以后调用原方法
    ((void(*)(id, SEL))originalImp)(self, @selector(showAlert));
} 
+ (void)load 
{
    // 原方法 
    Method origMethod = class_getInstanceMethod(objc_getClass("ViewController"), @selector(testSwizzle)); 
    // 伪造的方法 
    Method newMethod = class_getInstanceMethod(self, @selector(showAlert)); 
    // 交换两个方法的IMP 
    method_exchangeImplementations(origMethod, newMethod); 
    originalImp = method_getImplementation(newMethod); 
    NSLog(@"This func has been hooked by Swizzle!!!");
} 
@end