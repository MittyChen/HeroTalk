#include "VideoScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "LHVideoPlayerImplCpp.h"
#else
#include "platform/android/jni/JniHelper.h"
#endif

#include "DramaDirector.h"
#include "LoudSpeaker.h"
#include "LoadingScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocostudio;
using namespace ui;

Scene* VideoScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = VideoScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool VideoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    touchcount  = 0;
    
    scheduleUpdate();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(VideoScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    return true;
}

void VideoScene::update(float delta)
{
    
}


bool VideoScene::onTouchBegan(Touch *touch, Event *unused_event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    touchcount++;
    auto lamdaResetcount = [&](Node* pSender){
        touchcount=0;
    };
    this->runAction( Sequence::create(DelayTime::create(1),CallFuncN::create(lamdaResetcount), NULL) );
    
    if(touchcount >= 2)
    {
        LHVideoPlayerImplCpp::skipVideo();
        touchcount = 0;
    }else{
        
        LHVideoPlayerImplCpp::showHelpText("快速点击跳过视频");
//         Size visibleSize = Director::getInstance()->getVisibleSize();
//        
//        Label* text = Label::create();
//        text->setString("快速双击跳过视频");
//        text->setSystemFontSize(30);
//        text->setAnchorPoint(Vec2(0.5,0.5));
//        
//        LayerColor* lc =LayerColor::create(Color4B(50,50,50,10));
//        lc->setAnchorPoint(Vec2(0.5,0.5));
//        lc->setContentSize(Size(text->getContentSize().width+20,text->getContentSize().height+20));
//        lc->setPosition(Vec2(visibleSize.width/2 - lc->getContentSize().width/2,visibleSize.height/6));
//        
//        text->setPosition(lc->getContentSize()/2);
//        this->addChild(lc);
//        lc->addChild(text);
//        
//        lc->runAction(FadeIn::create(1));
//        
//        auto lamdafade = [=](Node* pSender){
//            lc->runAction(FadeOut::create(1));
//        };
//        
//        auto lamdaRemove = [=](Node* pSender){
//            this->removeChild(lc);
//        };
//        this->runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(lamdafade),CallFuncN::create(lamdaRemove), NULL));
    }
#endif
    return true;
}

void VideoScene::onEnter()
{
    Layer::onEnter();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto lambastartgame = [=]()
    {
        auto scene = MainMenuScene::createScene();
                // run
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene));

    };
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    LHVideoPlayerImplCpp::playMP4WithName("res/Audio/splashvideo", Rect(0, 0, visibleSize.width, visibleSize.height),lambastartgame);
    LHVideoPlayerImplCpp::setSkipTitle("跳过");
#endif
    
    
    
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
//    JniMethodInfo t;
//    if (JniHelper::getStaticMethodInfo(t,
//                                       "org/cocos2dx/lib/Cocos2dxActivity",
//                                       "playVideo",
//                                       "(Ljava/lang/String;)V"))
//    {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID, 
//                                    t.env->NewStringUTF("res/Audio/HE.mp4"));
//    }
    
    
    Size size = Director::getInstance()->getVisibleSize();
    experimental::ui::VideoPlayer* videoPlayer = experimental::ui::VideoPlayer::create();
    
    videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setContentSize(Size(size.width , size.height));
    
    
//    videoPlayer->setKeepAspectRatioEnabled(true);
//    videoPlayer->setFullScreenEnabled(true);
    this->addChild(videoPlayer);
    videoPlayer->setFileName(FileUtils::getInstance()->fullPathForFilename("splashvideo.mp4"));
    videoPlayer->play();
    
    
    auto lambastartgame = [=](Ref* pSender,const cocos2d::experimental::ui::VideoPlayer::EventType eventType)
    {
        
        switch (eventType) {
            case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
                break;
            case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
                break;
            case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
                break;
            case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
            {
                auto scene = MainMenuScene::createScene();
                // run
                Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
                break;
            }
            default:
                break;
        }
    };
    videoPlayer->addEventListener(lambastartgame);
 
#endif
}
