#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <string>
#include <boost/function.hpp>
#include <time.h>
#include <event.h>

namespace Utility
{
    typedef boost::function<void (const std::string&)> FileWatcherCallback;
    class FileWatcher 
    {
    public:
        FileWatcher(
                const std::string& thePath, 
                unsigned theSec, 
                FileWatcherCallback theCallback);
        ~FileWatcher();

        int checkFile();
    private:
        std::string         filePathM;
        uint64_t            secM;
        FileWatcherCallback callbackM;
        time_t              lastModTimeM;
        struct event*       timerEventM;
    };
}

#endif /* FILEWATCHER_H */

