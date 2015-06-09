#include "PreGameScene.h"
#include "GameScene.h"
#include "LevelSelectScene.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
static int levelSelected = 0;
Scene* PreGameScene::createScene(int code)
{
    levelSelected = code;
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
    
    scheduleUpdate();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* lco =  LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height);
    this->addChild(lco);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PreGameScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto rootNode = CSLoader::createNode("PreGameScene.csb");
    this->addChild(rootNode);
    rootNode->setName("PreGameScene");
    
    
    
    cocos2d::ui::Button* btnEasy =  (cocos2d::ui::Button*)rootNode->getChildByTag(57);
    btnEasy->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );

    
    cocos2d::ui::Button* btnNormal =  (cocos2d::ui::Button*)rootNode->getChildByTag(59);
    btnNormal->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );
    
    
    cocos2d::ui::Button* btnHard =  (cocos2d::ui::Button*)rootNode->getChildByTag(60);
    btnHard->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );
    
    cocos2d::ui::Button* btnBack =  (cocos2d::ui::Button*)rootNode->getChildByTag(72);
    btnBack->addTouchEventListener(CC_CALLBACK_2(PreGameScene::gotoGame, this) );
 
    return true;
}
bool PreGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void PreGameScene::gotoGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    
    if(type == cocos2d::ui::Widget::TouchEventType::BEGAN){
        
        Node* selectType = (Node*)object;
        
        int tag = selectType->getTag();
        
        switch (tag) {
            case 57:
            {
            
                auto scene = GameScene::createScene(levelSelected,0);
                Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
                
                break;
            }
                
            case 59:
                
            {
                
                auto scene = GameScene::createScene(levelSelected,1);
                Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
                
                break;
                
            }
            case 60:
                
            {
                
                auto scene = GameScene::createScene(levelSelected,2);
                Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
                
                break;
            }
            case 72:
                
            {
                
                auto scene = LevelSelectScene::createScene(0);
                Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
                
                break;
            }
                
            default:
                break;
        }
    }
}