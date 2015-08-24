//
//  RTEmojiSprite.h
//  MyCppGame
//
//  Created by darklinden on 8/23/15.
//
//

#ifndef __MyCppGame__RTEmojiSprite__
#define __MyCppGame__RTEmojiSprite__

#include "cocos2d.h"

class RTEmojiSprite : public cocos2d::Sprite {
    
public:
    
    static RTEmojiSprite* create(int emojiId, cocos2d::Size size);
    
    virtual bool init(int emojiId, cocos2d::Size size);
    
    virtual void onEnter();
    
    virtual void onExit();
    
private:
    
    int _emojiId;
    cocos2d::Size _size;
    
};

#endif /* defined(__MyCppGame__RTEmojiSprite__) */
