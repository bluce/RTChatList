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
#include "DbgHelper.h"

#define RTChatNode_Width_Default      400
#define RTChatNode_WOffset_Default    16

#define RTChatNode_WOffset_Distance   8

#define Default_Max_Node_Height     600

#define Default_Node_Offset_Height  8
#define Default_Node_Offset_Height_Clear  3
#define Default_Node_Height_Distance  5
#define Default_Node_POP_Height_Distance  6

#define Default_Font_Size           20
#define Default_Image_Size          30
#define Default_Font_Alpha          255
#define Default_Font_Name           ""

#define Default_ALL_Color           Color3B(0x06, 0xff, 0xff)
#define Default_SAY_Color           Color3B(0x00, 0x78, 0xff)
#define Default_Clear_title_Color   Color3B(33, 0xff, 0xff)

#define Default_Cap_Insets          Rect(30, 45, 5, 5)
#define Default_Pop_Insets          Rect(5, 5, 200, 60)

typedef enum : int {
    tBg = 1,
    tArrow = 2,
    tTitleIcon = 3,
    tTitleLabel = 4,
    tContent = 5
} RTChatNodeTag;

typedef enum : int {
    zBg = 1,
    zArrow = 2,
    zTitleIcon = 3,
    zTitleLabel = 4,
    zContent = 5
} RTChatNodeZorder;

RTChatNode::RTChatNode()
{
#if COCOS2D_DEBUG
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = CC_CALLBACK_2(RTChatNode::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
#endif
}

RTChatNode::~RTChatNode()
{
    _eventDispatcher->removeEventListenersForTarget(this);
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
    txt->setContentSize(Size(_width - (RTChatNode_WOffset_Distance * 2), Default_Max_Node_Height));
    
    switch (form) {
        case FormType::BORDERED:
        {
            //使用内容填充富文本，获取rect
            RTParser::pushElements(txt, _content, RTParser::DesType::Say, "");
            txt->formatText();
            
            switch (_own) {
                case OwnType::SELF:
                {
                    auto bg = Scale9Sprite::create(Default_Cap_Insets, "chat/say-self.png");
                    n->addChild(bg, zBg, tBg);
                    
                    //添加标签图标
                    Sprite* icon = nullptr;
                    switch (_opt) {
                        case OptType::ALL:
                        {
                            icon = Sprite::create("chat/icon-all.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::ROOM:
                        {
                            icon = Sprite::create("chat/icon-room.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::DESK:
                        {
                            icon = Sprite::create("chat/icon-desk.png");
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
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, Default_Font_Size);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(_width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Height_Distance + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    lb->setPosition(Point(n->getContentSize().width / 2.0 - (lb->getContentSize().width / 2.0) - _widthOffset, titleY));
                    icon->setPosition(Point(n->getContentSize().width / 2.0 - (icon->getContentSize().width / 2.0) - lb->getContentSize().width - _widthOffset, titleY));
                    
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        float bg_w = lb->getBoundingBox().size.width + icon->getBoundingBox().size.width;
                        if (bg_w < txt->getCalcWidth()) {
                            bg_w = txt->getCalcWidth();
                        }
                        bg->setContentSize(Size(bg_w + _widthOffset + _widthOffset, containerSize.height));
                        bg->setPosition(Point((n->getBoundingBox().size.width - bg->getBoundingBox().size.width) / 2.0, 0));
                    }
                    else {
                        bg->setContentSize(containerSize);
                    }
                    
                    //设置富文本位置
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        txt->setPosition(Point(n->getBoundingBox().size.width - txt->getCalcWidth() - 30, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                    }
                    else {
                        txt->setPosition(Point(0, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                    }
                }
                    break;
                case OwnType::OTHER:
                {
                    auto bg = Scale9Sprite::create(Default_Cap_Insets, "chat/say-other.png");
                    n->addChild(bg, zBg, tBg);
                    
                    //添加标签图标
                    Sprite* icon = nullptr;
                    switch (_opt) {
                        case OptType::ALL:
                        {
                            icon = Sprite::create("chat/icon-all.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::ROOM:
                        {
                            icon = Sprite::create("chat/icon-room.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::DESK:
                        {
                            icon = Sprite::create("chat/icon-desk.png");
                            n->addChild(icon, zTitleIcon, tTitleIcon);
                        }
                            break;
                        case OptType::SYSTEM:
                        {
                            icon = Sprite::create("chat/icon-system.png");
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
                    auto lb = Label::createWithSystemFont(_title, Default_Font_Name, Default_Font_Size);
                    lb->setColor(Default_SAY_Color);
                    n->addChild(lb, zTitleLabel, tTitleLabel);
                    
                    //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
                    Size containerSize = Size(_width,
                                              Default_Node_Offset_Height + lb->getContentSize().height + Default_Node_Height_Distance + txt->getContentSize().height + Default_Node_Offset_Height);
                    
                    n->setContentSize(containerSize);
                    bg->setContentSize(containerSize);
                    
                    //设置标签位置
                    float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height - (icon->getContentSize().height / 2.0);
                    icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + _widthOffset, titleY));
                    lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + icon->getContentSize().width + _widthOffset, titleY));
                    
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        float bg_w = lb->getBoundingBox().size.width + icon->getBoundingBox().size.width;
                        if (bg_w < txt->getCalcWidth()) {
                            bg_w = txt->getCalcWidth();
                        }
                        bg->setContentSize(Size(bg_w + _widthOffset + _widthOffset, containerSize.height));
                        bg->setPosition(Point(- (containerSize.width - bg->getBoundingBox().size.width) / 2.0, 0));
                    }
                    else {
                        bg->setContentSize(containerSize);
                    }
                    
                    //设置富文本位置
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        txt->setPosition(Point(RTChatNode_WOffset_Distance + 0, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                    }
                    else {
                        txt->setPosition(Point(0 + RTChatNode_WOffset_Distance, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height + (txt->getContentSize().height / 2.0)));
                    }
                    
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
            //使用内容填充富文本，获取rect
            RTParser::pushElements(txt,
                                   _content,
                                   RTParser::DesType::Show,
                                   "",
                                   Color3B::WHITE,
                                   20,
                                   Size(30, 30),
                                   255);
            txt->formatText();
            
            //添加标签图标
            Sprite* icon = nullptr;
            switch (_opt) {
                case OptType::ALL:
                {
                    icon = Sprite::create("chat/icon-all.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                case OptType::ROOM:
                {
                    icon = Sprite::create("chat/icon-room.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                case OptType::DESK:
                {
                    icon = Sprite::create("chat/icon-desk.png");
                    n->addChild(icon, zTitleIcon, tTitleIcon);
                }
                    break;
                case OptType::SYSTEM:
                {
                    icon = Sprite::create("chat/icon-system.png");
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
            auto lb = Label::createWithSystemFont(_title, Default_Font_Name, Default_Font_Size);
            lb->setColor(Default_Clear_title_Color);
            n->addChild(lb, zTitleLabel, tTitleLabel);
            
            //使用 空白 - 富文本高度 - 空白 - 标签 - 空白 确定容器高度
            Size containerSize = Size(_width,
                                      Default_Node_Offset_Height_Clear + lb->getContentSize().height + Default_Node_Offset_Height_Clear + txt->getContentSize().height + Default_Node_Offset_Height_Clear);
            
            n->setContentSize(containerSize);
            
            //设置标签位置
            float titleY = n->getContentSize().height / 2.0 - Default_Node_Offset_Height_Clear - (icon->getContentSize().height / 2.0);
            icon->setPosition(Point( - (n->getContentSize().width / 2.0) + (icon->getContentSize().width / 2.0) + _widthOffset,
                                    titleY));
            lb->setPosition(Point(- (n->getContentSize().width / 2.0) + (lb->getContentSize().width / 2.0) + icon->getContentSize().width + _widthOffset,
                                  titleY));
            
            //设置富文本位置
            txt->setPosition(Point(0 + RTChatNode_WOffset_Distance, - (n->getContentSize().height / 2.0) + Default_Node_Offset_Height_Clear + (txt->getContentSize().height / 2.0)));
            
        }
            break;
        case FormType::POP:
        {
            //使用内容填充富文本，获取rect
            RTParser::pushElements(txt, _content,
                                   RTParser::DesType::Say,
                                   "",
                                   Color3B::WHITE,
                                   20,
                                   Size(30, 30),
                                   255);
            txt->formatText();
            
            switch (_own) {
                case OwnType::SELF:
                {
                    auto bg = Scale9Sprite::create(Default_Pop_Insets, "chat/desk-pop-bg.png");
                    n->addChild(bg, zBg, tBg);
                    auto arrow = Sprite::create("chat/desk-pop-arrow.png");
                    n->addChild(arrow, zArrow, tArrow);
                    
                    //使用 空白 - 富文本高度 - 空白 确定容器高度
                    Size containerSize = Size(_width,
                                              Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance + arrow->getContentSize().height);
                    n->setContentSize(containerSize);
                    
                    //设置标签位置
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        float bg_w = txt->getCalcWidth();
                        bg->setContentSize(Size(bg_w + _widthOffset + _widthOffset, Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance));
                    }
                    else {
                        bg->setContentSize(Size(containerSize.width, Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance));
                    }
                    bg->setPosition(Point(containerSize.width / 2.0, containerSize.height - (bg->getContentSize().height / 2.0)));
                    arrow->setPosition(Point(containerSize.width / 2.0, arrow->getContentSize().height / 2.0));
                    
                    //设置富文本位置
                    
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        txt->setPosition(Point(containerSize.width + (bg->getContentSize().width / 2.0) - txt->getCalcWidth() - 30, containerSize.height - (bg->getContentSize().height / 2.0)));
                    }
                    else {
                        txt->setPosition(bg->getPosition());
                    }
                }
                    break;
                case OwnType::OTHER:
                {
                    auto bg = Scale9Sprite::create(Default_Pop_Insets, "chat/desk-pop-bg.png");
                    n->addChild(bg, zBg, tBg);
                    auto arrow = Sprite::create("chat/desk-pop-arrow.png");
                    arrow->setRotation(180);
                    n->addChild(arrow, zArrow, tArrow);
                    
                    //使用 空白 - 富文本高度 - 空白 确定容器高度
                    Size containerSize = Size(_width,
                                              Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance + arrow->getContentSize().height);
                    n->setContentSize(containerSize);
                    
                    arrow->setPosition(Point(containerSize.width / 2.0, Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance + (arrow->getContentSize().height / 2.0)));
                    
                    //设置标签位置
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        float bg_w = txt->getCalcWidth();
                        bg->setContentSize(Size(bg_w + _widthOffset + _widthOffset, Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance));
                    }
                    else {
                        bg->setContentSize(Size(containerSize.width, Default_Node_POP_Height_Distance + txt->getContentSize().height + Default_Node_POP_Height_Distance));
                    }
                    bg->setPosition(Point(containerSize.width / 2.0, bg->getContentSize().height / 2.0));
                    
                    
                    //设置富文本位置
                    if (txt->getBoundingBox().size.height < Default_Font_Size * 2) {
                        txt->setPosition(Point(containerSize.width + (bg->getContentSize().width / 2.0) - txt->getCalcWidth() - 30, bg->getContentSize().height / 2.0));
                    }
                    else {
                        txt->setPosition(bg->getPosition());
                    }
                }
                    break;
                default:
                    return false;
                    break;
            }
        }
            break;
        default:
            break;
    }
    
    return true;
}

#if COCOS2D_DEBUG
bool RTChatNode::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
//    if (FormType::POP == _form) {
//        this->removeFromParent();
//    }
    return false;
}
#endif