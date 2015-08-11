//
//  HappyStartCell.h
//  HappyStart
//
//  Created by chenwei on 15/3/9.
//
//


//http://www.gamelook.com.cn/2014/12/195109
#ifndef __LevelNode__
#define __LevelNode__

#include"cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

struct LEVEL_DESIGN_FIND_COLOR{
    const char * type = "FIND_COLOR";
    int index;
    int bluecount;
    int redcount;
    int greencount;
};

struct LEVEL_DESIGN_GET_SCORE{
    const char * type = "GET_SCORE";
    int index;
    int score;
};


class LevelNode: public cocos2d::Sprite
{
public:
    int mcode;
    
public:
    LevelNode();
    ~LevelNode();
    
    CREATE_FUNC(LevelNode);
    virtual bool init();
    virtual void update(float delta);
    
    Label* levelcode;
    cocos2d::ParticleSystem* ps;
    cocos2d::ui::Button *  cloud;
//    Sprite* userNode ;
    void setLevelCode(int code);
    
    int getLevelCode();
    
    
    void startRain();
    
    void stopRain();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    void unlockLevel();
    
    bool isLocked;
    
    
    CC_SYNTHESIZE(int, address, AddressName);
    
    std::string type;
    int score;
    int bluecount;
    int redcount;
    int greencount;
};

#endif /* defined(LevelNode) */
