//
//  main.m
//  Signature
//
//  Created by Ann on 2022/5/9.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#include <CommonCrypto/CommonDigest.h>

/** 获取文件的md5值*/
NSString* getCodeResourcesMd5()
{
    NSString *newPath = [[NSBundle mainBundle] resourcePath];
    NSString *path = [newPath stringByAppendingString:@"/_CodeSignature/CodeResources"];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSLog(@"path = %@", path);

    if([fileManager fileExistsAtPath:path isDirectory:nil])
    {
        NSData *data = [NSData dataWithContentsOfFile:path];
        unsigned char digest[CC_MD5_DIGEST_LENGTH];
        CC_MD5(data.bytes, (CC_LONG)data.length, digest );
        NSMutableString *result = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
        for( int i = 0; i < CC_MD5_DIGEST_LENGTH; i++ )
        {
            [result appendFormat:@"%02x", digest[i]];

        }
        return result;
    }
    else{
        return nil;
    }
    
}

NSString *getTeamID()
{
    NSDictionary *query = [NSDictionary dictionaryWithObjectsAndKeys:
                           kSecClassGenericPassword, kSecClass,
                           @"bundleSeedID", kSecAttrAccount,
                           @"", kSecAttrService,
                           (id)kCFBooleanTrue, kSecReturnAttributes,
                           nil];
    CFDictionaryRef result = nil;
    OSStatus status = SecItemCopyMatching((CFDictionaryRef)query, (CFTypeRef *)&result);
    if (status == errSecItemNotFound){
        status = SecItemAdd((CFDictionaryRef)query, (CFTypeRef *)&result);
    }
    if (status != errSecSuccess){
        return nil;
    }
    NSString *accessGroup = [(__bridge NSDictionary *)result objectForKey:kSecAttrAccessGroup];
    NSArray *components = [accessGroup componentsSeparatedByString:@"."];
    NSString *teamID = [[components objectEnumerator] nextObject];
    CFRelease(result);
    return teamID;
}


int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }

    NSString* teamid  = getTeamID();
    NSLog(@"teamid = %@", teamid);
    NSString* md5 =  getCodeResourcesMd5();
    NSLog(@"md5 = %@", md5);
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
