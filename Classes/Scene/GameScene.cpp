#include "GameScene.h"
#include "cocostudio/CocoStudio.h"

#include "HappyStartCell.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
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
    _mMode = CELL_TOUCH_MODE::NORMAL_MODE;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->addChild(LayerColor::create(Color4B(0,200,120,255),visibleSize.width,visibleSize.height));
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);
    
    
    cocos2d::ui::Button* btnReset =  (cocos2d::ui::Button*)rootNode->getChildByTag(8);
    btnReset->addTouchEventListener(CC_CALLBACK_2(GameScene::loadMap, this) );
    
    cocos2d::ui::Button* btnBack =  (cocos2d::ui::Button*)rootNode->getChildByTag(10);
    btnBack->addTouchEventListener(CC_CALLBACK_2(GameScene::backOneStep, this) );
    
    
    cocos2d::ui::Button* btnExit =  (cocos2d::ui::Button*)rootNode->getChildByTag(9);
    btnExit->addTouchEventListener(CC_CALLBACK_2(GameScene::exitGame, this) );
    
    scoreLAbel = (cocos2d::ui::Text*)rootNode->getChildByTag(7);
    
    cocos2d::ui::CheckBox* mTool_OneShot = (cocos2d::ui::CheckBox*)rootNode->getChildByTag(11);
    mTool_OneShot->addClickEventListener(CC_CALLBACK_1(GameScene::deleteOneCell,this));
    onShotButtonPos = mTool_OneShot->getPosition();
    
    actionPlaying = false;
    count = 6 ;
    munitSize =(visibleSize.height < visibleSize.width?visibleSize.height:visibleSize.width - 120)/count - 1;
    unitOriginPosition = origin + Vec2((visibleSize.width - (munitSize + 1) * count)/2 ,  (visibleSize.height - (munitSize + 1) * count )/2);
    
    loadMap(NULL,cocos2d::ui::Widget::TouchEventType::BEGAN);
    
    auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("explod.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("colorfulmade.wav");
    
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    auto removeLamda = [=](Ref* pSender)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("playbg.mp3",true);
    };
    
    this->runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(removeLamda), NULL) );
   
    return true;
}


void GameScene::loadMap(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    scoreLAbel->setString("SCORE : 0");
    
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
            mm->setParameters(Color3B(25.5f * i,25.5f * j,10.f*(i+j)),unitOriginPosition,Size(munitSize,munitSize),Vec2(i,j),count);
            allcells.insert(pair<Vec2, HappyStartCell*> (Vec2(i,j), mm));
            addChild((Node*)mm);
        }
    }
    
    
    
    //储存上一步地图情况
    cellsCacheOne.clear();
    mpIterator = allcells.begin();
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
    
    
    //    int blueCount = count/3;
    //    int redcount = count/3;
    //    int greencount = count/3;
    //
    //
    
    srand(0);
    
    mpIterator = allcells.begin();
    
    for (; mpIterator != allcells.end(); ++mpIterator)
    {
        int typeFind = rand()%5;
        
        if(typeFind == 0)
        {
            typeFind = 1;
        }
        if (typeFind == 3) {
            typeFind=4;
        }
        
        mpIterator->second->setType((CELL_TYPE)typeFind);
    }
}



void GameScene::backOneStep(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{

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
        mCell->setParameters(Color3B(0,0,0),unitOriginPosition,Size(munitSize,munitSize),tempCell._posIndex,count);
        mCell->setType((CELL_TYPE) tempCell._mType);
        mCell->setdownShouldGo(0.0f);
        mCell->setleftShouldGo(0.0f);
        this->addChild((Node*) mCell);
        allcells.insert((pair<Vec2, HappyStartCell*> (tempCell._posIndex,mCell)));
    }
}



void GameScene::exitGame(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type)
{
    auto scene = MainMenuScene::createScene();
    // run
    Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    
}

void GameScene::deleteOneCell(cocos2d::Ref* object)
{
    if (_mMode == CELL_TOUCH_MODE::NORMAL_MODE) {
        _mMode = CELL_TOUCH_MODE::DELETE_ONE_MODE;
    }else{
        _mMode = CELL_TOUCH_MODE::NORMAL_MODE;
    }
}



void GameScene::update(float delta)
{
    
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
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
                
                switch (_mMode) {
                    case CELL_TOUCH_MODE::NORMAL_MODE:{
                        
                        cellsGet.clear();
                        
                        list<HappyStartCell*> templist = findTheGroupToRemove(mIt->second);
                        
                        
                       
                        
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
                                       CCLOG("GameScene::onTouchBegan stash HERE-------1--------");
                                       
                                       if(!(mmcell->gethasFind()))
                                       {
                                           CCLOG("GameScene::onTouchBegan stash HERE-------2-------- %d",i);
                                           
                                           templist.push_back(mmcell);
                                           mmcell->sethasFind(true);
                                       }
                                   
                                   }
                               
                            }
                            
//                            templist.push_back(mIt->second);

                        }
                        
                        if (!(mIt->second->getType() == CELL_TYPE::TYPE_7COLORS) ) {
                            
                        
                        if(templist.size() >= 5)
                        {
                            for(HappyStartCell* temp :templist)
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
                        
                        
                        
                        for(HappyStartCell* temp :templist)
                        {
                            cocos2d::ParticleSystem* ps = cocos2d::ParticleExplosion::create();
                            ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("whitedot.png"));
                            
                            ps->setPosition( unitOriginPosition +  temp->getposIndex()  * (1 + munitSize) + Vec2(munitSize/2,munitSize/2) );
//                            ps->setStartColor(Color4F(220,177.f,0.f,1.f));
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
                                allcells.erase(mpIterator);
                            }
                            removeChild((Node*)temp);
                            
                           
                            
                            
                            
//                            Vec2 targetPos = unitOriginPosition +  mIt->second->getposIndex()  * (1 + munitSize) + Vec2(munitSize/2,munitSize/2);
//                           
//
//                            
//                            auto lamdaMove = [=](Ref* pSender){
//                                if(mpIterator != allcells.end())
//                                {
//                                    allcells.erase(mpIterator);
//                                }
//                                removeChild((Node*)temp);
//                                
//                            };
//                            auto callmove = CallFuncN::create(lamdaMove);
//                            
//                            ((Node*)temp)->runAction(Sequence::create(MoveTo::create(1.0f, targetPos),callmove, NULL)  );
//                            
                            
                            
                            
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
                        
                        
                        break;
                    
                    }
                        
                    case CELL_TOUCH_MODE::DELETE_ONE_MODE:
                    {
                        
                        Vec2 targetPos = unitOriginPosition +  mIt->second->getposIndex()  * (1 + munitSize) + Vec2(munitSize/2,munitSize/2);
                        
                        cocos2d::ParticleSystem* ps = cocos2d::ParticleMeteor::create();
                        ps->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("startpart.png"));
                        
                        ps->setPosition( onShotButtonPos );
                        Vec2 mVec = (targetPos-onShotButtonPos);
                        ps->setStartColor(Color4F(mIt->second->getColor()));
                        
                        
                        
                        ps->setGravity(Vec2::ZERO);
                        ps->setTotalParticles(50);
//                        ps->setRotatePerSecondVar(0);
                        ps->setScale(0.6);
                        ps->setLife(1.0);
                        ps->setStartSpin(2);
                        ps->setRotation(mVec.getAngle() - 135.f);
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
                        };
                        
                        
                        auto callNextAnimation = cocos2d::CallFuncN::create(removeLamda);
                        
                        actionPlaying = true;
                        
                        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0f),callNextAnimation,NULL));
                        
                        
                        break;
                        
                }
                    default:
                        break;
                }
                
                
                
                
                
            }
            
            
        
        }
        
    }
    const char * scoretext = String::createWithFormat("SCORE : %lu" , count*count - allcells.size())->getCString();
    scoreLAbel->setString(scoretext);
    
    
    return true;
}


void GameScene::deleteTargetCell(map<Vec2,HappyStartCell*>::iterator   targetCell)
{
    
    allcells.erase(targetCell);
    removeChild((Node*)targetCell->second);

}

bool firstTimeRun = true;

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
        targetCell->sethasFind(true);
        
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

