#include "LuaRunner.h"
#include "Log.h"
#include <tolua++.h>

using namespace Script::Lua;

//-----------------------------------------------------------------------------

LuaRunner::LuaRunner()
    :luaStateM(NULL)
{
}

//-----------------------------------------------------------------------------

LuaRunner::~LuaRunner()
{
    fini();
}

//-----------------------------------------------------------------------------

int LuaRunner::init()
{
    luaStateM = luaL_newstate();
    if (NULL == luaStateM)
    {
        LOG_ERROR("failed to new a lua state!");
        return -1;
    }
    luaL_openlibs(luaStateM);
    return 0;
}

//-----------------------------------------------------------------------------

void LuaRunner::fini()
{
    if (luaStateM)
    {
        lua_close(luaStateM);
        luaStateM = NULL;
    }

}

//-----------------------------------------------------------------------------

int LuaRunner::loadFile(const std::string& theFile)
{
    int error = luaL_dofile(luaStateM, theFile.c_str());
    if (error)
    {
        LOG_ERROR("failed to load lua file:" << theFile
                << ", errstr:" << lua_tostring(luaStateM, -1));
        lua_pop(luaStateM, 1);
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

int LuaRunner::callFunc(
        const std::string& theFunc,
        const std::string& theLuaType,
        void* theArg)
{
    lua_getglobal(luaStateM, theFunc.c_str());
    tolua_pushusertype(luaStateM, (void*)theArg, theLuaType.c_str());
    if( lua_pcall(luaStateM, 1, 0, 0) != 0 )
    {
        LOG_WARN("failed to call lua function:" << theFunc
                << ", errstr:" << lua_tostring(luaStateM, -1));
        lua_pop(luaStateM, 1);
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

