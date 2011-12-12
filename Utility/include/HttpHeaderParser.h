#ifndef HTTPHEADERPARSER_H
#define HTTPHEADERPARSER_H

#include "RulesTable.hpp"

#include <map>
#include <vector>
#include <string>

namespace Utility
{
    struct HttpHeader
    {
        typedef std::vector<std::string> AttrVector;

        enum Method 
        {
            METHOD_GET = 0,
            METHOD_POST,
            METHOD_RSP,
            METHOD_CNT
        };

        Method methodM;
        int errorCodeM;
        std::string pathM;
        std::string protocolM;
        AttrVector attrVectorM; 
    };
    class HttpHeaderParser
    {
    public: 
        enum ParseState
        {
            //0 ~ 10000 is reserved for attr
            PARSE_STATE_NONE = 10000,
            PARSE_STATE_METHOD_GET,
            PARSE_STATE_METHOD_POST,
            PARSE_STATE_HOST,

            //special purpose
            PARSE_STATE_IGNORE_LINE,
            PARSE_STATE_SAVE_TO_END,
            PARSE_STATE_SAVE_TO_BLANK,
            PARSE_STATE_STATE_CNT
        };
    public: 
        HttpHeaderParser();
        ~HttpHeaderParser();
        
        void init();
        int parse(
                const std::string& theHeaderStr,
                HttpHeader& theOutputHeader);
    private:
        typedef std::map<int, std::string> HttpHeaderDefs;
        typedef RulesTable2<char, int, int> RulesTable;

        HttpHeaderDefs httpHeaderDefsM;
        RulesTable rulesTableM;
    };
}


#endif /* HTTPHEADERPARSER_H */

