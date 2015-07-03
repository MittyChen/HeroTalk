//
//  DramaDirector.cpp
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//导演

#include "DramaDirector.h"
#include "Character.h"
#include "Drama.h"
#include "SimpleAudioEngine.h"

const char* DramaDirector::currentTalkcontent;
DramaDirector* DramaDirector::_instance;
int DramaDirector::currentTalkingIndex;
Vector<Ref*> DramaDirector::contentListFinal;

std::vector<Character*> DramaDirector::characterListFinal;
std::function<void(Node*)>  DramaDirector::dramaFinishfunc;

bool DramaDirector::init()
{
    if(!Node::init())
    {
        return false;
    }
    
    speakmanager = LoudSpeaker::create();
    stageManager = StageManager::create();
   
    
    currentTalkcontent = "";
    currentTalkingIndex = 0;
    contentListFinal.empty();
    characterListFinal.clear();
    
    return true;
}

DramaDirector* DramaDirector::getInstance()
{
    if(_instance == NULL)
    {
        _instance =   DramaDirector::create();
    }
    return _instance;
}

void DramaDirector::setDelegate(Node& delegate,const char* bgmusic,const char* stagePlist,const char* plotPilist,const std::function<void(Node*)> func)
{
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgmusic);
    
    mDrama = Drama::create(stagePlist,plotPilist);
    dramaFinishfunc = func;
    
    stageManager->setDelegate(delegate);
    DramaDirector::getInstance()->characterMakeUp(delegate);
    
    delegate.schedule(schedule_selector(DramaDirector::tapToTakeForward) , 3, CC_REPEAT_FOREVER, 3);

    this->addChild(speakmanager);
    this->addChild(stageManager);
    this->addChild(mDrama);
    delegate.addChild(this);
    
    Vec2 originpos = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size originsize = Director::getInstance()->getVisibleSize();
    
    
    Label* skipLabel = Label::create();
    skipLabel->setString("跳过");
    skipLabel->setSystemFontSize(22);
    skipLabel->setAnchorPoint(Vec2(0.5,0.5));
    
    cocos2d::ui::Button * skipbutton = cocos2d::ui::Button::create("Default/Button_Normal.png");
    skipbutton->setVisible(true);
    
    skipbutton->setPosition(Vec2(originpos.x+originsize.width - 20 - skipbutton->getContentSize().width,originpos.y + originsize.height - 20 - skipbutton->getContentSize().height));
    
    skipbutton->setZOrder(STAGE_MASK_ZORDER+10);
    skipbutton->addChild(skipLabel);
    skipLabel->setPosition(skipbutton->getContentSize()/2);
    skipLabel->setTextColor(Color4B(0,200,120,255));
    
    skipbutton->setScale(2.0);
    auto skipDrama = [=](Ref* obj,cocos2d::ui::Widget::TouchEventType event)
    {
        currentTalkingIndex = contentListFinal.size() + 1;
        tapToTakeForward(0);
    };
    
    skipbutton->addTouchEventListener(skipDrama);
    delegate.addChild(skipbutton);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(bgmusic);
   
}

void DramaDirector::characterMakeUp(Node& delegate)
{
    characterListFinal.clear();
    contentListFinal.clear();
    if(stageManager->stageIsReady)
    {
        for(int i = 0; i < mDrama->characterList->count();++i)
        {
            __Dictionary* pdic = (__Dictionary*)mDrama->characterList->getObjectAtIndex(i);
            const char* characterName = ((__String*)pdic->objectForKey("CharacterName"))->getCString();
            const char*  nickName = ((__String*)pdic->objectForKey("CharacterNickName"))->getCString();
            const char* portraitTexture = ((__String*)pdic->objectForKey("CharacterPortraitTexture"))->getCString();
            int characterid = atoi(((__String*)pdic->objectForKey("CharacterID"))->getCString())  ;
            int positionindex = atoi(((__String*)pdic->objectForKey("PositionIndex"))->getCString());
            
            Character* onetest = Character::create();
            onetest->setCharacterName(characterName);
            onetest->setNickName(nickName);
            onetest->setPortraitTexture(portraitTexture);
            onetest->showPortraitAtPosition(positionindex, delegate);
            onetest->setID(characterid);
            characterListFinal.push_back(onetest);
        }
    }
    
    
    for(int i = 0; i < mDrama->talkContentList->count();++i)
    {
        __String* mstring = (__String*)mDrama->talkContentList->getObjectAtIndex(i);
        
        contentListFinal.pushBack(mstring);
    }
}

void DramaDirector::tapToTakeForward(float deltatime)
{
    if(currentTalkingIndex >= contentListFinal.size())
    {
        for(int i = 0; i < characterListFinal.size();++i)
        {
            characterListFinal.at(i)->removeFromParent();
        }
        
        if(this->getParent())
        {
            this->getParent()->unschedule(schedule_selector(DramaDirector::tapToTakeForward));
            this->getParent()->runAction(CallFuncN::create(dramaFinishfunc));
            this->removeFromParent();
        }
       
        stageManager->destroyStage();
        _instance = NULL;
        Character::talkContent = NULL;
        
        
        characterListFinal.clear();
        contentListFinal.clear();
        return;
    }
    
    __String* mstring = (__String*)(contentListFinal.at(currentTalkingIndex));
    
    
    char* mchars =  const_cast<char*>(mstring->getCString());
    int j = 0;
    char* marry [3];
    const char * split = ":";
    char * p;
    p = strtok (mchars,split);
    
    while(p!=NULL) {
        marry[j] = p;
        j++;
        p = strtok(NULL,split);
    }
    
    char* currentActionType = marry[1];
    
    int currentCharacter = atoi(marry[0]);
    
    if(strcasecmp(currentActionType,  "Talk") == 0)
    {
         currentTalkcontent =  marry[2];
        if(currentCharacter == 99)//旁白
        {
            DramaDirector::speakmanager->speakToTarget(DramaEventType::EVENT_DRAMA_SOUND_OFF);
        }
        else
        {
            for(int i = 0; i < characterListFinal.size();i++)
            {
                if((characterListFinal.at(i))->getID() == currentCharacter)
                {
                    DramaDirector::speakmanager->speakToTarget(DramaEventType((int)(DramaEventType::EVENT_DRAMA_CHARACTER0_TALK) + currentCharacter -100));
                }
            }
        }
        
    }

    currentTalkingIndex ++;
}