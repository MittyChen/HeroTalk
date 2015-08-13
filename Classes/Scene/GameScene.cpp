 #include "GameScene.h"
#include "cocostudio/CocoStudio.h"

#include "HappyStartCell.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "FailedPopup.h"
#include "FinishPopup.h"
#include "CCSpriteWithHue.h"

#include "PopupWithMask.h"
#include "LevelSelectScene.h"
#include "PreGameScene.h"
#include "SPCScene.h"
#include "CommonUtils.h"
#include "MittyToolData.h"
#include "ShopScene.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Share.h"
#endif

USING_NS_CC;

bool isGameWin = false;
bool isGameFailed = false;
bool GameScene::isPaused  = false;
static HappyStartCell* seletingCell = NULL;
bool GameScene::isGameFinish = false;
int GameScene::count = 8;
using namespace cocostudio::timeline;

bool isPauseFlag = false;
int framecount = 0;

static int mdifficult = 0;
static int levelco = 0;
static LevelNode* lv = NULL;
static int reaminingStep = 10;

Scene* GameScene::createScene(LevelNode* mLevel)
{
    auto scene = Scene::create();
    levelco = mLevel->getLevelCode();
    lv = LevelNode::create();
    
    lv->setLevelCode(mLevel->getLevelCode());
    
    reaminingStep = lv->stepNeed;
    
    lv->retain();
    
    count = 10;
    
    mdifficult = 2;
    
    SPCScene::resetresult();
    
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

GameScene::GameScene(){
    
}
GameScene::~GameScene(){
   
    if(lv!=NULL){
        lv->release();
        lv=NULL;
    }
    
    if ( allcells.size()>0 && allcells.begin()->second != NULL) {
        allcells.clear();
    }
    
    if (cellsGet.size()>0) {

        cellsGet.clear();
    }
    if (cellsCacheOne.size()>0) {

        cellsCacheOne.clear();
    }
    if (scoreLAbel!=NULL) {
        scoreLAbel->release();
        scoreLAbel=NULL;
    }
    if (spcdes!=NULL) {
        spcdes->release();
        spcdes = NULL;
    }
    
    

}
// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    scheduleUpdate();
    isPaused = false;
    isGameFinish = false;
    _mMode = CELL_TOUCH_MODE::NORMAL_MODE;
    
    reaminingStep = 3;
    
    redCount = 0;
    greenCount = 0;
    blueCount = 0;
    
 
    
    
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* mlcbg = LayerColor::create(GAMEBG_COLOR_NO_TRANSPARENT,visibleSize.width,visibleSize.height);
    
    mlcbg->setGlobalZOrder(-100);
    this->addChild(mlcbg);
    
    
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    
    rootNode->setName("MainSceneRoot");
    
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
    
    
    
    cocos2d::ui::Button* btnPause =  (cocos2d::ui::Button*)rootNode->getChildByTag(78);
    btnPause->addTouchEventListener(CC_CALLBACK_2(GameScene::pauseGame, this) );

    
    
    scoreLAbel = (cocos2d::ui::Text*)rootNode->getChildByTag(7);
    
    scoreLAbel->retain();
    const char * scoretext = String::createWithFormat("目标分数 %d" , lv->score)->getCString();
    scoreLAbel->setString(scoretext);
    
    auto steplabel = (cocos2d::ui::Text*)rootNode->getChildByTag(36);
    const char * steptext = String::createWithFormat("还剩 %d 步" ,reaminingStep)->getCString();
    steplabel->setString(steptext);
    

    
    
    cocos2d::ui::Text* labelLevel = (cocos2d::ui::Text*)rootNode->getChildByTag(74);
    
    const char * Leveltext = String::createWithFormat("第 %d 关" , lv->getLevelCode())->getCString();
    labelLevel->setString(Leveltext);
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(11);
    mTool_OneShot->addClickEventListener(CC_CALLBACK_1(GameScene::deleteOneCell,this));
    onShotButtonPos = mTool_OneShot->getPosition();
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(19);
    mTool_RandomType->addClickEventListener(CC_CALLBACK_1(GameScene::changeTypeRandom,this));
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(20);
    mTool_ChangeType->addClickEventListener(CC_CALLBACK_1(GameScene::changeType,this));
    
    
    oneshottoolsum = MittyToolData::getInstance()->getOneshotTool();
    changeTypesum =  MittyToolData::getInstance()->getChangeTypeTool();
    randomTypesum =  MittyToolData::getInstance()->getRandomTool();
    
   
    cocos2d::ui::Text* labelchanetype = (cocos2d::ui::Text*)(mTool_ChangeType->getChildByTag(31)->getChildByTag(36));
    const char * cttext = String::createWithFormat("%d" , changeTypesum)->getCString();
    labelchanetype->setString(cttext);
    
    cocos2d::ui::Text* labelrandomtype = (cocos2d::ui::Text*)(mTool_RandomType->getChildByTag(32)->getChildByTag(35));
    
    const char * rttext = String::createWithFormat("%d" , randomTypesum)->getCString();
    labelrandomtype->setString(rttext);
    
    
    cocos2d::ui::Text* labeloneshot = (cocos2d::ui::Text*)(mTool_OneShot->getChildByTag(33)->getChildByTag(34));
    const char * onstext = String::createWithFormat("%d" , oneshottoolsum)->getCString();
    labeloneshot->setString(onstext);
    
    spcdes = (cocos2d::ui::Text*)rootNode->getChildByTag(62);
    
    spcdes->retain();
    
    actionPlaying = false;
    
    Vec2 uiPosition = btnPause->getPosition() + Vec2(40, 0);
    
    int mcount = 6;
    
    if(count >= 6)
    {
        mcount = count;
    }
    
    
    float targetHeight = rootNode->getChildByTag(123)->getPositionY() - rootNode->getChildByTag(123)->getContentSize().height - onShotButtonPos.y - mTool_OneShot->getContentSize().height*mTool_ChangeType->getScale();
    
  
    munitSize =(targetHeight < visibleSize.width? (targetHeight - 60 ):(visibleSize.width - 60) )/(mcount + 1);
    
    float middleYY = (rootNode->getChildByTag(123)->getPositionY() + onShotButtonPos.y) / 2;
    
    float startY = middleYY - munitSize*count/2;
    
    float planB =  (visibleSize.width - (munitSize * count)) / 2;
    
    unitOriginPosition = Vec2( planB , startY);
    
    LayerColor* mlc = LayerColor::create(Color4B(100, 255, 100, 50), (munitSize + 1)* count + 10, (munitSize + 1) * count + 10);
    
    mlc->setPosition(unitOriginPosition - Vec2(5,5));
    mlc->setAnchorPoint(Vec2(0, 0));
    mlc->setGlobalZOrder(-2);
    this->addChild(mlc);
    
    addChild(rootNode);
    
    
    lv->setLevelCode(levelco);
    
    
    const char* currentActionType =  lv->type.c_str();
    
    if ( strncasecmp(currentActionType,"FIND_COLOR", 10 )  == 0) {
        scoreLAbel->setVisible(false);
    }else if ( strncasecmp(currentActionType,"GET_SCORE", 9 )  == 0) {
        
        rootNode->getChildByTag(123)->setVisible(false);;
        rootNode->getChildByTag(126)->setVisible(false);;
        rootNode->getChildByTag(127)->setVisible(false);;
        rootNode->getChildByTag(128)->setVisible(false);;
        rootNode->getChildByTag(129)->setVisible(false);;
        rootNode->getChildByTag(130)->setVisible(false);;
        
        
        
    }else if ( strncasecmp(currentActionType,"CHESS_MODE", 10 )  == 0) {
        
        rootNode->getChildByTag(123)->setVisible(false);
        rootNode->getChildByTag(126)->setVisible(false);
        rootNode->getChildByTag(127)->setVisible(false);
        rootNode->getChildByTag(128)->setVisible(false);
        rootNode->getChildByTag(129)->setVisible(false);
        rootNode->getChildByTag(130)->setVisible(false);
        scoreLAbel->setVisible(false);
        rootNode->getChildByTag(36)->setVisible(false);;
        
        _mMode = CELL_TOUCH_MODE::CHESS_MODE;
        
        auto lamdaSPC = [=](Ref* pSender){
            
            startSPC(this, ui::Widget::TouchEventType::ENDED);
            
        };
        rootNode->getChildByTag(50)->setVisible(true);
        
        this->runAction(Sequence::create(DelayTime::create(1.5),CallFuncN::create(lamdaSPC), NULL));
    }
    
    
    
    loadMap(NULL,cocos2d::ui::Widget::TouchEventType::ENDED);
    
    
    
    auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    
    
//    LayerColor* mLayerMask = LayerColor::create(cocos2d::Color4B(0, 0, 0, 150));
//    mLayerMask->setContentSize(cocos2d::Size(visibleSize.width,visibleSize.height));
//    this->addChild(mLayerMask);
//    mLayerMask->setVisible(false);
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan =  [this](Touch * ,Event *)
//    {
//        if (!isPaused) {
//            return true;
//        }else{
//            return false;
//        }
//        
//    };
//    
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,mLayerMask);
//    
    DrawNode * mylines = DrawNode::create();
    this->addChild(mylines);
    //    mylines->setPosition(Director::getInstance()->getVisibleSize()/2 );
    
    mylines->drawLine(Vec2(0, 0), Vec2(900,900), Color4F(200,100,100, 255));
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    auto removeLamda = [=](Ref* pSender)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("playbg.mp3",true);
    };
    
    this->runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(removeLamda), NULL) );
   
    
    cellsCacheOne.clear();
    
    
    
//    for(int i=0; i<10; i++)
//    {
//        auto alien = SpriteWithHue::create("alien.png");
//        alien->setPosition(Vec2((visibleSize.width + origin.x) * CCRANDOM_0_1(), (visibleSize.height + origin.y) * CCRANDOM_0_1()));
//        alien->setHue(2 * M_PI * CCRANDOM_0_1());
//        this->addChild(alien,1);
//    }
    return true;
}
//更新道具数量显示
void GameScene::updateToolsBadge(){
    
    auto rootNode = this->getChildByName("MainSceneRoot");
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(11);
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(19);
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(20);
    
    oneshottoolsum = MittyToolData::getInstance()->getOneshotTool();
    changeTypesum =  MittyToolData::getInstance()->getChangeTypeTool();
    randomTypesum =  MittyToolData::getInstance()->getRandomTool();
    
    cocos2d::ui::Text* labelchanetype = (cocos2d::ui::Text*)(mTool_ChangeType->getChildByTag(31)->getChildByTag(36));
    const char * cttext = String::createWithFormat("%d" , changeTypesum)->getCString();
    labelchanetype->setString(cttext);
    cocos2d::ui::Text* labelrandomtype = (cocos2d::ui::Text*)(mTool_RandomType->getChildByTag(32)->getChildByTag(35));
    const char * rttext = String::createWithFormat("%d" , randomTypesum)->getCString();
    labelrandomtype->setString(rttext);
    cocos2d::ui::Text* labeloneshot = (cocos2d::ui::Text*)(mTool_OneShot->getChildByTag(33)->getChildByTag(34));
    const char * onstext = String::createWithFormat("%d" , oneshottoolsum)->getCString();
    labeloneshot->setString(onstext);
    
}

void GameScene::showShortOfTool()
{
    if (isPaused) {
        return;
    }
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* lco =  LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height);
    this->addChild(lco);
    lco->setTag(-190);
    
    auto pauselayer = CSLoader::createNode("ToolShortLayer.csb");
    
    this->addChild(pauselayer);
    pauselayer->setName("shor_tool_Layer");
    
    pauselayer->setContentSize(visibleSize);
    ui::Helper::doLayout(pauselayer);
    
    Sprite* pauseRoot = (Sprite*)pauselayer->getChildByTag(53);
    pauseRoot->setPosition(Vec2( visibleSize.width/2 , visibleSize.height + pauseRoot->getContentSize().height ));
    
    pauseRoot->setScale(visibleSize.width * 6 / 7 / pauseRoot->getContentSize().width);
    pauseRoot->runAction(MoveTo::create(0.1, Vec2(visibleSize.width/2, visibleSize.height/2 - 20)));
    
    
    cocos2d::ui::Button* btnExit =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(54);
    btnExit->addTouchEventListener(CC_CALLBACK_2(GameScene::goShop, this) );
    
    cocos2d::ui::Button* backtogame =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(62);
    backtogame->addTouchEventListener(CC_CALLBACK_2(GameScene::hideShortOfTool, this) );
    
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    mTool_OneShot->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    mTool_RandomType->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    mTool_ChangeType->setTouchEnabled(false);
    
    
    isPaused = true;
}

void GameScene::hideShortOfTool(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    auto removePauseLamda = [=](Ref* pSender)mutable{
        
        getChildByName("shor_tool_Layer")->removeFromParent();
        
    };
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    this->removeChildByTag(-190);
    Sprite* pauseRoot = (Sprite*)getChildByName("shor_tool_Layer")->getChildByTag(53);
    
    pauseRoot->runAction(Sequence::create(  MoveTo::create(0.25, Vec2(visibleSize.width/2 , visibleSize.height + pauseRoot->getContentSize().height) ),CallFuncN::create(removePauseLamda), NULL));
    
    isPauseFlag = true;
    
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    mTool_OneShot->setTouchEnabled(true);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    mTool_RandomType->setTouchEnabled(true);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    mTool_ChangeType->setTouchEnabled(true);
}

void GameScene::onEnter(){
    Layer::onEnter();
    int winside = SPCScene::winSide();
    if (winside==0)
    {
        
        spcdes->setVisible(true);
        const char * spctext = String::createWithFormat("左边的先手")->getCString();
        spcdes->setString(spctext);
    }else if (winside==1)
    {
        
        spcdes->setVisible(true);
        const char * spctext = String::createWithFormat("右边的先手")->getCString();
        spcdes->setString(spctext);
    }else{
        spcdes->setVisible(false);
    }
}

void GameScene::loadMap(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    redCount =0;
    greenCount = 0;
    blueCount = 0;
    reaminingStep = lv->stepNeed;
    

    
    auto rootNode = this->getChildByName("MainSceneRoot");
    cocos2d::ui::Text* redCountt = (cocos2d::ui::Text*)rootNode->getChildByTag(123);
    const char * scoretext = String::createWithFormat(" x %d " ,  lv->redcount)->getCString();
    redCountt->setString(scoretext);
    
    cocos2d::ui::Text* greenCountt = (cocos2d::ui::Text*)rootNode->getChildByTag(126);
    const char * greenT = String::createWithFormat(" x %d " ,lv->greencount)->getCString();
    greenCountt->setString(greenT);
    
    cocos2d::ui::Text* blueCountt = (cocos2d::ui::Text*)rootNode->getChildByTag(127);
    const char * blueT = String::createWithFormat(" x %d " ,lv->bluecount)->getCString();
    blueCountt->setString(blueT);
    
    
    auto steplabel = (cocos2d::ui::Text*)rootNode->getChildByTag(36);
    const char * steptext = String::createWithFormat("还剩 %d 步" ,reaminingStep)->getCString();
    steplabel->setString(steptext);
    
    isGameWin = false;
    isGameFailed = false;
    
    if(isGameFinish)
    {
        cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
        mTool_OneShot->setTouchEnabled(true);
        
        cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
        mTool_RandomType->setTouchEnabled(true);
        
        cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
        mTool_ChangeType->setTouchEnabled(true);
        
        
        if (((Node*)object)->getParent()->getName() == "MainSceneRoot") {
            return;
        }
        this->removeChildByName("GameFinish");
        this->removeChildByName("GameFailed");
        
        isPauseFlag = true;
        
        isGameFinish = false;
        
        const char * scoretext = String::createWithFormat("目标分数 %d" , lv->score)->getCString();
        scoreLAbel->setString(scoretext);
        
        map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
        
        for (; mpIterator != allcells.end(); ++mpIterator)
        {
            if (mpIterator!=allcells.end()) {
                this->removeChild((Node*)mpIterator->second);
            }
        }
        
        if (allcells.begin()->second != NULL) {
            allcells.clear();
        }
        for(int i = 0; i < count; ++i )
        {
            for(int j =0 ; j < count ; ++j)
            {
                HappyStartCell* mm =  HappyStartCell::create();
                mm->setParameters(Color3B(25.5f * i,25.5f * j,10.f*(i+j)),unitOriginPosition,cocos2d::Size(munitSize,munitSize),Vec2(i,j),count);
                allcells.insert(pair<Vec2, HappyStartCell*> (Vec2(i,j), mm));
                addChild((Node*)mm);
                if (this->getChildByTag(-190)) {
                    mm->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
                }
                if (this->getChildByName("GameFinish")) {
                    
                    auto frootNode = this->getChildByName("GameFinish")->getChildByName("FinishPopRoot");
                    
                    mm->setGlobalZOrder(frootNode->getGlobalZOrder()-1);
                }
            }
        }
        
        srand(0);
        
        mpIterator = allcells.begin();
        
        for (; mpIterator != allcells.end(); ++mpIterator)
        {
            
            switch (_mMode) {
                case CELL_TOUCH_MODE::CHESS_MODE:
                {
                    
                    mpIterator->second->setType(CELL_TYPE::TYPE_NORMAL);
                    break;
                }
                    
                case CELL_TOUCH_MODE::NORMAL_MODE:
                case CHANGE_COLOR_RANDOM:
                case CHANGE_COLOR:
                {
                    int typeFind = 1;
        
                    switch (mdifficult) {
                        case 0:
                            typeFind = random(1,3);
                            break;
                        case 1:
                            typeFind = random(1,5);
                            break;
                        case 2:
                            typeFind = random(1,7);
                            break;
                        default:
                            break;
                    }
                    
                    if (typeFind == 7) {
                        
                       typeFind = random(1,100) > 90 ? 7 : random(1, 6);
                    }
                    
                    
                    mpIterator->second->setType((CELL_TYPE)typeFind);
                 break;
                }
                    
                   
                default:
                    break;
            };

        }
         return;
    }
    
//    if (isPaused){
//        return;
//    }
    
    
    const char * scoretext1 = String::createWithFormat("目标分数 %d" , lv->score)->getCString();
    scoreLAbel->setString(scoretext1);
    
    map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
    
    for (; mpIterator != allcells.end(); ++mpIterator)
    {
        if (mpIterator!=allcells.end()) {
            this->removeChild((Node*)mpIterator->second);
        }
    }
    
    if (allcells.begin()->second != NULL) {
          allcells.clear();
    }
    for(int i = 0; i < count; ++i )
    {
        for(int j =0 ; j < count ; ++j)
        {
            HappyStartCell* mm =  HappyStartCell::create();
            mm->setParameters(Color3B(25.5f * i,25.5f * j,10.f*(i+j)),unitOriginPosition,cocos2d::Size(munitSize,munitSize),Vec2(i,j),count);
            allcells.insert(pair<Vec2, HappyStartCell*> (Vec2(i,j), mm));
            addChild((Node*)mm);
            if (this->getChildByTag(-190)) {
                mm->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
            }
            if (this->getChildByName("GameFinish")) {
                
                auto frootNode = this->getChildByName("GameFinish")->getChildByName("FinishPopRoot");
                
                mm->setGlobalZOrder(frootNode->getGlobalZOrder()-1);
            }
            
        }
    }
    
    
    
    //储存上一步地图情况
    cellsCacheOne.clear();
//    mpIterator = allcells.begin();
//    for (; mpIterator != allcells.end(); ++mpIterator)
//    {
//        if(mpIterator != allcells.end()){
//            // 深度拷贝
//            float xpos = mpIterator->second->getposIndex().x;
//            float ypos = mpIterator->second->getposIndex().y;
//            struct HappyStartStruct datatemp = {Vec2(xpos,ypos),mpIterator->second->getType()};
//            cellsCacheOne.push_back( datatemp );
//        }
//    }
    
    
    //    int blueCount = count/3;
    //    int redcount = count/3;
    //    int greencount = count/3;
    //
    //
    
    srand(0);
    
    

    mpIterator = allcells.begin();
    
    for (; mpIterator != allcells.end(); ++mpIterator)
    {
        switch (_mMode) {
            case CELL_TOUCH_MODE::CHESS_MODE:
            {
                mpIterator->second->setType(CELL_TYPE::TYPE_NORMAL);
                break;
            }
                
            case CELL_TOUCH_MODE::NORMAL_MODE:
            case CHANGE_COLOR_RANDOM:
            case CHANGE_COLOR:
            {
                int typeFind = 1;
                
                switch (mdifficult) {
                    case 0:
                        typeFind = random(1,3);
                        break;
                    case 1:
                        typeFind = random(1,5);
                        break;
                    case 2:
                        typeFind = random(1,7);
                        break;
                    default:
                        break;
                }
                
                if (typeFind == 7) {
                    
                    typeFind = random(1,100) > 90 ? 7 : random(1, 6);
                }
                
                
                mpIterator->second->setType((CELL_TYPE)typeFind);
                break;
            }
                
                
            default:
                break;
        };
    }
}



void GameScene::seletctCellolor(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    if(type == cocos2d::ui::Widget::TouchEventType::ENDED){
    
        Node* selectType = (Node*)object;
        
        int tag = selectType->getTag();
        
        switch (tag) {
            case 32:
                seletingCell->setType(CELL_TYPE::TYPE_RED);
                
                break;
            case 33:
                seletingCell->setType(CELL_TYPE::TYPE_BLUE);
                
                break;
            case 34:
                seletingCell->setType(CELL_TYPE::TYPE_GREEN);
                
                break;
                
            case 49:
                seletingCell->setType(CELL_TYPE::TYPE_GRAY);
                
                break;
            case 50:
                seletingCell->setType(CELL_TYPE::TYPE_PINK);
                
                break;
            case 51:
                seletingCell->setType(CELL_TYPE::TYPE_PURPLE);
                
                break;
                
                
            default:
                break;
        }
        isPauseFlag = true;
        this->removeChildByTag(1999);
        seletingCell= NULL;
    }
}



void GameScene::backOneStep(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
//    if (isPaused){
//        return;
//    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            if(cellsCacheOne.size() == 0 || (cellsCacheOne.size() == 4 && allcells.size()==count*count-1))
            {
                CommonUtils::showToastOnIOS(*this,"没有可返回的步数哦 QAQ~");
                
                return;
            }
            
            map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
            
            for (; mpIterator != allcells.end(); ++mpIterator)
            {
                if (mpIterator!=allcells.end()) {
                    this->removeChild((Node*)mpIterator->second);
                }
            }
            
            allcells.clear();
            
            for(HappyStartStruct tempCell : cellsCacheOne)
            {
                
                HappyStartCell* mCell = HappyStartCell::create();
                mCell->setParameters(Color3B(0,0,0),unitOriginPosition,cocos2d::Size(munitSize,munitSize),tempCell._posIndex,count);
                mCell->setType((CELL_TYPE) tempCell._mType);
                
                mCell->setdownShouldGo(0.0f);
                mCell->setleftShouldGo(0.0f);
                this->addChild((Node*) mCell);
                allcells.insert((pair<Vec2, HappyStartCell*> (tempCell._posIndex,mCell)));
                
                if (this->getChildByTag(-190)) {
                    mCell->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
                }
                if (this->getChildByName("GameFinish")) {
                    
                    auto frootNode = this->getChildByName("GameFinish")->getChildByName("FinishPopRoot");
                    
                    mCell->setGlobalZOrder(frootNode->getGlobalZOrder()-1);
                }
                
            }
            cellsCacheOne.clear();
            
            break;
        }
        default:
            break;
    }
    
}

void GameScene::startSPC(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    Scene* scene = SPCScene::createScene();
    Director::getInstance()->pushScene(scene);
}



void GameScene::pauseGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    if (isPaused) {
        return;
    }
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* lco =  LayerColor::create(Color4B(0,0,0,100), visibleSize.width, visibleSize.height);
    this->addChild(lco);
    lco->setTag(-190);
    
    auto pauselayer = CSLoader::createNode("PauseGameLayer.csb");
    
    this->addChild(pauselayer);
    pauselayer->setName("Pause_Game_Layer");
    
    pauselayer->setContentSize(visibleSize);
    ui::Helper::doLayout(pauselayer);
    
    Sprite* pauseRoot = (Sprite*)pauselayer->getChildByTag(49);
    pauseRoot->setPosition(Vec2( visibleSize.width/2 , visibleSize.height + pauseRoot->getContentSize().height ));
    
    pauseRoot->setScale(visibleSize.width * 6 / 7 / pauseRoot->getContentSize().width);
//    pauseRoot->runAction( Sequence::create(MoveTo::create(0.1, Vec2(visibleSize.width/2, visibleSize.height/2 - 20)),MoveTo::create(0.2, Vec2(visibleSize.width/2, visibleSize.height/2)), NULL) );
    pauseRoot->runAction(MoveTo::create(0.1, Vec2(visibleSize.width/2, visibleSize.height/2)) );
    

    
    cocos2d::ui::Button* btnSPC =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(60);
    btnSPC->addTouchEventListener(CC_CALLBACK_2(GameScene::startSPC, this) );
    
    cocos2d::ui::Button* btnReset =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(52);
    btnReset->addTouchEventListener(CC_CALLBACK_2(GameScene::loadMap, this) );
    
    cocos2d::ui::Button* btnBack =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(56);
    btnBack->addTouchEventListener(CC_CALLBACK_2(GameScene::backOneStep, this) );
    
    
    cocos2d::ui::Button* btnExit =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(50);
    btnExit->addTouchEventListener(CC_CALLBACK_2(GameScene::exitGame, this) );
    
    
    
    cocos2d::ui::Button* levels =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(54);
    levels->addTouchEventListener(CC_CALLBACK_2(GameScene::gotoLevelSelect, this) );
    
    cocos2d::ui::Button* backtogame =  (cocos2d::ui::Button*)pauseRoot->getChildByTag(58);
    backtogame->addTouchEventListener(CC_CALLBACK_2(GameScene::pauseGameBack, this) );
    
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    mTool_OneShot->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    mTool_RandomType->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    mTool_ChangeType->setTouchEnabled(false);
    

    isPaused = true;
}

void GameScene::pauseGameBack(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    auto removePauseLamda = [=](Ref* pSender)mutable{
    
        getChildByName("Pause_Game_Layer")->removeFromParent();
        
    };
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    this->removeChildByTag(-190);
    Sprite* pauseRoot = (Sprite*)getChildByName("Pause_Game_Layer")->getChildByTag(49);
    
    pauseRoot->runAction(Sequence::create(  MoveTo::create(0.25, Vec2(visibleSize.width/2 , visibleSize.height + pauseRoot->getContentSize().height) ),CallFuncN::create(removePauseLamda), NULL));
    
    isPauseFlag = true;
    
   
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    mTool_OneShot->setTouchEnabled(true);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    mTool_RandomType->setTouchEnabled(true);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    mTool_ChangeType->setTouchEnabled(true);
    
}


void GameScene::exitGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    isGameWin = false;
    isGameFailed = false;
    
        auto scene = MainMenuScene::createScene();
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void GameScene::goShop(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    isGameWin = false;
    isGameFailed = false;
    
    auto scene = ShopScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void GameScene::shareScore(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    Share::getInstance()->share();
#endif
}

void GameScene::gotoLevelSelect(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
        auto scene =  LevelSelectScene::createScene(0);
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}


void GameScene::gotoLevelNext(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
     isGameWin = false;
    isGameFailed = false;
        LevelNode* lbn = LevelNode::create();
        lbn->setLevelCode(lv->getLevelCode()+1);
        
        auto scene =  PreGameScene::createScene(lbn);
        // run
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
   
}



void GameScene::deleteOneCell(cocos2d::Ref* object)
{
    
    if (isPaused){
        return;
    }
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    
    mTool_ChangeType->stopAllActions();
    mTool_RandomType->stopAllActions();
    
    mTool_ChangeType->setScale(0.2971);
    mTool_RandomType->setScale(0.2971);
    
    mTool_ChangeType->setSelected(false);
    mTool_RandomType->setSelected(false);
    
    if (_mMode == CELL_TOUCH_MODE::DELETE_ONE_MODE) {
        _mMode = CELL_TOUCH_MODE::NORMAL_MODE;
        
        cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
        
        mTool_OneShot->stopAllActions();
        mTool_OneShot->setScale(0.2971);
        
    }else{
        _mMode = CELL_TOUCH_MODE::DELETE_ONE_MODE;
        ((cocos2d::ui::CheckBox*)object)->runAction(RepeatForever::create(Sequence::create( ScaleTo::create(0.5, 1.1 * ((Node*)object)->getScale()),ScaleTo::create(0.5, 1.0 * ((Node*)object)->getScale() ),NULL)));
    }
    
    
}

void GameScene::changeTypeRandom(cocos2d::Ref* object)
{
        if (isPaused){
            return;
        }
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    
    mTool_OneShot->stopAllActions();
    mTool_ChangeType->stopAllActions();
    
    mTool_OneShot->setSelected(false);
    mTool_ChangeType->setSelected(false);
    
    mTool_ChangeType->setScale(0.2971);
    mTool_OneShot->setScale(0.2971);
    
    
    if (_mMode == CELL_TOUCH_MODE::CHANGE_COLOR_RANDOM) {
        _mMode = CELL_TOUCH_MODE::NORMAL_MODE;
        cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
        mTool_RandomType->stopAllActions();
        mTool_RandomType->setScale(0.2971);
    }else{
        
        _mMode = CELL_TOUCH_MODE::CHANGE_COLOR_RANDOM;
        
        ((cocos2d::ui::CheckBox*)object)->runAction(RepeatForever::create(Sequence::create( ScaleTo::create(0.5, 1.1 * ((Node*)object)->getScale()),ScaleTo::create(0.5, 1.0 * ((Node*)object)->getScale() ),NULL)));
        
    }
}

void GameScene::changeType(cocos2d::Ref* object)
{
    if (isPaused){
        return;
    }
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    
    mTool_OneShot->stopAllActions();
    mTool_RandomType->stopAllActions();
    
    mTool_OneShot->setSelected(false);
    mTool_RandomType->setSelected(false);
    
    
    mTool_RandomType->setScale(0.2971);
    mTool_OneShot->setScale(0.2971);
    if (_mMode == CELL_TOUCH_MODE::CHANGE_COLOR) {
        
        _mMode = CELL_TOUCH_MODE::NORMAL_MODE;
        cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
        
        mTool_ChangeType->stopAllActions();
        mTool_ChangeType->setScale(0.2971);
        
    }else{
        
        _mMode = CELL_TOUCH_MODE::CHANGE_COLOR;
        
        ((cocos2d::ui::CheckBox*)object)->runAction(RepeatForever::create(Sequence::create( ScaleTo::create(0.5, 1.1 * ((Node*)object)->getScale()),ScaleTo::create(0.5, 1.0 * ((Node*)object)->getScale() ),NULL)));
    }
}


void GameScene::update(float delta)
{
    
    //延迟一帧重置暂停标识
    if(framecount == 1){
        isPauseFlag = false;
        framecount = 0;
        isPaused = false;
    }
    
    if (isPauseFlag) {
        framecount++ ;
    }
}
int playerCount = 0;
bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (isPaused || isGameFinish){
        return false;
    }
    
    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    float touchX = touch->getStartLocationInView().x;
    float touchY = visibleSize.height - touch->getStartLocationInView().y;
    
    
    if(!actionPlaying && touchX > unitOriginPosition.x  && touchY > unitOriginPosition.y  && touchX < unitOriginPosition.x + (munitSize + 1) * count && touchY < unitOriginPosition.y + (munitSize+1) * count)
    {
        int indexX = (int)(touchX - unitOriginPosition.x) / ((int)munitSize + 1);
        int indexY = (int)(touchY - unitOriginPosition.y) / ((int)munitSize + 1);
        
        map<Vec2,HappyStartCell*>::iterator mIt = allcells.find(Vec2(indexX,indexY) );
        if(  mIt != allcells.end())
        {
            if(mIt->second)
            {
//                 mIt->second->_glprogramstate->setUniformFloat("nightDegree",1.0);
                 Vec2 touchpos = mIt->second->getposIndex();
                switch (_mMode) {
                    case CELL_TOUCH_MODE::CHESS_MODE:
                    {
                        int winside = SPCScene::winSide();
                        
                        if (playerCount%2 == 0) {
                            if(winside==0){
                                
                                mIt->second->setType(CELL_TYPE::TYPE_GREEN);
                            }else{
                                
                                mIt->second->setType(CELL_TYPE::TYPE_RED);
                            }
                        }else{
                            if(winside==1){
                            
                            mIt->second->setType(CELL_TYPE::TYPE_GREEN);
                        }else{
                            
                            mIt->second->setType(CELL_TYPE::TYPE_RED);
                        }
                        }
                        if (!mIt->second->getLocked()) {
                            playerCount ++;
                        }
                        
                        mIt->second->setLocked(true);
                        
                        checkoutResult();
                        
                        
                        break;
                    }
                        
                    case CELL_TOUCH_MODE::NORMAL_MODE:{
                        
                        cellsGet.clear();
                        
                        list<HappyStartCell*> templist = findTheGroupToRemove(mIt->second);//相邻所有
                        
                        
//                        list<HappyStartCell*> templist = getAllSameAround(mIt->second);//依据横竖斜向连线
                        
                        
                        for(HappyStartCell* temp :templist)
                        {
                            if (temp->getType() == CELL_TYPE::TYPE_7COLORS) {
                                
                                for(int i = 0;i < count;i++){
                                    
                                    if(allcells.find(Vec2(i,temp->getposIndex().y))!=allcells.end()){
                                        
                                        HappyStartCell* mmcell = allcells.find(Vec2(i,temp->getposIndex().y))->second;
                                        
                                        if(!(mmcell->gethasFind()))
                                        {
                                            templist.push_back(mmcell);
                                            mmcell->sethasFind(true);
                                        }
                                        
                                    }
                                    
                                }
                                
                                for(int i = 0;i < count;i++){
                                    HappyStartCell* mmcell = allcells.find(Vec2(temp->getposIndex().x,i))->second;
                                    
                                    if(allcells.find(Vec2(temp->getposIndex().x,i)) !=allcells.end() && mmcell &&  !mmcell->gethasFind())
                                    {
                                        templist.push_back(mmcell);
                                        mmcell->sethasFind(true);
                                    }
                                }
                                
                            }
                        }
                        
                        if(templist.size() == 0 && mIt->second->getType() ==  CELL_TYPE::TYPE_7COLORS){
                            //纵向消除
                            for(int i = 0;i < count;i++){
                                
                                HappyStartCell* mmcell = allcells.find(Vec2(mIt->second->getposIndex().x,i))->second;
                                if(allcells.find(Vec2(mIt->second->getposIndex().x,i)) != allcells.end()  &&  !mmcell->gethasFind() )
                                {
                                    templist.push_back(mmcell);
                                    mmcell->sethasFind(true);
                                }
                            }
                            //横向消除
                            for(int i = 0;i < count;i++){
                                
                                if(allcells.find(Vec2(i,mIt->second->getposIndex().y)) != allcells.end())
                                {
                                    HappyStartCell* mmcell = allcells.find(Vec2(i,mIt->second->getposIndex().y))->second;
                                    
                                    if(!(mmcell->gethasFind()))
                                    {
                                        templist.push_back(mmcell);
                                        mmcell->sethasFind(true);
                                    }
                                    
                                }
                                
                            }
                            
                            //                            templist.push_back(mIt->second);
                            
                        }

                        
                        auto blinkLamdas = [=](Node* psender)mutable{
                            
                            for(HappyStartCell* temp :templist)
                            {
                                temp->runAction(Blink::create(0.5, 2));
                            }
                        };
                        
                        
                        
                        isPaused = true;
                        
                        auto removeLoseLamdas = [=](Node* psender)mutable{
                            
                            
                            if(!mIt->second)
                                return ;
                            
                            
                            map<Vec2, HappyStartCell*>::iterator  _mpIterator = allcells.begin();
                            cellsCacheOne.clear();
                            for (; _mpIterator != allcells.end(); ++_mpIterator)
                            {
                                if(_mpIterator != allcells.end()){
                                    // 深度拷贝
                                    float xpos = _mpIterator->second->getposIndex().x;
                                    float ypos = _mpIterator->second->getposIndex().y;
                                    struct HappyStartStruct datatemp = {Vec2(xpos,ypos),_mpIterator->second->getType()};
                                    cellsCacheOne.push_back( datatemp );
                                }
                            }
                            
                            if (!(mIt->second->getType() == CELL_TYPE::TYPE_7COLORS) ) {
                                
                                
                                if(templist.size() >= 5)
                                {
                                    for(auto temp :templist)
                                    {
                                        if(temp  && temp->getposIndex().y == mIt->second->getposIndex().y && temp->getposIndex().x == mIt->second->getposIndex().x)
                                        {
                                            HappyStartCell* tempcell7 = HappyStartCell::create();
                                            tempcell7->setParameters(cocos2d::Color3B::BLUE,unitOriginPosition, cocos2d::Size(munitSize,munitSize),mIt->second->getposIndex() , count);
                                            
                                            this->removeChild((Node*)mIt->second);
                                            templist.remove(temp);
                                            mIt->second = tempcell7;
                                            tempcell7->setType(CELL_TYPE::TYPE_7COLORS);
                                            this->addChild((Node*)tempcell7);
                                            
                                            if (this->getChildByTag(-190)) {
                                                tempcell7->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
                                            }
                                            
                                            if (this->getChildByName("GameFinish")) {
                                                
                                                auto frootNode = this->getChildByName("GameFinish")->getChildByName("FinishPopRoot");
                                                
                                                tempcell7->setGlobalZOrder(frootNode->getGlobalZOrder()-1);
                                            }
                                            
                                            
                                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("colorfulmade.wav",false, 1.0f, 0.0f, 0.6f);
                                            
                                            break;
                                        }
                                    }
                                }
                                
                            }
                            map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
                            
                            for (; mpIterator != allcells.end(); ++mpIterator)
                            {
                                if(mpIterator != allcells.end()){
                                    int countBelowWillRemove = 0;
                                    
                                    for(auto temp :templist)
                                    {
                                        if(temp  && temp->getposIndex().y < mpIterator->second->getposIndex().y && temp->getposIndex().x == mpIterator->second->getposIndex().x)// below the target
                                        {
                                            countBelowWillRemove ++;
                                        }
                                    }
                                    
                                    
                                    mpIterator->second->setdownShouldGo(countBelowWillRemove);
                                    
                                }
                            }
                            for(HappyStartCell* temp :templist)
                            {
                                
                                cocos2d::ParticleSystem* ps = cocos2d::ParticleExplosion::create();
                                ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("whitedot.png"));
                                ps->setPosition( unitOriginPosition +  temp->getposIndex()  * (1 + munitSize) + Vec2(munitSize/2,munitSize/2) );
                                ps->setStartColor(Color4F(temp->getColor()));
                                
                                ps->setTotalParticles(300);
                                ps->setScale(1);
                                ps->setLife(0.5);
                                ps->setSpeed(150);
                                ps->setStartSpin(munitSize);
                                this->addChild(ps);
                                
                                map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.find(temp->getposIndex());
                                
                                
                                
                                
                                if(mpIterator != allcells.end())
                                {
                                    
                                    switch (mpIterator->second->getType()) {
                                        case TYPE_NORMAL:
                                            break;
                                        case TYPE_7COLORS:
                                            break;
                                        case TYPE_BLUE:
                                            blueCount++;
                                            break;
                                        case TYPE_GREEN:
                                            greenCount++;
                                            break;
                                        case TYPE_GRAY:
                                            
                                            break;
                                        case TYPE_PINK:
                                            
                                            break;
                                        case TYPE_PURPLE:
                                            
                                            break;
                                        case TYPE_RED:
                                            redCount++;
                                            break;
                                        default:
                                            break;
                                    }
                                    
                                    allcells.erase(mpIterator);
                                }
                                
                            }
                            
                            for(HappyStartCell* temp :templist)
                            {
//                                
//                                if (temp->getType() == CELL_TYPE::TYPE_7COLORS) {
//                                    
//                                    if ( temp->getposIndex() != touchpos) {
//                                        
//                                        Touch* mTouch = new Touch();
//                                        mTouch->setTouchInfo(0, temp->getPositionX(), temp->getPositionY());
//                                        onTouchBegan(mTouch,NULL);
//                                    }else{
//                                        removeChild((Node*)temp);
//                                    }
//                                    
//                                }else
                                {
                                    removeChild((Node*)temp);
                                }
                            }
                            
                            
                            
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explod.wav",  false,
                                                                                        1.0f,  0.0f,  0.2f);
                            
                            for (int i = 0; i < count; i++) {
                                
                                int countY = 0;
                                
                                //某一列的cell剩余数
                                for (int j = 0; j<count; j++){
                                    
                                    if(allcells.find(Vec2(i,j)) != allcells.end())
                                    {
                                        countY++;
                                        
                                    }
                                }
                                
                                if(countY == 0)//第i列的cell个数为0
                                {
                                    for(int t = i+1; t < count; t++)
                                    {
                                        for (int k = 0; k < count; k++) {//此列以后所有cell的leftShouldGO 需要加1
                                            
                                            if (allcells.find(Vec2(t,k))!=allcells.end()) {
                                                //座椅运动矢量自加  位置X自减
                                                allcells.find(Vec2(t,k))->second->setleftShouldGo( allcells.find(Vec2(t,k))->second->getleftShouldGo()+1);
                                                allcells.find(Vec2(t,k))->second->setposIndex(allcells.find(Vec2(t,k))->second->getposIndex() +  cocos2d::Vec2(-1,0.0f));
                                            }
                                        }
                                        
                                    }
                                    
                                }
                            }
                            
                            
                            
                            //保存所有cell副本
                            list<HappyStartCell*> allcellsTemp;
                            
                            mpIterator = allcells.begin();
                            
                            for (; mpIterator != allcells.end(); mpIterator++)
                            {
                                mpIterator->second->setposIndex(mpIterator->second->getposIndex() +  cocos2d::Vec2(0,-1 * mpIterator->second->getdownShouldGo()));
                                
                                allcellsTemp.push_back(mpIterator->second);
                            }
                            
                            
                            allcells.clear();
                            
                            //重置所有 allcells的key
                            for (HappyStartCell* tempcell :allcellsTemp)
                            {
                                if(tempcell)
                                {
                                    tempcell->settimeToDelay(0.0f);
                                    tempcell->sethasFind(false);
                                    allcells.insert(pair<Vec2, HappyStartCell*> (tempcell->getposIndex(), tempcell));
                                }
                            }
                            
                            
                            isPauseFlag = true;
                            
                            
                        };
                        auto produceCell = [=](Node* pSender){
                            produceCells(pSender);
                            checkoutResult();
                        };
                        
                        this->runAction(Sequence::create(CallFuncN::create(blinkLamdas),DelayTime::create(0.5),CallFuncN::create(removeLoseLamdas),DelayTime::create(1),CallFuncN::create(produceCell), NULL));
                        
                        break;
                    
                    }
                        
                    case CELL_TOUCH_MODE::DELETE_ONE_MODE:
                    {
                        if (MittyToolData::getInstance()->getOneshotTool() <=0 ) {
                            showShortOfTool();
                            return false;
                        }
                        MittyToolData::getInstance()->useOneshotTool();
                        updateToolsBadge();
                        
                        Vec2 targetPos = unitOriginPosition +  mIt->second->getposIndex()  * (1 + munitSize) + Vec2(munitSize/2,munitSize/2);
                        
                        cocos2d::ParticleSystem* ps = cocos2d::ParticleMeteor::create();
                        ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("ball.png"));
                        
                        ps->setPosition( onShotButtonPos );
                        Vec2 mVec = (targetPos-onShotButtonPos);
                        ps->setStartColor(Color4F(mIt->second->getColor()));
                        
                        
                        
                        ps->setGravity(Vec2::ZERO);
                        ps->setTotalParticles(100);
//                        ps->setRotatePerSecondVar(0);
                        
                        ps->setScale(0.6);
                        ps->setLife(0.1);
//                        ps->setStartSpin(2);
//                        ps->setRotation(mVec.getAngle() - 135.f);
                        ps->setTag(1000);
                        this->addChild(ps);
                        
                        ps->runAction(MoveTo::create(1.0f, targetPos));
                        
                        //make the action delay 1s
                        
                        auto removeLamda = [=](Ref* pSender)
                        {
                            
                            
                            this->removeChildByTag(1000);
                            
                            cellsGet.clear();
                            list<HappyStartCell*> templist ;
                            
                            templist.push_back(mIt->second);
                            
                            map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
                            
                            for (; mpIterator != allcells.end(); ++mpIterator)
                            {
                                if(mpIterator != allcells.end()){
                                    int countBelowWillRemove = 0;
                                    
                                    for(HappyStartCell* temp :templist)
                                    {
                                        
                                        if(temp  && temp->getposIndex().y < mpIterator->second->getposIndex().y && temp->getposIndex().x == mpIterator->second->getposIndex().x)// below the target
                                        {
                                            countBelowWillRemove ++;
                                        }
                                    }
                                    
                                    mpIterator->second->setdownShouldGo(countBelowWillRemove);
                                    
                                }
                            }
                            
                            cocos2d::ParticleSystem* ps = cocos2d::ParticleExplosion::create();
                            ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("whitedot.png"));
                            
                            ps->setPosition( unitOriginPosition +  mIt->second->getposIndex()  * (1 + munitSize) + Vec2(munitSize/2,munitSize/2) );
                            //                            ps->setStartColor(Color4F(220,177.f,0.f,1.f));
                            ps->setStartColor(Color4F(mIt->second->getColor()));
                            
                            ps->setTotalParticles(300);
                            ps->setScale(1);
                            ps->setLife(0.5);
                            ps->setSpeed(150);
                            ps->setStartSpin(munitSize);
                            this->addChild(ps);
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explod.wav",  false,
                                                                                        1.0f,  0.0f,  0.2f);
                            
                            
                            mpIterator = allcells.begin();
                            cellsCacheOne.clear();
                            for (; mpIterator != allcells.end(); ++mpIterator)
                            {
                                if(mpIterator != allcells.end()){
                                    // 深度拷贝
                                    float xpos = mpIterator->second->getposIndex().x;
                                    float ypos = mpIterator->second->getposIndex().y;
                                    struct HappyStartStruct datatemp = {Vec2(xpos,ypos),mpIterator->second->getType()};
                                    cellsCacheOne.push_back( datatemp );
                                }
                            }
                            
                            
                            mpIterator = allcells.begin();
                            
                            for (; mpIterator != allcells.end(); mpIterator++)
                            {
                                mpIterator->second->settimeToDelay(1.0f);
                                
                            }
                            
                            
                            removeChild((Node*)mIt->second);
                            allcells.erase(mIt);
                            
                            
                            
                            for (int i = 0; i < count; i++) {
                                
                                int countY = 0;
                                
                                //某一列的cell剩余数
                                for (int j = 0; j<count; j++){
                                    
                                    if(allcells.find(Vec2(i,j)) != allcells.end())
                                    {
                                        countY++;
                                        
                                    }
                                }
                                
                                if(countY == 0)//第i列的cell个数为0
                                {
                                    for(int t = i+1; t < count; t++)
                                    {
                                        for (int k = 0; k < count; k++) {//此列以后所有cell的leftShouldGO 需要加1
                                            
                                            if (allcells.find(Vec2(t,k))!=allcells.end()) {
                                                //座椅运动矢量自加  位置X自减
                                                allcells.find(Vec2(t,k))->second->setleftShouldGo( allcells.find(Vec2(t,k))->second->getleftShouldGo()+1);
                                                allcells.find(Vec2(t,k))->second->setposIndex(allcells.find(Vec2(t,k))->second->getposIndex() +  cocos2d::Vec2(-1,0.0f));
                                            }
                                        }
                                        
                                    }
                                    
                                }
                            }
                            
                            
                            //保存所有cell副本
                            list<HappyStartCell*> allcellsTemp;
                            
                            mpIterator = allcells.begin();
                            
                            for (; mpIterator != allcells.end(); mpIterator++)
                            {
////                                //=-------------- i was confused ------ if this commented the parmeter ps(particlesystem) run a crash
//                                float bef = mpIterator->second->getposIndex().y;
////                                bef--;
                                mpIterator->second->setposIndex(mpIterator->second->getposIndex() +  cocos2d::Vec2(0,-1 * mpIterator->second->getdownShouldGo()));
                                
                                
                                allcellsTemp.push_back(mpIterator->second);
                            }
                            
                            
                            allcells.clear();
                            
                            //重置所有 allcells的key
                            for (HappyStartCell* tempcell :allcellsTemp)
                            {
                                if(tempcell)
                                {
                                    allcells.insert(pair<Vec2, HappyStartCell*> (tempcell->getposIndex(), tempcell));
                                }
                            }
                            
                            
                            mpIterator = allcells.begin();
                            
                            for (; mpIterator != allcells.end(); mpIterator++)
                            {
                                mpIterator->second->settimeToDelay(0.0f);
                            }
                             actionPlaying = false;
                            checkoutResult();
                            
                            produceCells(NULL);
                            
                            
                        };
                        auto blinklamda = [=](Node* pSender){
                            
                                mIt->second->runAction(Blink::create(1, 2));
                            
                        };
                        
                        auto callNextAnimation = cocos2d::CallFuncN::create(removeLamda);
                        
                        actionPlaying = true;
                        
                        this->runAction(cocos2d::Sequence::create(cocos2d::CallFuncN::create(blinklamda),cocos2d::DelayTime::create(1.0f),callNextAnimation,NULL));
                        
                        
                        break;
                        
                }
                        
                    case CHANGE_COLOR_RANDOM:
                        if (MittyToolData::getInstance()->getRandomTool() <=0 ) {
                            showShortOfTool();
                            return false;
                        }
                        MittyToolData::getInstance()->useRandomTool();
                       
                        updateToolsBadge();
                        mIt->second->changeTypeRandom();
                        
                        
                        break;
                        
                    case CHANGE_COLOR:
                    {
                        if (MittyToolData::getInstance()->getChangeTypeTool() <=0 ) {
                            showShortOfTool();
                            return false;
                        }
                        MittyToolData::getInstance()->useChangeTypeTool();
                       
                        updateToolsBadge();
                        
                        auto rootNode = CSLoader::createNode("ColorSelectNode.csb");
                        rootNode->setAnchorPoint(Vec2(0.5, 0.5));
                        
                        this->addChild(rootNode);
                        rootNode->setTag(1999);
                        rootNode->setGlobalZOrder(5);
                        rootNode->setPosition( mIt->second->getPosition() );
                        rootNode->setScale(0.01);
                        rootNode->runAction(ScaleTo::create(0.2, 1));
                        
                        cocos2d::ui::Button* btnRed =  (cocos2d::ui::Button*)rootNode->getChildByTag(32);//红
                        btnRed->addTouchEventListener(CC_CALLBACK_2(GameScene::seletctCellolor, this) );
                        btnRed->setPropagateTouchEvents(false);
                        btnRed->setSwallowTouches(true);
                        
                        cocos2d::ui::Button* btnBlue =  (cocos2d::ui::Button*)rootNode->getChildByTag(33);//蓝
                        btnBlue->addTouchEventListener(CC_CALLBACK_2(GameScene::seletctCellolor, this) );
                        btnBlue->setPropagateTouchEvents(false);
                        
                        cocos2d::ui::Button* btnGreen =  (cocos2d::ui::Button*)rootNode->getChildByTag(34);//绿
                        btnGreen->addTouchEventListener(CC_CALLBACK_2(GameScene::seletctCellolor, this) );
                        btnGreen->setPropagateTouchEvents(false);
                        
                        cocos2d::ui::Button* btnGray =  (cocos2d::ui::Button*)rootNode->getChildByTag(49);//黄
                        btnGray->addTouchEventListener(CC_CALLBACK_2(GameScene::seletctCellolor, this) );
                        btnGray->setPropagateTouchEvents(false);
                        btnGray->setSwallowTouches(true);
                        
                        cocos2d::ui::Button* btnPink =  (cocos2d::ui::Button*)rootNode->getChildByTag(50);//粉
                        btnPink->addTouchEventListener(CC_CALLBACK_2(GameScene::seletctCellolor, this) );
                        btnPink->setPropagateTouchEvents(false);
                        
                        cocos2d::ui::Button* btnPurple =  (cocos2d::ui::Button*)rootNode->getChildByTag(51);//紫
                        btnPurple->addTouchEventListener(CC_CALLBACK_2(GameScene::seletctCellolor, this) );
                        btnPurple->setPropagateTouchEvents(false);
                        
                        
                        
                        
                   
                        seletingCell = mIt->second;
                        
                        isPaused = true;
                        
                        break;
                    }
                        
                    default:
                        break;
                }
                
            }
           
        }
        
    }
    
    return true;
}


void GameScene::deleteTargetCell(map<Vec2,HappyStartCell*>::iterator   targetCell)
{
    
    allcells.erase(targetCell);
    removeChild((Node*)targetCell->second);

}

bool firstTimeRun = true;



list<HappyStartCell*> GameScene::getAllSameAround(HappyStartCell* targetCell)
{

    list<HappyStartCell*> templist;// = findTheGroupToRemove(targetCell);
    
    
    
    list<HappyStartCell*> templistH = getCellsSameToThisHorizental(targetCell);
    
    
    list<HappyStartCell*> templistV= getCellsSameToThisVertical(targetCell);
    
    
    list<HappyStartCell*> templistslantleft = getCellsSameToThisSlantLeft(targetCell);
    
    
    list<HappyStartCell*> templistslantright = getCellsSameToThisSlantRight(targetCell);
    
    
    for (HappyStartCell* tempcell: templistH) {
        if(tempcell->getposIndex() != targetCell->getposIndex())
        {
            templist.push_back(tempcell);
        }
    }
    
    for (HappyStartCell* tempcell: templistV) {
        if(tempcell->getposIndex() != targetCell->getposIndex())
        {
            templist.push_back(tempcell);
        }
    }
    
    
    for (HappyStartCell* tempcell: templistslantleft) {
        if(tempcell->getposIndex() != targetCell->getposIndex())
        {
            templist.push_back(tempcell);
        }
    }
    
    for (HappyStartCell* tempcell: templistslantright) {
        if(tempcell->getposIndex() != targetCell->getposIndex())
        {
            templist.push_back(tempcell);
        }
    }
    
    
    templist.push_back(targetCell);
    return templist;
}





//递归遍历查询所有临近的方块
 list<HappyStartCell*> GameScene::findTheGroupToRemove(HappyStartCell* targetCell)
{
   
    if(targetCell)
    {
        enum CELL_TYPE targetType = targetCell->getType();
        
        //  targetCell->sethasFind(true);
        int targetposX =  targetCell->getposIndex().x;
        
        int targetposY =  targetCell->getposIndex().y;
        
        
        list<HappyStartCell*> mcellsAround;
        
        if(allcells.find(Vec2(targetposX + 1 , targetposY)) != allcells.end()){
            
            mcellsAround.push_back(allcells.find(Vec2(targetposX + 1 , targetposY))->second);
        }
        
        if(allcells.find(Vec2(targetposX - 1 , targetposY)) != allcells.end()){
            
            mcellsAround.push_back(allcells.find(Vec2(targetposX - 1 , targetposY))->second);
        }
        if(allcells.find(Vec2(targetposX , targetposY+1)) != allcells.end()){
            
            mcellsAround.push_back(allcells.find(Vec2(targetposX , targetposY+1))->second);
        }
        if(allcells.find(Vec2(targetposX , targetposY-1)) != allcells.end()){
            
            mcellsAround.push_back(allcells.find(Vec2(targetposX , targetposY-1))->second);
        }
        if(firstTimeRun && getCountSameToThis(targetCell)> 0 )
        {
            cellsGet.push_back(targetCell);
            targetCell->sethasFind(true);
            firstTimeRun = false;
        }
        
        
        for (HappyStartCell* tempCell:mcellsAround) {
                if(tempCell && tempCell->getType() == targetType  && !tempCell->gethasFind())
                {
                    cellsGet.push_back(tempCell);
                    tempCell->sethasFind(true);
                    if(getCountSameToThis(tempCell) == 0)
                    {
                        continue;
                    }else{
                         findTheGroupToRemove(tempCell);
                    }
                }
            }
        }
     return cellsGet;
}

//确认周围可以进行递归的方块数目
int GameScene::getCountSameToThis(HappyStartCell* targetCell)
{
    enum CELL_TYPE targetType = targetCell->getType();
    int doCount = 0;
    if(targetCell)
    {
//        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
        list<HappyStartCell*> mcellsAround;
        
        
        if(allcells.find(Vec2(targetposX + 1 , targetposY)) != allcells.end()){
        
            mcellsAround.push_back(allcells.find(Vec2(targetposX + 1 , targetposY))->second);
        }
        
        if(allcells.find(Vec2(targetposX - 1 , targetposY)) != allcells.end()){
            
           mcellsAround.push_back(allcells.find(Vec2(targetposX - 1 , targetposY))->second);
        }
        if(allcells.find(Vec2(targetposX , targetposY+1)) != allcells.end()){
            
           mcellsAround.push_back(allcells.find(Vec2(targetposX , targetposY+1))->second);
        }
        if(allcells.find(Vec2(targetposX , targetposY-1)) != allcells.end()){
            
            mcellsAround.push_back(allcells.find(Vec2(targetposX , targetposY-1))->second);
        }
        
        for (HappyStartCell* tempCell:mcellsAround) {
            if(tempCell && tempCell->getType() == targetType  && !tempCell->gethasFind())
            {
                doCount++;
            }
        }
    
    }
    
    return doCount; 
}


 list<HappyStartCell*>  GameScene::getCellsSameToThisSlantLeft(HappyStartCell* targetCell)
{
    
    map<Vec2, HappyStartCell*>::iterator  mpIteratort = allcells.begin();
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    
    enum CELL_TYPE targetType = targetCell->getType();
     list<HappyStartCell*> mcellsAround;
    if(targetCell)
    {
        //        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
        for (int i = targetposX-1;i >= 0; i--) {
            if( allcells.find(Vec2(i ,targetposY + targetposX -i )) != allcells.end()  && !allcells.find(Vec2(i ,targetposY + targetposX -i ))->second->gethasFind()){
            
                if(allcells.find(Vec2(i ,targetposY + targetposX -i ))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(i ,targetposY + targetposX -i ))->second->gethasFind()) {
                    continue;
                }
                allcells.find(Vec2(i ,targetposY + targetposX -i  ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(i ,targetposY + targetposX -i ))->second);
            }
        }
        
        
        for (int i = targetposX+1; i < count; i++) {
            if( allcells.find(Vec2(i ,targetposY + targetposX -i )) != allcells.end() && !allcells.find(Vec2(i ,targetposY + targetposX -i ))->second->gethasFind()){
                
                if(allcells.find(Vec2(i ,targetposY + targetposX -i ))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(i ,targetposY + targetposX -i ))->second->gethasFind()) {
                    continue;
                }
                allcells.find(Vec2(i ,targetposY + targetposX -i  ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(i ,targetposY + targetposX -i ))->second);
            }
        }
        if (mcellsAround.size() >0) {
            mcellsAround.push_back(targetCell);
        }
    }
    mpIteratort = allcells.begin();
    
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    return mcellsAround;
}
list<HappyStartCell*>  GameScene::getCellsSameToThisSlantRight(HappyStartCell* targetCell)
{
    map<Vec2, HappyStartCell*>::iterator  mpIteratort = allcells.begin();
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    
    enum CELL_TYPE targetType = targetCell->getType();
    list<HappyStartCell*> mcellsAround;
    if(targetCell)
    {
        //        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
        
        for (int i = 1;i <= (MAX(targetposX, targetposY)); i++) {
            if( allcells.find(Vec2(targetposX-i ,targetposY -i )) != allcells.end() && !allcells.find(Vec2(targetposX-i ,targetposY -i ))->second->gethasFind()){
                
                if(allcells.find(Vec2(targetposX-i ,targetposY -i))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(targetposX-i ,targetposY -i ))->second->gethasFind()) {
                    continue;
                }
                allcells.find(Vec2(targetposX-i ,targetposY -i ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(targetposX-i ,targetposY -i ))->second);
            }
        }
        
        
        for (int i = 1; i < count - (MIN(targetposX, targetposY)) ; i++) {
            if( allcells.find(Vec2(targetposX+i ,targetposY+i )) != allcells.end() && !allcells.find(Vec2(targetposX+i ,targetposY+i))->second->gethasFind()){
                
                if(allcells.find(Vec2(targetposX+i ,targetposY+i))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(targetposX+i ,targetposY+i))->second->gethasFind()) {
                    continue;
                }
                allcells.find(Vec2(targetposX+i ,targetposY+i))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(targetposX+i ,targetposY+i))->second);
            }
        }
        
        if (mcellsAround.size() >0) {
            mcellsAround.push_back(targetCell);
        }
    }
    mpIteratort = allcells.begin();
    
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    return mcellsAround;
}


list<HappyStartCell*>  GameScene::getCellsSameToThisHorizental(HappyStartCell* targetCell)
{
    map<Vec2, HappyStartCell*>::iterator  mpIteratort = allcells.begin();
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    
    enum CELL_TYPE targetType = targetCell->getType();
    list<HappyStartCell*> mcellsAround;
    if(targetCell)
    {
        //        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
        for (int i = targetposX-1;i >= 0; i--) {
            if( allcells.find(Vec2(i ,targetposY  )) != allcells.end() && !allcells.find(Vec2(i ,targetposY ))->second->gethasFind()){
                
                if(allcells.find(Vec2(i ,targetposY))->second->getType() != targetType )
                {
                    break;
                }
                
                if (allcells.find(Vec2(i ,targetposY ))->second->gethasFind()) {
                    continue;
                }
                
                
                allcells.find(Vec2(i ,targetposY ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(i ,targetposY ))->second);
            }
        }
        
        
        for (int i = targetposX+1; i < count; i++) {
            if( allcells.find(Vec2(i ,targetposY  )) != allcells.end() && !allcells.find(Vec2(i ,targetposY ))->second->gethasFind()){
                
                if(allcells.find(Vec2(i ,targetposY ))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(i ,targetposY ))->second->gethasFind()) {
                    continue;
                }
                allcells.find(Vec2(i ,targetposY ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(i ,targetposY ))->second);
            }
        }
        
        if (mcellsAround.size() >0) {
            mcellsAround.push_back(targetCell);
        }
    }
    mpIteratort = allcells.begin();
    
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    return mcellsAround;
}


list<HappyStartCell*>  GameScene::getCellsSameToThisVertical(HappyStartCell* targetCell)
{
    
    map<Vec2, HappyStartCell*>::iterator  mpIteratort = allcells.begin();
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    
    enum CELL_TYPE targetType = targetCell->getType();
    list<HappyStartCell*> mcellsAround;
    if(targetCell)
    {
        //        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
        for (int i = targetposY-1;i >= 0; i--) {
            if( allcells.find(Vec2(targetposX ,i )) != allcells.end() && !allcells.find(Vec2(targetposX ,i))->second->gethasFind()){
                
                if(allcells.find(Vec2(targetposX ,i))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(targetposX,i))->second->gethasFind()) {
                    continue;
                }
                
                allcells.find(Vec2(targetposX,i ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(targetposX,i ))->second);
            }
        }
        
        
        for (int i = targetposY+1; i < count; i++) {
            if( allcells.find(Vec2(targetposX ,i )) != allcells.end() && !allcells.find(Vec2(targetposX ,i ))->second->gethasFind()){
                
                if(allcells.find(Vec2(targetposX ,i ))->second->getType() != targetType )
                {
                    break;
                }
                if (allcells.find(Vec2(targetposX,i))->second->gethasFind()) {
                    continue;
                }
                allcells.find(Vec2(targetposX ,i ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(targetposX ,i ))->second);
            }
        }
        
        if (mcellsAround.size() >0) {
            mcellsAround.push_back(targetCell);
        }
    }
    
    mpIteratort = allcells.begin();
    
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    return mcellsAround;
}


list<HappyStartCell*>  GameScene::getCellsHorizentalFor7Color(HappyStartCell* targetCell)
{
    map<Vec2, HappyStartCell*>::iterator  mpIteratort = allcells.begin();
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    
    list<HappyStartCell*> mcellsAround;
    if(targetCell)
    {
        //        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
      
        for (int i = targetposX-1;i >= 0; i--) {
            if( allcells.find(Vec2(i ,targetposY  )) != allcells.end() && !allcells.find(Vec2(i ,targetposY ))->second->gethasFind()){
                allcells.find(Vec2(i ,targetposY ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(i ,targetposY ))->second);
            }
        }
        
        
        for (int i = targetposX+1; i < count; i++) {
            if( allcells.find(Vec2(i ,targetposY  )) != allcells.end() && !allcells.find(Vec2(i ,targetposY ))->second->gethasFind()){
                allcells.find(Vec2(i ,targetposY ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(i ,targetposY ))->second);
            }
        }
        
        if (mcellsAround.size() >0) {
            mcellsAround.push_back(targetCell);
        }
    }
    mpIteratort = allcells.begin();
    
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    return mcellsAround;
    
    
}


list<HappyStartCell*>  GameScene::getCellsVerticalFor7Color(HappyStartCell* targetCell)
{
    
    map<Vec2, HappyStartCell*>::iterator  mpIteratort = allcells.begin();
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    
    list<HappyStartCell*> mcellsAround;
    if(targetCell)
    {
        //        targetCell->sethasFind(true);
        
        int targetposX =  targetCell->getposIndex().x;
        int targetposY =  targetCell->getposIndex().y;
        for (int i = targetposY-1;i >= 0; i--) {
            if( allcells.find(Vec2(targetposX ,i )) != allcells.end() && !allcells.find(Vec2(targetposX ,i))->second->gethasFind()){
                
                
                allcells.find(Vec2(targetposX,i ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(targetposX,i ))->second);
            }
        }
        
        
        for (int i = targetposY+1; i < count; i++) {
            if( allcells.find(Vec2(targetposX ,i )) != allcells.end() && !allcells.find(Vec2(targetposX ,i ))->second->gethasFind()){
                allcells.find(Vec2(targetposX ,i ))->second->sethasFind(true);
                mcellsAround.push_back(allcells.find(Vec2(targetposX ,i ))->second);
            }
        }
        
        if (mcellsAround.size() >0) {
            mcellsAround.push_back(targetCell);
        }
    }
    
    mpIteratort = allcells.begin();
    
    
    for (; mpIteratort != allcells.end(); ++mpIteratort)
    {
        mpIteratort->second->sethasFind(false);
    }
    
    return mcellsAround;
}

void GameScene::produceCells(Node* psender)
{
    if (isGameFinish) {
        return;
    }
    while(allcells.size() != count*count) {
        
        for (int i = 0; i< count; i++) {
            for (int j = count-1; j >=0; j--) {
                //不存在这个cell
                if (allcells.find(Vec2(i, j)) == allcells.end()) {
                    
                    HappyStartCell* mm =  HappyStartCell::create();
                    mm->setParameters(Color3B(25.5f * i,25.5f * j,10.f*(i+j)),unitOriginPosition,cocos2d::Size(munitSize,munitSize),Vec2(i,j),count);
                    allcells.insert(pair<Vec2, HappyStartCell*> (Vec2(i,j), mm));
                    addChild((Node*)mm);
                  
                    if (this->getChildByTag(-190)) {
                        mm->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
                    }
                    
                    if (this->getChildByName("GameFinish")) {
                        
                        auto frootNode = this->getChildByName("GameFinish")->getChildByName("FinishPopRoot");
                        
                        mm->setGlobalZOrder(frootNode->getGlobalZOrder()-1);
                    }
                    
                    
                    
                    
                    srand(0);
                    
                        switch (_mMode) {
                            case CELL_TOUCH_MODE::CHESS_MODE:
                            {
                                mm->setType(CELL_TYPE::TYPE_NORMAL);
                                break;
                            }
                                
                            case CELL_TOUCH_MODE::NORMAL_MODE:
                            case CHANGE_COLOR_RANDOM:
                            case CELL_TOUCH_MODE::DELETE_ONE_MODE:
                            case CHANGE_COLOR:
                            {
                                int typeFind = 1;
                                
                                switch (mdifficult) {
                                    case 0:
                                        typeFind = random(1,3);
                                        break;
                                    case 1:
                                        typeFind = random(1,5);
                                        break;
                                    case 2:
                                        typeFind = random(1,7);
                                        break;
                                    default:
                                        break;
                                }
                                
                                if (typeFind == 7) {
                                    
                                    typeFind = random(1,100) > 90 ? 7 : random(1, 6);
                                }
                                
                                
                                 mm->setType((CELL_TYPE)typeFind);
                                break;
                            }
                            default:
                                break;
                        };
                    
                }else{
                    continue;
                }
            }
        }
    }
    
    
    
}

//查询结果
void GameScene::checkoutResult()
{
    if(isGameFinish){
        return;
    }
    isPaused = true;
    
    int finalred = lv->redcount - redCount ;
    if (finalred < 0) {
        finalred = 0;
    }
    int finalgreen = lv->greencount - greenCount ;
    if (finalgreen < 0) {
        finalgreen = 0;
    }
    int finalblue = lv->bluecount - blueCount ;
    if (finalblue < 0) {
        finalblue = 0;
    }
    
    auto rootNode = this->getChildByName("MainSceneRoot");
   
    int nowScore = lv->score -(redCount*10 + greenCount*20 + blueCount*30);
    const char * scotext = String::createWithFormat("目标分数 : %d" , nowScore > 0 ? nowScore : 0 )->getCString();
    
    if (rootNode->getChildByTag(123)&&rootNode->getChildByTag(123)->isVisible()) {
        
        cocos2d::ui::Text* redCountt = (cocos2d::ui::Text*)rootNode->getChildByTag(123);
        const char * scoretext = String::createWithFormat(" x %d " , finalred)->getCString();
        redCountt->setString(scoretext);
        
        
        
        cocos2d::ui::Text* greenCountt = (cocos2d::ui::Text*)rootNode->getChildByTag(126);
        const char * greenT = String::createWithFormat(" x %d " ,finalgreen)->getCString();
        greenCountt->setString(greenT);
        
        cocos2d::ui::Text* blueCountt = (cocos2d::ui::Text*)rootNode->getChildByTag(127);
        const char * blueT = String::createWithFormat(" x %d " ,finalblue)->getCString();
        blueCountt->setString(blueT);

    }
    
    if (scoreLAbel&&scoreLAbel->isVisible()) {
        
        scoreLAbel = (cocos2d::ui::Text*)(rootNode->getChildByTag(7));
        scoreLAbel->setString(scotext);
        scoreLAbel->runAction(Sequence::create(ScaleTo::create(0.15,1.1),ScaleTo::create(0.15, 1.0), NULL));
        
    }
    
    switch (_mMode) {
        case CELL_TOUCH_MODE::CHESS_MODE:
        {
            map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
            
            
            for (; mpIterator != allcells.end(); ++mpIterator)
            {
                if (mpIterator->second->getType() == CELL_TYPE::TYPE_NORMAL) {
                    continue;
                }
                unsigned long countsizeRight  = getCellsSameToThisSlantRight(mpIterator->second).size();
                unsigned long countsizeH  =  getCellsSameToThisHorizental(mpIterator->second).size();
                unsigned long countsizeV = getCellsSameToThisVertical(mpIterator->second).size();
                unsigned long countsizeleft  = getCellsSameToThisSlantLeft(mpIterator->second).size();
               
                isPauseFlag = true;
                if (countsizeRight >= 3 || countsizeH >= 3 || countsizeV >= 3 ||countsizeleft >= 3 ) {
                    
                    auto gotoFinish = [=](Ref* pSender)
                    {
                        
                        cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
                        auto rootNode = CSLoader::createNode("AddScoreLayer.csb");
                       
                        
                        
                        rootNode->setAnchorPoint(Vec2(0.5,0.5));
                       
                        this->addChild(rootNode);
                        
                        
                        if (this->getChildByTag(-190)) {
                            rootNode->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
                        }
                        
                        
                        rootNode->setName("scoreshow");
                        rootNode->setPosition(visibleSize/2);
                        cocos2d::ui::Text* scorete = (cocos2d::ui::Text*)rootNode->getChildByTag(94);
                        
                        const char * scoretext = "";
                        if (mpIterator->second->getType() == 2) {
                            scoretext = "左边胜利啦～";
                        }else{
                            scoretext = "右边胜利啦～";
                        }
                        scorete->setString(scoretext);
                        
                        
                        rootNode->runAction(Sequence::create(ScaleTo::create(0.15,2.05),ScaleTo::create(0.15, 2.0), NULL));
                        isPaused = true;
                        isGameFinish = true;
                    };
                    
                    auto hidewin = [=](Ref* pSender)
                    {
                        auto hidewinDone = [=](Ref* pSender)
                        {
                            this->removeChildByName("scoreshow");
//                            isPauseFlag = true;
//                            this->loadMap(this, ui::Widget::TouchEventType::ENDED );
                        };
                        
                        playerCount = 0;
                        this->getChildByName("scoreshow")->runAction(Sequence::create(ScaleTo::create(0.15,0),CallFuncN::create(hidewinDone), NULL));
                        
                        gameWin();
                    };
                    
                    
                    this->runAction(Sequence::create(DelayTime::create(1),CallFuncN::create(gotoFinish),DelayTime::create(2) ,CallFuncN::create(hidewin), NULL) );
                    
                    break;
                }
            }
            
            break;
        }
            
        case CELL_TOUCH_MODE::NORMAL_MODE:
        case CHANGE_COLOR_RANDOM:
        case CELL_TOUCH_MODE::DELETE_ONE_MODE:
        case CHANGE_COLOR:
        {
            reaminingStep--;
            if (reaminingStep<0) {
                reaminingStep = 0;
            }
            auto steplabel = (cocos2d::ui::Text*)rootNode->getChildByTag(36);
            const char * steptext = String::createWithFormat("还剩 %d 步" ,reaminingStep)->getCString();
            steplabel->setString(steptext);
            isPauseFlag = true;
            if ( (redCount >= lv->redcount && greenCount>=lv->greencount && blueCount>=lv->bluecount&& strcmp(lv->type.c_str(), "FIND_COLOR") == 0 )   || (strcmp(lv->type.c_str(), "GET_SCORE") == 0 && nowScore <= 0)) {
                gameWin();
                return;
            }else{
                if(reaminingStep <= 0){
                    gameFailed();
                }
            }
            isPaused = true;
            int sumCount = 0;
            bool canNotContinue = false;
            
            map<Vec2, HappyStartCell*>::iterator  mpIterator = allcells.begin();
            
            
            for (; mpIterator != allcells.end(); ++mpIterator)
            {
                sumCount += getCountSameToThis(mpIterator->second);
            }
            
            canNotContinue = ((sumCount==0)?true:false);
            
            mpIterator = allcells.begin();
            for (; mpIterator != allcells.end(); ++mpIterator)
            {
                if (mpIterator->second->getType() == CELL_TYPE::TYPE_7COLORS) {
                    canNotContinue = false;
                    
                }
            }
            
            CCLOG("canNotContinue --- %d",canNotContinue);
            
            if(canNotContinue)
            {
                Sprite* cannotcontinue = Sprite::create("cannotcontinue.png");
                cannotcontinue->setPosition(Director::getInstance()->getVisibleSize()/2);
                cannotcontinue->setScale(0.1);
                this->addChild(cannotcontinue);
                
                auto createLamada = [=](Ref* pSender)mutable{
                    
                    loadMap(NULL,cocos2d::ui::Widget::TouchEventType::ENDED);
                };
                
                auto removeLamada = [=](Ref* pSender)mutable{
                
                    cannotcontinue->removeFromParent();
                
                };
                
                if (this->getChildByTag(-190)) {
                    cannotcontinue->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
                }
                
                if (this->getChildByName("GameFinish")) {
                    
                    auto frootNode = this->getChildByName("GameFinish")->getChildByName("FinishPopRoot");
                    
                    cannotcontinue->setGlobalZOrder(frootNode->getGlobalZOrder()-1);
                }
                
                cannotcontinue->runAction(Sequence::create( ScaleTo::create(0.5, 0.4),DelayTime::create(1),CallFuncN::create(createLamada),DelayTime::create(0.5),ScaleTo::create(0.5, 0.1),CallFuncN::create(removeLamada),NULL));
            }
            break;
        }
            
            
        default:
            break;
    };
}
void GameScene::gameWin()
{
    if (isGameWin) {
        return;
    }
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    mTool_OneShot->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    mTool_RandomType->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    mTool_ChangeType->setTouchEnabled(false);
    
    isGameFinish = true;
    isGameWin = true;
    isPaused = true;
    auto gotoFinish = [=](Ref* pSender)
    {
        Layer* ly = FinishPopup::create();
        this->addChild(ly);
        ly->setName("GameFinish");
        
        auto rootNode = ly->getChildByName("FinishPopRoot");
        cocos2d::ui::Button* btnRetry =  (cocos2d::ui::Button*)rootNode->getChildByTag(19)->getChildByTag(14);
        btnRetry->addTouchEventListener(CC_CALLBACK_2(GameScene::loadMap, this) );
        
        cocos2d::ui::Button* btnNext =  (cocos2d::ui::Button*)rootNode->getChildByTag(19)->getChildByTag(31);
        btnNext->addTouchEventListener(CC_CALLBACK_2(GameScene::gotoLevelNext, this) );
        
        cocos2d::ui::Button* btnExit =  (cocos2d::ui::Button*)rootNode->getChildByTag(19)->getChildByTag(16);
        btnExit->addTouchEventListener(CC_CALLBACK_2(GameScene::exitGame, this) );
        
        cocos2d::ui::Button* btnShare =  (cocos2d::ui::Button*)rootNode->getChildByTag(19)->getChildByTag(75);
        btnShare->addTouchEventListener(CC_CALLBACK_2(GameScene::shareScore, this) );
        
        
        cocos2d::ui::Text* helptext = (cocos2d::ui::Text*)rootNode->getChildByTag(19)->getChildByTag(53);
        
        std::string  storyArr[3] = {"使用随机变色道具，\n可能性价比很高哦~","要是你是个多疑的人，\n最好使用选择变色道具吧~","要是有个小东西挡住你的去路，\n用消除单个道具干掉他吧！！！"};
        
        __String* ss = __String::createWithFormat("HERO_TALK_UNLOCKED_LEVEL_%d",lv->getLevelCode()+1);
        UserDefault::getInstance()->setBoolForKey(ss->getCString(),true);
        
        if (UserDefault::getInstance()->getIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED") < lv->getLevelCode()+1 ){
            UserDefault::getInstance()->setIntegerForKey("HERO_TALK_MAX_LEVEL_UNLOCKED",lv->getLevelCode()+1);
        }
        
        
        rootNode->setLocalZOrder(10000);
        
        
        
        int index = random(0, 2);
        helptext->setString(storyArr[index]);
        
        //             helptext->setString("afahfvkjv");
        
        cocos2d::ui::Text* scorete = (cocos2d::ui::Text*)rootNode->getChildByTag(19)->getChildByTag(77);
        
        const char * scoretext = String::createWithFormat("得分 : %d" , redCount*10 + greenCount*20 + blueCount*30 )->getCString();
        scorete->setString(scoretext);
        
        cocos2d::ui::Text* scoretee = (cocos2d::ui::Text*)rootNode->getChildByTag(19)->getChildByTag(121);
        
        const char * sctext = String::createWithFormat("击败了 %d %%的玩家" , testScoreDegree(redCount*10 + greenCount*20 + blueCount*30) )->getCString();
        scoretee->setString(sctext);
        
        
        isPaused = true;
        isGameFinish = true;
    };
    
    this->runAction(Sequence::create(DelayTime::create(1),CallFuncN::create(gotoFinish), NULL) );
    
}

void GameScene::gameFailed()
{
    if (isGameFailed) {
        return;
    }
    isPaused = true;
    isGameFinish = true;
    isGameFailed = true;
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(11);
    mTool_OneShot->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_RandomType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(19);
    mTool_RandomType->setTouchEnabled(false);
    
    cocos2d::ui::CheckBox* mTool_ChangeType = (cocos2d::ui::CheckBox*)(Sprite*)getChildByName("MainSceneRoot")->getChildByTag(20);
    mTool_ChangeType->setTouchEnabled(false);
    
    
    isPaused = true;
    auto gotoFinish = [=](Ref* pSender)
    {
        Layer* ly = FailedPopup::create();
        this->addChild(ly);
        ly->setName("GameFailed");
        
        auto rootNode = ly->getChildByName("FailedPopRoot");
        cocos2d::ui::Button* btnRetry =  (cocos2d::ui::Button*)rootNode->getChildByTag(47)->getChildByTag(49);
        btnRetry->addTouchEventListener(CC_CALLBACK_2(GameScene::loadMap, this) );
        
        
        cocos2d::ui::Button* btnExit =  (cocos2d::ui::Button*)rootNode->getChildByTag(47)->getChildByTag(48);
        btnExit->addTouchEventListener(CC_CALLBACK_2(GameScene::exitGame, this) );
        
        cocos2d::ui::Text* helptext = (cocos2d::ui::Text*)rootNode->getChildByTag(47)->getChildByTag(51);
        
        std::string  storyArr[3] = {"使用随机变色道具，\n可能性价比很高哦~","要是你是个多疑的人，\n最好使用选择变色道具吧~","要是有个小东西挡住你的去路，\n用消除单个道具干掉他吧！！！"};
        
        if (this->getChildByTag(-190)) {
            rootNode->setGlobalZOrder(this->getChildByTag(-190)->getGlobalZOrder()-1);
        }
        
        int index = random(0, 2);
        helptext->setString(storyArr[index]);
       
       
    };
    
    this->runAction(Sequence::create(DelayTime::create(1.0),CallFuncN::create(gotoFinish), NULL) );
    
}

int GameScene::testScoreDegree(float score){

    int scoreTarget[10] = {0,100,200,300,400,500,600,700,800,900};
    
    for (int i = 0;i < 10 ;i++) {
        if(score < scoreTarget[i]){
            return  (i-1) * 10;
        }
    }
    
    return 0;
}
