#include "PopupWithMask.h"
#include "TalkScene.h"

#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;

Scene* PopupWithMask::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PopupWithMask::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PopupWithMask::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    scheduleUpdate();
     Size visibleSize = Director::getInstance()->getVisibleSize();
    
    LayerColor* lco =  LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height);
    this->addChild(lco);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PopupWithMask::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,lco);
    
    return true;
}
bool PopupWithMask::onTouchBegan(Touch *touch, Event *unused_event)
{
    return false;
}
