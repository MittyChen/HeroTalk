//
//  LoudSpeaker.cpp
//  HeroTalk
//
//  Created by chenwei on 15/4/30.
//
//

#include "LoudSpeaker.h"
#include "DramaDirector.h"
#include "Character.h"
int LoudSpeaker::muser0state;
int LoudSpeaker::muser1state;
int LoudSpeaker::muser2state;

bool LoudSpeaker::init()
{
    muser0state = 0;
    muser1state = 0;
    muser2state = 0;
    return true;
}

void LoudSpeaker::speakToTarget(DramaEventType type)
{
    
   
    
    switch (type) {
        case EVENT_DRAMA_CHARACTER_ROTATE:
            __NotificationCenter::getInstance()->postNotification("setCharacterRotate",nullptr);
            break;
            
        case EVENT_DRAMA_SOUND_OFF:
        {
            DramaDirector::getInstance()->stageManager->setBGState(STAGE_MASK_ZORDER + 1);
            
            for(int i = 0; i < DramaDirector::characterListFinal.size();++i)
            {
                
                DramaDirector::characterListFinal.at(i)->goBack();
                
            }
            
            Character::talkContent->setString(DramaDirector::currentTalkcontent);
            
            break;
        }
            
        case EVENT_DRAMA_CHARACTER0_TALK:
        {
            
            for(int i = 0; i < DramaDirector::characterListFinal.size();++i)
            {

                if(DramaDirector::characterListFinal.at(i)->getID() == 100)
                {
                    DramaDirector::characterListFinal.at(i)->goFront();
                }else{
                    DramaDirector::characterListFinal.at(i)->goBack();
                }

            }
            
           Character::talkContent->setString(DramaDirector::currentTalkcontent);
           DramaDirector::getInstance()->stageManager->setBGState(STAGE_MASK_ZORDER -1);
            
            break;
        }
        case EVENT_DRAMA_CHARACTER1_TALK:
            for(int i = 0; i < DramaDirector::characterListFinal.size();++i)
            {
                
                if(DramaDirector::characterListFinal.at(i)->getID() == 101)
                {
                    DramaDirector::characterListFinal.at(i)->goFront();
                }else{
                    DramaDirector::characterListFinal.at(i)->goBack();
                }
                
            }
            
            Character::talkContent->setString(DramaDirector::currentTalkcontent);
            DramaDirector::getInstance()->stageManager->setBGState(STAGE_MASK_ZORDER -1);
            
            break;
        case EVENT_DRAMA_CHARACTER2_TALK:
            for(int i = 0; i < DramaDirector::characterListFinal.size();++i)
            {
                
                if(DramaDirector::characterListFinal.at(i)->getID() == 102)
                {
                    DramaDirector::characterListFinal.at(i)->goFront();
                }else{
                    DramaDirector::characterListFinal.at(i)->goBack();
                }
                
            }
            
            Character::talkContent->setString(DramaDirector::currentTalkcontent);
            DramaDirector::getInstance()->stageManager->setBGState(STAGE_MASK_ZORDER -1);
            
            break;
            
        default:
            break;
    }
}