//
//  main.m
//  AntiInject
//
//  Created by AYL on 2023/1/12.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void scan()
{
    Dl_info info;
    void *handle = dlopen(NULL, RTLD_NOW);
    while (handle) {
        if (dladdr(handle, &info)) {
            NSLog(@"%s\n", info.dli_fname);
        }
        handle = dlopen(NULL, RTLD_NEXT);
    }
}

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
