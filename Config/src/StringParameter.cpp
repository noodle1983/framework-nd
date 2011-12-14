#include "StringParameter.h"
#include "Log.h"

#include <stdio.h>

using namespace Config;

//-----------------------------------------------------------------------------

StringParameter::StringParameter(const std::string& theName)
    : nameM(theName)
{
}

//-----------------------------------------------------------------------------

StringParameter::StringParameter(const StringParameter& theStringParam)
    : nameM(theStringParam._getName())
    , valueM(theStringParam._getValue())
{
}
//-----------------------------------------------------------------------------

StringParameter::~StringParameter()
{}

//-----------------------------------------------------------------------------

const std::string StringParameter::get()
{
    boost::shared_lock<boost::shared_mutex> lock(valueMutexM);
    return valueM;
}

//-----------------------------------------------------------------------------

int StringParameter::set(const std::string& theValue)
{
    if (theValue == valueM)
        return 0;

    {
        boost::unique_lock<boost::shared_mutex> lock(valueMutexM);
        valueM = theValue;
    }
    {
        boost::unique_lock<boost::mutex> lock(watcherMutexM);
        for (WatcherMap::iterator it = changesWatchersM.begin();
                it != changesWatchersM.end(); it++)
        {
            (it->second)(theValue);
        }
    }
    CFG_DEBUG("set config[" << nameM << "=" << theValue << "]");
    return 0;
}

//-----------------------------------------------------------------------------

void StringParameter::registerWatcher(void *theKey, Watcher& theWatcher)
{
    boost::unique_lock<boost::mutex> lock(watcherMutexM);
    changesWatchersM[theKey] = theWatcher;
}

//-----------------------------------------------------------------------------

void StringParameter::unregisterWatcher(void *theKey)
{
    boost::unique_lock<boost::mutex> lock(watcherMutexM);
    changesWatchersM.erase(theKey);
}

//-----------------------------------------------------------------------------




