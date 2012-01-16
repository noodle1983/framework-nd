#include "DirCreator.h"
#include <string>
#include <string.h>

using namespace Utility;

int main()
{
    creatDir(std::string("abc"));
    creatDir("abc/a");
    return 0;
}

