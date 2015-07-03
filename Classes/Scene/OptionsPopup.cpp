#include "OptionsPopup.h"
#include "MainMenuScene.h"

#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "CommonUtils.h"
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
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
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
    fengye->runAction(FadeTo::create(0.5, 200));
    
    this->addChild(fengye);
    
    
    
    this->addChild(LayerColor::create(GAMEBG_COLOR,visibleSize.width,visibleSize.height));
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(OptionsPopup::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    listener->setSwallowTouches(true);
  
    
    
    auto rootNode = CSLoader::createNode("OptionsLayer.csb");
    this->addChild(rootNode);
    rootNode->setName("opetions");
    
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
    
//    rootNode->runAction(Sequence::create(ScaleTo::create(0.15,1.01),ScaleTo::create(0.15, 1.0), NULL));
    
    cocos2d::ui::CheckBox* audio_check = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(21);
    audio_check->addClickEventListener(CC_CALLBACK_1(OptionsPopup::checkAudio,this));
    
    if (UserDefault::getInstance()->getBoolForKey("HERO_TALK_AUDIO_ON") == true)
    {
        audio_check->setSelected(true);
    }else{
        audio_check->setSelected(false);
    }
    
    
    
    cocos2d::ui::CheckBox* effect_check = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(22);
    effect_check->addClickEventListener(CC_CALLBACK_1(OptionsPopup::checkEffect,this));
    
    if (UserDefault::getInstance()->getBoolForKey("HERO_TALK_EFFECT_ON") == true)
    {
        effect_check->setSelected(true);
    }else{
        effect_check->setSelected(false);
    }
    
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
        UserDefault::getInstance()->setBoolForKey("HERO_TALK_AUDIO_ON",true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
    }else{
        UserDefault::getInstance()->setBoolForKey("HERO_TALK_AUDIO_ON",false);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    }
}

void OptionsPopup::checkEffect(cocos2d::Ref* object)
{
    if (CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume() == 0)
    {
        UserDefault::getInstance()->setBoolForKey("HERO_TALK_EFFECT_ON",true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    }else{
        UserDefault::getInstance()->setBoolForKey("HERO_TALK_EFFECT_ON",false);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
}

void OptionsPopup::closeOptions(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    Scene* op = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, op));
}