#include "HttpHeaderParser.h"
#include <assert.h>
#include <iostream>

using namespace Utility;
using namespace std;

//-----------------------------------------------------------------------------
HttpHeaderParser::HttpHeaderParser()
{
    init();
}

//-----------------------------------------------------------------------------

HttpHeaderParser::~HttpHeaderParser()
{
}



//-----------------------------------------------------------------------------

void HttpHeaderParser::init()
{
    httpHeaderDefsM.clear();
    httpHeaderDefsM[PARSE_STATE_METHOD_GET]  = "GET "; 
    httpHeaderDefsM[PARSE_STATE_METHOD_POST] = "POST "; 
    httpHeaderDefsM[PARSE_STATE_METHOD_RSP] = "HTTP/1."; 
    int attrId = 0;
    httpHeaderDefsM[attrId++] = "Host:"; 
    httpHeaderDefsM[attrId++] =  "User-Agent:"; 
    httpHeaderDefsM[attrId++] =  "Referer:"; 
    httpHeaderDefsM[attrId++] =  "Content-Type:"; 

    rulesTableM.clear();
    rulesTableM.setDefaultRule(PARSE_STATE_IGNORE_LINE);

    map<string, int> subStates;
    int nextSubState = -1;

    HttpHeaderDefs::iterator it = httpHeaderDefsM.begin();
    for (; it != httpHeaderDefsM.end(); it++)
    {
        string key = it->second;
        int preState = PARSE_STATE_NONE;
        int curState = PARSE_STATE_NONE;
        for (unsigned i = 1; i < key.length(); i++) 
        {
            string preStr = key.substr(0, i);
            map<string, int>::iterator it = subStates.find(preStr);
            if (it != subStates.end())
            {
                curState = it->second;            
            }
            else
            {
                subStates[preStr] = nextSubState;
                curState = nextSubState;
                nextSubState--;
            }
            rulesTableM.setRule(key.at(i-1), preState, curState);
            preState = curState;
        }
        rulesTableM.setRule(key.at(key.length()-1), preState, it->first);
    }
}

//-----------------------------------------------------------------------------

int HttpHeaderParser::parse(
        const std::string& theHeaderStr,
        HttpHeader& theOutputHeader)
{
    theOutputHeader.attrVectorM.clear();
    theOutputHeader.attrVectorM.assign(httpHeaderDefsM.size(), "");

    int state = PARSE_STATE_NONE;
    int saveStart = -1;
    int keyId = PARSE_STATE_NONE;
    int headerLen = theHeaderStr.length();
    int i = 0;
    for (; i < headerLen; i++)
    {
        char ch = theHeaderStr[i];
        if ('\n' == ch)
        {
            state = PARSE_STATE_NONE;
            saveStart = -1;
            continue;
        }

        if (PARSE_STATE_IGNORE_LINE == state) 
        {
            continue;
        }
        if (PARSE_STATE_SAVE_TO_END == state)
        {
            if (' ' == ch && saveStart == (int)i)
            {
                saveStart = i + 1;
                continue;
            }
            else if ('\r' == ch)
            {
                //assert(saveStart != -1);
                //std::cout << "key id:" << keyId << ", out:" << theHeaderStr.substr(saveStart, i - saveStart) << endl;
                if (PARSE_STATE_METHOD_GET == keyId
                        || PARSE_STATE_METHOD_POST == keyId)
                {
                    theOutputHeader.protocolM = 
                        theHeaderStr.substr(saveStart, i - saveStart);
                }
                else
                {
                    theOutputHeader.attrVectorM[keyId] = 
                        theHeaderStr.substr(saveStart, i - saveStart);
                }
                state = PARSE_STATE_IGNORE_LINE;
                continue;
            }
            else
            {
                continue;
            }

        }
        if (PARSE_STATE_SAVE_TO_BLANK == state)
        {
            if (' ' == ch && saveStart == (int)i)
            {
                saveStart = i + 1;
                continue;
            }
            else if (' ' == ch)
            {
                assert(saveStart != -1);
                //std::cout << "key id:" << keyId << ", out:" << theHeaderStr.substr(saveStart, i - saveStart) << endl;
                if (PARSE_STATE_METHOD_GET == keyId
                        || PARSE_STATE_METHOD_POST == keyId)
                {
                    theOutputHeader.pathM = 
                        theHeaderStr.substr(saveStart, i - saveStart);
                    saveStart = i + 1;
                    state = PARSE_STATE_SAVE_TO_END;
                }
                else if (PARSE_STATE_METHOD_RSP == keyId)
                {
                    std::string errorCode = 
                        theHeaderStr.substr(saveStart, i - saveStart);
                    theOutputHeader.errorCodeM = atoi(errorCode.c_str());
                    saveStart = i + 1;
                    state = PARSE_STATE_IGNORE_LINE;
                }
                else
                {
                    state = PARSE_STATE_IGNORE_LINE;
                }
                continue;
            }
            else
            {
                continue;
            }
        }

        state = rulesTableM.getRule(ch, state);
        if (PARSE_STATE_METHOD_GET == state)
        {
            keyId = PARSE_STATE_METHOD_GET;
            state = PARSE_STATE_SAVE_TO_BLANK;
            saveStart = i + 1;
            theOutputHeader.methodM = HttpHeader::METHOD_GET;
            continue;
        }
        if (PARSE_STATE_METHOD_POST == state)
        {
            keyId = PARSE_STATE_METHOD_POST;
            state = PARSE_STATE_SAVE_TO_BLANK;
            saveStart = i + 1;
            theOutputHeader.methodM = HttpHeader::METHOD_POST;
            continue;
        }
        if (PARSE_STATE_METHOD_RSP == state)
        {
            keyId = PARSE_STATE_METHOD_RSP;
            state = PARSE_STATE_SAVE_TO_BLANK;
            saveStart = i + 2;
            theOutputHeader.methodM = HttpHeader::METHOD_RSP;
            continue;
        }

        
        if (httpHeaderDefsM.find(state) != httpHeaderDefsM.end())
        {
            keyId = state;
            state = PARSE_STATE_SAVE_TO_END;
            saveStart = i + 1;
            continue;
        }
         
    }
    //check if it is end
    if (PARSE_STATE_SAVE_TO_END == state)
    {
        if (PARSE_STATE_METHOD_GET == keyId
                || PARSE_STATE_METHOD_POST == keyId)
        {
            theOutputHeader.protocolM = 
                theHeaderStr.substr(saveStart, i - saveStart);
        }
        else
        {
            theOutputHeader.attrVectorM[keyId] = 
                theHeaderStr.substr(saveStart, i - saveStart);
        }
        state = PARSE_STATE_IGNORE_LINE;
    }

    return 0;
}

//-----------------------------------------------------------------------------



