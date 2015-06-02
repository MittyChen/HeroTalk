
#ifndef BraveFrontier_StringComparator_h
#define BraveFrontier_StringComparator_h


using namespace std;


template < class Temp >
string toString(Temp t)
{
	stringstream ss;
	ss << t;
	return ss.str();
}


class StringComparator
{

    
public:


	static vector< string > comparate(string inStr)
	{
        
		vector< string > sList;	/* 1文字ずつ分けた結果を保持 */
		int count = 0;			/* 全角文字を読み込み始めてから何回たったか */ 
		char bufC[10];		/* 全角文字保存用 */
		bool isLastFULL = false;	/* 前回が全角かどうか */
        
        // clear list
		sList.clear();
        
        
        for( int i = 0; i < inStr.length(); i++)
		{
            signed char nextChar = reinterpret_cast<signed char&>(inStr.at(i)); //Androidはunsignedとしてもっている
            
			// 今回が半角
			if(nextChar > 0)
			{
				// 前回が全角
				if(isLastFULL)
				{
					bufC[count] = '\0';
					sList.push_back( toString(bufC) );
				}
                
                string addstr = toString(inStr.at(i));
                
				// リストに追加
				sList.push_back( addstr );

				isLastFULL = false;
				count = 0;
			}

			// 今回が全角
			else
			{
				if(count == 3 || ((count > 0) && ((inStr.at(i) | 0x3F) == 0xFF)))
				{
					bufC[count] = '\0';
                    
                    string addstr = toString(bufC);
                    
                    
                    if( addstr.find( "´" ) != string::npos )
                    {
                        CCLog( "ssggg add str=%s", addstr.c_str() );
                    }
                    
                    
					sList.push_back( addstr );
                    //CCLog( " add str=%s", toString(bufC).c_str() );
                    
					count = 0;
				}

				isLastFULL = true;
				bufC[count++] = inStr.at(i);

			}
		}

		// 最後の文字が全角だったときの処理
		if(isLastFULL)
		{
			bufC[count] = '\0';
			sList.push_back( toString(bufC) );
		}
    

		return sList;
        
        
	}
};


#endif
