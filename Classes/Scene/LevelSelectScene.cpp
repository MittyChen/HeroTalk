#include "LevelSelectScene.h"
#include "MainMenuScene.h"
#include "LevelNode.h"
#include "PreGameScene.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
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
    
    
//    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("Levels.plist");
//    
//    __Dictionary* pdic = Dictionary::createWithContentsOfFile(fullPath.c_str());
    

//    Label* levelsec = Label::create("关卡", "Arial", 60);
//    levelsec->setColor(Color3B(0,100,100));
//    levelsec->setPosition( Vec2(20 + levelsec->getContentSize().width/2, -levelsec->getContentSize().height/2 + visibleSize.height -20));
//    
//    this->addChild(levelsec);
    
   
    
    Sprite * mapbg = Sprite::create("selectbg.png");
    mapbg->setPosition(visibleSize/2);
    mapbg->setContentSize(visibleSize);
    this->addChild(mapbg);
    
    
    Sprite * feiting = Sprite::create("qiting.png");
    
    
    feiting->setPosition(Vec2(visibleSize.width+feiting->getContentSize().width/2, visibleSize.height*4/6) );
    feiting->runAction( RepeatForever::create(Sequence::create(MoveTo::create(60, Vec2(-feiting->getContentSize().width/2 - 20,  visibleSize.height*5/6 )),RotateBy::create(0.02,Vec3(0, 180, 0)),MoveTo::create(60, Vec2(visibleSize.width+feiting->getContentSize().width/2, visibleSize.height*4/6)),RotateBy::create(0.02,Vec3(0, -180, 0)), NULL)));
    feiting->setScale(0.1);
    
    
    this->addChild(feiting);
    
    
    ui::ScrollView* m_scrollView =  ui::ScrollView::create();
    m_scrollView->setDirection( ui::ScrollView::Direction::HORIZONTAL);
    m_scrollView->setAnchorPoint(Point::ZERO);
    m_scrollView->setPosition(Vec2::ZERO);
    m_scrollView->setInnerContainerSize(Size(visibleSize.width * (LEVEL_COUNT / 4 ), visibleSize.height));

    
    m_scrollView->setContentSize(visibleSize);
    m_scrollView->setSwallowTouches(false);
    
    this->addChild(m_scrollView);
    
    LayerColor* lcc = LayerColor::create(Color4B(0,0,0,200),visibleSize.width,visibleSize.height);
    
    this->addChild(lcc);
    
    Sprite* door0 = Sprite::create("clouddoorside.png");
    door0->setAnchorPoint(Vec2(0.5, 0));
    door0->setScale(visibleSize.width/door0->getContentSize().width);
    
    
    Sprite* door1 = Sprite::create("clouddoorside.png");
    door1->setAnchorPoint(Vec2(0.5, 0));
    door1->setRotation(180);
    door1->setScale(visibleSize.width/door1->getContentSize().width);
    
    
    door0->setPosition(Vec2(visibleSize.width/2, -door0->getContentSize().height));
    door1->setPosition(Vec2(visibleSize.width/2, visibleSize.height + door1->getContentSize().height));
    
    
    
    this->addChild(door0);
    this->addChild(door1);
    
//    door0->runAction(MoveTo::create(6, Vec2(visibleSize.width/2, -door0->getContentSize().height)));
//    door1->runAction(MoveTo::create(6, Vec2(visibleSize.width/2,  visibleSize.height+door1->getContentSize().height)));
//

    
    auto rootNode = CSLoader::createNode("LevelSelectNode.csb");
    m_scrollView->addChild(rootNode);
    rootNode->setPosition(visibleSize/2);
    rootNode->setName("LevelSelectNode");
    
    
    

    
    //    auto bgSpirit = Sprite::create("selectbg.jpg");
//    bgSpirit->setPosition(Vec2(bgSpirit->getContentSize().width /2 ,  bgSpirit->getContentSize().height/2));
//    bgSpirit->setScale(visibleSize.width / this->getContentSize().width  * 1.5) ;
//    this->addChild(bgSpirit);
//    bgSpirit->setZOrder(-300);
    
    
  
    
    cocos2d::ui::Button* backbtn = cocos2d::ui::Button::create("goback.png");
   
    
//    backbtn->setPosition(backbtn->getContentSize()*2);
    backbtn->setPosition( Vec2( backbtn->getContentSize().width /2  , -backbtn->getContentSize().height/2 + visibleSize.height));
    
    backbtn->setScale(0.5);
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
//    DrawNode * mylines = DrawNode::create();
//    m_scrollView->addChild(mylines);
//    mylines->setPosition(visibleSize/2 );
    
    int maxlevel = UserDefault::getInstance()->getIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED");
    int zOrderFirst = 0;
    for (int i = 1; i < LEVEL_COUNT; i++) {
        LevelNode* lbn = LevelNode::create();
//        int randomRate = cocos2d::random(-40, 40);
        
//        Vec2 lbnPos = Vec2(lbn->getContentSize().width/2 -visibleSize.width/2 + 100 * i + randomRate,  50 + (i%2==0?80:-50) + randomRate );
//        lbn->setPosition(lbnPos);
        
         
        lbn->setLevelCode(i);
        
       
        
//        m_scrollView->addChild(lbn);
      
        
        rootNode->getChildByTag(-1)->addChild((Node*)lbn);

        if (i==1) {
            zOrderFirst = lbn->getGlobalZOrder();
        }
        
            lbn->setPosition(rootNode->getChildByTag(i)->getPosition());
 
//        lbn->setGlobalZOrder(rootNode->getChildByTag(i)->getGlobalZOrder());
        
        destinPos = lbn->getPosition();
    
        
        if(lbn->getLevelCode() == maxlevel){
            
            
            __String* cc = __String::createWithFormat("LevelNode_%d",maxlevel);
            cocos2d::ui::Button* btnlevelnodeMax =  (cocos2d::ui::Button*) lbn->getChildByName(cc->getCString());
            
            m_scrollView->scrollToPercentHorizontal( 100 * ((lbn->getPosition().x * (maxlevel)) / i) / (m_scrollView->getInnerContainerSize().width) , 2, true);
            Sprite* userNode = Sprite::create("hero5.png");
            userNode->setScale(0.2);
            userNode->setPosition(btnlevelnodeMax->getPosition() + Vec2(0,btnlevelnodeMax->getContentSize().height*3/2));
            userNode->setLocalZOrder(btnlevelnodeMax->getLocalZOrder()-1);
            lbn->addChild(userNode);
            maxlevelpos = (Node*)userNode;
        }
        
        
        
//        if(originPos != Vec2::ZERO)
//        {
//            
//            mylines->drawQuadBezier(originPos, i%2==0?Vec2(originPos.x, destinPos.y):Vec2(destinPos.x, originPos.y), destinPos, 20, Color4F(120, 120, 0, 100));
//        }
        
        if(originPos != Vec2::ZERO)
        {

            Sprite* rainbow = Sprite::create("route1.png");
            rainbow->setScale((destinPos - originPos).length()*9/10/rainbow->getContentSize().width);
            //            rainbow->setAnchorPoint(Vec2(0, 0));
            float anglex =(destinPos - originPos).getAngle()*180/3.141592653;
            rainbow->setRotation(-anglex);
            rainbow->setAnchorPoint(Vec2(0.5, 0));
            rainbow->setPosition((destinPos+originPos)/2 + visibleSize/2  );
            rootNode->getChildByTag(-1)->addChild(rainbow);
            rainbow->setGlobalZOrder(zOrderFirst-1);
        }
        
        
        __String* cc = __String::createWithFormat("LevelNode_%d",i);
        cocos2d::ui::Button* btnlevelnode =  (cocos2d::ui::Button*) lbn->getChildByName(cc->getCString());
        btnlevelnode->addTouchEventListener(CC_CALLBACK_2(LevelSelectScene::selectLevelAction, this) );
        
        originPos = lbn->getPosition();
    }
   
    rootNode->getChildByTag(-1)->setGlobalZOrder(zOrderFirst-2);
    rootNode->getChildByTag(-2)->setGlobalZOrder(zOrderFirst-2);
    rootNode->getChildByTag(91)->setGlobalZOrder(zOrderFirst-2);
    
    
    auto gtouchScroll = [maxlevelpos,m_scrollView,door0,door1,lcc,visibleSize](Ref* obj,cocos2d::ui::ScrollView::EventType event)mutable
    {
        switch (event) {
            case cocos2d::ui::ScrollView::EventType::SCROLLING:
            {
//                
//                float cc =  m_scrollView->getInnerContainer()->getPositionPercent().x;
////                CCLOG("cc ===  %f",cc);
//                
//            
//                
//                if (cc > -1) {
//                    lcc->setVisible(false);
//                    
//                    door0->setPosition(Vec2(visibleSize.width/2, 0-door0->getContentSize().height));
//                    door1->setPosition(Vec2(visibleSize.width/2,  visibleSize.height + door1->getContentSize().height));
//                    
//                    
//                }
//                else if(cc < -1 && cc>-2){
//                    lcc->setVisible(true);
//                    lcc->runAction( TintTo::create(2,  255*(cc + 2), 0, 0));
////                    
//                    CCLOG("height0******** %f",-door0->getContentSize().height + (-1-cc)*door0->getContentSize().height);
//                    CCLOG("height1******** %f",visibleSize.height + door1->getContentSize().height - (-1-cc)*(door1->getContentSize().height));
//                    door0->setPosition(Vec2(visibleSize.width/2, -door0->getContentSize().height + (-1-cc)*door0->getContentSize().height));
//                    door1->setPosition(Vec2(visibleSize.width/2, visibleSize.height + door1->getContentSize().height - (-1-cc)*door1->getContentSize().height));
//                }
                
                float controlWidth = visibleSize.width/3;
                if (m_scrollView->getInnerContainer()->getPositionX()+ maxlevelpos->getPosition().x < 0  && m_scrollView->getInnerContainer()->getPositionX()+ maxlevelpos->getPosition().x > -controlWidth) {
                    
                    float perce = (m_scrollView->getInnerContainer()->getPosition().x + maxlevelpos->getPosition().x) / controlWidth;
                    lcc->setVisible(true);
                     CCLOG("perce ===  %f",perce);
                    door0->setPosition(Vec2(visibleSize.width/2, -door0->getContentSize().height + (-perce)*door0->getContentSize().height));
                    door1->setPosition(Vec2(visibleSize.width/2, visibleSize.height + door1->getContentSize().height - (-perce)*door1->getContentSize().height));
                    
                    
                }else if(m_scrollView->getInnerContainer()->getPositionX()+ maxlevelpos->getPosition().x > 0){
                
                    lcc->setVisible(false);
                }
                
                
                break;
            }
                
                
            default:
                break;
        }
    };
    
    
    
    m_scrollView->addEventListener(gtouchScroll);
    
    
    
    
//    SpriteFrameCache *frameCache=SpriteFrameCache::getInstance();
//    frameCache->addSpriteFramesWithFile("Animal_0008.plist");
//    Vector<SpriteFrame*> animations ;
//    char str[100]={0};
//    for(int i = 1; i< 6; i++)
//    {
//        sprintf(str,"Animal_0008_%02d.png",i);
//        SpriteFrame *frame = frameCache->getSpriteFrameByName(str);
//        animations.pushBack(frame);
//    }
//    Animation* animation = Animation::createWithSpriteFrames(animations,0.5f,-1);
//    AnimationCache::getInstance()->addAnimation(animation,"Animal_0008");
//    Animation* danceAnimation = CCAnimationCache::getInstance()->getAnimation("Animal_0008");
//    Animate* animate = CCAnimate::create(danceAnimation);
//    Sprite* grossini = Sprite::create();
//    SpriteFrame* frame = frameCache->getSpriteFrameByName("Animal_0008_01.png");
//    grossini->setDisplayFrame(frame);
//    addChild(grossini);
//    grossini->runAction(animate);
//    grossini->setPosition(20+grossini->getContentSize().width, grossini->getContentSize().height/2 + 15);
// 
//    
//    
//    auto waterSpirit = Sprite::create("water.png");
//    waterSpirit->setPosition(Vec2(waterSpirit->getContentSize().width /2 ,  0));
//    waterSpirit->setScale(visibleSize.width/waterSpirit->getContentSize().width *10, visibleSize.height/waterSpirit->getContentSize().height /20 * maxlevel);
//    
//    // create a Lens3D action
////    ActionInterval* lens = Lens3D::create(10, Size(32, 24), Vec2(100, 180), 150);
//    
//    // create a Waved3D action
//    ActionInterval* waves = Waves3D::create(30, Size(10, 10), 20, 20);
//    auto nodeGrid = NodeGrid::create();
//    nodeGrid->addChild(waterSpirit);
//    nodeGrid->runAction(RepeatForever::create(waves));
//    
////    nodeGrid->runAction(RepeatForever::create(lens));
//    
//    this->addChild(nodeGrid);
//    
//    
//    auto waterpoolSpirit = Sprite::create("poolwater.png");
//    waterpoolSpirit->setPosition(Vec2(visibleSize.width /2 ,  waterpoolSpirit->getContentSize().height/2  - 5));
//    
//    waterpoolSpirit->setScale(visibleSize.width/waterpoolSpirit->getContentSize().width);
//    
//    this->addChild(waterpoolSpirit);
//    
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = CC_CALLBACK_2(LevelSelectScene::onTouchBegan,this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
//    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//    
//    
    
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
         
        auto scene = PreGameScene::createScene(mcode);
        Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    }
}

bool LevelSelectScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}