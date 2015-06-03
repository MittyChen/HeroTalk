#include "FinishPopup.h"
#include "GameScene.h"

#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;

Scene* FinishPopup::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = FinishPopup::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FinishPopup::init()
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
    
    LayerColor* lco =  LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height);
    this->addChild(lco);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FinishPopup::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    auto rootNode = CSLoader::createNode("GameFinishScene.csb");
    rootNode->setAnchorPoint(Vec2(0.5,0.5));
    rootNode->setPosition(visibleSize/2);
    this->addChild(rootNode);
    rootNode->setName("FinishPopRoot");
    rootNode->runAction(Sequence::create(ScaleTo::create(0.15,1.1),ScaleTo::create(0.15, 1.0), NULL));
    return true;
}
bool FinishPopup::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
