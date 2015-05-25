//
//  LHVideoOverlayView.h
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <UIKit/UIKit.h>

@interface LHVideoOverlayView : UIView
{
//    UIButton *_btnSkip;
    UILabel * _labelSkip;
}
- (void)setSkipTitle: (NSString *)title;

-(void)showHelpText:(NSString*)text;
@end
#endif