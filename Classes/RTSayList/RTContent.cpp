/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "RTContent.h"
#include "platform/CCFileUtils.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/ccUTF8.h"
#include "ui/UIHelper.h"
#include "RTEmojiSprite.h"

NS_CC_BEGIN

namespace ui {
    
    bool RTElement::init(int tag, const Color3B &color, GLubyte opacity)
    {
        _tag = tag;
        _color = color;
        _opacity = opacity;
        return true;
    }
    
    RTElementText* RTElementText::create(int tag, const Color3B &color, GLubyte opacity, const std::string& text, const std::string& fontName, float fontSize)
    {
        RTElementText* element = new (std::nothrow) RTElementText();
        if (element && element->init(tag, color, opacity, text, fontName, fontSize))
        {
            element->autorelease();
            return element;
        }
        CC_SAFE_DELETE(element);
        return nullptr;
    }
    
    bool RTElementText::init(int tag, const Color3B &color, GLubyte opacity, const std::string& text, const std::string& fontName, float fontSize)
    {
        if (RTElement::init(tag, color, opacity))
        {
            _text = text;
            _fontName = fontName;
            _fontSize = fontSize;
            return true;
        }
        return false;
    }
    
    RTElementNewLine* RTElementNewLine::create()
    {
        RTElementNewLine* element = new (std::nothrow) RTElementNewLine();
        if (element && element->init())
        {
            element->autorelease();
            return element;
        }
        CC_SAFE_DELETE(element);
        return nullptr;
    }
    
    bool RTElementNewLine::init()
    {
        if (RTElement::init(0, Color3B::WHITE, 255))
        {
            return true;
        }
        return false;
    }
    
    RTElementImage* RTElementImage::create(int tag, const Color3B &color, GLubyte opacity, const std::string& filePath)
    {
        RTElementImage* element = new (std::nothrow) RTElementImage();
        if (element && element->init(tag, color, opacity, filePath))
        {
            element->autorelease();
            return element;
        }
        CC_SAFE_DELETE(element);
        return nullptr;
    }
    
    bool RTElementImage::init(int tag, const Color3B &color, GLubyte opacity, const std::string& filePath)
    {
        if (RTElement::init(tag, color, opacity))
        {
            _filePath = filePath;
            return true;
        }
        return false;
    }
    
    RTElementCustomNode* RTElementCustomNode::create(int tag, const Color3B &color, GLubyte opacity, cocos2d::Node *customNode)
    {
        RTElementCustomNode* element = new (std::nothrow) RTElementCustomNode();
        if (element && element->init(tag, color, opacity, customNode))
        {
            element->autorelease();
            return element;
        }
        CC_SAFE_DELETE(element);
        return nullptr;
    }
    
    bool RTElementCustomNode::init(int tag, const Color3B &color, GLubyte opacity, cocos2d::Node *customNode)
    {
        if (RTElement::init(tag, color, opacity))
        {
            _customNode = customNode;
            _customNode->retain();
            return true;
        }
        return false;
    }
    
    bool RTElementEmoji::init(int tag, const cocos2d::Color3B &color, GLubyte opacity, cocos2d::Size s, int emojiId)
    {
        auto sp = RTEmojiSprite::create(emojiId, s);
        return RTElementCustomNode::init(tag, color, opacity, sp);
    }
    
    RTElementEmoji* RTElementEmoji::create(int tag, const cocos2d::Color3B &color, GLubyte opacity, cocos2d::Size s, int emojiId)
    {
        RTElementEmoji* element = new (std::nothrow) RTElementEmoji();
        if (element && element->init(tag, color, opacity, s, emojiId))
        {
            element->autorelease();
            return element;
        }
        CC_SAFE_DELETE(element);
        return nullptr;
    }
    
    RTContent::RTContent():
    _formatTextDirty(true),
    _leftSpaceWidth(0.0f),
    _verticalSpace(0.0f),
    _elementRenderersContainer(nullptr)
    {
        
    }
    
    RTContent::~RTContent()
    {
        _RTElements.clear();
    }
    
    RTContent* RTContent::create()
    {
        RTContent* widget = new (std::nothrow) RTContent();
        if (widget && widget->init())
        {
            widget->autorelease();
            return widget;
        }
        CC_SAFE_DELETE(widget);
        return nullptr;
    }
    
    bool RTContent::init()
    {
        if (Widget::init())
        {
            return true;
        }
        return false;
    }
    
    void RTContent::initRenderer()
    {
        _elementRenderersContainer = Node::create();
        _elementRenderersContainer->setAnchorPoint(Vec2(0.5f, 0.5f));
        addProtectedChild(_elementRenderersContainer, 0, -1);
    }
    
    void RTContent::insertElement(RTElement *element, int index)
    {
        _RTElements.insert(index, element);
        _formatTextDirty = true;
    }
    
    void RTContent::pushBackElement(RTElement *element)
    {
        _RTElements.pushBack(element);
        _formatTextDirty = true;
    }
    
    void RTContent::removeElement(int index)
    {
        _RTElements.erase(index);
        _formatTextDirty = true;
    }
    
    void RTContent::removeElement(RTElement *element)
    {
        _RTElements.eraseObject(element);
        _formatTextDirty = true;
    }
    
    void RTContent::formatText()
    {
        if (_formatTextDirty)
        {
            _elementRenderersContainer->removeAllChildren();
            _elementRenders.clear();
            if (_ignoreSize)
            {
                addNewLine();
                for (ssize_t i=0; i<_RTElements.size(); i++)
                {
                    RTElement* element = _RTElements.at(i);
                    Node* elementRenderer = nullptr;
                    switch (element->_type)
                    {
                        case RTElement::Type::TEXT:
                        {
                            RTElementText* elmtText = static_cast<RTElementText*>(element);
                            if (FileUtils::getInstance()->isFileExist(elmtText->_fontName))
                            {
                                elementRenderer = Label::createWithTTF(elmtText->_text.c_str(), elmtText->_fontName, elmtText->_fontSize);
                            }
                            else
                            {
                                elementRenderer = Label::createWithSystemFont(elmtText->_text.c_str(), elmtText->_fontName, elmtText->_fontSize);
                            }
                            break;
                        }
                        case RTElement::Type::IMAGE:
                        {
                            RTElementImage* elmtImage = static_cast<RTElementImage*>(element);
                            elementRenderer = Sprite::create(elmtImage->_filePath.c_str());
                            break;
                        }
                        case RTElement::Type::CUSTOM:
                        {
                            RTElementCustomNode* elmtCustom = static_cast<RTElementCustomNode*>(element);
                            elementRenderer = elmtCustom->_customNode;
                            break;
                        }
                        default:
                            break;
                    }
                    elementRenderer->setColor(element->_color);
                    elementRenderer->setOpacity(element->_opacity);
                    pushToContainer(elementRenderer);
                }
            }
            else
            {
                addNewLine();
                for (ssize_t i=0; i<_RTElements.size(); i++)
                {
                    
                    RTElement* element = static_cast<RTElement*>(_RTElements.at(i));
                    switch (element->_type)
                    {
                        case RTElement::Type::TEXT:
                        {
                            RTElementText* elmtText = static_cast<RTElementText*>(element);
                            handleTextRenderer(elmtText->_text.c_str(), elmtText->_fontName.c_str(), elmtText->_fontSize, elmtText->_color, elmtText->_opacity);
                            break;
                        }
                        case RTElement::Type::IMAGE:
                        {
                            RTElementImage* elmtImage = static_cast<RTElementImage*>(element);
                            handleImageRenderer(elmtImage->_filePath.c_str(), elmtImage->_color, elmtImage->_opacity);
                            break;
                        }
                        case RTElement::Type::CUSTOM:
                        {
                            RTElementCustomNode* elmtCustom = static_cast<RTElementCustomNode*>(element);
                            handleCustomRenderer(elmtCustom->_customNode);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            formarRenderers();
            _formatTextDirty = false;
        }
    }
    
    void RTContent::handleTextRenderer(const std::string& text, const std::string& fontName, float fontSize, const Color3B &color, GLubyte opacity)
    {
        auto fileExist = FileUtils::getInstance()->isFileExist(fontName);
        Label* textRenderer = nullptr;
        if (fileExist)
        {
            textRenderer = Label::createWithTTF(text, fontName, fontSize);
        }
        else
        {
            textRenderer = Label::createWithSystemFont(text, fontName, fontSize);
        }
        float textRendererWidth = textRenderer->getContentSize().width;
        _leftSpaceWidth -= textRendererWidth;
        if (_leftSpaceWidth < 0.0f)
        {
            float overstepPercent = (-_leftSpaceWidth) / textRendererWidth;
            std::string curText = text;
            size_t stringLength = StringUtils::getCharacterCountInUTF8String(text);
            int leftLength = stringLength * (1.0f - overstepPercent);
            std::string leftWords = Helper::getSubStringOfUTF8String(curText,0,leftLength);
            std::string cutWords = Helper::getSubStringOfUTF8String(curText, leftLength, stringLength - leftLength);
            if (leftLength > 0)
            {
                Label* leftRenderer = nullptr;
                if (fileExist)
                {
                    leftRenderer = Label::createWithTTF(Helper::getSubStringOfUTF8String(leftWords, 0, leftLength), fontName, fontSize);
                }
                else
                {
                    leftRenderer = Label::createWithSystemFont(Helper::getSubStringOfUTF8String(leftWords, 0, leftLength), fontName, fontSize);
                }
                if (leftRenderer)
                {
                    leftRenderer->setColor(color);
                    leftRenderer->setOpacity(opacity);
                    pushToContainer(leftRenderer);
                }
            }
            
            addNewLine();
            handleTextRenderer(cutWords.c_str(), fontName, fontSize, color, opacity);
        }
        else
        {
            textRenderer->setColor(color);
            textRenderer->setOpacity(opacity);
            pushToContainer(textRenderer);
        }
    }
    
    void RTContent::handleImageRenderer(const std::string& fileParh, const Color3B &color, GLubyte opacity)
    {
        Sprite* imageRenderer = Sprite::create(fileParh);
        handleCustomRenderer(imageRenderer);
    }
    
    void RTContent::handleCustomRenderer(cocos2d::Node *renderer)
    {
        Size imgSize = renderer->getContentSize();
        _leftSpaceWidth -= imgSize.width;
        if (_leftSpaceWidth < 0.0f)
        {
            addNewLine();
            pushToContainer(renderer);
            _leftSpaceWidth -= imgSize.width;
        }
        else
        {
            pushToContainer(renderer);
        }
    }
    
    void RTContent::addNewLine()
    {
        _leftSpaceWidth = _customSize.width;
        _elementRenders.push_back(new Vector<Node*>());
    }
    
    void RTContent::formarRenderers()
    {
        if (_ignoreSize)
        {
            float newContentSizeWidth = 0.0f;
            float newContentSizeHeight = 0.0f;
            
            Vector<Node*>* row = (_elementRenders[0]);
            float nextPosX = 0.0f;
            for (ssize_t j=0; j<row->size(); j++)
            {
                Node* l = row->at(j);
                l->setAnchorPoint(Vec2::ZERO);
                l->setPosition(nextPosX, 0.0f);
                _elementRenderersContainer->addChild(l, 1);
                Size iSize = l->getContentSize();
                newContentSizeWidth += iSize.width;
                newContentSizeHeight = MAX(newContentSizeHeight, iSize.height);
                nextPosX += iSize.width;
            }
            _elementRenderersContainer->setContentSize(Size(newContentSizeWidth, newContentSizeHeight));
        }
        else
        {
            float newContentSizeHeight = 0.0f;
            float *maxHeights = new float[_elementRenders.size()];
            
            for (size_t i=0; i<_elementRenders.size(); i++)
            {
                Vector<Node*>* row = (_elementRenders[i]);
                float maxHeight = 0.0f;
                for (ssize_t j=0; j<row->size(); j++)
                {
                    Node* l = row->at(j);
                    maxHeight = MAX(l->getContentSize().height, maxHeight);
                }
                maxHeights[i] = maxHeight;
                newContentSizeHeight += maxHeights[i];
            }
            
            _customSize.height = newContentSizeHeight;
            float nextPosY = _customSize.height;
            for (size_t i=0; i<_elementRenders.size(); i++)
            {
                Vector<Node*>* row = (_elementRenders[i]);
                float nextPosX = 0.0f;
                nextPosY -= (maxHeights[i] + _verticalSpace);
                
                for (ssize_t j=0; j<row->size(); j++)
                {
                    Node* l = row->at(j);
                    l->setAnchorPoint(Vec2::ZERO);
                    l->setPosition(nextPosX, nextPosY);
                    _elementRenderersContainer->addChild(l, 1);
                    nextPosX += l->getContentSize().width;
                }
            }
            _elementRenderersContainer->setContentSize(_customSize);
            delete [] maxHeights;
        }
        
        size_t length = _elementRenders.size();
        for (size_t i = 0; i<length; i++)
        {
            Vector<Node*>* l = _elementRenders[i];
            l->clear();
            delete l;
        }    
        _elementRenders.clear();
        
        if (_ignoreSize)
        {
            Size s = getVirtualRendererSize();
            this->setContentSize(s);
        }
        else
        {
            this->setContentSize(_customSize);
        }
        updateContentSizeWithTextureSize(_contentSize);
        _elementRenderersContainer->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f);
    }
    
    void RTContent::adaptRenderers()
    {
        this->formatText();
    }
    
    void RTContent::pushToContainer(cocos2d::Node *renderer)
    {
        if (_elementRenders.size() <= 0)
        {
            return;
        }
        _elementRenders[_elementRenders.size()-1]->pushBack(renderer);
    }
    
    void RTContent::setVerticalSpace(float space)
    {
        _verticalSpace = space;
    }
    
    void RTContent::setAnchorPoint(const Vec2 &pt)
    {
        Widget::setAnchorPoint(pt);
        _elementRenderersContainer->setAnchorPoint(pt);
    }
    
    Size RTContent::getVirtualRendererSize() const
    {
        return _elementRenderersContainer->getContentSize();
    }
    
    void RTContent::ignoreContentAdaptWithSize(bool ignore)
    {
        if (_ignoreSize != ignore)
        {
            _formatTextDirty = true;
            Widget::ignoreContentAdaptWithSize(ignore);
        }
    }
    
    std::string RTContent::getDescription() const
    {
        return "RTContent";
    }
    
}

NS_CC_END
