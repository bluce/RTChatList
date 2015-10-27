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
        CLEAR,          //透明无背景
        BORDERED,       //左右箭头背景
        POP             //向下箭头背景
    } FormType;
    
    typedef enum : int {
        SELF,           //自己发送的话
        OTHER           //收到其他人发送的话
    } OwnType;
    
    //1 同桌,2 同一种房间, 如新手房,3 喇叭 4 系统公告
    typedef enum : int {
        NONE = 0,       //未知类型，错误
        DESK = 1,       //同桌发送
        ROOM = 2,       //房间发送
        ALL = 3,        //全员发送，喇叭
        SYSTEM = 4      //系统信息
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
    
#if COCOS2D_DEBUG
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
#endif
    
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
