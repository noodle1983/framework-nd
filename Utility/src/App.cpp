#include "App.h"
#include "DirCreator.h"
#include "Log.h"

#include <unistd.h>
#include <signal.h>

using namespace Utility;
int App::closedM = false;
boost::mutex App::closedMutexM;
boost::condition_variable App::closedCondM;

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

void App::wait()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, App::sigStop);
    signal(SIGINT, App::sigStop);

    boost::unique_lock<boost::mutex> lock(closedMutexM);
    while(!closedM)
    {
        closedCondM.wait(lock);
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

//-----------------------------------------------------------------------------

void App::sigStop(int sig)
{
    LOG_DEBUG("receive signal " << sig << ". stopping...");
    boost::lock_guard<boost::mutex> lock(closedMutexM);
    closedM = true;
    closedCondM.notify_one();
}



