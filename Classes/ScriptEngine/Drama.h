//
//  Drama.h
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//

#ifndef __HeroTalk__Drama__
#define __HeroTalk__Drama__

#include "cocos2d.h"
#include "Character.h"
using namespace cocos2d;
USING_NS_CC;

#include <stdio.h>
class Drama: public Node
{
public:
    // 人 时 地
    CC_SYNTHESIZE(int, timeOfDay, TimeOfDay);
    CC_SYNTHESIZE(int, address, AddressName);
    static __Array* characterList;
    static __Array* talkContentList;
    virtual bool init();
//    CREATE_FUNC(Drama);
    static Drama* create(const char*stagePlist,const char* plotPlist);
    
private:
    
};
#endif /* defined(__HeroTalk__Drama__) */
