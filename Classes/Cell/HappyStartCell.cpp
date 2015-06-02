//
//  HappyStartCell.cpp
//  HappyStart
//
//  Created by chenwei on 15/3/9.
//
//
#include "cocostudio/CocoStudio.h"
#include "HappyStartCell.h"

using namespace cocostudio::timeline;

HappyStartCell::HappyStartCell()
{
    
}

void HappyStartCell::setParameters(cocos2d::Color3B mcolor,cocos2d::Vec2 originPos,cocos2d::Size unitSize,cocos2d::Vec2 posIndex,int count){

    _downShouldGo = 0.0f;
    _leftShouldGo = 0.0f;
    _timeToDelay = 0.0f;
    this->sethasFind(false);
    this->setsameToTarget(false);
    this->setposIndex(posIndex);
    this->setunitSize(unitSize);
    this->setoriginPos(originPos);
    _cellsToFind.push_back( (posIndex.x-1) + posIndex.y * count);
    _cellsToFind.push_back ( (posIndex.x+1) + posIndex.y * count);
    _cellsToFind.push_back( posIndex.x +  (posIndex.y+1)* count);
    _cellsToFind.push_back( posIndex.x + (posIndex.y-1) * count);

    this->initWithFile("ball.png");
    
    this->setScale(unitSize.width/this->getContentSize().width, unitSize.height /this->getContentSize().height);
    this->setPosition(originPos.x + posIndex.x * (unitSize.width +1) + unitSize.width/2 , originPos.y + posIndex.y *  (unitSize.height+1)+ unitSize.width/2 );
}
void HappyStartCell::setActualColor(cocos2d::Color3B mcolor){
    this->setColor(mcolor);
}

cocos2d::Color3B  HappyStartCell::getActualColor(){
    return this->getColor();
}


HappyStartCell::~HappyStartCell()
{

}

bool HappyStartCell::init()
{
    
    if(!Sprite::init())
    {
        return false;
    }
    this->scheduleUpdate();
    this->initShaders();
    return true;

}
 
void HappyStartCell::goLeftAnimationRun()
{
    
    if(_leftShouldGo > 0)
    {
           this->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(0.4f,cocos2d::Vec2(-1*_leftShouldGo  * _unitSize.width,0.0f)),NULL));
        
        
        _leftShouldGo = 0;
    }

}

void HappyStartCell::update(float delta)
{
    
    //只要计算出改cell下方几只丢失 就可以计算出改cell要下落的高度。
    if(_downShouldGo > 0 )
    {
    
        auto callNextAnimation = cocos2d::CallFunc::create(this, callfunc_selector(HappyStartCell::goLeftAnimationRun));
        
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(_timeToDelay),cocos2d::MoveBy::create(0.4f,cocos2d::Vec2(0.0f,-5 -1*_downShouldGo * (_unitSize.width + 1))),cocos2d::MoveBy::create(0.05f,cocos2d::Vec2(0.0f,5.0f)),callNextAnimation,NULL));
        _downShouldGo = 0;
    }
    
    if(_leftShouldGo > 0)
    {
        this->runAction(cocos2d::RotateBy::create(1.0f, -144.f));
        
        this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(_timeToDelay+0.4),cocos2d::MoveBy::create(0.4f,cocos2d::Vec2(-1*_leftShouldGo  * _unitSize.width,0.0f)),NULL));
        
        _leftShouldGo = 0;
    }
    
}


enum CELL_TYPE HappyStartCell::getType()
{
    return _mType;
}

void HappyStartCell::setType(enum CELL_TYPE mtype)
{
    _mType = mtype;
//    string filename = cocos2d::StringUtils::format("cell_%d.jpg",mtype);
//    this->setTexture(filename);
//    this->setColor(cocos2d::Color3B(120,200,200));
    
    
    if (mtype==TYPE_7COLORS) {
         this->setTexture("ball.png");
        Sprite* round7 = Sprite::create("bold7.png");
        this->addChild(round7);
        round7->setAnchorPoint(Vec2(0.5,0.5));
        round7->setPosition(this->getContentSize()/2);
        round7->setTag(999);
        round7->runAction(RepeatForever::create(RotateBy::create(5, 360)));
        
        this->setColor(cocos2d::Color3B(120, 255,120));
    }else{
        
        if(this->getChildByTag(999))
        {
            this->removeChildByTag(999);
        }
        
        if(mtype ==TYPE_RED )
        {
            this->setColor(cocos2d::Color3B(249, 62, 79));
        }else if(mtype == TYPE_GREEN)
        {
            this->setColor(cocos2d::Color3B(28, 241,183));
        }else if(mtype == TYPE_BLUE)
        {
            this->setColor(cocos2d::Color3B(0,155,245));
        }
        
//        this->setColor(cocos2d::Color3B(255 *( mtype & 4 ), 255 *( mtype & 2 ),255 *( mtype & 1 )));
    }
    
    this->setRotation(45*mtype);
}

void HappyStartCell::initShaders()
{
//    auto fileUtiles = FileUtils::getInstance();
//    auto fragmentFullPath = fileUtiles->fullPathForFilename("Shaders/night.fsh");
//    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
//    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
//    _glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
//    
//    
//    _glprogramstate->setUniformFloat("nightDegree",1.0);
//    
//    this->setGLProgramState(_glprogramstate);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
//                                                      [this](EventCustom*)
//                                                      {
//                                                          glprogram->reset();
//                                                          glprogram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
//                                                          glprogram->link();
//                                                          glprogram->updateUniforms();
//                                                      }
//                                                      );
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
//#endif
    
    
}

void HappyStartCell::changeTypeRandom()
{
     this->setType((CELL_TYPE)random(1, 4));
}

