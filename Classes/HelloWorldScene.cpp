#include "HelloWorldScene.h"

#include "RTParser.h"
#include "DbgHelper.h"
#include "RTChatList.h"
#include "RTEmojiSprite.h"

USING_NS_CC;

#define Default_Cell_Offset_Height 10
#define Default_Cell_Offset_Width 15

HelloWorld::HelloWorld()
{
    
}

HelloWorld::~HelloWorld()
{
    
}

HelloWorld* HelloWorld::create()
{
    HelloWorld *pRet = new (std::nothrow) HelloWorld();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B::WHITE) )
    {
        return false;
    }
    
    DbgHelper::setDebugLevel(-1);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Point center = Point(visibleSize.width / 2.0, visibleSize.height / 2.0);
    Size size = Size(460, 460);
    std::string json = FileUtils::getInstance()->getStringFromFile("res/test.json");
    
    //test animate
#if 0
    Size s = Size(80, 80);
    Vector<cocos2d::SpriteFrame *> frames;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/emoji.plist");
    for (int i = 0; i < 30; i++) {
        auto spf = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("emoji_%d_%02d.png", 1, i));
        if (!spf) {
            continue;
        }
        frames.pushBack(spf);
    }
    auto sp = Sprite::createWithSpriteFrame(frames.at(0));
    sp->setScale(s.width / sp->getBoundingBox().size.width, s.height / sp->getBoundingBox().size.height);
    auto anim = Animation::createWithSpriteFrames(frames);
    anim->setDelayPerUnit(0.2f);
    anim->setRestoreOriginalFrame(true);
    auto a = Animate::create(anim);
    sp->runAction(RepeatForever::create(a));
    
    sp->setPosition(Point(visibleSize.width / 2.0, visibleSize.height / 2.0));
    this->addChild(sp);
#endif
    
    //test bordered list
#if 1
    DbgHelper::colorRect(this,
                         Rect(100, 100, 460, 460),
                         Color4F::GRAY,
                         1);
    
    auto list = RTChatList::create(size);
    list->setPosition(Point(100, 100));
    this->addChild(list);
    
    for (int i = 0; i < 20; i++) {
        if (i % 2 == 0) {
            auto sn = RTChatNode::create(RTChatNode::FormType::BORDERED, RTChatNode::OwnType::SELF, RTChatNode::OptType::ALL, "［Hello World!］", json);
            list->pushNode(sn);
        }
        else {
            auto sn = RTChatNode::create(RTChatNode::FormType::BORDERED, RTChatNode::OwnType::OTHER, RTChatNode::OptType::SYSTEM, "［Hello World!］", json);
            list->pushNode(sn);
        }
    }
#endif
    
    //test clear list
#if 0
    DbgHelper::colorRect(this,
                         Rect(100, 100, 460, 460),
                         Color4F::GRAY,
                         1);
    
    auto list = RTChatList::create(size);
    list->setPosition(Point(100, 100));
    this->addChild(list);
    
    for (int i = 0; i < 20; i++) {
        if (i % 2 == 0) {
            auto sn = RTChatNode::create(RTChatNode::FormType::CLEAR, RTChatNode::OwnType::SELF, RTChatNode::OptType::ALL, "［Hello World!］", json, 460, 10);
            list->pushNode(sn);
        }
        else {
            auto sn = RTChatNode::create(RTChatNode::FormType::CLEAR, RTChatNode::OwnType::OTHER, RTChatNode::OptType::SYSTEM, "［Hello World!］", json, 460, 10);
            list->pushNode(sn);
        }
    }
#endif

    //test single node
#if 0
    auto sn = RTChatNode::create(RTChatNode::OwnType::SELF, RTChatNode::OptType::ALL, "［Hello World!］", json);
    auto n = sn->getNode();
    n->setPosition(Point(visibleSize.width / 2.0, visibleSize.height / 2.0));
    this->addChild(n);
#endif
    
    //test emoji sprite
#if 0
    auto s = RTEmojiSprite::create(1, size);
    s->setPosition(Point(visibleSize.width / 2.0, visibleSize.height / 2.0));
    this->addChild(s);
#endif
    
    return true;
}
