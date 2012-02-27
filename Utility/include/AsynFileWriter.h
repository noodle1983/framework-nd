#ifndef ASYNFILEWRITER_H
#define ASYNFILEWRITER_H

#include "BoostProcessor.h"
#include "FileWriter.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace Utility
{
    class AsynFileWriter
    {
    public:
        AsynFileWriter(
                const std::string& theModelName, 
                const std::string& theHeaderLine = "");
        ~AsynFileWriter();

        void write(const uint64_t theId, const std::string& theContent, const time_t theTime);
        void _write(
                Utility::FileWriter* theFileWriter, 
                const std::string* theContent, 
                const time_t theTime);

    private:
        unsigned ioThreadCntM;
        std::vector<Utility::FileWriter*> fileWriterVectorM;
    };

}

#endif /* ASYNFILEWRITER_H */

