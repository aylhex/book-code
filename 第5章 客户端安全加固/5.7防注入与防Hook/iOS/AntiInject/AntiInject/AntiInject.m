//
//  AntiInject.m
//  AntiInject
//
//  Created by AYL on 2023/1/12.
//

#import "AntiInject.h"
#include <stdio.h>
#include <mach-o/dyld.h>

@implementation AntiInject

-(void) check_inject_by_env
{
    char *env = getenv("DYLD_INSERT_LIBRARIES");
    NSLog(@"Lib path === %s",env);
    if(env != NULL)
    {
        exit(0);
    }
}

bool check_block_list(const char* libpath)
{
    // 黑名单示例
    char* black_list[2] = {"/var/containers/Bundle/Application","/system"};
    
    for(int i = 0; i <2; i++)
    {
        if(strnstr(libpath, black_list[i], strlen(black_list[i])) != 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

-(void) check_inject_by_block_list
{
    int count = _dyld_image_count();
    for (int i = 0; i < count; i++) {
       const char * libPath = _dyld_get_image_name(i);
        // 判断加载的动态库是否包含黑名单
        if(check_block_list(libPath))
        {
            exit(0);
        }
    }
}


@end

