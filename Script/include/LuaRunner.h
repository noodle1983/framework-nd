#ifndef LUARUNNER_H
#define LUARUNNER_H

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include <string>
#include <boost/shared_ptr.hpp>

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

        int loadFile(const std::string& theFile);
        void registCppType(RegistFunc theRegistFunc);
        int callFunc(
                const std::string& theFunc,
                const std::string& theLuaType,
                void* theArg);
    private:
        lua_State *luaStateM;
    };
    typedef boost::shared_ptr<LuaRunner> LuaRunnerPtr;

    //-----------------------------------------------------------------------------
    inline void LuaRunner::registCppType(RegistFunc theRegistFunc)
    {
        (*theRegistFunc)(luaStateM);
    }
}
}


#endif /* LUARUNNER_H */

