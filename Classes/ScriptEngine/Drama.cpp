//
//  Drama.cpp
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//戏剧

#include "Drama.h"

#include "DramaDirector.h"

 __Array* Drama::characterList;
 __Array* Drama::talkContentList;


bool Drama::init()
{
    if(!Node::init())
    {
        return false;
    }
    characterList = __Array::create();
    talkContentList= __Array::create();
    
    return true;
}

Drama* Drama::create(const char*stagePlist,const char* plotPlist)
{
    Drama *pRet = new(std::nothrow) Drama();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        
        characterList->removeAllObjects();
        talkContentList->removeAllObjects();
        
        
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename(stagePlist);
        
        __Dictionary* pdic = Dictionary::createWithContentsOfFile(fullPath.c_str());
        
        auto* address = pdic->objectForKey("Address");
        
        auto timeofDay = pdic->objectForKey("Timeofday");
        
        DramaDirector::getInstance()->stageManager->setBackTexture(((__String*)address)->getCString());
        
        characterList = (__Array*)pdic->objectForKey("Chararacters");
        
        std::string talkcontentfullPath = FileUtils::getInstance()->fullPathForFilename(plotPlist);
        
        __Dictionary* talkdic = Dictionary::createWithContentsOfFile(talkcontentfullPath.c_str());
        
        
        talkContentList =  (__Array*)talkdic->objectForKey("TalkContent");
 
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

