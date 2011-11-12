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
        ~IntParameter();

        typedef boost::function<void (const int)> Watcher;
        typedef std::list<Watcher> WatcherList;

        int get();
        int set(const int theValue);
        void setRange(const int theMin, const int theMax);
        void registerWatcher(Watcher& theWatcher);

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

