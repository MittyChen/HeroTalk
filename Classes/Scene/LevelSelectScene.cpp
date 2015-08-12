#include "LevelSelectScene.h"
#include "MainMenuScene.h"
#include "LevelNode.h"
#include "PreGameScene.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "CommonUtils.h"
USING_NS_CC;

static int levelToUnlock = 0;


cocos2d::Vector<Ref*>  LevelSelectScene::mLevelStr;
static LevelNode* selectNode = NULL;
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
//    scheduleUpdate();
    
    if (mLevelStr.size()>0) {
        mLevelStr.clear();
    }
    
    for (int i = 1; i < LEVEL_COUNT; i++) {
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("LevelDesign.plist");
        
        __Dictionary* pdic = Dictionary::createWithContentsOfFile(fullPath.c_str());
        std::string mkey = StringUtils::toString(i);
        auto* value = pdic->objectForKey(mkey);
        mLevelStr.pushBack(value);
    }
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* fengye = Sprite::create("fengye.png");
    fengye->setPosition(visibleSize/2);
    fengye->setAnchorPoint(Vec2(0.5,0.5));
    fengye->runAction(FadeTo::create(0.001, 255));
    float scaleX = visibleSize.width/fengye->getContentSize().width;
    float scaleY = visibleSize.height/fengye->getContentSize().height;
    float scaleFactor = scaleX>scaleY?scaleX:scaleY;
    fengye->setScale(scaleFactor);
    //出现一次 淡化至透明
    fengye->runAction(FadeTo::create(0.5, 255));
    this->addChild(fengye);
    
    LayerColor* lcc = LayerColor::create(Color4B(0,0,0,200),visibleSize.width,visibleSize.height);
    this->addChild(lcc);
    
    ui::ScrollView* m_scrollView =  ui::ScrollView::create();
    m_scrollView->setDirection( ui::ScrollView::Direction::VERTICAL);
    m_scrollView->setAnchorPoint(cocos2d::Point::ZERO);
    m_scrollView->setPosition(Vec2::ZERO);
    m_scrollView->setContentSize(visibleSize);
    m_scrollView->setSwallowTouches(false);
    m_scrollView->setName("ScrollViewNode");
    this->addChild(m_scrollView);
    
    auto rootNode = CSLoader::createNode("LevelSelectNode.csb");
    m_scrollView->addChild(rootNode);
    rootNode->setPosition(visibleSize/2);
    rootNode->setName("LevelSelectNode");
    rootNode->runAction(FadeTo::create(0.5, 180));
    rootNode->getChildByTag(91)->setScale(visibleSize.width/rootNode->getChildByTag(91)->getContentSize().width);
    rootNode->getChildByTag(91)->setPosition(-1 * visibleSize/2);
    
    
    m_scrollView->setInnerContainerSize( cocos2d::Size(visibleSize.width, rootNode->getChildByTag(91)->getContentSize().height*rootNode->getChildByTag(91)->getScale()));
    
    
    cocos2d::ui::Button* backbtn = cocos2d::ui::Button::create("goback.png");
    backbtn->setPosition( Vec2( backbtn->getContentSize().width /2  , -backbtn->getContentSize().height/2 + visibleSize.height));
    backbtn->setScale(0.7);
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
    
    Node* maxlevelpos = NULL;
    
    int maxlevel = UserDefault::getInstance()->getIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED");
    if (maxlevel  < levelToUnlock) {
        __String* ss = __String::createWithFormat("HERO_TALK_UNLOCKED_LEVEL_%d",levelToUnlock);
        UserDefault::getInstance()->setBoolForKey(ss->getCString(),true);
        UserDefault::getInstance()->setIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED",levelToUnlock);
        maxlevel = levelToUnlock;
    }
    
    int zOrderFirst = 0;
    float maxY = 0;
    float allY = 0;
    Sprite* userNode = Sprite::create("diamond.png");
    for (int i = 1; i < LEVEL_COUNT; i++) {
        LevelNode* lbn = LevelNode::create();
        lbn->setLevelCode(i);
        rootNode->getChildByTag(91)->addChild((Node*)lbn);
        if (i==1) {
            zOrderFirst = lbn->getGlobalZOrder();
        }
        lbn->setPosition(rootNode->getChildByTag(i)->getPosition() - visibleSize/2);
        lbn->setTag(-1*i);
        destinPos = lbn->getPosition();
    
        if(lbn->getLevelCode() == maxlevel){
            
            __String* cc = __String::createWithFormat("LevelNode_%d",maxlevel);
            cocos2d::ui::Button* btnlevelnodeMax =  (cocos2d::ui::Button*) lbn->getChildByName(cc->getCString());
            maxY = lbn->getPosition().y;
            
            userNode->setScale(0.3);
            userNode->setPosition(btnlevelnodeMax->getPosition() + Vec2(0,btnlevelnodeMax->getContentSize().height*2));
            userNode->setLocalZOrder(btnlevelnodeMax->getLocalZOrder()-1);
            lbn->addChild(userNode);
            
            userNode->runAction(RepeatForever::create(JumpTo::create(1, btnlevelnodeMax->getPosition() + Vec2(0,btnlevelnodeMax->getContentSize().height*2) , 15, 1)));
            
            
            Sprite* lightp = Sprite::create("whitedotlight.png");
            lightp->setPosition(btnlevelnodeMax->getPosition() + Vec2(0,0));
            lightp->setScale(2);
            lbn->addChild(lightp);
            lightp->runAction(RepeatForever::create(Blink::create(1, 1)));
            
            
            maxlevelpos = (Node*)lbn;
        }
        
        if(originPos != Vec2::ZERO)
        {

            Sprite* rainbow = Sprite::create("levellink.png");
            rainbow->setScale((destinPos-originPos).length()/rainbow->getContentSize().width
                              );
            
            float anglex =(destinPos - originPos).getAngle()*180/3.141592653;
            rainbow->setRotation(-anglex);
            
            rainbow->setAnchorPoint(Vec2(0.5, 0.5));
            rainbow->setPosition((destinPos+originPos)/2 + visibleSize/2  );
            
            rootNode->getChildByTag(91)->addChild(rainbow);
            rainbow->setGlobalZOrder(zOrderFirst-1);
            
            rainbow->setColor(lbn->cloud->getColor());
        }
        
        
        __String* cc = __String::createWithFormat("LevelNode_%d",i);
        cocos2d::ui::Button* btnlevelnode =  (cocos2d::ui::Button*) lbn->getChildByName(cc->getCString());
        btnlevelnode->addTouchEventListener(CC_CALLBACK_2(LevelSelectScene::selectLevelAction, this) );
        
        originPos = lbn->getPosition();
        
        
        if (i == LEVEL_COUNT-1) {
            allY = lbn->getPositionY();
            m_scrollView->scrollToPercentVertical( 100 * ( 1 - maxY / allY) , 1, true);
            userNode->setGlobalZOrder(lbn->getGlobalZOrder()+1);
        }
        
        
    }
   
    rootNode->getChildByTag(-111)->setGlobalZOrder(zOrderFirst-2);
    rootNode->getChildByTag(-2)->setGlobalZOrder(zOrderFirst-2);
    rootNode->getChildByTag(91)->setGlobalZOrder(zOrderFirst-2);
    
    if(maxlevelpos!=NULL)
    {
        float nodeMaxPosX = visibleSize.width *  m_scrollView->getInnerContainer()->getPositionPercent().x + maxlevelpos->getPosition().x;
        float controlWidth =  nodeMaxPosX - visibleSize.width;
        
    }
      lcc->setVisible(false);
   
    
    auto removeLamda = [=](Ref* pSender)
    {
       
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("levelselect.mp3",true);
        
    };
    
    this->runAction(Sequence::create(DelayTime::create(1),CallFuncN::create(removeLamda), NULL) );
    
    return true;
}
LevelSelectScene::~LevelSelectScene(){

//    if (mLevelStr.size()>0 ) {
//        mLevelStr.clear();
//    }
//    
//    if (selectNode != NULL) {
//        selectNode->release();
//        selectNode = NULL;
//    }
}
void LevelSelectScene::update(float delta)
{
    
}

void LevelSelectScene::selectLevelAction(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type == ui::Widget::TouchEventType::ENDED) {
        
        int mcode = ((LevelNode*)(((Node*) pSender)->getParent()))->getLevelCode();
        selectNode = LevelNode::create();
        selectNode->setLevelCode(mcode);
        auto scene = PreGameScene::createScene(selectNode);
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    }
}

bool LevelSelectScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}