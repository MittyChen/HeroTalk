//
//  MittyToolData.cpp
//  HeroTalk
//
//  Created by 陈伟 on 15/8/13.
//
//

#include "MittyToolData.h"
#include "cocos2d.h"

USING_NS_CC;

MittyToolData * MittyToolData::mData = NULL;

MittyToolData* MittyToolData::getInstance(){
  
    if (mData)
    {
        return mData;
    }
    else
    {
         mData = new MittyToolData();
    }
    return mData;
}


void MittyToolData::initWithKeys(){
    bool hasinstalled = UserDefault::getInstance()->getBoolForKey(KEY_AFTER_FIRST_INSTALLED);
    
    //默认false 首次安装
    if (!hasinstalled) {
        
        UserDefault::getInstance()->setIntegerForKey(KEY_CHANGE_TYPE,2);
        UserDefault::getInstance()->setIntegerForKey(KEY_RANDOM_TYPE,2);
        UserDefault::getInstance()->setIntegerForKey(KEY_ONE_SHOT,2);
        changeTypeTool = 2;
        randomTypeTool = 2;
        oneshotTool = 2;
        UserDefault::getInstance()->setBoolForKey(KEY_AFTER_FIRST_INSTALLED,true);
        coins = 200;
        UserDefault::getInstance()->setIntegerForKey(KEY_COINS_NUM,200);
        
    }else{
        changeTypeTool =  UserDefault::getInstance()->getIntegerForKey(KEY_CHANGE_TYPE);
        randomTypeTool =  UserDefault::getInstance()->getIntegerForKey(KEY_RANDOM_TYPE);
        oneshotTool =  UserDefault::getInstance()->getIntegerForKey(KEY_ONE_SHOT);
        coins =  UserDefault::getInstance()->getIntegerForKey(KEY_COINS_NUM);
    }
}

int MittyToolData::getChangeTypeTool(){
    return changeTypeTool;
}


int MittyToolData::getRandomTool(){
    return randomTypeTool;
}


int MittyToolData::getOneshotTool(){
    return oneshotTool;
}

void MittyToolData::useChangeTypeTool(){
    if (changeTypeTool<=0) {
        return;
    }
    UserDefault::getInstance()->setIntegerForKey(KEY_CHANGE_TYPE, changeTypeTool--);
}

void MittyToolData::useRandomTool(){
    if (randomTypeTool<=0) {
        return;
    }
     UserDefault::getInstance()->setIntegerForKey(KEY_RANDOM_TYPE, randomTypeTool--);
}

void MittyToolData::useOneshotTool(){
    if (oneshotTool<=0) {
        return;
    }
     UserDefault::getInstance()->setIntegerForKey(KEY_ONE_SHOT, oneshotTool--);
}

void MittyToolData::addChangeTypeTool(int count){
    UserDefault::getInstance()->setIntegerForKey(KEY_CHANGE_TYPE, changeTypeTool+=count);
}

void MittyToolData::addRandomTool(int count){
    UserDefault::getInstance()->setIntegerForKey(KEY_RANDOM_TYPE, randomTypeTool+=count);
}


void MittyToolData::addOneshotTool(int count){
    UserDefault::getInstance()->setIntegerForKey(KEY_ONE_SHOT, oneshotTool+=count);
}

int MittyToolData::getcoins()
{
    return  coins;

}
void MittyToolData::usecoins(int amount)
{
    if (coins<=0) {
        return;
    }
    UserDefault::getInstance()->setIntegerForKey(KEY_COINS_NUM, coins-=amount);
}

void MittyToolData::addcoins(int count)
{
    UserDefault::getInstance()->setIntegerForKey(KEY_COINS_NUM, coins+=count);
}


MittyToolData::MittyToolData(){
    initWithKeys();
}
MittyToolData::~MittyToolData(){

}