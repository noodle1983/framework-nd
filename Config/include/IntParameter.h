#ifndef INTPARAMETER_H
#define INTPARAMETER_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <list>
#include <string>

namespace Config
{
    class IntParameter
    {
    public:
        IntParameter(const std::string& theName);
        IntParameter(const IntParameter& theIntParam);
        ~IntParameter();

        typedef boost::function<void (const int)> Watcher;
        typedef std::list<Watcher> WatcherList;

        int get();
        int set(const std::string& theValue);
        int set(const int theValue);
        void setRange(const std::string& theRange);
        void setRange(const int theMin, const int theMax);
        void registerWatcher(Watcher& theWatcher);

        const std::string& _getName()const {return nameM;}
        bool  _getCheckRange()const {return checkRangeM;}
        int  _getMinValue()const {return minValueM;}
        int  _getMaxValue()const {return maxValueM;}
        int  _getValue()const {return valueM;}
    private:
        const std::string nameM;
        bool checkRangeM;
        int minValueM;
        int maxValueM;
        int valueM;
        WatcherList changesWatchersM;
        boost::shared_mutex valueMutexM;
        boost::mutex watcherMutexM;
    };
}
#endif /* INTPARAMETER_H */

