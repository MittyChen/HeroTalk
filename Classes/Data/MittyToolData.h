//
//  MittyToolData.h
//  HeroTalk
//
//  Created by 陈伟 on 15/8/13.
//
//

#ifndef __HeroTalk__MittyToolData__
#define __HeroTalk__MittyToolData__

#define KEY_CHANGE_TYPE "MITTY_HT_CHANGE_TYPE_SUM"
#define KEY_RANDOM_TYPE "MITTY_HT_RANDOM_TYPE_SUM"
#define KEY_ONE_SHOT "MITTY_HT_ONE_SHOT_SUM"
#define KEY_AFTER_FIRST_INSTALLED "MITTY_HT_AFTER_FIRST_INSTALLED"
#define KEY_COINS_NUM "MITTY_HT_KEY_COINS_NUM"

#include <stdio.h>
class MittyToolData
{

public:
    MittyToolData();
    ~MittyToolData();
    
    static MittyToolData * mData;
    static MittyToolData* getInstance();
    void initWithKeys();
    
    int getChangeTypeTool();
    int getRandomTool();
    int getOneshotTool();
    
    void useChangeTypeTool();
    void useRandomTool();
    void useOneshotTool();
    
    int getcoins();
    
    void addChangeTypeTool(int count);
    void addRandomTool(int count);
    void addOneshotTool(int count);
    
    void usecoins(int amount);
    void addcoins(int count);
    
private:
    int changeTypeTool;
    int randomTypeTool;
    int oneshotTool;
    
    
    int coins;

};
#endif /* defined(__HeroTalk__MittyToolData__) */
