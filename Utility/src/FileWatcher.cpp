#include "FileWatcher.h"
#include "Log.h"
#include "Reactor.h"
#include "BoostProcessor.h"

#include <sys/stat.h>
#include <unistd.h>
#include <boost/bind.hpp>


using namespace Utility;

//-----------------------------------------------------------------------------

void on_filewatcher_timeout(int theFd, short theEvt, void *theArg)
{
    FileWatcher* theWatcher = (FileWatcher*) theArg;
    theWatcher->checkFile();
}

//-----------------------------------------------------------------------------

FileWatcher::FileWatcher(
        const std::string& thePath, 
        unsigned theSec, 
        FileWatcherCallback theCallback)
    : filePathM(thePath)
    , secM(theSec)
    , callbackM(theCallback)
    , lastModTimeM(0)
    , timerEventM(NULL)
{
    struct stat fileStat;
    if (0 == stat(filePathM.c_str(), &fileStat))
    {
        lastModTimeM = fileStat.st_mtime;
    }
    else
    {
        CFG_WARN("failed to stat file:" << filePathM
                << ". errno:" << errno);
    }

    timerEventM = Net::Reactor::Reactor::instance()->newPersistTimer(
                            on_filewatcher_timeout, 
                            this); 
    struct timeval tv;
    tv.tv_sec = (secM == 0 ? 5 : secM); 
    tv.tv_usec = 0;
    event_add(timerEventM, &tv);
}

//-----------------------------------------------------------------------------

FileWatcher::~FileWatcher()
{
    if (timerEventM)
    {
        Net::Reactor::Reactor::instance()->delEvent(timerEventM);
        timerEventM = NULL;
    }
}

//-----------------------------------------------------------------------------

int FileWatcher::checkFile()
{
    struct stat fileStat;
    if (0 == stat(filePathM.c_str(), &fileStat))
    {
        if (lastModTimeM != fileStat.st_mtime)
        {
            Processor::BoostProcessor::manInstance()->process(
                    (uint64_t)timerEventM, 
                    new Processor::Job(boost::bind(callbackM, filePathM)));
            lastModTimeM = fileStat.st_mtime;
            CFG_DEBUG("reload file:" << filePathM);
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

