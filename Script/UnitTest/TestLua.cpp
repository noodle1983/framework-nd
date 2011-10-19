#include "Lua/LuaRunner.h"
#include <LuaObject/Message.h>
#include <iostream>
#include <string>

using namespace Script::Lua;
int tolua_Message_open (lua_State* tolua_S);
int main()
{
    LuaRunner runner;
    Message m;
    m.i = 99;
    m.str = "message.str";
    runner.init();
    runner.loadFile("test.lua");
    runner.regist(&tolua_Message_open);
    runner.callFunc("printHello", "Message", &m);
    runner.callFunc("printHello", "Message", &m);
    runner.callFunc("printHello", "Message", &m);
    runner.fini();
    std::cout << "Message:" << m.i << ":" 
                            << m.str << std::endl;
    return 0;
}

