#import <SpringBoard/SpringBoard.h>
%hook SpringBoard
-(void)applicationDidFinishLaunching:(id)application {
    %orig;
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Test"
        message:@"Tweak test！！！"
        delegate:nil
        cancelButtonTitle:@"确定"
        otherButtonTitles:nil];
    [alert show];

    NSLog(@"-----------------------------Tweak test！！！------------------------------");
}
%end