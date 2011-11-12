#include <IntParameter.h>
#include "Log.h"

using namespace Config;

//-----------------------------------------------------------------------------

IntParameter::IntParameter(const std::string& theName)
    : nameM(theName)
    , checkRangeM(false)
    , minValueM(0)
    , maxValueM(0)
    , valueM(0)
{
}

//-----------------------------------------------------------------------------

IntParameter::~IntParameter()
{}

//-----------------------------------------------------------------------------

int IntParameter::get()
{
    boost::shared_lock<boost::shared_mutex> lock(valueMutexM);
    return valueM;
}

//-----------------------------------------------------------------------------

int IntParameter::set(const int theValue)
{
    if (theValue == valueM)
        return 0;

    if (!checkRangeM 
            || (checkRangeM && theValue >= minValueM && theValue <= maxValueM))
    {
        {
            boost::unique_lock<boost::shared_mutex> lock(valueMutexM);
            valueM = theValue;
        }
        for (WatcherList::iterator it = changesWatchersM.begin();
                it != changesWatchersM.end(); it++)
        {
            (*it)(theValue);
        }
    }
    else
    {
        ERROR("invalid value: "<< theValue
                << " for " << nameM 
                << "(" << minValueM << "-" << maxValueM << ")");        
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

void IntParameter::setRange(const int theMin, const int theMax)
{
    checkRangeM = true;
    minValueM = theMin;
    maxValueM = theMax;
}

//-----------------------------------------------------------------------------

void IntParameter::registerWatcher(Watcher& theWatcher)
{
    boost::unique_lock<boost::mutex> lock(watcherMutexM);
    changesWatchersM.push_back(theWatcher);
}

//-----------------------------------------------------------------------------



