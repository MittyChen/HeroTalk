#include "CopyRightScene.h"
#include "MainMenuScene.h"
#include "LevelSelectScene.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
Scene* CopyRightScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CopyRightScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CopyRightScene::init()
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
    listener->onTouchBegan = CC_CALLBACK_2(CopyRightScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto rootNode = CSLoader::createNode("CopyRightScene.csb");
    this->addChild(rootNode);
    rootNode->setName("CopyRightScene");
    
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
    
    
    
    cocos2d::ui::Button* btnback =  (cocos2d::ui::Button*)rootNode->getChildByTag(86);
    btnback->addTouchEventListener(CC_CALLBACK_2(CopyRightScene::gotoGame, this) );

 
    return true;
}
bool CopyRightScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void CopyRightScene::gotoGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    
}