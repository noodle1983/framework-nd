#ifndef STRINGPARAMETER_H
#define STRINGPARAMETER_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <list>
#include <string>

namespace Config
{
    class StringParameter
    {
    public:
        StringParameter(const std::string& theName);
        StringParameter(const StringParameter& theStringParam);
        ~StringParameter();

        typedef boost::function<void (const std::string&)> Watcher;
        typedef std::list<Watcher> WatcherList;

        const std::string get();
        int set(const std::string& theValue);
        void registerWatcher(Watcher& theWatcher);

        const std::string& _getName()const {return nameM;}
        const std::string& _getValue()const {return valueM;}
    private:
        const std::string nameM;
        std::string valueM;
        WatcherList changesWatchersM;
        boost::shared_mutex valueMutexM;
        boost::mutex watcherMutexM;
    };
}
#endif /* STRINGPARAMETER_H */

