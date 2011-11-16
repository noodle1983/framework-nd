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
        for (WatcherList::iterator it = changesWatchersM.begin();
                it != changesWatchersM.end(); it++)
        {
            (*it)(theValue);
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------

void StringParameter::registerWatcher(Watcher& theWatcher)
{
    boost::unique_lock<boost::mutex> lock(watcherMutexM);
    changesWatchersM.push_back(theWatcher);
}

//-----------------------------------------------------------------------------



