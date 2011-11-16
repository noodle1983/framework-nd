#ifndef LOGGER_H
#define LOGGER_H

#include "Singleton.hpp"

#include <log4cplus/logger.h>
#include <boost/thread.hpp>

namespace Log
{
    class Logger
    {
    public:
        Logger();
        ~Logger();

        log4cplus::Logger& trafficLogger();
        log4cplus::Logger& configLogger();
        inline void setLogLevel(log4cplus::LogLevel theLogLevel){trafficLoggerM.setLogLevel(theLogLevel);}

    private:
        bool isTrafficLoggerInited;
        log4cplus::Logger trafficLoggerM;
        boost::mutex trafficLoggerMutexM;
        bool isConfigLoggerInited;
        log4cplus::Logger configLoggerM;
        boost::mutex configLoggerMutexM;
    };
    typedef DesignPattern::Singleton<Logger> LoggerSingleton;
}

#endif /* LOGGER_H */

