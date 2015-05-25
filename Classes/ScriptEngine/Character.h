//
//  Character.h
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//

#ifndef __HeroTalk__Character__
#define __HeroTalk__Character__

#include "cocos2d.h"

USING_NS_CC;

#include <stdio.h>

class Character: public cocos2d::Sprite
{
public:
    CREATE_FUNC(Character);
    void takeAction();
    virtual bool init();
    void showPortraitAtPosition(int posindex,Node& parent);
    void hidePortrait(std::function<void()> nextAction);
    void setCharacterRotate(Ref* pSender);
    void setCharacterState(Ref* pSender);
    
    CC_SYNTHESIZE(const char*, _name, CharacterName);
    
//    CC_SYNTHESIZE(const char*, _nickname, NickName);
    
    const char* _nickname;
    void setNickName(const char* name);
    const char* getNickName();
    
    
    
    
    CC_SYNTHESIZE(int, _age, Age);
    
    int _mid;
    void setID(int mid);
    int getID();
    
    CC_SYNTHESIZE(int, _gendor, Gendor);// 女:0 男:1 其他:-1
    CC_SYNTHESIZE(const char*, _portraitTexture, PortraitTexture);
    CC_SYNTHESIZE(const char*, _commonTexture, CommonTexture);
    
    
    int positionIndex;
    bool shouldRotatewhenShowPortrait;
    
    void goFront();
    
    void goBack();
public:
    
    Vector<Sequence*> _actionQueue;
    
    
    
    static Label* talkContent;
    
    Label* characterNamelabel;
};


#endif /* defined(__HeroTalk__Character__) */
