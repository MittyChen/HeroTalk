#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "HappyStartCell.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "ui/CocosGUI.h"
#include "CommonUtils.h"

enum CELL_TOUCH_MODE{
    NORMAL_MODE = -1,
    DELETE_ONE_MODE = 1,
    CHANGE_COLOR = 2,
    CHANGE_COLOR_RANDOM = 3,
};

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int code ,int difficultyp);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void update(float delta);
     virtual bool  onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    
    int getCountSameToThis(HappyStartCell* targetCell);
    list<HappyStartCell*> getCellsSameToThisSlantLeft(HappyStartCell* targetCell);//反斜向
    list<HappyStartCell*> getCellsSameToThisSlantRight(HappyStartCell* targetCell);//正斜向
    list<HappyStartCell*> findTheGroupToRemove(HappyStartCell* targetCell);//横竖向
    
    list<HappyStartCell*> getAllSameAround(HappyStartCell* targetCell);//横竖向
    
    
    
    
    void loadMap(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type) ;
    
    
    void backOneStep(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type) ;
    
    void exitGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type) ;
    
    void gotoLevelSelect(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
    void deleteOneCell(cocos2d::Ref* object);
    
    void deleteTargetCell(map<cocos2d::Vec2,HappyStartCell*>::iterator   targetCell);
    
    void checkoutResult();
    
    void changeTypeRandom(cocos2d::Ref* object);
    
    void changeType(cocos2d::Ref* object);
    
    void seletctCellolor(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
    void pauseGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
    void pauseGameBack(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
    
    void gotoLevelNext(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);
    
public:

    float munitSize;
     static int count;
    map<cocos2d::Vec2, HappyStartCell*> allcells;
    cocos2d::Vec2 unitOriginPosition;
    list<HappyStartCell*> cellsGet; //在递归方法中，使用临时容器注意被清空。
    list<HappyStartStruct> cellsCacheOne;
    enum CELL_TOUCH_MODE _mMode;
    
    cocos2d::Vec2 onShotButtonPos;
    
    bool actionPlaying;
    
    cocos2d::ui::Text* scoreLAbel;
    
    static bool isPaused;
    
    static bool isGameFinish;
    
};

#endif // __GAME_SCENE_H__
