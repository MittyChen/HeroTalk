#include "SPCscene.h"
#include "CommonUtils.h"
USING_NS_CC;


using namespace ui;
int SPCScene::winsideresult = -1;
Scene* SPCScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SPCScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SPCScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
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
    fengye->runAction(FadeTo::create(0.5, 200));
    
    this->addChild(fengye);
    
    this->addChild(LayerColor::create(GAMEBG_COLOR,visibleSize.width,visibleSize.height));
    

    
    Label* la = Label::create();
    la->setPosition(Vec2(visibleSize.width/2, visibleSize.height *6/7 - la->getContentSize().height * 2));
    la->setString("猜拳大战!");
    la->setSystemFontSize(72);
    la->setTextColor(Color4B(255, 255, 255, 255));
    this->addChild(la);
    
    scheduleUpdate();
    spcFile[0] =  "sessior.png";
    spcFile[1] =  "punch.png";
    spcFile[2] =  "cloth.png";
    
    int ram = random(0, 2);
    Sprite* SPCs1 = Sprite::create(spcFile[ram]);
    SPCs1->setAnchorPoint(Vec2(0.5, 0));
    SPCs1->setRotation(90);
    SPCs1->setPosition(Vec2(0,Director::getInstance()->getVisibleSize().height/2));
    this->addChild(SPCs1);
    
    auto lambaSessior = [=](Node* pSender)
    {
        int rams = random(0, 2);
        SPCs1->setTexture(spcFile[rams]);
        typeLeft = (SPC_TYPE)rams;
    };
    SPCs1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1),CallFuncN::create(lambaSessior),DelayTime::create(0.1),CallFuncN::create(lambaSessior),DelayTime::create(0.1),CallFuncN::create(lambaSessior), NULL)));
    
    int ram1 = random(0, 2);
    Sprite* SPCs2 = Sprite::create(spcFile[ram1]);
    SPCs2->setAnchorPoint(Vec2(0.5, 0));
    SPCs2->setRotation(-90);
    SPCs2->setPosition(Vec2(Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height/2));
    this->addChild(SPCs2);
    
    auto lambaSessior1 = [=](Node* pSender)
    {
        int rams = random(0, 2);
        SPCs2->setTexture(spcFile[rams]);
        typeRight = (SPC_TYPE)rams;
    };
    
    SPCs2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1),CallFuncN::create(lambaSessior1),DelayTime::create(0.1),CallFuncN::create(lambaSessior1),DelayTime::create(0.1),CallFuncN::create(lambaSessior1), NULL)));
    
    Sprite* winman = Sprite::create("spcnowin.png");
    winman->setPosition(Vec2(visibleSize.width/2, visibleSize.height/3));
    winman->setScale(0.6);
    this->addChild(winman);
    
    
    auto stopAndCheck = [=](Node* pSender)
    {
        SPCs1->stopAllActions();
        SPCs2->stopAllActions();
        
        
        
        if (typeLeft == typeRight){
            
            auto retrySPC = [=](Node* pSender)
            {
                auto scene =  SPCScene::createScene();
                Director::getInstance()->replaceScene(scene);
            };
            
            this->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create(retrySPC) ,NULL));
        }else{
            
            if (leftWin()) {
                winsideresult = 0;
                winman->setTexture("rightwin.png");
                winman->setRotation3D(Vec3(0, 180, 0));
                SPCs1->runAction(Blink::create(2,10));
            }else{
                winsideresult = 1;
                winman->setTexture("rightwin.png");
                winman->setRotation3D(Vec3(0, 0, 0));
                SPCs2->runAction(Blink::create(2,10));
            }
            
            
            auto backtoScnee = [=](Node* pSender)
            {
                Director::getInstance()->popScene();
            };
            
            this->runAction(Sequence::create(DelayTime::create(2), CallFuncN::create(backtoScnee) ,NULL));
        }
    };
    
    this->runAction(Sequence::create(DelayTime::create(3), CallFuncN::create(stopAndCheck) ,NULL));

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(SPCScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
     
    return true;
}

bool SPCScene::leftWin()
{
    switch (typeLeft) {
        case SPC_TYPE::TYPE_CLOTH :
            if (typeRight== TYPE_PUNCH) {
                return true;
            }
            break;
        case SPC_TYPE::TYPE_PUNCH:
            if (typeRight== TYPE_SCESSIOR) {
                return true;
            }
            break;
        case SPC_TYPE::TYPE_SCESSIOR:
            if (typeRight== TYPE_CLOTH) {
                return true;
            }
            break;
        default:
            break;
    }
    
    return false;
}
bool SPCScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    
    return true;
}

int SPCScene::winSide()
{
    return  winsideresult;
}



