#include "UserAgentParser.h"

#include <iostream>

using namespace Utility;
using namespace std;

int main()
{
    UserAgentParser parser;
    PhoneInfo phoneInfo;
    string userAgentStr = "Android_CellLocation_1.0;000000000000001;;htccn_chs;HTC Wildfire S;8";
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

