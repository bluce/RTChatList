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

class RTChatNode : public cocos2d::Node
{
   
public:
    
    typedef enum : int {
        CLEAR,   //自己发送的话
        BORDERED   //收到其他人发送的话
    } FormType;
    
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
    
    static RTChatNode* create(FormType form,
                              OwnType own,
                              OptType opt,
                              const std::string& title,
                              const std::string& content,
                              float width = -1,
                              float widthOffset = -1);
    
    virtual bool init(FormType form,
                      OwnType own,
                      OptType opt,
                      const std::string& title,
                      const std::string& content,
                      float width = -1,
                      float widthOffset = -1);
    
    const FormType getFormType() const { return _form; }
    const OwnType getOwnType() const { return _own; }
    const OptType getOptType() const { return _opt; }
    const std::string getTitle() const { return _title; }
    const std::string getContent() const { return _content; }
    const float getWidth() const { return _width; }
    const float getWidthOffset() const { return _widthOffset; }
    
private:
    
    FormType _form;
    OwnType _own;
    OptType _opt;
    std::string _title;
    std::string _content;
    float _width;
    float _widthOffset;
};

#endif /* defined(__MyCppGame__RTChatNode__) */
