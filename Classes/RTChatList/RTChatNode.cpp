//
//  RTChatNode.cpp
//  MyCppGame
//
//  Created by HanShaokun on 21/8/15.
//
//

#include "RTChatNode.h"
#include "ui/CocosGUI.h"
#include "RTParser.h"
#include "RTContent.h"

#define RTChatNode_Width_Default      400
#define RTChatNode_WOffset_Default   10

#define Default_Max_Node_Height     600

#define Default_Node_Offset_Height  10

#define Default_Font_Size           32.0
#define Default_Font_Alpha          255
#define Default_Font_Name           "Arial-bold"

#define Default_ALL_Color           Color3B(0x06, 0xff, 0xff)
#define Default_SAY_Color           Color3B(0x00, 0x78, 0xff)

#define Default_Cap_Insets          Rect(30,45,5,5)

typedef enum : int {
    tBg = 1,
    tTitleIcon = 2,
    tTitleLabel = 3,
    tContent = 4
} RTChatNodeTag;

typedef enum : int {
    zBg = 1,
    zTitleIcon = 2,
    zTitleLabel = 3,
    zContent = 4
} RTChatNodeZorder;

RTChatNode::RTChatNode()
{
    
}

RTChatNode::~RTChatNode()
{
    
}

RTChatNode* RTChatNode::create(FormType form,
                               OwnType own,
                               OptType opt,
                               const std::string &title,
                               const std::string &content,
                               float width,
                               float widthOffset)
{
    auto n = new (std::nothrow) RTChatNode();
    if (n && n->init(form, own, opt, title, content, width, widthOffset)) {
        n->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(n);
    }
    return n;
}

bool RTChatNode::init(FormType form,
                      OwnType own,
                      OptType opt,
                      const std::string &title,
                      const std::string &content,
                      float width,
                      float widthOffset)
{
    _form = form;
    _own = own;
    _opt = opt;
    _title = std::string(title);
    _content = std::string(content);
    
    if (width != -1) {
        _width = width;
    }
    else {
        _width = RTChatNode_Width_Default;
    }
    
    if (widthOffset != -1) {
        _widthOffset = widthOffset;
    }
    else {
        _widthOffset = RTChatNode_WOffset_Default;
    }
    
    auto n = this;
    
    //创建富文本标签
    auto txt = RTContent::create();
    n->addChild(txt, zContent, tContent);
    txt->ignoreContentAdaptWithSize(false);
    txt->setContentSize(Size(_width - (_widthOffset * 2), Default_Max_Node_Height));
    
    //使用内容填充富文本，获取rect
    RTParser::pushElements(txt, _content);
    txt->formatText();
    
    switch (form) {
        case FormType::BORDERED:
        {
            switch (_own) {
                case OwnType::SELF:
                {
                    auto bg = Scale9Sprite::create(Default_Cap_Insets, "res/say-self.png");
                    n->addChild(bg, zBg, tBg);
                    
                    //添加标签图标
                    Sprite* icon = nullptr;
                    switch (_opt) {
                        case OptType::ALL:
                        {
                            icon = Sprite::create("res/icon-all.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::ROOM:
                        {
                            icon = Sprite::create("res/icon-room.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::DESK:
                        {
                            icon = Sprite::create("res/icon-desk.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::SYSTEM:
                        default:
                            break;
                    }
                    
                    if (!icon) {
                        return false;
                    }
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(_width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point(n->getContentSize().width / 2.0 - (icon->getContentSize().width / 2.0) - _widthOffset - _widthOffset,
                                            titleY));
                    lb->setPosition(Point(n->getContentSize().width / 2.0 - (lb->getContentSize().width / 2.0) - _widthOffset - icon->getContentSize().width - _widthOffset - _widthOffset,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                case OwnType::OTHER:
                {
                    auto bg = Scale9Sprite::create(Default_Cap_Insets, "res/say-other.png");
                    n->addChild(bg, zBg, tBg);
                    
                    //添加标签图标
                    Sprite* icon = nullptr;
                    switch (_opt) {
                        case OptType::ALL:
                        {
                            icon = Sprite::create("res/icon-all.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::ROOM:
                        {
                            icon = Sprite::create("res/icon-room.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::DESK:
                        {
                            icon = Sprite::create("res/icon-desk.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::SYSTEM:
                        {
                            icon = Sprite::create("res/icon-system.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        default:
                            break;
                    }
                    
                    if (!icon) {
                        return false;
                    }
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(_width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + _widthOffset + _widthOffset,
                                            titleY));
                    lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + _widthOffset + icon->getContentSize().width + _widthOffset + _widthOffset,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0 + _widthOffset, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                default:
                    return false;
                    break;
            }
        }
            break;
        case FormType::CLEAR:
        {
            //添加标签图标
            Sprite* icon = nullptr;
            switch (_opt) {
                case OptType::ALL:
                {
                    icon = Sprite::create("res/icon-all.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                case OptType::ROOM:
                {
                    icon = Sprite::create("res/icon-room.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                case OptType::DESK:
                {
                    icon = Sprite::create("res/icon-desk.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                case OptType::SYSTEM:
                {
                    icon = Sprite::create("res/icon-system.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                default:
                    break;
            }
            
            if (!icon) {
                return false;
            }
            
            //添加标签文本
            auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
            lb->setColor(Default_SAY_Color);
            n->addChild(lb, zTitleLabel, tTitleLabel);
            
            //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
            Size containerSize = Size(_width,
                                      lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height);
            
            n->setContentSize(containerSize);
            
            //设置标签位置
            float titleY = n->getContentSize().height / 2.0 - (icon->getContentSize().height / 2.0);
            icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + _widthOffset + _widthOffset,
                                    titleY));
            lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + _widthOffset + icon->getContentSize().width + _widthOffset + _widthOffset,
                                  titleY));
            
            //设置富文本位置
            txt->setPosition(Point(0 + _widthOffset, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
            
        }
        default:
            break;
    }
    
    
    return true;
}
