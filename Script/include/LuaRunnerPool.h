#ifndef LUARUNNERPOOL_H
#define LUARUNNERPOOL_H

#include "LuaRunner.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include <string>
#include <vector>
#include <boost/thread.hpp>

namespace Script
{
namespace Lua
{
    typedef std::vector<RegistFunc> ModRegFunsArray;
    typedef std::vector<LuaRunnerPtr> RunnerArray;

    class LuaRunnerPool
    {
    public:
        LuaRunnerPool();
        ~LuaRunnerPool();

        //must setFile/registCppType before init
        void setFile(const std::string& theFile);
        void registCppType(RegistFunc theRegistFunc);
        int init(const int theCount);

        int reset();
        void fini();

        int callFunc(
                const uint64_t theRunnerId,
                const std::string& theFunc,
                const std::string& theLuaType,
                void* theArg);
    private:
        bool isRunnerCntValid()
        {return runnerCountM > 1 && runnerCountM < 100;}

        int runnerCountM;
        std::string scriptFileM;
        ModRegFunsArray modRegFunsM;
        RunnerArray runnerArrayM;
        boost::shared_mutex runnerArrayMutexM;
    };
}
}


#endif /* LUARUNNERPOOL_H */

