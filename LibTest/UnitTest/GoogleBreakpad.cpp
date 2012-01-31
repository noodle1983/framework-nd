#include "App.h"

using namespace Utility;

void crash()
{
  volatile int* a = (int*)(NULL);
  *a = 1;
}


int main(int argc, char* argv[])
{
    AppSingleton::instance()->setDumpWhenCrash();
    crash();
    return 0;
}
