//
//  HappyStartCell.cpp
//  HappyStart
//
//  Created by chenwei on 15/3/9.
//
//
#include "cocostudio/CocoStudio.h"
#include "LevelNode.h"

using namespace cocostudio::timeline;

LevelNode::LevelNode()
{
    
}

LevelNode::~LevelNode()
{

}

bool LevelNode::init()
{
    
    if(!Sprite::init())
    {
        return false;
    }
//    this->scheduleUpdate();
    mcode = 1;
    int indexz = random(0, 4);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    __String* ss = __String::createWithFormat("cloud_%d.png",indexz);
    cloud = cocos2d::ui::Button::create(ss->getCString());
    cloud->setName("LevelNode_1");
    
    cloud->setAnchorPoint(Vec2(0.5, 0.5));
    cloud->setPosition(visibleSize/2);
    
    levelcode = Label::create("LEVEL 1", "Arial", 20);
    levelcode->setColor(Color3B(0,100,255));
    levelcode->setAnchorPoint(Vec2(0.5, 0.5));
    levelcode->setPosition(visibleSize/2);
    
    
    ps = cocos2d::CCParticleRain::create();
    ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("whitedot.png"));
    ps->setContentSize(cloud->getContentSize());
    ps->setPosition (cloud->getPosition() );
    
    ps->setStartColor(Color4F(200,100,100,100));
    ps->setTotalParticles(100);
    ps->setLife(1.0);
    ps->setSpeed(30);
    ps->setScaleX(cloud->getContentSize().width/visibleSize.width /2);
    ps->setScaleY(2);
    
    this->addChild(ps);
    this->addChild(cloud);
    this->addChild(levelcode);
    ps->stopSystem();
    
    
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = CC_CALLBACK_2(LevelNode::onTouchBegan,this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
//    listener->setSwallowTouches(true);
    
    
    return true;

}


void LevelNode::update(float delta)
{
    

}

void LevelNode::setLevelCode(int code)
{
    __String* ss = __String::createWithFormat("LEVEL %d",code);
    levelcode->setString(ss->getCString());
    
    __String* cc = __String::createWithFormat("LevelNode_%d",code);
    cloud->setName(cc->getCString());
    mcode = code;
    
}

int LevelNode::getLevelCode()
{
    return mcode;
}


void LevelNode::startRain()
{
    ps->resetSystem();
}

void LevelNode::stopRain()
{
     ps->stopSystem();
}
bool LevelNode::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
