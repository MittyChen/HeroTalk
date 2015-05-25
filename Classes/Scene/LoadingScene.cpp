#include "LoadingScene.h"
#include "TalkScene.h"
USING_NS_CC;

using namespace ui;

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
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
    listener->onTouchBegan = CC_CALLBACK_2(LoadingScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    
    
//    this->addChild(LayerColor::create(Color4B(0,200,120,255),visibleSize.width,visibleSize.height));
   
   
    Label* gameTitle = Label::create();
    gameTitle->setString("你得通过这个游戏才能继续下面的剧情。。。");
    gameTitle->setSystemFontName("Helvetica");
    gameTitle->setSystemFontSize(30);
    gameTitle->setAnchorPoint(Vec2(0.5,0.5));
    gameTitle->setPosition(Vec2(visibleSize.width/2,visibleSize.height*1/6));
    this->addChild(gameTitle);
    
    return true;
}
bool LoadingScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    Director::getInstance()->popScene();
    return true;
}
