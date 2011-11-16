#ifndef LOG_H
#define LOG_H

#include "Logger.h"

#include <log4cplus/logger.h>

//-----------------------------------------------------------------------------
//trafficLogger
#define TRACE(msg) \
LOG4CPLUS_TRACE(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define DEBUG(msg) \
LOG4CPLUS_DEBUG(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define INFO(msg) \
LOG4CPLUS_INFO(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define WARN(msg) \
LOG4CPLUS_WARN(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define ERROR(msg) \
LOG4CPLUS_ERROR(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define FATAL(msg) \
LOG4CPLUS_FATAL(Log::LoggerSingleton::instance()->trafficLogger(), msg)

//-----------------------------------------------------------------------------
//configLogger
#define CFG_TRACE(msg) \
LOG4CPLUS_TRACE(Log::LoggerSingleton::instance()->configLogger(), msg)

#define CFG_DEBUG(msg) \
LOG4CPLUS_DEBUG(Log::LoggerSingleton::instance()->configLogger(), msg)

#define CFG_INFO(msg) \
LOG4CPLUS_INFO(Log::LoggerSingleton::instance()->configLogger(), msg)

#define CFG_WARN(msg) \
LOG4CPLUS_WARN(Log::LoggerSingleton::instance()->configLogger(), msg)

#define CFG_ERROR(msg) \
LOG4CPLUS_ERROR(Log::LoggerSingleton::instance()->configLogger(), msg)

#define CFT_FATAL(msg) \
LOG4CPLUS_FATAL(Log::LoggerSingleton::instance()->configLogger(), msg)
#endif /* LOG_H */
