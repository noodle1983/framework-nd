#ifndef LUARUNNER_H
#define LUARUNNER_H

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include <string>

typedef int (*RegistFunc)(lua_State* theState);

namespace Script
{
namespace Lua
{
    class LuaRunner
    {
    public:
        LuaRunner();
        ~LuaRunner();

        int init();
        void fini();

        void regist(RegistFunc theRegistFunc);
        int loadFile(const std::string& theFile);
        int callFunc(
                const std::string& theFunc,
                const std::string& theLuaType,
                void* theArg);
    private:
        lua_State *luaStateM;
    };

    //-----------------------------------------------------------------------------
    inline void LuaRunner::regist(RegistFunc theRegistFunc)
    {
        (*theRegistFunc)(luaStateM);
    }
}
}


#endif /* LUARUNNER_H */

