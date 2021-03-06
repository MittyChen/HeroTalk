#include "MainMenuScene.h"
#include "StoryScene.h"
#include "SimpleAudioEngine.h"
#include "LevelSelectScene.h"
#include "cocostudio/CocoStudio.h"
#include "OptionsPopup.h"
#include "CopyRightScene.h"
#include "CommonUtils.h"
#include "ShopScene.h"
#include "HappyStartCell.h"
#include "Ads.h"

USING_NS_CC;

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

//    scheduleUpdate();
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MainMenuScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
   
    Sprite* fengye = Sprite::create("fengye.png");
    
    ParticleSnow* ccsz = ParticleSnow::createWithTotalParticles(66);
    ccsz->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("whitedot.png"));
    ccsz->setStartColorVar(cocos2d::Color4F(1, 10,40,255));
    ccsz->setSpeed(5);
    ccsz->setPosition(Vec2(visibleSize.width/2,0));
    ccsz->cocos2d::Node::setRotation(180);
    ccsz->setScale(3);
    this->addChild(ccsz);
    
    
    fengye->setPosition(visibleSize/2);
    fengye->setAnchorPoint(Vec2(0.5,0.5));
    fengye->runAction(FadeTo::create(0.001, 200));
    float scaleX = visibleSize.width/fengye->getContentSize().width;
    float scaleY = visibleSize.height/fengye->getContentSize().height;
    float scaleFactor = scaleX>scaleY?scaleX:scaleY;
    fengye->setScale(scaleFactor);
    //出现一次 淡化至透明
     fengye->runAction(Sequence::create(ScaleTo::create(0.001, scaleFactor*1.2),ScaleTo::create(1.5, scaleFactor*1), NULL) );
    //循环进行播放
//    fengye->runAction(RepeatForever::create(Sequence::create(FadeTo::create(5, 0),ScaleTo::create(0.01, 1.2),FadeTo::create(0.5, 200),ScaleTo::create(2.5, 1), NULL) ));
    
    this->addChild(fengye);
    
    
    
    Sprite* flower = Sprite::create("mainmenubg.png");
    flower->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2) );
    flower->setAnchorPoint(Vec2(0.5,0.5));
    flower->setScale(visibleSize.width*5/6/flower->getContentSize().width);
    this->addChild(flower);
    flower->runAction(RepeatForever::create(RotateBy::create(50, 360)));
    
    this->addChild(LayerColor::create(GAMEBG_COLOR,visibleSize.width,visibleSize.height));
    
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
        dotP->setColor(cocos2d::Color3B(28, 241,183));
        
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
    
//    Label* gameTitle = Label::create();
//    gameTitle->setString("彩");
//    gameTitle->setSystemFontName("Arial");
//    gameTitle->setSystemFontSize(100);
//    gameTitle->setAnchorPoint(Vec2(0.5,0.5));
//    gameTitle->setPosition(Vec2(visibleSize.width/2,visibleSize.height*4/6));
//    this->addChild(gameTitle);
    
    Sprite* gameTitle = Sprite::create("cai.png");
    gameTitle->setAnchorPoint(Vec2(0.5,0.5));
    gameTitle->setPosition(Vec2(visibleSize.width/2,visibleSize.height*4/6));
    gameTitle->setScale(1);
    this->addChild(gameTitle);
    
    
    
    cocos2d::ui::Button * skipbutton = cocos2d::ui::Button::create("startBtn.png");
    skipbutton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 5 / 12));
    skipbutton->setScale(0.7);
    this->addChild(skipbutton);
    
//    Label* skipLabel = Label::create();
//    skipLabel->setString("开始");
//    skipLabel->setSystemFontSize(20);
//    skipLabel->setAnchorPoint(Vec2(0.5,0.5));
//    skipbutton->addChild(skipLabel);
//    skipLabel->setPosition(skipbutton->getContentSize()/2);
//    skipLabel->setTextColor(Color4B(0,200,150,255));
//    skipbutton->setScale(2.0);
    
    cocos2d::ui::Button * options = cocos2d::ui::Button::create("settings.png");
    options->setScale(0.3);
    options->setPosition(Vec2( -options->getContentSize().width / 3  + visibleSize.width,  options->getContentSize().height/2));
    this->addChild(options);
    
    cocos2d::ui::Button * shopbtn = cocos2d::ui::Button::create("shopbtn.png");
    shopbtn->setScale(0.17);
    shopbtn->setPosition(Vec2(visibleSize.width - options->getPosition().x ,options->getPosition().y));
    this->addChild(shopbtn);
    
    cocos2d::ui::Button * copyrightbtn = cocos2d::ui::Button::create("copyright.png");
    copyrightbtn->setScale(0.3);
    copyrightbtn->setPosition(Vec2(options->getPosition().x ,options->getPosition().y + copyrightbtn->getContentSize().height*copyrightbtn->getScale()*2));
    this->addChild(copyrightbtn);
    
    auto gotoCopyRight = [](Ref* pSender,cocos2d::ui::Widget::TouchEventType type){
        if (type != ui::Widget::TouchEventType::ENDED) {
            return;
        }
        auto scene = CopyRightScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    };
    copyrightbtn->addTouchEventListener(gotoCopyRight);
    
    auto gobackMain = [this](Ref* pSender ,cocos2d::ui::Widget::TouchEventType event){
        if (event != ui::Widget::TouchEventType::ENDED) {
            return;
        }
        
        auto scene = MainMenuScene::createScene();
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        
    };
    
    
    auto gotShop = [this,gobackMain,visibleSize](Ref* pSender ,cocos2d::ui::Widget::TouchEventType event){
        if (event != ui::Widget::TouchEventType::ENDED) {
            return;
        }
        Scene* scene  = ShopScene::createScene();
    
        // run
        Director::getInstance()->pushScene(TransitionFade::create(1, scene));
        
    };
    shopbtn->addTouchEventListener(gotShop);
    
    
    auto gotoOptions = [this](Ref* pSender ,cocos2d::ui::Widget::TouchEventType event){
        if (event != ui::Widget::TouchEventType::ENDED) {
            return;
        }
        
        Scene* op = OptionsPopup::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, op));
    };
    
    options->addTouchEventListener(gotoOptions);
    

    auto skipDrama = [=](Ref* obj,cocos2d::ui::Widget::TouchEventType event)
    {
        
        if (event != ui::Widget::TouchEventType::ENDED) {
            return;
        }
        
//        bool istalkSeen = UserDefault::getInstance()->getBoolForKey("UserSeenTalk");
//        if (!istalkSeen) {
//            auto scene = StoryScene::createScene();
//            // run
//            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
//            
//        }
//        else
        {
            
            auto scene = LevelSelectScene::createScene(0);
            // run
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        }
    };
    skipbutton->addTouchEventListener(skipDrama);
    
    
//    for(int i = 0; i < 5; ++i )
//    {
//        for(int j =0 ; j < 5 ; ++j)
//        {
//            HappyStartCell* mm =  HappyStartCell::create();
//            mm->setParameters(Color3B(25.5f * i,25.5f * j,10.f*(i+j)),Vec2(0,0),cocos2d::Size(40,40),Vec2(random(1, 20),random(1,20)),4);
//            addChild((Node*)mm);
//            int typeFind = random(1,7);
//            
//            if (typeFind == 7) {
//                
//                typeFind = random(1,100) > 90 ? 7 : random(1, 6);
//            }
//            
//            mm->setType((CELL_TYPE)typeFind);
//            
//            
//            auto gonextStroyline = [=](Node* pSender)mutable
//            {
//                int typeFind = random(1,6);
//                
//                mm->setParameters(Color3B(25.5f * i,25.5f * j,10.f*(i+j)),Vec2(0,0),cocos2d::Size(40,40),Vec2(random(1, 20),random(1,20)),25);
//                
//                mm->setType((CELL_TYPE)typeFind);
//            };
//            
//            CallFuncN* nextLine = CallFuncN::create(gonextStroyline);
//            
//            mm->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1),nextLine, NULL)));
//            
//        }
//    }
    
    
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("gamebg.mp3",true);
    
    return true;
}

void MainMenuScene::onEnter(){
    Layer::onEnter();
    Ads::getInstance()->showAds(AdsType::AD_TYPE_BANNER);
    
}
void MainMenuScene::onExit(){
    Ads::getInstance()->hideAds(AdsType::AD_TYPE_BANNER);
    Layer::onExit();
}


bool MainMenuScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    return true;
}
void MainMenuScene::playClickCallBack(Ref* sender,cocos2d::ui::TouchEventType type)
{
    bool istalkSeen = UserDefault::getInstance()->getBoolForKey("UserSeenTalk");
    if (!istalkSeen) {
        auto scene = StoryScene::createScene();
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    }
    else
    {
        
        int maxlevel =  UserDefault::getInstance()->getIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED");
        auto scene = LevelSelectScene::createScene(maxlevel);
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    }
}

MainMenuScene::~MainMenuScene(){
}