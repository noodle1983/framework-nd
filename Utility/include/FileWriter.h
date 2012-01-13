#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <string>
#include <stdint.h>

namespace Utility
{
    class FileWriter
    {
    public:
        FileWriter(
                const std::string& theModelName, 
                const int64_t theIndex, 
                const std::string& theHeaderLine = "");
        ~FileWriter();

        void write(const std::string& theContent);

    private:
        void closeFile();
        void switchFile();

    private:
        std::string headerLineM;
        std::string modelNameM;
        unsigned indexM;
        unsigned switchTimeM;
        std::ofstream fileStreamM;
        unsigned curWriteTimeM;
        std::string curFileNameM;
        std::string outDirM;
    };

}

#endif /* FILEWRITER_H */

