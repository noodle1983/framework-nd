#include "Config.h"
#include <iostream>

using namespace std;
using namespace Config;

int main()
{
    int i = ConfigCenter::instance()->get("log.debuglevel", 0);
    cout << "get log.debuglevel: " << i << endl;
    ConfigCenter::instance()->set("log.debuglevel", i+1);
    ConfigCenter::instance()->saveXml("config.xml");
    return 0;
}

