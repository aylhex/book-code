//
//  CheckHook.h
//  CheckHook_iOS
//
//  Created by AYL on 2023/2/6.
//

#import <Foundation/Foundation.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <objc/objc.h>
#include <objc/runtime.h>
#include <mach-o/dyld.h>

NS_ASSUME_NONNULL_BEGIN

@interface CheckHook : NSObject
bool checkMSHook(void* method_name);
bool checkFishHook(char *functionName);
bool checkSwizzleHook(const char* clsname,const char* methodname);
bool checkHook(const char* method_name);
@end

NS_ASSUME_NONNULL_END
