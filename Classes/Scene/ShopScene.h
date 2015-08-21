#ifndef ShopScene_scene
#define ShopScene_scene

#include "cocos2d.h" 
#include "ui/CocosGUI.h"

#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"

//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//#import "CKIAPManager.h"
//#endif

enum PRODUCT_ID{
    PRODUCTID_ONESHOT,
    PRODUCTID_RANDOM,
    PRODUCTID_CHANGETYPE,
    PRODUCTID_NONE,
} ;
class ShopScene : public cocos2d::Layer
{

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    // implement the "static create()" method manually
    CREATE_FUNC(ShopScene);
    virtual void update(float delta);
    void gotoGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
    void buyItem(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
    void selectProduct(cocos2d::Ref* object);
private:
    PRODUCT_ID productId;
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    CKIAPManager* IAPManager;
//#endif
    
    
};

#endif // PreGameScene_scene
