//
//  DramaDirector.h
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//

#ifndef __HeroTalk__DramaDirector__
#define __HeroTalk__DramaDirector__

#include "StageManager.h"
#include "LoudSpeaker.h"
#include "cocos2d.h"
#include "Drama.h"
#include "ui/CocosGUI.h"
#include "Character.h"
USING_NS_CC;

class DramaDirector: public Node
{
public:
    CREATE_FUNC(DramaDirector);
    virtual bool init();
    static DramaDirector* getInstance();
    void setDelegate(Node& delegate,const char* bgmusic,const char* stagePlist,const char* plotPilist,const std::function<void(Node*)> func);
    
    void characterMakeUp(Node& delegate);
    void tapToTakeForward(float deltatime);

    static DramaDirector* _instance;
    
    LoudSpeaker* speakmanager;
    StageManager* stageManager;
    
    Drama* mDrama;
    
    static int currentTalkingIndex;
    static const char* currentTalkcontent;
    
    
    __Array* talkContentArray;
    
    
    static std::vector<Character*> characterListFinal;
    static Vector<Ref*> contentListFinal;
    static std::function<void(Node*)> dramaFinishfunc;
    
};
#endif /* defined(__HeroTalk__DramaDirector__) */
