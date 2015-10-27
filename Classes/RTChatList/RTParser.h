
#ifndef __EM_RICHPARSER_H__
#define __EM_RICHPARSER_H__

#include "cocos2d.h"
#include "RTContent.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class RTParser
{
public:
    
    typedef enum : int {
        Barrage,
        Say,
        Show
    } DesType;
    
	// from IRichParser protocol
    static bool pushElements(RTContent* rt,
                             const std::string& json,
                             const DesType config,
                             const std::string& fontName = "Arial",
                             const Color3B& fontColor = Color3B::BLACK,
                             const float fontSize = 20,
                             const Size& imgSize = Size(30, 30),
                             const GLubyte alpha = 255);
    
    RTParser();
    ~RTParser();
};

#endif//__EM_RICHPARSER_H__
