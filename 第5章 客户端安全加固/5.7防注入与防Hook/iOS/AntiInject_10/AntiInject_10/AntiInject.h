//
//  AntiInject.h
//  AntiInject
//
//  Created by AYL on 2023/1/12.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AntiInject : NSObject

-(void) check_inject_by_env;
-(bool) checkWhiteList;
@end

NS_ASSUME_NONNULL_END
