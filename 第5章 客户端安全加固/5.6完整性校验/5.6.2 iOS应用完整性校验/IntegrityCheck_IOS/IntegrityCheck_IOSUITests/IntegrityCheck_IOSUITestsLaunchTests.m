//
//  IntegrityCheck_IOSUITestsLaunchTests.m
//  IntegrityCheck_IOSUITests
//
//  Created by TAL on 2022/6/2.
//

#import <XCTest/XCTest.h>

@interface IntegrityCheck_IOSUITestsLaunchTests : XCTestCase

@end

@implementation IntegrityCheck_IOSUITestsLaunchTests

+ (BOOL)runsForEachTargetApplicationUIConfiguration {
    return YES;
}

- (void)setUp {
    self.continueAfterFailure = NO;
}

- (void)testLaunch {
    XCUIApplication *app = [[XCUIApplication alloc] init];
    [app launch];

    // Insert steps here to perform after app launch but before taking a screenshot,
    // such as logging into a test account or navigating somewhere in the app

    XCTAttachment *attachment = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
    attachment.name = @"Launch Screen";
    attachment.lifetime = XCTAttachmentLifetimeKeepAlways;
    [self addAttachment:attachment];
}

@end
