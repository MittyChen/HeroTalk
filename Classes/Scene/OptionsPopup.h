#ifndef _OptionsPopupScene_
#define _OptionsPopupScene_

#include "cocos2d.h"

#include "ui/CocosGUI.h"
class OptionsPopup : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    // implement the "static create()" method manually
    CREATE_FUNC(OptionsPopup);
    void checkAudio(cocos2d::Ref* object);
    void checkEffect(cocos2d::Ref* object);
    
private:
    
};

#endif // OptionsPopup
