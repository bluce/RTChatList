

#include "RTParser.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"
#include "FileUtilsExtension.h"

#define Default_Font_Size   32.0
#define Default_Font_Color  Color3B::WHITE
#define Default_Font_Alpha  255
#define Default_Font_Name   "Arial"

bool RTParser::pushElements(RTContent *rt, const std::string &json)
{
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
        int emojiId = -1;                //表情id
        std::string txt = "";       //文本
        
        rapidjson::Value& jtype = jvo["type"];
        if (jtype.IsString()) {
            if (jtype.GetString()) {
                type = jtype.GetString();
            }
        }
        
        if ("" == type) {
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
//            "s": 20,            - 文本节点的字体大小，不填使用默认值
//            "c": "#ffffffff"    - 文本节点的字体颜色，不填使用默认值，顺序为rgba
            float fsize = Default_Font_Size;
            Color3B color = Default_Font_Color;
            GLubyte alpha = Default_Font_Alpha;
            
            rapidjson::Value& jtsize = jvo["s"];
            if (jtsize.IsDouble()) {
                fsize = jtsize.GetDouble();
            }
            else if (jtsize.IsInt()) {
                fsize = jtsize.GetInt();
            }
            
            rapidjson::Value& jcolor = jvo["c"];
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
                    
                    color = Color3B(r, g, b);
                    alpha = a;
                }
            }
            
            rt->pushBackElement(RTElementText::create(i, color, alpha, txt, Default_Font_Name, fsize));
        }
        else if ("e" == type) {
            //表情节点
            float fw = Default_Font_Size;
            float fh = Default_Font_Size;
            
            rapidjson::Value& jw = jvo["w"];
            if (jw.IsDouble()) {
                fw = jw.GetDouble();
            }
            else if (jw.IsInt()) {
                fw = jw.GetInt();
            }
            
            rapidjson::Value& jh = jvo["h"];
            if (jh.IsDouble()) {
                fh = jh.GetDouble();
            }
            else if (jh.IsInt()) {
                fh = jh.GetInt();
            }
            
            rt->pushBackElement(RTElementEmoji::create(i, Default_Font_Color, Default_Font_Alpha, Size(fw, fh), emojiId));
        }
    }
    
    return true;
}

