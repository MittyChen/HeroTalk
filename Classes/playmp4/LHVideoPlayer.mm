//
//  LHVideoPlayer.m
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "LHVideoPlayer.h"
#import "LHVideoOverlayView.h"
#import <MediaPlayer/MediaPlayer.h>
#import "LHVideoPlayerImplCpp.h"

@implementation LHVideoPlayer

- (id)init
{
    if ((self = [super init]))
    {
        _player = nil;
    }
    return self;
}

- (void)playMP4WithName: (NSString *)name VideoFrame:(CGRect)rect
{
    UIWindow *keyWindow = [[UIApplication sharedApplication] keyWindow];
    
    // 获取视频文件的名称
    NSString *url = [[NSBundle mainBundle]pathForResource:name ofType:@"mp4"];
    // 初始化player
    _player = [[MPMoviePlayerController alloc] initWithContentURL:[NSURL fileURLWithPath:url]];
    [keyWindow.rootViewController.view addSubview: [_player view]];
    
    // 设置player样式
    [_player setControlStyle: MPMovieControlStyleNone];
    [[_player view] setFrame: rect];
    
    // 当MP4完成播放的回调
    [[NSNotificationCenter defaultCenter]
     addObserver:self selector:@selector(movieFinishedCallback:)
     name:MPMoviePlayerPlaybackDidFinishNotification object:_player];
    
    // 开始播放影片
    [_player play];
    
    // 上层操作层
    _videoOverlayView = [ [LHVideoOverlayView alloc] initWithFrame: rect];
    [keyWindow.rootViewController.view addSubview: _videoOverlayView];
}

// 跳过MP4，直接结束
- (void)skipVideo
{
    if (_player){
        [_player stop];
        [self removePlayer:_player];
    }
    [self playerPlayFinished];
}

- (void)setSkipTitle: (NSString *)title
{
    [_videoOverlayView setSkipTitle:title];
}


-(void)showHelpText:(NSString*)text
{
    [_videoOverlayView showHelpText:text];
}
- (void)removePlayer:(MPMoviePlayerController*)player
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerPlaybackDidFinishNotification object:player];
    
    [player.view removeFromSuperview];
    [_player release];
    _player = nil;
    
    [_videoOverlayView removeFromSuperview];
    [_videoOverlayView release];
    _videoOverlayView = nil;
}

// 播放完成，通知外界
- (void)playerPlayFinished
{
    LHVideoPlayerImplCpp::finishCallback();
}

// MP4完成播放的回调
- (void)movieFinishedCallback:(NSNotification*) aNotification {
    MPMoviePlayerController *player = [aNotification object];
    [self removePlayer:player];
    [self playerPlayFinished];
}


@end
#endif