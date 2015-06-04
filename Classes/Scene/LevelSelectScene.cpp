#include "LevelSelectScene.h"
#include "MainMenuScene.h"
#include "LevelNode.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

static int levelToUnlock = 0;

Scene* LevelSelectScene::createScene(int unclockLevel)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    levelToUnlock = unclockLevel;
    
    // 'layer' is an autorelease object
    auto layer = LevelSelectScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
 
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelSelectScene::init()
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
    
    

    
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("Levels.plist");
    
    __Dictionary* pdic = Dictionary::createWithContentsOfFile(fullPath.c_str());
    

//    Label* levelsec = Label::create("关卡", "Arial", 60);
//    levelsec->setColor(Color3B(0,100,100));
//    levelsec->setPosition( Vec2(20 + levelsec->getContentSize().width/2, -levelsec->getContentSize().height/2 + visibleSize.height -20));
//    
//    this->addChild(levelsec);
    
    ui::ScrollView* m_scrollView =  ui::ScrollView::create();
    m_scrollView->setDirection( ui::ScrollView::Direction::HORIZONTAL);
    m_scrollView->setAnchorPoint(Point::ZERO);
    m_scrollView->setPosition(Vec2::ZERO);
    m_scrollView->setInnerContainerSize(Size(visibleSize.width * (LEVEL_COUNT / 4 ), visibleSize.height));

    
    m_scrollView->setContentSize(visibleSize);
    m_scrollView->setSwallowTouches(false);
    
    this->addChild(m_scrollView);
    
    auto bgSpirit = Sprite::create("selectbg.jpg");
    bgSpirit->setPosition(Vec2(bgSpirit->getContentSize().width /2 ,  bgSpirit->getContentSize().height/2));
    this->addChild(bgSpirit);
    bgSpirit->setZOrder(-300);
    
    cocos2d::ui::Button* backbtn = cocos2d::ui::Button::create("Default/Button_Normal.png");
    
    backbtn->setAnchorPoint(Vec2(0.5, 0.5));
    
//    backbtn->setPosition(backbtn->getContentSize()*2);
    backbtn->setPosition( Vec2(40 + backbtn->getContentSize().width, -backbtn->getContentSize().height/2 + visibleSize.height -40));
    
    backbtn->setScale(2);
    backbtn->setTitleText("返回");
    backbtn->setTitleFontSize(20);
    backbtn->setTitleColor(Color3B(0,200,120));
    this->addChild(backbtn);
    
    
    auto gotomain = [=](Ref* obj,cocos2d::ui::Widget::TouchEventType event)
    {
        Scene* scene = MainMenuScene::createScene();
         Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    };
    
    backbtn->addTouchEventListener(gotomain);
    
    
    Vec2 originPos = Vec2::ZERO;
    Vec2 destinPos = Vec2::ZERO;
    DrawNode * mylines = DrawNode::create();
    m_scrollView->addChild(mylines);
    mylines->setPosition(visibleSize/2 );
    LevelNode* maxindexUnlocked = NULL;
    
    for (int i = 1; i < LEVEL_COUNT; i++) {
        LevelNode* lbn = LevelNode::create();
        int randomRate = cocos2d::random(-40, 40);
        lbn->setPosition(Vec2(lbn->getContentSize().width/2 -visibleSize.width/2 + 200 * i, 50 + (i%2==0?50:-50) + randomRate ));
        lbn->setLevelCode(i);
        
        if(levelToUnlock >= i)
        {
            lbn->unlockLevel();
            
        }
        if(levelToUnlock == i)
        {
            m_scrollView->scrollToPercentHorizontal( 100 * ((lbn->getPosition().x * (i+2)) / i) / (m_scrollView->getInnerContainerSize().width) , 2, true);
        }
        
        //lbn->getPosition().x / (visibleSize.width * (LEVEL_COUNT / 4 ))
        if(!(lbn->isLocked))
        {
            lbn->startRain();
        }
        
        m_scrollView->addChild(lbn);
        destinPos = lbn->getPosition();
        
        if(originPos != Vec2::ZERO)
        {
            mylines->drawQuadBezier(originPos, i%2==0?Vec2(originPos.x, destinPos.y):Vec2(destinPos.x, originPos.y), destinPos, 20, Color4F(120, 120, 0, 100));
        }

        __String* cc = __String::createWithFormat("LevelNode_%d",i);
        
        
        cocos2d::ui::Button* btnlevelnode =  (cocos2d::ui::Button*) lbn->getChildByName(cc->getCString());
        btnlevelnode->addTouchEventListener(CC_CALLBACK_2(LevelSelectScene::selectLevelAction, this) );
        
        auto isunlock = pdic->objectForKey(cc->getCString());
        
        
        if (isunlock != NULL) {
            if(strcmp(((__String*)isunlock)->getCString(), "ok") == 0)
            {
                lbn->unlockLevel();
                lbn->startRain();
                maxindexUnlocked = lbn;
            }
        }
        
        originPos = lbn->getPosition();
        
        if (i==LEVEL_COUNT-1) {
            
            cocos2d::ui::Button* btnlevelnode =  (cocos2d::ui::Button*) lbn->getChildByName(cc->getCString());
            btnlevelnode->addTouchEventListener(CC_CALLBACK_2(LevelSelectScene::selectLevelAction, this) );
            
            
            Sprite* userNode = Sprite::create("whitedot.png");
            Vec2 pos = Vec2(maxindexUnlocked->getPosition().x, maxindexUnlocked->getPosition().y + maxindexUnlocked->getContentSize().height + userNode->getContentSize().height/2);
            userNode->setPosition(pos);
            userNode->setAnchorPoint(Vec2(0.5, 0.5));
            this->addChild(userNode);
        }
    }
   
    
    
    auto waterSpirit = Sprite::create("water.png");
    waterSpirit->setPosition(Vec2(bgSpirit->getContentSize().width /2 ,  0));
    waterSpirit->setScale(visibleSize.width/waterSpirit->getContentSize().width *10, visibleSize.height/waterSpirit->getContentSize().height /10 * maxindexUnlocked->getLevelCode());
    
    // create a Lens3D action
//    ActionInterval* lens = Lens3D::create(10, Size(32, 24), Vec2(100, 180), 150);
    
    // create a Waved3D action
    ActionInterval* waves = Waves3D::create(30, Size(15, 15), 40, 10);
    auto nodeGrid = NodeGrid::create();
    nodeGrid->addChild(waterSpirit);
    nodeGrid->runAction(RepeatForever::create((Sequence*)Sequence::create(waves, NULL)));
    this->addChild(nodeGrid);
    
    
    auto waterpoolSpirit = Sprite::create("poolwater.png");
    waterpoolSpirit->setPosition(Vec2(visibleSize.width /2 ,  0));
    
    waterpoolSpirit->setScale(visibleSize.width/waterpoolSpirit->getContentSize().width);
    
    this->addChild(waterpoolSpirit);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LevelSelectScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    
    
    auto removeLamda = [=](Ref* pSender)
    {
       
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("levelselect.mp3",true);
        
    };
    
    this->runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(removeLamda), NULL) );
    

    return true;
}

void LevelSelectScene::update(float delta)
{
    
}

void LevelSelectScene::selectLevelAction(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED) {
        int mcode = ((LevelNode*)(((Node*) pSender)->getParent()))->getLevelCode();
        CCLOG("关卡 %d 被选择",mcode);
        
        auto scene = GameScene::createScene(mcode);
        Director::getInstance()->replaceScene(scene);
        
    }
}

bool LevelSelectScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}