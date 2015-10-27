#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

#include "RTParser.h"
#include "DbgHelper.h"
#include "RTChatList.h"
#include "RTEmojiSprite.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

class HelloWorld : public LayerColor
{
    
public:
    
    HelloWorld();
    ~HelloWorld();
    
    // implement the "static create()" method manually
    static HelloWorld* create();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    // Barrage
    
    class ChatMsg {
    public:
        RTChatNode::OptType opt = RTChatNode::OptType::NONE;
        std::string json = "";
        std::string title = "";
        RTChatNode::OwnType own = RTChatNode::OwnType::OTHER;
        
        ChatMsg(RTChatNode::OptType _opt,
                std::string _json,
                std::string _title,
                RTChatNode::OwnType _own) :
        opt(_opt),
        json(_json),
        title(_title),
        own(_own)
        {};
        
        ~ChatMsg() {};
    };

    void pushBarrage(ChatMsg msg);
    void showBarrage();
    std::map<int, bool> _barrageLines;
    std::vector<ChatMsg> _barrageMsgList;
    
};

#endif // __HELLOWORLD_SCENE_H__
