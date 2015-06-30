#ifndef __LEVELSELECT_SCENE_H__
#define __LEVELSELECT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define LEVEL_COUNT 19

class LevelSelectScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int unclockLevel);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void update(float delta);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    // implement the "static create()" method manually
    CREATE_FUNC(LevelSelectScene);
    void selectLevelAction(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    
};

#endif // __LEVELSELECT_SCENE_H__
