//
//  CommonUtils
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//
//

#ifndef __HeroTalk__CommonUtils__
#define __HeroTalk__CommonUtils__

#include "cocos2d.h"
#include <sstream>
#include <string>
USING_NS_CC;

class CommonUtils
{
public:
    
    static void showToastOnIOS(Node& pdelegate,const char* message);
    
    //字符串操作
    
    static std::string IntToString( int number );
    static const char* IntToCString( int number );
    static std::string FloatToString( float number );
    static int StrToInt( std::string str );
    static int StrToInt( char* str );
    static float StrToFloat( std::string str );
    static float StrToFloat( char* str );
    static long StrToLong( std::string str );
    static std::string appendStr( std::string str1, const char* str2 );
    static std::string appendStr( const char* str1, const char* str2 );
    static std::vector<std::string> split( std::string str, const char* delim );
    static std::string strReplace( std::string string1, std::string string2, std::string string3 );
    static float getStringWidth( std::string str, int fontsize );
    
    
    
//文件操作
    static std::string getLocalPath();
    static std::string getLocalPath( std::string filename );
    static const char* getFileExtension( const char* path );
    static std::string getFileName( const char* path );
    static std::string getFileName( std::string path );
    
    
//随机数获取
    static int getRandom( int min, int max );
    static float getRandomForFloat( float min, float max );

//项目版本
    std::string getProjectVersion();
    
//获得禁词
    static std::vector<std::string> * getCsvBanWordForResource( std::string path );
    

};
#endif /* defined(__HeroTalk__CommonUtils__) */
