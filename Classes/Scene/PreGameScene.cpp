#include "PreGameScene.h"
#include "GameScene.h"
#include "LevelSelectScene.h"
#include "cocostudio/CocoStudio.h"
#include "CommonUtils.h"
#include "Ads.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
static int levelSelected = 0;
static LevelNode* selectNodePre = NULL;

Scene* PreGameScene::createScene(LevelNode* node)
{
    levelSelected = node->getLevelCode() ;
    
    selectNodePre = LevelNode::create();
    
    selectNodePre->setLevelCode(levelSelected);
    
    selectNodePre->retain();
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PreGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PreGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    scheduleUpdate();
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
//    LayerColor* lco =  LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height);
//    this->addChild(lco);
    
    
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
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PreGameScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto rootNode = CSLoader::createNode("PreGameScene.csb");
    this->addChild(rootNode);
    rootNode->setName("PreGameScene");
    
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
    
//    auto waterpoolSpirit = Sprite::create("poolwater.png");
//    waterpoolSpirit->setAnchorPoint(Vec2(0, 0));
//    waterpoolSpirit->setScale(visibleSize.width/waterpoolSpirit->getContentSize().width);
    
    
    
    cocos2d::ui::Button* btnEasy =  (cocos2d::ui::Button*)rootNode->getChildByTag(57);
    btnEasy->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );

    
    cocos2d::ui::Button* btnNormal =  (cocos2d::ui::Button*)rootNode->getChildByTag(59);
    btnNormal->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );
    
    
    cocos2d::ui::Button* btnHard =  (cocos2d::ui::Button*)rootNode->getChildByTag(60);
    btnHard->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );
    
    cocos2d::ui::Button* btnBack =  (cocos2d::ui::Button*)rootNode->getChildByTag(72);
    btnBack->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );
 
//    auto waterSpirit = Sprite::create("water.png");
//    waterSpirit->setPosition(Vec2(waterSpirit->getContentSize().width /2 ,  0));
//    waterSpirit->setScale(visibleSize.width/waterSpirit->getContentSize().width * 20, (waterpoolSpirit->getContentSize().height *waterpoolSpirit->getScaleY()  - 10)/waterSpirit->getContentSize().height );
//    
//    // create a Waved3D action
//    ActionInterval* waves = Waves3D::create(30, cocos2d::Size(10, 10), 15, 20);
//    auto nodeGrid = NodeGrid::create();
//    nodeGrid->addChild(waterSpirit);
//    nodeGrid->runAction(RepeatForever::create(waves));
//    this->addChild(nodeGrid);
    
    if ( LevelSelectScene::mLevelStr.size()>0 ) {
        
        LevelSelectScene::mLevelStr.clear();
    }
    
    for (int i = 1; i < LEVEL_COUNT; i++) {
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("LevelDesign.plist");
        
        __Dictionary* pdic = Dictionary::createWithContentsOfFile(fullPath.c_str());
        std::string mkey = StringUtils::toString(i);
        
        auto* value = pdic->objectForKey(mkey);
        LevelSelectScene::mLevelStr.pushBack(value);
    }
    
    
   
//    this->addChild(waterpoolSpirit);
    
    
    return true;
}
bool PreGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void PreGameScene::gotoGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    
    if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
        
        
        Node* selectType = (Node*)object;
        
        int tag = selectType->getTag();
        selectNodePre->setLevelCode(levelSelected);
        
        switch (tag) {
            case 57:
            {
            
                auto scene = GameScene::createScene(selectNodePre,0);
                Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
                
                break;
            }
                
            case 59:
                
            {
                auto scene = GameScene::createScene(selectNodePre,1);
                Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
                
                break;
                
            }
            case 60:
                
            {
                auto scene = GameScene::createScene(selectNodePre,2);
                Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
                
                break;
            }
            case 72:
                
            {
                
                auto scene = LevelSelectScene::createScene(selectNodePre->getLevelCode());
                Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
                
                break;
            }
                
            default:
                break;
        }
        if (selectNodePre!=NULL) {
            selectNodePre->release();
            selectNodePre = NULL;
        }
    }
}



void PreGameScene::onEnter(){
    Layer::onEnter();
    Ads::getInstance()->showAds(AdsType::AD_TYPE_BANNER,2);
    
}
void PreGameScene::onExit(){
    Ads::getInstance()->hideAds(AdsType::AD_TYPE_BANNER,2);
    Layer::onExit();
}

