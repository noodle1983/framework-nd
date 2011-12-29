#ifndef USERAGENTPARSER_H
#define USERAGENTPARSER_H

#include "RulesTable.hpp"

#include <map>
#include <vector>
#include <string>

namespace Utility
{
    struct PhoneInfo
    {
        PhoneInfo()
            : idM(-1)
            , terminalTypeM(0)
        {}
        ~PhoneInfo()
        {}
        int idM;
        std::string renderM;
        std::string modelM;
        std::string osM;
        int terminalTypeM;
    };

    class UserAgentParser
    {
    public: 
        struct RuleDef
        {
            std::string primaryKeyM;
            std::string subKeyM;
            PhoneInfo phoneInfoM;
        };
        typedef std::vector<PhoneInfo> PhoneInfoVector;
        typedef std::vector<std::string> KeyVector;

        typedef RulesTable2<char, int, int> RulesTable;
        struct StateNode
        {
            StateNode()
                : isDoneM (false)
            {}
            ~StateNode(){}
            bool isDoneM;
            std::string localKeyM;
            PhoneInfo localPhoneInfoM; //if there is no subKey

            RulesTable subRulesTableM;
            KeyVector subKeyVectorM;
            PhoneInfoVector subPhoneInfoVectorM;
        };
        typedef std::vector<StateNode> StateNodeVector;
        typedef std::map<std::string, int> String2StateMap;

        enum ParseState
        {
            /* 0~max matched key state/index */
            PARSE_STATE_NONE = -1,
            PARSE_STATE_UNCOMPLETE_KEY_START = -2
            /* -max~-2 uncompleted key state*/
        };
    public: 
        UserAgentParser();
        ~UserAgentParser();
        
        void init();
        void initRuleDef(std::vector<RuleDef>& theRuleDefVector);
        void initStateForAString(
                const std::string& theKey,
                const int theKeyState,
                RulesTable& theRulesTable,
                String2StateMap& theString2StateMap,
                int& theNextUncompletedState);
        bool match(
                const char* theInputStr,
                const unsigned theLen,
                PhoneInfo& thePhoneInfo);
    private:
        StateNodeVector stateNodeVectorM;
        RulesTable rulesTableM;
    };
}


#endif /* USERAGENTPARSER_H */

