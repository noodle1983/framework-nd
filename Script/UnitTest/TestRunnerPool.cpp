#include "LuaRunnerPool.h"
#include <LuaObject/Message.h>
#include <iostream>
#include <string>

using namespace Script::Lua;
int tolua_Message_open (lua_State* tolua_S);
int main()
{
    Message m;
    m.i = 99;
    m.str = "message.str";

    LuaRunnerPool runner;
    runner.setFile("test.lua");
    runner.registCppType(&tolua_Message_open);
    runner.init(10);

    runner.callFunc(1, "printHello", "Message", &m);
    runner.callFunc(2, "printHello", "Message", &m);
    runner.callFunc(3, "printHello", "Message", &m);
    runner.fini();
    std::cout << "Message:" << m.i << ":"
                            << m.str << std::endl;
    return 0;
}

