//
//  CommonUtils
//  HeroTalk
//
//  Created by chenwei on 15/4/27.
//

#include "CommonUtils.h"

#include "StringComparator.h"


void CommonUtils::showToastOnIOS(Node& pdelegate,const char* message)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

    Label* text = Label::create();
    text->setString(message);
    text->setSystemFontSize(30);
    text->setAnchorPoint(Vec2(0.5,0.5));

    LayerColor* lc =LayerColor::create(Color4B(50,50,50,10));
    lc->setAnchorPoint(Vec2(0.5,0.5));
    lc->setContentSize(cocos2d::Size(text->getContentSize().width+20,text->getContentSize().height+20));
    lc->setPosition(Vec2(visibleSize.width/2 - lc->getContentSize().width/2,visibleSize.height/6));

    text->setPosition(lc->getContentSize()/2);
    pdelegate.addChild(lc);
    lc->addChild(text);

    lc->runAction(FadeIn::create(1));

    auto lamdafade = [=](Node* pSender){
        lc->runAction(FadeOut::create(1));
    };

    auto lamdaRemove = [=](Ref* pSender){
        lc->removeFromParent();
    };
    lc->setZOrder(1000);
    pdelegate.runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(lamdafade),CallFuncN::create(lamdaRemove), NULL));

}

/**
 * int数値変換字符串。
 */
std::string CommonUtils::IntToString(int number)
{
    
    std::stringstream ss;
    ss << number;
    return ss.str();
}

/**
 * float数値変換字符串。
 */
std::string CommonUtils::FloatToString( float number )
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}


/**
 * 数値変換字符串。
 */
const char* CommonUtils::IntToCString(int number)
{
    return IntToString( number ).c_str();
}

/**
 * 文字列转int
 */
int CommonUtils::StrToInt( std::string str )
{
    int ret = 0;
    
    std::stringstream ss;
    ss << str;
    ss >> ret;
    
    return ret;
    
}

/**
 * 文字列変換int。
 */
int CommonUtils::StrToInt( char* str )
{
    int ret = 0;
    
    std::stringstream ss;
    ss << std::string( str );
    ss >> ret;
    
    return ret;
}

/**
 * 文字列から数値への変換。
 */
float CommonUtils::StrToFloat( std::string str )
{
    float ret = 0;
    
    std::stringstream ss;
    ss << str;
    ss >> ret;
    
    return ret;
    
}

/**
 * 文字列変換float。
 */
float CommonUtils::StrToFloat( char* str )
{
    float ret = 0;
    
    std::stringstream ss;
    ss << std::string( str );
    ss >> ret;
    
    return ret;
}

/**
 * 文字列转long
 */
long CommonUtils::StrToLong( std::string str )
{
    long ret = 0;
    
    std::stringstream ss;
    ss << std::string( str );
    ss >> ret;
    
    return ret;
}


/**
 * 文字列結合。
 */
std::string CommonUtils::appendStr( std::string str1, const char* str2 )
{
    std::string copystr = std::string( str1 );
    
    copystr.append( str2 );
    
    return copystr;
}

/**
 * 文字列結合。
 */
std::string CommonUtils::appendStr( const char* str1, const char* str2 )
{
    return appendStr( std::string( str1 ), str2 );
}

/**
 * 文字列分割。
 */
std::vector<std::string> CommonUtils::split( std::string str, const char* delim)
{
    std::vector<std::string> result;
    size_t current = 0, found;
    
    while( ( found = str.find_first_of( delim, current ) ) != std::string::npos )
    {
        result.push_back( std::string( str, current, found - current ) );
        current = found + 1;
    }
    
    result.push_back( std::string( str, current, str.size() - current ) );
    
    return result;
}
/**
 * 获取本地可写路径
 */
std::string CommonUtils::getLocalPath()
{
    return FileUtils::getInstance()->getWritablePath();
}

/**
 * 可写文件路径
 */
std::string CommonUtils::getLocalPath( std::string filename )
{
    std::string str = appendStr( getLocalPath(), "/" );
    
    str.append( filename );
    
    return str;
}


/*
 *范围随机整数
 */
int CommonUtils::getRandom( int min, int max )
{
    
    if( min > max )
    {
        int value = min;
        
        min = max;
        max = value;
    }
    
    int rand = 0;
    
    for( int i = 0; i < 10; i++ )
    {
        rand = arc4random() % ( ( max - min ) + 1 );
        
        
        //arc4random_uniform(1);
    }
    
    return rand + min;
}


/*
 * 范围随机浮点数
 */
float CommonUtils::getRandomForFloat( float min, float max )
{
    if( min > max )
    {
        float value = min;
        
        min = max;
        max = value;
    }
    
    int rand = 0;
    
    int min1 = (int)( min * 1000 );
    int max1 = (int)( max * 1000 );
    
    for( int i = 0; i < 10; i++ )
    {
        rand = arc4random() % ( ( max1 - min1 ) + 1 );
    }
    
    rand += min1;
    
    float frand = (float)rand;
    float ret = frand/1000;
    
    return ret;
}

std::string CommonUtils::getProjectVersion()
{
    std::string ver = "";
    
    // AMDROID用
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#else
    
//    NSString * version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
//    
//    ver = "ver." + string( [ version UTF8String ] );
//    
//    ver.substr(0,ver.length()-1);
    
#endif
    
    return ver;
}

/*
 *  字符串替换。
 *  string1 置換対象
 *  string2 検索文字列。
 *  string3 置換文字列。
 */
std::string CommonUtils::strReplace( std::string string1, std::string string2, std::string string3 )
{
    std::string::size_type  pos( string1.find( string2 ) );
    
    while( pos != std::string::npos )
    {
        string1.replace( pos, string2.length(), string3 );
        pos = string1.find( string2, pos + string3.length() );
    }
    
    return string1;
}


/*
 * 文字列幅の取得。
 */
float CommonUtils::getStringWidth( std::string str, int fontsize )
{
    
    float width = 0;
    
    std::vector< string > sList = StringComparator::comparate( str );
    
    for( int i = 0; i < sList.size(); i++ )
    {
        float font_width = fontsize;
        
        // 半角
        if( sList[i].length() == 1 )
        {
            width += ((font_width/3)*2);
        }
        // 全角
        else
        {
            width += font_width;
        }
    }
    
    return width;
}

const char* CommonUtils::getFileExtension( const char* path )
{
    char *p;
    char *tail;
    
    tail = strchr( path, '\0' );
    
    for( p = tail; p >= path; p-- )
    {
        if ( '.' == *p ) return p+1;
    }
    
    return tail;
}

string CommonUtils::getFileName( const char* path )
{
    string str1( path );
    
    int pos1 = str1.find_last_of("/");
    pos1 = pos1 + 1;
    
    int pos2 = str1.find_first_of(".");

    string str2 = str1.substr( pos1, pos2 );
    
    return str2;
}

/*
 * ファイル名の取得。
 * path ファイルパス。
 */
string CommonUtils::getFileName( string path )
{
    return getFileName( path.c_str() );
}

//禁词
vector<string> * CommonUtils::getCsvBanWordForResource( string path )
{
    vector<string> * csvlist = new vector<string>;
    
    
    CCFileData *data = new CCFileData( path.c_str(), "r");
    
    std::string* pLoadedString = new std::string((const char *)data->getBuffer(), data->getSize() );
    
    delete data;
    
    int spos = 0;
    
    for( ;; )
    {
        // 改行コードで分割
        int kidx = pLoadedString->find_first_of("\n", spos );
        
        if( kidx == string::npos )
            break;
        
        string linestr = pLoadedString->substr( spos, kidx - spos );
        spos += ( kidx - spos );
        spos++;
        
        csvlist->push_back(linestr);
    }
    
    delete pLoadedString;
    
    return csvlist;
}


//device ID
//+ (NSString*)getUserDeviceID
//{
//    //先师按照玩家的登录时间获取 如果可以获取到玩家的各种设备信息 那么就按照设备信息
//    NSDate *datenow = [NSDate date];
//    NSString *userId = [NSString stringWithFormat:@"Kuma%li", (long)[datenow timeIntervalSince1970]];
//    
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
//    SEL selector = NSSelectorFromString(@"uniqueIdentifier");
//    if ([[UIDevice currentDevice] respondsToSelector:selector]) {
//        return [[UIDevice currentDevice] performSelector:selector];
//    }
//    
//    SEL selector2 = NSSelectorFromString(@"identifierForVendor");
//    if ([[UIDevice currentDevice] respondsToSelector:selector2]) {
//        return [[UIDevice currentDevice] performSelector:selector];
//    }
//    
//    SEL selector3 = NSSelectorFromString(@"macaddress");
//    if ([[UIDevice currentDevice] respondsToSelector:selector3]) {
//        NSString *macaddress = [[UIDevice currentDevice] performSelector:selector];
//        NSString *bundleIdentifier = [[NSBundle mainBundle]bundleIdentifier];
//        NSString *stringToHash = [NSString stringWithFormat:@"%@%@",macaddress,bundleIdentifier];
//        return stringToHash;
//    }
//#pragma clang diagnostic pop
//    
//    return userId;
//}