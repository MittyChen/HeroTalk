#ifndef __SPC_SCENE_H__
#define __SPC_SCENE_H__

#include "cocos2d.h"



enum SPC_TYPE
{
    TYPE_SCESSIOR,
    TYPE_PUNCH,
    TYPE_CLOTH
};
class SPCScene : public cocos2d::Layer
{
public:
    
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    // implement the "static create()" method manually
    CREATE_FUNC(SPCScene);
    bool leftWin();
    static int winSide();
    static void resetresult();
private:
    const char* spcFile[3];
    SPC_TYPE typeLeft;
    SPC_TYPE typeRight;
    static int winsideresult;
    
};

#endif // __SPC_SCENE_H__
