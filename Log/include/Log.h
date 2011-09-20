#ifndef LOG_H
#define LOG_H

#include <log4cplus/logger.h>

namespace Log
{
    class Logger
    {
    public:    
        ~Logger();

        static Logger* instance();
        inline log4cplus::Logger& logger(){return logImplM;}
        inline void setLogLevel(log4cplus::LogLevel theLogLevel){logImplM.setLogLevel(theLogLevel);}

    private:
        Logger();
        void init();

        log4cplus::Logger logImplM;
        static Logger* logM;
    };
}

#define TRACE(msg) \
LOG4CPLUS_TRACE(Log::instance()->logImplM, msg)

#define DEBUG(msg) \
LOG4CPLUS_DEBUG(Log::instance()->logImplM, msg)

#define INFO(msg) \
LOG4CPLUS_INFO(Log::instance()->logImplM, msg)

#define WARN(msg) \
LOG4CPLUS_WARN(Log::instance()->logImplM, msg)

#define ERROR(msg) \
LOG4CPLUS_ERROR(Log::instance()->logImplM, msg)

#define FATAL(msg) \
LOG4CPLUS_FATAL(Log::instance()->logImplM, msg)

#endif /* LOG_H */
