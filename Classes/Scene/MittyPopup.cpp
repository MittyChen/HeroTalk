#include "MittyPopup.h"
#include "GameScene.h"

#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;

Scene* MittyPopup::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MittyPopup::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MittyPopup::init()
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
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MittyPopup::onTouchBegan,this);
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto rootNode = CSLoader::createNode("MittyPopLayer.csb");
    
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
    
    rootNode->setAnchorPoint(Vec2(0.5,0.5));
    rootNode->setPosition(visibleSize/2);
    this->addChild(rootNode);
    rootNode->setName("MittyPopRoot");
    
    rootNode->runAction(Sequence::create(ScaleTo::create(0.15,1.1),ScaleTo::create(0.15, 1.0), NULL));
    
    cocos2d::ui::Button* mButton = (cocos2d::ui::Button*)rootNode->getChildByTag(203)->getChildByTag(214);
    mButton->addTouchEventListener(CC_CALLBACK_2(MittyPopup::exitPop, this) );
    return true;
}
bool MittyPopup::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}
void MittyPopup::exitPop(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    this->removeFromParent();
}

void MittyPopup::setTitleAndText(const char * title,const char * text){
    Node* rootnode = this->getChildByName("MittyPopRoot");
    
    cocos2d::ui::Text* tittle = (cocos2d::ui::Text*)rootnode->getChildByTag(203)->getChildByTag(215);
    tittle->setString(title);
    cocos2d::ui::Text* textt = (cocos2d::ui::Text*)rootnode->getChildByTag(203)->getChildByTag(208);
    textt->setString(text);
    

}
