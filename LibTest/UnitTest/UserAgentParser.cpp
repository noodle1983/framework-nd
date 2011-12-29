#include "UserAgentParser.h"

#include <iostream>

using namespace Utility;
using namespace std;

int main()
{
    UserAgentParser parser;
    PhoneInfo phoneInfo;
    string userAgentStr = "Motorola MB525";
    if (parser.match(userAgentStr.c_str(), userAgentStr.length(), phoneInfo))
    { 
        cout << "render: " << phoneInfo.renderM
            << ", model: " << phoneInfo.modelM
            << ", os: " << phoneInfo.osM << endl;
    }
    else
    {
        cout << "can't parse ua:" << userAgentStr << endl;
    }
}

