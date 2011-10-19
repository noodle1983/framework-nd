#include "Log.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <log4cplus/fileappender.h>

using namespace Log;

//-----------------------------------------------------------------------------

static boost::mutex logInstanceMutex;
static boost::shared_ptr<Logger> logInstanceReleaser;
Logger* Logger::logM = NULL;

//-----------------------------------------------------------------------------

Logger* Logger::instance()
{
    if (NULL == logM)
    {
        boost::lock_guard<boost::mutex> lock(logInstanceMutex);
        if (NULL == logM)
        {
            logM = new Logger();
            logInstanceReleaser.reset(logM);
            logM->init();
        }
    }
    return logM;
}

//-----------------------------------------------------------------------------

Logger::Logger()
{
}

//-----------------------------------------------------------------------------

Logger::~Logger()
{
}

//-----------------------------------------------------------------------------

void Logger::init()
{
    log4cplus::SharedAppenderPtr append(new log4cplus::RollingFileAppender("trouble_shooting.log",
                                                    10 * 1024 * 1024, // max file size
                                                    10               //max backup index
                                                    ));
    std::string pattern = "%d{%m/%d/%y %H:%M:%S} %t %p [%l] %m%n";
    std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(pattern));
    append->setLayout(layout);
    logImplM = log4cplus::Logger::getInstance("Logger");
    logImplM.addAppender(append);    
    logImplM.setLogLevel(log4cplus::ALL_LOG_LEVEL);
}

//-----------------------------------------------------------------------------





