//
//  RTEmojiSprite.cpp
//  MyCppGame
//
//  Created by darklinden on 8/23/15.
//
//

#include "RTEmojiSprite.h"

using namespace cocos2d;

RTEmojiSprite* RTEmojiSprite::create(int emojiId, cocos2d::Size size)
{
    auto s = new (std::nothrow) RTEmojiSprite();
    if (s && s->init(emojiId, size)) {
        s->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(s);
    }
    return s;
}

bool RTEmojiSprite::init(int emojiId, cocos2d::Size size)
{
    _emojiId = emojiId;
    _size = size;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/emoji.plist");
    
    auto spf = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("emoji_%d_%02d.png", _emojiId, 1));
    if (!spf) {
        return false;
    }
    
    if (! this->initWithSpriteFrame(spf) ) {
        return false;
    }
    
    this->setScale(_size.width / this->getContentSize().width,
                   _size.height / this->getContentSize().height);
    
    return true;
}

void RTEmojiSprite::onEnter()
{
    Sprite::onEnter();
    
    
    Vector<cocos2d::SpriteFrame *> frames;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/emoji.plist");
    
    for (int i = 1; i < 30; i++) {
        auto spf = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("emoji_%d_%02d.png", _emojiId, i));
        if (!spf) {
            break;
        }
        frames.pushBack(spf);
    }
    
    if (!frames.size()) {
        return;
    }
    
    auto anim = Animation::createWithSpriteFrames(frames);
    anim->setDelayPerUnit(0.2f);
    anim->setRestoreOriginalFrame(true);
    auto a = Animate::create(anim);
    this->runAction(RepeatForever::create(a));
    
}

void RTEmojiSprite::onExit()
{
    Sprite::onExit();
    
    this->stopAllActions();
    this->unscheduleAllCallbacks();
}