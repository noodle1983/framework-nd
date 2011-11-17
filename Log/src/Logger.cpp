#include "Logger.h"
#include "ConfigCenter.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <log4cplus/fileappender.h>

using namespace Log;
using namespace Config;

//-----------------------------------------------------------------------------

Logger::Logger()
    : isTrafficLoggerInited(false)
    , isConfigLoggerInited(false)
{
}

//-----------------------------------------------------------------------------

Logger::~Logger()
{
}

//-----------------------------------------------------------------------------

log4cplus::Logger& Logger::trafficLogger()
{
    if (!isTrafficLoggerInited)
    {
        boost::lock_guard<boost::mutex> lock(trafficLoggerMutexM);
        if (!isTrafficLoggerInited)
        {
            int logLevel = ConfigCenter::instance()->get("log.level", 3);
            logLevel *= 10000;

            std::string logFilename = ConfigCenter::instance()->
                get("log.filename", "trouble_shooting.log");

            int fileSize = ConfigCenter::instance()->get("log.fileSize", 10);
            int fileNum = ConfigCenter::instance()->get("log.fileNum", 10);
            std::string logPattern = ConfigCenter::instance()->
                get("log.pattern", "%D{%y-%m-%d %H:%M:%S.%q} %-5p [%l] %m%n");

            isTrafficLoggerInited = true;

            log4cplus::SharedAppenderPtr append(
                    new log4cplus::RollingFileAppender(logFilename,
                                                       fileSize * 1024 * 1024, // max file size
                                                       fileNum               //max backup index
                                                       ));
            std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(logPattern));
            append->setLayout(layout);
            trafficLoggerM = log4cplus::Logger::getInstance("Traffic");
            trafficLoggerM.addAppender(append);
            trafficLoggerM.setLogLevel(logLevel);
            LOG4CPLUS_DEBUG(trafficLoggerM, "------------------------------------");
        }
    }
    return trafficLoggerM;

}

//-----------------------------------------------------------------------------

log4cplus::Logger& Logger::configLogger()
{
    if (!isConfigLoggerInited)
    {
        boost::lock_guard<boost::mutex> lock(configLoggerMutexM);
        if (!isConfigLoggerInited)
        {
            isConfigLoggerInited = true;
            log4cplus::SharedAppenderPtr append(new log4cplus::RollingFileAppender("config.log",
                                                            10 * 1024 * 1024, // max file size
                                                            10               //max backup index
                                                            ));
            std::string pattern = "%D{%y-%m-%d %H:%M:%S.%q} %-5p [%l] %m%n";
            std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(pattern));
            append->setLayout(layout);
            configLoggerM = log4cplus::Logger::getInstance("Config");
            configLoggerM.addAppender(append);
            configLoggerM.setLogLevel(log4cplus::ALL_LOG_LEVEL);
            LOG4CPLUS_DEBUG(configLoggerM, "------------------------------------");
        }
    }
    return configLoggerM;
}

//-----------------------------------------------------------------------------





