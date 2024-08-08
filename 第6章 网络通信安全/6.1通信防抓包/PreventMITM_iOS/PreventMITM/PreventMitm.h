//
//  PreventMitm.h
//  PreventMITM
//
//  Created by Ann on 2022/5/17.
//

#import <UIKit/UIKit.h>

@interface PreventMitm : NSObject
-(BOOL) isProxy;
// 设置不走系统代理
-(void) setProxy;
// ssl pinning
-(void) testSSLPinning;
@end
/* PreventMitm_h */
