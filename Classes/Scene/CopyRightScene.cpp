#include "CopyRightScene.h"
#include "MainMenuScene.h"
#include "LevelSelectScene.h"
#include "cocostudio/CocoStudio.h"


#include "Movie.h"
#include "GIFMovie.h"
#include "CacheGif.h"
#include "InstantGif.h"
#include "GifBase.h"
#include "CommonUtils.h"


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
void CopyRightScene::update(float delta)
{
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
    
    Sprite* fengye = Sprite::create("fengye.png");
    fengye->setPosition(visibleSize/2);
    fengye->setAnchorPoint(Vec2(0.5,0.5));
    fengye->runAction(FadeTo::create(0.001, 255));
    
    float scaleX = visibleSize.width/fengye->getContentSize().width;
    float scaleY = visibleSize.height/fengye->getContentSize().height;
    float scaleFactor = scaleX>scaleY?scaleX:scaleY;
    fengye->setScale(scaleFactor);
    //出现一次 淡化至透明
    fengye->runAction(FadeTo::create(0.5, 255));
    
    this->addChild(fengye);
    
    
    this->addChild(LayerColor::create(GAMEBG_COLOR,visibleSize.width,visibleSize.height));
//    Color4B(0,200,120,255) 、、grreen
    
   
    
    
    
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

 
    
    
//    std::string info = Director::getInstance()->getTextureCache()->getCachedTextureInfo();
//    std::string name = FileUtils::getInstance()->fullPathForFilename("girl1312.gif");
//    GifBase *gif = InstantGif::create(name.c_str());
//    if(gif == NULL)
//    {
//        CCLOG("%s","create gif failed");
//        return  true;
//    }
//    gif->setAnchorPoint(Point(0,0));
//    this->addChild(gif);
//    gif->setPosition(Point(0,0));
//    gif->setTag(1000);
//    info = Director::getInstance()->getTextureCache()->getCachedTextureInfo();

    
    return true;
}
bool CopyRightScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void CopyRightScene::gotoGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    
}

