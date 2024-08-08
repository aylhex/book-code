//
//  AntiDebug.h
//  antidebug
//
//  Created by Ann on 2023/1/8.
//

#import <Foundation/Foundation.h>
#import <dlfcn.h>
#import <sys/types.h>
#import <sys/syscall.h>
#import <sys/sysctl.h>
#import <unistd.h> 
#import <sys/ioctl.h>


NS_ASSUME_NONNULL_BEGIN

@interface AntiDebug : NSObject

-(void) anti_debug_by_ptrace;
-(void) anti_debug_by_syscall;
-(void) anti_debug_by_svc;
-(void) anti_debug_by_svc_syscall;
-(void) check_debug_by_sysctl;
-(void) check_debug_by_isatty;
-(void) check_debug_by_ioctl;

@end

NS_ASSUME_NONNULL_END
