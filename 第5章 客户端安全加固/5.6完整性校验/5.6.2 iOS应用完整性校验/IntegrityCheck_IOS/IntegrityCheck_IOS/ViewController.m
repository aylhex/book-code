//
//  ViewController.m
//  IntegrityCheck_IOS
//
//  Created by TAL on 2022/6/2.
//

#import "ViewController.h"
#include <CommonCrypto/CommonDigest.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

// 获取文件的Hash值
NSString* getFileHashValue()
{
    NSString *currentPath = [[NSBundle mainBundle] resourcePath];
    NSString *path = [currentPath stringByAppendingString:@"/_CodeSignature/CodeResources"];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSLog(@"path = %@", path);
    NSDirectoryEnumerator *dirEnum =[fileManager enumeratorAtPath:currentPath];
    
    NSString *tempPath;
    
    while((tempPath=[dirEnum nextObject])!= nil)
    {
        NSString *temp= [NSString stringWithFormat:@"%@%@", @"/",tempPath];
        NSString *path = [currentPath stringByAppendingString:temp];
        BOOL isDir;
        if([fileManager fileExistsAtPath:path isDirectory:&isDir] && !isDir)
        {
            NSData *data = [NSData dataWithContentsOfFile:path];
            //NSLog(@"Hash file name ======>  %@",tempPath);
            unsigned char digest[CC_MD5_DIGEST_LENGTH];
            CC_MD5(data.bytes, (CC_LONG)data.length, digest );
            NSMutableString *result = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
            for( int i = 0; i < CC_MD5_DIGEST_LENGTH; i++ )
            {
                [result appendFormat:@"%02x", digest[i]];
            }
            
            NSLog(@"Hash file name:%@ =====> %@",  tempPath,result);
        }
    }
    
    return nil;
    
}

- (IBAction)mButton:(id)sender {
    
    NSString* md5 =  getFileHashValue();
//    NSLog(@"md5 = %@", md5);
//    [mLabel setText:md5];
}
@end
