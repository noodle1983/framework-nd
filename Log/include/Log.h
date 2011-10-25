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
LOG4CPLUS_TRACE(Log::Logger::instance()->logger(), msg)

#define DEBUG(msg) \
LOG4CPLUS_DEBUG(Log::Logger::instance()->logger(), msg)

#define INFO(msg) \
LOG4CPLUS_INFO(Log::Logger::instance()->logger(), msg)

#define WARN(msg) \
LOG4CPLUS_WARN(Log::Logger::instance()->logger(), msg)

#define ERROR(msg) \
LOG4CPLUS_ERROR(Log::Logger::instance()->logger(), msg)

#define FATAL(msg) \
LOG4CPLUS_FATAL(Log::Logger::instance()->logger(), msg)

#endif /* LOG_H */
