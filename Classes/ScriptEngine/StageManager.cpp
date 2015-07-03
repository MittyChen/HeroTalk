//
//  StageManager.cpp
//  HeroTalk
//
//  Created by chenwei on 15/4/29.
//
//

#include "StageManager.h"

 Sprite* StageManager::talkframe = NULL;
// Sprite* StageManager::backTexture= NULL;
 Label* StageManager::talkContent= NULL;


bool StageManager::init()
{
    stageIsReady = false;
    stagemask = LayerColor::create(Color4B(0,0,0,150));
    stagemask->retain();
    //此方法限定尽在剧情位置进行DramaDirector实例化
    stagemask->setTag(STAGE_MASK_TAG);
    stagemask->setZOrder(STAGE_MASK_ZORDER);
    Vec2 originpos = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size originsize = Director::getInstance()->getVisibleSize();

    
    talkframe = Sprite::create("talkframe.png");
    talkframe->setAnchorPoint(Vec2(0,0));
    talkframe->setScale( originsize.width /talkframe->getContentSize().width, originsize.height*0.4/talkframe->getContentSize().height );
    talkframe->setZOrder(STAGE_MASK_ZORDER + 10);
    talkframe->setVisible(false);
    talkframe->retain();
    
    backTexture = Sprite::create();
    
    return true;
}
//StageManager* StageManager::create(Node* parent)
//{
//    StageManager* res = StageManager::create();
//    parent->addChild((Node*)res);
//    return  res;
//}
void StageManager::setDelegate(Node& delegate)
{
   
    delegate.addChild(stagemask);
    delegate.addChild(talkframe);
    delegate.addChild(backTexture);
    
    stageIsReady = true;
}
void StageManager::setBackTexture(const char* _backTexture)
{
    Vec2 originpos = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size originsize = Director::getInstance()->getVisibleSize();

    this->_backTexture = _backTexture;
    backTexture->setTexture(_backTexture);
    backTexture->setAnchorPoint(Vec2(0,0));
    backTexture->setScale( originsize.width /backTexture->getContentSize().width, originsize.height/backTexture->getContentSize().height );
    backTexture->setZOrder(STAGE_MASK_ZORDER + 1);
    
}


void updateTalkContent(Node* delegate)
{

}

void StageManager::setBGState(int state)
{
    talkframe->setVisible(true);
    backTexture->setZOrder(state);
}
void StageManager::destroyStage()
{
//    stagemask = NULL;
//    stagemask->release();
//    talkframe = NULL;
//    talkframe->release();
//    backTexture = NULL;
//    backTexture->release();
    
    
}
