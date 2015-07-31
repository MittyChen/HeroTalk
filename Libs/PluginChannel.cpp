//
//  PluginChannel.m
//  sample
//
//  Created by cocos2dx on 14-11-10.
//  Copyright (c) 2014年 cocos2dx. All rights reserved.
//



#include "PluginChannel.h"
#include <stdlib.h>
#include "Analytics.h"
//#include "Push.h"

// #include "PartViews.h"

using namespace anysdk::framework;

#define  LOG_TAG    "PluginChannel"

//UAPI login check

const char * uapi_key = "C2F4D917-BF63-101D-734E-820E09683468";
const char * uapi_secret="5dff4ecdd5cfe07929b4cab63d47bbc2";



void CCMessageBox(const char* content, const char* title)
{
}

void CCExit()
{
    ////
    exit(0);
}

PluginChannel* PluginChannel::_pInstance = NULL;
static AllProductsInfo _myProducts;

PluginChannel::PluginChannel()
{
    _pluginsIAPMap = NULL;
    
}

PluginChannel::~PluginChannel()
{
    unloadPlugins();
}

PluginChannel* PluginChannel::getInstance()
{
    if (_pInstance == NULL) {
        _pInstance = new PluginChannel();
    }
    return _pInstance;
}

void PluginChannel::purge()
{
    if (_pInstance)
    {
        delete _pInstance;
        _pInstance = NULL;
    }
}

//使用anysdk.com的时候注释掉这个宏就可以
//#define isQudaoInfo 1

void PluginChannel::loadPlugins()
{
    std::string appKey = "64A47E4A-2025-6723-845A-8679ACDD201D";
    std::string appSecret = "9500a46effb7e985ae78ea5be2f248e6";
    std::string privateKey = "C196D882A09C91742293DE8B5B177C33";
    std::string oauthLoginServer = "http://uapi.oauth.cocimg.com/api/anysdkrecord/LoginOauth/";
    
    
    AgentManager::getInstance()->init(appKey,appSecret,privateKey,oauthLoginServer);
    
    //使用框架中代理类进行插件初始化
    AgentManager::getInstance()->loadAllPlugins();
    
    //对用户系统设置监听类
    if(AgentManager::getInstance()->getUserPlugin())
    {
        AgentManager::getInstance()->getUserPlugin()->setActionListener(this);
    }
    
    //对支付系统设置监听类
    printf("Load plugins invoked\n");
    _pluginsIAPMap  = AgentManager::getInstance()->getIAPPlugin();
    std::map<std::string , ProtocolIAP*>::iterator iter;
    for(iter = _pluginsIAPMap->begin(); iter != _pluginsIAPMap->end(); iter++)
    {
        (iter->second)->setResultListener(this);
    }
    Analytics::getInstance()->startSession();
    
    _iapIPhone = getIAPIphone();
}

void PluginChannel::unloadPlugins()
{
    printf("Unload plugins invoked\n");
    AgentManager::getInstance()->unloadAllPlugins();
//    Analytics::getInstance()->logTimedEventEnd("Unload");
}

std::string PluginChannel::getPluginId()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        return AgentManager::getInstance()->getUserPlugin()->getPluginId();
    }
    return "";
}

void PluginChannel::login()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        AgentManager::getInstance()->getUserPlugin()->login();
    }
}


void PluginChannel::logout()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("logout"))
        {
            
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("logout",NULL);
        }
    }
}

bool PluginChannel::isFunctionSupported(std::string strClassName)
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        return AgentManager::getInstance()->getUserPlugin()->isFunctionSupported(strClassName);
        
    }
    return false;
}

const char* PluginChannel::getResourceId(std::string name)
{
    return "";
}

void PluginChannel::ChoosePayMode(std::vector<std::string>& pluginId)
{
}

void PluginChannel::payMode(std::string id)
{
    std::map<std::string , ProtocolIAP*>::iterator iter;
    iter = _pluginsIAPMap->find(id);
    
    if(iter != _pluginsIAPMap->end())
    {
        (iter->second)->payForProduct(productInfo);
    }
}

ProtocolIAP* PluginChannel::getIAPIphone()
{
    if(_pluginsIAPMap)
    {
        std::map<std::string , ProtocolIAP*>::iterator it = _pluginsIAPMap->begin();
        for (; it != _pluginsIAPMap->end(); it++) {
            printf("it->first: %s----\n", it->first.c_str());
            if (it->first == IAP_IPHONE_ID) {
                return it->second;
            }
        }
    }
    return NULL;
}

void PluginChannel::requestProducts(const char * productId)
{
    printf("payRequest\n");
    if ( NULL!= _iapIPhone ) {
        PluginParam param1(productId);
        _iapIPhone->callFuncWithParam("requestProducts", &param1, NULL);
    }
    else{
        printf("iap iphone isn't find!\n");
    }
}

void PluginChannel::pay()
{
    if ( NULL != _iapIPhone ) {
        _iapIPhone->payForProduct(productInfo);
        return;
    }
    
    std::map<std::string , ProtocolIAP*>::iterator it = _pluginsIAPMap->begin();
    (it->second)->payForProduct(productInfo);
    
    PluginParam param3("size");
    std::vector<PluginParam*> params;
    params.push_back(&param3);
}

void PluginChannel::resetPayState()
{
    ProtocolIAP::resetPayState();
    pay();
}

std::string PluginChannel::getUserId()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        return AgentManager::getInstance()->getUserPlugin()->getUserID();
    }
    return "";
}

void PluginChannel::enterPlatform()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        //使用isFunctionSupported可先判断该插件是否支持该功能
        if(isFunctionSupported("enterPlatform"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("enterPlatform",NULL);
        }
    }
}

void PluginChannel::showToolBar(ToolBarPlace place)
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("showToolBar"))
        {
            PluginParam paramInfo(place);
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("showToolBar",&paramInfo,NULL);
        }
    }
}

void PluginChannel::hideToolBar()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("hideToolBar"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("hideToolBar",NULL);
        }
    }
}

void PluginChannel::pause()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("pause"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("pause",NULL);
        }
    }
}

void PluginChannel::destroy()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("destroy"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("destroy",NULL);
        }
    }
}

void PluginChannel::Exit()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("exit"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("exit",NULL);
        }
        
    }
}

void PluginChannel::antiAddictionQuery()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("antiAddictionQuery"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("antiAddictionQuery",NULL);
        }
        
    }
}

void PluginChannel::accountSwitch()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("accountSwitch"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("accountSwitch",NULL);
        }
        
    }
}

void PluginChannel::realNameRegister()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(isFunctionSupported("realNameRegister"))
        {
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("realNameRegister",NULL);
        }
        
    }
}

void PluginChannel::submitLoginGameRole()
{
    if(AgentManager::getInstance()->getUserPlugin())
    {
        if(PluginChannel::getInstance()->isFunctionSupported("submitLoginGameRole"))
        {
            PluginParam data(userInfo);
            AgentManager::getInstance()->getUserPlugin()->callFuncWithParam("submitLoginGameRole",&data,NULL);
        }
    }
}

void ShowTipDialog()
{
    ////
}

void PluginChannel::onRequestResult(RequestResultCode ret, const char* msg, AllProductsInfo info)
{
    //info: 商品获取到的信息，请在这里进行商品界面的展示。
    printf("get all iap-iphone products info:%lu\n", info.size());
    _myProducts = info;
    productInfo = info["1"];
    pay();
}

void PluginChannel::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
    printf("onPayResult, %s\n", msg);
   
    std::string temp = "fail";
    switch(ret)
    {
        case kPaySuccess://支付成功回调
        {
            temp = "Success";
            char* res;
            sprintf(res,"{\"order\":\"%s\",\"data\":\"%s\"}",msg,info["Product_Name"].c_str());
            
            break;
        }
        case kPayFail://支付失败回调
            
            break;
        case kPayCancel://支付取消回调
           
            break;
        case kPayNetworkError://支付超时回调
            
            break;
        case kPayProductionInforIncomplete://支付超时回调
            
            break;
        case kPayNowPaying:
            ShowTipDialog();
            break;
        default:
            break;
    }
}

void PluginChannel::onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg)
{
    printf("PluginChannel, onActionResult:%d, msg%s\n",code,msg);
    bool _userLogined = false;
    switch(code)
    {
        case kInitSuccess://初始化SDK成功回调
 
            break;
        case kInitFail://初始化SDK失败回调
            CCExit();
            break;
        case kLoginSuccess://登陆成功回调
        {
            _userLogined = true;
           
            char* res;
            sprintf(res,"{\"channel\":\"anysdk\",\"uapi_key\":\"%s\",\"uapi_secret\":\"%s\",\"ext\":\"%s\"}",uapi_key,uapi_secret,msg);
            
            break;
        }
            
        case kLoginNetworkError://登陆失败回调
            
            break;
            
        case kLoginCancel://登陆取消回调
            
            break;
            
        case kLoginFail://登陆失败回调
            
            _userLogined = false;
            break;
        case kLogoutSuccess://登出成功回调
            
            break;
        case kLogoutFail://登出失败回调
            break;
        case kPlatformEnter://平台中心进入回调
            break;
        case kPlatformBack://平台中心退出回调
            break;
        case kPausePage://暂停界面回调
            break;
        case kExitPage://退出游戏回调
            CCExit();
            break;
        case kAntiAddictionQuery://防沉迷查询回调
            break;
        case kRealNameRegister://实名注册回调
            break;
        case kAccountSwitchSuccess://切换账号成功回调
            break;
        case kAccountSwitchFail://切换账号成功回调
            break;
        default:
            break;
    }
}

std::string PluginChannel::getChannelId()
{
    return AgentManager::getInstance()->getChannelId();
}
