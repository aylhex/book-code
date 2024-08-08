#import <UIKit/UIKit.h>
%hook ViewController            // 要hook的类
- (void)testCydiaSubstrate      // 要hook的方法
{    
    NSLog(@"This func has been hooked by Cydia Substrate!");
    %orig;                      // 执行结束后调用原函数
}
%end