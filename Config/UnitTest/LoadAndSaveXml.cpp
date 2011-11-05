#include "Config.h"
#include <iostream>

using namespace std;
using namespace Config;

int main()
{
    ConfigCenter::loadConfig("LoadAndSaveXml.xml");
    int i = ConfigCenter::instance()->get("log.debuglevel", 0);
    cout << "get log.debuglevel: " << i << endl;
    ConfigCenter::instance()->set("log.debuglevel", i+1);
    ConfigCenter::instance()->saveXml("out_config.xml");
    return 0;
}

