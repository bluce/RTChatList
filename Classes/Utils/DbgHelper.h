//
//  DbgHelper.h
//  MyCppGame
//
//  Created by HanShaokun on 22/8/15.
//
//

#ifndef __MyCppGame__DbgHelper__
#define __MyCppGame__DbgHelper__

#include "cocos2d.h"

using namespace cocos2d;

class DbgHelper {
    
public:
    
    static void setDebugLevel(int level);
    static void colorRect(Node* parent,
                          const Point& center,
                          const Size& size,
                          const Color4F& color,
                          int level = 0,
                          int zOrder = 0);
    static void colorRect(Node* parent,
                          const Rect& r,
                          const Color4F& color,
                          int level = 0,
                          int zOrder = 0);
    
private:
    
    static int _debugLevel;
    
};

#endif /* defined(__MyCppGame__DbgHelper__) */
