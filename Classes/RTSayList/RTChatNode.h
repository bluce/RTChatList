//
//  RTChatNode.h
//  MyCppGame
//
//  Created by HanShaokun on 21/8/15.
//
//

#ifndef __MyCppGame__RTChatNode__
#define __MyCppGame__RTChatNode__

#include "cocos2d.h"

class RTChatNode : public cocos2d::Ref
{
   
public:
    
    typedef enum : int {
        SELF,   //自己发送的话
        OTHER   //收到其他人发送的话
    } OwnType;
    
    typedef enum : int {
        NONE,   //未知类型，错误
        ALL,    //全员发送，喇叭
        ROOM,   //房间发送
        DESK,   //同桌发送
        SYSTEM  //系统信息
    } OptType;
    
    RTChatNode();
    ~RTChatNode();
    
    static RTChatNode* create(OwnType own, OptType opt, const std::string &title, const std::string &content);
    
    virtual bool init(OwnType own, OptType opt, const std::string & title, const std::string & content);
    
    cocos2d::Node* getNode();
    
    const cocos2d::Size getContentSize() const { return _contentSize; }
    const OwnType getOwnType() const { return _own; }
    const OptType getOptType() const { return _opt; }
    const std::string getTitle() const { return _title; }
    const std::string getContent() const { return _content; }
    
private:
    
    cocos2d::Size _contentSize = cocos2d::Size::ZERO;
    OwnType _own;
    OptType _opt;
    std::string _title;
    std::string _content;
    
};

#endif /* defined(__MyCppGame__RTChatNode__) */
