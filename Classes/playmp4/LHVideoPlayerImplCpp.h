//
//  LHVideoPlayerImplCpp.h
//  VideoDemo
//
//  Created by LiuYanghui on 14-5-5.
//
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS


#ifndef __VideoDemo__LHVideoPlayerImplCpp__
#define __VideoDemo__LHVideoPlayerImplCpp__


class LHVideoPlayerImplCpp
{
public:
    /**
     * 开始播放MP4视频
     * name 视频名称，不要带后缀".mp4"。(比如文件是test.mp4, 那么name就传"test")
     * frameRect 视频显示的区域。全屏(Rect(0, 0, visibleSize.width, visibleSize.height))
     */
    static void playMP4WithName(const char* name, cocos2d::Rect frameRect,const std::function<void()>& callback);
    
    /**
     * 设置跳过影片按钮title，默认无跳过影片功能
     */
    static void setSkipTitle(const char* title);
    
    static void finishCallback();
    
    static void skipVideo();
    
    static void showHelpText(const char * textC);
    
    static void touchAudio(const char * textC);
    static void resetTouchCount();
//    static void showHelpText(const char* messageC,float screenWidth, float screenHeight);
public:
    static std::function<void()> finishcallback;
    
private:
    static int touchcount ;
};

#endif /* defined(__VideoDemo__LHVideoPlayerImplCpp__) */

#endif