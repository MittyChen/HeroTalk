//
//  LHVideoPlayerImplCpp.cpp
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "LHVideoPlayerImplCpp.h"
#import "LHVideoPlayerImpl.h"
//#import "CKIAPManager.h"

int LHVideoPlayerImplCpp::touchcount = 0 ;
std::function<void()> LHVideoPlayerImplCpp::finishcallback;
//static CKIAPManager* IAPmanager = NULL;
void LHVideoPlayerImplCpp::playMP4WithName(const char* name, cocos2d::Rect frameRect,const std::function<void()>& callback )
{
    frameRect.origin =frameRect.origin * .5;
    frameRect.size =frameRect.size * .5;
    [LHVideoPlayerImpl playMP4WithName:[NSString stringWithUTF8String:name] VideoFrame:CGRectMake(frameRect.origin.x, frameRect.origin.y, frameRect.size.width, frameRect.size.height)];
    finishcallback = callback;
    
//    IAPmanager = [[CKIAPManager alloc]init];
}

void LHVideoPlayerImplCpp::setSkipTitle(const char* title)
{
    [LHVideoPlayerImpl setSkipTitle:[NSString stringWithUTF8String:title]];
}

void LHVideoPlayerImplCpp::finishCallback()
{
    finishcallback();
}

void LHVideoPlayerImplCpp::skipVideo()
{
    [LHVideoPlayerImpl skipVideo];
}

void LHVideoPlayerImplCpp::showHelpText(const char * textC)
{
    NSString* text = [NSString stringWithUTF8String:textC];
    [LHVideoPlayerImpl showHelpText:text];
}

void LHVideoPlayerImplCpp::touchAudio(const char * textC){
    touchcount++;
    
    auto lamdaresetcount =[]()
    {
        touchcount=0;
    };
    
    
    [UIView animateWithDuration:0.0 delay:2.0 options:UIViewAnimationOptionAllowUserInteraction animations:^{
        
    } completion:^(BOOL finished) {
        
        touchcount=0;
        
    }];

    if(touchcount >= 2)
    {
        LHVideoPlayerImplCpp::skipVideo();
        touchcount = 0;
    }else{
        
        LHVideoPlayerImplCpp::showHelpText(textC);
    }
}
void LHVideoPlayerImplCpp::resetTouchCount()
{
    touchcount=0;
}
#endif