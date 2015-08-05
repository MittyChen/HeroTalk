#include "AppDelegate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectScene.h"
#include "VideoScene.h"
#include "ShopScene.h"
#include "CommonUtils.h"
#include "PluginChannel.h"
#include "Analytics.h"
#include "SPCScene.h"
USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}


bool AppDelegate::applicationDidFinishLaunching() {

    cocos2d::Size originsize = Director::getInstance()->getVisibleSize();

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("HeroTalk", cocos2d::Rect(0, 0, originsize.width , originsize.height));
        director->setOpenGLView(glview);
    }
    
    director->getOpenGLView()->setDesignResolutionSize( originsize.width , originsize.height, ResolutionPolicy::SHOW_ALL);
    
    // turn on display FPS
//    director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    
    FileUtils::getInstance()->addSearchPath("res");
    
    FileUtils::getInstance()->addSearchPath("res/DramaPlist");

    FileUtils::getInstance()->addSearchPath("res/Shaders");

    FileUtils::getInstance()->addSearchPath("res/Sounds");

    FileUtils::getInstance()->addSearchPath("res/Audio");
    
    FileUtils::getInstance()->addSearchPath("res/clouds");
    
    FileUtils::getInstance()->addSearchPath("res/Animals");
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("explod.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("colorfulmade.wav");
    
    __String* ss = __String::createWithFormat("HERO_TALK_UNLOCKED_LEVEL_%d",1);
    UserDefault::getInstance()->setBoolForKey(ss->getCString(),true);
    
    if (UserDefault::getInstance()->getIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED") == 0) {
        UserDefault::getInstance()->setIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED",1);
        
    }
    
    if (UserDefault::getInstance()->getBoolForKey("HERO_TALK_AUDIO_ON") == false)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    }
    if (UserDefault::getInstance()->getBoolForKey("HERO_TALK_EFFECT_ON") == false)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.0);
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    }
    
    PluginChannel::getInstance()->loadPlugins();
    
    int maxlevel =  UserDefault::getInstance()->getIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED");
    // create a scene. it's an autorelease object
//    auto scene =  GameScene::createScene(8,0);
//    auto scene =  LevelSelectScene::createScene(maxlevel);
    auto scene =  VideoScene::createScene();
//    auto scene =  SPCScene::createScene();
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
     Analytics::getInstance()->stopSession();
    // if you use SimpleAudioEngine, it must be pause
     CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    Analytics::getInstance()->startSession();
    // if you use SimpleAudioEngine, it must resume here
     CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
     
}


