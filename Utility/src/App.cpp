#include "App.h"
#include "DirCreator.h"
#include "Log.h"

#include <unistd.h>
#include <signal.h>

using namespace Utility;

//-----------------------------------------------------------------------------

App::App()
    : crashDumpHandlerM(NULL)
{

}

//-----------------------------------------------------------------------------

App::~App()
{
    if (crashDumpHandlerM)
    {
        delete crashDumpHandlerM;
    }
}

//-----------------------------------------------------------------------------

void App::setRunInBackground()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        //failed
        printf("failed to fork!\n");
        exit(-1);
    }
    else if (pid > 0)
    {
        //main processor
        printf("server start with pid %d.\n", pid);
        exit(0);
    }
    setsid();
}

//-----------------------------------------------------------------------------

void App::setDumpWhenCrash()
{
    if (NULL == crashDumpHandlerM)
    {
        createDir("./CrashDump");
        crashDumpHandlerM = new google_breakpad::ExceptionHandler("./CrashDump", NULL, App::dumpCallback, NULL, true);
    }
}

//-----------------------------------------------------------------------------

bool App::dumpCallback(const char* dump_path,
                        const char* minidump_id,
                        void* context,
                        bool succeeded)
{
    LOG_WARN("Crash Dump: " << dump_path << "/" << minidump_id << ".dmp");
    return succeeded;
}




