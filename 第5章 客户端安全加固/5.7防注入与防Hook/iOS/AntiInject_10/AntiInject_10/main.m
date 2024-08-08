//
//  main.m
//  AntiInject_10
//
//  Created by Ann on 2023/1/14.
//  Copyright © 2023 Ann. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[]) {
    
    char *env = getenv("DYLD_INSERT_LIBRARIES");
    NSLog(@"Lib path === %s",env);
    
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
