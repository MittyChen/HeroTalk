#include "TalkScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"


#include "DramaDirector.h"
#include "LoudSpeaker.h"
#include "LoadingScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocostudio;
using namespace ui;

Scene* TalkScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TalkScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TalkScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    scheduleUpdate();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(TalkScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto lambaStartGame = [=](Node* pSender)
    {
        auto scene = GameScene::createScene();
        // run
        
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
        
//        Director::getInstance()->pushScene(LoadingScene::createScene());
    };
    
    DramaDirector::getInstance()->setDelegate(*this,"dramabg.mp3","DramaDescription.plist","Plots.plist",lambaStartGame);
    
    return true;
}
int mindex = 2;
bool TalkScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(mindex >4 )
    {
        mindex = 2;
    }
//    DramaDirector::getInstance()->tapToTakeForward(0);
    
    return true;
}
void TalkScene::playClickCallBack(Ref* sender,cocos2d::ui::TouchEventType type)
{
    auto scene = GameScene::createScene();
    // run
//    Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    
    Director::getInstance()->pushScene(LoadingScene::createScene());
}