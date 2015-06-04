#include "OptionsPopup.h"
#include "GameScene.h"

#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;

Scene* OptionsPopup::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = OptionsPopup::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool OptionsPopup::init()
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
    listener->onTouchBegan = CC_CALLBACK_2(OptionsPopup::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    listener->setSwallowTouches(true);
  
    
    
    auto rootNode = CSLoader::createNode("OptionsLayer.csb");
    this->addChild(rootNode);
    rootNode->setName("opetions");
    
    
    cocos2d::ui::CheckBox* audio_check = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(21);
    audio_check->addClickEventListener(CC_CALLBACK_1(OptionsPopup::checkAudio,this));
    cocos2d::ui::CheckBox* effect_check = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(22);
    effect_check->addClickEventListener(CC_CALLBACK_1(OptionsPopup::checkEffect,this));
    
    
    cocos2d::ui::Button* levels =  (cocos2d::ui::Button*)rootNode->getChildByTag(12);
    levels->addTouchEventListener(CC_CALLBACK_2(OptionsPopup::closeOptions, this) );

    return true;
}
bool OptionsPopup::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    return false;
}


void OptionsPopup::checkAudio(cocos2d::Ref* object)
{
    if (CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() == 0)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    }
}

void OptionsPopup::checkEffect(cocos2d::Ref* object)
{
    if (CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume() == 0)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
}

void OptionsPopup::closeOptions(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    this->removeFromParent();
}