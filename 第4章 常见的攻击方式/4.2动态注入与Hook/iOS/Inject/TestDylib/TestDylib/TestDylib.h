//
//  TestDylib.h
//  TestDylib
//
//  Created by Ann on 2022/11/20.
//

#import <Foundation/Foundation.h>

//! Project version number for TestDylib.
FOUNDATION_EXPORT double TestDylibVersionNumber;

//! Project version string for TestDylib.
FOUNDATION_EXPORT const unsigned char TestDylibVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <TestDylib/PublicHeader.h>


NS_ASSUME_NONNULL_BEGIN

@interface TestDylib : NSObject

- (void)startInject;

@end

NS_ASSUME_NONNULL_END
