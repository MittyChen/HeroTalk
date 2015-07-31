#include "ShopScene.h"
#include "MainMenuScene.h"

//#include "cocostudio/CCObjectExtensionData.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "Movie.h"
#include "GIFMovie.h"
#include "CacheGif.h"
#include "InstantGif.h"
#include "GifBase.h"
#include "CommonUtils.h"

USING_NS_CC;

using namespace std;
using namespace cocos2d::ui;



using namespace cocostudio::timeline;
using namespace ui;

Scene* ShopScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ShopScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
void ShopScene::update(float delta)
{
}
// on "init" you need to initialize your instance
bool ShopScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    productId = "initValue";
    scheduleUpdate();
    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    IAPManager = [[CKIAPManager alloc]init];
//#endif
   
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
   
    auto shopscene = CSLoader::createNode("ShopScene.csb");
    
    Sprite* fengye = Sprite::create("fengye.png");
    fengye->setPosition(visibleSize/2);
    fengye->setAnchorPoint(Vec2(0.5,0.5));
    fengye->runAction(FadeTo::create(0.001, 255));
    
    float scaleX = visibleSize.width/fengye->getContentSize().width;
    float scaleY = visibleSize.height/fengye->getContentSize().height;
    float scaleFactor = scaleX>scaleY?scaleX:scaleY;
    fengye->setScale(scaleFactor);
    //出现一次 淡化至透明
    fengye->runAction(FadeTo::create(0.5, 200));
    this->addChild(fengye);
    
    this->addChild(LayerColor::create(GAMEBG_COLOR,visibleSize.width,visibleSize.height));
    shopscene->setTag(111);
    this->addChild(shopscene);
    
    
    shopscene->setContentSize(visibleSize);
    ui::Helper::doLayout(shopscene);
    
    cocos2d::ui::Button* backbtn =  (cocos2d::ui::Button*)shopscene->getChildByTag(41);
    backbtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::gotoGame, this) );

    cocos2d::ui::Button* buybtn =  (cocos2d::ui::Button*)shopscene->getChildByTag(27);
    buybtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::buyItem, this) );


    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)shopscene->getChildByTag(17)->getChildByTag(113);
    mTool_OneShot->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)shopscene->getChildByTag(17)->getChildByTag(114);
    mTool_ChangeType->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)shopscene->getChildByTag(17)->getChildByTag(115);
    mTool_RandomType->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    return true;
}
void ShopScene::selectProduct(cocos2d::Ref* object)
{
    auto mcheck = (cocos2d::ui::CheckBox*)object;
//    ((cocostudio::)mcheck->getUserObject())
    
    productId = "";
    
    cocos2d::ui::Text* descript = (cocos2d::ui::Text*)this->getChildByTag(111)->getChildByTag(26);
    const char * desp = "";
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)this->getChildByTag(111)->getChildByTag(17)->getChildByTag(113);
    mTool_OneShot->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)this->getChildByTag(111)->getChildByTag(17)->getChildByTag(114);
    mTool_ChangeType->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)this->getChildByTag(111)->getChildByTag(17)->getChildByTag(115);
    mTool_RandomType->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    if(((Node*)object)->getTag() == mTool_OneShot->getTag() ){
        mTool_ChangeType->setSelected(false);
        mTool_RandomType->setSelected(false);
        desp = "可以消除任意的一个元素！！";
        
    }
    if(((Node*)object)->getTag() == mTool_ChangeType->getTag() ){
        mTool_OneShot->setSelected(false);
        mTool_RandomType->setSelected(false);
        desp = "可以改变任一元素的属性！！";
    }
    if(((Node*)object)->getTag() == mTool_RandomType->getTag() ){
        mTool_OneShot->setSelected(false);
        mTool_ChangeType->setSelected(false);
        desp = "可使任一元素属性随机变化！！";
    }
    
//    com.mitty.iap.singledelete
//    com.mitty.iap.selecttype
//    com.mitty.iap.randomtype
    
    descript->setString(desp);
}

bool ShopScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void ShopScene::gotoGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}
void ShopScene::buyItem(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    NSString* stringId = [NSString stringWithUTF8String:productId];
//    [IAPManager purchasesProduct:stringId];
//#endif
    
}
