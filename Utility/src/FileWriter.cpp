#include "FileWriter.h"

#include <unistd.h>
#include <Log.h>
#include <ConfigCenter.h>
#include <DirCreator.h>

using namespace Utility;
using namespace Config;

//-----------------------------------------------------------------------------

FileWriter::FileWriter(
        Processor::BoostProcessor* theProcessor,
        const std::string& theModelName, 
        const int64_t theIndex, 
        const std::string& theHeaderLine)
    : headerLineM(theHeaderLine)
    , modelNameM(theModelName)
    , indexM(theIndex)
    , switchTimeM(0)
    , curWriteTimeM(0)
    , processorM(theProcessor)
    , timerHandlerM(NULL)
{
    switchTimeM = ConfigCenter::instance()->get(modelNameM+ ".swtTime", 3600);
    outDirM = ConfigCenter::instance()->get(modelNameM+ ".outDir", "./OutputFiles");
    createDir(outDirM);
}

//-----------------------------------------------------------------------------

FileWriter::FileWriter(
        const std::string& theModelName, 
        const int64_t theIndex,
        const std::string& theHeaderLine)
    : headerLineM(theHeaderLine)
    , modelNameM(theModelName)
    , indexM(theIndex)
    , switchTimeM(0)
    , curWriteTimeM(0)
    , processorM(NULL)
    , timerHandlerM(NULL)
{
    switchTimeM = ConfigCenter::instance()->get(modelNameM+ ".swtTime", 3600);
    outDirM = ConfigCenter::instance()->get(modelNameM+ ".outDir", "./OutputFiles");
    createDir(outDirM);
}

//-----------------------------------------------------------------------------

FileWriter::~FileWriter()
{
    closeFile();
    if (timerHandlerM)
    {
        processorM->cancelLocalTimer(indexM, timerHandlerM);
    }
}

//-----------------------------------------------------------------------------

void FileWriter::write(const std::string& theContent)
{
    startTimer();
    switchFile();
    fileStreamM << theContent;
}

//-----------------------------------------------------------------------------

void FileWriter::startTimer()
{
    if (NULL == processorM)
        return;
    
    if (NULL != timerHandlerM)
    {
        processorM->cancelLocalTimer(
            indexM, timerHandlerM);
    }

    time_t now = time((time_t*)NULL);
    struct timeval tv;
    tv.tv_sec = switchTimeM - now % switchTimeM;
    tv.tv_usec = 0;
    timerHandlerM = processorM->addLocalTimer(
            indexM, tv, FileWriter::onTimeout, (void*)this);
}

//-----------------------------------------------------------------------------

void FileWriter::onTimeout(int theFd, short theEvt, void *theArg)
{
    FileWriter* writer = (FileWriter*) theArg;
    writer->timerHandlerM = NULL;
    writer->switchFile();
    writer->startTimer();
}

//-----------------------------------------------------------------------------


void FileWriter::switchFile()
{
    time_t now = time((time_t*)NULL);
    unsigned newWriteTime = now / switchTimeM * switchTimeM;
    if (newWriteTime != curWriteTimeM)
    {
        closeFile();

        curWriteTimeM = newWriteTime;
        struct tm tmTime;
        localtime_r(&now, &tmTime);
        char filename[128] = {0};
        snprintf(filename, sizeof(filename), 
                "%04d%02d%02d%02d%02d%02d_%02d__%s_%04d%02d%02d",
                tmTime.tm_year + 1900,
                tmTime.tm_mon + 1,
                tmTime.tm_mday,
                tmTime.tm_hour,
                tmTime.tm_min,
                tmTime.tm_sec,
                indexM,
                modelNameM.c_str(),
                tmTime.tm_year + 1900,
                tmTime.tm_mon + 1,
                tmTime.tm_mday
                );
        curFileNameM = std::string(filename);
        std::string filePath = outDirM + "/" + curFileNameM + ".txt";
        fileStreamM.open(filePath.c_str(), std::ios_base::out|std::ios_base::app);
        fileStreamM << headerLineM;
    }
}

//-----------------------------------------------------------------------------

void FileWriter::closeFile()
{
    if (fileStreamM.is_open())
    {
        fileStreamM.close();
        std::string fromFilePath = outDirM + "/" + curFileNameM + ".txt";
        std::string toFilePath = outDirM + "/" + curFileNameM + ".ss7";
        if(0 != link(fromFilePath.c_str(), toFilePath.c_str()))
        {
            LOG_ERROR("fail to rename the file:" << fromFilePath
                    << ", errno:" << errno);
        }
        else
        {
            unlink(fromFilePath.c_str());
        }
    }
}

//-----------------------------------------------------------------------------

