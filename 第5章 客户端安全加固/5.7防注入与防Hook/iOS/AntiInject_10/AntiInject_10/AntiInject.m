//
//  AntiInject.m
//  AntiInject
//
//  Created by AYL on 2023/1/12.
//

#import "AntiInject.h"
//#include <stdio.h>
//#include <mach-o/dyld.h>

@implementation AntiInject

-(void) check_inject_by_env
{
    char *env = getenv("DYLD_INSERT_LIBRARIES");
    NSLog(@"Lib path === %s",env);
}


//
//-(bool) checkWhiteList
//{
//    int count = _dyld_image_count();
//    for (int i = 0; i < count; i++) {
//        //遍历拿到库名称！
//       const char * libName = _dyld_get_image_name(i);
//
//        NSLog(@"libName===>\n%s",libName);
//
//
//        //判断是否在白名单内,应用本身的路径是不确定的,所以要除外.
////        if (!strstr(libraries, imageName)&&!strstr(imageName, "/var/mobile/Containers/Bundle/Application")) {
////            printf("该库非白名单之内！！\n%s",imageName);
////            return NO;
////        }
//    }
//    return YES;
//}


@end

