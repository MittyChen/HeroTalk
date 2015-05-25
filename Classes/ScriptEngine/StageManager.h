//
//  StageManager.h
//  HeroTalk
//
//  Created by chenwei on 15/4/29.
//
//

#ifndef __HeroTalk___StageManager__
#define __HeroTalk___StageManager__
#include "cocos2d.h"
USING_NS_CC;

#define STAGE_MASK_ZORDER 50
#define STAGE_MASK_TAG    9999
#define STAGE_TALK_CONTENT_TAG    9998
class StageManager: public Node
{
public:
    LayerColor* stagemask;
    CREATE_FUNC(StageManager);
    const char* _backTexture;
    void setBackTexture(const char* _backTexture);
    virtual bool init();
    void setDelegate(Node& delegate);
    void updateTalkContent(Node* delegate);
    void setBGState(int state);
    
    void destroyStage();
    
    bool stageIsReady;
private:
    static Sprite* talkframe;
    Sprite* backTexture;
    static Label* talkContent;
    
};

#endif /* defined(__HeroTalk___StageManager__) */
