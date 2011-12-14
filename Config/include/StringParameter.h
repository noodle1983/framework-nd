#ifndef STRINGPARAMETER_H
#define STRINGPARAMETER_H

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <map>
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
        typedef std::map<void*, Watcher> WatcherMap;

        const std::string get();
        int set(const std::string& theValue);
        void registerWatcher(void *theKey, Watcher& theWatcher);
        void unregisterWatcher(void *theKey);

        const std::string& _getName()const {return nameM;}
        const std::string& _getValue()const {return valueM;}
    private:
        const std::string nameM;
        std::string valueM;
        WatcherMap changesWatchersM;
        boost::shared_mutex valueMutexM;
        boost::mutex watcherMutexM;
    };
}
#endif /* STRINGPARAMETER_H */

