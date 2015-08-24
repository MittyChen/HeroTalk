#ifndef _MittyPopup_Scene_
#define _MittyPopup_Scene_

#include "cocos2d.h"

#include "ui/CocosGUI.h"
class MittyPopup : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    // implement the "static create()" method manually
    CREATE_FUNC(MittyPopup);
    
    void exitPop(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    void setTitleAndText(const char * title,const char * text);
private:
    
};

#endif // MittyPopup
