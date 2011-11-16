#include "LuaRunnerPool.h"
#include "Log.h"
#include <tolua++.h>

using namespace Script::Lua;

//-----------------------------------------------------------------------------

LuaRunnerPool::LuaRunnerPool()
    :runnerCountM(0)
{
}

//-----------------------------------------------------------------------------

LuaRunnerPool::~LuaRunnerPool()
{
    fini();
}

//-----------------------------------------------------------------------------

void LuaRunnerPool::setFile(const std::string& theFile)
{
    scriptFileM = theFile;
}

//-----------------------------------------------------------------------------

void LuaRunnerPool::registCppType(RegistFunc theRegistFunc)
{
    modRegFunsM.push_back(theRegistFunc);
}

//-----------------------------------------------------------------------------

int LuaRunnerPool::init(const int theCnt)
{
    runnerCountM = theCnt;
    if (!isRunnerCntValid())
    {
        ERROR("invalid LuaRunner Count:" << theCnt
                << ". please set it to the same as thread count!");
        exit(-1);
    }
    return reset();

}

int LuaRunnerPool::reset()
{
    RunnerArray tmpRunnerArray;
    for (int i = 0; i < runnerCountM; i++)
    {
        LuaRunnerPtr luaRunner(new LuaRunner());
        if (0 != luaRunner->init() 
                || 0 != luaRunner->loadFile(scriptFileM))
        {
            ERROR("can't load lua script file:" << scriptFileM);
            exit(-1);
        }
        ModRegFunsArray::iterator it = modRegFunsM.begin();
        for (; it != modRegFunsM.end(); it++)
        {
            luaRunner->registCppType(*it);
        }
        tmpRunnerArray.push_back(luaRunner);
    }

    boost::unique_lock<boost::shared_mutex> lock(runnerArrayMutexM);
    runnerArrayM = tmpRunnerArray;
    return 0;
}

//-----------------------------------------------------------------------------

void LuaRunnerPool::fini()
{
    boost::unique_lock<boost::shared_mutex> lock(runnerArrayMutexM);
    RunnerArray::iterator it = runnerArrayM.begin();
    for (; it != runnerArrayM.end(); it++)
    {
        (*it)->fini();
    }
    runnerArrayM.clear();

}

//-----------------------------------------------------------------------------

int LuaRunnerPool::callFunc(
        const uint64_t theRunnerId,
        const std::string& theFunc,
        const std::string& theLuaType,
        void* theArg)
{
    LuaRunnerPtr luaRunner;
    {
        boost::shared_lock<boost::shared_mutex> lock(runnerArrayMutexM);
        luaRunner = runnerArrayM[theRunnerId % runnerCountM];
    }
    return luaRunner->callFunc(
            theFunc,
            theLuaType,
            theArg);
}

//-----------------------------------------------------------------------------

