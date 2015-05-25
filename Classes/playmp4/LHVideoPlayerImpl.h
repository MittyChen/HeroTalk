//
//  LHVideoPlayerImpl.h
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <Foundation/Foundation.h>

@interface LHVideoPlayerImpl : NSObject

+ (void)playMP4WithName: (NSString *)name VideoFrame:(CGRect)rect;
+ (void)skipVideo;
+ (void)setSkipTitle: (NSString *)title;
+ (void)showHelpText:(NSString*)text;
@end
#endif