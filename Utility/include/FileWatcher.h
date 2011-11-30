#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <string>
#include <boost/function.hpp>
#include <time.h>
#include <event.h>

namespace Utility
{
    typedef void (*FileWatcherCallback)(std::string theFileName);
    class FileWatcher 
    {
    public:
        FileWatcher(
                const std::string& thePath, 
                unsigned theSec, 
                FileWatcherCallback theCallback);
        ~FileWatcher();

        void checkFile();
    private:
        std::string         filePathM;
        uint64_t            secM;
        FileWatcherCallback callbackM;
        time_t              lastModTimeM;
        struct event*       timerEventM;
    };
}

#endif /* FILEWATCHER_H */

