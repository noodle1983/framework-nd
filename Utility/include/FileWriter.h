#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "BoostProcessor.h"
#include <fstream>
#include <string>
#include <stdint.h>
#include <unistd.h>

struct event;

namespace Utility
{
    class FileWriter
    {
    public:
        FileWriter(
                Processor::BoostProcessor* theProcessor,
                const std::string& theModelName, 
                const int64_t theIndex, 
                const std::string& theHeaderLine = "");
        FileWriter(
                const std::string& theModelName, 
                const int64_t theIndex, 
                const std::string& theHeaderLine = "");
        ~FileWriter();

        void write(const std::string& theContent, const time_t theTime);

        void _write(const std::string* theContent, const time_t theTime);
    private:
        void startTimer(const time_t &theTime);
        static void onTimeout(int theFd, short theEvt, void *theArg);
        void closeFile();
        void switchFile(const time_t &theTime);


    private:
        std::string headerLineM;
        std::string modelNameM;
        unsigned indexM;
        int64_t switchTimeM;
        std::ofstream fileStreamM;
        unsigned curWriteTimeM;
        unsigned curTimerTimeM;
        std::string curFileNameM;
        std::string outDirM;

        Processor::BoostProcessor* processorM;
        struct event* timerHandlerM;

    };

}

#endif /* FILEWRITER_H */

