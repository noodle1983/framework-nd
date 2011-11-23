#ifndef LOG_H
#define LOG_H

#include "Logger.h"

#include <log4cplus/logger.h>

//-----------------------------------------------------------------------------
//trafficLogger
#define LOG_TRACE(msg) \
LOG4CPLUS_TRACE(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define LOG_DEBUG(msg) \
LOG4CPLUS_DEBUG(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define LOG_INFO(msg) \
LOG4CPLUS_INFO(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define LOG_WARN(msg) \
LOG4CPLUS_WARN(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define LOG_ERROR(msg) \
LOG4CPLUS_ERROR(Log::LoggerSingleton::instance()->trafficLogger(), msg)

#define LOG_FATAL(msg) \
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
