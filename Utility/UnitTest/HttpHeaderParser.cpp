#include "RulesTable.hpp"
#include <string>
#include <map>
#include <assert.h>
#include <utility>

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

	RulesTable2<unsigned char, int, int> rulesTable;
    rulesTable.setDefaultRule(IGNORE_LINE);

    map<string, int> subStates;
    int nextSubState = -1;

    string key = keyPair.second;
    int preState = NONE;
    int curState = NONE;
    for (int i = 1; i < key.length(); i++) 
    {
        string preStr = key.substr(0, i);
        map<string, int>::iterator it = subStates.find(preStr);
        if (it != subStates.end())
        {
            curState = it->second;            
        }
        else
        {
            subStates[preState] = nextSubState;
            curState = nextSubState;
            nextSubState--;
        }
        rulesTable[key[i-1]][preState] = curState;
        preState = curState
    }
    rulesTable[key[key.length()-1]][preState] = keyPair.first;


    {
        int state = NONE;
		int saveStart = -1;
		int keyId = NONE;
        for (int i = 0; i < input.lengty(); i++)
        {
            char ch = input[i];
			if ('0x0A' == ch)
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
				if (' ' == ch && saveStart == (i - 1))
				{
					saveStart = i;
					continue;
				}
                else if ('0x0D' == ch)
                {
					assert(saveStart != -1);
					cout << "key id:" << keyId << ", out:" << input.substr(saveStart, i - saveStart);
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
				if (' ' == ch && saveStart == (i - 1))
				{
					saveStart = i;
					continue;
				}
				else if (' ' == ch)
				{
					assert(saveStart != -1);
					cout << "key id:" << keyId << ", out:" << input.substr(saveStart, i - saveStart);
					state = IGNORE_LINE;
                    continue;
				}
				else
				{
					continue;
				}
			}

			state = subStates.getRule(ch, state);
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

