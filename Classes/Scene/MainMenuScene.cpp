#include "MainMenuScene.h"
#include "TalkScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectScene.h"
#include "cocostudio/CocoStudio.h"
#include "OptionsPopup.h"

USING_NS_CC;


bool isoptions = false;
using namespace ui;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    scheduleUpdate();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MainMenuScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    
    this->addChild(LayerColor::create(Color4B(0,200,120,255),visibleSize.width,visibleSize.height));
   
    Sprite* fengye = Sprite::create("fengye.png");
    fengye->setPosition(visibleSize/2);
    fengye->setAnchorPoint(Vec2(0.5,0.5));
    fengye->runAction(FadeTo::create(0.001, 255));
   
    

    
    float scaleX = visibleSize.width/fengye->getContentSize().width;
    float scaleY = visibleSize.height/fengye->getContentSize().height;
    
    float scaleFactor = scaleX>scaleY?scaleX:scaleY;
    
    fengye->setScale(scaleFactor);
    
    
    //出现一次 淡化至透明
     fengye->runAction(Sequence::create(ScaleTo::create(0.001, scaleFactor*1.2),ScaleTo::create(2.5, scaleFactor*1),FadeTo::create(5, 50), NULL) );
    //循环进行播放
//    fengye->runAction(RepeatForever::create(Sequence::create(FadeTo::create(5, 0),ScaleTo::create(0.01, 1.2),FadeTo::create(0.5, 200),ScaleTo::create(2.5, 1), NULL) ));
    
    this->addChild(fengye);
    
    {
        Sprite* dotP = Sprite::create("whitedot.png");
        dotP->setPosition(Vec2::ZERO);
//        dotP->runAction(TintTo::create(0.01, 255, 89, 56));
        
        ccBezierConfig ccb;
        ccb.controlPoint_1 = Vec2(visibleSize.height/2 + 280,visibleSize.height/2 - 280);
        ccb.controlPoint_2 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 - 280);
        ccb.endPosition = Vec2(visibleSize.width,visibleSize.height);
        BezierTo* actionb = BezierTo::create(10, ccb);
        
        
        ccBezierConfig ccb2;
        ccb2.controlPoint_1 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 + 280);
        ccb2.controlPoint_2 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 + 280);
        ccb2.endPosition = Vec2(0,0);
        BezierTo* actionb2 = BezierTo::create(10, ccb2);
        
        dotP->runAction(RepeatForever::create(Sequence::create(actionb,actionb2, NULL)));
        
        this->addChild(dotP);
        dotP->setColor(cocos2d::Color3B(249, 62, 79));
//        dotP->runAction(RepeatForever::create(Sequence::create(TintTo::create(10, 0, 200, 120),TintTo::create(10, 255, 89, 56), NULL) ));
    }
//
//    {
//        Sprite* dotP = Sprite::create("whitedot.png");
//        dotP->setPosition(visibleSize);
////        dotP->runAction(TintTo::create(0.01,255, 89, 56));
//        
//        ccBezierConfig ccb;
//        ccb.controlPoint_1 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 + 280);
//        ccb.controlPoint_2 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 + 280);
//        ccb.endPosition = Vec2::ZERO;
//        BezierTo* actionb = BezierTo::create(10, ccb);
//        
//        
//        ccBezierConfig ccb2;
//        ccb2.controlPoint_1 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 - 280);
//        ccb2.controlPoint_2 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 - 280);
//        ccb2.endPosition = visibleSize;
//        BezierTo* actionb2 = BezierTo::create(10, ccb2);
//        
//        dotP->runAction(RepeatForever::create(Sequence::create(actionb,actionb2, NULL)));
//        
//        this->addChild(dotP);
//        dotP->setColor(cocos2d::Color3B(28, 241,183));
////        dotP->runAction(RepeatForever::create(Sequence::create(TintTo::create(10, 0, 200, 120),TintTo::create(10, 255, 89, 56), NULL) ));
//    }
//    
//    
//    
//    
//    
//    {
//        Sprite* dotP = Sprite::create("whitedot.png");
//        dotP->setPosition(Vec2(0,visibleSize.height));
////        dotP->runAction(TintTo::create(0.01, 255, 89, 56));
//        
//        ccBezierConfig ccb;
//        ccb.controlPoint_1 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 - 280);
//        ccb.controlPoint_2 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 - 280);
//        ccb.endPosition = Vec2(visibleSize.width,0);
//        BezierTo* actionb = BezierTo::create(10, ccb);
//        
//        
//        ccBezierConfig ccb2;
//        ccb2.controlPoint_1 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 + 280);
//        ccb2.controlPoint_2 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 + 280);
//        ccb2.endPosition = Vec2(0,visibleSize.height);
//        BezierTo* actionb2 = BezierTo::create(10, ccb2);
//        
//        dotP->runAction(RepeatForever::create(Sequence::create(actionb,actionb2, NULL)));
//        dotP->setColor(cocos2d::Color3B(0,155,245));
//        this->addChild(dotP);
//        
////        dotP->runAction(RepeatForever::create(Sequence::create(TintTo::create(10, 0, 200, 120),TintTo::create(10, 255, 89, 56), NULL) ));
//    }
    
    {
        Sprite* dotP = Sprite::create("whitedot.png");
        dotP->setPosition(Vec2(visibleSize.width,0));
//        dotP->runAction(TintTo::create(0.01,255, 89, 56));
        
        ccBezierConfig ccb;
        ccb.controlPoint_1 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 + 280);
        ccb.controlPoint_2 = Vec2(visibleSize.width/2 + 280,visibleSize.height/2 + 280);
        ccb.endPosition = Vec2(0,visibleSize.height);
        BezierTo* actionb = BezierTo::create(10, ccb);
        
        
        ccBezierConfig ccb2;
        ccb2.controlPoint_1 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 - 280);
        ccb2.controlPoint_2 = Vec2(visibleSize.width/2 - 280,visibleSize.height/2 - 280);
        ccb2.endPosition = Vec2(visibleSize.width,0);
        BezierTo* actionb2 = BezierTo::create(10, ccb2);
        
        dotP->runAction(RepeatForever::create(Sequence::create(actionb,actionb2, NULL)));
        
        this->addChild(dotP);
        
//        dotP->runAction(RepeatForever::create(Sequence::create(TintTo::create(10, 0, 200, 120),TintTo::create(10, 255, 89, 56), NULL) ));
    }
    
    
    
    
    
    
//    Sprite* dotP2 = Sprite::create("whitedot.png");
//    dotP2->setPosition(Vec2(600,600));
//    
//    this->addChild(dotP2);
//    
//    TargetedAction* ta = TargetedAction::create(dotP2, ReverseTime::create(actionb));
//    this->runAction(ta);
    
    Label* gameTitle = Label::create();
    gameTitle->setString("度");
    gameTitle->setSystemFontName("Arial");
    gameTitle->setSystemFontSize(100);
    gameTitle->setAnchorPoint(Vec2(0.5,0.5));
    gameTitle->setPosition(Vec2(visibleSize.width/2,visibleSize.height*4/6));
    this->addChild(gameTitle);
    cocos2d::ui::Button * skipbutton = cocos2d::ui::Button::create("Default/Button_Normal.png");
    skipbutton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 1 / 3));
    Label* skipLabel = Label::create();
    skipLabel->setString("开始");
    skipLabel->setSystemFontSize(22);
    skipLabel->setAnchorPoint(Vec2(0.5,0.5));
    skipbutton->addChild(skipLabel);
    skipLabel->setPosition(skipbutton->getContentSize()/2);
    skipLabel->setTextColor(Color4B(0,200,150,255));
    skipbutton->setScale(2.0);
    
    cocos2d::ui::Button * options = cocos2d::ui::Button::create("settings.png");
    options->setScale(0.3);
    options->setPosition(Vec2( -options->getContentSize().width/2  + visibleSize.width,visibleSize.height-options->getContentSize().height*2));
    this->addChild(options);
    
    
    auto gotoOptions = [this](Ref* pSender ,cocos2d::ui::Widget::TouchEventType event){
        isoptions = true;
        OptionsPopup* op = OptionsPopup::create();
        
        this->addChild(op);
    };
    options->addTouchEventListener(gotoOptions);
    
    
    
    auto skipDrama = [=](Ref* obj,cocos2d::ui::Widget::TouchEventType event)
    {
        bool istalkSeen = UserDefault::getInstance()->getBoolForKey("UserSeenTalk");
        if (!istalkSeen) {
            auto scene = TalkScene::createScene();
            // run
            Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
            
        }
        else
        {
            auto scene = LevelSelectScene::createScene(0);
            // run
            Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
        }
    };
    skipbutton->addTouchEventListener(skipDrama);
    this->addChild(skipbutton);

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("gamebg.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("playbg.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("levelselect.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("gamebg.mp3",true);
    
    
    return true;
}
bool MainMenuScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    return true;
}
void MainMenuScene::playClickCallBack(Ref* sender,cocos2d::ui::TouchEventType type)
{
    bool istalkSeen = UserDefault::getInstance()->getBoolForKey("UserSeenTalk");
    if (!istalkSeen) {
        auto scene = TalkScene::createScene();
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    }
    else
    {
        auto scene = LevelSelectScene::createScene(0);
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    }
}