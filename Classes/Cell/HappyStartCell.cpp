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

    _cellsToFindSlantLeft.push_back((posIndex.x-1) + (posIndex.y+1) * count);
    _cellsToFindSlantLeft.push_back((posIndex.x+1) + (posIndex.y-1) * count);
    
    
    _cellsToFindSlantright.push_back((posIndex.x-1) + (posIndex.y-1) * count);
    _cellsToFindSlantright.push_back((posIndex.x+1) + (posIndex.y+1) * count);
    
    
   
    this->initWithFile("ball.png");
    
    this->setScale(unitSize.width/this->getContentSize().width, unitSize.height /this->getContentSize().height);
    this->setPosition(originPos.x + posIndex.x * (unitSize.width +1) + unitSize.width/2 , originPos.y + unitSize.height*count );
    float velocity = Director::getInstance()->getVisibleSize().height / 0.5;
    Vec2 posDes = Vec2(originPos.x + posIndex.x * (unitSize.width +1) + unitSize.width/2 , originPos.y + posIndex.y *  (unitSize.height+1)+ unitSize.width/2 );
    this->runAction(MoveTo::create((getPositionY() - posDes.y )/velocity, posDes));
    
    
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
     _isLocked =false;
    _mType = TYPE_NORMAL;
    _posIndex = Vec2(0, 0);
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
//        this->runAction(cocos2d::RotateBy::create(1.0f, -144.f));
        
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
    
    if (_isLocked) {
        return;
    }
    
    switch (mtype) {
        case TYPE_NORMAL:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(200,200,200));
            break;
        case TYPE_BLOCK:
        {
            this->setTexture("ball.png");
            Sprite* round7 = Sprite::create("boom.png");
            this->addChild(round7);
            round7->setAnchorPoint(Vec2(0.5,0.5));
            round7->setPosition(this->getContentSize()/2);
            round7->setTag(999);
            round7->setGlobalZOrder(this->getGlobalZOrder());
            float scalenow = round7->getScale();
            this->setRotation3D(Vec3(0,90,0));
            round7->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,2.2*scalenow),ScaleTo::create(0.5,1.8*scalenow), NULL)  ));
            this->setColor(cocos2d::Color3B(225,118,0));
            this->runAction(RotateTo::create(1.0, Vec3(0,0,0)));
            break;
        }
        case TYPE_7COLORS:
        {
            this->setTexture("ball.png");
            Sprite* round7 = Sprite::create("ten.png");
            this->addChild(round7);
            round7->setAnchorPoint(Vec2(0.5,0.5));
            round7->setPosition(this->getContentSize()/2);
            round7->setTag(999);
            round7->setGlobalZOrder(this->getGlobalZOrder());
            //        round7->runAction(RepeatForever::create(RotateBy::create(5, 360)));
            float scalenow = round7->getScale();
            this->setRotation3D(Vec3(0,90,0));
            round7->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,2.2*scalenow),ScaleTo::create(0.5,1.8*scalenow), NULL)  ));
            this->setColor(cocos2d::Color3B(225,118,0));
            this->runAction(RotateTo::create(1.0, Vec3(0,0,0)));
             break;
        }
        case TYPE_BLUE:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(0,155,245));
            break;
        case TYPE_GREEN:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(28, 241,183));
            break;
        case TYPE_GRAY:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(246, 236,12));
            break;
        case TYPE_PINK:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(224, 28 ,155));
            break;
        case TYPE_PURPLE:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(100, 0 ,221));
            break;
        case TYPE_RED:
            if(this->getChildByTag(999))
            {
                this->removeChildByTag(999);
            }
            this->setColor(cocos2d::Color3B(249, 62, 79));
            break;
        default:
            break;
    }
    
    
    
//    this->setRotation(45*mtype);
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

