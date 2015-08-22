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

#define Default_Max_Node_Width      400
#define Default_Max_Node_Height     600
#define Default_Node_Offset_Width   10
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

RTChatNode* RTChatNode::create(OwnType own, OptType opt, const std::string &title, const std::string &content)
{
    auto n = new (std::nothrow) RTChatNode();
    if (n && n->init(own, opt, title, content)) {
        n->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(n);
    }
    return n;
}

bool RTChatNode::init(OwnType own, OptType opt, const std::string &title, const std::string &content)
{
    _own = own;
    _opt = opt;
    _title = std::string(title);
    _content = std::string(content);
    
    return true;
}

cocos2d::Node* RTChatNode::getNode()
{
    auto n = Node::create();
    
    //创建富文本标签
    auto txt = RTContent::create();
    n->addChild(txt, zContent, tContent);
    
    //使用内容填充富文本，获取rect
    RTParser::pushElements(txt, _content);
    
    Size sz = txt->getContentSize();
    float width = Default_Max_Node_Width - (Default_Node_Offset_Width * 2);
    float height = Default_Max_Node_Height;
    txt->ignoreContentAdaptWithSize(false);
    txt->setContentSize(Size(width, height));
    
    txt->formatText();
    
    switch (_own) {
        case OwnType::SELF:
        {
            auto bg = Scale9Sprite::create(Default_Cap_Insets, "res/say-self.png");
            n->addChild(bg, zBg, tBg);
            
            switch (_opt) {
                case OptType::ALL: //全员发送，喇叭
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-all.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point(n->getContentSize().width / 2.0 - (icon->getContentSize().width / 2.0) - Default_Node_Offset_Width - Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(n->getContentSize().width / 2.0 - (lb->getContentSize().width / 2.0) - Default_Node_Offset_Width - icon->getContentSize().width - Default_Node_Offset_Width - Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                    //ROOM,   //房间发送
                case OptType::ROOM:
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-room.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point(n->getContentSize().width / 2.0 - (icon->getContentSize().width / 2.0) - Default_Node_Offset_Width - Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(n->getContentSize().width / 2.0 - (lb->getContentSize().width / 2.0) - Default_Node_Offset_Width - icon->getContentSize().width - Default_Node_Offset_Width - Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                    //DESK,   //同桌发送
                case OptType::DESK:
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-desk.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point(n->getContentSize().width / 2.0 - (icon->getContentSize().width / 2.0) - Default_Node_Offset_Width - Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(n->getContentSize().width / 2.0 - (lb->getContentSize().width / 2.0) - Default_Node_Offset_Width - icon->getContentSize().width - Default_Node_Offset_Width - Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                    //SYSTEM  //系统信息
                case OptType::SYSTEM:
                default:
                    break;
            }
        }
            break;
        case OwnType::OTHER:
        {
            auto bg = Scale9Sprite::create(Default_Cap_Insets, "res/say-other.png");
            n->addChild(bg, zBg, tBg);
            
            switch (_opt) {
                    //ALL,    //全员发送，喇叭
                case OptType::ALL:
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-all.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + Default_Node_Offset_Width + icon->getContentSize().width + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0 + Default_Node_Offset_Width, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                    //ROOM,   //房间发送
                case OptType::ROOM:
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-room.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + Default_Node_Offset_Width + icon->getContentSize().width + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0 + Default_Node_Offset_Width, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                    //DESK,   //同桌发送
                case OptType::DESK:
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-desk.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + Default_Node_Offset_Width + icon->getContentSize().width + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0 + Default_Node_Offset_Width, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                    //SYSTEM  //系统信息
                case OptType::SYSTEM:
                {
                    //添加标签图标
                    auto icon = Sprite::create("res/icon-system.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                    
                    //添加标签文本
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, icon->getContentSize().height);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(Default_Max_Node_Width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Offset_Height + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                            titleY));
                    lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + Default_Node_Offset_Width + icon->getContentSize().width + Default_Node_Offset_Width + Default_Node_Offset_Width,
                                          titleY));
                    
                    //设置富文本位置
                    txt->setPosition(Point(0 + Default_Node_Offset_Width, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                }
                    break;
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
    
    _contentSize = n->getContentSize();
    return n;
}