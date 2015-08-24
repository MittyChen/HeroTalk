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
#include "MittyToolData.h"
#include "MittyPopup.h"
USING_NS_CC;

using namespace std;
using namespace cocos2d::ui;

using namespace cocostudio::timeline;
using namespace ui;

static int tooloneshotprice = 20;
static int toolrandomprice = 20;
static int toolchangetypeprice =30;

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
//    scheduleUpdate();
    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    IAPManager = [[CKIAPManager alloc]init];
//#endif
    productId = PRODUCTID_NONE;
    
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
    
    Sprite* flower = Sprite::create("mainmenubg.png");
    flower->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2) );
    flower->setAnchorPoint(Vec2(0.5,0.5));
    flower->setScale(visibleSize.width*5/6/flower->getContentSize().width);
    this->addChild(flower);
    
    
    this->addChild(LayerColor::create(GAMEBG_COLOR,visibleSize.width,visibleSize.height));
    
    shopscene->setTag(111);
    this->addChild(shopscene);
    
    
    shopscene->setContentSize(visibleSize);
    ui::Helper::doLayout(shopscene);
    
    cocos2d::ui::Button* backbtn =  (cocos2d::ui::Button*)shopscene->getChildByTag(41);
    backbtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::gotoGame, this) );

    cocos2d::ui::Button* buybtn =  (cocos2d::ui::Button*)shopscene->getChildByTag(27);
    buybtn->addTouchEventListener(CC_CALLBACK_2(ShopScene::buyItem, this) );


    cocos2d::ui::Text* coinnum = (cocos2d::ui::Text*)shopscene->getChildByTag(15)->getChildByTag(16);
    char * numstr = (char*) malloc(32);
    sprintf(numstr,"X %d",MittyToolData::getInstance()->getcoins());
    coinnum->setString(numstr);
    
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)shopscene->getChildByTag(17)->getChildByTag(113);
    mTool_OneShot->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)shopscene->getChildByTag(17)->getChildByTag(114);
    mTool_ChangeType->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)shopscene->getChildByTag(17)->getChildByTag(115);
    mTool_RandomType->addClickEventListener(CC_CALLBACK_1(ShopScene::selectProduct,this));
    
    
    cocos2d::ui::Text* oneshotprice = (cocos2d::ui::Text*)mTool_OneShot->getChildByTag(17);
    char * numsohotstr = (char*) malloc(32);
    sprintf(numsohotstr,"需要 %d 金币", tooloneshotprice);
    oneshotprice->setString(numsohotstr);
    
    cocos2d::ui::Text* oneshotnum = (cocos2d::ui::Text*)oneshotprice->getChildByTag(21);
    char * numsohothavestr = (char*) malloc(32);
    sprintf(numsohothavestr,"拥有 %d 个",MittyToolData::getInstance()->getOneshotTool());
    oneshotnum->setString(numsohothavestr);
    
    cocos2d::ui::Text* changeprice = (cocos2d::ui::Text*)mTool_ChangeType->getChildByTag(18);
    char * numchangestr = (char*) malloc(32);
    sprintf(numchangestr,"需要 %d 金币", toolchangetypeprice);
    changeprice->setString(numchangestr);
    
    cocos2d::ui::Text* changenum = (cocos2d::ui::Text*)changeprice->getChildByTag(22);
    char * changehavestr = (char*) malloc(32);
    sprintf(changehavestr,"拥有 %d 个",MittyToolData::getInstance()->getChangeTypeTool());
    changenum->setString(changehavestr);
    
    
    
    cocos2d::ui::Text* randomprice = (cocos2d::ui::Text*)mTool_RandomType->getChildByTag(19);
    char * numrandomstr = (char*) malloc(32);
    sprintf(numrandomstr,"需要 %d 金币", toolrandomprice);
    randomprice->setString(numrandomstr);
    
    cocos2d::ui::Text* randomnum = (cocos2d::ui::Text*)randomprice->getChildByTag(23);
    char * randomnumhavestr = (char*) malloc(32);
    sprintf(randomnumhavestr,"拥有 %d 个",MittyToolData::getInstance()->getRandomTool());
    randomnum->setString(randomnumhavestr);
    
    
    return true;
}
void ShopScene::selectProduct(cocos2d::Ref* object)
{
//    auto mcheck = (cocos2d::ui::CheckBox*)object;
//    ((cocostudio::)mcheck->getUserObject())
    
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
        productId = PRODUCTID_ONESHOT;
        
        if (mTool_OneShot->isSelected()) {
            desp = "商品用途说明";
            productId = PRODUCTID_NONE;
        }
    }
    if(((Node*)object)->getTag() == mTool_ChangeType->getTag() ){
        mTool_OneShot->setSelected(false);
        mTool_RandomType->setSelected(false);
        desp = "可以改变任一元素的属性！！";
        
        productId = PRODUCTID_CHANGETYPE;
        
        if (mTool_ChangeType->isSelected()) {
            desp = "商品用途说明";
            productId = PRODUCTID_NONE;
        }
        
    }
    if(((Node*)object)->getTag() == mTool_RandomType->getTag() ){
        mTool_OneShot->setSelected(false);
        mTool_ChangeType->setSelected(false);
        desp = "可使任一元素属性随机变化！！";
        
        productId = PRODUCTID_RANDOM;
        
        if (mTool_RandomType->isSelected()) {
            desp = "商品用途说明";
            productId = PRODUCTID_NONE;
        }
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
    
    switch (productId) {
        case PRODUCTID_ONESHOT:
            if (MittyToolData::getInstance()->getcoins() < tooloneshotprice) {
                auto scene = MittyPopup::create();
                scene->setTitleAndText("", "金币不足啦～\n快去游戏赚取金币吧！！！");
                this->addChild(scene);
                return;
            }
            MittyToolData::getInstance()->usecoins(tooloneshotprice);
            MittyToolData::getInstance()->addOneshotTool(1);
            
            break;
         
        case PRODUCTID_RANDOM:
            if (MittyToolData::getInstance()->getcoins() < toolrandomprice) {
                
                auto scene = MittyPopup::create();
                scene->setTitleAndText("", "金币不足啦～\n快去游戏赚取金币吧！！！");
                this->addChild(scene);
                
                return;
            }
            MittyToolData::getInstance()->usecoins(toolrandomprice);
            MittyToolData::getInstance()->addRandomTool(1);
            break;
        case PRODUCTID_CHANGETYPE:
            if (MittyToolData::getInstance()->getcoins() < toolchangetypeprice) {
                auto scene = MittyPopup::create();
                scene->setTitleAndText("", "金币不足啦～\n快去游戏赚取金币吧！！！");
                this->addChild(scene);
                return;
            }
            MittyToolData::getInstance()->usecoins(toolchangetypeprice);
            MittyToolData::getInstance()->addChangeTypeTool(1);
            break;
        default:
            break;
    }
    
    cocos2d::ui::Text* coinnum = (cocos2d::ui::Text*)this->getChildByTag(111)->getChildByTag(15)->getChildByTag(16);
    char * numstr = (char*) malloc(32);
    sprintf(numstr,"X %d",MittyToolData::getInstance()->getcoins());
    coinnum->setString(numstr);
    coinnum->runAction(Sequence::create(ScaleTo::create(0.2, 1.2),ScaleBy::create(0.2, 1), NULL));
    
    cocos2d::ui::Text* oneshotnum = (cocos2d::ui::Text*)this->getChildByTag(111)->getChildByTag(17)->getChildByTag(113)->getChildByTag(17)->getChildByTag(21);
    char * numsohothavestr = (char*) malloc(32);
    sprintf(numsohothavestr,"拥有 %d 个",MittyToolData::getInstance()->getOneshotTool());
    oneshotnum->setString(numsohothavestr);
    oneshotnum->runAction(Sequence::create(ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1), NULL));
    
    cocos2d::ui::Text* randomnum = (cocos2d::ui::Text*)this->getChildByTag(111)->getChildByTag(17)->getChildByTag(115)->getChildByTag(19)->getChildByTag(23);
    char * randomnumhavestr = (char*) malloc(32);
    sprintf(randomnumhavestr,"拥有 %d 个",MittyToolData::getInstance()->getRandomTool());
    randomnum->setString(randomnumhavestr);
    randomnum->runAction(Sequence::create(ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1), NULL));
    
    
    cocos2d::ui::Text* changenum = (cocos2d::ui::Text*)this->getChildByTag(111)->getChildByTag(17)->getChildByTag(114)->getChildByTag(18)->getChildByTag(22);
    char * changenumhavestr = (char*) malloc(32);
    sprintf(changenumhavestr,"拥有 %d 个",MittyToolData::getInstance()->getChangeTypeTool());
    changenum->setString(changenumhavestr);
    
    changenum->runAction(Sequence::create(ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1), NULL));
    
    
}
