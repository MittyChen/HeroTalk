#ifndef __TALK_SCENE_H__
#define __TALK_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class TalkScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    // implement the "static create()" method manually
    CREATE_FUNC(TalkScene);
    void playClickCallBack(Ref* sender,cocos2d::ui::TouchEventType type);
    
private:
    
};

#endif // __TALK_SCENE_H__
