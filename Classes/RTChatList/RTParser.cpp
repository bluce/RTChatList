

#include "RTParser.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"

#define kBarrage    "barrage"
#define kSay        "say"
#define kShow       "show"

bool RTParser::pushElements(RTContent* rt,
                            const std::string& json,
                            const DesType config,
                            const std::string& fontName,
                            const Color3B& fontColor,
                            const float fontSize,
                            const Size& imgSize,
                            const GLubyte alpha)
{
    std::string _fontName = fontName;
    Color3B _fontColor = fontColor;
    float _fontSize = fontSize;
    Size _imgSize = imgSize;
    GLubyte _alpha = alpha;
    std::string key = "";
    switch (config) {
        case DesType::Barrage:
            key = kBarrage;
            break;
        case DesType::Say:
            key = kSay;
            break;
        case DesType::Show:
            key = kShow;
            break;
        default:
            break;
    }
    
    rapidjson::Document jsonDoc;
    jsonDoc.Parse<0>(json.c_str());
    
    if (jsonDoc.HasParseError()) {
        CCLOG("RTParser::pushElements parse json failed [%s]", json.c_str());
        return false;
    }
    
    if (!jsonDoc.IsArray()) {
        CCLOG("RTParser::pushElements jsonDoc is not array list [%s]", json.c_str());
        return false;
    }
    
    for (int i = 0; i < jsonDoc.Size(); i++) {
        rapidjson::Value& jvo = jsonDoc[i];
        if (!jvo.IsObject()) {
            CCLOG("RTParser::pushElements jsonDoc array content at [%d] is not object [%s]", i, json.c_str());
            return false;
        }
        
        std::string type = "";      //类型
        int emojiId = -1;           //表情id
        std::string txt = "";       //文本
        
        rapidjson::Value& jtype = jvo["type"];
        if (jtype.IsString()) {
            if (jtype.GetString()) {
                type = jtype.GetString();
            }
        }
        
        if ("" == type || "t" == type) {
            rapidjson::Value& jtxt = jvo["t"];
            if (jtxt.IsString()) {
                if (jtxt.GetString()) {
                    txt = jtxt.GetString();
                    type = "t";
                }
            }
        }
        
        if (type != "t") {
            rapidjson::Value& jem = jvo["e"];
            if (jem.IsInt()) {
                emojiId = jem.GetInt();
                type = "e";
            }
        }
        
        
        if ("t" == type) {
            //文本节点
            rapidjson::Value& jstyle = jvo[key.c_str()];
            Color3B _remoteFontColor = _fontColor;
            if (jstyle.IsObject()) {
                rapidjson::Value& jfn = jstyle["fn"];
                if (jfn.IsString()) {
                    _fontName = jfn.GetString();
                }
                
                rapidjson::Value& jtsize = jstyle["s"];
                if (jtsize.IsDouble()) {
                    _fontSize = jtsize.GetDouble();
                }
                else if (jtsize.IsInt()) {
                    _fontSize = jtsize.GetInt();
                }
                
                rapidjson::Value& jcolor = jstyle["c"];
                if (jcolor.IsString()) {
                    std::string sc = jcolor.GetString();
                    if (sc.length() == 9) {
                        char* end = NULL;
                        
                        std::string sr = sc.substr(1, 2);
                        end = NULL;
                        int r = static_cast<int>(strtol(sr.c_str(), &end, 16));
                        
                        std::string sg = sc.substr(3, 2);
                        end = NULL;
                        int g = static_cast<int>(strtol(sg.c_str(), &end, 16));
                        
                        std::string sb = sc.substr(5, 2);
                        end = NULL;
                        int b = static_cast<int>(strtol(sb.c_str(), &end, 16));
                        
                        std::string sa = sc.substr(7, 2);
                        end = NULL;
                        int a = static_cast<int>(strtol(sa.c_str(), &end, 16));
                        
                        _remoteFontColor = Color3B(r, g, b);
                        _alpha = a;
                    }
                }
            }
            
            rt->pushBackElement(RTElementText::create(i, _remoteFontColor, _alpha, txt, _fontName, _fontSize));
        }
        else if ("e" == type) {
            //表情节点
            
            rapidjson::Value& jstyle = jvo[key.c_str()];
            
            if (jstyle.IsObject()) {
                
                rapidjson::Value& jw = jstyle["w"];
                if (jw.IsDouble()) {
                    _imgSize.width = jw.GetDouble();
                }
                else if (jw.IsInt()) {
                    _imgSize.width = jw.GetInt();
                }
                
                rapidjson::Value& jh = jstyle["h"];
                if (jh.IsDouble()) {
                    _imgSize.height = jh.GetDouble();
                }
                else if (jh.IsInt()) {
                    _imgSize.height = jh.GetInt();
                }
            }
            
            rt->pushBackElement(RTElementEmoji::create(i, _fontColor, _alpha, _imgSize, emojiId));
        }
    }
    
    return true;
}

