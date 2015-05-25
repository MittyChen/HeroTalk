//
//  Character.cpp
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//角色

#include "Character.h"
#include "DramaDirector.h"
Label* Character::talkContent;
bool Character::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    positionIndex = 0;
    if(!talkContent)
    {
        talkContent = Label::create();
    }
    
    shouldRotatewhenShowPortrait = false;
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Character::setCharacterRotate), "setCharacterRotate", nullptr);
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Character::setCharacterState), "setCharacterState", NULL);
    return true;
}
void Character::setID(int mid)
{
    _mid = mid;
    this->setTag(_mid);
}

int Character::getID()
{
    return this->getTag();
}

/*  角色出现的屏幕位置说明
    ------------------
        |       |
        |       |
    ------------------
        |       |
    0   |   1   |    2
    ------------------
        |       |
        |       |
    ------------------
 */
void Character::showPortraitAtPosition(int posindex,Node& parent)
{
    positionIndex = posindex;
    Vec2 originpos = Director::getInstance()->getVisibleOrigin();
    Size originsize = Director::getInstance()->getVisibleSize();
    
    if(!parent.getChildByTag(STAGE_TALK_CONTENT_TAG))
    {
        talkContent->setAnchorPoint(Vec2(0.5,0.5));
        talkContent->setString( DramaDirector::currentTalkcontent);
        talkContent->setSystemFontSize(36);
        talkContent->setPosition( Vec2(originpos.x + originsize.width/2, originpos.y + originsize.height/5) );
        talkContent->setZOrder(STAGE_MASK_ZORDER + 11);
        talkContent->setTag(STAGE_TALK_CONTENT_TAG);
        parent.addChild(talkContent);
    }
    if(shouldRotatewhenShowPortrait)
    {
        this->setRotation3D(Vec3(-90,0,0));
        
        this->runAction(Sequence::create(RotateTo::create(0.6,Vec3(0,0,0)),RotateBy::create(0.2,Vec3(-10,0,0)),RotateBy::create(0.2,Vec3(10,0,0)),RotateBy::create(0.2,Vec3(-5,0,0)),NULL));
    }
    
    this->setTexture(_portraitTexture);
    
    if(originsize.width> originsize.height)//横屏
    {
        float scaley = originsize.height*7/12 / this->getContentSize().height;
        this->setScale(scaley);
    }else//竖屏
    {
        float scalex =  (originsize.width/3) / this->getContentSize().width;
        float scaley = (originsize.height/3) / this->getContentSize().height;
        this->setScale(scalex>scaley?scaley:scalex);
    }
    
    this->setAnchorPoint(Vec2::ZERO);
    float jumpheight = 20;
    switch (posindex) {
        case 0:
            this->setPosition(Vec2(0,(originpos.y+originsize.height)/3 - jumpheight));
            break;
        case 2:
            this->setPosition(Vec2((originpos.x+originsize.width - this->getContentSize().width*this->getScale()),(originpos.y+originsize.height)/3 - jumpheight));
            break;
        case 1:
            this->setPosition(Vec2((originpos.x+originsize.width)/3,(originpos.y+originsize.height)/3 - jumpheight));
            break;
        default:
            break;
    }
    
    this->runAction(Sequence::create(DelayTime::create(1.0),JumpBy::create(0.5, Vec2(0,jumpheight), jumpheight, 1), NULL) );
    
    characterNamelabel = Label::create();
    characterNamelabel->setString(this->getCharacterName());
    characterNamelabel->setSystemFontSize(56);
    characterNamelabel->setTextColor(Color4B(255, 0, 255, 255));
    characterNamelabel->setPosition( Vec2(originpos.x + originsize.width/4, originpos.y + originsize.height/5) );
    characterNamelabel->setZOrder(STAGE_MASK_ZORDER + 11);
    this->addChild(characterNamelabel);
    
    parent.addChild(this);
}

void Character::hidePortrait(std::function<void()> nextAction)
{
    CallFunc* nextAc = CallFunc::create(nextAction);
    if(!shouldRotatewhenShowPortrait){
        this->runAction(Sequence::create(FadeOut::create(0.3),nextAc, NULL));
    }else{
        this->runAction(Sequence::create(RotateTo::create(0.4,Vec3(-90,0,0)),nextAc, NULL));
    }
}

void Character::setCharacterRotate(Ref* pSender)
{
    shouldRotatewhenShowPortrait = true;
    this->runAction(FadeIn::create(0.01));
}
/*
   0------隐藏状态
   1------幕布后面
   2------幕布前面
   3------销毁态
*/

void Character::setCharacterState(Ref* pSender)
{
    int state = 0;
    if(positionIndex == 0)
    {
        state = LoudSpeaker::muser0state;
    }else if (positionIndex == 2)
    {
        state = LoudSpeaker::muser1state;
    }else
    {
        state = LoudSpeaker::muser2state;
    }
    
//    
//    Node* parent = DramaDirector::getInstance()->mdelegate;
//    auto mask =  this->getParent()->getChildByTag(STAGE_MASK_TAG);
//    int maskZorder = mask->getZOrder();
    switch (state) {
        case 0:
            this->setVisible(false);
            break;
            
        case 1:
            this->setVisible(true);
            this->setZOrder(STAGE_MASK_ZORDER-1);
            break;
        case 2:
            
            this->setVisible(true);
            this->setZOrder(STAGE_MASK_ZORDER+1);
            break;
         
        case 3:
            RemoveSelf();
            break;
        default:
            break;
    }
    talkContent->setString(DramaDirector::currentTalkcontent);
}


void Character::setNickName(const char* name)
{
    _nickname = name;
}

const char* Character::getNickName()
{
    return _nickname;
}

void Character::goFront()
{
    this->setVisible(true);
    this->setZOrder(STAGE_MASK_ZORDER+1);
}

void Character::goBack()
{
    this->setVisible(true);
    this->setZOrder(STAGE_MASK_ZORDER-1);
}

