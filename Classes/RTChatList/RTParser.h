
#ifndef __EM_RICHPARSER_H__
#define __EM_RICHPARSER_H__

#include "cocos2d.h"
#include "RTContent.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class RTParser
{
public:
	// from IRichParser protocol
    static bool pushElements(RTContent* rt, const std::string& json);
    RTParser();
    ~RTParser();
};

#endif//__EM_RICHPARSER_H__
