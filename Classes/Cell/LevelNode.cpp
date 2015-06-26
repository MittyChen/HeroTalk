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
    isLocked = true;
    int indexz = random(0, 4);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
//    __String* ss = __String::createWithFormat("cloud_%d.png",indexz);
//    cloud = cocos2d::ui::Button::create(ss->getCString());
    
    cloud = cocos2d::ui::Button::create("whitedot.png");
    cloud->setName("LevelNode_1");
    
    cloud->setAnchorPoint(Vec2(0.5, 0.5));
    cloud->setPosition(visibleSize/2);
    cloud->setScale(2);
    
    levelcode = Label::create("1", "Arial", 50);
    levelcode->setColor(Color3B(50,200,200));
    levelcode->setAnchorPoint(Vec2(0.5, 0.5));
    levelcode->setPosition(visibleSize/2);
    
    
    ps = cocos2d::CCParticleRain::create();
    ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("whitedot.png"));
//    ps->setContentSize(cloud->getContentSize());
    ps->setPosition (cloud->getPosition() );
    
    ps->setStartSize(ps->getStartSize()*1.5);
    ps->setStartColor(Color4F(0,100,255,255));
    ps->setTotalParticles(200);
    ps->setLife(4.0);
    ps->setSpeed(20);
    
    
//    ps->setScaleX(cloud->getContentSize().width/visibleSize.width /2);
//    ps->setScaleY(2);
    
    ps->setPosVar( Vec2(cloud->getContentSize().width * cloud->getScale() /3, 0) );
    this->addChild(ps);
    this->addChild(cloud);
    this->addChild(levelcode);
    
    
   
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = CC_CALLBACK_2(LevelNode::onTouchBegan,this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
//    listener->setSwallowTouches(true);
    
//    userNode = Sprite::create("whitedot.png");
//    Vec2 pos = Vec2(visibleSize.width/2, visibleSize.height/2 + cloud->getContentSize().height/2 + userNode->getContentSize().height/2);
//    
//    userNode->setAnchorPoint(Vec2(0.5, 0.5));
//    userNode->setPosition(pos);
//    this->setPosition(pos);
//    this->addChild(userNode);
    
    
    return true;

}


void LevelNode::update(float delta)
{
    

}

void LevelNode::setLevelCode(int code)
{
    __String* ss = __String::createWithFormat("%d",code);
    levelcode->setString(ss->getCString());
    
    __String* cc = __String::createWithFormat("LevelNode_%d",code);
    cloud->setName(cc->getCString());
    mcode = code;
    
    __String* sstemp = __String::createWithFormat("HERO_TALK_UNLOCKED_LEVEL_%d",mcode);
    bool isunlock = UserDefault::getInstance()->getBoolForKey(sstemp->getCString());
    if (isunlock) {
        unlockLevel();
        startRain();
    }else{
        stopRain();
    }
    
    
}

int LevelNode::getLevelCode()
{
    return mcode;
}


void LevelNode::startRain()
{
//    cloud->setColor(Color3B(255,255,255));
    
    cloud->setEnabled(true);
//    ps->resetSystem();
     ps->stopSystem();
}

void LevelNode::stopRain()
{
    cloud->setEnabled(false);
    
    
//    cloud->setColor(Color3B(180,180,180));
    
    float redr = random(180, 200);
    float greenr = random(110, 120);
    float bluer = random(150, 180);
    
    cloud->setColor(Color3B(redr,greenr,bluer));
    
     ps->stopSystem();
}
bool LevelNode::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
void LevelNode::unlockLevel()
{
//    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("Levels.plist");
//    
//    __Dictionary* pdic = Dictionary::createWithContentsOfFile(fullPath.c_str());
//    
//    pdic->setObject(__String::create("ok"), cloud->getName());
//    
//    
//    
//    Value mval = Value("ok");
//    ValueMap _levelsmap;
////    _levelsmap.insert(make_pair(cloud->getName(),mval));
//    
//    for (int i = mcode; i>0; i--) {
//        __String* ss = __String::createWithFormat("LevelNode_%d",i);
//        
//        _levelsmap.insert(std::make_pair(ss->getCString(),mval));
//    }
//    
//    
//    FileUtils::getInstance()->writeToFile(_levelsmap, fullPath);//放在包中的文件在重启会被覆盖 丢失数据

    __String* ss = __String::createWithFormat("HERO_TALK_UNLOCKED_LEVEL_%d",mcode);
    UserDefault::getInstance()->setBoolForKey(ss->getCString(),true);
    
    UserDefault::getInstance()->setIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED",mcode);
    isLocked = false;
    cloud->runAction(TintTo::create(4, 255, 255, 255));
}