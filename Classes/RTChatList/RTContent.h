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

#ifndef __UIRTContent_H__
#define __UIRTContent_H__

#include "ui/UIWidget.h"
#include "ui/GUIExport.h"

NS_CC_BEGIN

namespace ui {
    
    class CC_GUI_DLL RTElement : public Ref
    {
    public:
        enum class Type
        {
            TEXT,
            NEWLINE,
            IMAGE,
            CUSTOM
        };
        RTElement(){};
        virtual ~RTElement(){};
        bool init(int tag, const Color3B& color, GLubyte opacity);
        Type getType() { return _type; }
        
    protected:
        Type _type;
        int _tag;
        Color3B _color;
        GLubyte _opacity;
        friend class RTContent;
    };
    
    class CC_GUI_DLL RTElementText : public RTElement
    {
    public:
        RTElementText(){_type = Type::TEXT;};
        virtual ~RTElementText(){};
        bool init(int tag, const Color3B& color, GLubyte opacity, const std::string& text, const std::string& fontName, float fontSize);
        static RTElementText* create(int tag, const Color3B& color, GLubyte opacity, const std::string& text, const std::string& fontName, float fontSize);
    protected:
        std::string _text;
        std::string _fontName;
        float _fontSize;
        friend class RTContent;
        
    };
    
    class CC_GUI_DLL RTElementNewLine : public RTElement
    {
    public:
        RTElementNewLine(){_type = Type::NEWLINE;};
        virtual ~RTElementNewLine(){};
        bool init();
        static RTElementNewLine* create();
    protected:
        friend class RTContent;
    };
    
    class CC_GUI_DLL RTElementImage : public RTElement
    {
    public:
        RTElementImage(){_type = Type::IMAGE;};
        virtual ~RTElementImage(){};
        bool init(int tag, const Color3B& color, GLubyte opacity, const std::string& filePath);
        static RTElementImage* create(int tag, const Color3B& color, GLubyte opacity, const std::string& filePath);
    protected:
        std::string _filePath;
        Rect _textureRect;
        int _textureType;
        friend class RTContent;
    };
    
    class CC_GUI_DLL RTElementCustomNode : public RTElement
    {
    public:
        RTElementCustomNode(){_type = Type::CUSTOM;};
        virtual ~RTElementCustomNode(){CC_SAFE_RELEASE(_customNode);};
        bool init(int tag, const Color3B& color, GLubyte opacity, Node* customNode);
        static RTElementCustomNode* create(int tag, const Color3B& color, GLubyte opacity, Node* customNode);
    protected:
        Node* _customNode;
        friend class RTContent;
    };
    
    class CC_GUI_DLL RTElementEmoji : public RTElementCustomNode
    {
    public:
        RTElementEmoji(){_type = Type::CUSTOM;};
        virtual ~RTElementEmoji(){};
        bool init(int tag, const Color3B& color, GLubyte opacity, Size s, int emojiId);
        static RTElementEmoji* create(int tag, const Color3B& color, GLubyte opacity, Size s, int emojiId);
        
    protected:
        Node* _customNode;
        friend class RichText;
    };
    
    class CC_GUI_DLL RTContent : public Widget
    {
    public:
        RTContent();
        virtual ~RTContent();
        static RTContent* create();
        void insertElement(RTElement* element, int index);
        void pushBackElement(RTElement* element);
        void removeElement(int index);
        void removeElement(RTElement* element);
        
        void setVerticalSpace(float space);
        virtual void setAnchorPoint(const Vec2 &pt);
        virtual Size getVirtualRendererSize() const override;
        void formatText();
        virtual void ignoreContentAdaptWithSize(bool ignore);
        virtual std::string getDescription() const override;
        float getCalcWidth() { return _calcWidth; }
        
    CC_CONSTRUCTOR_ACCESS:
        virtual bool init() override;
        
    protected:
        virtual void adaptRenderers();
        
        virtual void initRenderer();
        void pushToContainer(Node* renderer);
        void handleTextRenderer(const std::string& text, const std::string& fontName, float fontSize, const Color3B& color, GLubyte opacity);
        void handleImageRenderer(const std::string& fileParh, const Color3B& color, GLubyte opacity);
        void handleCustomRenderer(Node* renderer);
        void formarRenderers();
        void addNewLine();
    protected:
        bool _formatTextDirty;
        Vector<RTElement*> _RTElements;
        std::vector<Vector<Node*>*> _elementRenders;
        float _leftSpaceWidth;
        float _verticalSpace;
        float _calcWidth;
        Node* _elementRenderersContainer;
    };
    
}

NS_CC_END

#endif /* defined(__UIRTContent__) */
