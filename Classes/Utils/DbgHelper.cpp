//
//  DbgHelper.cpp
//  MyCppGame
//
//  Created by HanShaokun on 22/8/15.
//
//

#include "DbgHelper.h"

int DbgHelper::_debugLevel = -1;

void DbgHelper::setDebugLevel(int level)
{
    _debugLevel = level;
}

void DbgHelper::colorRect(cocos2d::Node *parent,
                          const Point &center,
                          const cocos2d::Size &size,
                          const cocos2d::Color4F &color,
                          int level,
                          int zOrder)
{
    if (_debugLevel <= level) {
        auto bg = DrawNode::create();
        bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
        bg->setContentSize(size);
        bg->drawSolidRect(Point::ZERO, size, color);
        bg->setPosition(center);
        parent->addChild(bg, zOrder);
    }
}

void DbgHelper::colorRect(cocos2d::Node *parent,
                          const cocos2d::Rect &r,
                          const cocos2d::Color4F &color,
                          int level,
                          int zOrder)
{
    if (_debugLevel <= level) {
        auto bg = DrawNode::create();
        bg->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        bg->setContentSize(r.size);
        bg->drawSolidRect(Point::ZERO, r.size, color);
        bg->setPosition(r.origin);
        parent->addChild(bg, zOrder);
    }
}