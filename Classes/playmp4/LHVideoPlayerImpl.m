//
//  LHVideoPlayerImpl.m
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "LHVideoPlayerImpl.h"
#import "LHVideoPlayer.h"

@implementation LHVideoPlayerImpl

static LHVideoPlayer *_videoPlayer = nil;

+ (void)playMP4WithName: (NSString *)name VideoFrame:(CGRect)rect
{
    if (!_videoPlayer) {
        _videoPlayer = [[LHVideoPlayer alloc] init];
    }
    [_videoPlayer playMP4WithName:name VideoFrame:rect];
}

+ (void)skipVideo
{
    if (_videoPlayer) {
        [_videoPlayer skipVideo];
    }
}

+ (void)setSkipTitle: (NSString *)title
{
    if (_videoPlayer) {
        [_videoPlayer setSkipTitle:title];
    }
}

+(void)showHelpText:(NSString*)text;
{
    [_videoPlayer showHelpText:text];
}

@end
#endif