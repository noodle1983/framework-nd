#include "ConfigCenter.h"
#include <iostream>
using namespace std;

int main()
{
    int test0 = Config::ConfigCenter::instance()->get("test.test0", -1);
    cout << "test0:" << test0 << endl;
    int test1 = Config::ConfigCenter::instance()->get("test.test1", -1);
    cout << "test1:" << test1 << endl;
    int notexist = Config::ConfigCenter::instance()->get("notexist", -1);
    cout << "notexist:" << notexist << endl;
    return 0;
}

