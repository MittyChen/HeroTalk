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
    void setLevelCode(int code);
    
    int getLevelCode();
    
    
    void startRain();
    
    void stopRain();
    
     virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
};

#endif /* defined(LevelNode) */
