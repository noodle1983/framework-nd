#include "DirCreator.h"
#include <string>
#include <string.h>

using namespace Utility;

int main()
{
    createDir(std::string("abc"));
    createDir("abc/a");
    return 0;
}

