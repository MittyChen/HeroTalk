#include "StoryScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LevelSelectScene.h"

#include "TalkScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocostudio;
using namespace ui;

Scene* StoryScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StoryScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}




// on "init" you need to initialize your instance
bool StoryScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
 
    
    scheduleUpdate();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(StoryScene::onTouchBegan,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
//    auto lambalabel = [=](Node* pSender)
//    {
//        Label* levelsec = Label::create("ALL FOR BENIFIT", "Arial", 80);
//        levelsec->setColor(Color3B(255,255,255));
//        levelsec->setPosition(Director::getInstance()->getVisibleSize()/2);
//        levelsec->runAction(Sequence::create(FadeIn::create(2),DelayTime::create(2),FadeOut::create(2), NULL));
//        
//        this->addChild(levelsec);
//    };
//
//    
//    SpriteFrameCache *frameCache=SpriteFrameCache::getInstance();
//    frameCache->addSpriteFramesWithFile("Animal_0651.plist");
//    Vector<SpriteFrame*> animations ;
//    char str[100]={0};
//    for(int i = 1; i< 10; i++)
//    {
//        sprintf(str,"Animal_0651_%02d.png",i);
//        SpriteFrame *frame = frameCache->getSpriteFrameByName(str);
//        animations.pushBack(frame);
//    }
//    Animation* animation = Animation::createWithSpriteFrames(animations,0.25f,1);
//    AnimationCache::getInstance()->addAnimation(animation,"Animal_0651");
//    Animation* danceAnimation = CCAnimationCache::getInstance()->getAnimation("Animal_0651");
//    Animate* animate = CCAnimate::create(danceAnimation);
//    Sprite* grossini = Sprite::create();
//    SpriteFrame* frame = frameCache->getSpriteFrameByName("Animal_0651_01.png");
//    grossini->setDisplayFrame(frame);
//    addChild(grossini);
//    
//    grossini->runAction( Sequence::create(animate,FadeOut::create(1),CallFuncN::create(lambalabel),NULL));
//
//    grossini->setScale(3);
//    grossini->setPosition(Director::getInstance()->getVisibleSize()/2);

    
    
    const char*  storyArr[3] = {"人生不只有黑白","从现在开始","你需要色彩"};
    
    Label* storyLabel = Label::create();
    storyLabel->setSystemFontSize(36);
    storyLabel->setString(storyArr[0]);
    storyLabel->setPosition(Director::getInstance()->getVisibleSize()/2);
    this->addChild(storyLabel);
    
    
    auto currentLine = 0;
    auto gonextStroyline = [=](Node* pSender)mutable
    {
        currentLine ++;
       
        if (currentLine <= 2 ) {
            storyLabel->setString(storyArr[currentLine]);
        }else
        {
            
            
            
            auto scene = TalkScene::createScene();
            // run
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        }
    };
    
    
    CallFuncN* nextLine = CallFuncN::create(gonextStroyline);
    
    //gonextStroyline(NULL);
    storyLabel->runAction(Repeat::create(Sequence::create(FadeIn::create(2),DelayTime::create(2),FadeOut::create(2),nextLine, NULL), 3));
//    storyLabel->runAction(nextLine);
    return true;
}
bool StoryScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
void StoryScene::playClickCallBack(Ref* sender,cocos2d::ui::TouchEventType type)
{
    
}