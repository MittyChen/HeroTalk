//
//  LoudSpeaker.h
//  HeroTalk
//
//  Created by chenwei on 15/4/30.
//
//

#ifndef __HeroTalk__LoudSpeaker__
#define __HeroTalk__LoudSpeaker__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

enum DramaEventType
{
    EVENT_DRAMA_CHARACTER_ROTATE,
    EVENT_DRAMA_SOUND_OFF,//场外音
    EVENT_DRAMA_CHARACTER0_TALK,
    EVENT_DRAMA_CHARACTER1_TALK,
    EVENT_DRAMA_CHARACTER2_TALK
    
};


class LoudSpeaker: public Node
{
public:
    CREATE_FUNC(LoudSpeaker);
    
    virtual bool init();
    
    void speakToTarget(DramaEventType type);
    
    static int muser0state;
    
    static int muser1state;
    
    static int muser2state;
    
};
#endif /* defined(__HeroTalk__LoudSpeaker__) */
