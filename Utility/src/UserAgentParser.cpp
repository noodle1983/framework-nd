#include "UserAgentParser.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace Utility;
using namespace std;

typedef std::map<std::string, int> Key2StateNodeMap;

//-----------------------------------------------------------------------------
UserAgentParser::UserAgentParser()
{
    init();
}

//-----------------------------------------------------------------------------

UserAgentParser::~UserAgentParser()
{
}

//-----------------------------------------------------------------------------

void UserAgentParser::initRuleDef(std::vector<RuleDef>& theRuleDefVector)
{
    ifstream phoneInfoFile("PhoneInfo.txt");
    assert(phoneInfoFile.is_open());

    char ruleStr[256] = {0};
    while(phoneInfoFile.getline(ruleStr, sizeof(ruleStr)))
    {
        std::vector<std::string> attrVector;
        boost::split(attrVector, ruleStr, boost::is_any_of("\t"));
        assert(attrVector.size() == 12);

        RuleDef ruleDef;
  //      ruleDef.phoneInfoM.idM = atoi(attrVector[0].c_str());
        ruleDef.primaryKeyM = boost::to_upper_copy(attrVector[1]);
        boost::trim(ruleDef.primaryKeyM);
        ruleDef.subKeyM = boost::to_upper_copy(attrVector[2]);
        boost::trim(ruleDef.subKeyM);

        ruleDef.phoneInfoM.renderIdM = atoi(attrVector[3].c_str()); 
        ruleDef.phoneInfoM.modelIdM = atoi(attrVector[4].c_str());
        ruleDef.phoneInfoM.osIdM = atoi(attrVector[5].c_str());
        ruleDef.phoneInfoM.supportEdgeM = atoi(attrVector[6].c_str());
        ruleDef.phoneInfoM.supportTdM = atoi(attrVector[7].c_str());
        ruleDef.phoneInfoM.supportWcdmaM = atoi(attrVector[8].c_str());
        ruleDef.phoneInfoM.supportHspaM = atoi(attrVector[9].c_str());
        ruleDef.phoneInfoM.supportLteM = atoi(attrVector[10].c_str());
        ruleDef.phoneInfoM.supportCdmaM = atoi(attrVector[11].c_str());

        if (!ruleDef.primaryKeyM.empty())
        {
            theRuleDefVector.push_back(ruleDef);
        }
    }
    phoneInfoFile.close(); 

}

//-----------------------------------------------------------------------------

void UserAgentParser::init()
{
    //get Rules Definination
    std::vector<RuleDef> ruleDefVectorM;
    initRuleDef(ruleDefVectorM);

    //init Rules Table
    rulesTableM.clear();
    rulesTableM.setDefaultRule(PARSE_STATE_NONE);

    //key to index
    String2StateMap primaryString2StateMap;
    int primaryNextUncompletedState = PARSE_STATE_UNCOMPLETE_KEY_START;
    std::vector<String2StateMap> subString2StateMaps;
    std::vector<int> subNextUncompletedStates;
    
    //
    stateNodeVectorM.clear();
    int nextStateNodeIndex = 0;

    Key2StateNodeMap key2StateNodeMap;

    std::vector<RuleDef>::iterator it = ruleDefVectorM.begin();
    for (; it != ruleDefVectorM.end(); it++)
    {
        int stateNodeIndex = -1;
        //get State Node Index
        {
            Key2StateNodeMap::iterator stateNodeIt = 
                key2StateNodeMap.find(it->primaryKeyM);
            if (stateNodeIt != key2StateNodeMap.end()) 
            {
                stateNodeIndex = stateNodeIt->second; 
            }
            else
            {
                stateNodeIndex = nextStateNodeIndex++;       
                key2StateNodeMap[it->primaryKeyM] = stateNodeIndex;

                //init vector
                stateNodeVectorM.push_back(StateNode());
                subString2StateMaps.push_back(String2StateMap());
                subNextUncompletedStates.push_back(PARSE_STATE_UNCOMPLETE_KEY_START);
            }
        }

        //init primaryKey
        StateNode* stateNode = &stateNodeVectorM[stateNodeIndex]; 
        if (stateNode->localKeyM.empty())
        {
            stateNode->localKeyM = it->primaryKeyM;
            initStateForAString(it->primaryKeyM, stateNodeIndex, 
                rulesTableM, primaryString2StateMap, primaryNextUncompletedState);
            stateNode->subRulesTableM.setDefaultRule(PARSE_STATE_NONE);
        }
        //init subKey
        String2StateMap* subString2StateMap = &subString2StateMaps[stateNodeIndex];
        int* subNextUncompletedState = &subNextUncompletedStates[stateNodeIndex];
        if (it->subKeyM.empty())
        {
            stateNode->isDoneM = true;
            stateNode->localPhoneInfoM = it->phoneInfoM;
        }
        else
        {
            /*
            RulesTable subRulesTableM;
            KeyVector subKeyVectorM;
            PhoneInfoVector subPhoneInfoVectorM;
            */
            //subKeyStateIndex is the state, and also the index of the vector
            int subKeyStateIndex = stateNode->subKeyVectorM.size();
            stateNode->subKeyVectorM.push_back(it->subKeyM);
            stateNode->subPhoneInfoVectorM.push_back(it->phoneInfoM);
            initStateForAString(it->subKeyM, subKeyStateIndex, 
                stateNode->subRulesTableM, 
                *subString2StateMap, 
                *subNextUncompletedState);
        }


    }
}

void UserAgentParser::initStateForAString(
        const std::string& theKey,
        const int theKeyState,
        RulesTable& theRulesTable,
        String2StateMap& theString2StateMap,
        int& theNextUncompletedState)
{
    int preState = PARSE_STATE_NONE;
    int curState = PARSE_STATE_NONE;

    for (unsigned i = 1; i < theKey.length(); i++) 
    {
        string preStr = theKey.substr(0, i);
        String2StateMap::iterator it = theString2StateMap.find(preStr);
        if (it != theString2StateMap.end())
        {
            curState = it->second;            
        }
        else
        {
            theString2StateMap[preStr] = theNextUncompletedState;
            curState = theNextUncompletedState;
            theNextUncompletedState--;
        }
        theRulesTable.setRule(theKey.at(i-1), preState, curState);
        preState = curState;
    }
    theString2StateMap[theKey] = theKeyState;
    theRulesTable.setRule(theKey.at(theKey.length()-1), preState, theKeyState);
}


//-----------------------------------------------------------------------------

bool UserAgentParser::match(
                const char* theInputStr,
                const unsigned theLen,
                PhoneInfo& thePhoneInfo)
{
    int state = PARSE_STATE_NONE;
    int matchedPrimaryKey = -1;
    bool matchedSubKey = false; //for longest match
    int nodeSize = stateNodeVectorM.size();
    for (unsigned i = 0; i < theLen; i++)
    {
        char ch = ::toupper(theInputStr[i]);
        if ('\n' == ch || '\r' == ch || 0 == ch)
            break;

        if (matchedPrimaryKey < 0)
        {
            state = rulesTableM.getRule(ch, state);
            if (state >= 0)
            {
                assert (state <= nodeSize);
                matchedPrimaryKey = state;
                if (stateNodeVectorM[state].subKeyVectorM.empty())
                {
                    thePhoneInfo = stateNodeVectorM[state].localPhoneInfoM;
                    return true;
                }
                state = PARSE_STATE_NONE;
            }
        }
        else
        {
            StateNode* stateNode = &stateNodeVectorM[matchedPrimaryKey]; 
            state = stateNode->subRulesTableM.getRule(ch, state);
            if (state >= 0)
            {
                assert(unsigned(state) <= stateNode->subPhoneInfoVectorM.size());
                thePhoneInfo = stateNode->subPhoneInfoVectorM[state];
                //we don't return in order to match the longest str
                //and set the isDoneM to false to bapass the reset
                //return true;
                matchedSubKey = true;
            }
            else if (matchedSubKey && PARSE_STATE_NONE == state)
            {
                return true;
            }
        }
    }
    if (matchedSubKey)
        return true;

    if (matchedPrimaryKey >= 0 && stateNodeVectorM[matchedPrimaryKey].isDoneM)
    {
        thePhoneInfo = stateNodeVectorM[matchedPrimaryKey].localPhoneInfoM;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------



