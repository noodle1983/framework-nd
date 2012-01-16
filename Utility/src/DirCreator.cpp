#include "DirCreator.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

using namespace Utility;

//-----------------------------------------------------------------------------
void Utility::creatDir(const std::string& thePath)
{
    if (thePath.empty())
        return;

    const char* pFind = thePath.c_str();
    while ((pFind = strchr(pFind, '/')) != 0) 
    {
        std::string preDir = thePath.substr(0, pFind - thePath.c_str());
        pFind++;
        DIR* dir = opendir(preDir.c_str());
        if (dir != NULL)
        {
            closedir(dir);
            continue;
        }
        if (errno != ENOENT)
        {
            return;
        }

        mkdir(preDir.c_str(), 0774); 
    }
    DIR* dir = opendir(thePath.c_str());
    if (dir != NULL)
    {
        closedir(dir);
        return;
    }
    if (errno != ENOENT)
    {
        return;
    }
    mkdir(thePath.c_str(), 0774); 
}

//-----------------------------------------------------------------------------

