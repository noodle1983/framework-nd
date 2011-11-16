#include "IntParameter.h"
#include "Log.h"

#include <stdio.h>

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

IntParameter::IntParameter(const IntParameter& theIntParam)
    : nameM(theIntParam._getName())
    , checkRangeM(theIntParam._getCheckRange())
    , minValueM(theIntParam._getMinValue())
    , maxValueM(theIntParam._getMaxValue())
    , valueM(theIntParam._getValue())
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

int IntParameter::set(const std::string& theValue)
{
    int value = 0;
    if (0 == sscanf(theValue.c_str(), "%d", &value))
    {
        CFG_ERROR(nameM << "'s value " << theValue << " is invalid.");
        return -1;
    }
    return set(value);
}

//-----------------------------------------------------------------------------

int IntParameter::set(const int theValue)
{
    if (theValue == valueM)
        return 0;

    bool notifyWatcher = false;

    {
        boost::upgrade_lock<boost::shared_mutex> lock(valueMutexM);
        if (!checkRangeM 
                || (checkRangeM && theValue >= minValueM && theValue <= maxValueM))
        {
            {
                boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
                valueM = theValue;
            }
            notifyWatcher = true;
        }
        else
        {
            CFG_ERROR("invalid value: "<< theValue
                    << " for " << nameM 
                    << "(" << minValueM << "-" << maxValueM << ")");        
            return -1;
        }
    }
    if (notifyWatcher)
    {
        boost::unique_lock<boost::mutex> lock(watcherMutexM);
        for (WatcherList::iterator it = changesWatchersM.begin();
                it != changesWatchersM.end(); it++)
        {
            (*it)(theValue);
        }
    }
    CFG_DEBUG("set config[" << nameM << "=" << theValue << "]");
    return 0;
}

//-----------------------------------------------------------------------------

void IntParameter::setRange(const std::string& theRange)
{
    if (theRange.empty())
        return;

    int minValue = 0;
    int maxValue = 0;
    if (0 == sscanf(theRange.c_str(), "%d-%d", &minValue, &maxValue))
    {
        CFG_ERROR(nameM << "'s range " << theRange << " is invalid.");
        return;
    }
    if (minValue < maxValue)
    {
        return setRange(minValue, maxValue);
    }
    else
    {
        return setRange(maxValue, minValue);
    }

}

//-----------------------------------------------------------------------------

void IntParameter::setRange(const int theMin, const int theMax)
{
    boost::unique_lock<boost::shared_mutex> lock(valueMutexM);
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



