#include "RulesTable.hpp"
#include <string>
#include <map>
#include <assert.h>
#include <utility>

#include <iostream>

using namespace std;

enum ParseState
{
    NONE = 0,
    USER_AGENT,

	//special purpose
    IGNORE_LINE,
	SAVE_TO_END,
	SAVE_TO_BLANK,
    STATE_CNT
};

int main ()
{
    string input = "User-Agent: Firfox\r\n";
    std::pair<ParseState, string> keyPair = make_pair(USER_AGENT, "User-Agent:"); 

	RulesTable2<char, int, int> rulesTable;
    rulesTable.setDefaultRule(IGNORE_LINE);

    map<string, int> subStates;
    int nextSubState = -1;

    string key = keyPair.second;
    int preState = NONE;
    int curState = NONE;
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
        rulesTable.setRule(key.at(i-1), preState, curState);
        preState = curState;
    }
    rulesTable.setRule(key.at(key.length()-1), preState, keyPair.first);


    {
        int state = NONE;
		int saveStart = -1;
		int keyId = NONE;
        for (unsigned i = 0; i < input.length(); i++)
        {
            char ch = input[i];
			if ('\n' == ch)
			{
				state = NONE;
				saveStart = -1;
				continue;
			}

            if (IGNORE_LINE == state) 
            {
				continue;
            }
			if (SAVE_TO_END == state)
			{
				if (' ' == ch && saveStart == (int)(i - 1))
				{
					saveStart = i;
					continue;
				}
                else if ('\r' == ch)
                {
					assert(saveStart != -1);
					std::cout << "key id:" << keyId << ", out:" << input.substr(saveStart, i - saveStart);
					state = IGNORE_LINE;
                    continue;
                }
                else
                {
                    continue;
                }

			}
			if (SAVE_TO_BLANK == state)
			{
				if (' ' == ch && saveStart == (int)(i - 1))
				{
					saveStart = i;
					continue;
				}
				else if (' ' == ch)
				{
					assert(saveStart != -1);
					std::cout << "key id:" << keyId << ", out:" << input.substr(saveStart, i - saveStart);
					state = IGNORE_LINE;
                    continue;
				}
				else
				{
					continue;
				}
			}

			state = rulesTable.getRule(ch, state);
			if (USER_AGENT == state)
			{
				keyId = USER_AGENT;
				state = SAVE_TO_END;
				saveStart = i + 1;
				continue;
			}

        }
    }

    return 0;
}

