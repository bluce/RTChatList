#include "HelloWorldScene.h"



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
    if ( !LayerColor::initWithColor(Color4B::GRAY) )
    {
        return false;
    }
    
    DbgHelper::setDebugLevel(-1);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Point center = Point(visibleSize.width / 2.0, visibleSize.height / 2.0);
    Size size = Size(460, 460);
    std::string json = FileUtils::getInstance()->getStringFromFile("chat/test2.json");
    
    //test animate
#if 0
    Size s = Size(80, 80);
    Vector<cocos2d::SpriteFrame *> frames;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chat/emoji.plist");
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
    
    auto b = Button::create("chat/desk-pop-bg.png");
    b->setPosition(Point(b->getContentSize()));
    this->addChild(b);
    b->addTouchEventListener([=] (Ref* r, Widget::TouchEventType t) {
        switch (t) {
            case Widget::TouchEventType::ENDED:
            {
                ChatMsg cmsg = ChatMsg(RTChatNode::OptType::DESK,
                                       json,
                                       "[Hello World]",
                                       RTChatNode::OwnType::SELF);
                this->pushBarrage(cmsg);
            }
                break;
            default:
                break;
        }
    });
    
    return true;
}

#pragma mark -



void HelloWorld::pushBarrage(ChatMsg msg)
{
    _barrageMsgList.push_back(msg);
    showBarrage();
}

void HelloWorld::showBarrage()
{
    const int lineCount = 10;
    int pos = -1;
    
    if (_barrageMsgList.size() <= 0) {
        return;
    }
    
    //init
    if (_barrageLines.size() < lineCount) {
        for (int i = 0; i < lineCount; i++) {
            _barrageLines.insert(std::pair<int, bool>(i, false));
        }
    }
    
    //get idle pos
    for (int i = 0; i < _barrageLines.size(); i++) {
        if (false == _barrageLines[i]) {
            _barrageLines.erase(i);
            _barrageLines.insert(std::pair<int, bool>(i, true));
            pos = i;
            break;
        }
    }
    
    if (-1 != pos)
    {
        //get first msg
        auto itmsg = _barrageMsgList.begin();
        auto msg = *itmsg;
        _barrageMsgList.erase(itmsg);
        
        srand((unsigned)time(0));
        int r = rand() % 40;
        int randompos = 500 + r;
        
        float top = randompos - (pos * 40);
        auto txt = RTContent::create();
        this->addChild(txt, 100);
        txt->ignoreContentAdaptWithSize(true);
        
        switch (msg.opt) {
            case RTChatNode::OptType::ALL:
            {
                txt->pushBackElement(RTElementText::create(0, Color3B::BLACK, 255, StringUtils::format("%s", msg.title.c_str()).c_str(), "Arial", 30));
            }
                break;
            case RTChatNode::OptType::SYSTEM:
            {
                txt->pushBackElement(RTElementText::create(0, Color3B::BLACK, 255, StringUtils::format("%s", msg.title.c_str()).c_str(), "Arial", 30));
            }
                break;
            default:
                break;
        }
        
        RTParser::pushElements(txt, msg.json, RTParser::DesType::Barrage, "", Color3B::WHITE, 30, Size(44, 44), 255);
        txt->formatText();
        
        Size visibleSize = Director::getInstance()->getWinSize();
        Size s = txt->getContentSize();
        txt->setPosition(Point(visibleSize.width + (s.width / 2.0), top));
        
        auto actionTo = MoveTo::create(10, Point(-(s.width), top));
        auto actend = CallFunc::create([=]() {
            txt->removeFromParent();
        });
        auto actseq = Sequence::create(actionTo, actend, NULL);
        
        DelayTime *de = DelayTime::create(5);
        auto cal= CallFunc::create([=]() {
            this->_barrageLines.erase(pos);
            this->_barrageLines.insert(std::pair<int, bool>(pos, false));
            this->showBarrage();
        });
        auto deseq = Sequence::create(de, cal, NULL);
        
        auto spa = Spawn::create(actseq, deseq, NULL);
        txt->runAction(spa);
    }
    else {
        if (_barrageMsgList.size() > (lineCount * 2)) {
            for (int i = 0; i < _barrageLines.size(); i++) {
                _barrageLines.erase(i);
                _barrageLines.insert(std::pair<int, bool>(i, false));
            }
            this->showBarrage();
        }
    }
}
